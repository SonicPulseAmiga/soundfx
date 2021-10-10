/****h* SoundFX/SFX-BootWin.c [4.2] *
*
*  NAME
*    SFX-BootWin.c
*  COPYRIGHT
*    $VER: SFX-BootWin.c 4.2 (29.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    bootprogress-window for SFX - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    12.Nov.1997
*  MODIFICATION HISTORY
*    29.May.2002	V 4.2	most recent version
*    03.Nov.2000	V 4.1	most recent version
*    22.Feb.2000	V 4.00	most recent version
*    10.Aug.1998	V 3.70	most recent version
*    12.Nov.1997	V 3.60	initial version
*  NOTES
*
*******
*/

#define SFX_BootWin_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

//-- bootwin functions

void OpenGFX(void);
void CloseGFX(void);
void UpdateGFX(char *text,UBYTE proz);

//-- private

void DisplayGFX(char *fn);

//-- globals ------------------------------------------------------------------

struct Window		*stwin;
struct ViewPort		*stvp;
struct ColorMap 	*stcm;
struct RastPort 	*strp;
WORD paltrans[16]={-1};
UWORD fonty,yog,yot;
UWORD yots,yote;
UWORD yogs,yoge;

struct TagItem stwintags[]={
	WA_Left,				0,
	WA_Top,					0,
	WA_InnerWidth,			320,
	WA_InnerHeight,			201,		// we overwrite that later on
	WA_IDCMP,				0,
	WA_Flags,				WFLG_SMART_REFRESH|WFLG_RMBTRAP|WFLG_BORDERLESS,
	WA_Title,				NULL,
	WA_ScreenTitle,			(ULONG)VERS,
	WA_PubScreenName,		(ULONG)"Workbench",
	WA_PubScreenFallBack,	TRUE,
	TAG_DONE
};

//-- definitions --------------------------------------------------------------

//-- bootwin functions

void OpenGFX(void)
{
	struct Screen	*scr;
	struct TextAttr *sysfont;
	struct TextFont *SysFont;
	ULONG h;

	if(!(scr=LockPubScreen(NULL))) return;
	sysfont=scr->Font;
	SysFont=OpenDiskFont(sysfont);
	fonty=max(11,(sysfont->ta_YSize+4));
	stvp=&scr->ViewPort;
	stcm=stvp->ColorMap;

	h=201+(fonty<<1)+11;
	stwintags[0].ti_Data=(scr->Width>>1)-163;;
	stwintags[1].ti_Data=(scr->Height>>1)-(110+(scr->Height>>3));
	stwintags[3].ti_Data=h+1;
	if(!(stwin=OpenWindowTagList(NULL,stwintags))) return;
	strp=stwin->RPort;SetFont(strp,SysFont);

	DisplayGFX("progdir:Data/SFX-Titel.rawbrush");

	SetAPen(strp,2);Move(strp,  0,h);Draw(strp,0,201);Draw(strp,319,201);			// big 3D-Box
	SetAPen(strp,1);Draw(strp,319,h);Draw(strp,0,h);

	yog=205;yogs=yog+1;yoge=yog+(fonty-1);
	SetAPen(strp,1);Move(strp,  8,yog+fonty);Draw(strp,8,yog);Draw(strp,311,yog);	// small 3D-Box No.1
	SetAPen(strp,2);Draw(strp,311,yog+fonty);Draw(strp,8,yog+fonty);
	yot=208+fonty;yots=yot+1;yote=yot+(fonty-1);
	SetAPen(strp,1);Move(strp,  8,yot+fonty);Draw(strp,8,yot);Draw(strp,311,yot);	// small 3D-Box No.2
	SetAPen(strp,2);Draw(strp,311,yot+fonty);Draw(strp,8,yot+fonty);

	UnlockPubScreen(NULL,scr);
}

void CloseGFX(void)
{
	register UBYTE h;
	
	if(stwin) {
		for(h=0;h<16;h++) ReleasePen(stcm,(ULONG)paltrans[h]);
		CloseWindow(stwin);stwin=NULL;
	}
}

void UpdateGFX(char *text,UBYTE proz)
{
	char pstr[5];
	LONG sl;

	MSG2("%03d%% : %s",proz,text);

	if(stwin) {
		if(text) {
			SetAPen(strp,0);RectFill(strp,9,yots,310,yote);
			sl=strlen(text);
			while(sl>0 && TextLength(strp,text,sl)>297) sl--;
			SetAPen(strp,1);Move(strp,12,yot+(fonty-5));Text(strp,text,sl);
		}
		SetAPen(strp,0);RectFill(strp,9+(proz*3),yogs,310,yoge);
		SetAPen(strp,3);RectFill(strp,9,yogs,9+(proz*3),yoge);
		pstr[2]=' ';pstr[3]='%';pstr[4]='\0';
		if(proz>9) pstr[0]=48+(proz/10);
		else pstr[0]=' ';
		pstr[1]=48+(proz%10);
		SetDrMd(strp,JAM1);
		SetAPen(strp,2);Move(strp,12,yog+(fonty-5));Text(strp,pstr,4);
		SetDrMd(strp,JAM2);
	}
}

//-- private

void DisplayGFX(char *fn)
{
	register UBYTE h;
	register ULONG i,j;
	UBYTE *bmap;
	ULONG size=320*201;
	UBYTE done=FALSE;
	struct RastPort trp;
	struct BitMap *tbm;
	ULONG col32[6];
	ULONG r,g,b;
	BYTE ro,go,bo,m;

	if((bmap=AllocVec(size,MEMF_ANY))) {
		int fh;
		if(fh=Open(fn,MODE_OLDFILE)) {
			if(Read(fh,bmap,size)==size) done=TRUE;
			else printf("Can't read splash screen logo data\n");
			Close(fh);
		}
//		FILE *infile;
//		if(infile=fopen(fn,"rb")) {
//			if(fread(bmap,size,1,infile)) done=TRUE;
//			else printf("Can't read splash screen logo data\n");
//			fclose(infile);
//		}
		else printf("Can't open splash screen logo file\n");
	}
	else printf("Can't alloc mem\n");
	if(done) {
		GetRGB32(stcm,0,1,col32);
		j=((col32[0]&0xFF)+(col32[1]&0xFF)+(col32[2]&0xFF))/3;		// avg col;
		m=ro=(col32[0]&0xFF)-j;										// tint like wb-color 0
		go=(col32[1]&0xFF)-j;if(go<m) m=go;
		bo=(col32[2]&0xFF)-j;if(bo<m) m=bo;
		ro-=m;go-=m;bo-=m;
		for(h=0;h<16;h++) {
			j=(h<<4)+ro;if(j>255) j=255;r=MakeCol32(j);
			j=(h<<4)+go;if(j>255) j=255;g=MakeCol32(j);
			j=(h<<4)+bo;if(j>255) j=255;b=MakeCol32(j);
			paltrans[h]=ObtainBestPen(stcm,r,g,b,OBP_Precision,PRECISION_EXACT);
		}
		for(i=0;i<size;i++) bmap[i]=paltrans[bmap[i]>>4];
		if(tbm=AllocBitMap(1,201,8,BMF_CLEAR,NULL)) {	      /* Initialize the temporary bitmap */
		    InitRastPort(&trp);
		    trp.BitMap=tbm;
			WritePixelArray8(strp,0,0,319,200,bmap,&trp);
			FreeBitMap(tbm);
			FreeVec(bmap);
		}
	}
}

//-- eof ----------------------------------------------------------------------
