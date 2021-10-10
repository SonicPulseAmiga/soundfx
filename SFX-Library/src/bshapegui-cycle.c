/****h* sfxsupport.library/bshapegui-cycle.c [4.2] *
*
*  NAME
*    bshapegui-cycle.c
*  COPYRIGHT
*    $VER: bshapegui-cycle.c 4.2 (07.07.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Modulator BlendShapes GUI : Cycle - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    26.Nov.1995
*  MODIFICATION HISTORY
*    07.Jul.2003	V 4.2	most recent version
*    25.Apr.2001	V 4.1	most recent version
*							C: ModWin*(),RefreshMW*() naming
*							A: Describe*() functions
*							A: User_FrqEnv_Energy (beta)
*							C: BShape naming
*    31.May.2000	V 1.30	most recent version
*    29.Apr.1999	V 1.20	most recent version
*    26.Nov.1995	V 1.00	initial version
*  IDEAS
*  NOTES
*
*******
*/

#define BSHAPEGUI_CYCLE_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator Cycle

void ShowWin_ModCycle (Modulator *mod,Source *src1,Source *src2);
void RefreshWin_ModCycle  (struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1);

//-- definitions --------------------------------------------------------------

//-- Modulator Cycle

#define GADID_OKAY		0

#define GADID_MCY_OSZI	(GADID_OKAY+1)

#define GADID_MCY_FRQ	(GADID_MCY_OSZI+1)
#define GADIX_MCY_FRQ	0

#define GADID_MCY_PHA	(GADID_MCY_FRQ+1)
#define GADIX_MCY_PHA	(GADIX_MCY_FRQ+1)

void ShowWin_ModCycle(Modulator *mod,Source *src1,Source *src2) {
	struct NewGadget ng;
	struct Gadget *g;
	struct RastPort *mwrp;
	struct Window *mwwin;
	struct Gadget *mwGadgets[GADIX_MCY_PHA+1];
	struct Gadget *mwGList=NULL;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	UBYTE quit=0;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(0)+GetRunTime(scy[68]);
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(3);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[140]);
	UWORD xh;
	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS,
		WA_Gadgets,			0l,
		WA_Title,			(ULONG)"Cycle",
		WA_ScreenTitle,		(ULONG)VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};
	ModCycle *mc=(ModCycle *)(mod->bshpdata);

	if(g=CreateContext(&mwGList)) {
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[29]);
		ng.ng_TopEdge		=ys3+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[80]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=GetRunTime(ScrFontB);
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[44]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[FRMCONTENT_YOFFSET]);
		ng.ng_GadgetText	="Osci";
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetID		=GADID_MCY_OSZI;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,OsziLabels,GTCY_Active,mc->oszi,TAG_DONE);

		ng.ng_TopEdge		=ys2+GetRunTime(scy[FRMCONTENT_YOFFSET+12]);
		ng.ng_GadgetText	="Frq";
		ng.ng_GadgetID		=GADID_MCY_FRQ;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,mc->frq,GTST_MaxChars,PARBUFLEN,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=GetRunTime(stringextend);
		g->Activation|=GACT_STRINGEXTEND;
		mwGadgets[GADIX_MCY_FRQ]=g;

		ng.ng_TopEdge		=ys2+GetRunTime(scy[FRMCONTENT_YOFFSET+24]);
		ng.ng_GadgetText	="Pha";
		ng.ng_GadgetID		=GADID_MCY_PHA;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,mc->pha,GTST_MaxChars,PARBUFLEN,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=GetRunTime(stringextend);
		g->Activation|=GACT_STRINGEXTEND;
		mwGadgets[GADIX_MCY_PHA]=g;

		if(g) {
			wintags[0].ti_Data=(GetRunTime(xres)-winw)>>1;
			wintags[1].ti_Data=(GetRunTime(yres)-winh)>>1;
			wintags[2].ti_Data=winw;
			wintags[3].ti_Data=winh;
			wintags[6].ti_Data=(ULONG)mwGList;
			if(mwwin=OpenWindowTagList(0l,wintags)) {
				GT_RefreshWindow(mwwin,0l);
				mwrp=mwwin->RPort;SetFont(mwrp,GetRunTime(scrfont));

				xh=GetRunTime(scx[140]);

				LIB_INTCALL(DrawTitle(mwrp,"Wave"         ,xs1,ys1,xh));
				LIB_INTCALL(DrawTitle(mwrp,"Parameters"   ,xs1,ys2,xh));

				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys1,xh,ANYFRM_HEIGHT(0)+GetRunTime(scy[68])));	/* Gfx */
				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys2,xh,ANYFRM_HEIGHT(3)));					/* Par */
				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys3,xh,CMDFRM_HEIGHT));	/* Okay */

				LIB_INTCALL(DrawGadBW(mwrp,xs1+GetRunTime(scx[5]),ys1+GetRunTime(scy[FRMCONTENT_YOFFSET]),GetRunTime(scx[128])+3,GetRunTime(scy[64])+3));	 /* Curve */

				RefreshWin_ModCycle(mwrp,mod,src1,src2,xs1,ys1);

				while(!quit) {
					WaitPort(mwwin->UserPort);
					while(imsg=GT_GetIMsg(mwwin->UserPort)) {
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						switch(iclass) {
							case IDCMP_GADGETUP:
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_OKAY:
										quit=1;break;
									case GADID_MCY_OSZI:
										mc->oszi=icode;
										RefreshWin_ModCycle(mwrp,mod,src1,src2,xs1,ys1);
										break;
									case GADID_MCY_FRQ:
										LIB_INTCALL(HandleParameter(mc->frq,mwwin,mwGadgets[GADIX_MCY_FRQ]));
										RefreshWin_ModCycle(mwrp,mod,src1,src2,xs1,ys1);
										break;
									case GADID_MCY_PHA:
										LIB_INTCALL(HandleParameter(mc->pha,mwwin,mwGadgets[GADIX_MCY_PHA]));
										RefreshWin_ModCycle(mwrp,mod,src1,src2,xs1,ys1);
										break;
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									case KEY_ENTER:
									case KEY_RETURN:
									case 'O':
									case 'o':
										quit=1;break;
									case KEY_TAB:
										ActivateGadget(mwGadgets[GADIX_MCY_FRQ],mwwin,0l);
										break;
									case KEY_ESC:
										quit=2;break;
								}
								break;
							case IDCMP_CLOSEWINDOW:
								quit=2;break;
							case IDCMP_RAWKEY:
								if(icode==RKEY_HELP)
									LIB_INTCALL(ShowHelp("html/nodes/node01.07.02.html"));
								break;
						}
					}
				}
				CloseWindow(mwwin);
			}
			else LIB_INTCALL(Message(errOpenWindow,NULL,__FILE__,__LINE__));
			FreeGadgets(mwGList);
		}
		else LIB_INTCALL(Message(errCreateGadgets,NULL,__FILE__,__LINE__));
	}
	else LIB_INTCALL(Message(errCreateContext,NULL,__FILE__,__LINE__));
	if(quit!=2) Describe_ModCycle(mod);
}

void RefreshWin_ModCycle(struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1) {
	register UWORD i;
	double fc=(GetRunTime(scy[64])-1.0);
	LONG xoff,yoff;
	ULONG xres1=GetRunTime(scx[128]);
	ULONG oslen=mod->slen,osrat=mod->srat;
	Param oparam;
	ModCycle *mcy=(ModCycle *)(mod->bshpdata);

	strcpy(oparam,mcy->frq);						// Alternate setting, so that the curve fits into preview window
	if((UC_GetParameterUnit(mcy->frq,UGRP_ABSFREQUENCY)!=INVALID_UNIT) || (UC_GetParameterUnit(mcy->frq,UGRP_ABSTIME)!=INVALID_UNIT)) {
		if((mcy->oszi!=MC_OSZI_RND) && (mcy->oszi!=MC_OSZI_SRND)) sprintf(mcy->frq,"%ld sv",xres1);
		else sprintf(mcy->frq,"%ld sv",(ULONG)(xres1/10.0));
	}
	mod->slen=xres1;mod->srat=xres1;
	LIB_INTCALL(AssignBShape(mod));

	yoff=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET])+GetRunTime(scy[64])+1;xoff=xs1+GetRunTime(scx[5])+2;

	SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_BACK]);
	RectFill(rp,xoff,ys1+GetRunTime(scy[FRMCONTENT_YOFFSET])+2,xs1+GetRunTime(scx[5])+GetRunTime(scx[128])+1,yoff);

	SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_GRID]);
	Move(rp,xoff				 ,yoff-GetRunTime(scy[32]));Draw(rp,xoff+GetRunTime(scx[128]),yoff-GetRunTime(scy[32]));
	//Move(rp,xoff+GetRunTime(scx[64]),yoff-GetRunTime(scy[64]));Draw(rp,xoff+GetRunTime(scx[64]) ,yoff);

	SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_LINE]);
	Move(rp,xoff,yoff-(ULONG)(fc*mod->modfunc(0,xres1,(void *)mod->bshpdata)));	// draw curve
	for(i=0;i<xres1;i++) Draw(rp,xoff+i,yoff-(ULONG)(fc*mod->modfunc(i,xres1,(void *)mod->bshpdata)));

	mod->slen=oslen;mod->srat=osrat;				// Restore settings
	strcpy(mcy->frq,oparam);
}

//-- eof ----------------------------------------------------------------------
