/****h* Equalize-FFT/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (02.05.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Equalize-FFT Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    02.May.2003	V 4.2	most recent version
*    24.Jan.2001	V 4.1	most recent version
*    02.Aug.2000	V 1.25	most recent version
*    18.Jul.1998	V 1.25	initial version
*  NOTES
*
*  IDEAS
*    * Flip-Button für Level : level=((level==0)?max:(1.0/level))
*    * level-adjust inputgadget
*      Faktor eingeben und enter -> alle/akt. Wert wird mit Faktor multipliziert
*    * proper FFT -> ZeroPadding
*      m  : mb : the number of bands
*      mm : mw : size of buffer
*      m2 : ms : step-width
*      -- : mv : number of values to put into buffer and to retrieve (>=ms)
*
*******
*/

#define EQUALIZE_FFT_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Equalize-FFT.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

void DrawEqfs(struct Instance *instance,struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye);
void DrawEqf(struct Instance *instance,struct RastPort *rp,UBYTE aktband,UWORD xs,UWORD ys,UWORD xe,UWORD ye);
void AdaptEqfs(struct Instance *instance,UBYTE amode);

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_CFGSEL	(GADID_BASE+1)
#define GADIX_CFGSEL	(GADIX_BASE+1)
#define GADID_CFGLIST	(GADID_CFGSEL)
#define GADIX_CFGLIST	(GADIX_CFGSEL)
#define GADID_CFGNAME	(GADID_CFGLIST+1)
#define GADIX_CFGNAME	(GADIX_CFGLIST+1)
#define GADID_CFGADD	(GADID_CFGNAME+1)
#define GADIX_CFGADD	(GADIX_CFGNAME+1)
#define GADID_CFGDEL	(GADID_CFGADD+1)
#define GADIX_CFGDEL	(GADIX_CFGADD+1)

#define GADID_SRC		(GADID_CFGSEL+CFGSEL_IDCT)
#define GADIX_SRC		(GADIX_CFGSEL+CFGSEL_IXCT)

#define GADID_BAND		(GADID_SRC+SRC_IDCT)
#define GADIX_BAND		(GADIX_SRC+SRC_IXCT)

#define GADID_VAL		(GADID_BAND+1)
#define GADIX_VAL		(GADIX_BAND+1)

#define GADID_RANGE		(GADID_VAL+1)

#define GADID_MODE		(GADID_RANGE+1)
#define GADIX_MODE		(GADIX_VAL+1)

#define GADID_WFKT		(GADID_MODE+1)
#define GADIX_WFKT		(GADIX_MODE+1)

#define GADID_BANDS		(GADID_WFKT+WFKT_IDCT)
#define GADIX_BANDS		(GADIX_WFKT+WFKT_IXCT)

#define GADID_STEPS		(GADID_BANDS+1)
#define GADIX_STEPS		(GADIX_BANDS+1)

#define GADID_ZEROS		(GADID_STEPS+1)
#define GADIX_ZEROS		(GADIX_STEPS+1)

#define GADID_BANDFRQ	(GADID_ZEROS+1)
#define GADIX_BANDFRQ	(GADIX_ZEROS+1)

#define GADID_ASRC		(GADID_BANDFRQ+1)
#define GADIX_ASRC		(GADIX_BANDFRQ+1)

#define GADID_AMODE		(GADID_ASRC+SRC_IDCT)
#define GADIX_AMODE		(GADIX_ASRC+SRC_IXCT)

#define GADID_ADAPT		(GADID_AMODE+1)
#define GADIX_ADAPT		(GADIX_AMODE+1)

#define GADID_UP2		(GADID_ADAPT+1)
#define GADID_UP		(GADID_UP2+1)
#define GADID_DOWN		(GADID_UP+1)
#define GADID_DOWN2		(GADID_DOWN+1)

#define GADID_LEFT2		(GADID_DOWN2+1)
#define GADID_LEFT		(GADID_LEFT2+1)
#define GADID_HORIZFLIP	(GADID_LEFT+1)
#define GADID_RIGHT		(GADID_HORIZFLIP+1)
#define GADID_RIGHT2	(GADID_RIGHT+1)

#define LAST_GADIX		(GADIX_ADAPT+1)

// helper

#define GetBandFrequency(band,srat) (((double)(band)*(double)((srat)>>1))/64.0)

// adapt modes

#define ADAPT_SUPRESS	0
#define ADAPT_AMPLIFY	1

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[4];
};

UBYTE 		aktband=0;		// wegen RefreshGUI

UBYTE *BandLabels[]={ "4","8","16","32","64","128","256","512",0l };
UBYTE *StepLabels[]={ "1","2","4","8","16","32","64","128","256",0l };
UBYTE *ModeLabels[]={ "Cur","All",0l };
UBYTE *AdapLabels[]={ "Supress","Amplify",0l };

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int SAVEDS ASM __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) goto Error;
	return(FALSE);
	Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void SAVEDS ASM __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	if(cfg)				CfgIO_RemConfig(cfg);
	SFXMod_CloseCommonAndGUILibs;
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

void SAVEDS ASM *LIBSFXMod_ClientDataInit(REG(a0,RTime *RunTime_)) {
	struct Instance *instance;

	INTRO;

	if(instance=(struct Instance *)AllocVec(sizeof(struct Instance),MEMF_ANY|MEMF_CLEAR)) {
		SFXMod_ModDescInit(0,PD_SOURCE      ,"S1",&(instance->src));
		SFXMod_ModDescInit(1,PD_WINFUNCTION ,"W1",&(instance->wfkt1));
		SFXMod_ModDescInit(2,PD_SOURCE      ,"S2",&(instance->asrc));
		SFXMod_ModDescInit(3,PD_END         ,0l  ,0l);	
		SFXMod_ClientDataInitCommon;
	}
	OUTRO;

	return((void *)instance);
}

SFXMod_DoClientDataDone

//-- Modulroutinen

UBYTE SAVEDS ASM LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	SFXMod_Interface_GUIVariables
	SFXMod_Interface_CfgSelVariables
	SFXMod_Interface_NotifyVariables

	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+RunTime->scy[117+FRMTITLE_HEIGHT];
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(1);
	UWORD ys5=ys4+1+ANYFRM_HEIGHT(1);
	UWORD ys6=ys5+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys6+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+RunTime->scx[553+10];
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

	UWORD xs=xs1+RunTime->scx[ 11],ys=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
	UWORD xl=2  +RunTime->scx[520],yl=2  +RunTime->scy[100];
	UWORD xe=xs+xl,ye=ys+yl;

	register UBYTE o;
	UBYTE mouseb=FALSE,range=FALSE,amode=FALSE;
	UBYTE omptr=MPTR_STANDARD;
	UBYTE oldband;
	UWORD mousex,mousey;
	double fact,frq;
	double newval,oldval,dif;
	LONG val1;
	Param param;

	INTRO;
	aktband=0;
	wintags[4].ti_Data|=IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS;
	wintags[5].ti_Data|=WFLG_REPORTMOUSE;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[226];
		ng.ng_TopEdge		=ys6+RunTime->scy[2];
		ng.ng_Width			=RunTime->scx[100];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	=GetString(btnProcess);
		ng.ng_TextAttr		=RunTime->ScrFontB;
		ng.ng_GadgetID		=GADID_GO;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime->VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_GO]=g;

		InitCfgSel(&cfgsel,PROGDIR"_operators/"PRJ_NAME);
		g=AddCfgSel(&cfgsel,modGadgets,g,GADID_CFGSEL,GADIX_CFGSEL,xs2,ys1,winh-2);

		g=AddSource(&(instance->src),modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];			// single band editing
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[40];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="Band";
		ng.ng_GadgetID		=GADID_BAND;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,3,GTIN_Number,aktband,TAG_DONE);
		modGadgets[GADIX_BAND]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[125];
		ng.ng_Width			=RunTime->scx[60];
		ng.ng_GadgetText	="Val";
		ng.ng_GadgetID		=GADID_VAL;
		sprintf(param,"%lf",instance->eqf[aktband]);
		UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->eqfstr,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_VAL]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[225];
		ng.ng_Width			=RunTime->scx[140];
		ng.ng_GadgetText	="Frq";
		ng.ng_GadgetID		=GADID_BANDFRQ;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,TAG_DONE);
		modGadgets[GADIX_BANDFRQ]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[370];			// curve processing
		ng.ng_Width			=RunTime->scx[70];
		ng.ng_GadgetText	="Range";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_RANGE;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[488];
		ng.ng_GadgetText	="Mode";
		ng.ng_GadgetID		=GADID_MODE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ModeLabels,GTCY_Active,instance->mode,TAG_DONE);
		modGadgets[GADIX_MODE]=g;

		g=AddWinFunction(&(instance->wfkt1),modGadgets,g,GADID_WFKT,GADIX_WFKT,xs1,ys4+RunTime->scy[FRMCONTENT_YOFFSET]);

		ng.ng_LeftEdge		=xs1+RunTime->scx[252];			// fft params
		ng.ng_TopEdge		=ys4+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[70];
		ng.ng_GadgetText	="Bands";
		ng.ng_GadgetID		=GADID_BANDS;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,BandLabels,GTCY_Active,instance->bands,TAG_DONE);
		modGadgets[GADIX_BANDS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[370];
		ng.ng_GadgetText	="Steps";
		ng.ng_GadgetID		=GADID_STEPS;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,StepLabels,GTCY_Active,instance->steps,TAG_DONE);
		modGadgets[GADIX_STEPS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[488];
		ng.ng_GadgetText	="Zeros";
		ng.ng_GadgetID		=GADID_ZEROS;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_zeros,TAG_DONE);
		modGadgets[GADIX_ZEROS]=g;
															// curve adaption
		g=AddSource(&(instance->asrc),modGadgets,g,GADID_ASRC,GADIX_ASRC,xs1,ys5);

		ng.ng_LeftEdge		=xs1+RunTime->scx[370];
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[100];
		ng.ng_GadgetText	="Effect";
		ng.ng_GadgetID		=GADID_AMODE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,AdapLabels,GTCY_Active,amode,TAG_DONE);
		modGadgets[GADIX_AMODE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[488];
		ng.ng_Width			=RunTime->scx[70];
		ng.ng_GadgetText	="Adapt";
		ng.ng_GadgetID		=GADID_ADAPT;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_ADAPT]=g;

		ng.ng_LeftEdge		=xs;							// Arrows horiz.
		ng.ng_TopEdge		=ye+1;
		ng.ng_Width			=RunTime->scx[104];
		ng.ng_Height		=RunTime->scy[10];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_ArrowLeft2;
		ng.ng_GadgetID		=GADID_LEFT2;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs+RunTime->scx[104];
		ng.ng_GadgetText	=IMAGE_ArrowLeft;
		ng.ng_GadgetID		=GADID_LEFT;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs+RunTime->scx[208];
		ng.ng_Width			=RunTime->scx[106];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="F";
		ng.ng_GadgetID		=GADID_HORIZFLIP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs+RunTime->scx[314];
		ng.ng_Width			=RunTime->scx[104];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_ArrowRight;
		ng.ng_GadgetID		=GADID_RIGHT;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs+RunTime->scx[418];
		ng.ng_GadgetText	=IMAGE_ArrowRight2;	
		ng.ng_GadgetID		=GADID_RIGHT2;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xe+1;							/* Arrows vert. */
		ng.ng_TopEdge		=ys;
		ng.ng_Width			=RunTime->scx[19];
		ng.ng_Height		=RunTime->scy[26];
		ng.ng_GadgetText	=IMAGE_ArrowUp2;
		ng.ng_GadgetID		=GADID_UP2;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_TopEdge		=ys+RunTime->scy[26];
		ng.ng_Height		=RunTime->scy[25];
		ng.ng_GadgetText	=IMAGE_ArrowUp;
		ng.ng_GadgetID		=GADID_UP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_TopEdge		=ys+RunTime->scy[51];
		ng.ng_GadgetText	=IMAGE_ArrowDown;
		ng.ng_GadgetID		=GADID_DOWN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_TopEdge		=ys+RunTime->scy[76];
		ng.ng_Height		=RunTime->scy[26];
		ng.ng_GadgetText	=IMAGE_ArrowDown2;	
		ng.ng_GadgetID		=GADID_DOWN2;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Window",xs1+RunTime->scx[45],ys4+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys5+RunTime->scy[FRMTEXT_YPOS(1)]);

			xh=RunTime->scx[553+10];
			DrawTitle(modrp,"Source"			,xs1,ys1,xh);
			DrawTitle(modrp,"Frequency-Curve"	,xs1,ys2,xh);
			DrawTitle(modrp,"Curve-Editing"		,xs1,ys3,xh);
			DrawTitle(modrp,"FFT-Parameters"	,xs1,ys4,xh);
			DrawTitle(modrp,"Curve-Adaption"	,xs1,ys5,xh);
			DrawTitle(modrp,"Presets"			,xs2,ys1,CFGSEL_WIDTH);

			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);						/* Src */
			DrawGadWB(modrp,xs1,ys2,xh,RunTime->scy[117+FRMTITLE_HEIGHT]);	/* EQ-Field */
			DrawGadWB(modrp,xs1,ys3,xh,ANYFRM_HEIGHT(1));					/* Src */
			DrawGadWB(modrp,xs1,ys4,xh,ANYFRM_HEIGHT(1));					/* Par */
			DrawGadWB(modrp,xs1,ys5,xh,ANYFRM_HEIGHT(1));					/* Par */
			DrawGadWB(modrp,xs1,ys6,xh,CMDFRM_HEIGHT);						/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);			/* CfgSel */

			DrawGadBW(modrp,xs,ys,xl,yl);
			DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);

			SetCfgSel     (&cfgsel        ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource     (&(instance->src  ),modWin,modGadgets,GADIX_SRC);
			SetSource     (&(instance->asrc ),modWin,modGadgets,GADIX_ASRC);
			SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);

			if(!enabled) {
				GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(modGadgets[GADIX_ADAPT],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
			}
			if(instance->src.srcbuf) {
				sprintf(param,"%ld Hz - %ld Hz",(ULONG)GetBandFrequency(aktband  ,instance->src.srcbuf->srat),(ULONG)GetBandFrequency(aktband+1,instance->src.srcbuf->srat));
				GT_SetGadgetAttrs(modGadgets[GADIX_BANDFRQ],modWin,0l,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
			}

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=TRUE;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_ADAPT],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						instance->src.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src.rmode=SetRngMode(instance->src.srcbuf);
						instance->asrc.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->asrc.rmode=SetRngMode(instance->src.srcbuf);
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=FALSE;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_ADAPT],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					}
					SetSource   (&(instance->src ),modWin,modGadgets,GADIX_SRC );
					SetSource   (&(instance->asrc),modWin,modGadgets,GADIX_ASRC);
				}
				if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						mousex	=(imsg->MouseX)-(modWin->BorderLeft);
						mousey	=(imsg->MouseY)-(modWin->BorderTop);
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(!HandleSource     (&(instance->src),  modWin,modGadgets,GADID_SRC,   GADIX_SRC,   gnr,icode)
								&& !HandleSource     (&(instance->asrc), modWin,modGadgets,GADID_ASRC,  GADIX_ASRC,  gnr,icode)
								&& !HandleWinFunction(&(instance->wfkt1),modWin,modGadgets,GADID_WFKT,  GADIX_WFKT,  gnr,icode)
								&& !HandleCfgSel     (&cfgsel        ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_BAND:
											CheckGad_int(modGadgets[GADIX_BAND],modWin,&val1,1,0,65);aktband=(UBYTE)val1;
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											if(instance->src.srcbuf) {
												sprintf(param,"%ld Hz - %ld Hz",(ULONG)GetBandFrequency(aktband  ,instance->src.srcbuf->srat),(ULONG)GetBandFrequency(aktband+1,instance->src.srcbuf->srat));
												GT_SetGadgetAttrs(modGadgets[GADIX_BANDFRQ],modWin,0l,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
											}
											ActivateGadget(modGadgets[GADIX_VAL],modWin,0L);
											break;
										case GADID_VAL:
											HandleParameter(instance->eqfstr,modWin,modGadgets[GADIX_VAL]);
											UC_ConvertParameter(param,instance->eqfstr,UGRP_AMPLITUDE,AMP_FC);
											newval=atof(param);
											if(newval<0.0) newval=0.0;
											else if(newval>4.0) newval=4.0;
											DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
											instance->eqf[aktband]=newval;
											DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											ActivateGadget(modGadgets[GADIX_BAND],modWin,0L);
											break;
										case GADID_MODE:
											instance->mode=icode;
											break;
										case GADID_BANDS:
											instance->bands=icode;
											break;
										case GADID_STEPS:
											instance->steps=icode;
											break;
										case GADID_ZEROS:
											HandleParameter(instance->p_zeros,modWin,modGadgets[GADIX_ZEROS]);
											break;
										case GADID_RANGE:
											range=1;
											break;
										case GADID_AMODE:
											amode=icode;
											break;
										case GADID_ADAPT:
											AdaptEqfs(instance,amode);
											DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											break;
										case GADID_LEFT2:
											for(o=0;o<60;o++) instance->eqf[o]=instance->eqf[o+5];
											instance->eqf[64]=instance->eqf[63]=instance->eqf[62]=instance->eqf[61]=instance->eqf[60]=1.0;
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											break;
										case GADID_LEFT:
											for(o=0;o<64;o++) instance->eqf[o]=instance->eqf[o+1];
											instance->eqf[64]=1.0;
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											break;
										case GADID_HORIZFLIP:
											for(o=0;o<32;o++) {
												oldval=instance->eqf[o];
												instance->eqf[o]=instance->eqf[64-o];
												instance->eqf[64-o]=oldval;
											}
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											break;
										case GADID_RIGHT:
											for(o=64;o>0;o--) instance->eqf[o]=instance->eqf[o-1];
											instance->eqf[0]=1.0;
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											break;
										case GADID_RIGHT2:
											for(o=64;o>4;o--) instance->eqf[o]=instance->eqf[o-5];
											instance->eqf[0]=instance->eqf[1]=instance->eqf[2]=instance->eqf[3]=instance->eqf[4]=1.0;
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											break;
										case GADID_UP2:
											if(instance->mode) {
												for(o=0;o<=64;o++) instance->eqf[o]=min(instance->eqf[o]+0.05,4.0);
												DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											}
											else {
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
												instance->eqf[aktband]=min(instance->eqf[aktband]+0.05,4.0);
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
											}
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											break;
										case GADID_UP:
											if(instance->mode) {
												for(o=0;o<=64;o++) instance->eqf[o]=min(instance->eqf[o]+0.01,4.0);
												DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											}
											else {
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
												instance->eqf[aktband]=min(instance->eqf[aktband]+0.01,4.0);
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
											}
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											break;
										case GADID_DOWN:
											if(instance->mode) {
												for(o=0;o<=64;o++) instance->eqf[o]=max(instance->eqf[o]-0.01,0.0);
												DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											}
											else {
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
												instance->eqf[aktband]=max(instance->eqf[aktband]-0.01,0.0);
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
											}
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											break;
										case GADID_DOWN2:
											if(instance->mode) {
												for(o=0;o<=64;o++) instance->eqf[o]=max(instance->eqf[o]-0.05,0.0);
												DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
											}
											else {
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
												instance->eqf[aktband]=max(instance->eqf[aktband]-0.05,0.0);
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
											}
											sprintf(param,"%lf",instance->eqf[aktband]);
											UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											break;
									}
								}
								break;
							case IDCMP_MOUSEBUTTONS:
								if(icode==SELECTDOWN) mouseb=TRUE;
								if(icode==SELECTUP && mouseb) {
									mouseb=FALSE;
									GT_SetGadgetAttrs(modGadgets[GADIX_BAND],modWin,0l,GTIN_Number,aktband,TAG_DONE);
									sprintf(param,"%lf",instance->eqf[aktband]);
									UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
									GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
									if(instance->src.srcbuf) {
										sprintf(param,"%ld Hz - %ld Hz",(ULONG)GetBandFrequency(aktband  ,instance->src.srcbuf->srat),(ULONG)GetBandFrequency(aktband+1,instance->src.srcbuf->srat));
										GT_SetGadgetAttrs(modGadgets[GADIX_BANDFRQ],modWin,0l,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
									}
								}		/* No break !!! */
							case IDCMP_MOUSEMOVE:
								if((mousex>xs) && (mousex<xe) && (mousey>ys) && (mousey<ye)) {
									if(mouseb) {
										if(range) {
											range=0;
											oldband=aktband;aktband=(mousex-(xs+1))/(double)RunTime->scx[8];
											if(aktband>64) aktband=64;
											if(aktband!=oldband) {
												dif=labs(aktband-oldband);
												fact=(instance->eqf[aktband]-instance->eqf[oldband])/(double)dif;frq=instance->eqf[oldband];
												if(oldband<aktband) for(o=oldband;o<aktband;o++) { instance->eqf[o]=frq;frq+=fact; }
												else for(o=oldband;o>aktband;o--) { instance->eqf[o]=frq;frq+=fact; }
												DrawEqfs(instance,modrp,xs+1,ys+1,xe-1,ye-1);
												if(instance->src.srcbuf) {
													sprintf(param,"%ld Hz - %ld Hz",(ULONG)GetBandFrequency(aktband  ,instance->src.srcbuf->srat),(ULONG)GetBandFrequency(aktband+1,instance->src.srcbuf->srat));
													GT_SetGadgetAttrs(modGadgets[GADIX_BANDFRQ],modWin,0l,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
												}
											}
										}
										else {
											oldband=aktband;aktband=(mousex-(xs+1))/(double)RunTime->scx[8];
											if(aktband>64) aktband=64;
											if(aktband!=oldband) {
												GT_SetGadgetAttrs(modGadgets[GADIX_BAND],modWin,0l,GTIN_Number,aktband,TAG_DONE);
												sprintf(param,"%lf",instance->eqf[aktband]);
												UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
												GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
												if(instance->src.srcbuf) {
													sprintf(param,"%ld Hz - %ld Hz",(ULONG)GetBandFrequency(aktband  ,instance->src.srcbuf->srat),(ULONG)GetBandFrequency(aktband+1,instance->src.srcbuf->srat));
													GT_SetGadgetAttrs(modGadgets[GADIX_BANDFRQ],modWin,0l,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
												}
											}
											oldval=instance->eqf[aktband];
											val1=((ye-1)-mousey)*(4.0/RunTime->scaley);
											if(val1>400) val1=400;
											if(val1<0) val1=0;
											if(val1!=oldval) {
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
												instance->eqf[aktband]=((double)val1/100.0);
												DrawEqf(instance,modrp,aktband,xs+1,ys+1,xe-1,ye-1);
												sprintf(param,"%lf",instance->eqf[aktband]);
												UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
												GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
											}
										}
									}
									if(omptr!=MPTR_SIZE82) { SetWindowPointer(modWin,WA_Pointer,RunTime->MPtrSize82->Pointer,TAG_DONE);omptr=MPTR_SIZE82; }
								}
								else {
									if(omptr!=MPTR_STANDARD) { SetWindowPointer(modWin,TAG_DONE);omptr=MPTR_STANDARD; }
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_BAND],modWin,0l);
										break;
								}
								break;
							SFXMod_Interface_Events
						}
					}
				}
			}
			SFXMod_Interface_WinDone
			SFXMod_Interface_FReqDone
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		FreeGadgets(modGList);
		DoneCfgSel(&cfgsel);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	SFXMod_Interface_NotifyDone
	OUTRO_BOOL(ret);
	return(ret);
	Error:
	OUTRO_BOOL(FALSE);
	return(FALSE);
}

UBYTE SAVEDS ASM LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	//-- loop
	UBYTE l;
	//-- prowin
	PWData *pwd=NULL;
	ProWinTitle title;
	//-- procesing length
	ULONG rm_start,rm_len;
	//-- src & dst buffers
	SInfo *dstbuf=NULL,*srcbuf;
	//-- buffer access
	SmpAccess *srcsa,*dstsa;
	SAFuncPtr SARead,SAWrite;
	ULONG blen;
	//-- administrative
	Param param;
	UBYTE newdst=FALSE;
	UBYTE okay=TRUE;

	INTRO;

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:				// wir wurden e.g. per ARexx gestartet
			UnlockBuffer(instance->src.srcbuf);
			if((instance->src.srcbuf=LockBuffer(pdata->src[0]))==NULL) return(FALSE);
			instance->src.rmode=RNGMD_ALL;
			break;
		case CM_STREAM:
			UnlockBuffer(instance->src.srcbuf);
			if((instance->src.srcbuf=LockBuffer(pdata->src[0]))==NULL) return(FALSE);
			instance->src.rmode=RNGMD_ALL;
			dstbuf=pdata->dst[0];
			break;
		case CM_PREHEAR:
			break;
	}

	UC_ConvertParameter(param,instance->p_zeros,UGRP_PROPOTION,PROP_FC);
	instance->zeros=atof(param);

	srcbuf=instance->src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);
	instance->slen=rm_len;
	instance->srat=srcbuf->srat;

	if(!dstbuf) {
		if(dstbuf=NewSample()) {
			if(AllocSampleMem(dstbuf,srcbuf->slen,CH_COUNT(srcbuf->channels))) {
				BufferFill(srcbuf,dstbuf);
				newdst=TRUE;
			}
			else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
	}
	if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
				if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {
					activate_ch((SFXCoreInstance *)instance,l);
					if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);
					srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					dstsa=SAInit(dstbuf,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					if(srcsa && dstsa) {
						SARead =srcsa->safunc;
						SAWrite=dstsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
						for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
							instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
							process((SFXCoreInstance *)instance,blen);
							SAWrite(dstsa,dstsa->blen);					// akt Block schreiben
						}
					}
					else okay=FALSE;
					if(srcsa) SADone(srcsa,srcbuf);
					if(dstsa) SADone(dstsa,dstbuf);
					if(rm_start+rm_len<srcbuf->slen) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }										  // es wurde abgebrochen
				}
				else CopyDataCh(srcbuf,dstbuf,l,0,0,srcbuf->slen);
			}
			if(okay && newdst) AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);
		}
		else okay=FALSE;
		deactivate((SFXCoreInstance *)instance);
		DelProWin(pwd);
	}
	else okay=FALSE;
	if(!okay) {
		if(newdst) RemSample(dstbuf);
		OUTRO_BOOL(FALSE);
		return(FALSE);
	}
	OUTRO_BOOL(TRUE);
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;pf=ParseWinFunctionSet(&(instance->wfkt1),"W1",params);
		if(!pf && !stricmp(RexxPar2,"Lev"))			{ instance->eqf[atoi(RexxPar3)%65]=(double)(atoi(RexxPar4)%100)/100.0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Mode"))		{ instance->mode		=CfgIO_MatchString(RexxPar3,ModeLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ instance->bands	=CfgIO_MatchString(RexxPar3,BandLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Steps"))		{ instance->steps	=CfgIO_MatchString(RexxPar3,StepLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Zeros"))		{ strntcpy(instance->p_zeros ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=ParseWinFunctionGet(&(instance->wfkt1),"W1",params,ret);
		if(!pf && !stricmp(RexxPar2,"Lev"))			{ sprintf(ret,"%d",(ULONG)(instance->eqf[atoi(RexxPar3)%65]*100.0));pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Mode"))		{ strcpy(ret,ModeLabels[instance->mode]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ strcpy(ret,BandLabels[instance->bands]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Steps"))		{ strcpy(ret,StepLabels[instance->steps]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Zeros"))		{ strntcpy(ret,instance->p_zeros ,PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	register UBYTE i;
	char strItemName[10];
	//double dmy=1.0;

	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	for(i=0;i<65;i++) {
		sprintf(strItemName,"Eqf%d",i);
		//instance->eqf[i]=CfgIO_ReadFloat		(cfg,"General"	,strItemName,&dmy);			// Rückgabe von double funktioniert nicht
		instance->eqf[i]=atof(CfgIO_ReadString	(cfg,"General"	,strItemName,"1.0"));
	}
	instance->mode=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Mode"	,ModeLabels[instance->mode]),ModeLabels);
	instance->bands=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Bands",BandLabels[instance->bands]),BandLabels);
	instance->steps=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Steps",StepLabels[instance->steps]),StepLabels);
	strcpy(instance->p_zeros,	CfgIO_ReadString	(cfg,"General"	,"Zeros"	,instance->p_zeros));
}

void SaveSettings(struct Instance *instance,char *fname) {
	register UBYTE i;
	char strItemName[10];

	SaveModSettings(cfg,instance->moddesc);
	for(i=0;i<65;i++) {
		sprintf(strItemName,"Eqf%d",i);
		CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->eqf[i]);
	}
	CfgIO_WriteString		  (cfg,"General"	,"Mode"			,ModeLabels[instance->mode]);
	CfgIO_WriteString		  (cfg,"General"	,"Bands"		,BandLabels[instance->bands]);
	CfgIO_WriteString		  (cfg,"General"	,"Steps"		,StepLabels[instance->steps]);
	CfgIO_WriteString		  (cfg,"General"	,"Zeros"		,instance->p_zeros);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	register UBYTE i;
	
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;
	instance->asrc.srcbuf=0L;
	instance->asrc.rmode=RNGMD_ALL;

	instance->wfkt1.win=WIN_HAMMING;
	instance->wfkt1.par=0.54;
	instance->wfkt1.desc[0]='\0';

	for(i=0;i<65;i++) instance->eqf[i]=1.0;
	instance->bands=4;						// 64
	instance->steps=4;						// 16
	UC_ConvertParameter(instance->p_zeros ,"50 %",UGRP_PROPOTION,PROP_PROZ);
	instance->mode=0;										/* Cur */
	UC_ConvertParameter(instance->eqfstr,"1.0",UGRP_AMPLITUDE,AMP_FC);
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	char param[PARBUFLEN+1];
	
	SetSource     (&(instance->src),  modWin,modGadgets,GADIX_SRC);
	SetSource     (&(instance->asrc), modWin,modGadgets,GADIX_ASRC);
	SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);
	sprintf(param,"%lf",instance->eqf[aktband]);
	UC_ConvertParameter(instance->eqfstr,param,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->eqfstr,UGRP_AMPLITUDE));
	GT_SetGadgetAttrs(modGadgets[GADIX_BAND   ],modWin,0l,GTIN_Number,aktband,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_VAL    ],modWin,0l,GTST_String,instance->eqfstr,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MODE   ],modWin,0l,GTCY_Active,instance->mode,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_BANDS  ],modWin,0l,GTCY_Active,instance->bands,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_STEPS  ],modWin,0l,GTCY_Active,instance->steps,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_ZEROS  ],modWin,0l,GTST_String,instance->p_zeros,TAG_DONE);
	if(instance->src.srcbuf) {
		sprintf(param,"%ld Hz - %ld Hz",(ULONG)GetBandFrequency(aktband  ,instance->src.srcbuf->srat),(ULONG)GetBandFrequency(aktband+1,instance->src.srcbuf->srat));
		GT_SetGadgetAttrs(modGadgets[GADIX_BANDFRQ],modWin,0l,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
	}

	{
		// this is a very ugly hack, the slider field should realy be a custom boopsi gadget
		UWORD ys2=2 + 1+SRCSEL_HEIGHT;
		UWORD xs1=2;

		UWORD xs=xs1+RunTime->scx[ 11],ys=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		UWORD xl=2  +RunTime->scx[520],yl=2  +RunTime->scy[100];
		UWORD xe=xs+xl,ye=ys+yl;

		DrawEqfs(instance,modWin->RPort,xs+1,ys+1,xe-1,ye-1);
	}
}

//-- private Stuff ------------------------------------------------------------

void DrawEqfs(struct Instance *instance,struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye) {
	register UWORD j,y;
	UBYTE col;

	SetAPen(rp,5);RectFill(rp,xs,ys,xe,ye);			/* Löschen */

	SetAPen(rp,0);									/* Balken */
	for(j=0;j<=64;j+=2) RectFill(rp,xs+RunTime->scx[(j<<3)],ys,xs+RunTime->scx[7+(j<<3)],ye);

	SetAPen(rp,6);									/* vert. Linien */
	Move(rp,xs,ye-RunTime->scy[25]);Draw(rp,xe,ye-RunTime->scy[25]);
	SetDrPt(rp,43690);
	Move(rp,xs,ye-RunTime->scy[50]);Draw(rp,xe,ye-RunTime->scy[50]);
	Move(rp,xs,ye-RunTime->scy[75]);Draw(rp,xe,ye-RunTime->scy[75]);

	for(j=0;j<=64;j+=10) {							/* horiz. Linien */
		Move(rp,xs+RunTime->scx[3+(j<<3)],ys);
		Draw(rp,xs+RunTime->scx[3+(j<<3)],ye);
	}
	SetDrPt(rp,65535);

	SetAPen(rp,RunTime->sfxprefs_gui->Pens[PEN_SMP_LOOP]);	/* Regler */
	SetDrMd(rp,COMPLEMENT);
	for(j=0;j<=64;j++) {
		y=(UWORD)(instance->eqf[j]*25.0);
		col=RunTime->sfxprefs_gui->Pens[PEN_SMP_MARK]^(UBYTE)((j&1)?5:0);
		SetAPen(rp,col);SetWriteMask(rp,col);
		Move(rp,xs+RunTime->scx[(j<<3)],ye-RunTime->scy[y]);Draw(rp,xs+RunTime->scx[6+(j<<3)],ye-RunTime->scy[y]);
	}
	SetWriteMask(rp,0xFF);SetDrMd(rp,JAM2);
}

void DrawEqf(struct Instance *instance,struct RastPort *rp,UBYTE aktband,UWORD xs,UWORD ys,UWORD xe,UWORD ye) {
	register UWORD y;
	UBYTE col=RunTime->sfxprefs_gui->Pens[PEN_SMP_MARK]^(UBYTE)((aktband&1)?5:0);

	SetAPen(rp,col);SetWriteMask(rp,col);SetDrMd(rp,COMPLEMENT);
	y=(UWORD)(instance->eqf[aktband]*25.0);
	Move(rp,xs+RunTime->scx[(aktband<<3)],ye-RunTime->scy[y]);Draw(rp,xs+RunTime->scx[6+(aktband<<3)],ye-RunTime->scy[y]);
	SetWriteMask(rp,0xFF);SetDrMd(rp,JAM2);
}

void AdaptEqfs(struct Instance *instance,UBYTE amode) {
	ULONG pos;
	register UWORD o;
	UBYTE l;
	register ULONG j;
	PWData *pwd=NULL;
	ProWinTitle title;
	SInfo *srcbuf;
	ULONG rm_start,rm_len,blen;
	SAMPLE *sbuf;
	SAMPLE *irbufr,*irbufw;
	SmpAccess *srcsa;
	SAFuncPtr SARead;
	UWORD mb,mw,ms,mv;
	ULONG size;
	double *flbuf_r,*flbuf_i=0l,*flp1_r,*flp1_i;
	double *avgbuf=0L,*avgptr;
	double *win_fkt1=0l,*winfp;
	UWORD *bitrevtab=0l;
	double *sintab1=0l,*sintab2=0l,*costab=0l;
	double div1,div2,div3,fact;
	SAMPLE *irbuf=0l,*irbufbase;
	ULONG irbufmask,irbufsize;

	for(o=0;o<=64;o++) instance->eqf[o]=1.0;

	srcbuf=instance->asrc.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,instance->asrc.rmode);
	instance->slen=rm_len;
	instance->srat=srcbuf->srat;

	mb=1<<(instance->bands+2);						// the number of frequency bands we want to get
	mw=mb<<1;										// the width of the databuffer is twice as large as the number of bands
	mv=mw-(mw>>1);									// put how many values into databuffer, zero the rest
	ms=1<<instance->steps;							// do a analysys every <ms> steps
	if(ms>=(mb>>1)) ms=mb>>1;						// steps must be at least mb/2
	size=(mw+2)*sizeof(double);
	if(	(flbuf_r	=(double *)AllocVec(size,MEMF_ANY))
	&&	(flbuf_i	=(double *)AllocVec(size,MEMF_ANY))
	&&	(win_fkt1	=NewWindowFkt(mv,instance->wfkt1.win,&instance->wfkt1.par))
	&&				 NewFFTTabs(&bitrevtab,&sintab1,&sintab2,&costab,mb)
	&&	(avgbuf		=(double *)AllocVec((mb+1)*sizeof(double),MEMF_ANY|MEMF_CLEAR))
	&&	(irbuf		=NewRingBuf(mw,&irbufbase,&irbufmask,&irbufsize))
	&&	(pwd		=NewProWin())) {					// ein neues asynchrones Statusfenster öffnen
		//for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
		for(l=0;l<1;l++) {
			if((srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {	// sequentiell Blöcke konstanter Größe lesen
				MSG("analysing");
				SARead =srcsa->safunc;
				blen=Bytes2Samples(srcsa->blen);
				div1=div2=1.0;
				memset(irbuf,0,(irbufsize<<1));irbufw=irbufbase;
				pos=(ms-1);			// weil curlen erst später erhöht wird
				sprintf(title,GetString(msgProgress),PRJ_NAME"/Adapt",l+1,1/*CH_COUNT(srcbuf->channels)*/);
				StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
				for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++) {
					sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
					j=0;
					while((j<blen) && (instance->curlen<instance->slen)) {		// die Blocklänge durchgehen
						*irbufw=*(sbuf++);
						irbufw=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufw+1)&irbufmask));
						if(instance->curlen==pos) {										  /* we've read the next ms samples */
							pos+=ms;
							flp1_r=flbuf_r;flp1_i=flbuf_i;winfp=win_fkt1;
							irbufr=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufw-mv)&irbufmask));
							for(o=0;o<mv;o++) {	/* read in values */
								*(flp1_r++)=(*(winfp++))*((double)(*irbufr)/32768.0);*(flp1_i++)=0.0;
								irbufr=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufr+1)&irbufmask));
							}
							for(o=mv;o<mw;o++) {/* set rest to zero */
								*(flp1_r++)=*(flp1_i++)=0.0;
							}

							Transform(flbuf_r,flbuf_i,bitrevtab,sintab1,costab,-1,mb);	/* -> frequency domain */

							flp1_r=flbuf_r;flp1_i=flbuf_i;avgptr=avgbuf;
							div3=div1/div2;
							for(o=0;o<=mb;o++) {
								*avgptr=(*avgptr+sqrt((*flp1_r)*(*flp1_r)+(*flp1_i)*(*flp1_i))/div1)*div3;	/* average (without overflow) */
								flp1_r++;flp1_i++;avgptr++;
							}
							div1=div2;div2+=1.0;
						}
						instance->curlen++;j++;
					}
				}
				if(ProWinAborted(instance->curlen)) l=CH_MAX;					// es wurde abgebrochen
				else {
					// DEBUG
					//div1=0.0;/*fact=(double)(m>>1);*/
					//for(o=0;o<=m;o++) {
					//	//avgbuf[o]*=fact;
					//	div1+=avgbuf[o];
					//}
					//MSG1("summe=%8.6lf",div1);
					// DEBUG
					fact=(double)mb/64.0;
					switch(amode) {
						case ADAPT_SUPRESS:
							for(o=0;o<=64;o++) {
								instance->eqf[o]=1.0-avgbuf[(UWORD)(o*fact)];
								//MSG3("shape[%3d->%3d]=%8.6lf",o,(UWORD)(o*fact),avgbuf[(UWORD)(o*fact)]);
							}
							break;
						case ADAPT_AMPLIFY:
							for(o=0;o<=64;o++) instance->eqf[o]=1.0+avgbuf[(UWORD)(o*fact)];
							break;
					}
					MSG("adapted");
				}
				SADone(srcsa,srcbuf);
			}
		}
	}
	else { Message(errAllocateMemory,NULL,__FILE__,__LINE__); }
	if(pwd)			DelProWin(pwd);
	if(flbuf_r)		FreeVec((ULONG *)flbuf_r);
	if(flbuf_i)		FreeVec((ULONG *)flbuf_i);
					DelFFTTabs(bitrevtab,sintab1,sintab2,costab);
	if(win_fkt1)	DelWindowFkt(win_fkt1);
	if(avgbuf)		FreeVec((ULONG *)avgbuf);
}

//-- eof ----------------------------------------------------------------------
