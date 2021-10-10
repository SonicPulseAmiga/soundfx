/****h* sfxsupport.library/req-period.c [4.3] *
*
*  NAME
*    req-period.c
*  COPYRIGHT
*    $VER: req-period.c 4.3 (16.09.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    get period requester - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Sep.1998
*  MODIFICATION HISTORY
*    16.Sep.2003	V 4.3	most recent version
*    18.Feb.2003	V 4.2	most recent version
*    28.Jun.2000	V 1.30	most recent version
*    26.Sep.1998	V 1.20	most recent version
*    19.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define REQ_PERIOD_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

#include "SFX-ImgKeyboard.h"
#include "SFX-Periodes.h"

//-- prototypes ---------------------------------------------------------------

ULONG		ASM LIB_EXPORT(GetPeriode(REG(d0,ULONG srat),REG(a0,SInfo *si)));
UBYTE		ASM LIB_EXPORT(Rate2KeyFrq(REG(d0,ULONG   srat),REG(a0,STRPTR key ),REG(a1,double *frq)));
UBYTE		ASM LIB_EXPORT(Key2RateFrq(REG(a0,STRPTR  key ),REG(a1,ULONG *srat),REG(a2,double *frq)));
UBYTE		ASM LIB_EXPORT(Frq2RateKey(REG(a0,double *frq ),REG(a1,ULONG *srat),REG(a2,STRPTR  key)));

//-- defines ------------------------------------------------------------------

#define GADID_OKAY	0

#define GADID_RATE	(GADID_OKAY+1)
#define GADIX_RATE	0
#define GADID_KEY	(GADID_RATE+1)
#define GADIX_KEY	(GADIX_RATE+1)
#define GADID_FRQ	(GADID_KEY+1)
#define GADIX_FRQ	(GADIX_KEY+1)
#define GADID_OTHER	(GADID_FRQ+1)
#define GADIX_OTHER	(GADIX_FRQ+1)

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

ULONG SAVEDS ASM LIB_EXPORT(GetPeriode(REG(d0,ULONG srat),REG(a0,SInfo *si))) {
	struct NewGadget ng;
	struct Window *gpwin;
	struct Gadget *g;
	struct Gadget *gpGList=NULL;
	struct Gadget *gpGadgets[GADIX_OTHER+1];
	struct RastPort *gprp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	UBYTE runing=TRUE,changes=FALSE;
	UWORD mousex,mousey,xo,sl;
	UBYTE okt=0,halftone=0,nr,otherrat;
	char rat[10],key[4],frq[20];
	double freq;
	UBYTE *OtherRLabels[]	={"Other","8000 Hz","11025 Hz","22050 Hz","28867 Hz","32000 Hz","44100 Hz","48000 Hz","57734 Hz","96000 Hz",0l };
	UWORD ys1=2;
	UWORD ys2=ys1+31+GetRunTime(scy[FRAME_HEIGHT(2)+6]);
	UWORD winh=ys2+2+GetRunTime(scy[FRAME_HEIGHT(1)]);
	UWORD ys,yh=GetRunTime(scy[12]);
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[434]);
	UWORD xh;
	struct TagItem wintags[]={
/*  0 */WA_Left,			0,
/*  1 */WA_Top,				0,
/*  2 */WA_InnerWidth,		0,
/*  3 */WA_InnerHeight,		0,
/*  4 */WA_IDCMP,			BUTTONIDCMP|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS,
/*  5 */WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
/*  6 */WA_Gadgets,			0l,
/*  7 */WA_Title,			0l,
/*  8 */WA_ScreenTitle,		(ULONG)VERS,
/*  9 */WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
/* 10 */WA_BlockPen,		1,
		TAG_DONE
	};

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER(si,__FUNC__,"1.par",4));

	LIB_INTCALL(Rate2KeyFrq(srat,key,&freq));
	sprintf(rat,"%ld",srat);
	if(freq>0.0) sprintf(frq,"%lf",freq);
	else sprintf(frq,"--------");
	switch(srat) {
		case  8000: otherrat=1;break;
		case 11025: otherrat=2;break;
		case 22050: otherrat=3;break;
		case 28867: otherrat=4;break;
		case 32000: otherrat=5;break;
		case 44100: otherrat=6;break;
		case 48000: otherrat=7;break;
		case 57734:	otherrat=8;break;
		case 96000:	otherrat=9;break;
		default:	otherrat=0;break;
	}

	if(g=CreateContext(&gpGList)) {
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[177]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[80]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	=LIB_INTCALL(GetString(strOkay));
		ng.ng_TextAttr		=GetRunTime(ScrFontB);
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ys=ys1+30+GetRunTime(scy[8]);
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[58]);
		ng.ng_TopEdge		=ys;
		ng.ng_Width			=GetRunTime(scx[70]);
		ng.ng_GadgetText	="Rate";
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetID		=GADID_RATE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,rat,GTTX_CopyText,TRUE,TAG_DONE);
		gpGadgets[GADIX_RATE]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[203]);
		ng.ng_GadgetText	="Key";
		ng.ng_GadgetID		=GADID_KEY;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,key,GTTX_CopyText,TRUE,TAG_DONE);
		gpGadgets[GADIX_KEY]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[328]);
		ng.ng_GadgetText	="Frq";
		ng.ng_GadgetID		=GADID_FRQ;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,frq,GTTX_CopyText,TRUE,TAG_DONE);
		gpGadgets[GADIX_FRQ]=g;

		ys+=yh;
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[58]);
		ng.ng_TopEdge		=ys;
		ng.ng_Width			=GetRunTime(scx[100]);
		ng.ng_GadgetText	="Other";
		ng.ng_GadgetID		=GADID_OTHER;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,OtherRLabels,GTCY_Active,otherrat,TAG_DONE);
		gpGadgets[GADIX_OTHER]=g;

		if(g) {
			wintags[0].ti_Data=(GetRunTime(xres)-winw)>>1;
			wintags[1].ti_Data=(GetRunTime(yres)-winh)>>1;
			wintags[2].ti_Data=winw;
			wintags[3].ti_Data=winh;
			wintags[6].ti_Data=(ULONG)gpGList;
			wintags[7].ti_Data=(ULONG)LIB_INTCALL(GetString(strSelectFrequency));
			if(gpwin=OpenWindowTagList(0l,wintags)) {
				GT_RefreshWindow(gpwin,0l);
				gprp=gpwin->RPort;SetFont(gprp,GetRunTime(scrfont));

				SetAPen(gprp,1);
				xh=GetRunTime(scx[434]);
				LIB_INTCALL(DrawGadWB(gprp,xs1,ys1,xh,30+GetRunTime(scy[FRAME_HEIGHT(2)+6])));
				LIB_INTCALL(DrawGadWB(gprp,xs1,ys2,xh,GetRunTime(scy[FRAME_HEIGHT(1)])));

				xo=xs1+4+((GetRunTime(scx[427])-(5*70))>>1);
				sl=(70-TextLength(gprp,"F1",2))>>1;

				ys=ys1+2+GetRunTime(scy[6]);
				DrawImage(gprp,&keyboard,xo    ,ys);Move(gprp,xo+sl    ,ys);Text(gprp,"F1",2);
				DrawImage(gprp,&keyboard,xo+70 ,ys);Move(gprp,xo+sl+ 70,ys);Text(gprp,"F2",2);
				DrawImage(gprp,&keyboard,xo+140,ys);Move(gprp,xo+sl+140,ys);Text(gprp,"F3",2);
				DrawImage(gprp,&keyboard,xo+210,ys);Move(gprp,xo+sl+210,ys);Text(gprp,"F4",2);
				DrawImage(gprp,&keyboard,xo+280,ys);Move(gprp,xo+sl+280,ys);Text(gprp,"F5",2);

				while(runing) {
					WaitPort(gpwin->UserPort);
					while(imsg=GT_GetIMsg(gpwin->UserPort)) {
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						mousex	=imsg->MouseX-gpwin->BorderLeft;
						mousey	=imsg->MouseY-gpwin->BorderTop;
						GT_ReplyIMsg(imsg);
						if(iclass==IDCMP_GADGETUP) {
							switch((int)(((struct Gadget *)iadr)->GadgetID)) {
								case GADID_OKAY:
									runing=FALSE;break;
								case GADID_OTHER:
									otherrat=icode;
									switch(otherrat) {
										case 1:	srat= 8000;break;
										case 2:	srat=11025;break;
										case 3:	srat=22050;break;
										case 4:	srat=28867;break;
										case 5:	srat=32000;break;
										case 6:	srat=44100;break;
										case 7: srat=48000;break;
										case 8: srat=57734;break;
										case 9: srat=96000;break;
									}
									LIB_INTCALL(Rate2KeyFrq(srat,key,&freq));
									sprintf(rat,"%ld",srat);
									if(freq>0.0) sprintf(frq,"%lf",freq);
									else sprintf(frq,"--------");
									GT_SetGadgetAttrs(gpGadgets[GADIX_RATE],gpwin,0l,GTTX_Text,rat,TAG_DONE);
									GT_SetGadgetAttrs(gpGadgets[GADIX_KEY ],gpwin,0l,GTTX_Text,key,TAG_DONE);
									GT_SetGadgetAttrs(gpGadgets[GADIX_FRQ ],gpwin,0l,GTTX_Text,frq,TAG_DONE);
									if(si) {
										si->srat=srat;
										if(GetRunTime(play) && si==GetRunTime(plsi)) LIB_INTCALL(SetNewPlaybackRate());
									}
									break;
							}
						}
						/*
						if(iclass==IDCMP_VANILLAKEY) {
							switch(icode) {
								case KEY_ENTER:
								case KEY_RETURN:
									runing=FALSE;break;
								case KEY_ESC:
									runing=FALSE;srat=0;break;
								case 'y':			// C-
								case 'Y':
									halftone=0;changes=TRUE;break;
								case 's':			// C#
								case 'S':
									halftone=1;changes=TRUE;break;
								case 'x':			// D-
								case 'X':
									halftone=2;changes=TRUE;break;
								case 'd':			// D#
								case 'D':
									halftone=3;changes=TRUE;break;
								case 'c':			// E-
								case 'C':
									halftone=4;changes=TRUE;break;
								case 'v':			// F-
								case 'V':
									halftone=5;changes=TRUE;break;
								case 'g':			// F#
								case 'G':
									halftone=6;changes=TRUE;break;
								case 'b':			// G-
								case 'B':
									halftone=7;changes=TRUE;break;
								case 'h':			// G#
								case 'H':
									halftone=8;changes=TRUE;break;
								case 'n':			// A-
								case 'N':
									halftone=9;changes=TRUE;break;
								case 'j':			// A#-
								case 'J':
									halftone=10;changes=TRUE;break;
								case 'm':			// H-
								case 'M':
									halftone=11;changes=TRUE;break;
							}
						}
						*/
						if(iclass==IDCMP_CLOSEWINDOW) { runing=FALSE;srat=0; }
						if(iclass==IDCMP_RAWKEY) {
							switch(icode) {
								case RKEY_ENTER:
								case RKEY_RETURN:
									runing=FALSE;break;
								case RKEY_ESC:
									runing=FALSE;srat=0;break;
								case RKEY_HELP:
									LIB_INTCALL(ShowHelp("html/nodes/node01.05.04.html"));break;
								case RKEY_F1:	okt=0;changes=TRUE;break;		/* Oktave 0 */
								case RKEY_F2:	okt=1;changes=TRUE;break;		/* Oktave 1 */
								case RKEY_F3:	okt=2;changes=TRUE;break;		/* Oktave 2 */
								case RKEY_F4:	okt=3;changes=TRUE;break;		/* Oktave 3 */
								case RKEY_F5:	okt=4;changes=TRUE;break;		/* Oktave 4 */
								case 0x31:		halftone= 0;changes=TRUE;break;	/* C- */
								case 0x21:		halftone= 1;changes=TRUE;break;	/* C# */
								case 0x32:		halftone= 2;changes=TRUE;break;	/* D- */
								case 0x22:		halftone= 3;changes=TRUE;break;	/* D# */
								case 0x33:		halftone= 4;changes=TRUE;break;	/* E- */
								case 0x34:		halftone= 5;changes=TRUE;break;	/* F- */
								case 0x24:		halftone= 6;changes=TRUE;break;	/* F# */
								case 0x35:		halftone= 7;changes=TRUE;break;	/* G- */
								case 0x25:		halftone= 8;changes=TRUE;break;	/* G# */
								case 0x36:		halftone= 9;changes=TRUE;break;	/* A- */
								case 0x26:		halftone=10;changes=TRUE;break;	/* A# */
								case 0x37:		halftone=11;changes=TRUE;break;	/* H- */
							}
						}
						if(iclass==IDCMP_MOUSEBUTTONS) {
							if(icode==SELECTUP) {
								if(mousex>xo && mousex<xo+350 && mousey>(ys+4) && mousey<ys+28) {
									mousex-=xo;mousey-=(ys+4);
									okt=mousex/70;
									mousex-=(okt*70);
									if(mousex> 0 && mousex<10) halftone= 0;	/* C- */
									if(mousex>10 && mousex<20) halftone= 2;	/* D- */
									if(mousex>20 && mousex<30) halftone= 4;	/* E- */
									if(mousex>30 && mousex<40) halftone= 5;	/* F- */
									if(mousex>40 && mousex<50) halftone= 7;	/* G- */
									if(mousex>50 && mousex<60) halftone= 9;	/* A- */
									if(mousex>60 && mousex<70) halftone=11;	/* H- */
									if(mousex> 6 && mousex<14 && mousey<15) halftone= 1;	/* C# */
									if(mousex>16 && mousex<24 && mousey<15) halftone= 3;	/* D# */
									if(mousex>36 && mousex<44 && mousey<15) halftone= 6;	/* F# */
									if(mousex>46 && mousex<54 && mousey<15) halftone= 8;	/* G# */
									if(mousex>56 && mousex<64 && mousey<15) halftone=10;	/* A# */
									changes=TRUE;
								}
							}
						}
						if(changes) {
							nr=59-(okt*12+halftone);
							srat=PerTab[nr].rate;sprintf(rat,"%ld",srat);
							freq=PerTab[nr].frq;sprintf(frq,"%lf",freq);
							GT_SetGadgetAttrs(gpGadgets[GADIX_RATE],gpwin,0l,GTTX_Text,rat,TAG_DONE);
							GT_SetGadgetAttrs(gpGadgets[GADIX_KEY ],gpwin,0l,GTTX_Text,PerTab[nr].key,TAG_DONE);
							GT_SetGadgetAttrs(gpGadgets[GADIX_FRQ ],gpwin,0l,GTTX_Text,frq,TAG_DONE);
							if(otherrat) {
								switch(srat) {
									case  8000: otherrat=1;break;
									case 11025: otherrat=2;break;
									case 22050: otherrat=3;break;
									case 28867: otherrat=4;break;
									case 32000: otherrat=5;break;
									case 44100: otherrat=6;break;
									case 48000: otherrat=7;break;
									case 57734:	otherrat=8;break;
									case 96000:	otherrat=9;break;
									default:	otherrat=0;break;
								}
								GT_SetGadgetAttrs(gpGadgets[3],gpwin,0l,GTCY_Active,otherrat,TAG_DONE);
							}
							if(si) {
								si->srat=srat;
								if(GetRunTime(play) && si==GetRunTime(plsi)) LIB_INTCALL(SetNewPlaybackRate());
							}
							changes=FALSE;
						}
					}
				}
				CloseWindow(gpwin);
			}
			else LIB_INTCALL(Message(errOpenWindow,NULL,__FILE__,__LINE__));
			FreeGadgets(gpGList);
		}
		else LIB_INTCALL(Message(errCreateGadgets,NULL,__FILE__,__LINE__));
	}
	else LIB_INTCALL(Message(errCreateContext,NULL,__FILE__,__LINE__));
	return(srat);
}

UBYTE SAVEDS ASM LIB_EXPORT(Rate2KeyFrq(REG(d0,ULONG srat),REG(a0,STRPTR key),REG(a1,double *frq))) {
	register UBYTE i;
	UBYTE ret=FALSE;

	if(key) strcpy(key,"---");
	if(frq) *frq=0.0;
	if(srat>0) {
		for(i=0;i<60;i++) {
			if(PerTab[i].rate==srat) {
				if(key) strcpy(key,PerTab[i].key);
				if(frq) *frq=PerTab[i].frq;
				//okt=(59-i)/12;halftone=(59-i)-okt*12;
				ret=TRUE;i=60;
			}
		}
	}
	//ASSERT_RESET;
	//ASSERT(key && (key[3]=='\0'));
	return(ret);
}

// *frq must not be NULL

UBYTE SAVEDS ASM LIB_EXPORT(Key2RateFrq(REG(a0,STRPTR key),REG(a1,ULONG *srat),REG(a2,double *frq))) {
	register UBYTE i;
	UBYTE ret=FALSE;
	BYTE keynr=-1,okt=-1;
	double rat;

	ASSERT_RESET;
	ASSERT(frq);
	ASSERT(key);

	switch(key[0]) {
		case 'C':
		case 'c':
			if(key[1]=='-') keynr=0;
			else if(key[1]=='#') keynr=1;
			break;
		case 'D':
		case 'd':
			if(key[1]=='-') keynr=2;
			else if(key[1]=='#') keynr=3;
			break;
		case 'E':
		case 'e':
			if(key[1]=='-') keynr=4;
			break;
		case 'F':
		case 'f':
			if(key[1]=='-') keynr=5;
			else if(key[1]=='#') keynr=6;
			break;
		case 'G':
		case 'g':
			if(key[1]=='-') keynr=7;
			else if(key[1]=='#') keynr=8;
			break;
		case 'A':
		case 'a':
			if(key[1]=='-') keynr=9;
			else if(key[1]=='#') keynr=10;
			break;
		case 'H':
		case 'h':
		case 'B':
		case 'b':
			if(key[1]=='-') keynr=11;
			break;
	}
	if(key[2]>='0' && key[2]<='9') okt=key[2]-48;
	if(keynr!=-1 && okt!=-1) {
		*frq=55.0*(1L<<okt);
		 rat=1763.780231*(1L<<okt);
		for(i=0;i<keynr+3;i++) {
			*frq*=1.059463094;
			 rat*=1.059463094;
		}
		*srat=(ULONG)rat;
		ret=TRUE;
	}
	return(ret);
}

UBYTE SAVEDS ASM LIB_EXPORT(Frq2RateKey(REG(a0,double *frq),REG(a1,ULONG *srat),REG(a2,STRPTR key))) {
	register UBYTE i;
	UBYTE ret=FALSE;
	ULONG frq2=(ULONG)(*frq*100.0);

	if(key) strcpy(key,"---");
	if(srat) *srat=0;
	if(*frq!=0.0) {
		for(i=0;i<60;i++) {
			//if(PerTab[i].frq==*frq)
			if((ULONG)(PerTab[i].frq*100.0)==frq2) {
				if(srat) *srat=PerTab[i].rate;
				if(key) strcpy(key,PerTab[i].key);
				ret=TRUE;i=60;
			}
		}
	}
	return(ret);
}

//-- eof ----------------------------------------------------------------------
