/****h*SoundFX/SFX-PrefsVMem.c [4.3] *
*
*  NAME
*    SFX-PrefsVMem.c
*  COPYRIGHT
*    $VER: SFX-PrefsVMem.c 4.3 (28.01.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    virtual memory related options - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    28.Jan.2004	V 4.3	most recent version
*    26.Sep.2001	V 4.2	most recent version
*    11.Jan.2001	V 4.1	most recent version
*    20.Sep.1998	V 3.71	most recent version
*							* restore sample filereq. path after choosing vmem path
*    24.Aug.1998	V 3.70	most recent version
*    13.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_PrefsVMem_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE PrefsVMem(void);

//-- defines ------------------------------------------------------------------

#define GADID_OKAY			(0)

#define GADID_VMENABLED		(GADID_OKAY+1)
#define GADIX_VMENABLED		(0)

#define GADID_SWAPPOPUP		(GADID_VMENABLED+1)
#define GADIX_SWAPPOPUP		(GADIX_VMENABLED+1)

#define GADID_SWAP			(GADID_SWAPPOPUP+1)
#define GADIX_SWAP			(GADIX_SWAPPOPUP+1)

#define GADID_BLKSIZE		(GADID_SWAP+1)
#define GADIX_BLKSIZE		(GADIX_SWAP+1)

//-- definitions --------------------------------------------------------------

UBYTE PrefsVMem(void)
{
	struct NewGadget ng;
	struct Window *prwin;
	struct Gadget *g;
	struct Gadget *prGList=NULL;
	struct Gadget *prGadgets[GADIX_BLKSIZE+1];
	struct RastPort *prrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	UBYTE quit=0;
	SFXPrefs_VMem	tprefs_vmem;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(3);
	UWORD winh=ys2+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime.scx[292];
	UWORD xh;

	tprefs_vmem	=sfxprefs_vmem;

	if(g=CreateContext(&prGList))
	{
		ng.ng_LeftEdge		=xs1+RunTime.scx[106];
		ng.ng_TopEdge		=ys2+RunTime.scy[2];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=RunTime.ScrFontB;
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[89];					/* VMem On/Off */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[26];
		ng.ng_GadgetText	="Use VMem";
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetID		=GADID_VMENABLED;
		ng.ng_Flags			=PLACETEXT_LEFT;
		if(RunTime.status!=STAT_DEMO) g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_vmem.enabled,GTCB_Scaled,TRUE,TAG_DONE);
		else g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,(sfxprefs_vmem.enabled=FALSE),GTCB_Scaled,TRUE,GA_Disabled,TRUE,TAG_DONE);
		prGadgets[GADIX_VMENABLED]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[89];					/* Swappath */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime.scx[15];
		ng.ng_TextAttr		=RunTime.SigFont;
		ng.ng_GadgetText	=IMAGE_DirReq;
		ng.ng_GadgetID		=GADID_SWAPPOPUP;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		prGadgets[GADIX_SWAPPOPUP]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[105];
		ng.ng_Width			=RunTime.scx[180];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_SWAP;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,SFXPrefs_VMem_SwapPathLen-1,GTST_String,sfxprefs_vmem.swap_path,TAG_DONE);
		prGadgets[GADIX_SWAP]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[89];
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime.scx[100];
		ng.ng_GadgetText	="Block Size";
		ng.ng_GadgetID		=GADID_BLKSIZE;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,10,GTIN_Number,sfxprefs_vmem.blksize,TAG_DONE);
		prGadgets[GADIX_BLKSIZE]=g;

		if(g) {
			MemUsage(sfxtitle);
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)prGList;
			Dialog_Wintags_Title		=(ULONG)GetString(menPrefsVMem);
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(prwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(prwin,NULL);

				prrp=prwin->RPort;SetFont(prrp,RunTime.scrfont);

				SetAPen(prrp,1);
				RText(prrp,"Swap Path",xs1+RunTime.scx[81],ys1+RunTime.scy[FRMTEXT_YPOS(2)]);

				xh=RunTime.scx[292];
				DrawTitle(prrp,"Virtual Memory"		,xs1,ys1,xh);

				DrawGadWB(prrp,xs1,ys1,xh,ANYFRM_HEIGHT(3));			/* VMem */
				DrawGadWB(prrp,xs1,ys2,xh,CMDFRM_HEIGHT);				/* Okay */

				if(!sfxprefs_vmem.enabled) {
					GT_SetGadgetAttrs(prGadgets[GADIX_SWAPPOPUP],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(prGadgets[GADIX_SWAP     ],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(prGadgets[GADIX_BLKSIZE  ],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
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
								case GADID_VMENABLED:
									if((sfxprefs_vmem.enabled=((prGadgets[GADIX_VMENABLED]->Flags&GFLG_SELECTED)!=0))) {
										GT_SetGadgetAttrs(prGadgets[GADIX_SWAPPOPUP],prwin,NULL,GA_Disabled,FALSE,TAG_DONE);
										GT_SetGadgetAttrs(prGadgets[GADIX_SWAP     ],prwin,NULL,GA_Disabled,FALSE,TAG_DONE);
										GT_SetGadgetAttrs(prGadgets[GADIX_BLKSIZE  ],prwin,NULL,GA_Disabled,FALSE,TAG_DONE);
									}
									else {
										GT_SetGadgetAttrs(prGadgets[GADIX_SWAPPOPUP],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
										GT_SetGadgetAttrs(prGadgets[GADIX_SWAP     ],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
										GT_SetGadgetAttrs(prGadgets[GADIX_BLKSIZE  ],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
									}
									break;
								case GADID_SWAPPOPUP:
									SetInitialFileAndDrawer((IsNotEmptyString(sfxprefs_vmem.swap_path) ? sfxprefs_vmem.swap_path : "progdir:"));
									File_Reqtags_TitleText		=(ULONG)GetString(strSelectVMemPath);
									File_Reqtags_DoMultiSelect	=FALSE;
									File_Reqtags_DoSaveMode		=FALSE;
									File_Reqtags_DrawersOnly	=TRUE;
									if(AslRequest(File_Request,File_Reqtags)) {
										strcpy(sfxprefs_vmem.swap_path,File_Request->fr_Drawer);
										sfxprefs_vmem.swap_path[strlen(File_Request->fr_Drawer)]=0;
										GT_SetGadgetAttrs(prGadgets[GADIX_SWAP],prwin,NULL,GTST_String,sfxprefs_vmem.swap_path,TAG_DONE);
									}
									break;
								case GADID_SWAP:
									strcpy(sfxprefs_vmem.swap_path,((struct StringInfo *)(prGadgets[GADIX_SWAP]->SpecialInfo))->Buffer);
									break;
								case GADID_BLKSIZE:
									CheckGad_int(prGadgets[GADIX_BLKSIZE],prwin,(LONG *)(&sfxprefs_vmem.blksize),1,0,MAXLONGP);
									sfxprefs_vmem.blksize&=(~0x7FL);
									GT_SetGadgetAttrs(prGadgets[GADIX_BLKSIZE],prwin,NULL,GTIN_Number,sfxprefs_vmem.blksize,TAG_DONE);
									break;
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
									sfxprefs_vmem=tprefs_vmem;
									quit=1;
									break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) {
							sfxprefs_vmem=tprefs_vmem;
							quit=1;
						}
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)
							ShowHelp("html/nodes/node01.06.03.html");
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
	return(0);
}

//-- eof ----------------------------------------------------------------------
