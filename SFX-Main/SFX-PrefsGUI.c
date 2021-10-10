/****h* SoundFX/SFX-PrefsGUI.c [4.2] *
*
*  NAME
*    SFX-PrefsGUI.c
*  COPYRIGHT
*    $VER: SFX-PrefsGUI.c 4.25.26.09.01 by Stefan Kost 1998-2001
*  FUNCTION
*    GUI related options window - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    26.Sep.2001	V 4.2	most recent version
*    07.Sep.2000	V 4.1	most recent version
*    03.Jun.2000	V 4.00	most recent version
*    24.Aug.1998	V 3.70	most recent version
*    13.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_PrefsGUI_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE PrefsGUI(void);

//-- defines ------------------------------------------------------------------

#define GADID_OKAY			(0)

#define GADID_PALETTE1		(GADID_OKAY+1)

#define GADID_GRABWBCOL		(GADID_PALETTE1+1)

#define GADID_PALR			(GADID_GRABWBCOL+1)
#define GADIX_PALR			(0)

#define GADID_PALRLAB		(GADID_PALR+1)
#define GADIX_PALRLAB		(GADIX_PALR+1)

#define GADID_PALG			(GADID_PALRLAB+1)
#define GADIX_PALG			(GADIX_PALRLAB+1)

#define GADID_PALGLAB		(GADID_PALG+1)
#define GADIX_PALGLAB		(GADIX_PALG+1)

#define GADID_PALB			(GADID_PALGLAB+1)
#define GADIX_PALB			(GADIX_PALGLAB+1)

#define GADID_PALBLAB		(GADID_PALB+1)
#define GADIX_PALBLAB		(GADIX_PALB+1)

#define GADID_PALETTE2		(GADID_PALBLAB+1)
#define GADIX_PALETTE2		(GADIX_PALBLAB+1)

#define GADID_PENSEL		(GADID_PALETTE2+1)
#define GADIX_PENSEL		(GADIX_PALETTE2+1)

#define GADID_SCRPOPUP		(GADID_PENSEL+1)

#define GADID_SCR			(GADID_SCRPOPUP+1)
#define GADIX_SCR			(GADIX_PENSEL+1)

#define GADID_SCRFONTPOPUP	(GADID_SCR+1)

#define GADID_SCRFONT		(GADID_SCRFONTPOPUP+1)
#define GADIX_SCRFONT		(GADIX_SCR+1)

#define GADID_STATFONTPOPUP	(GADID_SCRFONT+1)

#define GADID_STATFONT		(GADID_STATFONTPOPUP+1)
#define GADIX_STATFONT		(GADIX_SCRFONT+1)

#define MAX_PENS			18

//-- definitions --------------------------------------------------------------

UBYTE PrefsGUI(void)
{
	struct NewGadget ng;
	struct Window *prwin;
	struct Gadget *g;
	struct Gadget *prGList=NULL;
	struct Gadget *prGadgets[GADIX_STATFONT+1];
	struct RastPort *prrp;
	struct ViewPort *vp,*wbvp;
	struct Screen *wbscr;
	struct  ColorMap *wbcm;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	register UBYTE i;
	UBYTE quit=0,aktcol=0,aktpen=0,scroll=0,guichange=0,gradchange=0,depth;
	SFXPrefs_Gui	tprefs_gui;
	char fona[SFXPrefs_Gui_FontNameLen+10],color[10];
	ULONG wbcol[8*3];
	struct NameInfo NameInfo;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(5);
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(3);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+RunTime.scx[292];
	UWORD winw=xs2+3+RunTime.scx[292];
	UWORD xh;
	struct List penList;
	struct Node penNodes[MAX_PENS]={NULL};
	char penName[256];

	tprefs_gui	=sfxprefs_gui;

	GetDisplayInfoData(0,(UBYTE *)&NameInfo,sizeof(NameInfo),DTAG_NAME,sfxprefs_gui.scrmode);

	depth=3;
	if(sfxprefs_gui.depth>3) depth=4;

	NewList(&penList);
	sprintf(penName,"%3d GUI Background"	,sfxprefs_gui.Pens[PEN_GUI_BACK			]);penNodes[ 0].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 0]);
	sprintf(penName,"%3d GUI Shine"			,sfxprefs_gui.Pens[PEN_GUI_SHINE		]);penNodes[ 1].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 1]);
	sprintf(penName,"%3d GUI Shadow"		,sfxprefs_gui.Pens[PEN_GUI_SHADOW		]);penNodes[ 2].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 2]);
	sprintf(penName,"%3d GUI Highlight"		,sfxprefs_gui.Pens[PEN_GUI_HIGHLIGHT	]);penNodes[ 3].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 3]);
	sprintf(penName,"%3d GUI ColorText"		,sfxprefs_gui.Pens[PEN_GUI_COLORTEXT	]);penNodes[ 4].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 4]);
	sprintf(penName,"%3d GUI ColorTextGlow"	,sfxprefs_gui.Pens[PEN_GUI_COLORTEXTGLOW]);penNodes[ 5].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 5]);
	sprintf(penName,"%3d GUI GradientStart"	,sfxprefs_gui.Pens[PEN_GUI_GRADIENTSTART]);penNodes[ 6].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 6]);
	sprintf(penName,"%3d GUI GradientEnd"	,sfxprefs_gui.Pens[PEN_GUI_GRADIENTEND	]);penNodes[ 7].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 7]);

	sprintf(penName,"%3d SMP Background"	,sfxprefs_gui.Pens[PEN_SMP_BACK			]);penNodes[ 8].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 8]);
	sprintf(penName,"%3d SMP Text"			,sfxprefs_gui.Pens[PEN_SMP_TEXT			]);penNodes[ 9].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[ 9]);
	sprintf(penName,"%3d SMP Waveform"		,sfxprefs_gui.Pens[PEN_SMP_LINE			]);penNodes[10].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[10]);
	sprintf(penName,"%3d SMP Grid"			,sfxprefs_gui.Pens[PEN_SMP_GRID			]);penNodes[11].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[11]);
	sprintf(penName,"%3d SMP Maxlines"		,sfxprefs_gui.Pens[PEN_SMP_MAXL			]);penNodes[12].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[12]);
	sprintf(penName,"%3d SMP RMSlines"		,sfxprefs_gui.Pens[PEN_SMP_RMSL			]);penNodes[13].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[13]);
	sprintf(penName,"%3d SMP Avglines"		,sfxprefs_gui.Pens[PEN_SMP_AVGL			]);penNodes[14].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[14]);
	sprintf(penName,"%3d SMP Highlight"		,sfxprefs_gui.Pens[PEN_SMP_HIGHLIGHT	]);penNodes[15].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[15]);
	sprintf(penName,"%3d SMP Loop"			,sfxprefs_gui.Pens[PEN_SMP_LOOP			]);penNodes[16].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[16]);
	sprintf(penName,"%3d SMP Mark"			,sfxprefs_gui.Pens[PEN_SMP_MARK			]);penNodes[17].ln_Name=StringCopy(penName);AddTail(&penList,&penNodes[17]);

	if(g=CreateContext(&prGList)) {
		ng.ng_LeftEdge		=xs1+RunTime.scx[252];
		ng.ng_TopEdge		=ys3+RunTime.scy[2];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=RunTime.ScrFontB;
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[69];					/* Colors */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[162];
		ng.ng_Height		=RunTime.scy[23];
		ng.ng_GadgetText	="Palette";
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetID		=GADID_PALETTE1;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(PALETTE_KIND,g,&ng,GTPA_Depth,depth,GTPA_Color,aktcol,GTPA_ColorOffset,0,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[235];
		ng.ng_Width			=RunTime.scx[50];
		ng.ng_GadgetText	="Grab";
		ng.ng_GadgetID		=GADID_GRABWBCOL;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[69];
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime.scx[162];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="Red";
		ng.ng_GadgetID		=GADID_PALR;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(SLIDER_KIND,g,&ng,GTSL_Min,0,GTSL_Max,255,GTSL_Level,sfxprefs_gui.ScreenColors[aktcol][0],PGA_Freedom,LORIENT_HORIZ,GA_RelVerify,TRUE,GA_Immediate,TRUE,TAG_DONE);
		prGadgets[GADIX_PALR]=g;

		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+36];
		ng.ng_GadgetText	="Green";
		ng.ng_GadgetID		=GADID_PALG;
		g=CreateGadget(SLIDER_KIND,g,&ng,GTSL_Min,0,GTSL_Max,255,GTSL_Level,sfxprefs_gui.ScreenColors[aktcol][1],PGA_Freedom,LORIENT_HORIZ,GA_RelVerify,TRUE,GA_Immediate,TRUE,TAG_DONE);
		prGadgets[GADIX_PALG]=g;

		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+48];
		ng.ng_GadgetText	="Blue";
		ng.ng_GadgetID		=GADID_PALB;
		g=CreateGadget(SLIDER_KIND,g,&ng,GTSL_Min,0,GTSL_Max,255,GTSL_Level,sfxprefs_gui.ScreenColors[aktcol][2],PGA_Freedom,LORIENT_HORIZ,GA_RelVerify,TRUE,GA_Immediate,TRUE,TAG_DONE);
		prGadgets[GADIX_PALB]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[245];
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime.scx[40];
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_PALRLAB;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,TAG_DONE);
		prGadgets[GADIX_PALRLAB]=g;

		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+36];
		ng.ng_GadgetID		=GADID_PALGLAB;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,TAG_DONE);
		prGadgets[GADIX_PALGLAB]=g;

		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+48];
		ng.ng_GadgetID		=GADID_PALBLAB;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,TAG_DONE);
		prGadgets[GADIX_PALBLAB]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[69];					/* Pens */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[162];
		ng.ng_Height		=RunTime.scy[23];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="Palette";
		ng.ng_GadgetID		=GADID_PALETTE2;
		g=CreateGadget(PALETTE_KIND,g,&ng,GTPA_Depth,depth,GTPA_Color,1,GTPA_ColorOffset,0,TAG_DONE);
		prGadgets[GADIX_PALETTE2]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[69];
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime.scx[200];
		ng.ng_Height		=RunTime.scy[85];
		ng.ng_GadgetText	="Pens";
		ng.ng_GadgetID		=GADID_PENSEL;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,
							GTLV_Labels,&penList,
							GTLV_Selected,aktpen,GTLV_Top,aktpen,GTLV_ShowSelected,0,
							GTLV_CallBack,RunTime.colorlvhook,
							GTLV_MaxPen,255,
						TAG_DONE);
		prGadgets[GADIX_PENSEL]=g;


		ng.ng_LeftEdge		=xs1+RunTime.scx[69];					/* Screen */
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[15];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_TextAttr		=RunTime.SigFont;
		ng.ng_GadgetText	=IMAGE_ScrMdReq;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_GadgetID		=GADID_SCRPOPUP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[85];
		ng.ng_Width			=RunTime.scx[200];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_SCR;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,NameInfo.Name,GTTX_CopyText,TRUE,TAG_DONE);
		prGadgets[GADIX_SCR]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[69];
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime.scx[15];
		ng.ng_TextAttr		=RunTime.SigFont;
		ng.ng_GadgetText	=IMAGE_FontReq;
		ng.ng_GadgetID		=GADID_SCRFONTPOPUP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[85];
		ng.ng_Width			=RunTime.scx[200];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_SCRFONT;
		sprintf(fona,"%s , %d",sfxprefs_gui.scrfont_name,sfxprefs_gui.scrfont_size);
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,fona,GTTX_CopyText,TRUE,TAG_DONE);
		prGadgets[GADIX_SCRFONT]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[69];
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime.scx[15];
		ng.ng_TextAttr		=RunTime.SigFont;
		ng.ng_GadgetText	=IMAGE_FontReq;
		ng.ng_GadgetID		=GADID_STATFONTPOPUP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[85];
		ng.ng_Width			=RunTime.scx[200];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_STATFONT;
		sprintf(fona,"%s , %d",sfxprefs_gui.statfont_name,sfxprefs_gui.statfont_size);
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,fona,GTTX_CopyText,TRUE,TAG_DONE);
		prGadgets[GADIX_STATFONT]=g;

		if(g) {
			MemUsage(sfxtitle);
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|LISTVIEWIDCMP|SLIDERIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)prGList;
			Dialog_Wintags_Title		=(ULONG)GetString(menPrefsGUI);
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(prwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(prwin,NULL);

				prrp=prwin->RPort;SetFont(prrp,RunTime.scrfont);
				vp=&RunTime.sfxpubscr->ViewPort;

				SetAPen(prrp,1);
				RText(prrp,"Screen",xs1+RunTime.scx[61],ys2+RunTime.scy[FRMTEXT_YPOS(1)]);
				RText(prrp,"Scr. Font",xs1+RunTime.scx[61],ys2+RunTime.scy[FRMTEXT_YPOS(2)]);
				RText(prrp,"Stat. Font",xs1+RunTime.scx[61],ys2+RunTime.scy[FRMTEXT_YPOS(3)]);

				xh=RunTime.scx[292];
				DrawTitle(prrp,"Colors"					,xs1,ys1,xh);
				DrawTitle(prrp,"Resolution & Fonts"		,xs1,ys2,xh);
				DrawTitle(prrp,"Color Pens"				,xs2,ys1,xh);

				DrawGadWB(prrp,xs1,ys1,xh     ,ANYFRM_HEIGHT(5));					/* Palette */
				DrawGadWB(prrp,xs1,ys2,xh     ,ANYFRM_HEIGHT(3));					/* Screen */
				DrawGadWB(prrp,xs2,ys1,xh     ,ANYFRM_HEIGHT(5)+ANYFRM_HEIGHT(3)+1);/* Pens */
				DrawGadWB(prrp,xs1,ys3,xh+xh+1,CMDFRM_HEIGHT);						/* Okay  */

				sprintf(color,"%3d",sfxprefs_gui.ScreenColors[aktcol][0]);GT_SetGadgetAttrs(prGadgets[GADIX_PALRLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
				sprintf(color,"%3d",sfxprefs_gui.ScreenColors[aktcol][1]);GT_SetGadgetAttrs(prGadgets[GADIX_PALGLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
				sprintf(color,"%3d",sfxprefs_gui.ScreenColors[aktcol][2]);GT_SetGadgetAttrs(prGadgets[GADIX_PALBLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);

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
									quit=TRUE;
									break;
								case GADID_PALETTE1:
									aktcol=icode;
									GT_SetGadgetAttrs(prGadgets[GADIX_PALR],prwin,NULL,GTSL_Level,sfxprefs_gui.ScreenColors[aktcol][0],TAG_DONE);
									GT_SetGadgetAttrs(prGadgets[GADIX_PALG],prwin,NULL,GTSL_Level,sfxprefs_gui.ScreenColors[aktcol][1],TAG_DONE);
									GT_SetGadgetAttrs(prGadgets[GADIX_PALB],prwin,NULL,GTSL_Level,sfxprefs_gui.ScreenColors[aktcol][2],TAG_DONE);
									sprintf(color,"%3d",sfxprefs_gui.ScreenColors[aktcol][0]);GT_SetGadgetAttrs(prGadgets[GADIX_PALRLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									sprintf(color,"%3d",sfxprefs_gui.ScreenColors[aktcol][1]);GT_SetGadgetAttrs(prGadgets[GADIX_PALGLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									sprintf(color,"%3d",sfxprefs_gui.ScreenColors[aktcol][2]);GT_SetGadgetAttrs(prGadgets[GADIX_PALBLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									break;
								case GADID_GRABWBCOL:
									if((wbscr=LockPubScreen("Workbench"))) {				// lock wb screen
										wbvp=&wbscr->ViewPort;wbcm=wbvp->ColorMap;
										GetRGB32(wbcm,0,8,wbcol);								// grab colors
										UnlockPubScreen(NULL,wbscr);								// unlock wb screen
										// 0=0
										sfxprefs_gui.ScreenColors[0][0]=(UBYTE)(wbcol[0*3+0]&0xFF);sfxprefs_gui.ScreenColors[0][1]=(UBYTE)(wbcol[0*3+1]&0xFF);sfxprefs_gui.ScreenColors[0][2]=(UBYTE)(wbcol[0*3+2]&0xFF);
										SetRGB32(vp,0,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[0][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[0][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[0][2]));
										// 1=1
										sfxprefs_gui.ScreenColors[1][0]=(UBYTE)(wbcol[1*3+0]&0xFF);sfxprefs_gui.ScreenColors[1][1]=(UBYTE)(wbcol[1*3+1]&0xFF);sfxprefs_gui.ScreenColors[1][2]=(UBYTE)(wbcol[1*3+2]&0xFF);
										SetRGB32(vp,1,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[1][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[1][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[1][2]));
										// 2=2
										sfxprefs_gui.ScreenColors[2][0]=(UBYTE)(wbcol[2*3+0]&0xFF);sfxprefs_gui.ScreenColors[2][1]=(UBYTE)(wbcol[2*3+1]&0xFF);sfxprefs_gui.ScreenColors[2][2]=(UBYTE)(wbcol[2*3+2]&0xFF);
										SetRGB32(vp,2,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[2][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[2][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[2][2]));
										// 5=0+30
										MSG3("%ld %ld %ld",wbcol[0*3+0],wbcol[0*3+1],wbcol[0*3+2]);
										if((UBYTE)(wbcol[0*3+0]&0xFF)<(255-30))	sfxprefs_gui.ScreenColors[5][0]=(UBYTE)(wbcol[0*3+0]&0xFF)+30;
										else									sfxprefs_gui.ScreenColors[5][0]=255;
										if((UBYTE)(wbcol[0*3+1]&0xFF)<(255-30))	sfxprefs_gui.ScreenColors[5][1]=(UBYTE)(wbcol[0*3+1]&0xFF)+30;
										else									sfxprefs_gui.ScreenColors[5][1]=255;
										if((UBYTE)(wbcol[0*3+2]&0xFF)<(255-30))	sfxprefs_gui.ScreenColors[5][2]=(UBYTE)(wbcol[0*3+2]&0xFF)+30;
										else									sfxprefs_gui.ScreenColors[5][2]=255;
										SetRGB32(vp,5,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[5][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[5][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[5][2]));
										// 6=0-60
										if((UBYTE)(wbcol[0*3+0]&0xFF)>(0  +60))	sfxprefs_gui.ScreenColors[6][0]=(UBYTE)(wbcol[0*3+0]&0xFF)-60;
										else									sfxprefs_gui.ScreenColors[6][0]=0;
										if((UBYTE)(wbcol[0*3+1]&0xFF)>(0  +60))	sfxprefs_gui.ScreenColors[6][1]=(UBYTE)(wbcol[0*3+1]&0xFF)-60;
										else									sfxprefs_gui.ScreenColors[6][1]=0;
										if((UBYTE)(wbcol[0*3+2]&0xFF)>(0  +60))	sfxprefs_gui.ScreenColors[6][2]=(UBYTE)(wbcol[0*3+2]&0xFF)-60;
										else									sfxprefs_gui.ScreenColors[6][2]=0;
										SetRGB32(vp,6,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[6][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[6][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[6][2]));
										GT_SetGadgetAttrs(prGadgets[GADIX_PALR],prwin,NULL,GTSL_Level,sfxprefs_gui.ScreenColors[aktcol][0],TAG_DONE);
										GT_SetGadgetAttrs(prGadgets[GADIX_PALG],prwin,NULL,GTSL_Level,sfxprefs_gui.ScreenColors[aktcol][1],TAG_DONE);
										GT_SetGadgetAttrs(prGadgets[GADIX_PALB],prwin,NULL,GTSL_Level,sfxprefs_gui.ScreenColors[aktcol][2],TAG_DONE);
										sprintf(color,"%3d",sfxprefs_gui.ScreenColors[aktcol][0]);GT_SetGadgetAttrs(prGadgets[GADIX_PALRLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
										sprintf(color,"%3d",sfxprefs_gui.ScreenColors[aktcol][1]);GT_SetGadgetAttrs(prGadgets[GADIX_PALGLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
										sprintf(color,"%3d",sfxprefs_gui.ScreenColors[aktcol][2]);GT_SetGadgetAttrs(prGadgets[GADIX_PALBLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									}
									break;
								case GADID_PALR:
									sfxprefs_gui.ScreenColors[aktcol][0]=icode;
									SetRGB32(vp,aktcol,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][2]));
									sprintf(color,"%3d",icode);GT_SetGadgetAttrs(prGadgets[GADIX_PALRLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									scroll=0;
									break;
								case GADID_PALG:
									sfxprefs_gui.ScreenColors[aktcol][1]=icode;
									SetRGB32(vp,aktcol,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][2]));
									sprintf(color,"%3d",icode);GT_SetGadgetAttrs(prGadgets[GADIX_PALGLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									scroll=0;
									break;
								case GADID_PALB:
									sfxprefs_gui.ScreenColors[aktcol][2]=icode;
									SetRGB32(vp,aktcol,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][2]));
									sprintf(color,"%3d",icode);GT_SetGadgetAttrs(prGadgets[GADIX_PALBLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									scroll=0;
									break;
								case GADID_PALETTE2:
									sfxprefs_gui.Pens[aktpen]=icode;
									sprintf(penName,"%3d",icode);
									penNodes[aktpen].ln_Name[0]=penName[0];
									penNodes[aktpen].ln_Name[1]=penName[1];
									penNodes[aktpen].ln_Name[2]=penName[2];
									GT_SetGadgetAttrs(prGadgets[GADIX_PENSEL],prwin,NULL,GTLV_Labels,&penList,TAG_DONE);
									if((aktpen==PEN_GUI_GRADIENTSTART) || (aktpen==PEN_GUI_GRADIENTEND)) gradchange=TRUE;
									break;
								case GADID_PENSEL:
									aktpen=icode;
									GT_SetGadgetAttrs(prGadgets[GADIX_PALETTE2],prwin,NULL,GTPA_Color,sfxprefs_gui.Pens[aktpen],TAG_DONE);
									break;
								case GADID_SCRPOPUP:
									if(AslRequest(ScrMd_Request,ScrMd_Reqtags)) {
										sfxprefs_gui.scrmode	=ScrMd_Request->sm_DisplayID;
										sfxprefs_gui.xres		=ScrMd_Request->sm_DisplayWidth;
										sfxprefs_gui.yres		=ScrMd_Request->sm_DisplayHeight;
										sfxprefs_gui.depth		=ScrMd_Request->sm_DisplayDepth;
										sfxprefs_gui.oscan		=ScrMd_Request->sm_OverscanType;
										sfxprefs_gui.autoscroll	=ScrMd_Request->sm_AutoScroll;
										GetDisplayInfoData(0,(UBYTE *)&NameInfo,sizeof(NameInfo),DTAG_NAME,sfxprefs_gui.scrmode);
										GT_SetGadgetAttrs(prGadgets[GADIX_SCR],prwin,NULL,GTTX_Text,NameInfo.Name,GTTX_CopyText,TRUE,TAG_DONE);
										guichange=TRUE;
									}
									break;
								case GADID_SCRFONTPOPUP:
									Font_Reqtags_InitialName=(ULONG)sfxprefs_gui.scrfont_name;
									Font_Reqtags_InitialSize=(ULONG)sfxprefs_gui.scrfont_size;
									if(AslRequest(Font_Request,Font_Reqtags)) {
										SetFont(prrp,RunTime.scrfont);
										strcpy(sfxprefs_gui.scrfont_name,Font_Request->fo_Attr.ta_Name);
										sfxprefs_gui.scrfont_size=Font_Request->fo_Attr.ta_YSize;
										sprintf(fona,"%s , %d",sfxprefs_gui.scrfont_name,sfxprefs_gui.scrfont_size);
										GT_SetGadgetAttrs(prGadgets[GADIX_SCRFONT],prwin,NULL,GTTX_Text,fona,GTTX_CopyText,TRUE,TAG_DONE);
										guichange=TRUE;
									}
									break;
								case GADID_STATFONTPOPUP:
									Font_Reqtags_InitialName=(ULONG)sfxprefs_gui.statfont_name;
									Font_Reqtags_InitialSize=(ULONG)sfxprefs_gui.statfont_size;
									if(AslRequest(Font_Request,Font_Reqtags)) {
										SetFont(prrp,RunTime.scrfont);
										strcpy(sfxprefs_gui.statfont_name,Font_Request->fo_Attr.ta_Name);
										sfxprefs_gui.statfont_size=Font_Request->fo_Attr.ta_YSize;
										sprintf(fona,"%s , %d",sfxprefs_gui.statfont_name,sfxprefs_gui.statfont_size);
										GT_SetGadgetAttrs(prGadgets[GADIX_STATFONT],prwin,NULL,GTTX_Text,fona,GTTX_CopyText,TRUE,TAG_DONE);
										guichange=TRUE;
									}
									break;
							}
						}
						if(iclass==IDCMP_GADGETDOWN) {
							switch((int)(((struct Gadget *)iadr)->GadgetID)) {
								case GADID_PALR:
								case GADID_PALG:
								case GADID_PALB:
									scroll=TRUE;
									break;
							}
						}
						if(scroll && iclass==IDCMP_MOUSEMOVE) {
							switch((int)(((struct Gadget *)iadr)->GadgetID)) {
								case GADID_PALR:
									sfxprefs_gui.ScreenColors[aktcol][0]=icode;
									SetRGB32(vp,aktcol,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][2]));
									sprintf(color,"%3d",icode);GT_SetGadgetAttrs(prGadgets[GADIX_PALRLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									break;
								case GADID_PALG:
									sfxprefs_gui.ScreenColors[aktcol][1]=icode;
									SetRGB32(vp,aktcol,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][2]));
									sprintf(color,"%3d",icode);GT_SetGadgetAttrs(prGadgets[GADIX_PALGLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									break;
								case GADID_PALB:
									sfxprefs_gui.ScreenColors[aktcol][2]=icode;
									SetRGB32(vp,aktcol,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[aktcol][2]));
									sprintf(color,"%3d",icode);GT_SetGadgetAttrs(prGadgets[GADIX_PALBLAB],prwin,NULL,GTTX_Text,color,GTTX_CopyText,TRUE,TAG_DONE);
									break;
							}
						}
						if(iclass==IDCMP_VANILLAKEY) {
							switch(icode) {
								case KEY_ENTER:
								case KEY_RETURN:
								case 'O':
								case 'o':
									quit=TRUE;
									break;
								case KEY_ESC:
									sfxprefs_gui=tprefs_gui;
									quit=TRUE;
									break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) {
							sfxprefs_gui=tprefs_gui;
							for(i=0;i<(1L<<depth);i++) SetRGB32(vp,i,MakeCol32((ULONG)sfxprefs_gui.ScreenColors[i][0]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[i][1]),MakeCol32((ULONG)sfxprefs_gui.ScreenColors[i][2]));
							guichange=FALSE;
							quit=TRUE;
						}
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)
							ShowHelp("html/nodes/node01.06.01.html");
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

	for(i=0;i<MAX_PENS;i++) if(penNodes[i].ln_Name) FreeVec(penNodes[i].ln_Name);

	if((!guichange) && gradchange) { FreeGradientColors();ObtainGradientColors(); }
	if(guichange) RunTime.tryreopen=TRUE;
	return(guichange);
}

//-- eof ----------------------------------------------------------------------
