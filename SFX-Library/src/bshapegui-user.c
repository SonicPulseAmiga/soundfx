/****h* sfxsupport.library/bshapegui-user.c [4.2] *
*
*  NAME
*    bshapegui-user.c
*  COPYRIGHT
*    $VER: bshapegui-user.c 4.2 (07.07.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Modulator BlendShapes GUI : User - definitions
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

#define BSHAPEGUI_USER_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator User

void ShowWin_ModUser	(Modulator *mod,Source *src1,Source *src2);
void RefreshWin_ModUser	(struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1);

//-- definitions --------------------------------------------------------------

//-- Modulator User

#define GADID_OKAY		0

#define GADID_MUS_SRCSEL	(GADID_OKAY+1)
#define GADIX_MUS_SRCSEL	0

#define GADID_MUS_SRC		(GADID_MUS_SRCSEL+1)
#define GADIX_MUS_SRC		(GADIX_MUS_SRCSEL+1)

#define GADID_MUS_TYPE		(GADID_MUS_SRC+1)
#define GADIX_MUS_TYPE		(GADIX_MUS_SRC+1)

#define GADID_MUS_MODE		(GADID_MUS_TYPE+1)

#define GADID_MUS_ENV		(GADID_MUS_MODE+1)
#define GADIX_MUS_ENV		(GADIX_MUS_TYPE+1)

void ShowWin_ModUser(Modulator *mod,Source *src1,Source *src2) {
	struct NewGadget ng;
	struct Gadget *g;
	struct RastPort *mwrp;
	struct Window *mwwin;
	struct Gadget *mwGadgets[GADIX_MUS_ENV+1];
	struct Gadget *mwGList=NULL;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	UBYTE quit=0;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(1);
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(2);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[290]);
	UWORD xh;
	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS,
		WA_Gadgets,			0l,
		WA_Title,			(ULONG)"User",
		WA_ScreenTitle,		(ULONG)VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};
	ModUser *mu=(ModUser *)(mod->bshpdata);
	SInfo *tbuf;
	UBYTE *EmptyLabels[]={ " ",0l };
	struct Requester NullReq;

	if(g=CreateContext(&mwGList)) {
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[105]);
		ng.ng_TopEdge		=ys3+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[80]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=GetRunTime(ScrFontB);
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		if(GetRunTime(sfxprefs_misc)->srcselect) {
			ng.ng_LeftEdge		=xs1+GetRunTime(scx[37]);
			ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET]);
			ng.ng_Width			=GetRunTime(scx[248]);
			ng.ng_TextAttr		=GetRunTime(ScrFont);
			ng.ng_GadgetText	="";
			ng.ng_GadgetID		=GADID_MUS_SRCSEL;
			ng.ng_Flags			=PLACETEXT_IN;
			ng.ng_VisualInfo	=GetRunTime(VisualInfo);
			g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,EmptyLabels,GTCY_Active,0,GA_Disabled,TRUE,TAG_DONE);
			mwGadgets[GADIX_MUS_SRCSEL]=g;
			mwGadgets[GADIX_MUS_SRC]=0;
		}
		else {
			ng.ng_LeftEdge		=xs1+GetRunTime(scx[37]);
			ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET]);
			ng.ng_Width			=GetRunTime(scx[15]);
			ng.ng_TextAttr		=GetRunTime(SigFont);
			ng.ng_GadgetText	=IMAGE_PopUp;
			ng.ng_GadgetID		=GADID_MUS_SRCSEL;
			ng.ng_Flags			=PLACETEXT_IN;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
			mwGadgets[GADIX_MUS_SRCSEL]=g;

			ng.ng_LeftEdge		=xs1+GetRunTime(scx[53]);
			ng.ng_Width			=GetRunTime(scx[232]);
			ng.ng_TextAttr		=GetRunTime(ScrFont);
			ng.ng_GadgetText	="";
			ng.ng_GadgetID		=GADID_MUS_SRC;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,0l,GTTX_CopyText,TRUE,TAG_DONE);
			mwGadgets[GADIX_MUS_SRC]=g;
		}

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[53]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[FRMCONTENT_YOFFSET]);
		ng.ng_Width			=GetRunTime(scx[90]);
		ng.ng_GadgetText	="Type";
		ng.ng_GadgetID		=GADID_MUS_TYPE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,TypeLabels,GTCY_Active,mu->type,TAG_DONE);
		mwGadgets[GADIX_MUS_TYPE]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[195]);
		ng.ng_GadgetText	="Mode";
		ng.ng_GadgetID		=GADID_MUS_MODE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ModeLabels,GTCY_Active,mu->bmode,TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[53]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[FRMCONTENT_YOFFSET+12]);
		ng.ng_GadgetText	="Env";
		ng.ng_GadgetID		=GADID_MUS_ENV;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,EmptyLabels,GTCY_Active,0,GA_Disabled,TRUE,TAG_DONE);
		mwGadgets[GADIX_MUS_ENV]=g;

		if(g) {
			wintags[0].ti_Data=(GetRunTime(xres)-winw)>>1;
			wintags[1].ti_Data=(GetRunTime(yres)-winh)>>1;
			wintags[2].ti_Data=winw;
			wintags[3].ti_Data=winh;
			wintags[6].ti_Data=(ULONG)mwGList;
			if(mwwin=OpenWindowTagList(0l,wintags)) {
				GT_RefreshWindow(mwwin,0l);
				mwrp=mwwin->RPort;SetFont(mwrp,GetRunTime(scrfont));

				SetAPen(mwrp,1);
				LIB_INTCALL(RText(mwrp,"Src",xs1+GetRunTime(scx[29]),ys1+GetRunTime(scy[FRMTEXT_YPOS)(1)]));

				xh=GetRunTime(scx[290]);

				LIB_INTCALL(DrawTitle(mwrp,"Source"       ,xs1,ys1,xh));
				LIB_INTCALL(DrawTitle(mwrp,"Parameters"   ,xs1,ys2,xh));

				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys1,xh,ANYFRM_HEIGHT(1)));	/* Src */
				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys2,xh,ANYFRM_HEIGHT(2)));	/* Par */
				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys3,xh,CMDFRM_HEIGHT));		/* Okay */

				RefreshWin_ModUser(mwrp,mod,src1,src2,xs1,ys1);

				if(mu->modbuf) {
					if(!mwGadgets[GADIX_MUS_SRC]) {
						GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_SRCSEL],mwwin,0l,GTCY_Labels,GetRunTime(barray),GTCY_Active,GetIndexByNode(GetRunTime(blist),(struct Node *)(mu->modbuf)),GA_Disabled,FALSE,TAG_DONE);
					}
					else {
						GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_SRC],mwwin,0l,GTTX_Text,((struct Node *)(mu->modbuf))->ln_Name,TAG_DONE);
					}
				}
				switch(mu->type) {
					case MU_TYPE_NORMAL:
					case MU_TYPE_ABSOLUT:
						GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_ENV],mwwin,0l,GTCY_Labels,EmptyLabels,GTCY_Active,0,GA_Disabled,TRUE,TAG_DONE);
						break;
					case MU_TYPE_AMPENV:
						GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_ENV],mwwin,0l,GTCY_Labels,EnvLabels[mu->type-MU_TYPE_AMPENV],GTCY_Active,mu->env,GA_Disabled,FALSE,TAG_DONE);
						break;
					case MU_TYPE_FRQENV:
						GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_ENV],mwwin,0l,GTCY_Labels,EnvLabels[mu->type-MU_TYPE_AMPENV],GTCY_Active,mu->env,GA_Disabled,FALSE,TAG_DONE);
						break;
				}

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
									case GADID_MUS_SRCSEL:
										if(!mwGadgets[GADIX_MUS_SRC]) {
											if(icode!=GetIndexByNode(GetRunTime(blist),(struct Node *)(mu->modbuf))) {
												if(tbuf=(SInfo *)GetNodeByIndex(GetRunTime(blist),icode)) {
													Forbid();							// we only accept VM_MEMORY Samples as Modulators
													if(tbuf->storage==VM_MEMORY) {
														LIB_INTCALL(UnlockBuffer(mu->modbuf));
														mu->modbuf=LIB_INTCALL(LockBuffer(tbuf));
														mu->modid=mu->modbuf->id;
													}
													Permit();
												}
											}
										}
										else {
											InitRequester(&NullReq);Request(&NullReq,mwwin);SetWindowPointer(mwwin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											if(tbuf=LIB_INTCALL(GetListEntry())) {
												Forbid();							// we only accept VM_MEMORY Samples as Modulators
												if(tbuf->storage==VM_MEMORY) {
													LIB_INTCALL(UnlockBuffer(mu->modbuf));
													mu->modbuf=LIB_INTCALL(LockBuffer(tbuf));
													mu->modid=mu->modbuf->id;
													GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_SRC],mwwin,0l,GTTX_Text,((struct Node *)(mu->modbuf))->ln_Name,TAG_DONE);
												}
												Permit();
											}
											EndRequest(&NullReq,mwwin);SetWindowPointer(mwwin,TAG_DONE);
										}
										break;
									case GADID_MUS_TYPE:
										if(mu->type!=icode) {
											if(mu->envdata) { FreeVec(mu->envdata);mu->envdata=NULL; }
										}
										mu->type=icode;
										switch(mu->type) {
											case MU_TYPE_NORMAL:
											case MU_TYPE_ABSOLUT:
												GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_ENV],mwwin,0l,GTCY_Labels,EmptyLabels,GA_Disabled,TRUE,TAG_DONE);
												break;
											case MU_TYPE_AMPENV:
												GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_ENV],mwwin,0l,GTCY_Labels,EnvLabels[mu->type-MU_TYPE_AMPENV],GTCY_Active,mu->env,GA_Disabled,FALSE,TAG_DONE);
												if(!(mu->envdata=AllocVec(sizeof(AmpEnvMaxDecay),MEMF_ANY|MEMF_CLEAR)))
													GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_TYPE],mwwin,0l,GTCY_Active,(mu->type=MU_TYPE_NORMAL),TAG_DONE);
												break;
											case MU_TYPE_FRQENV:
												GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_ENV],mwwin,0l,GTCY_Labels,EnvLabels[mu->type-MU_TYPE_AMPENV],GTCY_Active,mu->env,GA_Disabled,FALSE,TAG_DONE);
												if(!(mu->envdata=AllocVec(sizeof(FrqEnvPeakSearch),MEMF_ANY|MEMF_CLEAR)))
													GT_SetGadgetAttrs(mwGadgets[GADIX_MUS_TYPE],mwwin,0l,GTCY_Active,(mu->type=MU_TYPE_NORMAL),TAG_DONE);
												break;
										}
										break;
									case GADID_MUS_MODE:
										mu->bmode=icode;
										break;
									case GADID_MUS_ENV:
										mu->env=icode;
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
									case KEY_ESC:
										quit=2;break;
								}
								break;
							case IDCMP_CLOSEWINDOW:
								quit=2;break;
							case IDCMP_RAWKEY:
								if(icode==RKEY_HELP)
									LIB_INTCALL(ShowHelp("html/nodes/node01.07.04.html"));
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
	if(quit!=2) Describe_ModUser(mod);
}

void RefreshWin_ModUser(struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1) {
}

//-- eof ----------------------------------------------------------------------
