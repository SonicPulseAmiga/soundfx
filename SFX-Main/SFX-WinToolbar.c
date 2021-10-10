/****h* SoundFX/SFX-WinToolbar.c [4.3] *
*
*  NAME
*    SFX-WinToolbar.c
*  COPYRIGHT
*    $VER: SFX-WinToolbar.c 4.3 (17.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Toolbarleiste for SFX Pubscreen - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    26.Jan.2000
*  MODIFICATION HISTORY
*    17.Apr.2004	V 4.3	most recent version
*    19.Feb.2003	V 4.2	most recent version
*    12.Nov.2000    V 4.1   most recent version
*                           C: now hide/show leaves buffer-name untouched
*    27.Jun.2000    V 4.0   most recent version
*    05.Feb.2000    V 3.82  most recent version
*    26.Jan.2000    V 3.82  initial version
*  NOTES
*
*******
*/

#define SFX_WinToolbar_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

extern struct Image imgZoomX0   ,imgZoomX1   ,imgZoomX2;
extern struct Image imgZoomY0   ,imgZoomY1   ,imgZoomY2;
extern struct Image imgZoomBoth0,imgZoomBoth1,imgZoomBoth2;
extern struct Image imgLoop0    ,imgLoop1    ,imgLoop2;
extern struct Image imgMark0    ,imgMark1    ,imgMark2;
extern struct Image imgZoom0    ,imgZoom1    ,imgZoom2;
extern struct Image imgTrace0   ,imgTrace1   ,imgTrace2;
extern struct Image imgLockCl0  ,imgLockCl1  ,imgLockCl2;
extern struct Image imgLockOp0  ,imgLockOp1  ,imgLockOp2;
extern struct Image imgVisible0 ,imgVisible1 ,imgVisible2;
extern struct Image imgHidden0  ,imgHidden1  ,imgHidden2;
extern struct Image imgChgBeg0  ,imgChgBeg1  ,imgChgBeg2;
extern struct Image imgChgEnd0  ,imgChgEnd1  ,imgChgEnd2;
extern struct Image imgWinM0    ,imgWinM1    ,imgWinM2;
extern struct Image imgWinO0    ,imgWinO1    ,imgWinO2;


#define DD_HEIGHT   200

//-- prototypes ---------------------------------------------------------------

UBYTE Open_ToolbarWin(void);
void Test_ToolbarWin (ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr);
void Test_DropDownWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr,BOOL *closedd);

void Refr_ToolbarLoader(void);
void Refr_ToolbarSaver(void);
void Refr_ToolbarOperator(void);
void Refr_ToolbarRexx(void);
void Refr_ToolbarPlayer(void);
void Refr_ToolbarBuffer(void);
void Refr_ToolbarEdit(void);
void Refr_ToolbarZoom(void);
void Refr_ToolbarRange(void);
void Refr_ToolbarWindows(void);

//-- Toolbar stuff

struct Gadget *CreateToolbars(struct NewToolbar *ntb,ULONG maxwidth,ULONG maxheight);
void DrawToolbars(struct NewToolbar *ntb,struct RastPort *rp);
void FreeToolbars(struct NewToolbar *ntb);

//-- Dropdown stuff

BOOL ShowDropDownWin_TextList(struct NewToolbar *ntb,struct List *choices,ULONG akt);
BOOL ShowDropDownWin_Images  (struct Gadget *gad,struct Gadget *gadgets);

//-- Enable/Disable Toolbar gadgets

void GadgetEnable(struct Gadget *gad);
void GadgetDisable(struct Gadget *gad);

//-- Labels for Toolbar DropDowns

void GadgetLabel(struct NewToolbar *ntb,STRPTR label);

//-- globals ------------------------------------------------------------------

struct Gadget *gadToolbar=NULL,*gadDropDown=NULL;
ULONG typDropDown;

//-- definitions --------------------------------------------------------------

UBYTE Open_ToolbarWin(void) {
	UBYTE ret=TRUE;

	INTRO;

	ToolBar_Wintags_Top			=RunTime.clientrect.TopEdge;
	ToolBar_Wintags_Width		=RunTime.clientrect.Width;
	ToolBar_Wintags_Height		=SFXToolbar[SFXTB_END].nt_Height;
	ToolBar_Wintags_Gadgets		=(ULONG)gadToolbar;
	ToolBar_Wintags_ScreenTitle	=(ULONG)sfxtitle;
	if(RunTime.winToolbar=OpenMWindow(ToolBar_Wintags,gwin_up,&RunTime.gwin_upct,gwin_idcmps)) {
		DrawToolbars(SFXToolbar,RunTime.winToolbar->RPort);
		RunTime.clientrect.TopEdge  +=SFXToolbar[SFXTB_END].nt_Height;
		RunTime.clientrect.Height   -=SFXToolbar[SFXTB_END].nt_Height;
		SetMenuStrip(RunTime.winToolbar,RunTime.Menu);
		Refr_ToolbarLoader();
		Refr_ToolbarSaver();
		Refr_ToolbarOperator();
		Refr_ToolbarRexx();
		Refr_ToolbarPlayer();
		Refr_ToolbarBuffer();
		Refr_ToolbarEdit();
		Refr_ToolbarZoom();
		Refr_ToolbarRange();
		Refr_ToolbarWindows();
		MSG("after Refr_*()");
		if(!RunTime.boolAHIOkay) GadgetDisable(SFXToolbar[GADIX_TB_RECORD].nt_Gadget);
		ret=FALSE;
	}
	OUTRO;
	return(ret);
}

void Test_ToolbarWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr) {
	struct NewToolbar       *ntb;
	static BOOL             swallowdd=FALSE;
	static struct Gadget    *ddgadget=NULL;

	switch(iclass) {
		case IDCMP_ACTIVEWINDOW:
			if((RunTime.winDropDown) && (ddgadget) && (imx>ddgadget->LeftEdge) && (imy>ddgadget->TopEdge) && (imx<ddgadget->LeftEdge+ddgadget->Width) && (imy<ddgadget->TopEdge+ddgadget->Height)) swallowdd=TRUE;
			break;
		case IDCMP_GADGETUP:
			switch(((struct Gadget *)iadr)->GadgetID) {
				case GADID_TB_LOAD:     SwitchIDCMPOFF();LoadSample();         SwitchIDCMPON();break;
				case GADID_TB_PREFLO:   SwitchIDCMPOFF();LoadSampleInterface();SwitchIDCMPON();break;
				case GADID_TB_CHLO:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_TextList(&SFXToolbar[GADIX_TB_LOADER],&LoaderList,RunTime.aloader)) { typDropDown=GADIX_TB_LOADER; }
					}
					else swallowdd=FALSE;
					break;

				case GADID_TB_SAVE:     SwitchIDCMPOFF();SaveSample();SwitchIDCMPON();break;
				case GADID_TB_PREFSA:   SwitchIDCMPOFF();SaveSampleInterface();SwitchIDCMPON();break;
				case GADID_TB_CHSA:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_TextList(&SFXToolbar[GADIX_TB_SAVER],&SaverList,RunTime.asaver)) { typDropDown=GADIX_TB_SAVER; }
					}
					else swallowdd=FALSE;
					break;

				case GADID_TB_RUNFX:    SwitchIDCMPOFF();ProcessSample(); SwitchIDCMPON();break;
				case GADID_TB_CHFX:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_TextList(&SFXToolbar[GADIX_TB_OPERATOR],&EffektList,RunTime.aktfx)) { typDropDown=GADIX_TB_OPERATOR; }
					}
					else swallowdd=FALSE;
					break;

				case GADID_TB_RUNREXX:  SwitchIDCMPOFF();StartRexx();SwitchIDCMPON();break;
				case GADID_TB_CHREXX:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_TextList(&SFXToolbar[GADIX_TB_REXX],&RexxList,RunTime.arxop)) { typDropDown=GADIX_TB_REXX; }
					}
					else swallowdd=FALSE;
					break;

				case GADID_TB_PLAYALL:  PlaySample_All();break;
				case GADID_TB_PLAYRNG:  PlaySample_Rng();break;
				case GADID_TB_STOP:     if(RunTime.play) StopSample();break;
				case GADID_TB_RECORD:   SwitchIDCMPOFF();RecordWin();SwitchIDCMPON();break;
				case GADID_TB_PREFPL:   SwitchIDCMPOFF();OPT_Player();SwitchIDCMPON();break;
				case GADID_TB_CHPL:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_TextList(&SFXToolbar[GADIX_TB_PLAYER],&PlayerList,RunTime.aplayer)) { typDropDown=GADIX_TB_PLAYER; }
					}
					else swallowdd=FALSE;
					break;

				case GADID_TB_HIDESHOW:
					ntb=((struct NewToolbar *)((struct Gadget *)iadr)->UserData);
					switch(RunTime.aktbuf->visibillity) {
						case SFX_SOUND_VISIBILLITY_HIDDEN:
							ShowSample();
							ntb->nt_Image=&imgVisible0;ntb->nt_HighImage=&imgVisible1;ntb->nt_ActiveImage=&imgVisible2;
							ntb->nt_Label=GetString(tbbHideShow0);
							break;
						case SFX_SOUND_VISIBILLITY_SHOWN:
							HideSample();
							ntb->nt_Image=&imgHidden0;ntb->nt_HighImage=&imgHidden1;   ntb->nt_ActiveImage=&imgHidden2;
							ntb->nt_Label=GetString(tbbHideShow1);
							break;
					}
					RemoveGadget(RunTime.winToolbar,(struct Gadget *)iadr);
					((struct Gadget *)iadr)->GadgetRender=ntb->nt_HighImage;
					((struct Gadget *)iadr)->SelectRender=ntb->nt_ActiveImage;
					AddGadget(RunTime.winToolbar,(struct Gadget *)iadr,0);
					RefreshGList((struct Gadget *)iadr,RunTime.winToolbar,NULL,1);
					SetStatus(ntb->nt_Label);
					break;
				case GADID_TB_PREFBUF:  SwitchIDCMPOFF();SampleOptions();SwitchIDCMPON();break;
				case GADID_TB_CHBUF:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_TextList(&SFXToolbar[GADIX_TB_BUFFER],&BufferList,GetBufferNr(RunTime.aktbuf->id))) { typDropDown=GADIX_TB_BUFFER; }
					}
					else swallowdd=FALSE;
					break;

				case GADID_TB_CUTMEN:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_Images(ddgadget,&tbCut)) { typDropDown=GADIX_TB_CUTMEN; }
					}
					else swallowdd=FALSE;
					break;
				case GADID_TB_COPYMEN:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_Images(ddgadget,&tbCopy)) { typDropDown=GADIX_TB_COPYMEN; }
					}
					else swallowdd=FALSE;
					break;
				case GADID_TB_PASTEMEN:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_Images(ddgadget,&tbPaste)) { typDropDown=GADIX_TB_PASTEMEN; }
					}
					else swallowdd=FALSE;
					break;
				case GADID_TB_ERASEMEN:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_Images(ddgadget,&tbErase)) { typDropDown=GADIX_TB_ERASEMEN; }
					}
					else swallowdd=FALSE;
					break;
				case GADID_TB_GRABMEN:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_Images(ddgadget,&tbGrab)) { typDropDown=GADIX_TB_GRABMEN; }
					}
					else swallowdd=FALSE;
					break;
				case GADID_TB_ZEROMEN:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_Images(ddgadget,&tbZero)) { typDropDown=GADIX_TB_ZEROMEN; }
					}
					else swallowdd=FALSE;
					break;
				case GADID_TB_OVERWRMEN:
					if(!swallowdd) {
						ddgadget=(struct Gadget *)iadr;
						if(ShowDropDownWin_Images(ddgadget,&tbOverwr)) { typDropDown=GADIX_TB_OVERWRMEN; }
					}
					else swallowdd=FALSE;
					break;

				case GADID_TB_ZOOMMODE:
					ntb=((struct NewToolbar *)((struct Gadget *)iadr)->UserData);
					switch(RunTime.zoommode) {
						case 0:
							ntb->nt_Image=&imgZoomY0;   ntb->nt_HighImage=&imgZoomY1;   ntb->nt_ActiveImage=&imgZoomY2;
							ntb->nt_Label=GetString(tbbZoomMode1);
							RunTime.zoommode=1;
							break;
						case 1:
							ntb->nt_Image=&imgZoomBoth0;ntb->nt_HighImage=&imgZoomBoth1;ntb->nt_ActiveImage=&imgZoomBoth2;
							ntb->nt_Label=GetString(tbbZoomMode2);
							RunTime.zoommode=2;
							break;
						case 2:
							ntb->nt_Image=&imgZoomX0;   ntb->nt_HighImage=&imgZoomX1;   ntb->nt_ActiveImage=&imgZoomY2;
							ntb->nt_Label=GetString(tbbZoomMode0);
							RunTime.zoommode=0;
							break;
					}
					RemoveGadget(RunTime.winToolbar,(struct Gadget *)iadr);
					((struct Gadget *)iadr)->GadgetRender=ntb->nt_HighImage;
					((struct Gadget *)iadr)->SelectRender=ntb->nt_ActiveImage;
					AddGadget(RunTime.winToolbar,(struct Gadget *)iadr,0);
					RefreshGList((struct Gadget *)iadr,RunTime.winToolbar,NULL,1);
					SetStatus(ntb->nt_Label);
					break;
				case GADID_TB_ZOOMIN:       ZoomIn   (RunTime.aktbuf,RunTime.zoommode+1);break;
				case GADID_TB_ZOOMOUT:      ZoomOut  (RunTime.aktbuf,RunTime.zoommode+1);break;
				case GADID_TB_ZOOMPIXEL:    ZoomPixel(RunTime.aktbuf,RunTime.zoommode+1);break;
				case GADID_TB_ZOOMFULL:     ZoomFull (RunTime.aktbuf,RunTime.zoommode+1);break;

				case GADID_TB_RNGMODE:
					RunTime.rngmode=(RunTime.rngmode+1)&3;
					Refr_ToolbarRange();
					break;
				case GADID_TB_LOCKLEN:
					ntb=((struct NewToolbar *)((struct Gadget *)iadr)->UserData);
					switch(RunTime.rnglock) {
						case 0: ntb->nt_Image=&imgLockCl0;ntb->nt_HighImage=&imgLockCl1;ntb->nt_ActiveImage=&imgLockCl2;RunTime.rnglock=TRUE;break;
						case 1: ntb->nt_Image=&imgLockOp0;ntb->nt_HighImage=&imgLockOp1;ntb->nt_ActiveImage=&imgLockOp2;RunTime.rnglock=FALSE;break;
					}
					if(RunTime.rnglock) GadgetEnable (SFXToolbar[GADIX_TB_MOVEMODE].nt_Gadget);
					else                GadgetDisable(SFXToolbar[GADIX_TB_MOVEMODE].nt_Gadget);
					RemoveGadget(RunTime.winToolbar,(struct Gadget *)iadr);
					((struct Gadget *)iadr)->GadgetRender=ntb->nt_HighImage;
					((struct Gadget *)iadr)->SelectRender=ntb->nt_ActiveImage;
					AddGadget(RunTime.winToolbar,(struct Gadget *)iadr,0);
					RefreshGList((struct Gadget *)iadr,RunTime.winToolbar,NULL,1);
					break;
				case GADID_TB_MOVEMODE:
					ntb=((struct NewToolbar *)((struct Gadget *)iadr)->UserData);
					switch(RunTime.movemode) {
						case 0: ntb->nt_Image=&imgChgEnd0;ntb->nt_HighImage=&imgChgEnd1;ntb->nt_ActiveImage=&imgChgEnd2;RunTime.movemode=TRUE;break;
						case 1: ntb->nt_Image=&imgChgBeg0;ntb->nt_HighImage=&imgChgBeg1;ntb->nt_ActiveImage=&imgChgBeg2;RunTime.movemode=FALSE;break;
					}
					RemoveGadget(RunTime.winToolbar,(struct Gadget *)iadr);
					((struct Gadget *)iadr)->GadgetRender=ntb->nt_HighImage;
					((struct Gadget *)iadr)->SelectRender=ntb->nt_ActiveImage;
					AddGadget(RunTime.winToolbar,(struct Gadget *)iadr,0);
					RefreshGList((struct Gadget *)iadr,RunTime.winToolbar,NULL,1);
					break;
				case GADID_TB_MOVEBEG:      RangeX1Beg(RunTime.aktbuf);break;
				case GADID_TB_MOVEEND:      RangeX2End(RunTime.aktbuf);break;
				case GADID_TB_MOVELEFTN:    RangeXZero(RunTime.aktbuf,-1);break;
				case GADID_TB_MOVERIGHTN:   RangeXZero(RunTime.aktbuf, 1);break;

				case GADID_TB_MOVELEFTF:
				case GADID_TB_MOVELEFTS:
				case GADID_TB_MOVERIGHTF:
				case GADID_TB_MOVERIGHTS:
					RunTime.rngmove=RNGMOVE_NONE;if(RunTime.rngmode==RNGMD_ZOOM) DrawSample(RunTime.aktbuf,0);
					break;

				case GADID_TB_MOVETOP:      RangeY2Top(RunTime.aktbuf);break;
				case GADID_TB_MOVEBOTTOM:   RangeY1Bot(RunTime.aktbuf);break;
				case GADID_TB_MOVEUPP:      RangeYPeak(RunTime.aktbuf, 1);break;
				case GADID_TB_MOVEDOWNP:    RangeYPeak(RunTime.aktbuf,-1);break;

				case GADID_TB_MOVEUPF:
				case GADID_TB_MOVEUPS:
				case GADID_TB_MOVEDOWNF:
				case GADID_TB_MOVEDOWNS:
					RunTime.rngmove=RNGMOVE_NONE;if(RunTime.rngmode==RNGMD_ZOOM) DrawSample(RunTime.aktbuf,0);
					break;

				case GADID_TB_WINMODE:
					ntb=((struct NewToolbar *)((struct Gadget *)iadr)->UserData);
					if(!RunTime.onewindow) {
						ntb->nt_Image=&imgWinO0;ntb->nt_HighImage=&imgWinO1;ntb->nt_ActiveImage=&imgWinO2;
						ntb->nt_Label=GetString(tbbWindowMode1);
						OneSampleWin();
					}
					else {
						ntb->nt_Image=&imgWinM0;ntb->nt_HighImage=&imgWinM1;ntb->nt_ActiveImage=&imgWinM2;
						ntb->nt_Label=GetString(tbbWindowMode0);
						MultiSampleWin();
					}
					RemoveGadget(RunTime.winToolbar,(struct Gadget *)iadr);
					((struct Gadget *)iadr)->GadgetRender=ntb->nt_HighImage;
					((struct Gadget *)iadr)->SelectRender=ntb->nt_ActiveImage;
					AddGadget(RunTime.winToolbar,(struct Gadget *)iadr,0);
					RefreshGList((struct Gadget *)iadr,RunTime.winToolbar,NULL,1);
					SetStatus(ntb->nt_Label);
					Refr_GlobalMenu();
					break;
			}
			if((!RunTime.winDropDown) && RunTime.aktbuf && (SFX_SOUND_IS_SHOWN(RunTime.aktbuf))) ActivateWindow(RunTime.aktbuf->win);
			break;
		case IDCMP_GADGETDOWN:
			switch(((struct Gadget *)iadr)->GadgetID) {
				case GADID_TB_MOVELEFTF:    RunTime.rngmove=RNGMOVE_X;RunTime.rngdiff=(RunTime.aktbuf->zoomxl>64) ? -(RunTime.aktbuf->zoomxl>>6) : -(RunTime.aktbuf->zoomxl>>1);break;
				case GADID_TB_MOVELEFTS:    RunTime.rngmove=RNGMOVE_X;RunTime.rngdiff=-1;break;
				case GADID_TB_MOVERIGHTS:   RunTime.rngmove=RNGMOVE_X;RunTime.rngdiff= 1;break;
				case GADID_TB_MOVERIGHTF:   RunTime.rngmove=RNGMOVE_X;RunTime.rngdiff=(RunTime.aktbuf->zoomxl>64) ?  (RunTime.aktbuf->zoomxl>>6) :  (RunTime.aktbuf->zoomxl>>1);break;

				case GADID_TB_MOVEUPF:      RunTime.rngmove=RNGMOVE_Y;RunTime.rngdiff= 256;break;
				case GADID_TB_MOVEUPS:      RunTime.rngmove=RNGMOVE_Y;RunTime.rngdiff= 1;break;
				case GADID_TB_MOVEDOWNS:    RunTime.rngmove=RNGMOVE_Y;RunTime.rngdiff=-1;break;
				case GADID_TB_MOVEDOWNF:    RunTime.rngmove=RNGMOVE_Y;RunTime.rngdiff=-256;break;
			}
			break;
		case IDCMP_RAWKEY:
			switch(icode) {
				case RKEY_HELP: ShowHelp("html/nodes/node01.03.__.html");break;
				default:        Test_RKeys(icode,iqual,RunTime.aktbuf); break;
			}
			break;
		case IDCMP_VANILLAKEY:
			Test_VKeys(icode,RunTime.aktbuf);
			break;
		case IDCMP_MENUPICK:
			Test_Menu(icode,iqual);
			break;
	}
}

void Test_DropDownWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr,BOOL *closedd) {
	switch(iclass) {
		case IDCMP_GADGETUP:
			switch(typDropDown) {                                       // after choosing a module only start fx immediately
				case GADIX_TB_LOADER:   RunTime.aloader=icode;Refr_ToolbarLoader();break;
				case GADIX_TB_SAVER:    RunTime.asaver =icode;Refr_ToolbarSaver();break;
				case GADIX_TB_OPERATOR: RunTime.aktfx  =icode;Refr_ToolbarOperator();SwitchIDCMPOFF();ProcessSample(); SwitchIDCMPON();break;
				case GADIX_TB_REXX:     RunTime.arxop  =icode;Refr_ToolbarRexx();break;
				case GADIX_TB_PLAYER:   RunTime.aplayer=icode;Refr_ToolbarPlayer();break;
				case GADIX_TB_BUFFER:
					RunTime.aktbuf =GetBufferPtr(0,icode);
					Refr_ToolbarBuffer();
					Refr_ToolbarRange();
					Refr_ContextMenu();
					if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) {
						ActivateWindow(RunTime.aktbuf->win);
					}
					break;

				case GADIX_TB_CUTMEN:
					switch(((struct Gadget *)iadr)->GadgetID) {
						case GADID_DD_CUT:      if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Cut(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
						case GADID_DD_CUTB:     if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Cut(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
						case GADID_DD_CUTE:     if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Cut(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
					}
					break;
				case GADIX_TB_COPYMEN:
					switch(((struct Gadget *)iadr)->GadgetID) {
						case GADID_DD_COPY:     if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Copy(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
						case GADID_DD_COPYB:    if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Copy(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
						case GADID_DD_COPYE:    if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Copy(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
					}
					break;
				case GADIX_TB_PASTEMEN:
					switch(((struct Gadget *)iadr)->GadgetID) {
						case GADID_DD_PASTE:        if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Paste(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
						case GADID_DD_PASTEB:       SwitchIDCMPOFF();Edit_Paste(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON();break;
						case GADID_DD_PASTEE:       SwitchIDCMPOFF();Edit_Paste(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON();break;
					}
					break;
				case GADIX_TB_ERASEMEN:
					switch(((struct Gadget *)iadr)->GadgetID) {
						case GADID_DD_ERASE:        if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Erase(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
						case GADID_DD_ERASEB:       if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Erase(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
						case GADID_DD_ERASEE:       if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Erase(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
					}
					break;
				case GADIX_TB_GRABMEN:
					switch(((struct Gadget *)iadr)->GadgetID) {
						case GADID_DD_GRAB:     if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Grab(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
						case GADID_DD_GRABB:    if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Grab(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
						case GADID_DD_GRABE:    if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Grab(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
					}
					break;
				case GADIX_TB_ZEROMEN:
					switch(((struct Gadget *)iadr)->GadgetID) {
						case GADID_DD_ZERO:     if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Zero(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
						case GADID_DD_ZEROB:    if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Zero(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
						case GADID_DD_ZEROE:    if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Zero(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
					}
					break;
				case GADIX_TB_OVERWRMEN:
					switch(((struct Gadget *)iadr)->GadgetID) {
						case GADID_DD_OVERWR:   if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Overwrite(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
						case GADID_DD_OVERWRB:  SwitchIDCMPOFF();Edit_Overwrite(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON();break;
					}
					break;
			}
			*closedd=TRUE;
			break;
		case IDCMP_INACTIVEWINDOW:
			*closedd=TRUE;
			break;
	}
}

void Refr_ToolbarLoader(void) {
	GadgetLabel(&SFXToolbar[GADIX_TB_LOADER],GetNameByIndex(&LoaderList,RunTime.aloader));
	if((((ModNode *)GetNodeByIndex(&LoaderList,RunTime.aloader))->info.gui)) GadgetEnable(SFXToolbar[GADIX_TB_PREFLO].nt_Gadget);
	else GadgetDisable(SFXToolbar[GADIX_TB_PREFLO].nt_Gadget);
}

void Refr_ToolbarSaver(void) {
	GadgetLabel(&SFXToolbar[GADIX_TB_SAVER],GetNameByIndex(&SaverList,RunTime.asaver));
	if((((ModNode *)GetNodeByIndex(&SaverList,RunTime.asaver))->info.gui)) GadgetEnable(SFXToolbar[GADIX_TB_PREFSA].nt_Gadget);
	else GadgetDisable(SFXToolbar[GADIX_TB_PREFSA].nt_Gadget);
	if(RunTime.aktsamples) GadgetEnable(SFXToolbar[GADIX_TB_SAVE].nt_Gadget);
	else GadgetDisable(SFXToolbar[GADIX_TB_SAVE].nt_Gadget);
}

void Refr_ToolbarOperator(void) {
	GadgetLabel(&SFXToolbar[GADIX_TB_OPERATOR],GetNameByIndex(&EffektList,RunTime.aktfx));
}

void Refr_ToolbarRexx(void) {
	GadgetLabel(&SFXToolbar[GADIX_TB_REXX],GetNameByIndex(&RexxList,RunTime.arxop));
}

void Refr_ToolbarPlayer(void) {
	GadgetLabel(&SFXToolbar[GADIX_TB_PLAYER],GetNameByIndex(&PlayerList,RunTime.aplayer));
	if((((ModNode *)GetNodeByIndex(&PlayerList,RunTime.aplayer))->info.gui)) GadgetEnable(SFXToolbar[GADIX_TB_PREFPL].nt_Gadget);
	else GadgetDisable(SFXToolbar[GADIX_TB_PREFPL].nt_Gadget);
	if(RunTime.aktsamples) {
		GadgetEnable(SFXToolbar[GADIX_TB_PLAYALL].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_PLAYRNG].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_STOP].nt_Gadget);
	}
	else {
		GadgetDisable(SFXToolbar[GADIX_TB_PLAYALL].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_PLAYRNG].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_STOP].nt_Gadget);
	}
}

void Refr_ToolbarBuffer(void) {
	struct NewToolbar *ntb=&SFXToolbar[GADIX_TB_HIDESHOW];
	struct Gadget *gad=SFXToolbar[GADIX_TB_HIDESHOW].nt_Gadget;

	INTRO;

	if(RunTime.aktbuf) {
		switch(RunTime.aktbuf->visibillity) {
			case SFX_SOUND_VISIBILLITY_HIDDEN:
				ntb->nt_Image=&imgHidden0;ntb->nt_HighImage=&imgHidden1;   ntb->nt_ActiveImage=&imgHidden2;
				ntb->nt_Label=GetString(tbbHideShow0);
				break;
			case SFX_SOUND_VISIBILLITY_SHOWN:
				ntb->nt_Image=&imgVisible0;ntb->nt_HighImage=&imgVisible1;ntb->nt_ActiveImage=&imgVisible2;
				ntb->nt_Label=GetString(tbbHideShow1);
				break;
		}
		RemoveGadget(RunTime.winToolbar,gad);
		gad->GadgetRender=ntb->nt_Image;
		gad->SelectRender=ntb->nt_ActiveImage;
		AddGadget(RunTime.winToolbar,gad,0);
		RefreshGList(gad,RunTime.winToolbar,NULL,1);
		// this happens when reopening the GUI
		if(RunTime.winStatus) SetStatus(ntb->nt_Label);

		GadgetLabel(&SFXToolbar[GADIX_TB_BUFFER],((struct Node *)RunTime.aktbuf)->ln_Name);
		GadgetEnable(SFXToolbar[GADIX_TB_CHBUF].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_PREFBUF].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_HIDESHOW].nt_Gadget);
	}
	else {
		GadgetLabel(&SFXToolbar[GADIX_TB_BUFFER],NULL);
		GadgetDisable(SFXToolbar[GADIX_TB_CHBUF].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_PREFBUF].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_HIDESHOW].nt_Gadget);
	}
	OUTRO;
}

void Refr_ToolbarEdit(void) {
	INTRO;
	if(RunTime.aktbuf) {
		GadgetEnable(SFXToolbar[GADIX_TB_CUTMEN   ].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_COPYMEN  ].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_PASTEMEN ].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_ERASEMEN ].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_GRABMEN  ].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_ZEROMEN  ].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_OVERWRMEN].nt_Gadget);
	}
	else {
		GadgetDisable(SFXToolbar[GADIX_TB_CUTMEN   ].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_COPYMEN  ].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_PASTEMEN ].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_ERASEMEN ].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_GRABMEN  ].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_ZEROMEN  ].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_OVERWRMEN].nt_Gadget);
	}
	OUTRO;
}

void Refr_ToolbarZoom(void) {
	INTRO;
	if(RunTime.aktbuf) {
		GadgetEnable(SFXToolbar[GADIX_TB_ZOOMMODE].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_ZOOMIN].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_ZOOMOUT].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_ZOOMPIXEL].nt_Gadget);
		GadgetEnable(SFXToolbar[GADIX_TB_ZOOMFULL].nt_Gadget);
	}
	else {
		GadgetDisable(SFXToolbar[GADIX_TB_ZOOMMODE].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_ZOOMIN].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_ZOOMOUT].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_ZOOMPIXEL].nt_Gadget);
		GadgetDisable(SFXToolbar[GADIX_TB_ZOOMFULL].nt_Gadget);
	}
	OUTRO;
}

void Refr_ToolbarRange(void) {
	register UBYTE i;
	struct NewToolbar *ntb=&SFXToolbar[GADIX_TB_RNGMODE];
	struct Gadget *gad=SFXToolbar[GADIX_TB_RNGMODE].nt_Gadget;
	BOOL act=TRUE;

	if((gad->GadgetRender==&imgLoop0) || (gad->GadgetRender==&imgMark0) || (gad->GadgetRender==&imgZoom0) || (gad->GadgetRender==&imgTrace0)) act=FALSE;
	switch(RunTime.rngmode) {
		case RNGMD_LOOP:
			ntb->nt_Image=&imgLoop0; ntb->nt_HighImage=&imgLoop1; ntb->nt_ActiveImage=&imgLoop2;
			ntb->nt_Label=GetString(tbbRangeMode0);
			break;
		case RNGMD_MARK:
			ntb->nt_Image=&imgMark0; ntb->nt_HighImage=&imgMark1; ntb->nt_ActiveImage=&imgMark2;
			ntb->nt_Label=GetString(tbbRangeMode1);
			break;
		case RNGMD_ZOOM:
			ntb->nt_Image=&imgZoom0; ntb->nt_HighImage=&imgZoom1; ntb->nt_ActiveImage=&imgZoom2;
			ntb->nt_Label=GetString(tbbRangeMode2);
			break;
		case RNGMD_TRACE:
			ntb->nt_Image=&imgTrace0;ntb->nt_HighImage=&imgTrace1;ntb->nt_ActiveImage=&imgTrace2;
			ntb->nt_Label=GetString(tbbRangeMode3);
			break;
		UNEXPECTED_DEFAULT
	}

	RemoveGadget(RunTime.winToolbar,gad);
	if(act) gad->GadgetRender=ntb->nt_HighImage;
	else    gad->GadgetRender=ntb->nt_Image;
	gad->SelectRender=ntb->nt_ActiveImage;
	AddGadget(RunTime.winToolbar,gad,0);
	RefreshGList(gad,RunTime.winToolbar,NULL,1);

	if(RunTime.winStatus) {
		SetStatus(ntb->nt_Label);
		Refr_StatusRange();
	}

	if(RunTime.rnglock) GadgetEnable (SFXToolbar[GADIX_TB_MOVEMODE].nt_Gadget);
	else                GadgetDisable(SFXToolbar[GADIX_TB_MOVEMODE].nt_Gadget);

	if(RunTime.aktsamples && RunTime.aktbuf) {
		switch(RunTime.rngmode) {
			case RNGMD_LOOP:
				for(i=GADIX_TB_MOVEBEG;i<=GADIX_TB_MOVEEND;i++)    GadgetEnable(SFXToolbar[i].nt_Gadget);
				for(i=GADIX_TB_MOVETOP;i<=GADIX_TB_MOVEBOTTOM;i++) GadgetDisable(SFXToolbar[i].nt_Gadget);
				break;
			case RNGMD_MARK:
			case RNGMD_ZOOM:
				for(i=GADIX_TB_MOVEBEG;i<=GADIX_TB_MOVEBOTTOM;i++) GadgetEnable(SFXToolbar[i].nt_Gadget);
				break;
			case RNGMD_TRACE:
				for(i=GADIX_TB_MOVEBEG;i<=GADIX_TB_MOVEBOTTOM;i++) GadgetDisable(SFXToolbar[i].nt_Gadget);
				break;
			UNEXPECTED_DEFAULT
		}
	}
	else {
		for(i=GADIX_TB_MOVEBEG;i<=GADIX_TB_MOVEBOTTOM;i++) GadgetDisable(SFXToolbar[i].nt_Gadget);
	}
}

void Refr_ToolbarWindows(void) {
	struct NewToolbar *ntb=&SFXToolbar[GADIX_TB_WINMODE];
	struct Gadget *gad=SFXToolbar[GADIX_TB_WINMODE].nt_Gadget;
	BOOL act=TRUE;

	if((gad->GadgetRender==&imgWinO0) || (gad->GadgetRender==&imgWinM0)) act=FALSE;
	if(RunTime.onewindow) {
		ntb->nt_Image=&imgWinO0;ntb->nt_HighImage=&imgWinO1;ntb->nt_ActiveImage=&imgWinO2;
		ntb->nt_Label=GetString(tbbWindowMode1);
	}
	else {
		ntb->nt_Image=&imgWinM0;ntb->nt_HighImage=&imgWinM1;ntb->nt_ActiveImage=&imgWinM2;
		ntb->nt_Label=GetString(tbbWindowMode0);
	}
	RemoveGadget(RunTime.winToolbar,gad);
	if(act) gad->GadgetRender=ntb->nt_HighImage;
	else    gad->GadgetRender=ntb->nt_Image;
	gad->SelectRender=ntb->nt_ActiveImage;
	AddGadget(RunTime.winToolbar,gad,0);
	RefreshGList(gad,RunTime.winToolbar,NULL,1);
}

//-- Toolbar stuff

struct Gadget *CreateToolbars(struct NewToolbar *ntb,ULONG maxwidth,ULONG maxheight) {
	struct NewToolbar *ntb_tbhead;
	UWORD x=0,y=0,w,h,lh=0;
	UWORD ix,iy,iw,ih;
	struct Gadget *glist=NULL,*ng,*og;

	og=CreateContext(&glist);

	while(ntb->nt_Type!=NT_END) {                           /* read all entries */
		if(ntb->nt_Type==NT_TOOLBAR) {                          /* a new toolbar starts */
			//MSG1("  %4d : New Toolbar ",x);
			ntb_tbhead=ntb++;                                   /* remember head element; we enter layoutinfo there */
			w=h=0;
			while(ntb->nt_Type==NT_ITEM) {                      /* read all toolbar elements and calculate toolbar size */
				if(ntb->nt_Image) { w+=(ntb->nt_Image->Width-2);if(ntb->nt_Image->Height>h) h=ntb->nt_Image->Height; }
				else              { w+=nt_UnpackX(ntb->nt_Size);if(ntb->nt_Height>h) h=nt_UnpackY(ntb->nt_Size); }
				ntb++;
			}
			w+=5;h+=3;                                          /* add some space */
			//MSG2("    w=%4d  h=%2d",w,h);
			if(w>maxwidth) w=maxwidth;                                              /* hope this never happens */
			if(x+w>maxwidth) { y+=(lh+1);lh=h;x=0;/*ntb_tbhead->nt_Image=(struct Image *)1;*/ } /* this toolbar won't fit into this line */
			if(h>lh) lh=h;                                      /* if this toolbar is higher than the ones before, change line-height */
			ntb=ntb_tbhead;
			ntb->nt_Pos =nt_Pack(x,y);
			ntb->nt_Size=nt_Pack(w,h);
			ntb++;
			ix=x+2;iy=y;x+=(w+1);
			//MSG2("    x=%4d  y=%2d",x,y);
			while(ntb->nt_Type==NT_ITEM) {                      /* read all toolbar elements again and create gadgets */
				if(ntb->nt_Image) {
					if(ng=AllocVec(sizeof(struct Gadget),MEMF_ANY)) {
						og->NextGadget  =ng;
						ng->NextGadget  =NULL;
						ng->LeftEdge    =ix;
						ng->TopEdge     =iy+(((h+1)-ntb->nt_Image->Height)>>1);
						ng->Width       =ntb->nt_Image->Width;
						ng->Height      =ntb->nt_Image->Height;
						ng->Flags       =GFLG_GADGIMAGE|GFLG_GADGHIMAGE;
						ng->Activation  =GACT_RELVERIFY|GACT_IMMEDIATE;
						ng->GadgetType  =GTYP_BOOLGADGET;
						ng->GadgetRender=ntb->nt_Image;
						ng->SelectRender=ntb->nt_ActiveImage;
						ng->GadgetText  =NULL;
						ng->MutualExclude=0L;
						ng->SpecialInfo =NULL;
						ng->GadgetID    =ntb->nt_GadgetID;
						ng->UserData    =ntb;
						ntb->nt_Gadget  =ng;
						og=ng;
						ix+=(ng->Width-2);
					}
				}
				else {
					/** @TODO: there is some gadget attaced (we need this for e.g. toolbar embeded TextEntry-Gadgets */
					//if(ntb->nt_Gadget) {
						//ng=ntb->nt_Gadget;
						//ng->NextGadget  =NULL;
						//og->NextGadget  =ng;
						//og=ng;
					//}
					iw=nt_UnpackX(ntb->nt_Size);ih=nt_UnpackY(ntb->nt_Size);
					//if(!ih) {                           // when closing and reopening, height must be set again
						switch(ntb->nt_Border) {        // set def height for one line of text
							case NTB_RAISED: 
							case NTB_SUNKEN: ih=RunTime.scrfont->tf_YSize+4;break;
							case NTB_RIDGE:  
							case NTB_GROOVE: ih=RunTime.scrfont->tf_YSize+6;break;
						}
						ntb->nt_Size=nt_Pack(iw,ih);
					//}
					ntb->nt_Pos =(ULONG)(ix<<16)|(iy+(((h+1)-ih)>>1));
					ix+=iw;
				}
				ntb++;
			}
		}
		else ntb++;
	}
	ntb->nt_Width=maxwidth;
	ntb->nt_Height=y+lh+1;
	return(glist);
}

void DrawToolbars(struct NewToolbar *ntb,struct RastPort *rp) {
	UWORD x,y,w,h;

	while(ntb->nt_Type!=NT_END) {                       /* read all entries */
		if(ntb->nt_Type==NT_TOOLBAR) {                          /* a toolbar */
			x=nt_UnpackX(ntb->nt_Pos );y=nt_UnpackY(ntb->nt_Pos );  /* get pos & size and draw it */
			w=nt_UnpackX(ntb->nt_Size);h=nt_UnpackY(ntb->nt_Size);
			DrawGadWB(rp,x,y,w,h);
			//if(ntb->nt_Image==(struct Image *)1) {          /* last toolbar in row */
				//x1+=x2+1;
				//DrawGadWB(rp,x1,y1,scr->Width-1,y2-1);
			//}
		}
		if(ntb->nt_Type==NT_ITEM) {                         /* a toolbar item */
			if(!ntb->nt_Image) {
				x=nt_UnpackX(ntb->nt_Pos );y=nt_UnpackY(ntb->nt_Pos );  /* get pos & size and draw it */
				w=nt_UnpackX(ntb->nt_Size);h=nt_UnpackY(ntb->nt_Size);
				switch(ntb->nt_Border) {
					case NTB_RAISED: DrawGadWB(rp,x+1,y,w-2,h);break;
					case NTB_SUNKEN: DrawGadBW(rp,x+1,y,w-2,h);break;
					case NTB_RIDGE:  DrawGadWB(rp,x+1,y,w-2,h);DrawGadBW(rp,x+2,y+1,w-4,h-2);break;
					case NTB_GROOVE: DrawGadBW(rp,x+1,y,w-2,h);DrawGadWB(rp,x+2,y+1,w-4,h-2);break;
				}
			}
		}
		ntb++;
	}
}

void FreeToolbars(struct NewToolbar *ntb) {
	while(ntb->nt_Type!=NT_END) {                       /* read all entries */
		if(ntb->nt_Type==NT_TOOLBAR) {                          /* a new toolbar starts */
			ntb++;
			while(ntb->nt_Type==NT_ITEM) {                      /* read all toolbar elements and free memory if neccesary */
				if(ntb->nt_Image && ntb->nt_Gadget) { FreeVec(ntb->nt_Gadget);ntb->nt_Gadget=NULL; }
				ntb++;
			}
		}
		else ntb++;
	}
}

//-- Dropdown stuff

BOOL ShowDropDownWin_TextList(struct NewToolbar *ntb,struct List *choices,ULONG akt) {
	struct NewGadget ng;
	struct Gadget *g;
	UWORD x,y,w,h;
	UWORD gh,gy;

	if(RunTime.winDropDown)         { CloseMWindow(RunTime.winDropDown,&RunTime.gwin_upct);MSG("oops"); }

	x=nt_UnpackX(ntb->nt_Pos );y=nt_UnpackY(ntb->nt_Pos );      /* get pos & size */                        
	w=nt_UnpackX(ntb->nt_Size);h=nt_UnpackY(ntb->nt_Size);

	gy=RunTime.winToolbar->TopEdge+y+h+1;

	if(g=CreateContext(&gadDropDown)) {
		ng.ng_LeftEdge      =0;
		ng.ng_TopEdge       =0;
		ng.ng_Width         =w;
		ng.ng_GadgetText    =NULL;  // is this allowed (seems to be okay)?
		ng.ng_TextAttr      =RunTime.ScrFont;
		ng.ng_GadgetID      =GADID_DD_LIST;
		ng.ng_Flags         =PLACETEXT_IN;
		ng.ng_VisualInfo    =RunTime.VisualInfo;

		gh=(GetNumberOfNodes(choices)*RunTime.scrfont->tf_YSize)+4;
		if(gy+gh>RunTime.clientrect.BottomEdge) {	// if its longer than the screen is heigh, trucate it and add scrollers
			gh=(((UWORD)(((RunTime.clientrect.BottomEdge-gy)-4)/RunTime.scrfont->tf_YSize))*RunTime.scrfont->tf_YSize)+4;
			ng.ng_Height        =gh;
			g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,choices,GTLV_Selected,akt,GTLV_Top,akt,GTLV_ShowSelected,0,TAG_DONE);
		}
		else {
			ng.ng_Height        =gh;
			//-- gadtools gnores GTLV_ScrollWidth=0
			// -> should be trated as Scrollers=No!
			// -> but crashes under AOS4
			g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,choices,GTLV_Selected,akt,GTLV_Top,akt,GTLV_ShowSelected,0,TAG_DONE);
		}

		if(g) {
			MemUsage(sfxtitle);
			DropDown_Wintags_Left		=x;
			DropDown_Wintags_Top		=gy;
			DropDown_Wintags_Width		=w;
			DropDown_Wintags_Height		=gh;
			DropDown_Wintags_Gadgets	=(ULONG)gadDropDown;
			DropDown_Wintags_ScreenTitle=(ULONG)sfxtitle;
			if(RunTime.winDropDown=OpenMWindow(DropDown_Wintags,gwin_up,&RunTime.gwin_upct,dwin_idcmps)) {
				GT_RefreshWindow(RunTime.winDropDown,NULL);
				return(TRUE);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	return(FALSE);
}

BOOL ShowDropDownWin_Images(struct Gadget *gad,struct Gadget *gadgets) {
	struct Gadget *t=gadgets;
	UWORD x,y,w,h=4;

	if(RunTime.winDropDown)         { CloseMWindow(RunTime.winDropDown,&RunTime.gwin_upct);MSG("oops"); }

	x=gad->LeftEdge-2;y=gad->TopEdge+gad->Height+1;
	w=gad->Width+4;
	while(t) { h+=t->Height;t=t->NextGadget; }

	MemUsage(sfxtitle);
	DropDown_Wintags_Left	=x;
	DropDown_Wintags_Top	=RunTime.winToolbar->TopEdge+y;
	DropDown_Wintags_Width	=w;
	DropDown_Wintags_Height	=h;
	DropDown_Wintags_Gadgets=(ULONG)gadgets;
	DropDown_Wintags_ScreenTitle=(ULONG)sfxtitle;
	if(RunTime.winDropDown=OpenMWindow(DropDown_Wintags,gwin_up,&RunTime.gwin_upct,dwin_idcmps)) {
		DrawGadWB(RunTime.winDropDown->RPort,0,0,w-1,h-1);
		return(TRUE);
	}
	else Message(errOpenWindow,NULL,__FILE__,__LINE__);
	return(FALSE);
}

//-- Enable/Disable Toolbar gadgets

void GadgetEnable(struct Gadget *gad) {
	RemoveGadget(RunTime.winToolbar,gad);
	gad->Flags&=~GFLG_DISABLED;
	AddGadget(RunTime.winToolbar,gad,0);
	RefreshGList(gad,RunTime.winToolbar,NULL,1);
}

void GadgetDisable(struct Gadget *gad) {
	RemoveGadget(RunTime.winToolbar,gad);
	gad->Flags|=GFLG_DISABLED;
	AddGadget(RunTime.winToolbar,gad,0);
	RefreshGList(gad,RunTime.winToolbar,NULL,1);
}

//-- Labels for Toolbar DropDowns

void GadgetLabel(struct NewToolbar *ntb,STRPTR label) {
	UWORD x,y,w,h;
	UWORD tl;

	x=nt_UnpackX(ntb->nt_Pos );y=nt_UnpackY(ntb->nt_Pos );      /* get pos & size */                        
	w=nt_UnpackX(ntb->nt_Size);h=nt_UnpackY(ntb->nt_Size);
	SetAPen(RunTime.winToolbar->RPort,0);RectFill(RunTime.winToolbar->RPort,x+3,y+2,x+(w-5),y+(h-3));
	if(label){
		tl=strlen(label);
		SetAPen(RunTime.winToolbar->RPort,1);SetBPen(RunTime.winToolbar->RPort,0);
		Move(RunTime.winToolbar->RPort,x+4,y+2+RunTime.scrfont->tf_Baseline);
		while((tl>0) && TextLength(RunTime.winToolbar->RPort,label,tl)>(w-6)) tl--;
		Text(RunTime.winToolbar->RPort,label,tl);
	}
}

//-- eof ----------------------------------------------------------------------
