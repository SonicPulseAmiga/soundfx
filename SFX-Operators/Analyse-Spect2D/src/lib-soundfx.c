/****h* Analyse-Spect2D/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2.(31.10.01) by Stefan Kost 1998-2001
*  FUNCTION
*    Analyse-Spect2D Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    31.Oct.2001	V 4.2	most recent version
*    27.Mar.2001	V 4.1	most recent version
*							A: frq is now shown as key as well (for a certain range)
*							A: gadgets for time,frq and mag
*							F: instance->lines is now bounded by minlines & maxlines
*							A: mouse pointer cross with harmonics
*    01.Aug.2000	V 1.2	most recent version
*    18.Jul.1998	V 1.2	initial version
*  NOTES
*  IDEAS
*    * make Interface & GraphView one window
*    * new Buttons
*      * Graph for Channel
*      * Toggle Color (color,gray-bw,gray-wb)
*      * adjustable Gamma via mouse (add a slider below color-gradient)
*    * check if there is enough space to show textfield in two or one lines in GraphView
*******
*/

#define ANALYSE_SPECT2D_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Analyse-Spect2D.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

UBYTE AnalyseFFT(struct Instance *instance,SInfo *srcbuf,ULONG rm_start,ULONG rm_len,UBYTE win,double par,UBYTE hmax);
char *ToXUnit(UBYTE unitx,ULONG  val,ULONG srat);
char *ToYUnit(UBYTE unity,double val);
void DisplayResults(struct Instance *instance,SInfo *srcbuf,ULONG rm_start,ULONG rm_len);

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

#define GADID_LINES		(GADID_SRC+SRC_IDCT)
#define GADIX_LINES		(GADIX_SRC+SRC_IXCT)

#define GADID_LINERANGE	(GADID_LINES+1)
#define GADIX_LINERANGE	(GADIX_LINES+1)

#define GADID_BANDS		(GADID_LINERANGE+1)
#define GADIX_BANDS		(GADIX_LINERANGE+1)

#define GADID_GAMMA		(GADID_BANDS+1)
#define GADIX_GAMMA		(GADIX_BANDS+1)

#define GADID_PALETTE	(GADID_GAMMA+1)
#define GADIX_PALETTE	(GADIX_GAMMA+1)

#define GADID_PREC		(GADID_PALETTE+1)
#define GADIX_PREC		(GADIX_PALETTE+1)

#define GADID_WFKT		(GADID_PREC+1)
#define GADIX_WFKT		(GADIX_PREC+1)

#define GADID_MAXSCAN	(GADID_WFKT+WFKT_IDCT)
#define GADIX_MAXSCAN	(GADIX_WFKT+WFKT_IXCT)

#define LAST_GADIX		(GADIX_MAXSCAN+1)


#define GADID_TIME		0
#define GADIX_TIME		0
#define GADID_FRQ		(GADID_TIME+1)
#define GADIX_FRQ		(GADIX_TIME+1)
#define GADID_MAG		(GADID_FRQ+1)
#define GADIX_MAG		(GADIX_FRQ+1)

#define LAST_GADIX2		(GADIX_MAG+1)

//-- pow(2.0,(1/12))
#define PITCH1	1.05946309436

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[3];
};

UBYTE *PrecLabels[]={ "high 2","high 1","normal","low 1","low 2",0l };
UBYTE *PaleLabels[]={ "gray","color",0l };
UBYTE *BandLabels[]={ "4","8","16","32","64","128","256","512",0l };

UWORD minlines,maxlines,maxbands;

#define MAXSCAN_FC	150.0

#define PREC_HIGH2	0
#define PREC_HIGH1	1
#define PREC_NORM	2
#define PREC_LOW1	3
#define PREC_LOW2	4

#define PREC_LINES(x) ((instance->prec>PREC_NORM)?(x>>(instance->prec-PREC_NORM)):(x<<(PREC_NORM-instance->prec)))
#define LINES_PREC(x) ((instance->prec>PREC_NORM)?(x<<(instance->prec-PREC_NORM)):(x>>(PREC_NORM-instance->prec)))

#define CROSSMODE_NONE		0
#define CROSSMODE_SIMPLE	1
#define CROSSMODE_HARMONY	2
#define CROSSMODE_MAX		2

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

	if(instance=(struct Instance *)AllocVec(sizeof(struct Instance),MEMF_ANY|MEMF_CLEAR)) {
		SFXMod_ModDescInit(0,PD_SOURCE      ,"S1",&(instance->src));
		SFXMod_ModDescInit(1,PD_WINFUNCTION ,"W1",&(instance->wfkt1));
		SFXMod_ModDescInit(2,PD_END         ,0l  ,0l);
		SFXMod_ClientDataInitCommon;
	}
	return((void *)instance);
}

SFXMod_DoClientDataDone

//-- Modulroutinen

UBYTE SAVEDS ASM LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	SFXMod_Interface_GUIVariables
	SFXMod_Interface_CfgSelVariables
	SFXMod_Interface_NotifyVariables

	UWORD winxr=RunTime->scx[46]+25,winyr=36+RunTime->scy[24];
	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(4);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

	Param param;

	maxlines=(RunTime->xres-winxr)-(instance->maxscan*(MAXSCAN_FC+5));
	minlines=RunTime->scx[100];
	if(RunTime->yres-winyr>512) maxbands=7;
	else if(RunTime->yres-winyr>256) maxbands=6;
	else maxbands=5;

	if(atoi(instance->p_lines)>(PREC_LINES(maxlines))) {
		sprintf(param,"%ld",PREC_LINES(maxlines));
		UC_ConvertParameter(instance->p_lines,param,UGRP_COUNT,COUN_ABS);
	}
	if(atoi(instance->p_lines)<(PREC_LINES(minlines))) {
		sprintf(param,"%ld",PREC_LINES(minlines));
		UC_ConvertParameter(instance->p_lines,param,UGRP_COUNT,COUN_ABS);
	}
	if(instance->bands>maxbands) instance->bands=maxbands;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[104];
		ng.ng_TopEdge		=ys3+RunTime->scy[2];
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

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="Lines";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_Flags			=PLACETEXT_LEFT;
		ng.ng_GadgetID		=GADID_LINES;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_lines,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_LINES]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="LineRng";
		ng.ng_GadgetID		=GADID_LINERANGE;
		sprintf(param,"%d...%d",PREC_LINES(minlines),PREC_LINES(maxlines));
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
		modGadgets[GADIX_LINERANGE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Bands";
		ng.ng_GadgetID		=GADID_BANDS;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,BandLabels,GTCY_Active,instance->bands,TAG_DONE);
		modGadgets[GADIX_BANDS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="Gamma";
		ng.ng_GadgetID		=GADID_GAMMA;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_gamma,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_GAMMA]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_GadgetText	="Palette";
		ng.ng_GadgetID		=GADID_PALETTE;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,PaleLabels,GTCY_Active,instance->pal,TAG_DONE);
		modGadgets[GADIX_PALETTE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="Prec.";
		ng.ng_GadgetID		=GADID_PREC;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,PrecLabels,GTCY_Active,instance->prec,TAG_DONE);
		modGadgets[GADIX_PREC]=g;

		g=AddWinFunction(&(instance->wfkt1),modGadgets,g,GADID_WFKT,GADIX_WFKT,xs1,ys2+RunTime->scy[FRMCONTENT_YOFFSET+36]);

		ng.ng_LeftEdge		=xs1+RunTime->scx[278];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+36];
		ng.ng_Width			=RunTime->scx[26];
		ng.ng_GadgetText	="MaxScan";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_MAXSCAN;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,instance->maxscan,GTCB_Scaled,TRUE,TAG_DONE);
		modGadgets[GADIX_MAXSCAN]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Window",xs1+RunTime->scx[45],ys2+RunTime->scy[FRMTEXT_YPOS(4)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"	,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(4));				/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			if(RunTime->sfxpubscr->RastPort.BitMap->Depth<=4) GT_SetGadgetAttrs(modGadgets[GADIX_PALETTE],modWin,0l,GTCY_Active,0,GA_Disabled,TRUE,TAG_DONE);

			SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource     (&(instance->src),  modWin,modGadgets,GADIX_SRC);
			SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);

			if(!enabled) GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=1;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						instance->src.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src.rmode=SetRngMode(instance->src.srcbuf);
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=0;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					}
					SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
				}
				if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(!HandleSource     (&(instance->src),  modWin,modGadgets,GADID_SRC,   GADIX_SRC,   gnr,icode)
								&& !HandleWinFunction(&(instance->wfkt1),modWin,modGadgets,GADID_WFKT,  GADIX_WFKT,  gnr,icode)
								&& !HandleCfgSel     (&cfgsel,		  modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_PALETTE:
											instance->pal=icode;
											break;
										case GADID_LINES:
											HandleParameter(instance->p_lines,modWin,modGadgets[GADIX_LINES]);
											if(atoi(instance->p_lines)>PREC_LINES(maxlines)) {
												sprintf(param,"%ld",PREC_LINES(maxlines));
												UC_ConvertParameter(instance->p_lines,param,UGRP_COUNT,COUN_ABS);
												GT_SetGadgetAttrs(modGadgets[GADIX_LINES   ],modWin,0l,GTST_String,instance->p_lines,TAG_DONE);
											}
											else if(atoi(instance->p_lines)<PREC_LINES(minlines)) {
												sprintf(param,"%ld",PREC_LINES(minlines));
												UC_ConvertParameter(instance->p_lines,param,UGRP_COUNT,COUN_ABS);
												GT_SetGadgetAttrs(modGadgets[GADIX_LINES   ],modWin,0l,GTST_String,instance->p_lines,TAG_DONE);
											}
											break;
										case GADID_BANDS:
											instance->bands=icode;
											break;
										case GADID_PREC:
											instance->prec=icode;
											if(atoi(instance->p_lines)>PREC_LINES(maxlines)) {
												sprintf(param,"%ld",PREC_LINES(maxlines));
												UC_ConvertParameter(instance->p_lines,param,UGRP_COUNT,COUN_ABS);
												GT_SetGadgetAttrs(modGadgets[GADIX_LINES   ],modWin,0l,GTST_String,instance->p_lines,TAG_DONE);
											}
											sprintf(param,"%d...%d",PREC_LINES(minlines),PREC_LINES(maxlines));
											GT_SetGadgetAttrs(modGadgets[GADIX_LINERANGE],modWin,0l,GTTX_Text  ,param,GTTX_CopyText,TRUE,TAG_DONE);
											break;
										case GADID_GAMMA:
											HandleParameter(instance->p_gamma,modWin,modGadgets[GADIX_GAMMA]);
											break;
										case GADID_MAXSCAN:
											instance->maxscan=((modGadgets[GADIX_MAXSCAN]->Flags&GFLG_SELECTED)?1:0);
											maxlines=(RunTime->xres-winxr)-(instance->maxscan*(MAXSCAN_FC+5));
											sprintf(param,"%d...%d",PREC_LINES(minlines),PREC_LINES(maxlines));
											GT_SetGadgetAttrs(modGadgets[GADIX_LINERANGE],modWin,0l,GTTX_Text  ,param,GTTX_CopyText,TRUE,TAG_DONE);
											if(atoi(instance->p_lines)>PREC_LINES(maxlines)) {
												sprintf(param,"%ld",PREC_LINES(maxlines));
												UC_ConvertParameter(instance->p_lines,param,UGRP_COUNT,COUN_ABS);
												GT_SetGadgetAttrs(modGadgets[GADIX_LINES   ],modWin,0l,GTST_String,instance->p_lines,TAG_DONE);
											}
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_LINES],modWin,0l);
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
	return(ret);
	Error:
	return(FALSE);
}

UBYTE SAVEDS ASM LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	//-- loop
	//-- prowin
	//-- procesing length
	ULONG rm_start,rm_len;
	//-- src & dst buffers
	SInfo *srcbuf;
	//-- buffer access
	//-- fx-param
	//-- fx-param modulation
	//-- administrative
	Param param;
	UBYTE okay=TRUE;

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:				// wir wurden e.g. per ARexx gestartet
		case CM_STREAM:				// no difference
			UnlockBuffer(instance->src.srcbuf);
			if((instance->src.srcbuf=LockBuffer(pdata->src[0]))==NULL) return(FALSE);
			instance->src.rmode=RNGMD_ALL;
			break;
		case CM_PREHEAR:
			break;
	}

	srcbuf=instance->src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);
	instance->slen=rm_len;
	instance->srat=srcbuf->srat;

	UC_ConvertParameter(param,instance->p_lines,UGRP_COUNT,COUN_ABS);
	instance->ml=atoi(param);

	UC_ConvertParameter(param,instance->p_gamma,UGRP_PROPOTION,PROP_FC);
	instance->gamma=atof(param);
	
	instance->m=1L<<(instance->bands+2);
		
	if(activate((SFXCoreInstance *)instance)) {
		activate_ch((SFXCoreInstance *)instance,0);

		if(AnalyseFFT(instance,srcbuf,rm_start,rm_len,instance->wfkt1.win,instance->wfkt1.par,255)) {
			/** @todo if(pdata->callmd==CM_DIRECT) SaveImage(); */
			DisplayResults(instance,srcbuf,rm_start,rm_len);
		}
		deactivate_ch((SFXCoreInstance *)instance,0);
	}
	else okay=FALSE;
	deactivate((SFXCoreInstance *)instance);
	if(!okay) {
		return(FALSE);
	}
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;pf=ParseWinFunctionSet(&(instance->wfkt1),"W1",params);
		if(!pf && !stricmp(RexxPar2,"Lines"))		{ strntcpy(instance->p_lines,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Gamma"))		{ strntcpy(instance->p_gamma,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Palette"))		{ instance->pal  =atoi(RexxPar3)%2;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ instance->bands=atoi(RexxPar3)%5;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Mode"))		{ instance->prec =atoi(RexxPar3)%3;pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=ParseWinFunctionGet(&(instance->wfkt1),"W1",params,ret);
		if(!pf && !stricmp(RexxPar2,"Lines"))		{ strntcpy(ret,instance->p_lines,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Gamma"))		{ strntcpy(ret,instance->p_gamma,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Palette"))		{ sprintf(ret,"%d",instance->pal);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ sprintf(ret,"%d",instance->bands);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Mode"))		{ sprintf(ret,"%d",instance->prec);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_lines,	CfgIO_ReadString		(cfg,"General"	,"Maxlines"	,"300"));
	strcpy(instance->p_gamma,	CfgIO_ReadString		(cfg,"General"	,"Gamma"	,"0.75"));
	instance->bands=			CfgIO_MatchString(
					 		CfgIO_ReadString		(cfg,"General"	,"Bands"	,BandLabels[5]),BandLabels);
	instance->prec=			CfgIO_MatchString(
					 		CfgIO_ReadString		(cfg,"General"	,"Mode"		,PrecLabels[1]),PrecLabels);
	instance->pal=			CfgIO_MatchString(
					 		CfgIO_ReadString		(cfg,"General"	,"Palette"	,PaleLabels[1]),PaleLabels);
	instance->maxscan=		CfgIO_ReadBool			(cfg,"General"	,"MaxScan"	,FALSE);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Maxlines"	,instance->p_lines);
	CfgIO_WriteString		  (cfg,"General"	,"Gamma"	,instance->p_gamma);
	CfgIO_WriteString		  (cfg,"General"	,"Bands"	,BandLabels[instance->bands]);
	CfgIO_WriteString		  (cfg,"General"	,"Mode"		,PrecLabels[instance->prec]);
	CfgIO_WriteString		  (cfg,"General"	,"Palette"	,PaleLabels[instance->pal]);
	CfgIO_WriteBool			  (cfg,"General"	,"MaxScan"	,instance->maxscan);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->wfkt1.win=WIN_HAMMING;
	instance->wfkt1.par=0.54;
	instance->wfkt1.desc[0]='\0';

	UC_ConvertParameter(instance->p_lines, "300" ,UGRP_COUNT,COUN_ABS);
	UC_ConvertParameter(instance->p_gamma,"0.75",UGRP_PROPOTION,PROP_FC);
	instance->pal=1;									/* colour */
	instance->bands=5;								/* => 128 */
	instance->prec=PREC_LOW1;								/* low 1 */
	instance->maxscan=0;								/* no maxscan */
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	Param param;

	SetSource     (&(instance->src),  modWin,modGadgets,GADIX_SRC);
	SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);
	if(atoi(instance->p_lines)>PREC_LINES(maxlines)) {
		sprintf(param,"%ld",PREC_LINES(maxlines));
		UC_ConvertParameter(instance->p_lines,param,UGRP_COUNT,COUN_ABS);
	}
	if(atoi(instance->p_lines)<PREC_LINES(minlines)) {
		sprintf(param,"%ld",PREC_LINES(minlines));
		UC_ConvertParameter(instance->p_lines,param,UGRP_COUNT,COUN_ABS);
	}
	GT_SetGadgetAttrs(modGadgets[GADIX_PALETTE  ],modWin,0l,GTCY_Active,instance->pal,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_LINES    ],modWin,0l,GTST_String,instance->p_lines,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_BANDS    ],modWin,0l,GTCY_Active,instance->bands,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_PREC     ],modWin,0l,GTCY_Active,instance->prec,TAG_DONE);
	sprintf(param,"%d...%d",PREC_LINES(minlines),PREC_LINES(maxlines));
	GT_SetGadgetAttrs(modGadgets[GADIX_LINERANGE],modWin,0l,GTTX_Text  ,param,GTTX_CopyText,TRUE,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_GAMMA    ],modWin,0l,GTST_String,instance->p_gamma,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAXSCAN  ],modWin,0l,GTCB_Checked,instance->maxscan,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

UBYTE AnalyseFFT(struct Instance *instance,SInfo *srcbuf,ULONG rm_start,ULONG rm_len,UBYTE win,double par,UBYTE hmax) {
	//-- loop
	register ULONG j;
	register double t;				/* Laufvariable Zeit */
	register UWORD o;				/* Laufvariable Bänder */
	//-- prowin
	PWData *pwd=NULL;
	ProWinTitle title;
	//-- procesing length
	//-- src & dst buffers
	//-- buffer access
	WORD *sbuf;
	SmpAccess *srcsa;
	SAFuncPtr SARead;
	ULONG blen;
	//-- fx-param
	//-- fx-param modulation
	//-- administrative
	//-- local
	ULONG size;
	UWORD m=instance->m,mm=m+m;
	WORD *rbufw,*rbufr;
	SAMPLE *rbuf=NULL,*rbufbase;
	register float *rasbuf;
	ULONG rbufmask,rbufsize;
	double *flbuf_r=0l,*flbuf_i=0l;
	double *win_fkt=0l;
	double rmax=0.0;				/* Variablen für Scalierung */
	register double *flp1_r,*flp1_i,*winfp;
	UWORD *bitrevtab=0l;
	double *sintab1=0l,*costab=0l;
	UBYTE notquit=TRUE;
	double st;
	UBYTE ch=0;
	float *rmaxp;

	st=(double)rm_len/(double)instance->ml;
	size=(mm+2)*sizeof(double);
	if(	(flbuf_r=(double *)AllocVec(size,MEMF_ANY))
	&&	(flbuf_i=(double *)AllocVec(size,MEMF_ANY))
	&&	(win_fkt=NewWindowFkt((mm+2),win,&par))
	&&			 NewFFTTabs(&bitrevtab,&sintab1,NULL,&costab,m)
	&&	(rbuf	=NewRingBuf(mm,&rbufbase,&rbufmask,&rbufsize))
	&&	(pwd	=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
		for(o=0;o<CH_COUNT(srcbuf->channels);o++) {					// select first active channel
			if(srcbuf->channelmask&(1<<o)) { ch=o;o=4; }
		}
		if((srcsa=SAInit(srcbuf,ch,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {	// sequentiell Blöcke konstanter Größe lesen
			SARead =srcsa->safunc;
			t=0.0;rasbuf=instance->raster;
			memset(rbuf,0,(rbufsize<<1));
			rbufw=rbufbase;
			blen=Bytes2Samples(srcsa->blen);
			sprintf(title,GetString(msgProgress),PRJ_NAME,1,3);
			StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
			for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++) {
				sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
				for(j=0;((j<blen) && (instance->curlen<instance->slen));j++,instance->curlen++) {	// die Blocklänge durchgehen
					*rbufw=*(sbuf++);										// get current value
					rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

					if(instance->curlen==(ULONG)t) {									// process a timeslice
						rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-mm))&rbufmask));
						flp1_r=flbuf_r;flp1_i=flbuf_i;winfp=win_fkt;
						for(o=0;o<=mm;o++) {
							*(flp1_r++)=(*(winfp++))*((double)*rbufr/32768.0);
							*(flp1_i++)=0.0;
							rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
						}

						Transform(flbuf_r,flbuf_i,bitrevtab,sintab1,costab,-1,m);	/* in Frequenzdarstellung transformieren */

						flp1_r=&flbuf_r[1];flp1_i=&flbuf_i[1];						// 0 is DC-Offset
						for(o=0;o<m;o++) {
							//*(rasbuf++)=sqrt((*flp1_r)*(*(flp1_r++))+(*flp1_i)*(*(flp1_i++)));	// Bug in SAS ??
							*(rasbuf++)=sqrt((*flp1_r)*(*flp1_r)+(*flp1_i)*(*flp1_i));		// really for(o=1;o<=m;o++)
							flp1_r++;flp1_i++;
						}
						t+=st;
					}
				}
			}
			if(ProWinAborted(instance->curlen)) notquit=FALSE;					// es wurde abgebrochen
			SADone(srcsa,srcbuf);
		}
		if(notquit) {
			rm_len=m*instance->ml;
			rasbuf=instance->raster;										// Maxima
			sprintf(title,GetString(msgProgress),PRJ_NAME,2,3);
			StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
			for(o=0,rmaxp=instance->rmaxa;o<m;o++,instance->curlen++,rasbuf++,rmaxp++) {
				if(*rasbuf>rmax)	rmax=*rasbuf;
				*rmaxp=*rasbuf;
			}
			for(j=1;j<instance->ml;j++) {
				for(o=0,rmaxp=instance->rmaxa;o<m;o++,instance->curlen++,rasbuf++,rmaxp++) {
					if(*rasbuf>rmax)	rmax=*rasbuf;
					if(*rasbuf>*rmaxp)	*rmaxp=*rasbuf;
				}
			}
			instance->rmaxa[m]=(float)rmax;
			if(ProWinAborted(instance->curlen)) notquit=FALSE;					// es wurde abgebrochen
		}
		if(notquit) {
			double gamma=instance->gamma;
			rasbuf=instance->raster;										// Skalieren
			sprintf(title,GetString(msgProgress),PRJ_NAME,3,3);
			StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
			for(;instance->curlen<rm_len;instance->curlen++,rasbuf++) if(*rasbuf) *rasbuf=(double)hmax*pow(((double)*rasbuf/(double)rmax),gamma);
			if(ProWinAborted(instance->curlen)) notquit=FALSE;					// es wurde abgebrochen
		}
	}
	else { Message(errAllocateMemory,NULL,__FILE__,__LINE__);notquit=FALSE; }
	size=(mm+2)*sizeof(double);
	if(pwd)			DelProWin(pwd);
	if(flbuf_r)		FreeVec((ULONG *)flbuf_r);
	if(flbuf_i)		FreeVec((ULONG *)flbuf_i);
					DelFFTTabs(bitrevtab,sintab1,NULL,costab);
	if(win_fkt)		DelWindowFkt(win_fkt);
	if(rbuf)		DelRingBuf(rbuf);
	return(notquit);
}

char *ToXUnit(UBYTE unitx,ULONG val,ULONG srat) {
	static Param text;
	Param text2;

	switch(unitx) {
		case ATME_SAMPLES:	sprintf(text,"%ld",val);break;
		case ATME_H_M_S_MS:
		case ATME_H_M_S_MF:
		case ATME_H_M_S_PF:
		case ATME_H_M_S_NF:
			sprintf(text2,"%lf ms",(double)val/((double)srat/1000.0));
			UC_ConvertParameter(text,text2,UGRP_ABSTIME,unitx);
			break;
	}
	return(text);
}

char *ToYUnit(UBYTE unity,double val){ 
	static Param text;

	switch(unity) {
		case AMP_FC:		sprintf(text,"%+6.4lf",val/32768.0);break;
		case AMP_ABSPROZ:	sprintf(text,"%+6ld %",(LONG)(val/327.68));break;
		case AMP_ABSPROM:	sprintf(text,"%+6ld %%",(LONG)(val/32.768));break;
		case AMP_LEVEL:		sprintf(text,"%+6ld",(ULONG)val);break;
		case AMP_DEZIBEL:
			if(val!=0.0) sprintf(text,"%+6.4lf db",(3.0*log(fabs(val/32768.0)))/log(1.41421356237));
			else sprintf(text,"   inf db");
			break;
	}
	return(text);
}

void DisplayResults(struct Instance *instance,SInfo *srcbuf,ULONG rm_start,ULONG rm_len) {
	register ULONG i;
	char title[100];
	//UWORD winx,winy;
	//UWORD winxr=RunTime->scx[46]+25,winyr=36+RunTime->scy[24];
	UWORD ys1=2,		yh1=22+RunTime->scy[6]+RunTime->scy[12];					// colorscale height + axis height + <m>
	UWORD ys2=ys1+1+yh1,yh2=RunTime->scy[3]+3*RunTime->scy[11];						// text fields
	UWORD winh=ys2+2+yh2;
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime->scx[46]+25;											// gfx width + <time> + <maxscan>

	struct TagItem as_wintags[]={
	/* 00 */WA_Left,			0,
	/* 01 */WA_Top,				0,
	/* 02 */WA_InnerWidth,		0,
	/* 03 */WA_InnerHeight,		0,
	/* 04 */WA_IDCMP,			IDCMP_RAWKEY|IDCMP_VANILLAKEY|IDCMP_CLOSEWINDOW|IDCMP_INTUITICKS|IDCMP_MOUSEMOVE,
	/* 05 */WA_Flags,			WFLG_SMART_REFRESH|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_CLOSEGADGET|WFLG_DEPTHGADGET|WFLG_GIMMEZEROZERO|WFLG_REPORTMOUSE,
	/* 06 */WA_Gadgets,			0l,
	/* 07 */WA_Title,			0l,
	/* 08 */WA_ScreenTitle,		(ULONG)PRJ_VERS,
	/* 09 */WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
	/* 10 */WA_BlockPen,		1,
			TAG_DONE
	};
	struct Screen		*as_scr;
	struct Window		*as_win;
	struct RastPort		*as_rp;
	struct ViewPort 	*as_vp;
	struct ColorMap 	*as_cm;
	struct Gadget		*g;
	struct Gadget		*modGList=0l;
	struct Gadget		*modGadgets[LAST_GADIX2];
	struct NewGadget 	ng;
	struct IntuiMessage *imsg;
	ULONG iclass;
	USHORT icode;
	UBYTE runing=TRUE;
	float *rbuf1,*rbuf2,*rbuf3,*rbuf4;	/* Puffer für Spectrogammergebisse */
	float *rmaxp;						/* Puffer für MaxAmp Graph */
	register UWORD t;							/* Laufvariable Zeit */
	register UWORD n;							/* Laufvariable Bänder */
	UWORD m=instance->m;
	UBYTE cst,off;
	UWORD frqanz,tl;
	ULONG colr,colg,colb;
	double frq,frqstep,ystep;
	double step,xsc1,xsc2,fc;
	WORD cr,cg,cb,cs;
	UWORD as_xs,as_xe,as_ys,as_ys2,as_ys3,as_ye;/* Variablen für Darstellung */
	UWORD as_xp,as_yp;
	UWORD rml;
	double lifc;
	UBYTE *line,*pline;							/* for WritePixelArray8 (WriteChunkyPixels) */
    struct RastPort trp;
    struct BitMap *tbm;
	UBYTE cmap[256];							/* Colourmapping */
	WORD oldptr=0;
	char str[30];
	UBYTE cm=CROSSMODE_HARMONY,ocm=CROSSMODE_HARMONY;
	int nmx,nmy,omx=0,omy=0;					/* for mouse pointer cross */
	WORD hd=0;									/* harmony distance */
	BOOL cross=FALSE;
	UWORD xp,yp;
	double cfrq;
	BYTE okt,key;
	char key2letter[12]={'C','C','D','D','E','F','F','G','G','A','A','H' };
	char key2blawhi[12]={'-','#','-','#','-','-','#','-','#','-','#','-' };
	Param param;

	as_scr=LockPubScreen(SFX_PUBSCRNAME);
	as_vp=&as_scr->ViewPort;
	as_cm= as_vp ->ColorMap;
	UnlockPubScreen(SFX_PUBSCRNAME,0l);
	if(!instance->pal) {					/* Grauscala */
		xsc1=0.0;
		for(i=0;i<256;i++,xsc1++) { colg=MakeCol32((ULONG)xsc1);cmap[i]=ObtainBestPen(as_cm,colg,colg,colg,OBP_Precision,PRECISION_EXACT); }
	}
	else {
		cst=32;cs=8;off=0;
		cr=0;cg=0;cb=0;										/* 000 -> 00F */
		colr=MakeCol32((ULONG)cr);colg=MakeCol32((ULONG)cg);
		for(i=0;i<cst;i++,cb+=cs) cmap[off++]=ObtainBestPen(as_cm,colr,colg,MakeCol32((ULONG)cb),OBP_Precision,PRECISION_EXACT);
		cr=0;cg=0;cb=255;									/* 00F -> 0FF */
		colr=MakeCol32((ULONG)cr);colb=MakeCol32((ULONG)cb);
		for(i=0;i<cst;i++,cg+=cs) cmap[off++]=ObtainBestPen(as_cm,colr,MakeCol32((ULONG)cg),colb,OBP_Precision,PRECISION_EXACT);
		cr=0;cg=255,cb=255;cs=-cs;							/* 0FF -> 0F0 */
		colr=MakeCol32((ULONG)cr);colg=MakeCol32((ULONG)cg);
		for(i=0;i<cst;i++,cb+=cs) cmap[off++]=ObtainBestPen(as_cm,colr,colg,MakeCol32((ULONG)cb),OBP_Precision,PRECISION_EXACT);
		cr=0;cg=255;cb=0;cs=-cs;							/* 0F0 -> FF0 */
		colg=MakeCol32((ULONG)cg);colb=MakeCol32((ULONG)cb);
		for(i=0;i<cst;i++,cr+=cs) cmap[off++]=ObtainBestPen(as_cm,MakeCol32((ULONG)cr),colg,colb,OBP_Precision,PRECISION_EXACT);
		cr=255;cg=255;cb=0;cs=-cs;							/* FF0 -> F00 */
		colr=MakeCol32((ULONG)cr);colb=MakeCol32((ULONG)cb);
		for(i=0;i<cst;i++,cg+=cs) cmap[off++]=ObtainBestPen(as_cm,colr,MakeCol32((ULONG)cg),colb,OBP_Precision,PRECISION_EXACT);
		cr=255;cg=0;cb=0;cs=-cs;							/* F00 -> F0F */
		colr=MakeCol32((ULONG)cr);colg=MakeCol32((ULONG)cg);
		for(i=0;i<cst;i++,cb+=cs) cmap[off++]=ObtainBestPen(as_cm,colr,colg,MakeCol32((ULONG)cb),OBP_Precision,PRECISION_EXACT);
		cr=255;cg=0;cb=255;cs>>=1;		/* F0F -> FFF */
		colr=MakeCol32((ULONG)cr);colb=MakeCol32((ULONG)cb);
		for(i=0;i<(cst+cst);i++,cg+=cs) cmap[off++]=ObtainBestPen(as_cm,colr,MakeCol32((ULONG)cg),colb,OBP_Precision,PRECISION_EXACT);
	}

	rml=LINES_PREC(instance->ml);
	//-- adjust window/section widths and heights
	winw+=rml;as_xs=xs1+8+RunTime->scx[42];as_xe=as_xs+rml;
	if(instance->maxscan) { 				/* add extra space for max amplitudes */
		winw+=(ULONG)(MAXSCAN_FC+5);
		as_xe+=(ULONG)MAXSCAN_FC;
	}
	yh1+=m;ys2+=m;winh+=m;
	as_ys=ys1+22+m+RunTime->scy[6];as_ye=as_ys-m;

	if(g=CreateContext(&modGList)) {
		/** @TODO check if we have enough space to place them in one or two lines */
		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[2];
		ng.ng_Width			=RunTime->scx[100];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	="Time";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_Flags			=PLACETEXT_LEFT;
		ng.ng_GadgetID		=GADID_TIME;
		ng.ng_VisualInfo	=RunTime->VisualInfo;
		sprintf(param,"123 ms");
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
		modGadgets[GADIX_TIME]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[2+11];
		ng.ng_GadgetText	="Frq";
		ng.ng_GadgetID		=GADID_FRQ;
		sprintf(param,"3804 Hz");
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
		modGadgets[GADIX_FRQ]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[2+22];
		ng.ng_GadgetText	="Mag";
		ng.ng_GadgetID		=GADID_MAG;
		sprintf(param,"-30 db");
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
		modGadgets[GADIX_MAG]=g;

		if(g) {
			as_wintags[0].ti_Data=(RunTime->xres-winw)>>1;
			as_wintags[1].ti_Data=(RunTime->yres-winh)>>1;
			as_wintags[2].ti_Data=winw;
			as_wintags[3].ti_Data=winh;
			as_wintags[6].ti_Data=(ULONG)modGList;
			sprintf(title,PRJ_NAME" : %s",((struct Node *)srcbuf)->ln_Name);
			as_wintags[7].ti_Data=(ULONG)title;
			if(RunTime->sfxpubscr==IntuitionBase->FirstScreen) as_wintags[5].ti_Data|=WFLG_ACTIVATE;
			if(as_win=OpenWindowTagList(0l,as_wintags)) {
				GT_RefreshWindow(as_win,0l);
				as_rp=as_win->RPort;
				SetFont(as_rp,RunTime->scrfont);
				DrawGadWB(as_rp,xs1,ys1,winw-4,yh1);		// winx-12,winy-(RunTime->scy[6]+12)
				DrawGadWB(as_rp,xs1,ys2,winw-4,yh2);		// winx-12,winy-(RunTime->scy[6]+12)
				SetAPen(as_rp,1);RectFill(as_rp,xs1+2,ys1+2,winw-5,ys1+(yh1-3));	// winx-13,winy-(RunTime->scy[6]+13)
				SetABPenDrMd(as_rp,2,1,JAM1);

				Move(as_rp,as_xs+rml,as_ys+1);Draw(as_rp,as_xs-1,as_ys+1);					/* X-Achse */
				Move(as_rp,as_xs-1,as_ys+1);Draw(as_rp,as_xs-1,as_ys-m);					/* Y-Achse */
				if(instance->maxscan) {
					Move(as_rp,as_xs+rml,as_ys+1);
					Draw(as_rp,as_xs+rml+(ULONG)MAXSCAN_FC,as_ys+1);
					Move(as_rp,as_xs+rml,as_ys+1);
					Draw(as_rp,as_xs+rml,as_ys-m);
				}

				frqanz=(ULONG)((double)m/(double)RunTime->scy[6]);							/* X-Achsenunterteilung */
				frq=((double)srcbuf->srat)/2.0;
				frqstep=frq/(double)frqanz;
				ystep=(double)m/(double)frqanz;
				as_ys3=(as_ys-m);
				off=RunTime->scrfont->tf_Baseline>>1;
				for(i=0;i<frqanz;i++) {
					as_ys2=as_ys3+(i*ystep);
					Move(as_rp,as_xs-1,as_ys2);Draw(as_rp,as_xs-4,as_ys2);
					sprintf(param,"%5ld",(ULONG)frq);tl=TextLength(as_rp,param,5);
					Move(as_rp,as_xs-(5+tl),as_ys2+off);Text(as_rp,param,5);
					frq-=frqstep;
				}
				tl=TextLength(as_rp,"0",1);
				Move(as_rp,as_xs-(5+tl),as_ys+off);Text(as_rp,"0",1);

				Move(as_rp,(as_xs+rml)-RunTime->scx[24],as_ys+3+RunTime->scy[6]);Text(as_rp,"Time",4);
				Move(as_rp,as_xs-(RunTime->scx[42]+3),(as_ys-m)+3);Text(as_rp,"F",1);			/* Frequency */
				Move(as_rp,as_xs-(RunTime->scx[42]+3),(as_ys-m)+3+RunTime->scy[ 6]);Text(as_rp,"r",1);
				Move(as_rp,as_xs-(RunTime->scx[42]+3),(as_ys-m)+3+RunTime->scy[12]);Text(as_rp,"q",1);
				if(instance->maxscan) {
					fc=1.0;
					while(instance->rmaxa[m]<fc) fc/=2.0;
					fc*=2.0;
					sprintf(str,"%5.3lf Amp",fc);
					fc=MAXSCAN_FC/fc;
					Move(as_rp,(as_xs+(rml+(ULONG)MAXSCAN_FC)-RunTime->scx[54]),as_ys+3+RunTime->scy[6]);Text(as_rp,str,9);

					rmaxp=instance->rmaxa;														/* MaxAmplitude Graph */
					as_xp=as_xs+rml;as_yp=as_ys-1;
					Move(as_rp,as_xp+(ULONG)(fc*(*(rmaxp++))),as_yp--);
					for(n=1;n<m;n++) Draw(as_rp,as_xp+(ULONG)(fc*(*(rmaxp++))),as_yp--);
				}
				step=(double)rml/256.0;													/* Farbscala */
				xsc1=as_xs-2;
				for(i=0;i<255;i++) {
					xsc2=xsc1+step;if((UWORD)xsc1==(UWORD)xsc2) xsc2+=1.0;
					SetAPen(as_rp,cmap[255-i]);RectFill(as_rp,(UWORD)xsc1, 9,(UWORD)xsc2,16);
					SetAPen(as_rp,cmap[    i]);RectFill(as_rp,(UWORD)xsc1,10,(UWORD)xsc2,15);
					xsc1+=step;
				}
																						/* Skalenbeschriftung */
				Move(as_rp,as_xs-2,25);															Text(as_rp,"0 %",3);
				Move(as_rp,as_xs+(UWORD)((rml-RunTime->scx[12])*pow(0.5,instance->gamma)),25);	Text(as_rp,"50 %",4);
				Move(as_rp,as_xs+( rml-RunTime->scx[30]),25);									Text(as_rp,"100 %",5);


				as_xp=as_xs;as_yp=as_ys-m;												/* Spectrogram Graph */
				if((line=AllocVec(16384/*2048*/,MEMF_ANY|MEMF_CLEAR))) {
					if(tbm=AllocBitMap(1,instance->ml,8,BMF_CLEAR,NULL)) {		    /* Initialize the temporary bitmap */
						InitRastPort(&trp);
						trp.BitMap=tbm;
						trp.Layer=NULL;
						switch(instance->prec) {
							case PREC_HIGH2:	// extrapolation x 2
								rbuf1=instance->raster;
								rbuf2=&instance->raster[m];
								rbuf3=&instance->raster[m+m];
								rbuf4=&instance->raster[m+m+m];
								for(t=0;t<(instance->ml>>2);t++,as_xp++) {	/* for every timeslice */
									pline=&line[((m-1)<<4)];
									for(n=0;n<m;n++) { *pline=cmap[((UWORD)(*(rbuf1++)+*(rbuf2++)+*(rbuf3++)+*(rbuf4++)))>>2];pline-=16; }		/* for every band */
									//WriteChunkyPixels(as_rp,as_xp,as_yp,as_xp,as_ys,line,1);
									WritePixelArray8 (as_rp,as_xp,as_yp,as_xp,as_ys,line,&trp);
									rbuf1=&rbuf1[m+m+m];
									rbuf2=&rbuf2[m+m+m];
									rbuf3=&rbuf3[m+m+m];
									rbuf4=&rbuf4[m+m+m];
								}
								break;
							case PREC_HIGH1:	// extrapolation x 1
								rbuf1=instance->raster;
								rbuf2=&instance->raster[m];
								for(t=0;t<(instance->ml>>1);t++,as_xp++) {	/* for every timeslice */
									pline=&line[((m-1)<<4)];
									for(n=0;n<m;n++) { *pline=cmap[((UWORD)(*(rbuf1++)+*(rbuf2++)))>>1];pline-=16; }		/* for every band */
									//WriteChunkyPixels(as_rp,as_xp,as_yp,as_xp,as_ys,line,1);
									WritePixelArray8 (as_rp,as_xp,as_yp,as_xp,as_ys,line,&trp);
									rbuf1=&rbuf1[m];
									rbuf2=&rbuf2[m];
								}
								break;
							case PREC_NORM:		// normal
								rbuf1=instance->raster;
								for(t=0;t<instance->ml;t++,as_xp++) {		/* for every timeslice */
									pline=&line[((m-1)<<4)];
									for(n=0;n<m;n++) { *pline=cmap[(UBYTE)*(rbuf1++)];pline-=16; }		/* for every band */
									//WriteChunkyPixels(as_rp,as_xp,as_yp,as_xp,as_ys,line,1);
									WritePixelArray8 (as_rp,as_xp,as_yp,as_xp,as_ys,line,&trp);
								}
								break;
							case PREC_LOW1:		// interpolation x 1
								rbuf1=instance->raster;
								rbuf2=&instance->raster[m];
								for(t=0;t<instance->ml;t++,as_xp+=2) {		/* for every timeslice */
									pline=&line[((m-1)<<4)+1];
									for(n=0;n<m;n++) {												/* for every band */
										*(pline--)=cmap[(ULONG)(*(rbuf2++)+*rbuf1)>>1];
										* pline   =cmap[(ULONG) *(rbuf1++)];pline-=15;
									}
									//WriteChunkyPixels(as_rp,as_xp,as_yp,as_xp+1,as_ys,line,2);
									WritePixelArray8 (as_rp,as_xp,as_yp,as_xp+1,as_ys,line,&trp);
								}
								break;
							case PREC_LOW2:		// interpolation x 2
								rbuf1=instance->raster;
								rbuf2=&instance->raster[m];
								for(t=0;t<instance->ml;t++,as_xp+=4) {		/* for every timeslice */
									pline=&line[((m-1)<<4)+3];
									for(n=0;n<m;n++) {												/* for every band */
										*(pline--)=cmap[(ULONG)(* rbuf2   *0.75+*rbuf1*0.25)   ];
										*(pline--)=cmap[(ULONG)(* rbuf2        +*rbuf1     )>>1];
										*(pline--)=cmap[(ULONG)(*(rbuf2++)*0.25+*rbuf1*0.75)   ];
										* pline   =cmap[(ULONG) *(rbuf1++)];pline-=13;
									}
									//WriteChunkyPixels(as_rp,as_xp,as_yp,as_xp+3,as_ys,line,4);
									WritePixelArray8 (as_rp,as_xp,as_yp,as_xp+3,as_ys,line,&trp);
								}
								break;
						}
						FreeBitMap(tbm);
					}
					FreeVec(line);
				}
				else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
				SetDrMd(as_rp,COMPLEMENT);SetAPen(as_rp,0);
				lifc=(double)rml/(double)srcbuf->slen;
				while(runing) {
					if(RunTime->play && RunTime->plsi==instance->src.srcbuf) {
						if(oldptr>0) {
							Move(as_rp,oldptr,as_ys);Draw(as_rp,oldptr,as_ys-m);		/* Delete Pos */
						}
						oldptr=as_xs+(RunTime->plptr-rm_start)*lifc;
						Move(as_rp,oldptr,as_ys);Draw(as_rp,oldptr,as_ys-m);			/* Draw Pos */
						Delay(1);
					}
					else WaitPort(as_win->UserPort);
					while(imsg=(struct IntuiMessage *)GetMsg(as_win->UserPort)) {
						iclass	=imsg->Class;
						icode	=imsg->Code;
						nmx		=imsg->MouseX-as_win->BorderLeft;
						nmy		=imsg->MouseY-as_win->BorderTop;
						ReplyMsg(imsg);
						switch(iclass) {
							case IDCMP_MOUSEMOVE:
								if(cross) {														// delete
									Move(as_rp,as_xs,omy);Draw(as_rp,as_xe,omy);omy-=hd;			// 1st harmonic
									if((ocm==CROSSMODE_HARMONY) && (hd>10)) {
										while(omy>as_ye) {
											Move(as_rp,as_xs,omy);Draw(as_rp,as_xe,omy);omy-=hd;	// next harmonics
										}
									}
									Move(as_rp,omx,as_ys);Draw(as_rp,omx,as_ye);					// time
								}
								if((nmx>as_xs) && (nmx<as_xe) && (nmy>as_ye) && (nmy<as_ys)) {
									//-- get mouse pointer pos nd calc info fields
									xp=nmx-as_xs;yp=as_ys-nmy;
									sprintf(param,"%s",ToXUnit(srcbuf->unitx,(ULONG)((double)xp*((double)srcbuf->slen/(double)rml)),srcbuf->srat));
									GT_SetGadgetAttrs(modGadgets[GADIX_TIME],as_win,0l,GTTX_Text  ,param,GTTX_CopyText,TRUE,TAG_DONE);
									frq=(double)(yp*srcbuf->srat)/(double)(m+m);
									if((frq>55.0) && (frq<8000.0)) {
										okt=key=0;
										while(frq>(55.0*(1L<<okt))) okt++;
										okt--;cfrq=55.0*(1L<<okt);
										while(frq>cfrq) {
											key++;cfrq*=PITCH1;
										}
										key-=3;if(key<0) { key+=12;okt--; }
										sprintf(param,"%ld hz (%c%c%d)",(ULONG)frq,key2letter[key],key2blawhi[key],okt);
									}
									else {
										sprintf(param,"%ld hz",(ULONG)frq);
									}
									GT_SetGadgetAttrs(modGadgets[GADIX_FRQ ],as_win,0l,GTTX_Text  ,param,GTTX_CopyText,TRUE,TAG_DONE);
									sprintf(param,"%s",ToYUnit(srcbuf->unity,(32767.0*pow((instance->raster[(PREC_LINES(xp)*m)+yp]/255.0),1.0/instance->gamma))));
									GT_SetGadgetAttrs(modGadgets[GADIX_MAG ],as_win,0l,GTTX_Text  ,param,GTTX_CopyText,TRUE,TAG_DONE);

									//-- draw cross if desired
									if(cm!=CROSSMODE_NONE) {									// draw
										omx=nmx;omy=nmy;hd=as_ys-nmy;
										Move(as_rp,as_xs,nmy);Draw(as_rp,as_xe,nmy);nmy-=hd;			// 1st harmonic
										if((cm==CROSSMODE_HARMONY) && (hd>10)) {
											while(nmy>as_ye) {
												Move(as_rp,as_xs,nmy);Draw(as_rp,as_xe,nmy);nmy-=hd;	// next harmonics
											}
										}
										Move(as_rp,nmx,as_ys);Draw(as_rp,nmx,as_ye);		// time
										cross=TRUE;ocm=cm;
									}
									else cross=FALSE;
								}
								else cross=FALSE;
								break;
							case IDCMP_CLOSEWINDOW:
								runing=FALSE;break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									case 'c':
									case 'C':
										cm++;
										if(cm>CROSSMODE_MAX) cm=0;
										if(cross) {														// delete
											Move(as_rp,as_xs,omy);Draw(as_rp,as_xe,omy);omy-=hd;			// 1st harmonic
											if((ocm==CROSSMODE_HARMONY) && (hd>10)) {
												while(omy>as_ye) {
													Move(as_rp,as_xs,omy);Draw(as_rp,as_xe,omy);omy-=hd;	// next harmonics
												}
											}
											Move(as_rp,omx,as_ys);Draw(as_rp,omx,as_ye);					// time
										}
										if((cm!=CROSSMODE_NONE) && (nmx>as_xs) && (nmx<as_xe) && (nmy>as_ye) && (nmy<as_ys)) {	// draw
											omx=nmx;omy=nmy;hd=as_ys-nmy;
											Move(as_rp,as_xs,nmy);Draw(as_rp,as_xe,nmy);nmy-=hd;			// 1st harmonic
											if((cm==CROSSMODE_HARMONY) && (hd>10)) {
												while(nmy>as_ye) {
													Move(as_rp,as_xs,nmy);Draw(as_rp,as_xe,nmy);nmy-=hd;	// next harmonics
												}
											}
											Move(as_rp,nmx,as_ys);Draw(as_rp,nmx,as_ye);		// time
											cross=TRUE;ocm=cm;
										}
										else cross=FALSE;
										break;
									case KEY_ESC:
										runing=FALSE;
										break;
								}
								break;
						}
					}
				}
				CloseWindow(as_win);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		FreeGadgets(modGList);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	
	for(i=0;i<256;i++) ReleasePen(as_cm,cmap[i]);
}

//-- eof ----------------------------------------------------------------------
