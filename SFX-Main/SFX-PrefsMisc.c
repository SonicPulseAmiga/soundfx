/****h*SoundFX/SFX-PrefsMisc.c [4.3] *
*
*  NAME
*    SFX-PrefsMisc.c
*  COPYRIGHT
*    $VER: SFX-PrefsMisc.c 4.3 (28.01.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    miscelaneous options window - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    28.Jan.2004	V 4.3	most recent version
*                           NEW : askexit 
*    03.Jun.2002	V 4.2	most recent version
*    11.Jan.2001	V 4.1	most recent version
*                           NEW : logmode, logpath
*    16.Jul.2000	V 4.0	most recent version
*							NEW : contextbutton
*    26.Jan.2000	V 3.82	most recent version
*    27.Apr.1999	V 3.81	most recent version
*							A: srcselect-prefs
*							C: layout
*    24.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_PrefsMisc_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE PrefsMisc(void);

//-- defines ------------------------------------------------------------------

#define GADID_OKAY			(0)

#define GADID_SIGAUD		(GADID_OKAY+1)				// Signals, when operation is finished
#define GADIX_SIGAUD		(0) 

#define GADID_SIGSCR		(GADID_SIGAUD+1)
#define GADIX_SIGSCR		(GADIX_SIGAUD+1)

#define GADID_REALMEM		(GADID_SIGSCR+1)			// Screentitle
#define GADIX_REALMEM		(GADIX_SIGSCR+1)

#define GADID_VIRTMEM		(GADID_REALMEM+1)
#define GADIX_VIRTMEM		(GADIX_REALMEM+1)

#define GADID_LOGO			(GADID_VIRTMEM+1)			// Backgroudlogo ?
#define GADIX_LOGO			(GADIX_VIRTMEM+1)

#define GADID_SRCSELECT		(GADID_LOGO+1)				// sourceselect mode (list,cycle)

#define GADID_CONTEXTBUTTON	(GADID_SRCSELECT+1)			// context-menu mouse-button (middle,right)

#define GADID_WINDOWMODE	(GADID_CONTEXTBUTTON+1)		// window mode (multi,single)

#define GADID_ASKEXIT		(GADID_WINDOWMODE+1)		// ask when exiting (never,loaded,unsaved,always)

#define GADID_LOGMODE		(GADID_ASKEXIT+1)			// logging (never,quiete-mode,always)

#define GADID_LOGPOPUP		(GADID_LOGMODE+1)
#define GADIX_LOGPOPUP		(GADIX_LOGO+1)

#define GADID_LOG			(GADID_LOGPOPUP+1)
#define GADIX_LOG			(GADIX_LOGPOPUP+1)

#define GADID_SAVEICONS     (GADID_LOG+1)				// save icons with samples
#define GADIX_SAVEICONS		(GADIX_LOG+1)

#define GADID_DEFTOOL		(GADID_SAVEICONS+1)			// how to handle the def-tool

#define LAST_GADIX			(GADIX_SAVEICONS+1)

//-- globals ------------------------------------------------------------------

UBYTE *SrcSelLabels[]={ "List","Cycle",NULL };
UBYTE *ContextButtonLabels[]={ "middle","right",NULL };
UBYTE *AskExitLabels[]={ "never","loaded samples","unsaved samples","always",NULL };
UBYTE *WindowModeLabels[]={ "multi","single",NULL };
UBYTE *LogModeLabels[]={ "never","quiet mode","always",NULL };
UBYTE *DefToolLabels[]={ "keep","set if no def.tool","always set",NULL };

//-- definitions --------------------------------------------------------------

UBYTE PrefsMisc(void)
{
	struct NewGadget ng;
	struct Window *prwin;
	struct Gadget *g;
	struct Gadget *prGList=NULL;
	struct Gadget *prGadgets[LAST_GADIX];
	struct RastPort *prrp,*bdrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	UBYTE quit=0;
	SFXPrefs_Misc	tprefs_misc;
	UBYTE obacklogo=sfxprefs_misc.backlogo;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(1);		// signal; title
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(5);		// logo, srcselect, contextbutton, askexit, windowmode; logging
	UWORD ys4=ys2+1+ANYFRM_HEIGHT(2);		// sample icons
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+RunTime.scx[200];
	UWORD winw=xs2+3+RunTime.scx[200];
	UWORD xh;
	struct Node *node;

	tprefs_misc	=sfxprefs_misc;

	if(g=CreateContext(&prGList)) {
		ng.ng_LeftEdge		=xs1+RunTime.scx[160];
		ng.ng_TopEdge		=ys3+RunTime.scy[2];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=RunTime.ScrFontB;
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[70];					/* Signal */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[26];
		ng.ng_GadgetText	="Sig-Aud";
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetID		=GADID_SIGAUD;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_misc.sig_a,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_SIGAUD]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[170];
		ng.ng_GadgetText	="Sig-Scr";
		ng.ng_GadgetID		=GADID_SIGSCR;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_misc.sig_s,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_SIGSCR]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[70];					/* Titlebar */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_GadgetText	="Real-Mem";
		ng.ng_GadgetID		=GADID_REALMEM;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_misc.realmem,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_REALMEM]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[170];
		ng.ng_GadgetText	="Virt-Mem";
		ng.ng_GadgetID		=GADID_VIRTMEM;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_misc.virtmem,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_VIRTMEM]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[170];					/* Backlogo */
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_GadgetText	="Background Logo";
		ng.ng_GadgetID		=GADID_LOGO;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_misc.backlogo,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_LOGO]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[110];					/* Sourceselect */
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime.scx[86];
		ng.ng_GadgetText	="Source Select";
		ng.ng_GadgetID		=GADID_SRCSELECT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,SrcSelLabels,GTCY_Active,sfxprefs_misc.srcselect,TAG_DONE);

		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+24];/* context-menu mouse-button */
		ng.ng_GadgetText	="Context Button";
		ng.ng_GadgetID		=GADID_CONTEXTBUTTON;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ContextButtonLabels,GTCY_Active,sfxprefs_misc.contextbutton,TAG_DONE);

		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+36];/* window mode */
		ng.ng_GadgetText	="Window Mode";
		ng.ng_GadgetID		=GADID_WINDOWMODE;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,WindowModeLabels,GTCY_Active,sfxprefs_misc.onewindow,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[70];					/* ask exit */
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+48];
		ng.ng_Width			=RunTime.scx[126];
		ng.ng_GadgetText	="Ask Exit";
		ng.ng_GadgetID		=GADID_ASKEXIT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,AskExitLabels,GTCY_Active,sfxprefs_misc.askexit,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime.scx[110];					/* logging mode */
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[86];
		ng.ng_GadgetText	="LogMode";
		ng.ng_GadgetID		=GADID_LOGMODE;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,LogModeLabels,GTCY_Active,sfxprefs_misc.logmode,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime.scx[59];					/* Logpath */
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime.scx[15];
		ng.ng_TextAttr		=RunTime.SigFont;
		ng.ng_GadgetText	=IMAGE_DirReq;
		ng.ng_GadgetID		=GADID_LOGPOPUP;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		prGadgets[GADIX_LOGPOPUP]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[75];
		ng.ng_Width			=RunTime.scx[121];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_LOG;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,SFXPrefs_Misc_LogPathLen-1,GTST_String,sfxprefs_misc.log_path,TAG_DONE);
		prGadgets[GADIX_LOG]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[170];					/* save icons */
		ng.ng_TopEdge		=ys4+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[26];
		ng.ng_GadgetText	="Save Icons";
		ng.ng_GadgetID		=GADID_SAVEICONS;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_misc.saveicons,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_SAVEICONS]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[75];				   /* def-tool mode */
		ng.ng_TopEdge		=ys4+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime.scx[121];
		ng.ng_GadgetText	="DefTool";
		ng.ng_GadgetID		=GADID_DEFTOOL;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,DefToolLabels,GTCY_Active,sfxprefs_misc.deftool,TAG_DONE);

		if(g) {
			MemUsage(sfxtitle);
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)prGList;
			Dialog_Wintags_Title		=(ULONG)GetString(menPrefsMisc);
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(prwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(prwin,NULL);

				prrp=prwin->RPort;SetFont(prrp,RunTime.scrfont);

				SetAPen(prrp,1);
				RText(prrp,"LogPath",xs2+RunTime.scx[51],ys2+RunTime.scy[FRMTEXT_YPOS(2)]);

				xh=RunTime.scx[200];
				DrawTitle(prrp,"Signals"		,xs1,ys1,xh);
				DrawTitle(prrp,"Memory Status"	,xs2,ys1,xh);
				DrawTitle(prrp,"Miscellaneous"	,xs1,ys2,xh);
				DrawTitle(prrp,"Logging"		,xs2,ys2,xh);
				DrawTitle(prrp,"Sample Icons"	,xs2,ys4,xh);

				DrawGadWB(prrp,xs1,ys1,xh,ANYFRM_HEIGHT(1));		/* Signal */
				DrawGadWB(prrp,xs2,ys1,xh,ANYFRM_HEIGHT(1));		/* Titlebar */
				DrawGadWB(prrp,xs1,ys2,xh,ANYFRM_HEIGHT(5));		/* Backlogo, Sourceselect, context-menu mouse-button, askexit */
				DrawGadWB(prrp,xs2,ys2,xh,ANYFRM_HEIGHT(2));		/* Logging */
				DrawGadWB(prrp,xs2,ys4,xh,ANYFRM_HEIGHT(2)+3);		/* Sample Icons */
				xh=RunTime.scx[200]+RunTime.scx[200]+1;
				DrawGadWB(prrp,xs1,ys3,xh,CMDFRM_HEIGHT);			/* Okay */

				if(sfxprefs_misc.logmode==LOGMODE_NEVER) {
					GT_SetGadgetAttrs(prGadgets[GADIX_LOGPOPUP],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(prGadgets[GADIX_LOG     ],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
				}

				while(!quit) {
					WaitPort(prwin->UserPort);
					while(imsg=GT_GetIMsg(prwin->UserPort)) {
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						if(iclass==IDCMP_GADGETUP) {
							switch((int)(((struct Gadget *)iadr)->GadgetID)) {
								case GADID_OKAY:
									quit=1;
									break;
								case GADID_SIGAUD:			sfxprefs_misc.sig_a			=((prGadgets[GADIX_SIGAUD ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_SIGSCR:			sfxprefs_misc.sig_s			=((prGadgets[GADIX_SIGSCR ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_REALMEM:			sfxprefs_misc.realmem		=((prGadgets[GADIX_REALMEM]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_VIRTMEM:			sfxprefs_misc.virtmem		=((prGadgets[GADIX_VIRTMEM]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_LOGO:			sfxprefs_misc.backlogo		=((prGadgets[GADIX_LOGO   ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_SRCSELECT:		sfxprefs_misc.srcselect		=icode;break;
								case GADID_CONTEXTBUTTON:	sfxprefs_misc.contextbutton	=icode;
									foreach(node,&BufferList) {								/* update all visible sample-windows */
										if(SFX_SOUND_IS_SHOWN((SInfo *)node) && (((SInfo *)node)->win)) {
											if(RunTime.sfxprefs_misc->contextbutton==1) ((SInfo *)node)->win->Flags|=WFLG_RMBTRAP;
											else ((SInfo *)node)->win->Flags&=~WFLG_RMBTRAP;
										}
									}	
									break;
								case GADID_WINDOWMODE:		sfxprefs_misc.onewindow		=icode;break;
								case GADID_ASKEXIT:			sfxprefs_misc.askexit		=icode;break;
								case GADID_LOGMODE:
									sfxprefs_misc.logmode=icode;
									if(sfxprefs_misc.logmode==LOGMODE_NEVER) {
										GT_SetGadgetAttrs(prGadgets[GADIX_LOGPOPUP],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
										GT_SetGadgetAttrs(prGadgets[GADIX_LOG     ],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
									}
									else {
										GT_SetGadgetAttrs(prGadgets[GADIX_LOGPOPUP],prwin,NULL,GA_Disabled,FALSE,TAG_DONE);
										GT_SetGadgetAttrs(prGadgets[GADIX_LOG     ],prwin,NULL,GA_Disabled,FALSE,TAG_DONE);
									}
									break;
								case GADID_LOGPOPUP:
									SetInitialFileAndDrawer((IsNotEmptyString(sfxprefs_misc.log_path) ? sfxprefs_misc.log_path :"T:sfx.log"));
									File_Reqtags_TitleText		=(ULONG)GetString(strSelectLogPath);
									File_Reqtags_DoMultiSelect	=FALSE;
									File_Reqtags_DoSaveMode		=FALSE;
									File_Reqtags_DrawersOnly	=FALSE;
									if(AslRequest(File_Request,File_Reqtags)) {
										strmfp(sfxprefs_misc.log_path,File_Request->fr_Drawer,File_Request->fr_File);
										GT_SetGadgetAttrs(prGadgets[GADIX_LOG],prwin,NULL,GTST_String,sfxprefs_misc.log_path,TAG_DONE);
									}
									break;
								case GADID_LOG:
									strcpy(sfxprefs_misc.log_path,((struct StringInfo *)(prGadgets[GADIX_LOG]->SpecialInfo))->Buffer);
									break;
								case GADID_SAVEICONS:		sfxprefs_misc.saveicons		=((prGadgets[GADIX_SAVEICONS]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_DEFTOOL:			sfxprefs_misc.deftool		=icode;break;
							}
						}
						if(iclass==IDCMP_VANILLAKEY) {
							switch(icode) {
								case KEY_ENTER:
								case KEY_RETURN:
								case 'O':
								case 'o':
									quit=1;
									break;
								case KEY_ESC:
									sfxprefs_misc=tprefs_misc;
									quit=1;
									break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) {
							sfxprefs_misc=tprefs_misc;
							quit=1;
						}
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)
							ShowHelp("html/nodes/node01.06.04.html");
					}
				}
				CloseWindow(prwin);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		FreeGadgets(prGList);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	if(obacklogo!=sfxprefs_misc.backlogo) {
		bdrp=RunTime.winBackDrop->RPort;
		if(sfxprefs_misc.backlogo) {
			SetABPenDrMd(bdrp,1,5,JAM1);
			DrawImage(bdrp,&backlogo,(RunTime.xres-278)>>1,(RunTime.yres-103)>>1);
		}
		else {
			SetAPen(bdrp,6);
			RectFill(bdrp,(RunTime.xres-278)>>1,(RunTime.yres-103)>>1,((RunTime.xres-278)>>1)+278,((RunTime.yres-103)>>1)+103);
		}
	}
	return(0);
}

//-- eof ----------------------------------------------------------------------
