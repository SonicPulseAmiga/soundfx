/****h* SoundFX/SFX-PrefsSamp.c [4.3] *
*
*  NAME
*    SFX-PrefsSamp.c
*  COPYRIGHT
*    $VER: SFX-PrefsSamp.c 4.3 (28.01.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    sample related options window - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    28.Jan.2004	V 4.3	most recent version
*    26.Sep.2001	V 4.2	most recent version
*    11.Jan.2001	V 4.1	most recent version
*    16.Jul.2000	V 4.00	most recent version
*							* new options (nolines,thrsize)
*    15.Sep.1998	V 3.71	most recent version
*							* some globals, used by CfgIO
*    24.Aug.1998	V 3.70	release version
*    13.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_PrefsSamp_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE PrefsSamp(void);

//-- defines ------------------------------------------------------------------

#define GADID_OKAY			(0)

#define GADID_SAMLOPPOPUP	(GADID_OKAY+1)

#define GADID_SAMLOP		(GADID_SAMLOPPOPUP+1)
#define GADIX_SAMLOP		0

#define GADID_SAMSAPPOPUP	(GADID_SAMLOP+1)

#define GADID_SAMSAP		(GADID_SAMSAPPOPUP+1)
#define GADIX_SAMSAP		(GADIX_SAMLOP+1)

#define GADID_SCAFONTPOPUP	(GADID_SAMSAP+1)

#define GADID_SCAFONT		(GADID_SCAFONTPOPUP+1)
#define GADIX_SCAFONT		(GADIX_SAMSAP+1)

#define GADID_DRAWMODE		(GADID_SCAFONT+1)

#define GADID_SAFEMODE		(GADID_DRAWMODE+1)

#define GADID_STORAGE		(GADID_SAFEMODE+1)

#define GADID_RASTX			(GADID_STORAGE+1)
#define GADIX_RASTX			(GADIX_SCAFONT+1)
#define GADID_RASTY			(GADID_RASTX+1)
#define GADIX_RASTY			(GADIX_RASTX+1)

#define GADID_MAXLINES		(GADID_RASTY+1)
#define GADIX_MAXLINES		(GADIX_RASTY+1)

#define GADID_SCAX			(GADID_MAXLINES+1)
#define GADIX_SCAX			(GADIX_MAXLINES+1)
#define GADID_SCAY			(GADID_SCAX+1)
#define GADIX_SCAY			(GADIX_SCAX+1)

#define GADID_RMSLINES		(GADID_SCAY+1)
#define GADIX_RMSLINES		(GADIX_SCAY+1)

#define GADID_SCAXUNIT		(GADID_RMSLINES+1)

#define GADID_SCAYUNIT		(GADID_SCAXUNIT+1)

#define GADID_AVGLINES		(GADID_SCAYUNIT+1)
#define GADIX_AVGLINES		(GADIX_RMSLINES+1)

#define GADID_QUICKDRAW		(GADID_AVGLINES+1)
#define GADIX_QUICKDRAW		(GADIX_AVGLINES+1)

#define GADID_SIZEMODE		(GADID_QUICKDRAW+1)

#define GADID_NX			(GADID_SIZEMODE+1)
#define GADIX_NX			(GADIX_QUICKDRAW+1)
#define GADID_NY			(GADID_NX+1)
#define GADIX_NY			(GADIX_NX+1)

#define GADID_ZX			(GADID_NY+1)
#define GADIX_ZX			(GADIX_NY+1)
#define GADID_ZY			(GADID_ZX+1)
#define GADIX_ZY			(GADIX_ZX+1)

#define GADID_NOLINES		(GADID_ZY+1)
#define GADIX_NOLINES		(GADIX_ZY+1)

#define GADID_THRSIZE		(GADID_NOLINES+1)
#define GADIX_THRSIZE		(GADIX_NOLINES+1)

#define GADID_COPYSTR		(GADID_THRSIZE+1)
#define GADIX_COPYSTR		(GADIX_THRSIZE+1)
#define GADID_COPYGRAB		(GADID_COPYSTR+1)
#define GADIX_COPYGRAB		(GADIX_COPYSTR+1)

#define GADID_AUTHSTR		(GADID_COPYGRAB+1)
#define GADIX_AUTHSTR		(GADIX_COPYGRAB+1)
#define GADID_AUTHGRAB		(GADID_AUTHSTR+1)
#define GADIX_AUTHGRAB		(GADIX_AUTHSTR+1)

#define LAST_GADIX			(GADIX_AUTHGRAB+1)

//-- globals ------------------------------------------------------------------

UBYTE *DrawModeLabels[]={ "Lines","Dots","DotsAbs","DotsHQ","Filled","FilledAbs","FilledHQ",NULL };
UBYTE *SafeLabels[]=	{ "never","if unsaved","always",NULL };
UBYTE *StorageLabels[]=	{ "Memory","Drive",NULL };
UBYTE *XUnitLabels[]=	{ "Samples","Time","SMPTE 24 fps","SMPTE 25 fps","SMPTE 30 fps",NULL };
UBYTE *YUnitLabels[]=	{ "Factor","Per cent","Per thousand","Level","Decibel",NULL };
UBYTE *SizeModeLabels[]={ "Absolute","Relative (%%)",NULL };

//-- definitions --------------------------------------------------------------

UBYTE PrefsSamp(void)
{
	struct NewGadget ng;
	struct Window *prwin;
	struct Gadget *g;
	struct Gadget *prGList=NULL;
	struct Gadget *prGadgets[LAST_GADIX];
	struct RastPort *prrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	UBYTE quit=0,guichange=0;
	LONG lval;
	UBYTE xunit,yunit;
	SFXPrefs_SBuf	tprefs_sbuf;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(5);
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(2);
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(2);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+RunTime.scx[292];
	UWORD winw=xs2+3+RunTime.scx[386];
	UWORD xh;
	char fona[80];
	struct Node *node;

	tprefs_sbuf	=sfxprefs_sbuf;
	sprintf(fona,"%s , %d",sfxprefs_sbuf.scafont_name,sfxprefs_sbuf.scafont_size);

	switch(sfxprefs_sbuf.unitx) {
		case ATME_SAMPLES:	xunit=0;break;
		case ATME_H_M_S_MS:	xunit=1;break;
		case ATME_H_M_S_MF:	xunit=2;break;
		case ATME_H_M_S_PF:	xunit=3;break;
		case ATME_H_M_S_NF:	xunit=4;break;
	}
	switch(sfxprefs_sbuf.unity) {
		case AMP_FC:		yunit=0;break;
		case AMP_ABSPROZ:	yunit=1;break;
		case AMP_ABSPROM:	yunit=2;break;
		case AMP_LEVEL:		yunit=3;break;
		case AMP_DEZIBEL:	yunit=4;break;
	}

	if(g=CreateContext(&prGList)) {
		ng.ng_LeftEdge		=xs1+RunTime.scx[252];
		ng.ng_TopEdge		=ys4+RunTime.scy[2];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=RunTime.ScrFontB;
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[89];					/* Loader-Samplepath */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[15];
		ng.ng_TextAttr		=RunTime.SigFont;
		ng.ng_GadgetText	=IMAGE_DirReq;
		ng.ng_GadgetID		=GADID_SAMLOPPOPUP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[105];
		ng.ng_Width			=RunTime.scx[180];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_SAMLOP;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,SFXPrefs_SBuf_SamplePathLen-1,GTST_String,sfxprefs_sbuf.sample_lo_path,TAG_DONE);
		prGadgets[GADIX_SAMLOP]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[89];					/* Saver-Samplepath */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime.scx[15];
		ng.ng_TextAttr		=RunTime.SigFont;
		ng.ng_GadgetText	=IMAGE_DirReq;
		ng.ng_GadgetID		=GADID_SAMSAPPOPUP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[105];
		ng.ng_Width			=RunTime.scx[180];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_SAMSAP;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,SFXPrefs_SBuf_SamplePathLen-1,GTST_String,sfxprefs_sbuf.sample_sa_path,TAG_DONE);
		prGadgets[GADIX_SAMSAP]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[89];					/* ScalaFont */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime.scx[15];
		ng.ng_TextAttr		=RunTime.SigFont;
		ng.ng_GadgetText	=IMAGE_FontReq;
		ng.ng_GadgetID		=GADID_SCAFONTPOPUP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[105];
		ng.ng_Width			=RunTime.scx[180];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_SCAFONT;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Text,fona,GTTX_Border,TRUE,TAG_DONE);
		prGadgets[GADIX_SCAFONT]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[89];
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+36];
		ng.ng_Width			=RunTime.scx[104];
		ng.ng_GadgetText	="Safe Check";
		ng.ng_GadgetID		=GADID_SAFEMODE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,SafeLabels,GTCY_Active,sfxprefs_sbuf.safemode,TAG_DONE);

		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+48];
		ng.ng_GadgetText	="Storage";
		ng.ng_GadgetID		=GADID_STORAGE;
		if(RunTime.status!=STAT_DEMO) g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,StorageLabels,GTCY_Active,sfxprefs_sbuf.storage-1,TAG_DONE);
		else                          g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,StorageLabels,GTCY_Active,sfxprefs_sbuf.storage-1,GA_Disabled,TRUE,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime.scx[79];					/* graphical Options */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[104];
		ng.ng_GadgetText	="Draw Mode";
		ng.ng_GadgetID		=GADID_DRAWMODE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,DrawModeLabels,GTCY_Active,sfxprefs_sbuf.drawmode,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime.scx[273];
		ng.ng_Width			=RunTime.scx[26];
		ng.ng_GadgetText	="Quick Draw";
		ng.ng_GadgetID		=GADID_QUICKDRAW;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_sbuf.quickdraw,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_QUICKDRAW]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[79];
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime.scx[26];
		ng.ng_GadgetText	="RastX";
		ng.ng_GadgetID		=GADID_RASTX;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_sbuf.rasterx,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_RASTX]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[176];
		ng.ng_GadgetText	="AxisX";
		ng.ng_GadgetID		=GADID_SCAX;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_sbuf.scalax,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_SCAX]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[257];
		ng.ng_Width			=RunTime.scx[120];
		ng.ng_GadgetText	="UnitX";
		ng.ng_GadgetID		=GADID_SCAXUNIT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,XUnitLabels,GTCY_Active,xunit,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime.scx[79];
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime.scx[26];
		ng.ng_GadgetText	="RastY";
		ng.ng_GadgetID		=GADID_RASTY;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_sbuf.rastery,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_RASTY]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[176];
		ng.ng_GadgetText	="AxisY";
		ng.ng_GadgetID		=GADID_SCAY;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_sbuf.scalay,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_SCAY]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[257];
		ng.ng_Width			=RunTime.scx[120];
		ng.ng_GadgetText	="UnitY";
		ng.ng_GadgetID		=GADID_SCAYUNIT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,YUnitLabels,GTCY_Active,yunit,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime.scx[79];
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+36];
		ng.ng_Width			=RunTime.scx[26];
		ng.ng_GadgetText	="Max-Lines";
		ng.ng_GadgetID		=GADID_MAXLINES;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_sbuf.maxlines,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_MAXLINES]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[176];
		ng.ng_GadgetText	="RMS-Lines";
		ng.ng_GadgetID		=GADID_RMSLINES;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_sbuf.rmslines,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_RMSLINES]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[273];
		ng.ng_GadgetText	="Avg-Lines";
		ng.ng_GadgetID		=GADID_AVGLINES;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_sbuf.avglines,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_AVGLINES]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[176];			/* NoLines when Sample bigger than .... */
		ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET+48];
		ng.ng_GadgetText	="No Lines";
		ng.ng_GadgetID		=GADID_NOLINES;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,sfxprefs_sbuf.nolines,GTCB_Scaled,TRUE,TAG_DONE);
		prGadgets[GADIX_NOLINES]=g;

		ng.ng_LeftEdge		=xs2+RunTime.scx[297];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_GadgetText	="Size Threshold";
		ng.ng_GadgetID		=GADID_THRSIZE;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,10,GTIN_Number,sfxprefs_sbuf.thrsize,TAG_DONE);
		prGadgets[GADIX_THRSIZE]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[89];			/* WinSize */
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[104];
		ng.ng_GadgetText	="Size Mode";
		ng.ng_GadgetID		=GADID_SIZEMODE;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,SizeModeLabels,GTCY_Active,sfxprefs_sbuf.mode,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[110];
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime.scx[65];
		ng.ng_GadgetText	="Normal WinXSize";
		ng.ng_GadgetID		=GADID_NX;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,4,GTIN_Number,sfxprefs_sbuf.xs1,TAG_DONE);
		prGadgets[GADIX_NX]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[221];
		ng.ng_GadgetText	="YSize";
		ng.ng_GadgetID		=GADID_NY;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,4,GTIN_Number,sfxprefs_sbuf.ys1,TAG_DONE);
		prGadgets[GADIX_NY]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[396];
		ng.ng_GadgetText	="Zoomed WinXSize";
		ng.ng_GadgetID		=GADID_ZX;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,4,GTIN_Number,sfxprefs_sbuf.xs2,TAG_DONE);
		prGadgets[GADIX_ZX]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[507];
		ng.ng_GadgetText	="YSize";
		ng.ng_GadgetID		=GADID_ZY;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,4,GTIN_Number,sfxprefs_sbuf.ys2,TAG_DONE);
		prGadgets[GADIX_ZY]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[79];			/* Information Strings */
		ng.ng_TopEdge		=ys3+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime.scx[400];
		ng.ng_GadgetText	="(C)";
		ng.ng_GadgetID		=GADID_COPYSTR;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,SFXPrefs_SBuf_InfoStrLen,GTST_String,sfxprefs_sbuf.copystr,TAG_DONE);
		prGadgets[GADIX_COPYSTR]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[480];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_GadgetText	="From Key";
		ng.ng_GadgetID		=GADID_COPYGRAB;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		prGadgets[GADIX_COPYGRAB]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[79];
		ng.ng_TopEdge		=ys3+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime.scx[400];
		ng.ng_GadgetText	="Author";
		ng.ng_GadgetID		=GADID_AUTHSTR;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,SFXPrefs_SBuf_InfoStrLen,GTST_String,sfxprefs_sbuf.authstr,TAG_DONE);
		prGadgets[GADIX_AUTHSTR]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[480];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_GadgetText	="From Key";
		ng.ng_GadgetID		=GADID_AUTHGRAB;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		prGadgets[GADIX_AUTHGRAB]=g;

		if(g) {
			MemUsage(sfxtitle);
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)prGList;
			Dialog_Wintags_Title		=(ULONG)GetString(menPrefsSample);
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(prwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(prwin,NULL);

				prrp=prwin->RPort;SetFont(prrp,RunTime.scrfont);

				SetAPen(prrp,1);
				RText(prrp,"Loader Path",xs1+RunTime.scx[81],ys1+RunTime.scy[FRMTEXT_YPOS(1)]);
				RText(prrp,"Saver Path",xs1+RunTime.scx[81],ys1+RunTime.scy[FRMTEXT_YPOS(2)]);
				RText(prrp,"Axis Font",xs1+RunTime.scx[81],ys1+RunTime.scy[FRMTEXT_YPOS(3)]);

				DrawTitle(prrp,"Sample Parameter"	,xs1,ys1,RunTime.scx[292]);
				DrawTitle(prrp,"Graphic Options"	,xs2,ys1,RunTime.scx[386]);
				xh=RunTime.scx[292]+RunTime.scx[386]+1;
				DrawTitle(prrp,"Window Size"		,xs1,ys2,xh);
				DrawTitle(prrp,"Information Strings",xs1,ys3,xh);

				DrawGadWB(prrp,xs1,ys1,RunTime.scx[292],ANYFRM_HEIGHT(5));		/* Misc */
				DrawGadWB(prrp,xs2,ys1,RunTime.scx[386],ANYFRM_HEIGHT(5));		/* Gfx Opts */
				xh=RunTime.scx[292]+RunTime.scx[386]+1;
				DrawGadWB(prrp,xs1,ys2,xh,ANYFRM_HEIGHT(2));					/* Winsize */
				DrawGadWB(prrp,xs1,ys3,xh,ANYFRM_HEIGHT(2));					/* Def. Infostrings */
				DrawGadWB(prrp,xs1,ys4,xh,CMDFRM_HEIGHT);						/* Okay */

				GT_SetGadgetAttrs(prGadgets[GADIX_THRSIZE],prwin,NULL,GA_Disabled,!sfxprefs_sbuf.nolines,TAG_DONE);

				if(RunTime.status==STAT_DEMO) {
					GT_SetGadgetAttrs(prGadgets[GADIX_COPYSTR ],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(prGadgets[GADIX_COPYGRAB],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(prGadgets[GADIX_AUTHSTR ],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(prGadgets[GADIX_AUTHGRAB],prwin,NULL,GA_Disabled,TRUE,TAG_DONE);
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
								case GADID_SAMLOPPOPUP:
									SetInitialFileAndDrawer((IsNotEmptyString(sfxprefs_sbuf.sample_lo_path) ? sfxprefs_sbuf.sample_lo_path :"progdir:"));
									File_Reqtags_TitleText		=(ULONG)GetString(strSelectSmpPath);
									File_Reqtags_DoMultiSelect	=FALSE;
									File_Reqtags_DoSaveMode		=FALSE;
									File_Reqtags_DrawersOnly	=TRUE;
									if(AslRequest(File_Request,File_Reqtags)) {
										strcpy(sfxprefs_sbuf.sample_lo_path,File_Request->fr_Drawer);
										sfxprefs_sbuf.sample_lo_path[strlen(File_Request->fr_Drawer)]='\0';
										GT_SetGadgetAttrs(prGadgets[GADIX_SAMLOP],prwin,NULL,GTST_String,sfxprefs_sbuf.sample_lo_path,TAG_DONE);
									}
									break;
								case GADID_SAMLOP:
									strcpy(sfxprefs_sbuf.sample_lo_path,((struct StringInfo *)(prGadgets[GADIX_SAMLOP]->SpecialInfo))->Buffer);
									break;
								case GADID_SAMSAPPOPUP:
									SetInitialFileAndDrawer((IsNotEmptyString(sfxprefs_sbuf.sample_sa_path) ? sfxprefs_sbuf.sample_sa_path :"progdir:"));
									File_Reqtags_TitleText		=(ULONG)GetString(strSelectSmpPath);
									File_Reqtags_DoMultiSelect	=FALSE;
									File_Reqtags_DoSaveMode		=FALSE;
									File_Reqtags_DrawersOnly	=TRUE;
									if(AslRequest(File_Request,File_Reqtags)) {
										strcpy(sfxprefs_sbuf.sample_sa_path,File_Request->fr_Drawer);
										sfxprefs_sbuf.sample_sa_path[strlen(File_Request->fr_Drawer)]='\0';
										GT_SetGadgetAttrs(prGadgets[GADIX_SAMSAP],prwin,NULL,GTST_String,sfxprefs_sbuf.sample_sa_path,TAG_DONE);
									}
									break;
								case GADID_SAMSAP:
									strcpy(sfxprefs_sbuf.sample_sa_path,((struct StringInfo *)(prGadgets[GADIX_SAMSAP]->SpecialInfo))->Buffer);
									break;
								case GADID_SCAFONTPOPUP:
									Font_Reqtags_InitialName=(ULONG)sfxprefs_sbuf.scafont_name;
									Font_Reqtags_InitialSize=(ULONG)sfxprefs_sbuf.scafont_size;
									if(AslRequest(Font_Request,Font_Reqtags)) {
										SetFont(prrp,RunTime.scrfont);
										strcpy(sfxprefs_sbuf.scafont_name,Font_Request->fo_Attr.ta_Name);
										sfxprefs_sbuf.scafont_size=Font_Request->fo_Attr.ta_YSize;
										sprintf(fona,"%s , %d",sfxprefs_sbuf.scafont_name,sfxprefs_sbuf.scafont_size);
										GT_SetGadgetAttrs(prGadgets[GADIX_SCAFONT],prwin,NULL,GTTX_Text,fona,GTTX_CopyText,TRUE,TAG_DONE);
										guichange=1;
									}
									break;
								case GADID_DRAWMODE:	sfxprefs_sbuf.drawmode	=icode;break;
								case GADID_SAFEMODE:	sfxprefs_sbuf.safemode	=icode;break;
								case GADID_STORAGE:		sfxprefs_sbuf.storage	=icode+1;break;
								case GADID_RASTX:		sfxprefs_sbuf.rasterx	=((prGadgets[GADIX_RASTX    ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_RASTY:		sfxprefs_sbuf.rastery	=((prGadgets[GADIX_RASTY    ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_MAXLINES:	sfxprefs_sbuf.maxlines	=((prGadgets[GADIX_MAXLINES ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_SCAX:		sfxprefs_sbuf.scalax	=((prGadgets[GADIX_SCAX     ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_SCAY:		sfxprefs_sbuf.scalay	=((prGadgets[GADIX_SCAY     ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_RMSLINES:	sfxprefs_sbuf.rmslines	=((prGadgets[GADIX_RMSLINES ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_AVGLINES:	sfxprefs_sbuf.avglines	=((prGadgets[GADIX_AVGLINES ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_QUICKDRAW:	sfxprefs_sbuf.quickdraw	=((prGadgets[GADIX_QUICKDRAW]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
								case GADID_SCAXUNIT:
									switch(icode) {
										case 0:	sfxprefs_sbuf.unitx=ATME_SAMPLES;break;
										case 1:	sfxprefs_sbuf.unitx=ATME_H_M_S_MS;break;
										case 2:	sfxprefs_sbuf.unitx=ATME_H_M_S_MF;break;
										case 3:	sfxprefs_sbuf.unitx=ATME_H_M_S_PF;break;
										case 4:	sfxprefs_sbuf.unitx=ATME_H_M_S_NF;break;
									}
									break;
								case GADID_SCAYUNIT:
									switch(icode) {
										case 0:	sfxprefs_sbuf.unity=AMP_FC;break;
										case 1:	sfxprefs_sbuf.unity=AMP_ABSPROZ;break;
										case 2:	sfxprefs_sbuf.unity=AMP_ABSPROM;break;
										case 3:	sfxprefs_sbuf.unity=AMP_LEVEL;break;
										case 4:	sfxprefs_sbuf.unity=AMP_DEZIBEL;break;
									}
									break;
								case GADID_SIZEMODE:
									switch((sfxprefs_sbuf.mode=icode)) {
										case 0:			/* absolut */
											sfxprefs_sbuf.xs1=(UWORD)(((double)(sfxprefs_sbuf.xs1*RunTime.clientrect.Width)/1000.0)+0.5);
											sfxprefs_sbuf.ys1=(UWORD)(((double)(sfxprefs_sbuf.ys1*RunTime.clientrect.Height)/1000.0)+0.5);
											sfxprefs_sbuf.xs2=(UWORD)(((double)(sfxprefs_sbuf.xs2*RunTime.clientrect.Width)/1000.0)+0.5);
											sfxprefs_sbuf.ys2=(UWORD)(((double)(sfxprefs_sbuf.ys2*RunTime.clientrect.Height)/1000.0)+0.5);
											break;
										case 1:			/* relativ */
											sfxprefs_sbuf.xs1=(UWORD)(((sfxprefs_sbuf.xs1*1000.0)/(double)RunTime.clientrect.Width)+0.5);
											sfxprefs_sbuf.ys1=(UWORD)(((sfxprefs_sbuf.ys1*1000.0)/(double)RunTime.clientrect.Height)+0.5);
											sfxprefs_sbuf.xs2=(UWORD)(((sfxprefs_sbuf.xs2*1000.0)/(double)RunTime.clientrect.Width)+0.5);
											sfxprefs_sbuf.ys2=(UWORD)(((sfxprefs_sbuf.ys2*1000.0)/(double)RunTime.clientrect.Height)+0.5);
											break;
									}
									GT_SetGadgetAttrs(prGadgets[GADIX_NX],prwin,NULL,GTIN_Number,sfxprefs_sbuf.xs1,TAG_DONE);
									GT_SetGadgetAttrs(prGadgets[GADIX_NY],prwin,NULL,GTIN_Number,sfxprefs_sbuf.ys1,TAG_DONE);
									GT_SetGadgetAttrs(prGadgets[GADIX_ZX],prwin,NULL,GTIN_Number,sfxprefs_sbuf.xs2,TAG_DONE);
									GT_SetGadgetAttrs(prGadgets[GADIX_ZY],prwin,NULL,GTIN_Number,sfxprefs_sbuf.ys2,TAG_DONE);
									break;
								case GADID_NX:
									switch(sfxprefs_sbuf.mode) {
										case 0:			/* absolut */
											CheckGad_int(prGadgets[GADIX_NX],prwin,&lval,1,32,RunTime.clientrect.Width);sfxprefs_sbuf.xs1=(UWORD)lval;
											break;
										case 1:			/* relativ */
											CheckGad_int(prGadgets[GADIX_NX],prwin,&lval,1,5,1000);sfxprefs_sbuf.xs1=(UWORD)lval;
											break;
									}
									break;
								case GADID_NY:
									switch(sfxprefs_sbuf.mode) {
										case 0:			/* absolut */
											CheckGad_int(prGadgets[GADIX_NY],prwin,&lval,1,16,RunTime.clientrect.Height);sfxprefs_sbuf.ys1=(UWORD)lval;
											break;
										case 1:			/* relativ */
											CheckGad_int(prGadgets[GADIX_NY],prwin,&lval,1,5,1000);sfxprefs_sbuf.ys1=(UWORD)lval;
											break;
									}
									break;
								case GADID_ZX:
									switch(sfxprefs_sbuf.mode) {
										case 0:			/* absolut */
											CheckGad_int(prGadgets[GADIX_ZX],prwin,&lval,1,32,RunTime.clientrect.Width);sfxprefs_sbuf.xs2=(UWORD)lval;
											break;
										case 1:			/* relativ */
											CheckGad_int(prGadgets[GADIX_ZX],prwin,&lval,1,5,1000);sfxprefs_sbuf.xs2=(UWORD)lval;
											break;
									}
									break;
								case GADID_ZY:
									switch(sfxprefs_sbuf.mode) {
										case 0:			/* absolut */
											CheckGad_int(prGadgets[GADIX_ZY],prwin,&lval,1,16,RunTime.clientrect.Height);sfxprefs_sbuf.ys2=(UWORD)lval;
										case 1:			/* relativ */
											CheckGad_int(prGadgets[GADIX_ZY],prwin,&lval,1,5,1000);sfxprefs_sbuf.ys2=(UWORD)lval;
											break;
									}
									break;
								case GADID_NOLINES:
									sfxprefs_sbuf.nolines	=((prGadgets[GADIX_NOLINES]->Flags&GFLG_SELECTED)==GFLG_SELECTED);
									GT_SetGadgetAttrs(prGadgets[GADIX_THRSIZE],prwin,NULL,GA_Disabled,!sfxprefs_sbuf.nolines,TAG_DONE);
									break;
								case GADID_THRSIZE:
									CheckGad_int(prGadgets[GADIX_THRSIZE],prwin,&lval,0,0,0);sfxprefs_sbuf.thrsize=lval;
									break;
								case GADID_COPYSTR:
									strcpy(sfxprefs_sbuf.copystr,((struct StringInfo *)(prGadgets[GADIX_COPYSTR]->SpecialInfo))->Buffer);
									break;
								case GADID_COPYGRAB:
									sprintf(sfxprefs_sbuf.copystr,"(C) by %s",RunTime.username);
									GT_SetGadgetAttrs(prGadgets[GADIX_COPYSTR],prwin,NULL,GTST_String,sfxprefs_sbuf.copystr,TAG_DONE);
									break;
								case GADID_AUTHSTR:
									strcpy(sfxprefs_sbuf.authstr,((struct StringInfo *)(prGadgets[GADIX_AUTHSTR]->SpecialInfo))->Buffer);
									break;
								case GADID_AUTHGRAB:
									strcpy(sfxprefs_sbuf.authstr,RunTime.username);
									GT_SetGadgetAttrs(prGadgets[GADIX_AUTHSTR],prwin,NULL,GTST_String,sfxprefs_sbuf.authstr,TAG_DONE);
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
									sfxprefs_sbuf=tprefs_sbuf;
									quit=1;
									break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) {
							sfxprefs_sbuf=tprefs_sbuf;
							quit=1;
						}
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)
							ShowHelp("html/nodes/node01.06.02.html");
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
	if(guichange) {
		Forbid();
		if(RunTime.scafont)		CloseFont(RunTime.scafont);
		RunTime.ScaFont->ta_Name =sfxprefs_sbuf.scafont_name;
		RunTime.ScaFont->ta_YSize=sfxprefs_sbuf.scafont_size;
		if(!(RunTime.scafont=OpenDiskFont(RunTime.ScaFont))) { sprintf(sfxmsg,GetString(errAccessFile),RunTime.ScaFont->ta_Name);Message(errAccessFile,sfxmsg,__FILE__,__LINE__); }
		Permit();
		
		foreach(node,&BufferList) {							/* alle sichtbaren Samples neuzeichnen */
			LayoutSample((SInfo *)node,0);
		}
	}
	return(0);
}

//-- eof ----------------------------------------------------------------------
