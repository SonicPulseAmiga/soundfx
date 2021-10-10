/****h* SoundFX/SFX-WinStatus.c [4.3] *
*
*  NAME
*    SFX-WinStatus.c
*  COPYRIGHT
*    $VER: SFX-WinStatus.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Statusleiste for SFX Pubscreen - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    26.Jan.2000
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    19.Feb.2003	V 4.2	most recent version
*    22.Aug.2000	V 4.1	most recent version
*    01.Jun.2000	V 4.00	most recent version
*    28.Jan.2000	V 3.82	most recent version
*    26.Jan.2000	V 3.70	initial version
*  NOTES
*
*  IDEAS
*    * 9 sample status fields
*      * use a smaller font (trinomic) for the fields
*      * put labels above the fields
*      * calculate sbh as max(sbh_info,sbh_fields)
*      * center the smaller one (sb_info)
*******
*/

#define SFX_WinStatus_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE Open_StatusWin(void);
void Test_StatusWin (ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr);

void Refr_StatusRange(void);

//-- defines ------------------------------------------------------------------

#define GADID_SB_STATUS	0
#define GADIX_SB_STATUS	0

#define GADID_SB_XS		(GADID_SB_STATUS+1)
#define GADIX_SB_XS		(GADIX_SB_STATUS+1)
#define GADID_SB_XE		(GADID_SB_XS+1)
#define GADIX_SB_XE		(GADIX_SB_XS+1)
#define GADID_SB_XL		(GADID_SB_XE+1)
#define GADIX_SB_XL		(GADIX_SB_XE+1)

#define GADID_SB_YS		(GADID_SB_XL+1)
#define GADIX_SB_YS		(GADIX_SB_XL+1)
#define GADID_SB_YE		(GADID_SB_YS+1)
#define GADIX_SB_YE		(GADIX_SB_YS+1)
#define GADID_SB_YL		(GADID_SB_YE+1)
#define GADIX_SB_YL		(GADIX_SB_YE+1)

#define GADID_SB_TX		(GADID_SB_YL+1)
#define GADIX_SB_TX		(GADIX_SB_YL+1)
#define GADID_SB_TY		(GADID_SB_TX+1)
#define GADIX_SB_TY		(GADIX_SB_TX+1)
#define GADID_SB_TSV	(GADID_SB_TY+1)
#define GADIX_SB_TSV	(GADIX_SB_TY+1)

#define LAST_GADIX		(GADIX_SB_TSV+1)

//-- globals ------------------------------------------------------------------

struct Gadget *stGList=NULL;
struct Gadget *stGadgets[LAST_GADIX];

//-- definitions --------------------------------------------------------------

UBYTE Open_StatusWin(void) {
	struct NewGadget ng;
	struct Gadget *g;
	struct RastPort *rp;
	UBYTE ret=TRUE;
	UWORD sbh_info  =RunTime.scy[11]+2+2;					// One Line of Text + 2 Pixel of Windowborder + 2 Pixels of Gadgetborder
	UWORD sbh_fields=(RunTime.statfont->tf_YSize+4)+(RunTime.scafont->tf_YSize+2)+2+2;
	UWORD sbh=max(sbh_info,sbh_fields);
	ULONG rw,sw,fw,x2,y,gw;

	if(g=CreateContext(&stGList)) {
		fw=3+6+3;											// width of inter-group spaces
		gw=/*RunTime.scx[50]*/ (RunTime.statfont->tf_XSize*9)+4;
		rw=(9*gw)+(2*5);									// width of 9 range edit-gadgets + spacing
		sw=(RunTime.clientrect.Width-1)-(fw+rw);
		x2=3+6+sw;											// start of second group
		
		ng.ng_LeftEdge		=3;
		ng.ng_TopEdge		=2;
		ng.ng_Width			=sw;
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_SB_STATUS;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime.VisualInfo;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,TAG_DONE);
		stGadgets[GADIX_SB_STATUS]=g;

		ng.ng_LeftEdge		=x2;
		ng.ng_TopEdge		=2+RunTime.statfont->tf_YSize+1;
		ng.ng_Width			=gw;
		ng.ng_Height		=RunTime.statfont->tf_YSize+4;
		ng.ng_TextAttr		=RunTime.StatFont;
		ng.ng_GadgetID		=GADID_SB_XS;
		ng.ng_Flags			=PLACETEXT_ABOVE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,NULL,GTST_MaxChars,(PARBUFLEN+1),TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=&stringextend2;
		g->Activation|=GACT_STRINGEXTEND;
		stGadgets[GADIX_SB_XS]=g;

		x2+=gw;
		ng.ng_LeftEdge		=x2;
		ng.ng_GadgetID		=GADID_SB_XE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,NULL,GTST_MaxChars,(PARBUFLEN+1),TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=&stringextend2;
		g->Activation|=GACT_STRINGEXTEND;
		stGadgets[GADIX_SB_XE]=g;

		x2+=gw;
		ng.ng_LeftEdge		=x2;
		ng.ng_GadgetID		=GADID_SB_XL;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,NULL,GTST_MaxChars,(PARBUFLEN+1),TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=&stringextend2;
		g->Activation|=GACT_STRINGEXTEND;
		stGadgets[GADIX_SB_XL]=g;

		x2+=gw+5;
		ng.ng_LeftEdge		=x2;
		ng.ng_GadgetID		=GADID_SB_YS;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,NULL,GTST_MaxChars,(PARBUFLEN+1),TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=&stringextend2;
		g->Activation|=GACT_STRINGEXTEND;
		stGadgets[GADIX_SB_YS]=g;

		x2+=gw;
		ng.ng_LeftEdge		=x2;
		ng.ng_GadgetID		=GADID_SB_YE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,NULL,GTST_MaxChars,(PARBUFLEN+1),TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=&stringextend2;
		g->Activation|=GACT_STRINGEXTEND;
		stGadgets[GADIX_SB_YE]=g;

		x2+=gw;
		ng.ng_LeftEdge		=x2;
		ng.ng_GadgetID		=GADID_SB_YL;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,NULL,GTST_MaxChars,(PARBUFLEN+1),TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=&stringextend2;
		g->Activation|=GACT_STRINGEXTEND;
		stGadgets[GADIX_SB_YL]=g;

		x2+=gw+5;
		ng.ng_LeftEdge		=x2;
		ng.ng_GadgetID		=GADID_SB_TX;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,NULL,GTST_MaxChars,(PARBUFLEN+1),TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=&stringextend2;
		g->Activation|=GACT_STRINGEXTEND;
		stGadgets[GADIX_SB_TX]=g;

		x2+=gw;
		ng.ng_LeftEdge		=x2;
		ng.ng_GadgetID		=GADID_SB_TY;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,NULL,GTST_MaxChars,(PARBUFLEN+1),TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=&stringextend2;
		g->Activation|=GACT_STRINGEXTEND;
		stGadgets[GADIX_SB_TY]=g;

		x2+=gw;
		ng.ng_LeftEdge		=x2;
		ng.ng_GadgetID		=GADID_SB_TSV;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,NULL,GTST_MaxChars,(PARBUFLEN+1),TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=&stringextend2;
		g->Activation|=GACT_STRINGEXTEND;
		stGadgets[GADIX_SB_TSV]=g;

		if(g) {
			StatusBar_Wintags_Top			=RunTime.clientrect.BottomEdge-(sbh-1);
			StatusBar_Wintags_Width			=RunTime.clientrect.Width;
			StatusBar_Wintags_Height		=sbh;
			StatusBar_Wintags_Gadgets		=(ULONG)stGList;
			StatusBar_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(RunTime.winStatus=OpenMWindow(StatusBar_Wintags,gwin_up,&RunTime.gwin_upct,gwin_idcmps)) {
				GT_RefreshWindow(RunTime.winStatus,NULL);
				DrawGadWB(RunTime.winStatus->RPort,0,0,RunTime.clientrect.Width-1,sbh-1);
				RunTime.clientrect.BottomEdge	-=sbh;
				RunTime.clientrect.Height		-=sbh;
				SetMenuStrip(RunTime.winStatus,RunTime.Menu);
				Refr_StatusRange();

				rp=RunTime.winStatus->RPort;
				SetFont(rp,RunTime.statfont);
				SetABPenDrMd(rp,1,sfxprefs_gui.Pens[PEN_GUI_BACK],JAM1);
				y=2+RunTime.statfont->tf_Baseline;
				x2=3+6+2+sw;
				Move(rp,x2,y);Text(rp,"x start",7);
				x2+=gw;
				Move(rp,x2,y);Text(rp,"x end",5);
				x2+=gw;
				Move(rp,x2,y);Text(rp,"x length",8);

				x2+=gw+5;
				Move(rp,x2,y);Text(rp,"y start",7);
				x2+=gw;
				Move(rp,x2,y);Text(rp,"y end",5);
				x2+=gw;
				Move(rp,x2,y);Text(rp,"y length",8);

				x2+=gw+5;
				Move(rp,x2,y);Text(rp,"mouse x",7);
				x2+=gw;
				Move(rp,x2,y);Text(rp,"mouse y",7);
				x2+=gw;
				Move(rp,x2,y);Text(rp,"m. level",8);

				SetFont(rp,RunTime.scrfont);
				ret=FALSE;
			}
		}
	}
	return(ret);
}

void Test_StatusWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr) {
	ULONG tpos;

	switch(iclass) {
		case IDCMP_GADGETUP:
			switch((int)(((struct Gadget *)iadr)->GadgetID)) {
				case GADID_SB_XS:
					tpos=FromXUnit(RunTime.aktbuf->unitx,((struct StringInfo *)(stGadgets[GADIX_SB_XS]->SpecialInfo))->Buffer,RunTime.aktbuf->srat);
					switch(RunTime.rngmode)	{
						case RNGMD_LOOP:
							DrawLoop(RunTime.aktbuf);
							NewRangeStart(tpos,RunTime.aktbuf->slen,&RunTime.aktbuf->loops,&RunTime.aktbuf->loopl);
							if(!RunTime.aktbuf->loop) RunTime.aktbuf->loop=TRUE;
							DrawLoop(RunTime.aktbuf);RecalcLoopPlayData();
							break;
						case RNGMD_MARK:
							DrawMark(RunTime.aktbuf);
							NewRangeStart(tpos,RunTime.aktbuf->slen,&RunTime.aktbuf->markxs,&RunTime.aktbuf->markxl);
							if(!RunTime.aktbuf->mark) RunTime.aktbuf->mark=TRUE;
							DrawMark(RunTime.aktbuf);
							break;
						case RNGMD_ZOOM:
							NewRangeStart(tpos,RunTime.aktbuf->slen,&RunTime.aktbuf->zoomxs,&RunTime.aktbuf->zoomxl);
							DrawSample(RunTime.aktbuf,0);SetSlider(RunTime.aktbuf,1);
							break;
					}
					Refr_StatusRange();
					break;
				case GADID_SB_XE:
					tpos=FromXUnit(RunTime.aktbuf->unitx,((struct StringInfo *)(stGadgets[GADIX_SB_XE]->SpecialInfo))->Buffer,RunTime.aktbuf->srat);
					switch(RunTime.rngmode)	{
						case RNGMD_LOOP:
							DrawLoop(RunTime.aktbuf);
							NewRangeEnd(tpos,RunTime.aktbuf->slen,&RunTime.aktbuf->loops,&RunTime.aktbuf->loopl);
							if(!RunTime.aktbuf->loop) RunTime.aktbuf->loop=TRUE;
							DrawLoop(RunTime.aktbuf);RecalcLoopPlayData();
							break;
						case RNGMD_MARK:
							DrawMark(RunTime.aktbuf);
							NewRangeEnd(tpos,RunTime.aktbuf->slen,&RunTime.aktbuf->markxs,&RunTime.aktbuf->markxl);
							if(!RunTime.aktbuf->mark) RunTime.aktbuf->mark=TRUE;
							DrawMark(RunTime.aktbuf);
							break;
						case RNGMD_ZOOM:
							NewRangeEnd(tpos,RunTime.aktbuf->slen,&RunTime.aktbuf->zoomxs,&RunTime.aktbuf->zoomxl);
							DrawSample(RunTime.aktbuf,0);SetSlider(RunTime.aktbuf,1);
							break;
					}
					Refr_StatusRange();
					break;
				case GADID_SB_XL:
					tpos=FromXUnit(RunTime.aktbuf->unitx,((struct StringInfo *)(stGadgets[GADIX_SB_XL]->SpecialInfo))->Buffer,RunTime.aktbuf->srat);
					switch(RunTime.rngmode)	{
						case RNGMD_LOOP:
							DrawLoop(RunTime.aktbuf);
							NewRangeLength(tpos,RunTime.aktbuf->slen,&RunTime.aktbuf->loops,&RunTime.aktbuf->loopl);
							if(!RunTime.aktbuf->loop) RunTime.aktbuf->loop=TRUE;
							DrawLoop(RunTime.aktbuf);RecalcLoopPlayData();
							break;
						case RNGMD_MARK:
							DrawMark(RunTime.aktbuf);
							NewRangeLength(tpos,RunTime.aktbuf->slen,&RunTime.aktbuf->markxs,&RunTime.aktbuf->markxl);
							if(!RunTime.aktbuf->mark) RunTime.aktbuf->mark=TRUE;
							DrawMark(RunTime.aktbuf);
							break;
						case RNGMD_ZOOM:
							NewRangeLength(tpos,RunTime.aktbuf->slen,&RunTime.aktbuf->zoomxs,&RunTime.aktbuf->zoomxl);
							DrawSample(RunTime.aktbuf,0);SetSlider(RunTime.aktbuf,1);
							break;
					}
					Refr_StatusRange();
					break;
				case GADID_SB_YS:
					tpos=FromYUnit(RunTime.aktbuf->unitx,((struct StringInfo *)(stGadgets[GADIX_SB_YS]->SpecialInfo))->Buffer);
					switch(RunTime.rngmode)	{
						case RNGMD_MARK:
							DrawMark(RunTime.aktbuf);
							NewRangeStart(tpos,SMP_MAX,&RunTime.aktbuf->markys,&RunTime.aktbuf->markyl);
							if(!RunTime.aktbuf->mark) RunTime.aktbuf->mark=TRUE;
							DrawMark(RunTime.aktbuf);
						case RNGMD_ZOOM:
							NewRangeStart(tpos,SMP_MAX,&RunTime.aktbuf->zoomys,&RunTime.aktbuf->zoomyl);
							DrawSample(RunTime.aktbuf,0);SetSlider(RunTime.aktbuf,2);
							break;
					}				
					Refr_StatusRange();
					break;
				case GADID_SB_YE:
					tpos=FromYUnit(RunTime.aktbuf->unity,((struct StringInfo *)(stGadgets[GADIX_SB_YE]->SpecialInfo))->Buffer);
					switch(RunTime.rngmode)	{
						case RNGMD_MARK:
							DrawMark(RunTime.aktbuf);
							NewRangeEnd(tpos,SMP_MAX,&RunTime.aktbuf->markys,&RunTime.aktbuf->markyl);
							if(!RunTime.aktbuf->mark) RunTime.aktbuf->mark=TRUE;
							DrawMark(RunTime.aktbuf);
							break;
						case RNGMD_ZOOM:
							NewRangeEnd(tpos,SMP_MAX,&RunTime.aktbuf->zoomys,&RunTime.aktbuf->zoomyl);
							DrawSample(RunTime.aktbuf,0);SetSlider(RunTime.aktbuf,2);
							break;
					}
					Refr_StatusRange();
					break;
				case GADID_SB_YL:
					tpos=FromYUnit(RunTime.aktbuf->unity,((struct StringInfo *)(stGadgets[GADIX_SB_YL]->SpecialInfo))->Buffer);
					switch(RunTime.rngmode)	{
						case RNGMD_MARK:
							DrawMark(RunTime.aktbuf);
							NewRangeLength(tpos,SMP_MAX,&RunTime.aktbuf->markys,&RunTime.aktbuf->markyl);
							if(!RunTime.aktbuf->mark) RunTime.aktbuf->mark=TRUE;
							DrawMark(RunTime.aktbuf);
							break;
						case RNGMD_ZOOM:
							NewRangeLength(tpos,SMP_MAX,&RunTime.aktbuf->zoomys,&RunTime.aktbuf->zoomyl);
							DrawSample(RunTime.aktbuf,0);SetSlider(RunTime.aktbuf,2);
							break;
					}				
					Refr_StatusRange();
					break;
//				case GADID_SB_TSV:
//					tpos=FromYUnit(RunTime.aktbuf->unity,((struct StringInfo *)(stGadgets[GADIX_SB_TSV]->SpecialInfo))->Buffer);
//					RunTime.aktbuf->buf[RunTime.aktbuf->chnr[ch]][RunTime.mx1]=lpos;
//					DrawSample(RunTime.aktbuf,0);
//					break;
			}
			if(RunTime.aktbuf && SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) ActivateWindow(RunTime.aktbuf->win);
			break;
		case IDCMP_RAWKEY:
			switch(icode) {
				case RKEY_HELP:	ShowHelp("html/nodes/node01.04.__.html");break;
				default:		Test_RKeys(icode,iqual,RunTime.aktbuf);	break;
			}
			break;
		case IDCMP_VANILLAKEY:	Test_VKeys(icode,RunTime.aktbuf);break;
		case IDCMP_MENUPICK:	Test_Menu(icode,iqual);break;
	}
}

void Refr_StatusRange(void) {
	register UBYTE i;

//	INTRO;
	
	if(RunTime.aktsamples && RunTime.aktbuf) {
		GT_SetGadgetAttrs(stGadgets[GADIX_SB_TX ],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.mx1,RunTime.aktbuf->srat),TAG_DONE);
		GT_SetGadgetAttrs(stGadgets[GADIX_SB_TY ],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToYUnit(RunTime.aktbuf->unity,RunTime.my1),TAG_DONE);
		GT_SetGadgetAttrs(stGadgets[GADIX_SB_TSV],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToYUnit(RunTime.aktbuf->unity,RunTime.my2),TAG_DONE);
		switch(RunTime.rngmode) {
			case RNGMD_LOOP:
				if(RunTime.aktbuf->loop) {
					GT_SetGadgetAttrs(stGadgets[GADIX_SB_XS],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.aktbuf->loops,RunTime.aktbuf->srat),TAG_DONE);
					GT_SetGadgetAttrs(stGadgets[GADIX_SB_XE],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.aktbuf->loops+RunTime.aktbuf->loopl,RunTime.aktbuf->srat),TAG_DONE);
					GT_SetGadgetAttrs(stGadgets[GADIX_SB_XL],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.aktbuf->loopl,RunTime.aktbuf->srat),TAG_DONE);
					for(i=GADIX_SB_YS;i<=GADIX_SB_YL;i++) GT_SetGadgetAttrs(stGadgets[i],RunTime.winStatus,NULL,GA_Disabled,TRUE,GTST_String,0,TAG_DONE);
				}
				else {
					for(i=GADIX_SB_XS;i<=GADIX_SB_YL;i++) GT_SetGadgetAttrs(stGadgets[i],RunTime.winStatus,NULL,GA_Disabled,TRUE,GTST_String,0,TAG_DONE);
				}
				break;
			case RNGMD_MARK:
				if(RunTime.aktbuf->mark) {
					GT_SetGadgetAttrs(stGadgets[GADIX_SB_XS],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.aktbuf->markxs,RunTime.aktbuf->srat),TAG_DONE);
					GT_SetGadgetAttrs(stGadgets[GADIX_SB_XE],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.aktbuf->markxs+RunTime.aktbuf->markxl,RunTime.aktbuf->srat),TAG_DONE);
					GT_SetGadgetAttrs(stGadgets[GADIX_SB_XL],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.aktbuf->markxl,RunTime.aktbuf->srat),TAG_DONE);
					GT_SetGadgetAttrs(stGadgets[GADIX_SB_YS],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToYUnit(RunTime.aktbuf->unity,RunTime.aktbuf->markys+SMP_MAXN),TAG_DONE);
					GT_SetGadgetAttrs(stGadgets[GADIX_SB_YE],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToYUnit(RunTime.aktbuf->unity,(RunTime.aktbuf->markys+SMP_MAXN)+RunTime.aktbuf->markyl),TAG_DONE);
					GT_SetGadgetAttrs(stGadgets[GADIX_SB_YL],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToYUnit(RunTime.aktbuf->unity,RunTime.aktbuf->markyl),TAG_DONE);
				}
				else {
					for(i=GADIX_SB_XS;i<=GADIX_SB_YL;i++) GT_SetGadgetAttrs(stGadgets[i],RunTime.winStatus,NULL,GA_Disabled,TRUE,GTST_String,0,TAG_DONE);
				}
				break;
			case RNGMD_ZOOM:
				GT_SetGadgetAttrs(stGadgets[GADIX_SB_XS],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.aktbuf->zoomxs,RunTime.aktbuf->srat),TAG_DONE);
				GT_SetGadgetAttrs(stGadgets[GADIX_SB_XE],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.aktbuf->zoomxs+RunTime.aktbuf->zoomxl,RunTime.aktbuf->srat),TAG_DONE);
				GT_SetGadgetAttrs(stGadgets[GADIX_SB_XL],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToXUnit(RunTime.aktbuf->unitx,RunTime.aktbuf->zoomxl,RunTime.aktbuf->srat),TAG_DONE);
				GT_SetGadgetAttrs(stGadgets[GADIX_SB_YS],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToYUnit(RunTime.aktbuf->unity,RunTime.aktbuf->zoomys+SMP_MAXN),TAG_DONE);
				GT_SetGadgetAttrs(stGadgets[GADIX_SB_YE],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToYUnit(RunTime.aktbuf->unity,(RunTime.aktbuf->zoomys+SMP_MAXN)+RunTime.aktbuf->zoomyl),TAG_DONE);
				GT_SetGadgetAttrs(stGadgets[GADIX_SB_YL],RunTime.winStatus,NULL,GA_Disabled,FALSE,GTST_String,ToYUnit(RunTime.aktbuf->unity,RunTime.aktbuf->zoomyl),TAG_DONE);
				break;
		}
	}
	else {
		for(i=GADIX_SB_XS;i<=GADIX_SB_TSV;i++) GT_SetGadgetAttrs(stGadgets[i],RunTime.winStatus,NULL,GA_Disabled,TRUE,GTST_String,0,TAG_DONE);
	}
//	OUTRO;
}

//-- eof ----------------------------------------------------------------------
