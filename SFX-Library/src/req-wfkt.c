/****h* sfxsupport.library/req-wfkt.c [4.2] *
*
*  NAME
*    req-wfkt.c
*  COPYRIGHT
*    $VER: req-wfkt.c 4.2 (04.06.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    get window function requester - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Sep.1998
*  MODIFICATION HISTORY
*    04.Jun.2002	V 4.2	most recent version
*    21.Aug.2000	V 4.1	most recent version
*    26.Sep.1998	V 1.20	most recent version
*    19.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define REQ_WFKT_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

UBYTE GetWFkt(WinFunction *wfkt);
void RefreshWFktWin(WinFunction *wfkt,struct Window *win,struct Gadget *Gadgets[],UWORD xs1,UWORD ys1);

//-- defines ------------------------------------------------------------------

#define GADID_OKAY		0

#define GADID_WFKTSEL	(GADID_OKAY+1)
#define GADIX_WFKTSEL	0

#define GADID_WFKTPAR	(GADID_WFKTSEL+1)
#define GADIX_WFKTPAR	(GADIX_WFKTSEL+1)

//-- definitions --------------------------------------------------------------

UBYTE GetWFkt(WinFunction *wfkt) {
	struct NewGadget ng;
	struct Gadget *g;
	struct RastPort *wfrp;
	struct Window *win;
	struct Gadget *gadgets[GADIX_WFKTPAR+1];
	struct Gadget *glist=0L;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	ULONG css=0,csm=0,ccs=500,ccm=500;
	UBYTE akttest=0,takt;
	UBYTE runing=TRUE,ret=FALSE;
	UWORD ys1=2;
	UWORD ys2=ys1+1+GetRunTime(scy[140]);
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

	if(!wfkt->desc[0]) RefreshWFktWin(wfkt,0L,0L,xs1,ys1);
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
			ng.ng_Height		=GetRunTime(scy[124]);
			ng.ng_GadgetText	="";
			ng.ng_TextAttr		=GetRunTime(ScrFont);
			ng.ng_GadgetID		=GADID_WFKTSEL;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,&wfktList,GTLV_Selected,0,GTLV_Top,0,GTLV_ShowSelected,0l,TAG_DONE);
			gadgets[GADIX_WFKTSEL]=g;

			ng.ng_LeftEdge		=xs1+GetRunTime(scx[46]);
			ng.ng_TopEdge		=ys1+GetRunTime(scy[127]);
			ng.ng_Width			=GetRunTime(scx[100]);
			ng.ng_Height		=GetRunTime(scy[11]);
			ng.ng_GadgetText	="Par";
			ng.ng_GadgetID		=GADID_WFKTPAR;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_String,0l,GTST_MaxChars,15,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=GetRunTime(stringextend);
			g->Activation|=GACT_STRINGEXTEND;
			gadgets[GADIX_WFKTPAR]=g;

			if(g) {
				wintags[0].ti_Data=(GetRunTime(xres)-winw)>>1;
				wintags[1].ti_Data=(GetRunTime(yres)-winh)>>1;
				wintags[2].ti_Data=winw;
				wintags[3].ti_Data=winh;
				wintags[6].ti_Data=(ULONG)glist;
				wintags[7].ti_Data=(ULONG)LIB_INTCALL(GetString(strSelectWinFunc));
				if(win=OpenWindowTagList(0l,wintags)) {
					GT_RefreshWindow(win,0l);
					wfrp=win->RPort;SetFont(wfrp,GetRunTime(scrfont));

					xh=GetRunTime(scx[294]);
					LIB_INTCALL(DrawGadWB(wfrp,xs1,ys1,xh,GetRunTime(scy[140])));				/* Par */
					LIB_INTCALL(DrawGadWB(wfrp,xs1,ys2,xh,GetRunTime(scy[FRAME_HEIGHT(1)])));	/* Okay */

					LIB_INTCALL(DrawGadBW(wfrp,xs1+GetRunTime(scx[154]),ys1+GetRunTime(scy[ 2]),GetRunTime(scx[128])+3,GetRunTime(scy[64])+3));	/* Window Wave */
					LIB_INTCALL(DrawGadBW(wfrp,xs1+GetRunTime(scx[154]),ys1+GetRunTime(scy[72]),GetRunTime(scx[128])+3,GetRunTime(scy[64])+3));

					RefreshWFktWin(wfkt,win,gadgets,xs1,ys1);

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
									case GADID_WFKTSEL:
										takt=icode;
										if(!akttest) { CurrentTime(&css,&csm);akttest=1; }
										else {
											CurrentTime(&ccs,&ccm);
											if(DoubleClick(css,csm,ccs,ccm) && wfkt->win==takt) {
												runing=FALSE;
												css=0;csm=0;ccs=500;ccm=500;akttest=0;
											}
											else { css=ccs;csm=ccm;ccs=500;ccm=500; }
										}
										wfkt->win=takt;
										switch(wfkt->win) {
											case WIN_BARTLETT:	wfkt->par=0.00;break;
											case WIN_BLACKMAN:	wfkt->par=0.00;break;
											case WIN_FEJER:		wfkt->par=0.00;break;
											case WIN_HALFSINE:	wfkt->par=0.00;break;
											case WIN_HALFSINEQ:	wfkt->par=0.00;break;
											case WIN_HALFSINES:	wfkt->par=0.00;break;
											case WIN_HAMMING:	wfkt->par=0.54;break;
											case WIN_HANNING:	wfkt->par=0.50;break;
											case WIN_KAISER:	wfkt->par=3.30;break;
											case WIN_RECTANGLE:	wfkt->par=0.00;break;
											case WIN_WELCH:		wfkt->par=0.00;break;
										}
										RefreshWFktWin(wfkt,win,gadgets,xs1,ys1);
										break;
									case GADID_WFKTPAR:
										wfkt->par=atof(((struct StringInfo *)(gadgets[GADIX_WFKTPAR]->SpecialInfo))->Buffer);
										RefreshWFktWin(wfkt,win,gadgets,xs1,ys1);
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
							if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP) LIB_INTCALL(ShowHelp("html/nodes/node01.05.05.html"));
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

void RefreshWFktWin(WinFunction *wfkt,struct Window *win,struct Gadget *Gadgets[],UWORD xs1,UWORD ys1) {
	register UWORD i;
	struct RastPort *rp;
	UBYTE akt=wfkt->win;
	char buf[15];
	UBYTE bits;
	ULONG xres1=GetRunTime(scx[128]),xres2,m;
	LONG xoff,yoff;
	double *buf_r,*buf_i=NULL;
	UWORD *brt=0l;
	double *ct=0l,*st=0l;
	double scl,fc,mi,ma,v;

	if(win) {
		GT_SetGadgetAttrs(Gadgets[GADIX_WFKTSEL],win,0l,GTLV_Selected,akt,GTLV_Top,akt,TAG_DONE);
		sprintf(buf,"%10.8lf",wfkt->par);
		switch(wfkt->win) {
			case WIN_BARTLETT:
			case WIN_BLACKMAN:
			case WIN_FEJER:
			case WIN_HALFSINE:
			case WIN_HALFSINEQ:
			case WIN_HALFSINES:
			case WIN_HAMMING:
			case WIN_HANNING:
			case WIN_RECTANGLE:
			case WIN_WELCH:
				GT_SetGadgetAttrs(Gadgets[GADIX_WFKTPAR],win,0l,GTST_String,buf,GA_Disabled,TRUE,TAG_DONE);break;
			case WIN_KAISER:
				GT_SetGadgetAttrs(Gadgets[GADIX_WFKTPAR],win,0l,GTST_String,buf,GA_Disabled,FALSE,TAG_DONE);break;
		}
		bits=1;while(xres1>>bits) bits++;
		xres2=1L<<(bits-1);m=1L<<(bits-2);
		if((buf_r=LIB_INTCALL(NewWindowFkt(xres2,wfkt->win,&wfkt->par)))
		&& (buf_i=(double *)AllocVec(xres2*sizeof(double),MEMF_ANY|MEMF_CLEAR))
		&& LIB_INTCALL(NewFFTTabs(&brt,&st,NULL,&ct,m))) {
			rp=win->RPort;
			SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_BACK]);
			RectFill(rp,xs1+GetRunTime(scx[154])+2,ys1+GetRunTime(scy[ 2])+2,xs1+GetRunTime(scx[154])+GetRunTime(scx[128])+1,ys1+GetRunTime(scy[ 2])+GetRunTime(scy[64])+1);
			RectFill(rp,xs1+GetRunTime(scx[154])+2,ys1+GetRunTime(scy[72])+2,xs1+GetRunTime(scx[154])+GetRunTime(scx[128])+1,ys1+GetRunTime(scy[72])+GetRunTime(scy[64])+1);
			SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_GRID]);
			Move(rp,xs1+GetRunTime(scx[154+64])+2,ys1+GetRunTime(scy[ 2])+2);Draw(rp,xs1+GetRunTime(scx[154)+64]+2,ys1+GetRunTime(scy[ 2+64])+1);
			Move(rp,xs1+GetRunTime(scx[154+64])+2,ys1+GetRunTime(scy[72])+2);Draw(rp,xs1+GetRunTime(scx[154)+64]+2,ys1+GetRunTime(scy[72+64])+1);
			SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_LINE]);

			yoff=ys1+GetRunTime(scy[2])+GetRunTime(scy[64])+1;xoff=xs1+GetRunTime(scx[154])+2;
			scl=GetRunTime(scy[64]);fc=((double)xres2/(double)xres1);
			Move(rp,xoff,yoff);							// draw window
			for(i=0;i<xres1;i++) Draw(rp,xoff+i,yoff-(ULONG)((scl*buf_r[(ULONG)(i*fc)])));
			Draw(rp,xoff+(xres1-1),yoff);

			LIB_INTCALL(Transform(buf_r,buf_i,brt,st,ct,-1,m));
			fc=20.0/log(10.0);
			mi=99999;ma=-99999;
			for(i=0;i<(xres2>>1);i++) {					// convert to db & find max and min
				//v=sqrt(buf_r[i]*buf_r[i]+buf_i[i]*buf_i[i]);
				v=sqrt(buf_r[i]*buf_r[i]);
				if(v<0.00000000001) v=0.00000000001;
				buf_r[i]=v=fc*log(v);
				if(v<mi) mi=v;
				if(v>ma) ma=v;
			}
			scl=(double)GetRunTime(scy[64])/(ma-mi);fc=((double)(xres2>>1)/(double)xres1);
			yoff=ys1+GetRunTime(scy[72])+GetRunTime(scy[64])+1;xoff=xs1+GetRunTime(scx[154])+2;
			//sprintf(__dbgmsg,"mi %6.4lf ma %6.4lf scl %6.4lf",mi,ma,scl);MSG(__dbgmsg);
			//sprintf(__dbgmsg,"xoff %8ld  yoff %8ld",xoff,yoff);MSG(__dbgmsg);
			Move(rp,xoff,yoff);							// draw window
			for(i=0;i<xres1;i++) Draw(rp,xoff+i,yoff-(LONG)((scl*(buf_r[(ULONG)(i*fc)]-mi))));
		}
		else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));
		if(buf_r)	LIB_INTCALL(DelWindowFkt(buf_r));
		if(buf_i)	FreeVec((APTR)buf_i);
		LIB_INTCALL(DelFFTTabs(brt,st,NULL,ct));
	}
	sprintf(wfkt->desc,"%s %10.8lf",WFktLabels[akt],wfkt->par);			// generate String
}

//-- eof ----------------------------------------------------------------------
