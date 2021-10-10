/****h* sfxsupport.library/bshapegui-curve.c [4.3] *
*
*  NAME
*    bshapegui-curve.c
*  COPYRIGHT
*    $VER: bshapegui-curve.c 4.3 (13.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Modulator BlendShapes GUI : Curve - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    26.Nov.1995
*  MODIFICATION HISTORY
*    13.Mar.2004	V 4.3	most recent version
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

#define BSHAPEGUI_CURVE_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator Curve
                                                                 
void ShowWin_ModCurve (Modulator *mod,Source *src1,Source *src2);
void RefreshWin_ModCurve  (struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1);

//-- definitions --------------------------------------------------------------

//-- Modulator Curve

#define GADID_OKAY		0

#define GADID_MCU_EXP	(GADID_OKAY+1)
#define GADIX_MCU_EXP	0

void ShowWin_ModCurve(Modulator *mod,Source *src1,Source *src2) {
	struct NewGadget ng;
	struct Gadget *g;
	struct RastPort *mwrp;
	struct Window *mwwin;
	struct Gadget *mwGadgets[GADIX_MCU_EXP+1];
	struct Gadget *mwGList=NULL;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode,mouseb=FALSE;
	UBYTE quit=FALSE;
	UWORD mousex,mousey;
	UBYTE omptr=MPTR_STANDARD;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(0)+GetRunTime(scy[68]);
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[140]);
	UWORD xh;
	UWORD xs=xs1+GetRunTime(scx[5])+1,ys=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET])+1;
	UWORD xe=xs1+GetRunTime(scx[5])+GetRunTime(scx[128])+2,ye=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET])+GetRunTime(scy[64])+2;
	double mx,my;
	Param param;
	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
		WA_Gadgets,			0l,
		WA_Title,			(ULONG)"Curve",
		WA_ScreenTitle,		(ULONG)VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};
	ModCurve *mc=(ModCurve *)(mod->bshpdata);

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
		ng.ng_GadgetText	="Exp";
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetID		=GADID_MCU_EXP;
		ng.ng_Flags			=PLACETEXT_LEFT;
		sprintf(param,"%f",mc->exp);
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,param,GTST_MaxChars,15,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=GetRunTime(stringextend);
		g->Activation|=GACT_STRINGEXTEND;
		mwGadgets[GADIX_MCU_EXP]=g;

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
				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys2,xh,ANYFRM_HEIGHT(1)));					/* Par */
				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys3,xh,CMDFRM_HEIGHT));						/* Okay */

				LIB_INTCALL(DrawGadBW(mwrp,xs1+GetRunTime(scx[5]),ys1+GetRunTime(scy[FRMCONTENT_YOFFSET]),GetRunTime(scx[128])+3,GetRunTime(scy[64])+3));	 /* Curve */

				RefreshWin_ModCurve(mwrp,mod,src1,src2,xs1,ys1);

				while(!quit) {
					WaitPort(mwwin->UserPort);
					while(imsg=GT_GetIMsg(mwwin->UserPort)) {
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						mousex	=imsg->MouseX-mwwin->BorderLeft;
						mousey	=imsg->MouseY-mwwin->BorderTop;
						GT_ReplyIMsg(imsg);
						switch(iclass) {
							case IDCMP_GADGETUP:
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_OKAY:
										quit=1;break;
									case GADID_MCU_EXP:
										mc->exp=atof(((struct StringInfo *)(mwGadgets[GADIX_MCU_EXP]->SpecialInfo))->Buffer);
										if(mc->exp<=0.001) {
											mc->exp=0.001;
											sprintf(param,"%f",mc->exp);
											GT_SetGadgetAttrs(mwGadgets[GADIX_MCU_EXP],mwwin,0l,GTST_String,param,TAG_DONE);
										}
										RefreshWin_ModCurve(mwrp,mod,src1,src2,xs1,ys1);
										break;
								}
								break;
							case IDCMP_MOUSEBUTTONS:
								if(icode==SELECTDOWN) mouseb=TRUE;
								if(icode==SELECTUP) mouseb=FALSE; /* No break !!! */
							case IDCMP_MOUSEMOVE:
								if((mousex>=xs) && (mousex<=xe) && (mousey>=ys) && (mousey<=ye)) {
									if(mouseb) {
										//mc->exp=((double)(mousex-xs))/GetRunTime(scx[128])+((double)(mousey-ys))/GetRunTime(scy[64]);
										my=1.0-(((double)(mousey-ys))/GetRunTime(scy[64]));if(my>1.0) my=1.0;
										mx=     ((double)(mousex-xs))/GetRunTime(scx[128]);if(mx>1.0) mx=1.0;
										if(mx>0.0 && my<1.0 && my>0.0) {
											mc->exp=log(my)/log(mx);
											if(mc->exp>400.0) mc->exp=400.0;
											sprintf(param,"%f",mc->exp);
											GT_SetGadgetAttrs(mwGadgets[GADIX_MCU_EXP],mwwin,0l,GTST_String,param,TAG_DONE);
											RefreshWin_ModCurve(mwrp,mod,src1,src2,xs1,ys1);
										}
									}
									if(omptr!=MPTR_MOVE) { SetWindowPointer(mwwin,WA_Pointer,GetRunTime(MPtrMove)->Pointer,TAG_DONE);omptr=MPTR_MOVE; }
								}
								else {
									if(omptr!=MPTR_STANDARD) { SetWindowPointer(mwwin,TAG_DONE);omptr=MPTR_STANDARD; }
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
										ActivateGadget(mwGadgets[GADIX_MCU_EXP],mwwin,0l);
										break;
									case KEY_ESC:
										quit=2;break;
								}
								break;
							case IDCMP_CLOSEWINDOW:
								quit=2;break;
							case IDCMP_RAWKEY:
								if(icode==RKEY_HELP)
									LIB_INTCALL(ShowHelp("html/nodes/node01.07.01.html"));
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
	if(quit!=2)	Describe_ModCurve(mod);
}

void RefreshWin_ModCurve(struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1) {
	register UWORD i;
	double fc=GetRunTime(scy[64]);
	LONG xoff,yoff;
	ULONG xres1=GetRunTime(scx[128]);

	LIB_INTCALL(AssignBShape(mod));

	yoff=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET])+GetRunTime(scy[64])+1;xoff=xs1+GetRunTime(scx[5])+2;

	SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_BACK]);
	RectFill(rp,xoff,ys1+GetRunTime(scy[FRMCONTENT_YOFFSET])+2,xs1+GetRunTime(scx[5])+GetRunTime(scx[128])+1,yoff);

	SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_GRID]);
	Move(rp,xoff				 ,yoff-GetRunTime(scy[32]));Draw(rp,xoff+GetRunTime(scx[128]),yoff-GetRunTime(scy[32]));
	Move(rp,xoff+GetRunTime(scx[64]),yoff-GetRunTime(scy[64]));Draw(rp,xoff+GetRunTime(scx[64]) ,yoff);

	SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_LINE]);
	Move(rp,xoff,yoff-(ULONG)(fc*mod->modfunc(0,xres1,(void *)mod->bshpdata)));							// draw curve
	for(i=0;i<xres1;i++) Draw(rp,xoff+i,yoff-(ULONG)(fc*mod->modfunc(i,xres1,(void *)mod->bshpdata)));
}

//-- eof ----------------------------------------------------------------------
