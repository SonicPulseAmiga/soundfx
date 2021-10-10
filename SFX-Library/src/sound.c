/****h* sfxsupport.library/sound.c [0.0] *
*
*  NAME
*    sound.c
*  COPYRIGHT
*    $VER: sound.c 0.0 (17.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    sound project tools - definitions
*  AUTHOR
*    Stefan Kost            sk      kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    17.Apr.2004    V 0.0   most recent version
*    28.Jan.2004	V 4.3	most recent version
*    07.Aug.2002    V 4.2   most recent version
*    11.Jan.2001    V 4.1   most recent version
*    16.Jul.2000    V 1.3   most recent version
*    27.Apr.1999    V 1.2   most recent version
*    19.Jul.1998    V 1.0   initial version
*  NOTES
*
*******
*/

#define SOUND_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

#include <images/titlebar.h>
#include <clib/titlebarimage_protos.h>
#include <pragmas/titlebarimage_pragmas.h>

//-- prototypes ---------------------------------------------------------------

//-- BufferTools

void		ASM LIB_EXPORT(BufferFill(REG(a0,SInfo *srcbuf),REG(a1,SInfo *dstbuf)));
SInfo *		ASM LIB_EXPORT(NewSample(void));

void		ASM LIB_EXPORT(AddSample(REG(a0,SInfo *si),REG(a1,STRPTR name)));
void		ASM LIB_EXPORT(RemSample(REG(a0,SInfo *si)));

void		ASM LIB_EXPORT(SetSlider(REG(a0,SInfo *si),REG(d0,UBYTE mode)));
void		ASM LIB_EXPORT(SetTitle(REG(a0,SInfo *si)));
void		ASM LIB_EXPORT(SetName(REG(a0,SInfo *si),REG(a1,STRPTR name)));
STRPTR 		ASM LIB_EXPORT(JoinSampleNames(REG(a0,STRPTR name1),REG(a1,STRPTR name2)));

void		ASM LIB_EXPORT(ShowSample(void));
void		ASM LIB_EXPORT(HideSample(void));

void		ASM LIB_EXPORT(SetOptWinBounds(REG(a0,UWORD *x),REG(a1,UWORD *y),REG(d0,UWORD w),REG(d1,UWORD h)));
UBYTE		WinHit(UWORD *windata,UWORD *xc,UWORD *yc,UWORD winanz,UWORD x,UWORD y);

SInfo *		ASM LIB_EXPORT(LockBuffer(REG(a0,SInfo *si)));
void		ASM LIB_EXPORT(UnlockBuffer(REG(a0,SInfo *si)));

void		ASM LIB_EXPORT(RecalcSampleLines(REG(a0,SInfo *si)));

//-- helper for scroller windows

UBYTE		ASM LIB_EXPORT(InitScrollerWinStuff(void));
void		ASM LIB_EXPORT(FreeScrollerWinStuff(void));

//-- private stuff

ULONG		GetNextFreeID(void);
void		BuildBufferArray(void);

UBYTE		InitScroller(SInfo *si);
void		FreeScroller(SInfo *si);

int			SysISize(void);

Object *	NewImageObject(ULONG which);
Object *	NewPropObject(ULONG freedom, Tag tag1, ...);
Object *	NewButtonObject(Object *image, Tag tag1, ...);

//-- defines ------------------------------------------------------------------

#ifndef IM
#define IM(o) ((struct Image *) o)
#endif

#ifndef GAD
#define GAD(o) ((struct Gadget *) o)
#endif

#ifndef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif
#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

#define RecalcHVisible(window) (window->GZZWidth)
#define RecalcVVisible(window) (window->GZZHeight)

//-- global variables ---------------------------------------------------------

WORD ZoomSizeSW[]={ 0,20,110,32 };
struct TagItem sawintags[]= {
/*  0 */    WA_Left,            0,
/*  1 */    WA_Top,             110,
/*  2 */    WA_InnerWidth,		320,
/*  3 */    WA_InnerHeight,     64,
/*  4 */    WA_IDCMP,           0l,
/*  5 */    WA_Flags,           WFLG_SMART_REFRESH|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_SIZEGADGET|WFLG_DRAGBAR|WFLG_REPORTMOUSE|WFLG_NEWLOOKMENUS,
/*  6 */    WA_Gadgets,         0l,
/*  7 */    WA_Title,           0l,
/*  8 */    WA_ScreenTitle,     0l,
/*  9 */    WA_PubScreenName,   (ULONG)SFX_PUBSCRNAME,
/* 10 */    WA_MinWidth,        110,
/* 11 */    WA_MinHeight,       32,
/* 12 */    WA_MaxWidth,        0,
/* 13 */    WA_MaxHeight,       0,
/* 14 */    WA_BlockPen,        1,
/* 15 */    WA_Zoom,            (ULONG)ZoomSizeSW,
/* 16 */    WA_AutoAdjust,      TRUE,
/* 17 */    WA_RptQueue,        2,
/* 18 */    WA_BackFill,        (ULONG)LAYERS_NOBACKFILL,
/* 19 */    TAG_DONE
};

struct Library *TitlebarImageBase=NULL;
struct DrawInfo *dri=NULL;
Object          *sizeimage=NULL;
Object          *leftimage=NULL;
Object          *rightimage=NULL;
Object          *upimage=NULL;
Object          *downimage=NULL;
Object 			*iconifyimage=NULL;
Object 			*popupimage=NULL;

//-- definitions --------------------------------------------------------------

//-- BufferTools

void SAVEDS ASM LIB_EXPORT(BufferFill(REG(a0,SInfo *srcbuf),REG(a1,SInfo *dstbuf))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(srcbuf,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(dstbuf,__FUNC__,"2.par",4));

	/** @todo Felder sortieren und dann memcpy */
	dstbuf->slen        =srcbuf->slen;
	dstbuf->srat        =srcbuf->srat;
	dstbuf->channels    =srcbuf->channels;
	dstbuf->loop        =srcbuf->loop;

	dstbuf->loopl       =srcbuf->loopl;
	dstbuf->loops       =srcbuf->loops;
	dstbuf->markxs      =srcbuf->markxs;
	dstbuf->markxl      =srcbuf->markxl;
	dstbuf->markys      =srcbuf->markys;
	dstbuf->markyl      =srcbuf->markyl;

	dstbuf->zoomxs      =srcbuf->zoomxs;
	dstbuf->zoomxl      =srcbuf->zoomxl;
	dstbuf->zoomys      =srcbuf->zoomys;
	dstbuf->zoomyl      =srcbuf->zoomyl;
	dstbuf->rasterx     =srcbuf->rasterx;
	dstbuf->rastery     =srcbuf->rastery;
	dstbuf->maxlines    =srcbuf->maxlines;
	dstbuf->rmslines    =srcbuf->rmslines;
	dstbuf->avglines    =srcbuf->avglines;
	dstbuf->drawmode    =srcbuf->drawmode;
	dstbuf->quickdraw   =srcbuf->quickdraw;

	dstbuf->safemode    =srcbuf->safemode;
	dstbuf->aktch       =srcbuf->aktch;
	dstbuf->channelmask =srcbuf->channelmask;
	dstbuf->mark        =srcbuf->mark;
	dstbuf->scalax      =srcbuf->scalax;
	dstbuf->scalay      =srcbuf->scalay;
	dstbuf->unitx       =srcbuf->unitx;
	dstbuf->unity       =srcbuf->unity;

	if(srcbuf->extdata) {
		if((dstbuf->extdata=AllocVec(srcbuf->extlength,MEMF_ANY))) {
			strcpy(dstbuf->extid,srcbuf->extid);
			memcpy(dstbuf->extdata,srcbuf->extdata,srcbuf->extlength);
			dstbuf->extlength=srcbuf->extlength;
		}
	}
}

SInfo * SAVEDS ASM LIB_EXPORT(NewSample(void)) {
	SInfo * si;
	time_t t;
	struct tm *p;
	char tmpbuf[300];

	if(si=(SInfo *)AllocVec(sizeof(SInfo),MEMF_ANY|MEMF_CLEAR)) {
		//-- we allocate the id in add Sample now, to avoid that several samples have the same id
		//si->id=GetNextFreeID();
		si->srat=16780;
		si->visibillity=SFX_SOUND_VISIBILLITY_HIDDEN;
		//si->channels=0;     // means mono=1
		si->aktch=1;
		si->channelmask=1;
		si->loop=SFXLOOP_NONE;
		//si->loops=0L;
		//si->loopl=0L;
		//si->mark=0;
		//si->markxs=0L;si->markxl=0L;
		//si->markys=0L;si->markyl=0L;
		//si->zoomxs=0L;si->zoomxl=0L;
		//si->zoomys=0L;
		si->zoomyl=65535;
		if(GetRunTime(sfxprefs_sbuf)) {
			si->drawmode=GetRunTime(sfxprefs_sbuf)->drawmode;
			si->rasterx=GetRunTime(sfxprefs_sbuf)->rasterx;
			si->rastery=GetRunTime(sfxprefs_sbuf)->rastery;
			si->maxlines=GetRunTime(sfxprefs_sbuf)->maxlines;
			si->rmslines=GetRunTime(sfxprefs_sbuf)->rmslines;
			si->avglines=GetRunTime(sfxprefs_sbuf)->avglines;
			si->scalax=GetRunTime(sfxprefs_sbuf)->scalax;
			si->scalay=GetRunTime(sfxprefs_sbuf)->scalay;
			si->unitx=GetRunTime(sfxprefs_sbuf)->unitx;
			si->unity=GetRunTime(sfxprefs_sbuf)->unity;
			si->quickdraw=GetRunTime(sfxprefs_sbuf)->quickdraw;
			si->safemode=GetRunTime(sfxprefs_sbuf)->safemode;
			time(&t);
			p=localtime(&t);
			strcpy(tmpbuf,GetRunTime(sfxprefs_sbuf)->copystr);
			strftime(&tmpbuf[strlen(tmpbuf)],299-strlen(tmpbuf)," %Y (%d.%m.%y)",p);
			si->cpyr=LIB_INTCALL(StringCopy(tmpbuf));
			si->auth=LIB_INTCALL(StringCopy(GetRunTime(sfxprefs_sbuf)->authstr));
			strftime(tmpbuf,255,"Processed with SoundFX (C) by Stefan Kost 1993-%Y",p);
			si->anno=LIB_INTCALL(StringCopy(tmpbuf));
		}
		((struct Node *)si)->ln_Name=LIB_INTCALL(StringCopy("Buffer"));
		//((struct Node *)si)->ln_Succ=0l;
		//((struct Node *)si)->ln_Pred=0l;
		si->storage=VM_NODATA;
	}
	return(si);
}

void SAVEDS ASM LIB_EXPORT(AddSample(REG(a0,SInfo *si),REG(a1,STRPTR name))) {
	register UBYTE i;
	UBYTE fail=FALSE;
	UWORD xp,yp,wi=0,he=0;
	static char sfxtitle[256];

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(name,__FUNC__,"2.par",4));

	if(si) {
		si->id=GetNextFreeID();
		if(si->loop && si->loops+si->loopl>=si->slen) {         // sometimes loop is saved wrong, do a correction here
			si->loopl=si->slen-si->loops;
		}
		i=0;
		if(si->channelmask&CH_LEFT)  si->chnr[i++]=0;
		if(si->channelmask&CH_RIGHT) si->chnr[i++]=1;
		if(si->channelmask&CH_FRONT) si->chnr[i++]=2;
		if(si->channelmask&CH_BACK)  si->chnr[i++]=3;
		si->aktch=i;
		MSG3("  channelparameters : channels %1d aktch %1d channelmask 0x%x",si->channels,si->aktch,si->channelmask);

		// initially add node to list, even though SetName() will re-insert the node,
		// because SetName() will always remove it too
		AddHead(GetRunTime(blist),(struct Node *)si);GetRunTime(aktsamples)++;
		LIB_INTCALL(SetName(si,name));
		MSG2("  add sample is called 0x%08ld::\"%s\"",((struct Node *)si)->ln_Name,((struct Node *)si)->ln_Name);

		//-- this is done in SetName() too
//      BuildBufferArray();
		if(GetRunTime(quiet)) {
			MSG("  new sample as hidden");
			si->visibillity=SFX_SOUND_VISIBILLITY_HIDDEN;
			si->win =NULL;
			si->rp  =NULL;
			si->xpos=0;
			si->ypos=0;
			si->xo  =0;
			si->yo  =0;
			si->xres=0;
			si->yres=0;
		}
		else {
			MSG("  add sample as shown");
			if(GetRunTime(sfxprefs_sbuf)->nolines &&                                   /* nolines is enabled AND */
				((si->slen*CH_COUNT(si->channels))>GetRunTime(sfxprefs_sbuf)->thrsize)) {    /* the size is above the thresh_size */
				si->maxlines=si->rmslines=si->avglines=FALSE;
			}
			if(GetRunTime(onewindow)) {
				sawintags[ 0].ti_Data=GetRunTime(clientrect).LeftEdge;
				sawintags[ 1].ti_Data=GetRunTime(clientrect).TopEdge;
				sawintags[ 2].ti_Tag =WA_Width;
				sawintags[ 2].ti_Data=GetRunTime(clientrect).Width;
				sawintags[ 3].ti_Tag =WA_Height;
				sawintags[ 3].ti_Data=GetRunTime(clientrect).Height;
				sawintags[ 5].ti_Data=WFLG_SMART_REFRESH|WFLG_CLOSEGADGET|WFLG_REPORTMOUSE|WFLG_NEWLOOKMENUS|WFLG_BACKDROP;
				sawintags[15].ti_Tag=TAG_IGNORE;
			}
			else {
				switch(GetRunTime(sfxprefs_sbuf)->mode) {
					case SIZEMODE_ABSOLUTE:
						wi=GetRunTime(sfxprefs_sbuf)->xs1;
						he=GetRunTime(sfxprefs_sbuf)->ys1;
						ZoomSizeSW[2]=GetRunTime(sfxprefs_sbuf)->xs2;
						ZoomSizeSW[3]=GetRunTime(sfxprefs_sbuf)->ys2;
						break;
					case SIZEMODE_RELATIVE:
						wi=(UWORD)(((GetRunTime(sfxprefs_sbuf)->xs1*GetRunTime(clientrect).Width)/1000.0)+0.5);
						he=(UWORD)(((GetRunTime(sfxprefs_sbuf)->ys1*GetRunTime(clientrect).Height)/1000.0)+0.5);
						ZoomSizeSW[2]=(UWORD)(((GetRunTime(sfxprefs_sbuf)->xs2*GetRunTime(clientrect).Width)/1000.0)+0.5);
						ZoomSizeSW[3]=(UWORD)(((GetRunTime(sfxprefs_sbuf)->ys2*GetRunTime(clientrect).Height)/1000.0)+0.5);
						break;
					UNEXPECTED_DEFAULT
				}
				LIB_INTCALL(SetOptWinBounds(&xp,&yp,wi,he));
				sawintags[ 0].ti_Data=xp;
				sawintags[ 1].ti_Data=yp;
				sawintags[ 2].ti_Tag =WA_InnerWidth;
				sawintags[ 2].ti_Data=wi;
				sawintags[ 3].ti_Tag =WA_InnerHeight;
				sawintags[ 3].ti_Data=he;
				sawintags[ 5].ti_Data=WFLG_SMART_REFRESH|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_SIZEGADGET|WFLG_DRAGBAR|WFLG_REPORTMOUSE|WFLG_NEWLOOKMENUS;
				sawintags[15].ti_Tag=WA_Zoom;
				ZoomSizeSW[0]=xp;
				ZoomSizeSW[1]=yp;
			}
			LIB_INTCALL(MemUsage(sfxtitle));
			sawintags[ 6].ti_Data=NULL;
			sawintags[ 7].ti_Data=(ULONG)si->title;
			sawintags[ 8].ti_Data=(ULONG)sfxtitle;
			sawintags[10].ti_Data=MAX(sawintags[10].ti_Data,GetRunTime(scx[100])+ 6);                 /* Samplewindow anpassen */
			sawintags[11].ti_Data=MAX(sawintags[11].ti_Data,GetRunTime(scy[) 14]+20);
			sawintags[12].ti_Data=GetRunTime(clientrect).Width;
			sawintags[13].ti_Data=GetRunTime(clientrect).Height;
			if(GetRunTime(sfxprefs_misc)->contextbutton==1) sawintags[5].ti_Data|=WFLG_RMBTRAP;
			else sawintags[5].ti_Data&=(~WFLG_RMBTRAP);
			if(GetRunTime(sfxpubscr)==IntuitionBase->FirstScreen) sawintags[5].ti_Data|=WFLG_ACTIVATE;
			else sawintags[5].ti_Data&=(~WFLG_ACTIVATE);
			if(InitScroller(si)) {
				if(si->win=LIB_INTCALL(OpenMWindow(sawintags,GetRunTime(swin_up),&GetRunTime(swin_upct),swin_idcmps))) {
					struct Gadget *gad;
					//-- this is neccesary so that the system recognizes the titlebar-gadget
					if((gad=(struct Gadget *)(si->scrobj[GADID_SWB_ICONIFY]))) {
						RemoveGadget(si->win,gad);
						AddGadget(si->win,gad,0);
					}
					if((gad=(struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))) {
						RemoveGadget(si->win,gad);
						AddGadget(si->win,gad,0);
					}
					// seems not to be required
					//RefreshGadgets(si->win->FirstGadget,si->win,NULL);
					si->visibillity=SFX_SOUND_VISIBILLITY_SHOWN;
					si->rp  =si->win->RPort;
					si->xpos=si->win->LeftEdge;
					si->ypos=si->win->TopEdge;
					//si->xo  =si->win->BorderLeft;
					//si->yo  =si->win->BorderTop;
					//si->xres=si->win->Width -(si->win->BorderLeft+si->win->BorderRight);
					//si->yres=si->win->Height-(si->win->BorderTop+si->win->BorderBottom);
					si->xo=si->yo=si->xres=si->yres=0;
					SetFont(si->rp,GetRunTime(scrfont));
					LIB_INTCALL(SetSlider(si,SLIDER_BOTH));
					LIB_INTCALL(SetTitle(si));
					LIB_INTCALL(RecalcSampleLines(si));
					SizeWindow(si->win,0,0);
				}
				else fail=TRUE;
			}
			else fail=TRUE;
		}
		MSG("add done");
		if(fail) LIB_INTCALL(RemSample(si));
		else {
			//-- write recovery data
			if(si->storage==VM_DRIVE) SaveSInfo(si);

			GetRunTime(aktbuf)=si;
			// do only wakeup sfx, if we are not in quiet mode
			if(!GetRunTime(quiet)) {
				MSG("  wake up sfx");
				GetRunTime(changesamples)=TRUE;                   // notify sfx, that a sample has been added
				if(FindTask(NULL)!=GetRunTime(sfxproc)) {      // we have two cases, the sample has been added by a foreign process or by ourself
					Signal(GetRunTime(sfxproc),0xFFFFFFFF);        // send a full wakeup to the main process
				}
			}
		}
	}
	else MSG("AddSample called with si=0L");
	OUTRO;
}

void SAVEDS ASM LIB_EXPORT(RemSample(REG(a0,SInfo *si))) {
	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

	if(si) {
		//MSG1("  sample exists : id=%ld",si->id);
		//-- unlink if neccesary
		if(si->id && (((struct Node *)si)->ln_Succ || ((struct Node *)si)->ln_Pred)) {
			if((struct Node *)si==GetRunTime(blist)->lh_Head) RemHead(GetRunTime(blist));
			else if((struct Node *)si==GetRunTime(blist)->lh_Tail) RemTail(GetRunTime(blist));
			else Remove((struct Node *)si);
			MSG("  node removed");

			GetRunTime(aktsamples)--;
			BuildBufferArray();

			if(GetRunTime(aktsamples)>0) GetRunTime(aktbuf)=(SInfo *)(GetRunTime(blist)->lh_Head);
			else GetRunTime(aktbuf)=NULL;
			//MSG1("new aktbuf : 0x%08lx",GetRunTime(aktbuf));

			if(SFX_SOUND_IS_SHOWN(si) && si->win) {
				FreeScroller(si);
				LIB_INTCALL(CloseMWindow(si->win,&GetRunTime(swin_upct)));
				MSG("  window closed");
			}

			GetRunTime(changesamples)=TRUE;       // notify sfx, that a (linked) sample has been removed
		}
		//-- remove recovery data
		if(si->storage==VM_DRIVE) RemoveSInfo(si);

		LIB_INTCALL(FreeSampleMem(si));
		MSG("  sample memory freed");

		if(si->extdata)                     FreeVec(si->extdata);
		MSG("    infodata memory freed");

		//if(si->cpyr)                        FreeVec(si->cpyr);
		if(si->cpyr) {
			MSG3("   cpyr : %08lx : \"%s\" : %ld",si->cpyr,si->cpyr,strlen(si->cpyr)+1);
			FreeVec(si->cpyr);
		}
		//if(si->auth)                        FreeVec(si->auth);
		if(si->auth) {
			MSG3("   auth : %08lx : \"%s\" : %ld",si->auth,si->auth,strlen(si->auth)+1);
			FreeVec(si->auth);
		}
		//if(si->anno)                        FreeVec(si->anno);
		if(si->anno) {
			MSG3("   anno : %08lx : \"%s\" : %ld",si->anno,si->anno,strlen(si->anno)+1);
			FreeVec(si->anno);
		}
		//if(si->title)                       FreeVec(si->title);
		if(si->title) {
			MSG3("  title : %08lx : \"%s\" : %ld",si->title,si->title,strlen(si->title)+1);
			FreeVec(si->title);
		}
		if(((struct Node *)si)->ln_Name){
			MSG3("   name : %08lx : \"%s\" : %ld",((struct Node *)si)->ln_Name,((struct Node *)si)->ln_Name,strlen(((struct Node *)si)->ln_Name)+1);
			FreeVec(((struct Node *)si)->ln_Name);
		}
		MSG("    string memory freed");

		FreeVec(si);
		MSG("  memory freed");
	}
	else MSG("RemSample called with si=0l !");

	OUTRO;
}

void SAVEDS ASM LIB_EXPORT(SetSlider(REG(a0,SInfo *si),REG(d0,UBYTE mode))) {
	ULONG z;
	register UBYTE sh;
	struct PropInfo *pi;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

	if(SFX_SOUND_IS_SHOWN(si) && si->win) {
		if(mode&SLIDER_X) {
			sh=0;
			while(si->slen>>sh>65535L) sh++;
			z=(si->zoomxl>>sh);z=max(z,1);
			SetAttrs(si->scrobj[GADID_SWB_HORIZ],PGA_Top,(si->zoomxs>>sh),PGA_Total,(si->slen>>sh),PGA_Visible,z,TAG_DONE);
			pi=(struct PropInfo *)((struct Gadget *)si->scrobj[GADID_SWB_HORIZ])->SpecialInfo;
			NewModifyProp((struct Gadget *)si->scrobj[GADID_SWB_HORIZ],si->win,NULL,pi->Flags,pi->HorizPot,pi->VertPot,pi->HorizBody,pi->VertBody,1);
		}
		if(mode&SLIDER_Y) {
			SetAttrs(si->scrobj[GADID_SWB_VERT],PGA_Top,65536-(si->zoomys+si->zoomyl),PGA_Total,65535,PGA_Visible,si->zoomyl,TAG_DONE);
			pi=(struct PropInfo *)((struct Gadget *)si->scrobj[GADID_SWB_VERT])->SpecialInfo;
			NewModifyProp((struct Gadget *)si->scrobj[GADID_SWB_VERT],si->win,NULL,pi->Flags,pi->HorizPot,pi->VertPot,pi->HorizBody,pi->VertBody,1);
		}
	}
}

void SAVEDS ASM LIB_EXPORT(SetTitle(REG(a0,SInfo *si))) {
	char tmpname[256];
	char stor[4]={'-','M','D','S'};

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

	if(SFX_SOUND_IS_SHOWN(si) && si->win) {                // do this only if there's a window open
		if(si->title) FreeVec(si->title);

		sprintf(tmpname,"%s:%1d:%c:%ld:%ld",((struct Node *)si)->ln_Name,CH_COUNT(si->channels),stor[si->storage],si->srat,si->slen);
		if(si->title=LIB_INTCALL(StringCopy(tmpname))) {
			MSG2("  new title is  0x%08ld::\"%s\"",si->title,si->title);
			SetWindowTitles(si->win,si->title,(UBYTE *)-1);
		}
	}
	//-- write recovery data
	if(si->storage==VM_DRIVE) SaveSInfo(si);

	OUTRO;
}

void SAVEDS ASM LIB_EXPORT(SetName(REG(a0,SInfo *si),REG(a1,STRPTR name))) {
	char name1[75],name2[80];
	UBYTE unique=0,ct,sl;
	SInfo *si2;

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(name,__FUNC__,"2.par",4));

	Remove((struct Node *)si);
	if(((struct Node *)si)->ln_Name) {
		FreeVec((((struct Node *)si)->ln_Name));
		((struct Node *)si)->ln_Name=0L;
	}
	if(name) {
		unique=1;
		si2=(SInfo *)(GetRunTime(blist)->lh_Head);
		while(si2 && ((struct Node *)si2)->ln_Name && ((struct Node *)si2)->ln_Succ) {
			if(!stricmp(name,((struct Node *)si2)->ln_Name)) { unique=0;si2=0; }
			else si2=(SInfo *)(((struct Node *)si2)->ln_Succ);
		}
	}
	if(!unique) {       // Name existiert bereits oder war leer
		if(name) {      // name existiert
			strncpy(name1,name,74);name1[74]='\0';
			sl=strlen(name1);
			if((sl>2) && isdigit(name1[sl-1]) && isdigit(name1[sl-2])) name1[sl-2]='\0';        // cut number
		}
		else strcpy(name1,"Buffer");           // name war leer
		ct=0;
		while(!unique) {
			sprintf(name2,"%s%02d",name1,ct);
			unique=1;
			si2=(SInfo *)(GetRunTime(blist)->lh_Head);
			while(si2 && ((struct Node *)si2)->ln_Name && ((struct Node *)si2)->ln_Succ) {
				if(!stricmp(name2,((struct Node *)si2)->ln_Name)) { unique=0;si2=0; }
				else si2=(SInfo *)(((struct Node *)si2)->ln_Succ);
			}
			ct++;
		}
		((struct Node *)si)->ln_Name=LIB_INTCALL(StringCopy(name2));
	}
	else ((struct Node *)si)->ln_Name=LIB_INTCALL(StringCopy(name));
	InsertByNameAsc(GetRunTime(blist),si);
	BuildBufferArray();

	OUTRO;
}

/** @todo: add parameter for samplejoin char, e.g. '+' or '-' (needed for Subtract) */

STRPTR  SAVEDS ASM LIB_EXPORT(JoinSampleNames(REG(a0,STRPTR name1),REG(a1,STRPTR name2))) {
	static SampleName name;
	STRPTR n1=name1,n2=name2;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(name1,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(name2,__FUNC__,"2.par",4));

	// search common prefix
	while((*n1) && (*n2) && (*n1==*n2)) { n1++;n2++; }

	if(n1>name1) {						// add only what differs
		if(*n2) sprintf(name,"%s+%s",name1,n2);
		else if(*n1) sprintf(name,"%s+%s",name2,n1);
		else sprintf(name,"%s",name1);
	}
	else {								// add both complete strings
		sprintf(name,"%s+%s",name1,name2);
	}
	return(name);
}

void SAVEDS ASM LIB_EXPORT(ShowSample(void)) {
	SInfo *si=GetRunTime(aktbuf);
	static char sfxtitle[256];

	if(GetRunTime(onewindow)) {
		sawintags[ 0].ti_Data=GetRunTime(clientrect).LeftEdge;
		sawintags[ 1].ti_Data=GetRunTime(clientrect).TopEdge;
		sawintags[ 2].ti_Tag =WA_Width;
		sawintags[ 2].ti_Data=GetRunTime(clientrect).Width;
		sawintags[ 3].ti_Tag =WA_Height;
		sawintags[ 3].ti_Data=GetRunTime(clientrect).Height;
		sawintags[ 5].ti_Data=WFLG_SMART_REFRESH|WFLG_CLOSEGADGET|WFLG_REPORTMOUSE|WFLG_NEWLOOKMENUS|WFLG_BACKDROP;
		sawintags[15].ti_Tag=TAG_IGNORE;
	}
	else {
		sawintags[ 2].ti_Tag =WA_InnerWidth;
		sawintags[ 3].ti_Tag =WA_InnerHeight;
		if(si->xres && si->yres) {
			sawintags[ 0].ti_Data=si->xpos;                      // just fill in attributes which are different from window to window
			sawintags[ 1].ti_Data=si->ypos;
			sawintags[ 2].ti_Data=si->xres;
			sawintags[ 3].ti_Data=si->yres;/* +GetRunTime(scy[6]);*/
		}
		else {
			UWORD xp,yp,wi=0,he=0;
			switch(GetRunTime(sfxprefs_sbuf)->mode) {
				case SIZEMODE_ABSOLUTE:
					wi=GetRunTime(sfxprefs_sbuf)->xs1;
					he=GetRunTime(sfxprefs_sbuf)->ys1;
					break;
				case SIZEMODE_RELATIVE:
					wi=(UWORD)(((GetRunTime(sfxprefs_sbuf)->xs1*GetRunTime(clientrect).Width)/1000.0)+0.5);
					he=(UWORD)(((GetRunTime(sfxprefs_sbuf)->ys1*GetRunTime(clientrect).Height)/1000.0)+0.5);
					break;
				UNEXPECTED_DEFAULT
			}
			LIB_INTCALL(SetOptWinBounds(&xp,&yp,wi,he));
			sawintags[ 0].ti_Data=xp;
			sawintags[ 1].ti_Data=yp;
			sawintags[ 2].ti_Data=wi;
			sawintags[ 3].ti_Data=he;
		}
		sawintags[ 5].ti_Data=WFLG_SMART_REFRESH|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_SIZEGADGET|WFLG_DRAGBAR|WFLG_REPORTMOUSE|WFLG_NEWLOOKMENUS;
		sawintags[15].ti_Tag=WA_Zoom;
	}
	LIB_INTCALL(MemUsage(sfxtitle));
	sawintags[ 6].ti_Data=0L;
	sawintags[ 7].ti_Data=(ULONG)si->title;
	sawintags[ 8].ti_Data=(ULONG)sfxtitle;
	sawintags[10].ti_Data=MAX(sawintags[10].ti_Data,GetRunTime(scx[100])+ 6);                 /* Samplewindow anpassen */
	sawintags[11].ti_Data=MAX(sawintags[11].ti_Data,GetRunTime(scy[ 14])+20);
	sawintags[12].ti_Data=GetRunTime(clientrect).Width;
	sawintags[13].ti_Data=GetRunTime(clientrect).Height;
	if(GetRunTime(sfxprefs_misc)->contextbutton==1) sawintags[5].ti_Data|=WFLG_RMBTRAP;
	else sawintags[5].ti_Data&=(~WFLG_RMBTRAP);
	if(GetRunTime(sfxpubscr)==IntuitionBase->FirstScreen) sawintags[5].ti_Data|=WFLG_ACTIVATE;
	else sawintags[5].ti_Data&=(~WFLG_ACTIVATE);
	if(InitScroller(si)) {
		if(si->win=LIB_INTCALL(OpenMWindow(sawintags,GetRunTime(swin_up),&GetRunTime(swin_upct),swin_idcmps))) {
			struct Gadget *gad;
			//-- this is neccesary so that the system recognizes the titlebar-gadget
			if((gad=(struct Gadget *)(si->scrobj[GADID_SWB_ICONIFY]))) {
				RemoveGadget(si->win,gad);
				AddGadget(si->win,gad,0);
			}
			if((gad=(struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))) {
				RemoveGadget(si->win,gad);
				AddGadget(si->win,gad,0);
			}
			si->visibillity=SFX_SOUND_VISIBILLITY_SHOWN;
			si->rp  =si->win->RPort;
			si->xpos=si->win->LeftEdge;
			si->ypos=si->win->TopEdge;
			//if(!si->xres || !si->yres) {
				//si->xo  =si->win->BorderLeft;
				//si->yo  =si->win->BorderTop;
				//si->xres=si->win->Width -(si->win->BorderLeft+si->win->BorderRight+1);
				//si->yres=si->win->Height-(si->win->BorderTop+si->win->BorderBottom+1);
			//}
			si->xo=si->yo=si->xres=si->yres=0;
			SetFont(si->rp,GetRunTime(scrfont));
			LIB_INTCALL(SetSlider(si,SLIDER_BOTH));
			LIB_INTCALL(SetTitle(si));
			SizeWindow(si->win,0,0);
		}
	}
}

void SAVEDS ASM LIB_EXPORT(HideSample(void)) {
	SInfo *si=GetRunTime(aktbuf);

	if(!GetRunTime(onewindow)) {
		si->xpos=si->win->LeftEdge;
		si->ypos=si->win->TopEdge;
		si->xres=si->win->Width -(si->win->BorderLeft+si->win->BorderRight);
		si->yres=si->win->Height-(si->win->BorderTop+si->win->BorderBottom);
		//si->xres=si->win->GZZWidth;
		//si->yres=si->win->GZZHeight-GetRunTime(scy[6]);
		//MSG2("xres=%5ld ,gzzxres=%5ld",si->xres,si->win->GZZWidth);
		//MSG2("yres=%5ld ,gzzyres=%5ld",si->yres,si->win->GZZHeight);
	}
	FreeScroller(si);
	LIB_INTCALL(CloseMWindow(si->win,&GetRunTime(swin_upct)));
	si->win=0L;
	si->visibillity=SFX_SOUND_VISIBILLITY_HIDDEN;
}

void SAVEDS ASM LIB_EXPORT(SetOptWinBounds(REG(a0,UWORD *x),REG(a1,UWORD *y),REG(d0,UWORD w),REG(d1,UWORD h))) {
	struct Screen *pubscr;
	struct Window *win;
	register UWORD xp,yp,i;
	UBYTE found=0,seek;
	UWORD xs,ys;
	UWORD *windata,winanz;
	UWORD xm,xc,yc,xcs=0,ycs=0;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(x,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(y,__FUNC__,"2.par",4));

	//-- here we start our search
	*x=GetRunTime(clientrect).LeftEdge;
	*y=GetRunTime(clientrect).TopEdge;

	//-- calculate Borderwidth and ~height, unfortunately some fields are blank !!
	w+=(GetRunTime(sfxpubscr)->WBorLeft+GetRunTime(sfxpubscr)->WBorRight+GetRunTime(sfxpubscr)->BarVBorder);
	h+=(GetRunTime(sfxpubscr)->WBorTop+GetRunTime(sfxpubscr)->WBorBottom+GetRunTime(sfxpubscr)->BarHBorder+GetRunTime(scy[6])+1);

	if(pubscr=LockPubScreen(SFX_PUBSCRNAME)) {
		// count and collect all windows and record positions and sizes into an array
		// leave out visitor windows as well as our backdrop, toolbar and status window
		win=pubscr->FirstWindow;winanz=0;
		while(win) {
			if((win->Flags&WFLG_VISITOR) || (win->UserData==WT_Basic)) winanz++;
			win=win->NextWindow;
		}
		if(windata=(UWORD *)AllocVec(winanz<<3,MEMF_ANY)) {
			win=pubscr->FirstWindow;i=0;
			while(win) {
				if((win->Flags&WFLG_VISITOR) || (win->UserData==WT_Basic)) {
					windata[i]=win->LeftEdge;i++;
					windata[i]=win->TopEdge;i++;
					windata[i]=win->LeftEdge+win->Width;i++;
					windata[i]=win->TopEdge+win->Height;i++;
				}
				win=win->NextWindow;
			}
			// start searching
			// x-loop
			xm=xp=GetRunTime(clientrect).LeftEdge;
			while(!found && xp<(GetRunTime(clientrect).RightEdge-w)) {
				xp=xm;xm=GetRunTime(clientrect).RightEdge;
				//MSG1("xp : %3d",xp);
				// y-loop
				yp=GetRunTime(clientrect).TopEdge;
				while(!found && yp<(GetRunTime(clientrect).BottomEdge-h)) {
					//MSG1("yp : %3d",yp);
					if(!WinHit(windata,&xc,&yc,winanz,xp,yp)) {     // check if pixel underneath is free
						//MSG2("possible free space at %3d %3d",xp,yp);
						seek=1;
						for(xs=xp;(xs<xp+w && seek);xs++) {             // check the whole space
							for(ys=yp;(ys<yp+h && seek);ys++) {
								if(WinHit(windata,&xcs,&ycs,winanz,xs,ys)) seek=0;
							}
						}
						//MSG2("seek finished with %3d %3d",xs-xp,ys-yp);
						if(seek) {
							found=1;*x=xp;*y=yp;
							//MSG2("free space at %3d %3d",xp,yp);
						}
						else {
							yp=ycs;
							if(xcs<xm) xm=xcs;
						}
					}
					else {
						yp=yc;
						if(xc<xm) xm=xc;
					}
				}
				//MSG1("yp : %3d",yp);
			}
			FreeVec(windata);
		}
		UnlockPubScreen(0l,pubscr);
	}
}

UBYTE WinHit(UWORD *windata,UWORD *xc,UWORD *yc,UWORD winanz,UWORD x,UWORD y) {
	register UWORD i,j;
	UBYTE hit=0;

	for(i=j=0;(i<winanz && !hit);i++,j+=4) {
		if(x>=windata[j] && x<=windata[j+2] && y>=windata[j+1] && y<=windata[j+3]) {
			hit=1;
			*xc=windata[j+2]+1;
			*yc=windata[j+3]+1;
		}
	}
	return(hit);
}

SInfo * SAVEDS ASM LIB_EXPORT(LockBuffer(REG(a0,SInfo *si))) {
	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

	if(si) si->lockct++;
	else MSG("Sample doesn't exists !");
	OUTRO;
	return(si);
}

void SAVEDS ASM LIB_EXPORT(UnlockBuffer(REG(a0,SInfo *si))) {
	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

	if(si) {
		ASSERT_RESET;
		ASSERT(si->lockct);
		if(ASSERT_OKAY) si->lockct--;
	}
	else MSG("Sample doesn't exists !");
	OUTRO;
}

#define ScanCore do {                                           \
	if(*buf>smax) smax=*buf;        /* peak search */           \
	if(*buf<smin) smin=*buf;                                    \
	cur+=(*buf>0) ? (*buf) : -(*buf);                           \
	buf++;                                                      \
} while(0)


void SAVEDS ASM LIB_EXPORT(RecalcSampleLines(REG(a0,SInfo *si))) {
	register ULONG i,j;
	UBYTE l;
	SAFuncPtr SARead;
	SmpAccess *sa;
	WORD *buf;
	LONG smin,smax,cur;
	double srms,savg;
	double fc,div1,div2,cval;

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

	if(!si->lines &&                                                         /* we have not yet calculated some lines AND */
		(si->maxlines || si->rmslines || si->avglines)) {              /* the user generally wants us to calculate some of them */
		si->lines=TRUE;                                                             /* remember that now we have done it */
		for(l=0;l<CH_COUNT(si->channels);l++) {
			smin=smax=0;
			savg=srms=0.0;
			div1=div2=1.0;
			if((sa=LIB_INTCALL(SAInit(si,l,0,si->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK,VM_DEFAULT_BLKLEN)))) {
			//if((sa=LIB_INTCALL(SAInit(si,l,si->zoomxs,si->zoomxl,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK,VM_DEFAULT_BLKLEN)))) {
				SARead=sa->safunc;
				for(sa->seg=1;sa->seg<sa->seganz;sa->seg++) {
					buf=(WORD *)sa->buffer1;if(!SARead(sa,sa->blen)) break;
					for(i=0;i<(sa->blen>>1);i+=128) {
						cur=0;
						for(j=0;j<8;j++) {                                  // 8*16=128
							ScanCore;ScanCore;ScanCore;ScanCore;
							ScanCore;ScanCore;ScanCore;ScanCore;
							ScanCore;ScanCore;ScanCore;ScanCore;
							ScanCore;ScanCore;ScanCore;ScanCore;
						}
						cval=((double)cur)/4194304.0;fc=(div1/div2);        // volume analysis (32768*128=4194304)
						savg=(savg+(      cval /div1))*fc;                  // avg (without overflow)
						srms=(srms+((cval*cval)/div1))*fc;                  // squared avg (without overflow)
						div1=div2;div2+=1.0;
					}
				}
				if(sa->seg==sa->seganz) {		// no failure
					ULONG rest=(si->slen-((sa->seganz-1)<<(sa->blensh-1)));
					buf=(WORD *)sa->buffer1;SARead(sa,sa->blen);
					//MSG2("  ### last block 0x%08lx...0x%08lx",buf,&((UBYTE *)buf)[sa->blen]);
					//MSG3("  ### remaining loops=%ld/%ld, slen=%ld",rest,(sa->blen>>1),si->slen);
					for(i=0;i<rest;i+=128) {
						cur=0;
						for(j=0;((j<32) && (i+(j<<2)<rest));j++) {             // 32*4=128
							ScanCore;ScanCore;ScanCore;ScanCore;
						}
						cval=(double)cur/4194304.0;fc=(div1/div2);      // volume analysis (32768*128=4194304)
						savg=(savg+(      cval /div1))*fc;                  // avg (without overflow)
						srms=(srms+((cval*cval)/div1))*fc;                  // squared avg (without overflow)
						div1=div2;div2+=1.0;
					}
					//MSG1("  ### after loop 0x%08lx",buf);
				}
				LIB_INTCALL(SADone(sa,si));
				si->smax[l]=smax;
				si->smin[l]=smin;
				si->srms[l]=(WORD)(32767.0*sqrt(srms));
				si->savg[l]=(WORD)(32767.0*savg);
			}
			else MSG("SAInit failed");
		}
	}
	OUTRO;
}

//-- helper for scroller windows

UBYTE SAVEDS ASM LIB_EXPORT(InitScrollerWinStuff(void)) {

	INTRO;

	if(!(dri=GetScreenDrawInfo(GetRunTime(sfxpubscr)))) goto Error;
	if(!(sizeimage		=NewImageObject(SIZEIMAGE))) goto Error;
	if(!(leftimage		=NewImageObject(LEFTIMAGE))) goto Error;
	if(!(rightimage		=NewImageObject(RIGHTIMAGE)))  goto Error;
	if(!(upimage		=NewImageObject(UPIMAGE)))  goto Error;
	if(!(downimage		=NewImageObject(DOWNIMAGE)))  goto Error;
	if(GetRunTime(os_version)<50) {
		//-- be more fault tolerant
		TitlebarImageBase=OpenLibrary("sys:Classes/Images/titlebar.image",40L);
		if(!TitlebarImageBase) TitlebarImageBase=OpenLibrary("progdir:Classes/Images/titlebar.image",40L);
		if(TitlebarImageBase) {
			if(!(iconifyimage	=NewObject(NULL,"tbiclass",SYSIA_Which,TBI_ICONIFYIMAGE, SYSIA_DrawInfo,dri,TAG_END)))  goto Error;
			if(!(popupimage		=NewObject(NULL,"tbiclass",SYSIA_Which,TBI_POPUPIMAGE  , SYSIA_DrawInfo,dri,TAG_END)))  goto Error;
		}
	}
	else {
		if(!(iconifyimage	=NewObject(NULL,"sysiclass",SYSIA_Which,ICONIFYIMAGE, SYSIA_DrawInfo,dri,TAG_END)))  goto Error;
		if(!(popupimage		=NewObject(NULL,"sysiclass",SYSIA_Which,POPUPIMAGE  , SYSIA_DrawInfo,dri,TAG_END)))  goto Error;
	}
	//DEBUG
	//MSG2("\tsize : %d %d",IM(sizeimage)->Width,IM(sizeimage)->Height);
	//MSG2("\tleft : %d %d",IM(leftimage)->Width,IM(leftimage)->Height);
	//DEBUG

	OUTRO_BOOL(TRUE);

	return(FALSE);
Error:
	OUTRO_BOOL(FALSE);
	return(TRUE);
}

void SAVEDS ASM LIB_EXPORT(FreeScrollerWinStuff(void)) {
	/** @TODO: crashes when we start and immediately exit sfx */
	if(sizeimage)			{ DisposeObject(sizeimage);sizeimage=NULL; }
	if(leftimage)			{ DisposeObject(leftimage);leftimage=NULL; }
	if(rightimage)			{ DisposeObject(rightimage);rightimage=NULL; }
	if(upimage)				{ DisposeObject(upimage);upimage=NULL; }
	if(downimage)			{ DisposeObject(downimage);downimage=NULL; }
	if(iconifyimage)		{ DisposeObject(iconifyimage);iconifyimage=NULL; }
	if(popupimage)			{ DisposeObject(popupimage);popupimage=NULL; }
	if(dri)					{ FreeScreenDrawInfo(GetRunTime(sfxpubscr),dri);dri=NULL; }
	if(TitlebarImageBase)	{ CloseLibrary(TitlebarImageBase);TitlebarImageBase=NULL; }
}

//-- private Stuff

ULONG GetNextFreeID(void) {
	SInfo *si;
	ULONG id=2;                 // keep ID 0 (for invalid) and 1 (for clip) free
	UBYTE valid=FALSE;

	while(!valid) {
		valid=TRUE;
		si=(SInfo *)(GetRunTime(blist)->lh_Head);
		while(si && !EndOfList(GetRunTime(blist),(struct Node *)si)) {
			if(id==si->id) { valid=FALSE;si=NULL; }
			else si=(SInfo *)(((struct Node *)si)->ln_Succ);
		}
		if(!valid) id++;
	}
	return(id);
}

void BuildBufferArray(void) {
	SInfo *si=(SInfo *)(GetRunTime(blist)->lh_Head);
	register ULONG ix=0;

	if(GetRunTime(barray)) FreeVec(GetRunTime(barray));
	if(GetRunTime(barray)=AllocVec((1+GetRunTime(aktsamples))*sizeof(STRPTR),MEMF_ANY)) {
		while(si && !EndOfList(GetRunTime(blist),(struct Node *)si)) {
			GetRunTime(barray[ix)++]=((struct Node *)si)->ln_Name;
			si=(SInfo *)(((struct Node *)si)->ln_Succ);
		}
		GetRunTime(barray[ix])=NULL;
	}
	MSG2("ix=%ld  aktsmp=%ld",ix,GetRunTime(aktsamples));
	ASSERT_RESET;
	ASSERT(ix==GetRunTime(aktsamples));
}

//-- scroller stuff

UBYTE InitScroller(SInfo *si) {
	register UBYTE i;
	int resolution = SysISize();
	WORD topborder = GetRunTime(sfxpubscr)->WBorTop + GetRunTime(scy[6]) + 1;
	/* Do not use screen->BarHeight, which is the height of the
	 * screens bar, not the height of the windows title bar. */
	WORD w = IM(sizeimage)->Width;
	WORD h = IM(sizeimage)->Height;
	WORD bw = (resolution == SYSISIZE_LOWRES) ? 1 : 2;
	WORD bh = (resolution == SYSISIZE_HIRES) ? 2 : 1;
	WORD rw = (resolution == SYSISIZE_HIRES) ? 3 : 2;
	WORD rh = (resolution == SYSISIZE_HIRES) ? 2 : 1;
	WORD gw,gh,gap;
	Object *tmpgad;

	gh = MAX(IM(leftimage)->Height, h);
	gh = MAX(IM(rightimage)->Height, gh);
	gw = MAX(IM(upimage)->Width, w);
	gw = MAX(IM(downimage)->Width, gw);

	gap = 1; /* If you have gadgets in the left window border, set 'gap' to the width of these gadgets. */

	for(i=0;i<SWB_GADANZ;i++) si->scrobj[i]=NULL;

	//DEBUG
	//Kprintf("generating border gadgets\n");
	//kprintf("tmpgad : %ld\n",tmpgad);
	//DEBUG

	if(!(tmpgad=NewPropObject(FREEHORIZ,
	 GA_Left,           rw + gap,
	 GA_RelBottom,      bh - gh + 2,
	 GA_RelWidth,       -gw - gap - IM(leftimage)->Width - IM(rightimage)->Width - rw - rw,
	 GA_Height,         gh - bh - bh - 2,
	 GA_BottomBorder,   TRUE,
	 //GA_Previous,       tmpgad,                         // otherwise CyberGuard WriteHI to Addr 0x00000000 !!
	 GA_ID,             GADID_SWB_HORIZ,
	 PGA_Total,         si->zoomxl,
	 PGA_Visible,       si->zoomxl,
	TAG_DONE))) return(FALSE);
	si->scrobj[GADID_SWB_HORIZ]=tmpgad;

	//DEBUG
	//kprintf("\thoriz okay\n");
	//kprintf("tmpgad : %ld %ld\n",tmpgad,si->scrobj[GADID_SWB_HORIZ]);
	//DEBUG

	if(!(tmpgad=NewPropObject(FREEVERT,
	 GA_RelRight,       bw - gw + 3,
	 GA_Top,            topborder + rh,
	 GA_Width,          gw - bw - bw - 4,
	 GA_RelHeight,      -topborder - h - IM(upimage)->Height - IM(downimage)->Height - rh - rh,
	 GA_RightBorder,    TRUE,
	 GA_Previous,       tmpgad,
	 GA_ID,             GADID_SWB_VERT,
	 PGA_Total,         si->zoomyl,
	 PGA_Visible,       si->zoomyl,
	TAG_DONE))) return(FALSE);
	si->scrobj[GADID_SWB_VERT]=tmpgad;

// DEBUG
//  kprintf("\tvert okay\n");
//  kprintf("tmpgad : %ld %ld\n",tmpgad,si->scrobj[GADID_SWB_VERT]);
// DEBUG

	if(!(tmpgad=NewButtonObject(leftimage,
	 GA_RelRight,       1 - IM(leftimage)->Width - IM(rightimage)->Width - gw,
	 GA_RelBottom,      1 - IM(leftimage)->Height,
	 GA_BottomBorder,   TRUE,
	 GA_Previous,       tmpgad,
	 GA_ID,             GADID_SWB_LEFT,
	TAG_DONE))) return(FALSE);
	si->scrobj[GADID_SWB_LEFT]=tmpgad;

// DEBUG
//  kprintf("\tleft okay\n");
//  kprintf("tmpgad : %ld %ld\n",tmpgad,si->scrobj[GADID_SWB_LEFT]);
// DEBUG

	if(!(tmpgad=NewButtonObject(rightimage,
	 GA_RelRight,       1 - IM(rightimage)->Width - gw,
	 GA_RelBottom,      1 - IM(rightimage)->Height,
	 GA_BottomBorder,   TRUE,
	 GA_Previous,       tmpgad,
	 GA_ID,             GADID_SWB_RIGHT,
	TAG_DONE))) return(FALSE);
	si->scrobj[GADID_SWB_RIGHT]=tmpgad;

// DEBUG
//  kprintf("\tright okay\n");
//  kprintf("tmpgad : %ld %ld\n",tmpgad,si->scrobj[GADID_SWB_RIGHT]);
// DEBUG

	if(!(tmpgad=NewButtonObject(upimage,
	 GA_RelRight,       1 - IM(upimage)->Width,
	 GA_RelBottom,      1 - IM(upimage)->Height - IM(downimage)->Height - h,
	 GA_RightBorder,    TRUE,
	 GA_Previous,       tmpgad,
	 GA_ID,             GADID_SWB_UP,
	TAG_DONE))) return(FALSE);
	si->scrobj[GADID_SWB_UP]=tmpgad;

// DEBUG
//  kprintf("\tup okay\n");
//  kprintf("tmpgad : %ld %ld\n",tmpgad,si->scrobj[GADID_SWB_UP]);
// DEBUG

	if(!(tmpgad = NewButtonObject(downimage,
		GA_RelRight,       1 - IM(downimage)->Width,
		GA_RelBottom,      1 - IM(downimage)->Height - h,
		GA_RightBorder,    TRUE,
		GA_Previous,       tmpgad,
		GA_ID,             GADID_SWB_DOWN,
	TAG_DONE))) return(FALSE);
	si->scrobj[GADID_SWB_DOWN]=tmpgad;

// DEBUG
//  kprintf("\tdown okay\n");
//  kprintf("tmpgad : %ld %ld\n",tmpgad,si->scrobj[GADID_SWB_DOWN]);
// DEBUG


	if(iconifyimage) {
		LONG pos;

		pos=(GetRunTime(onewindow))?(1 * (IM(iconifyimage)->Width - 1)):(3 * (IM(iconifyimage)->Width - 1));
		if(!(tmpgad = NewButtonObject(iconifyimage,
			GA_RelRight,		1 - pos,
			GA_Top,				0,
/*
			GA_Width,			IM(iconifyimage)->Width - 1,
			GA_Height,			IM(iconifyimage)->Height,
*/
			GA_TopBorder,		TRUE,
			GA_Titlebar,		TRUE,
			GA_Previous,		tmpgad,
			GA_ID,				GADID_SWB_ICONIFY,
			TAG_END))) return(FALSE);
		si->scrobj[GADID_SWB_ICONIFY]=tmpgad;

		pos=(GetRunTime(onewindow))?(2 * (IM(popupimage)->Width - 1)):(4 * (IM(popupimage)->Width - 1));
		if(!(tmpgad = NewButtonObject(popupimage,
			GA_RelRight,		1 - pos,
			GA_Top,				0,
/*
			GA_Width,			IM(popupimage)->Width - 1,
			GA_Height,			IM(popupimage)->Height,
*/
			GA_TopBorder,		TRUE,
			GA_Titlebar,		TRUE,
			GA_Previous,		tmpgad,
			GA_ID,				GADID_SWB_POPUP,
			TAG_END))) return(FALSE);
		si->scrobj[GADID_SWB_POPUP]=tmpgad;
	}

	sawintags[ 6].ti_Data=(ULONG)si->scrobj[GADID_SWB_HORIZ];
	sawintags[10].ti_Data=MAX(sawintags[10].ti_Data, gw + gap + IM(leftimage)->Width + IM(rightimage)->Width + rw + rw + KNOBHMIN);
	sawintags[11].ti_Data=MAX(sawintags[11].ti_Data, topborder + h + IM(upimage)->Height + IM(downimage)->Height + rh + rh + KNOBVMIN);
	return(TRUE);
}

void FreeScroller(SInfo *si) {
	register UBYTE i;

	for(i=0;i<SWB_GADANZ;i++) {
		if(si->scrobj[i]) {
			RemoveGadget(si->win,(struct Gadget *)si->scrobj[i]);
			DisposeObject(si->scrobj[i]);
		}
	}
}

// Calculates the basic size of the resolution.

int SysISize(VOID) {
	return(GetRunTime(sfxpubscr)->Flags & SCREENHIRES ? SYSISIZE_MEDRES : SYSISIZE_LOWRES); /* NB: SYSISIZE_HIRES not yet supported. */
}

/* Creates a sysiclass object. */
Object *NewImageObject(ULONG which) {
	return (NewObject(NULL, SYSICLASS,
		SYSIA_DrawInfo, dri,
		SYSIA_Which,    which,
		SYSIA_Size,     SysISize(),
	TAG_DONE));
}

/* Creates an object or our propgclass. */
Object *NewPropObject(ULONG freedom, Tag tag1, ...) {
	return (NewObject(NULL,PROPGCLASS,
		ICA_TARGET,     ICTARGET_IDCMP, /* Send update to IDCMP.  If we make it a model, we would send the notification to our model object. */
		PGA_Freedom,    freedom,
		PGA_NewLook,    TRUE,
		PGA_Borderless, ((dri->dri_Flags&DRIF_NEWLOOK) && (dri->dri_Depth>1)),  /* Borderless does only look right with newlook screens */
	TAG_MORE, &tag1));
}

/* Creates an object of our buttongclass. */
Object *NewButtonObject(Object *image, Tag tag1, ...) {
	return (NewObject(NULL,BUTTONGCLASS,
		ICA_TARGET, ICTARGET_IDCMP,
		GA_Image,   image,          /* No need for GA_Width/Height.  buttongclass is smart :) */
	TAG_MORE, &tag1));
}

//-- eof ----------------------------------------------------------------------
