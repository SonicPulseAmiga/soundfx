/****h* SoundFX/SFX-WinSmpOpts.c [4.2] *
*
*  NAME
*    SFX-WinSmpOpts.c
*  COPYRIGHT
*    $VER: SFX-WinSmpOpts.c 4.2 (19.02.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    sample options window - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    16.Jul.1998
*  MODIFICATION HISTORY
*    19.Feb.2003    V 4.2   most recent version
*    07.Sep.2000    V 4.1   most recent version
*    31.Jul.2000    V 4.00  most recent version
*    25.May.1999    V 3.81  most recent version
*    12.Aug.1998    V 3.70  most recent version
*    16.Jul.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#define SFX_WinSmpOpts_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void SampleOptions(void);
void Refr_SampleOptions_Length(struct Gadget *gadgets[],struct Window *win,SInfo *si);

//-- defines ------------------------------------------------------------------

#define GADID_OKAY          (0)
#define GADID_RESET         (GADID_OKAY+1)

#define GADID_SRCSEL        (GADID_RESET+1)
#define GADIX_SRCSEL        (0)

#define GADID_SRC           (GADID_SRCSEL+1)
#define GADIX_SRC           (GADIX_SRCSEL+1)

#define GADID_NAME          (GADID_SRC+1)                   // Samplebuffer settings
#define GADIX_NAME          (GADIX_SRC+1)

#define GADID_LOOP          (GADID_NAME+1)
#define GADIX_LOOP          (GADIX_NAME+1)

#define GADID_DRAWMODE      (GADID_LOOP+1)
#define GADIX_DRAWMODE      (GADIX_LOOP+1)

#define GADID_STORAGE       (GADID_DRAWMODE+1)
#define GADIX_STORAGE       (GADIX_DRAWMODE+1)

#define GADID_RASTX         (GADID_STORAGE+1)
#define GADIX_RASTX         (GADIX_STORAGE+1)

#define GADID_RASTY         (GADID_RASTX+1)
#define GADIX_RASTY         (GADIX_RASTX+1)

#define GADID_MAXLINES      (GADID_RASTY+1)
#define GADIX_MAXLINES      (GADIX_RASTY+1)

#define GADID_SCAX          (GADID_MAXLINES+1)
#define GADIX_SCAX          (GADIX_MAXLINES+1)

#define GADID_SCAY          (GADID_SCAX+1)
#define GADIX_SCAY          (GADIX_SCAX+1)

#define GADID_RMSLINES      (GADID_SCAY+1)
#define GADIX_RMSLINES      (GADIX_SCAY+1)

#define GADID_SCAXUNIT      (GADID_RMSLINES+1)
#define GADIX_SCAXUNIT      (GADIX_RMSLINES+1)

#define GADID_SCAYUNIT      (GADID_SCAXUNIT+1)
#define GADIX_SCAYUNIT      (GADIX_SCAXUNIT+1)

#define GADID_AVGLINES      (GADID_SCAYUNIT+1)
#define GADIX_AVGLINES      (GADIX_SCAYUNIT+1)

#define GADID_QUICKDRAW     (GADID_AVGLINES+1)
#define GADIX_QUICKDRAW     (GADID_AVGLINES+1)

#define GADID_CHAN1         (GADID_QUICKDRAW+1)
#define GADIX_CHAN1         (GADIX_QUICKDRAW+1)

#define GADID_CHAN2         (GADID_CHAN1+1)
#define GADIX_CHAN2         (GADIX_CHAN1+1)

#define GADID_CHAN3         (GADID_CHAN2+1)
#define GADIX_CHAN3         (GADIX_CHAN2+1)

#define GADID_CHAN4         (GADID_CHAN3+1)
#define GADIX_CHAN4         (GADIX_CHAN3+1)

#define GADID_SLEN          (GADID_CHAN4+1)
#define GADIX_SLEN          (GADIX_CHAN4+1)

#define GADID_SLENUNIT      (GADID_SLEN+1)
#define GADIX_SLENUNIT      (GADIX_SLEN+1)

#define GADID_SRATPOPUP     (GADID_SLENUNIT+1)

#define GADID_SRAT1         (GADID_SRATPOPUP+1)
#define GADIX_SRAT1         (GADIX_SLENUNIT+1)

#define GADID_SRAT2         (GADID_SRAT1+1)
#define GADIX_SRAT2         (GADIX_SRAT1+1)

#define GADID_COPY          (GADID_SRAT2+1)                 // Samplecomments
#define GADIX_COPY          (GADIX_SRAT2+1)

#define GADID_AUTH          (GADID_COPY+1)
#define GADIX_AUTH          (GADIX_COPY+1)

#define GADID_ANNO          (GADID_AUTH+1)
#define GADIX_ANNO          (GADIX_AUTH+1)

#define LAST_GADIX          (GADIX_ANNO+1)

//-- definitions --------------------------------------------------------------

void SampleOptions(void)
{
	register UBYTE i;
	struct NewGadget ng;
	struct Window *opWin;
	struct Gadget *g;
	struct Gadget *opGList=NULL;
	struct Gadget *opGadgets[LAST_GADIX];
	struct RastPort *oprp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	LONG nslen;
	UWORD icode;
	int mousex,mousey;
	BOOL dummy;
	NotifyTask nt;
	LONG sigin,sigout;
	UBYTE runing=TRUE;
	SInfo *si;
	//ULONG suc;
	LONG rate;
	double frq;
	UBYTE xunit,yunit;
	UBYTE *DrawModeLabels[]={ "Lines","Dots","DotsAbs","DotsHQ","Filled","FilledAbs","FilledHQ",NULL };
	UBYTE *LoopLabels[]=    { "None","Forward",NULL };	// "PingPong"
	UBYTE *StorageLabels[]= { "Memory","Drive",NULL };
	UBYTE *XUnitLabels[]=   { "Samples","Time","SMPTE 24 fps","SMPTE 25 fps","SMPTE 30 fps",NULL };
	UBYTE *YUnitLabels[]=   { "Factor","Per cent","Per thousand","Level","Dezibel",NULL };
	char key[4];
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(1);
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(5);
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(3);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+RunTime.scx[200];
	UWORD winw=xs2+3+RunTime.scx[386];
	UWORD xh;
	UBYTE redraw;

	Rate2KeyFrq(RunTime.aktbuf->srat,key,NULL);
	switch(RunTime.aktbuf->unitx) {
		case ATME_SAMPLES:  xunit=0;break;
		case ATME_H_M_S_MS: xunit=1;break;
		case ATME_H_M_S_MF: xunit=2;break;
		case ATME_H_M_S_PF: xunit=3;break;
		case ATME_H_M_S_NF: xunit=4;break;
		UNEXPECTED_DEFAULT
	}
	switch(RunTime.aktbuf->unity) {
		case AMP_FC:        yunit=0;break;
		case AMP_ABSPROZ:   yunit=1;break;
		case AMP_ABSPROM:   yunit=2;break;
		case AMP_LEVEL:     yunit=3;break;
		case AMP_DEZIBEL:   yunit=4;break;
		UNEXPECTED_DEFAULT
	}

	nt.task=FindTask(NULL);
	if((nt.sig=AllocSignal(-1))!=-1) {
		AddTail(&RunTime.gblist,&nt);
		si=RunTime.aktbuf;
		if(g=CreateContext(&opGList)) {
			ng.ng_LeftEdge      =xs1+RunTime.scx[193];                                  /* Commands */
			ng.ng_TopEdge       =ys4+RunTime.scy[2];
			ng.ng_Width         =RunTime.scx[80];
			ng.ng_Height        =RunTime.scy[11];
			ng.ng_GadgetText    ="_Okay";
			ng.ng_TextAttr      =RunTime.ScrFontB;
			ng.ng_GadgetID      =GADID_OKAY;
			ng.ng_Flags         =PLACETEXT_IN;
			ng.ng_VisualInfo    =RunTime.VisualInfo;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

			ng.ng_LeftEdge      =xs1+RunTime.scx[313];
			ng.ng_TextAttr      =RunTime.ScrFont;
			ng.ng_GadgetText    ="_Reset";
			ng.ng_GadgetID      =GADID_RESET;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

			if(RunTime.sfxprefs_misc->srcselect) {                                  /* Source-Select & Rename */
				ng.ng_LeftEdge      =xs1+RunTime.scx[37];
				ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET];
				ng.ng_Width         =RunTime.scx[246];
				ng.ng_GadgetText    ="";
				ng.ng_GadgetID      =GADID_SRCSEL;
				g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,RunTime.barray,GTCY_Active,GetIndexByNode(RunTime.blist,(struct Node *)(si)),TAG_DONE);
				opGadgets[GADIX_SRCSEL]=g;
				opGadgets[GADIX_SRC]=0;

				ng.ng_LeftEdge      =xs1+RunTime.scx[300];
				ng.ng_Width         =RunTime.scx[250];
				ng.ng_GadgetText    ="";
				ng.ng_GadgetID      =GADID_NAME;
				g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,50,GTST_String,((struct Node *)si)->ln_Name,TAG_DONE);
				opGadgets[GADIX_NAME]=g;
			}
			else {
				ng.ng_LeftEdge      =xs1+RunTime.scx[37];           /* Src */
				ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET];
				ng.ng_Width         =RunTime.scx[15];
				ng.ng_TextAttr      =RunTime.SigFont;
				ng.ng_GadgetText    =IMAGE_PopUp;
				ng.ng_GadgetID      =GADID_SRCSEL;
				g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
				opGadgets[GADIX_SRCSEL]=g;

				opGadgets[GADIX_SRC]=0;

				ng.ng_LeftEdge      =xs1+RunTime.scx[53];
				ng.ng_Width         =RunTime.scx[331];
				ng.ng_TextAttr      =RunTime.ScrFont;
				ng.ng_GadgetText    ="";
				ng.ng_GadgetID      =GADID_NAME;
				g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,50,GTST_String,((struct Node *)si)->ln_Name,TAG_DONE);
				opGadgets[GADIX_NAME]=g;
			}

			ng.ng_LeftEdge      =xs1+RunTime.scx[65];                   /* Sample Parameter */
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET];
			ng.ng_Width         =RunTime.scx[104];
			ng.ng_GadgetText    ="Loop";
			ng.ng_GadgetID      =GADID_LOOP;
			ng.ng_Flags         =PLACETEXT_LEFT;
			g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,LoopLabels,GTCY_Active,si->loop,TAG_DONE);
			opGadgets[GADIX_LOOP]=g;

			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET+12];
			ng.ng_GadgetText    ="Storage";
			ng.ng_GadgetID      =GADID_STORAGE;
			if(RunTime.status!=STAT_DEMO) g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,StorageLabels,GTCY_Active,si->storage-1,TAG_DONE);
			else                          g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,StorageLabels,GTCY_Active,si->storage-1,GA_Disabled,TRUE,TAG_DONE);
			opGadgets[GADIX_STORAGE]=g;

			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET+24];
			ng.ng_Width         =RunTime.scx[26];
			ng.ng_GadgetText    ="Channels";
			ng.ng_GadgetID      =GADID_CHAN1;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,(si->channelmask&1),GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_CHAN1]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[91];
			ng.ng_GadgetText    ="";
			ng.ng_GadgetID      =GADID_CHAN2;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,(si->channelmask&2),GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_CHAN2]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[117];
			ng.ng_GadgetText    ="";
			ng.ng_GadgetID      =GADID_CHAN3;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,(si->channelmask&4),GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_CHAN3]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[143];
			ng.ng_GadgetText    ="";
			ng.ng_GadgetID      =GADID_CHAN4;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,(si->channelmask&8),GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_CHAN4]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[65];
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET+36];
			ng.ng_Width         =RunTime.scx[60];
			ng.ng_GadgetText    ="SLen";
			ng.ng_GadgetID      =GADID_SLEN;
			g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,9,GTIN_Number,si->slen,TAG_DONE);
			opGadgets[GADIX_SLEN]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[126];
			ng.ng_Width         =RunTime.scx[65];
			ng.ng_GadgetText    ="";
			ng.ng_GadgetID      =GADID_SLENUNIT;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,TAG_DONE);
			opGadgets[GADIX_SLENUNIT]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[65];
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET+48];
			ng.ng_Width         =RunTime.scx[15];
			ng.ng_TextAttr      =RunTime.SigFont;
			ng.ng_GadgetText    =IMAGE_PopUp;
			ng.ng_GadgetID      =GADID_SRATPOPUP;
			ng.ng_Flags         =PLACETEXT_IN;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

			ng.ng_LeftEdge      =xs1+RunTime.scx[81];
			ng.ng_Width         =RunTime.scx[65];
			ng.ng_TextAttr      =RunTime.ScrFont;
			ng.ng_GadgetText    ="";
			ng.ng_GadgetID      =GADID_SRAT1;
			g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,6,GTIN_Number,si->srat,TAG_DONE);
			opGadgets[GADIX_SRAT1]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[146];
			ng.ng_Width         =RunTime.scx[45];
			ng.ng_GadgetText    ="";
			ng.ng_GadgetID      =GADID_SRAT2;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,3,GTST_String,key,TAG_DONE);
			opGadgets[GADIX_SRAT2]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[79];                   /* Graphic Options */
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET];
			ng.ng_Width         =RunTime.scx[104];
			ng.ng_GadgetText    ="Draw Mode";
			ng.ng_GadgetID      =GADID_DRAWMODE;
			ng.ng_Flags         =PLACETEXT_LEFT;
			g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,DrawModeLabels,GTCY_Active,si->drawmode,TAG_DONE);
			opGadgets[GADIX_DRAWMODE]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[273];
			ng.ng_Width         =RunTime.scx[26];
			ng.ng_GadgetText    ="Quick Draw";
			ng.ng_GadgetID      =GADID_QUICKDRAW;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,si->quickdraw,GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_QUICKDRAW]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[79];
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET+12];
			ng.ng_Width         =RunTime.scx[26];
			ng.ng_GadgetText    ="RastX";
			ng.ng_GadgetID      =GADID_RASTX;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,si->rasterx,GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_RASTX]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[176];
			ng.ng_GadgetText    ="AxisX";
			ng.ng_GadgetID      =GADID_SCAX;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,si->scalax,GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_SCAX]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[257];
			ng.ng_Width         =RunTime.scx[120];
			ng.ng_GadgetText    ="UnitX";
			ng.ng_GadgetID      =GADID_SCAXUNIT;
			g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,XUnitLabels,GTCY_Active,xunit,TAG_DONE);
			opGadgets[GADIX_SCAXUNIT]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[79];
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET+24];
			ng.ng_Width         =RunTime.scx[26];
			ng.ng_GadgetText    ="RastY";
			ng.ng_GadgetID      =GADID_RASTY;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,si->rastery,GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_RASTY]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[176];
			ng.ng_GadgetText    ="AxisY";
			ng.ng_GadgetID      =GADID_SCAY;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,si->scalay,GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_SCAY]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[257];
			ng.ng_Width         =RunTime.scx[120];
			ng.ng_GadgetText    ="UnitY";
			ng.ng_GadgetID      =GADID_SCAYUNIT;
			g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,YUnitLabels,GTCY_Active,yunit,TAG_DONE);
			opGadgets[GADIX_SCAYUNIT]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[79];
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET+36];
			ng.ng_Width         =RunTime.scx[26];
			ng.ng_GadgetText    ="Max-Lines";
			ng.ng_GadgetID      =GADID_MAXLINES;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,si->maxlines,GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_MAXLINES]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[176];
			ng.ng_GadgetText    ="RMS-Lines";
			ng.ng_GadgetID      =GADID_RMSLINES;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,si->rmslines,GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_RMSLINES]=g;

			ng.ng_LeftEdge      =xs2+RunTime.scx[273];
			ng.ng_GadgetText    ="Avg-Lines";
			ng.ng_GadgetID      =GADID_AVGLINES;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,si->avglines,GTCB_Scaled,TRUE,TAG_DONE);
			opGadgets[GADIX_AVGLINES]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[89];                   /* Information */
			ng.ng_TopEdge       =ys3+RunTime.scy[FRMCONTENT_YOFFSET];
			ng.ng_Width         =RunTime.scx[375];
			ng.ng_GadgetText    ="(C)";
			ng.ng_GadgetID      =GADID_COPY;
			ng.ng_Flags         =PLACETEXT_LEFT;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_String,si->cpyr,TAG_DONE);
			opGadgets[GADIX_COPY]=g;

			ng.ng_TopEdge       =ys3+RunTime.scy[FRMCONTENT_YOFFSET+12];
			ng.ng_GadgetText    ="Auth";
			ng.ng_GadgetID      =GADID_AUTH;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_String,si->auth,TAG_DONE);
			opGadgets[GADIX_AUTH]=g;

			ng.ng_TopEdge       =ys3+RunTime.scy[FRMCONTENT_YOFFSET+24];
			ng.ng_GadgetText    ="Anno";
			ng.ng_GadgetID      =GADID_ANNO;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_String,si->anno,TAG_DONE);
			opGadgets[GADIX_ANNO]=g;

			if(g) {
				MemUsage(sfxtitle);
				Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
				Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
				Dialog_Wintags_InnerWidth	=winw;
				Dialog_Wintags_InnerHeight	=winh;
				Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
				Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
				Dialog_Wintags_Gadgets		=(ULONG)opGList;
				Dialog_Wintags_Title		=(ULONG)GetString(strSmpOptions);
				Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
				if(opWin=OpenWindowTagList(NULL,Dialog_Wintags)) {
					if(si->channels<3) GT_SetGadgetAttrs(opGadgets[GADIX_CHAN4],opWin,NULL,GA_Disabled,TRUE,TAG_DONE);
					if(si->channels<2) GT_SetGadgetAttrs(opGadgets[GADIX_CHAN3],opWin,NULL,GA_Disabled,TRUE,TAG_DONE);
					if(si->channels<1) GT_SetGadgetAttrs(opGadgets[GADIX_CHAN2],opWin,NULL,GA_Disabled,TRUE,TAG_DONE);

					GT_RefreshWindow(opWin,NULL);
					oprp=opWin->RPort;SetFont(oprp,RunTime.scrfont);

					SetAPen(oprp,1);
					RText(oprp,"Src",xs1+RunTime.scx[ 29],ys1+RunTime.scy[FRMTEXT_YPOS(1)]);
					RText(oprp,"SRat",xs1+RunTime.scx[ 57],ys2+RunTime.scy[FRMTEXT_YPOS(5)]);

					DrawTitle(oprp,"Sample Parameter"   ,xs1,ys2,RunTime.scx[200]);
					DrawTitle(oprp,"Graphic Options"    ,xs2,ys2,RunTime.scx[386]);
					xh=RunTime.scx[200]+RunTime.scx[386]+1;
					DrawTitle(oprp,"Source"             ,xs1,ys1,xh);
					DrawTitle(oprp,"Information"        ,xs1,ys3,xh);

					DrawGadWB(oprp,xs1,ys2,RunTime.scx[200],ANYFRM_HEIGHT(5));  /* Sample Parameter */
					DrawGadWB(oprp,xs2,ys2,RunTime.scx[386],ANYFRM_HEIGHT(5));  /* Graphic Options */
					DrawGadWB(oprp,xs1,ys1,xh,ANYFRM_HEIGHT(1));                /* Source */
					DrawGadWB(oprp,xs1,ys3,xh,ANYFRM_HEIGHT(3));                /* Strings */
					DrawGadWB(oprp,xs1,ys4,xh,CMDFRM_HEIGHT);                   /* Okay/Reset */

					Refr_SampleOptions_Length(opGadgets,opWin,si);

					sigin=(1L<<opWin->UserPort->mp_SigBit)|(1L<<nt.sig);

					while(runing) {
						sigout=Wait(sigin);
						if(sigout&(1L<<nt.sig)) {
							si=RunTime.aktbuf;
							if(RunTime.sfxprefs_misc->srcselect) {
								GT_SetGadgetAttrs(opGadgets[GADIX_SRCSEL],opWin,NULL,GTCY_Labels,RunTime.barray,GTCY_Active,GetIndexByNode(RunTime.blist,(struct Node *)(si)),TAG_DONE);
							}
							Rate2KeyFrq(si->srat,key,NULL);
							//GT_SetGadgetAttrs(opGadgets[GADIX_SRC      ],opWin,NULL,GTTX_Text,((struct Node *)si)->ln_Name,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_NAME     ],opWin,NULL,GTST_String,((struct Node *)si)->ln_Name,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_DRAWMODE ],opWin,NULL,GTCY_Active ,si->drawmode,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_LOOP     ],opWin,NULL,GTCY_Active ,si->loop,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_STORAGE  ],opWin,NULL,GTCY_Active ,si->storage-1,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_RASTX    ],opWin,NULL,GTCB_Checked,si->rasterx,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_RASTY    ],opWin,NULL,GTCB_Checked,si->rastery,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_MAXLINES ],opWin,NULL,GTCB_Checked,si->maxlines,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_SCAX     ],opWin,NULL,GTCB_Checked,si->scalax,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_SCAY     ],opWin,NULL,GTCB_Checked,si->scalay,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_RMSLINES ],opWin,NULL,GTCB_Checked,si->rmslines,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_SCAXUNIT ],opWin,NULL,GTCY_Active,xunit,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_SCAYUNIT ],opWin,NULL,GTCY_Active,yunit,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_AVGLINES ],opWin,NULL,GTCB_Checked,si->avglines,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_QUICKDRAW],opWin,NULL,GTCB_Checked,si->quickdraw,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_CHAN1    ],opWin,NULL,GTCB_Checked,(si->channelmask&1),GA_Disabled,FALSE,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_CHAN2    ],opWin,NULL,GTCB_Checked,(si->channelmask&2),GA_Disabled,FALSE,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_CHAN3    ],opWin,NULL,GTCB_Checked,(si->channelmask&4),GA_Disabled,FALSE,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_CHAN4    ],opWin,NULL,GTCB_Checked,(si->channelmask&8),GA_Disabled,FALSE,TAG_DONE);
							if(si->channels<3) GT_SetGadgetAttrs(opGadgets[GADIX_CHAN4],opWin,NULL,GA_Disabled,TRUE,TAG_DONE);
							if(si->channels<2) GT_SetGadgetAttrs(opGadgets[GADIX_CHAN3],opWin,NULL,GA_Disabled,TRUE,TAG_DONE);
							if(si->channels<1) GT_SetGadgetAttrs(opGadgets[GADIX_CHAN2],opWin,NULL,GA_Disabled,TRUE,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_SLEN     ],opWin,NULL,GTIN_Number ,si->slen,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_SRAT1    ],opWin,NULL,GTIN_Number ,si->srat,TAG_DONE);
							GT_SetGadgetAttrs(opGadgets[GADIX_SRAT2    ],opWin,NULL,GTST_String ,key,TAG_DONE);
						}
						if(sigout&(1L<<opWin->UserPort->mp_SigBit)) {
							while(imsg=GT_GetIMsg(opWin->UserPort)) {
								iclass  =imsg->Class;
								iadr    =imsg->IAddress;
								icode   =imsg->Code;
								mousex  =imsg->MouseX;
								mousey  =imsg->MouseY;
								GT_ReplyIMsg(imsg);
								NG_HandleEvents(iclass,icode,mousex,mousey,iadr,opWin,&dummy);
								if(iclass==IDCMP_GADGETUP) {
									switch((int)(((struct Gadget *)iadr)->GadgetID)) {
										case GADID_OKAY:
											runing=FALSE;break;
										case GADID_RESET:
											redraw=FALSE;
											if(si->drawmode !=sfxprefs_sbuf.drawmode)  { si->drawmode    =sfxprefs_sbuf.drawmode;redraw=TRUE; }
											if(si->rasterx  !=sfxprefs_sbuf.rasterx)   { si->rasterx     =sfxprefs_sbuf.rasterx;redraw=TRUE; }
											if(si->rastery  !=sfxprefs_sbuf.rastery)   { si->rastery     =sfxprefs_sbuf.rastery;redraw=TRUE; }
											if(si->maxlines !=sfxprefs_sbuf.maxlines)  { si->maxlines    =sfxprefs_sbuf.maxlines;redraw=TRUE; }
											if(si->avglines !=sfxprefs_sbuf.avglines)  { si->avglines    =sfxprefs_sbuf.avglines;redraw=TRUE; }
											if(si->rmslines !=sfxprefs_sbuf.rmslines)  { si->rmslines    =sfxprefs_sbuf.rmslines;redraw=TRUE; }
											if(si->quickdraw!=sfxprefs_sbuf.quickdraw)   si->quickdraw   =sfxprefs_sbuf.quickdraw;
											if(si->unitx    !=sfxprefs_sbuf.unitx)     { si->unitx       =sfxprefs_sbuf.unitx;redraw=TRUE; }
											if(si->unity    !=sfxprefs_sbuf.unity)     { si->unity       =sfxprefs_sbuf.unity;redraw=TRUE; }
											if(si->scalax   !=sfxprefs_sbuf.scalax)    { si->scalax      =sfxprefs_sbuf.scalax;redraw=FALSE; }
											if(si->scalay   !=sfxprefs_sbuf.scalay)    { si->scalay      =sfxprefs_sbuf.scalay;redraw=FALSE; }
											// si->storage  =sfxprefs_sbuf.storage;
											switch(si->unitx) {
												case ATME_SAMPLES:  xunit=0;break;
												case ATME_H_M_S_MS: xunit=1;break;
												case ATME_H_M_S_MF: xunit=2;break;
												case ATME_H_M_S_PF: xunit=3;break;
												case ATME_H_M_S_NF: xunit=4;break;
												UNEXPECTED_DEFAULT
											}
											switch(si->unity) {
												case AMP_FC:        yunit=0;break;
												case AMP_ABSPROZ:   yunit=1;break;
												case AMP_ABSPROM:   yunit=2;break;
												case AMP_LEVEL:     yunit=3;break;
												case AMP_DEZIBEL:   yunit=4;break;
												UNEXPECTED_DEFAULT
											}
											GT_SetGadgetAttrs(opGadgets[GADIX_DRAWMODE ],opWin,NULL,GTCY_Active ,si->drawmode,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_STORAGE  ],opWin,NULL,GTCB_Checked,si->storage-1,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_RASTX    ],opWin,NULL,GTCB_Checked,si->rasterx,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_RASTY    ],opWin,NULL,GTCB_Checked,si->rastery,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_MAXLINES ],opWin,NULL,GTCB_Checked,si->maxlines,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_SCAX     ],opWin,NULL,GTCB_Checked,si->scalax,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_SCAY     ],opWin,NULL,GTCB_Checked,si->scalay,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_RMSLINES ],opWin,NULL,GTCB_Checked,si->rmslines,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_SCAXUNIT ],opWin,NULL,GTCY_Active,xunit,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_SCAYUNIT ],opWin,NULL,GTCY_Active,yunit,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_AVGLINES ],opWin,NULL,GTCB_Checked,si->avglines,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_QUICKDRAW],opWin,NULL,GTCB_Checked,si->quickdraw,TAG_DONE);
											LayoutSample(si,0);
											if(redraw) DrawSample(si,0);
											break;
										case GADID_SRCSEL:
											i=0;
											if(RunTime.sfxprefs_misc->srcselect) {
												if(icode!=GetIndexByNode(RunTime.blist,(struct Node *)(si))) {
													si=RunTime.aktbuf=(SInfo *)GetNodeByIndex(RunTime.blist,icode);
													i=1;
												}
											}
											else {
												if(si=GetListEntry()) {
													RunTime.aktbuf=si;
													i=1;
												}
											}
											if(i) {
												Rate2KeyFrq(si->srat,key,NULL);
												//GT_SetGadgetAttrs(opGadgets[GADIX_SRC      ],opWin,NULL,GTTX_Text,((struct Node *)si)->ln_Name,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_NAME     ],opWin,NULL,GTST_String,((struct Node *)si)->ln_Name,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_DRAWMODE ],opWin,NULL,GTCY_Active ,si->drawmode,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_LOOP     ],opWin,NULL,GTCY_Active ,si->loop,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_STORAGE  ],opWin,NULL,GTCY_Active ,si->storage-1,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_RASTX    ],opWin,NULL,GTCB_Checked,si->rasterx,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_RASTY    ],opWin,NULL,GTCB_Checked,si->rastery,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_MAXLINES ],opWin,NULL,GTCB_Checked,si->maxlines,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_SCAX     ],opWin,NULL,GTCB_Checked,si->scalax,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_SCAY     ],opWin,NULL,GTCB_Checked,si->scalay,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_RMSLINES ],opWin,NULL,GTCB_Checked,si->rmslines,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_SCAXUNIT ],opWin,NULL,GTCY_Active,xunit,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_SCAYUNIT ],opWin,NULL,GTCY_Active,yunit,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_AVGLINES ],opWin,NULL,GTCB_Checked,si->avglines,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_QUICKDRAW],opWin,NULL,GTCB_Checked,si->quickdraw,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_CHAN1    ],opWin,NULL,GTCB_Checked,(si->channelmask&1),GA_Disabled,FALSE,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_CHAN2    ],opWin,NULL,GTCB_Checked,(si->channelmask&2),GA_Disabled,FALSE,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_CHAN3    ],opWin,NULL,GTCB_Checked,(si->channelmask&4),GA_Disabled,FALSE,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_CHAN4    ],opWin,NULL,GTCB_Checked,(si->channelmask&8),GA_Disabled,FALSE,TAG_DONE);
												if(si->channels<3) GT_SetGadgetAttrs(opGadgets[GADIX_CHAN4],opWin,NULL,GA_Disabled,TRUE,TAG_DONE);
												if(si->channels<2) GT_SetGadgetAttrs(opGadgets[GADIX_CHAN3],opWin,NULL,GA_Disabled,TRUE,TAG_DONE);
												if(si->channels<1) GT_SetGadgetAttrs(opGadgets[GADIX_CHAN2],opWin,NULL,GA_Disabled,TRUE,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_SLEN     ],opWin,NULL,GTIN_Number ,si->slen,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_SRAT1    ],opWin,NULL,GTIN_Number ,si->srat,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_SRAT2    ],opWin,NULL,GTST_String ,key,TAG_DONE);
											}
											break;
										case GADID_NAME:
											SetName(si,((struct StringInfo *)(opGadgets[GADIX_NAME]->SpecialInfo))->Buffer);SetTitle(si);
											if(RunTime.sfxprefs_misc->srcselect) {
												GT_SetGadgetAttrs(opGadgets[GADIX_SRCSEL   ],opWin,NULL,GTCY_Labels,RunTime.barray,GTCY_Active,GetIndexByNode(RunTime.blist,(struct Node *)(si)),TAG_DONE);
											}
											Refr_ToolbarBuffer();
											break;
										case GADID_LOOP:
											si->loop=icode;
											if(si->loop) {
												if(si->loopl==0) {
													si->loops=0;
													si->loopl=si->slen-1;
												}
												RunTime.rngmode=RNGMD_LOOP;
											}
											DrawLoop(si);
											Refr_ToolbarRange();
											Refr_ContextMenu();
											break;
										case GADID_DRAWMODE:
											si->drawmode=icode;
											Request(&NullReq,opWin);
											SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											DrawSample(si,0);
											EndRequest(&NullReq,opWin);
											SetWindowPointer(opWin,TAG_DONE);
											break;
										case GADID_STORAGE:
#ifdef CHECK_REG
											if(!(keycounter%23)) SetKeyStatus_1;
											if(!(keycounter%27)) SetKeyStatus_2;
											if(IsKeyStatusOkay) {
#endif
												if(!si->lockct) {
													UBYTE res=TRUE;
													Request(&NullReq,opWin);
													SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
													switch(icode+1) {
														case VM_MEMORY: res=ConvDrv2Mem(si);break;
														case VM_DRIVE:  res=ConvMem2Drv(si);break;
													}
													if(!res) {
														GT_SetGadgetAttrs(opGadgets[GADIX_STORAGE  ],opWin,NULL,GTCY_Active,si->storage-1,TAG_DONE);
													}
													else {
														SetTitle(si);
													}
													EndRequest(&NullReq,opWin);
													SetWindowPointer(opWin,TAG_DONE);
												}
#ifdef CHECK_REG
											}
#endif
											break;
										case GADID_RASTX:       si->rasterx  =((opGadgets[GADIX_RASTX    ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);DrawSample(si,0);break;
										case GADID_RASTY:       si->rastery  =((opGadgets[GADIX_RASTY    ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);DrawSample(si,0);break;
										case GADID_SCAX:        si->scalax   =((opGadgets[GADIX_SCAX     ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);LayoutSample(si,0);break;
										case GADID_SCAY:        si->scalay   =((opGadgets[GADIX_SCAY     ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);LayoutSample(si,0);break;
										case GADID_QUICKDRAW:   si->quickdraw=((opGadgets[GADIX_QUICKDRAW]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
										case GADID_MAXLINES:
											si->maxlines =((opGadgets[GADIX_MAXLINES ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);
											Request(&NullReq,opWin);
											SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											if(si->maxlines) RecalcSampleLines(si);
											DrawSample(si,0);
											EndRequest(&NullReq,opWin);
											SetWindowPointer(opWin,TAG_DONE);
											break;
										case GADID_RMSLINES:
											si->rmslines =((opGadgets[GADIX_RMSLINES ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);
											Request(&NullReq,opWin);
											SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											if(si->rmslines) RecalcSampleLines(si);
											DrawSample(si,0);
											EndRequest(&NullReq,opWin);
											SetWindowPointer(opWin,TAG_DONE);
											break;
										case GADID_AVGLINES:
											si->avglines =((opGadgets[GADIX_AVGLINES ]->Flags&GFLG_SELECTED)==GFLG_SELECTED);
											Request(&NullReq,opWin);
											SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											if(si->avglines) RecalcSampleLines(si);
											DrawSample(si,0);
											EndRequest(&NullReq,opWin);
											SetWindowPointer(opWin,TAG_DONE);
											break;
										case GADID_SCAXUNIT:
											switch(icode) {
												case 0: si->unitx=ATME_SAMPLES;break;
												case 1: si->unitx=ATME_H_M_S_MS;break;
												case 2: si->unitx=ATME_H_M_S_MF;break;
												case 3: si->unitx=ATME_H_M_S_PF;break;
												case 4: si->unitx=ATME_H_M_S_NF;break;
											}
											if(si->scalax) DrawAxisX(si);
											Refr_SampleOptions_Length(opGadgets,opWin,si);
											break;
										case GADID_SCAYUNIT:
											switch(icode) {
												case 0: si->unity=AMP_FC;break;
												case 1: si->unity=AMP_ABSPROZ;break;
												case 2: si->unity=AMP_ABSPROM;break;
												case 3: si->unity=AMP_LEVEL;break;
												case 4: si->unity=AMP_DEZIBEL;break;
											}
											if(si->scalay) DrawAxisY(si);
											break;
										case GADID_CHAN1:
											if(opGadgets[GADIX_CHAN1]->Flags&GFLG_SELECTED) { si->channelmask|=1;si->aktch++; }
											else { si->channelmask&=254;si->aktch--; }
											i=0;
											if(si->channelmask&1) si->chnr[i++]=0;
											if(si->channelmask&2) si->chnr[i++]=1;
											if(si->channelmask&4) si->chnr[i++]=2;
											if(si->channelmask&8) si->chnr[i  ]=3;
											Request(&NullReq,opWin);
											SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											DrawSample(si,0);
											EndRequest(&NullReq,opWin);
											SetWindowPointer(opWin,TAG_DONE);
											break;
										case GADID_CHAN2:
											if(opGadgets[GADIX_CHAN2]->Flags&GFLG_SELECTED) { si->channelmask|=2;si->aktch++; }
											else { si->channelmask&=253;si->aktch--; }
											i=0;
											if(si->channelmask&1) si->chnr[i++]=0;
											if(si->channelmask&2) si->chnr[i++]=1;
											if(si->channelmask&4) si->chnr[i++]=2;
											if(si->channelmask&8) si->chnr[i  ]=3;
											Request(&NullReq,opWin);
											SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											DrawSample(si,0);
											EndRequest(&NullReq,opWin);
											SetWindowPointer(opWin,TAG_DONE);
											break;
										case GADID_CHAN3:
											if(opGadgets[GADIX_CHAN3]->Flags&GFLG_SELECTED) { si->channelmask|=4;si->aktch++; }
											else { si->channelmask&=251;si->aktch--; }
											i=0;
											if(si->channelmask&1) si->chnr[i++]=0;
											if(si->channelmask&2) si->chnr[i++]=1;
											if(si->channelmask&4) si->chnr[i++]=2;
											if(si->channelmask&8) si->chnr[i  ]=3;
											Request(&NullReq,opWin);
											SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											DrawSample(si,0);
											EndRequest(&NullReq,opWin);
											SetWindowPointer(opWin,TAG_DONE);
											break;
										case GADID_CHAN4:
											if(opGadgets[GADIX_CHAN4]->Flags&GFLG_SELECTED) { si->channelmask|=8;si->aktch++; }
											else { si->channelmask&=247;si->aktch--; }
											i=0;
											if(si->channelmask&1) si->chnr[i++]=0;
											if(si->channelmask&2) si->chnr[i++]=1;
											if(si->channelmask&4) si->chnr[i++]=2;
											if(si->channelmask&8) si->chnr[i  ]=3;
											Request(&NullReq,opWin);
											SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											DrawSample(si,0);
											EndRequest(&NullReq,opWin);
											SetWindowPointer(opWin,TAG_DONE);
											break;
										case GADID_SLEN:
											CheckGad_int(opGadgets[GADIX_SLEN],opWin,&nslen,1,2,2147483646);
											Request(&NullReq,opWin);
											SetWindowPointer(opWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
											SetSampleLength(si,nslen);
											EndRequest(&NullReq,opWin);
											SetWindowPointer(opWin,TAG_DONE);
											Refr_SampleOptions_Length(opGadgets,opWin,si);
											break;
										case GADID_SRATPOPUP:
											if(rate=GetPeriode(si->srat,si)) {
												si->srat=rate;
												Rate2KeyFrq(si->srat,key,NULL);
												GT_SetGadgetAttrs(opGadgets[GADIX_SRAT1],opWin,NULL,GTIN_Number,si->srat,TAG_DONE);
												GT_SetGadgetAttrs(opGadgets[GADIX_SRAT2],opWin,NULL,GTST_String,key,TAG_DONE);
												if(RunTime.play && si==RunTime.plsi) SetNewPlaybackRate();
												SetTitle(si);
												if(si->scalax && si->scays && si->unitx!=ATME_SAMPLES) DrawAxisX(si);
												Refr_SampleOptions_Length(opGadgets,opWin,si);
											}
											break;
										case GADID_SRAT1:
											CheckGad_int(opGadgets[GADIX_SRAT1],opWin,&rate,1,1000,100000);si->srat=(ULONG)rate;
											Rate2KeyFrq(si->srat,key,NULL);
											GT_SetGadgetAttrs(opGadgets[GADIX_SRAT2],opWin,NULL,GTST_String,key,TAG_DONE);
											if(RunTime.play && si==RunTime.plsi) SetNewPlaybackRate();
											SetTitle(si);
											if(si->scalax && si->scays && si->unitx!=ATME_SAMPLES) DrawAxisX(si);
											Refr_SampleOptions_Length(opGadgets,opWin,si);
											break;
										case GADID_SRAT2:
											strcpy(key,((struct StringInfo *)(opGadgets[GADIX_SRAT2]->SpecialInfo))->Buffer);
											key[0]=ToUpper(key[0]);key[2]=ToUpper(key[2]);key[3]=0;
											if(!Key2RateFrq(key,&si->srat,&frq)) Rate2KeyFrq(si->srat,key,NULL);
											GT_SetGadgetAttrs(opGadgets[GADIX_SRAT1],opWin,NULL,GTIN_Number,si->srat,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_SRAT2],opWin,NULL,GTST_String,key,TAG_DONE);
											if(RunTime.play && si==RunTime.plsi) SetNewPlaybackRate();
											SetTitle(si);
											if(si->scalax && si->scays && si->unitx!=ATME_SAMPLES) DrawAxisX(si);
											Refr_SampleOptions_Length(opGadgets,opWin,si);
											break;
										case GADID_COPY:
											if(si->cpyr) FreeVec((ULONG *)si->cpyr);
											si->cpyr=StringCopy(((struct StringInfo *)(opGadgets[GADIX_COPY]->SpecialInfo))->Buffer);
											break;
										case GADID_AUTH:
											if(si->auth) FreeVec((ULONG *)si->auth);
											si->auth=StringCopy(((struct StringInfo *)(opGadgets[GADIX_AUTH]->SpecialInfo))->Buffer);
											break;
										case GADID_ANNO:
											if(si->anno) FreeVec((ULONG *)si->anno);
											si->anno=StringCopy(((struct StringInfo *)(opGadgets[GADIX_ANNO]->SpecialInfo))->Buffer);
											break;
									}
								}
								if(iclass==IDCMP_VANILLAKEY) {
									switch(icode) {
										case KEY_ESC:               /* Okay */
										case KEY_ENTER:
										case KEY_RETURN:
										case 'O':
										case 'o':
											runing=FALSE;break;
										case 'R':                   /* Reset */
										case 'r':
											si->drawmode    =sfxprefs_sbuf.drawmode;
											si->rasterx     =sfxprefs_sbuf.rasterx;
											si->rastery     =sfxprefs_sbuf.rastery;
											si->maxlines    =sfxprefs_sbuf.maxlines;
											si->scalax      =sfxprefs_sbuf.scalax;
											si->scalay      =sfxprefs_sbuf.scalay;
											si->rmslines    =sfxprefs_sbuf.rmslines;
											si->unitx       =sfxprefs_sbuf.unitx;
											si->unity       =sfxprefs_sbuf.unity;
											si->avglines    =sfxprefs_sbuf.avglines;
											si->quickdraw   =sfxprefs_sbuf.quickdraw;
											// si->storage  =sfxprefs_sbuf.storage;
											switch(si->unitx) {
												case ATME_SAMPLES:  xunit=0;break;
												case ATME_H_M_S_MS: xunit=1;break;
												case ATME_H_M_S_MF: xunit=2;break;
												case ATME_H_M_S_PF: xunit=3;break;
												case ATME_H_M_S_NF: xunit=4;break;
											}
											switch(si->unity) {
												case AMP_FC:        yunit=0;break;
												case AMP_ABSPROZ:   yunit=1;break;
												case AMP_ABSPROM:   yunit=2;break;
												case AMP_LEVEL:     yunit=3;break;
												case AMP_DEZIBEL:   yunit=4;break;
											}
											GT_SetGadgetAttrs(opGadgets[GADIX_DRAWMODE ],opWin,NULL,GTCY_Active ,si->drawmode,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_STORAGE  ],opWin,NULL,GTCB_Checked,si->storage-1,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_RASTX    ],opWin,NULL,GTCB_Checked,si->rasterx,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_RASTY    ],opWin,NULL,GTCB_Checked,si->rastery,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_MAXLINES ],opWin,NULL,GTCB_Checked,si->maxlines,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_SCAX     ],opWin,NULL,GTCB_Checked,si->scalax,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_SCAY     ],opWin,NULL,GTCB_Checked,si->scalay,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_RMSLINES ],opWin,NULL,GTCB_Checked,si->rmslines,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_SCAXUNIT ],opWin,NULL,GTCY_Active,xunit,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_SCAYUNIT ],opWin,NULL,GTCY_Active,yunit,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_AVGLINES ],opWin,NULL,GTCB_Checked,si->avglines,TAG_DONE);
											GT_SetGadgetAttrs(opGadgets[GADIX_QUICKDRAW],opWin,NULL,GTCB_Checked,si->quickdraw,TAG_DONE);
											LayoutSample(si,0);
											break;
									}
								}
								if(iclass==IDCMP_CLOSEWINDOW) runing=FALSE;
								if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)
									ShowHelp("html/nodes/node01.05.03.html");
							}
						}
					}
					CloseWindow(opWin);
				}
				else Message(errOpenWindow,NULL,__FILE__,__LINE__);
				FreeGadgets(opGList);
			}
			else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateContext,NULL,__FILE__,__LINE__);
		if(((struct Node *)&nt)==RunTime.gblist.lh_Head) RemHead(&RunTime.gblist);
		else if(((struct Node *)&nt)==RunTime.gblist.lh_Tail) RemTail(&RunTime.gblist);
		else Remove((struct Node *)&nt);
		FreeSignal(nt.sig);
	}
	else Message(errAllocateSignal,NULL,__FILE__,__LINE__);
}

void Refr_SampleOptions_Length(struct Gadget *gadgets[],struct Window *win,SInfo *si)
{
	Param param1,param2;

	switch(si->unitx) {
		case ATME_SAMPLES:	sprintf(param2,"%ld",si->slen);break;
		case ATME_H_M_S_MS:
		case ATME_H_M_S_MF:
		case ATME_H_M_S_PF:
		case ATME_H_M_S_NF:
			sprintf(param1,"%lf ms",(double)si->slen/((double)si->srat/1000.0));
			UC_ConvertParameter(param2,param1,UGRP_ABSTIME,si->unitx|DONT_ADD_UNIT_NAME);
			break;
		UNEXPECTED_DEFAULT
	}
	GT_SetGadgetAttrs(gadgets[GADIX_SLENUNIT],win,NULL,GTTX_Text,param2,GTTX_CopyText,TRUE,TAG_DONE);
}

//-- eof ----------------------------------------------------------------------
