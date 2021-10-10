/****h* sfxsupport.library/req-interpol.c [4.3] *
*
*  NAME
*    req-interpol.c
*  COPYRIGHT
*    $VER: req-interpol.c 4.3 (05.08.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    get interpolator requester - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    05.Aug.2003
*  MODIFICATION HISTORY
*    05.Aug.2003	V 4.3	most recent version
*    05.Aug.2003	V 4.3	initial version
*  NOTES
*
*******
*/

#define REQ_INTERPOL_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

UBYTE GetInterpol(Interpolator *interpol);
void RefreshInterpolWin(Interpolator *interpol,struct Window *win,struct Gadget *Gadgets[],UWORD xs1,UWORD ys1);

//-- defines ------------------------------------------------------------------

#define GADID_OKAY		0

#define GADID_INTSEL	(GADID_OKAY+1)
#define GADIX_INTSEL	0

#define GADID_INTLEN	(GADID_INTSEL+1)
#define GADIX_INTLEN	(GADIX_INTSEL+1)

//-- definitions --------------------------------------------------------------

UBYTE GetInterpol(Interpolator *interpol) {
	struct NewGadget ng;
	struct Gadget *g;
	struct RastPort *rp;
	struct Window *win;
	struct Gadget *gadgets[GADIX_INTLEN+1];
	struct Gadget *glist=0L;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	ULONG css=0,csm=0,ccs=500,ccm=500;
	UBYTE akttest=0,takt;
	UBYTE runing=TRUE,ret=FALSE;
	UWORD ys1=2;
	UWORD ys2=ys1+1+GetRunTime(scy[70]);
	UWORD winh=ys2+2+GetRunTime(scy[FRAME_HEIGHT(1)]);
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[294]);
	UWORD xh;
	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS,
		WA_Gadgets,			0l,
		WA_Title,			0l,
		WA_ScreenTitle,		(ULONG)VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};

	if(!interpol->desc[0]) RefreshInterpolWin(interpol,0L,0L,xs1,ys1);
	else {
		if(g=CreateContext(&glist)) {
			ng.ng_LeftEdge		=xs1+GetRunTime(scx[107]);
			ng.ng_TopEdge		=ys2+GetRunTime(scy[2]);
			ng.ng_Width			=GetRunTime(scx[80]);
			ng.ng_Height		=GetRunTime(scy[11]);
			ng.ng_GadgetText	="_Okay";
			ng.ng_TextAttr		=GetRunTime(ScrFontB);
			ng.ng_GadgetID		=GADID_OKAY;
			ng.ng_Flags			=PLACETEXT_IN;
			ng.ng_VisualInfo	=GetRunTime(VisualInfo);
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

			ng.ng_LeftEdge		=xs1+GetRunTime(scx[8]);
			ng.ng_TopEdge		=ys1+GetRunTime(scy[2]);
			ng.ng_Width			=GetRunTime(scx[138]);
			ng.ng_Height		=GetRunTime(scy[50]);
			ng.ng_GadgetText	="";
			ng.ng_TextAttr		=GetRunTime(ScrFont);
			ng.ng_GadgetID		=GADID_INTSEL;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,&interpolList,GTLV_Selected,0,GTLV_Top,0,GTLV_ShowSelected,0l,TAG_DONE);
			gadgets[GADIX_INTSEL]=g;

			ng.ng_LeftEdge		=xs1+GetRunTime(scx[46]);
			ng.ng_TopEdge		=ys1+GetRunTime(scy[55]);
			ng.ng_Width			=GetRunTime(scx[100]);
			ng.ng_Height		=GetRunTime(scy[11]);
			ng.ng_GadgetText	="Len";
			ng.ng_GadgetID		=GADID_INTLEN;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_String,0l,GTST_MaxChars,15,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=GetRunTime(stringextend);
			g->Activation|=GACT_STRINGEXTEND;
			gadgets[GADIX_INTLEN]=g;

			if(g) {
				wintags[0].ti_Data=(GetRunTime(xres)-winw)>>1;
				wintags[1].ti_Data=(GetRunTime(yres)-winh)>>1;
				wintags[2].ti_Data=winw;
				wintags[3].ti_Data=winh;
				wintags[6].ti_Data=(ULONG)glist;
				wintags[7].ti_Data=(ULONG)LIB_INTCALL(GetString(strSelectInterpol));
				if(win=OpenWindowTagList(0l,wintags)) {
					GT_RefreshWindow(win,0l);
					rp=win->RPort;SetFont(rp,GetRunTime(scrfont));

					xh=GetRunTime(scx[294]);
					LIB_INTCALL(DrawGadWB(rp,xs1,ys1,xh,GetRunTime(scy[70])));					/* Par */
					LIB_INTCALL(DrawGadWB(rp,xs1,ys2,xh,GetRunTime(scy[FRAME_HEIGHT(1)])));		/* Okay */

					LIB_INTCALL(DrawGadBW(rp,xs1+GetRunTime(scx[154]),ys1+GetRunTime(scy[ 2]),GetRunTime(scx[128])+3,GetRunTime(scy[64])+3));	/* Window Wave */

					RefreshInterpolWin(interpol,win,gadgets,xs1,ys1);

					while(runing) {
						WaitPort(win->UserPort);
						while(imsg=GT_GetIMsg(win->UserPort)) {
							iclass	=imsg->Class;
							iadr	=imsg->IAddress;
							icode	=imsg->Code;
							GT_ReplyIMsg(imsg);
							if(iclass==IDCMP_GADGETUP) {
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_OKAY:
										runing=FALSE;ret=TRUE;break;
									case GADID_INTSEL:
										takt=icode;
										if(!akttest) { CurrentTime(&css,&csm);akttest=1; }
										else {
											CurrentTime(&ccs,&ccm);
											if(DoubleClick(css,csm,ccs,ccm) && interpol->type==takt) {
												runing=FALSE;
												css=0;csm=0;ccs=500;ccm=500;akttest=0;
											}
											else { css=ccs;csm=ccm;ccs=500;ccm=500; }
										}
										interpol->type=takt;
										switch(interpol->type) {
											case INT_TYPE_NONE:		interpol->range=0.0;break;
											case INT_TYPE_LIN:		interpol->range=2.0;break;
											case INT_TYPE_SI:		interpol->range=3.0;break;
											case INT_TYPE_LAGRANGE:	interpol->range=3.0;break;
										}
										RefreshInterpolWin(interpol,win,gadgets,xs1,ys1);
										break;
									case GADID_INTLEN:
										interpol->range=atof(((struct StringInfo *)(gadgets[GADIX_INTLEN]->SpecialInfo))->Buffer);
										RefreshInterpolWin(interpol,win,gadgets,xs1,ys1);
										break;
								}
								if(iclass==IDCMP_VANILLAKEY) {
									switch(icode) {
										case KEY_ENTER:
										case KEY_RETURN:
											runing=FALSE;ret=TRUE;break;
										case KEY_ESC:
											runing=FALSE;break;
									}
								}
							}
							if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP) LIB_INTCALL(ShowHelp("html/nodes/node01.05.06.html"));
							if(iclass==IDCMP_CLOSEWINDOW) runing=FALSE;
						}
					}
					CloseWindow(win);
				}
				else LIB_INTCALL(Message(errOpenWindow,NULL,__FILE__,__LINE__));
				FreeGadgets(glist);
			}
			else LIB_INTCALL(Message(errCreateGadgets,NULL,__FILE__,__LINE__));
		}
		else LIB_INTCALL(Message(errCreateContext,NULL,__FILE__,__LINE__));
	}
	return(ret);
}

void RefreshInterpolWin(Interpolator *interpol,struct Window *win,struct Gadget *Gadgets[],UWORD xs1,UWORD ys1) {
	register UWORD i;
	struct RastPort *rp;
	UBYTE akt=interpol->type;
	char buf[15];
	ULONG xres1=GetRunTime(scx[128]);
	LONG xoff,yoff;
	SAMPLE *rbuf,*rbufbase;
	ULONG rbufmask,rbufsize;
	double scl,val,rbufr,step;
	LONG irange;
	InterpolFuncPtr interpolfunc;

	switch(interpol->type) {
		case INT_TYPE_SI:
			if(interpol->range<3.0) interpol->range=3.0;
			break;
		case INT_TYPE_LAGRANGE:
			if(interpol->range<3.0) interpol->range=3.0;
			break;
	}
	if(win) {
		GT_SetGadgetAttrs(Gadgets[GADIX_INTSEL],win,0l,GTLV_Selected,akt,GTLV_Top,akt,TAG_DONE);
		sprintf(buf,"%7.4lf",interpol->range);
		switch(interpol->type) {
			case INT_TYPE_NONE:
			case INT_TYPE_LIN:
				GT_SetGadgetAttrs(Gadgets[GADIX_INTLEN],win,0l,GTST_String,buf,GA_Disabled,TRUE,TAG_DONE);break;
			case INT_TYPE_SI:
			case INT_TYPE_LAGRANGE:
				GT_SetGadgetAttrs(Gadgets[GADIX_INTLEN],win,0l,GTST_String,buf,GA_Disabled,FALSE,TAG_DONE);break;
		}
		// 4 test data plus enough for interpolation to both sides
		irange=interpol->range;
		if(!irange) irange=1;
		//MSG1("irange=%ld",irange);
		if(rbuf=LIB_INTCALL(NewRingBuf(((irange<<1)+4),&rbufbase,&rbufmask,&rbufsize))) {
			memset(rbuf,0,(rbufsize<<1));
			LIB_INTCALL(AssignInterpolFunc(interpol->type,&interpolfunc));
			rp=win->RPort;
			SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_BACK]);
			RectFill(rp,xs1+GetRunTime(scx[154])+2,ys1+GetRunTime(scy[ 2])+2,xs1+GetRunTime(scx[154])+GetRunTime(scx[128])+1,ys1+GetRunTime(scy[) 2]+GetRunTime(scy[64])+1);
			SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_GRID]);
			Move(rp,xs1+GetRunTime(scx[154+32])+2,ys1+GetRunTime(scy[ 2   ])+2);Draw(rp,xs1+GetRunTime(scx[154+ 32])+2,ys1+GetRunTime(scy[ 2+64])+1);
			Move(rp,xs1+GetRunTime(scx[154+64])+2,ys1+GetRunTime(scy[ 2   ])+2);Draw(rp,xs1+GetRunTime(scx[154+ 64])+2,ys1+GetRunTime(scy[ 2+64])+1);
			Move(rp,xs1+GetRunTime(scx[154+96])+2,ys1+GetRunTime(scy[ 2   ])+2);Draw(rp,xs1+GetRunTime(scx[154+ 96])+2,ys1+GetRunTime(scy[ 2+64])+1);
			Move(rp,xs1+GetRunTime(scx[154   ])+2,ys1+GetRunTime(scy[ 2+32])+2);Draw(rp,xs1+GetRunTime(scx[154+128])+2,ys1+GetRunTime(scy[ 2+32])+1);
			SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_LINE]);

			// generate testdata
			rbufbase[irange-1]=  1024;
			rbufbase[irange+0]=  4096;
			rbufbase[irange+1]= 28672;
			rbufbase[irange+2]=  8192;
			rbufbase[irange+3]=- 4096;
			rbufbase[irange+4]=-  512;
			rbufr=(double)Bytes2Samples((ULONG)rbufbase&rbufmask)+irange;
			// interpolate result
			yoff=ys1+GetRunTime(scy[2])+GetRunTime(scy[32])+1;xoff=xs1+GetRunTime(scx[154])+2;
			scl=GetRunTime(scy[32])/32768.0;
			Move(rp,xoff,yoff);							// draw window
			step=4.0/(double)xres1;
			for(i=0;i<xres1;i++,rbufr+=step) {
				val=interpolfunc(rbufbase,&rbufr,rbufmask,irange);
				//MSG3("i=%3ld  rbufr=%6.4lf  val=%lf",i,rbufr,val);
				Draw(rp,xoff+i,yoff-(LONG)(scl*val));
			}
			Draw(rp,xoff+(xres1-1),yoff);
			LIB_INTCALL(DelRingBuf(rbuf));
		}
		else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));
	}
	sprintf(interpol->desc,"%s %7.4lf",InterpolLabels[akt],interpol->range);			// generate String
}

//-- eof ----------------------------------------------------------------------
