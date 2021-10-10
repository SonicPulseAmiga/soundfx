/****h* Resample/lib-soundfx.c [0.0] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 0.0 (31.08.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Resample Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    31.Aug.2004    V 4.3   most recent version
*    31.Oct.2001	V 4.2	most recent version
*    14.Mar.2001	V 4.1	most recent version
*							F: several GUI- and Preset-Bugs
*							A: Lock-Parameter
*    31.Jul.2000	V 1.20	most recent version
*							F: abort-bug
*    18.Jul.1998	V 1.00	initial version
*  NOTES
*
*******
*/

#define RESAMPLE_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Resample.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

void RecalcModParams(struct Instance *instance);

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

#define GADID_FC		(GADID_SRC+SRC_IDCT)
#define GADIX_FC		(GADIX_SRC+SRC_IXCT)

#define GADID_LOCK		(GADID_FC+1)
#define GADIX_LOCK		(GADIX_FC+1)

#define GADID_SLENOLD	(GADID_LOCK+1)
#define GADIX_SLENOLD	(GADIX_LOCK+1)

#define GADID_SRATOLD	(GADID_SLENOLD+1)
#define GADIX_SRATOLD	(GADIX_SLENOLD+1)

#define GADID_SLENNEW	(GADID_SRATOLD+1)
#define GADIX_SLENNEW	(GADIX_SRATOLD+1)

#define GADID_SRATNEW	(GADID_SLENNEW+1)
#define GADIX_SRATNEW	(GADIX_SLENNEW+1)

#define GADID_SRATSEL	(GADID_SRATNEW+1)

#define GADID_INTER		(GADID_SRATSEL+1)
#define GADIX_INTER		(GADIX_SRATNEW+1)

#define GADID_AFLT		(GADID_INTER+INT_IDCT)
#define GADIX_AFLT		(GADIX_INTER+INT_IDCT)

#define GADID_ANZ		(GADID_AFLT+1)
#define GADIX_ANZ		(GADIX_AFLT+1)

#define GADID_WFKT		(GADID_ANZ+1)
#define GADIX_WFKT		(GADIX_ANZ+1)

#define LAST_GADIX		(GADIX_WFKT+WFKT_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[4];
};

#define LOCK_FC		0
#define LOCK_SRAT	1
#define LOCK_SLEN	2

Param oldslen,oldsrat;			// wegen RefreshGUI, RecalcModParams
Param newslen,newsrat;
ULONG slen,srat;

UBYTE *LockLabels[]={ "Factor","SRat","SLen",0l };

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
		SFXMod_ModDescInit(1,PD_INTERPOLATOR,"I1",&(instance->int1));
		SFXMod_ModDescInit(2,PD_WINFUNCTION ,"W1",&(instance->wfkt1));
		SFXMod_ModDescInit(3,PD_END         ,0l  ,0l);	
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

	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(4);
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(2);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

	UBYTE gh;
	Param param;

	if(instance->src.srcbuf) {
		sprintf(oldslen,"%ld sv",instance->src.srcbuf->slen);
		sprintf(oldsrat,"%ld Hz",instance->src.srcbuf->srat);
	}
	else {
		sprintf(oldslen,"0 sv");
		sprintf(oldsrat,"0 Hz");
	}

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[104];
		ng.ng_TopEdge		=ys4+RunTime->scy[2];
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

		ng.ng_LeftEdge		=xs1+RunTime->scx[64];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="SLen old";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_SLENOLD;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,oldslen,GTTX_CopyText,FALSE,TAG_DONE);
		modGadgets[GADIX_SLENOLD]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="new";
		ng.ng_GadgetID		=GADID_SLENNEW;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,newslen,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_SLENNEW]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[64];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="SRat old";
		ng.ng_GadgetID		=GADID_SRATOLD;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,oldsrat,GTTX_CopyText,FALSE,TAG_DONE);
		modGadgets[GADIX_SRATOLD]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_SRATSEL;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[230];
		ng.ng_Width			=RunTime->scx[74];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_SRATNEW;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,newsrat,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_SRATNEW]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[64];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="Factor";
		ng.ng_GadgetID		=GADID_FC;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_factor,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FC]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="Lock";
		ng.ng_GadgetID		=GADID_LOCK;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,LockLabels,GTCY_Active,instance->lock,TAG_DONE);
		modGadgets[GADIX_LOCK]=g;

		g=AddInterpolator(&(instance->int1),modGadgets,g,GADID_INTER,GADIX_INTER,xs1,ys2+RunTime->scy[FRMCONTENT_YOFFSET+36]);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[26];
		ng.ng_GadgetText	="Enable?";
		ng.ng_GadgetID		=GADID_AFLT;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,instance->aliasflt,GTCB_Scaled,TRUE,TAG_DONE);
		modGadgets[GADIX_AFLT]=g;

		g=AddWinFunction(&(instance->wfkt1),modGadgets,g,GADID_WFKT,GADIX_WFKT,xs1,ys3+RunTime->scy[FRMCONTENT_YOFFSET+12]);

		ng.ng_LeftEdge		=xs1+RunTime->scx[254];
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime->scx[50];
		ng.ng_GadgetText	="Num";
		ng.ng_GadgetID		=GADID_ANZ;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_anz,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_ANZ]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[ 29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"new",xs1+RunTime->scx[206],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Window",xs1+RunTime->scx[ 45],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Interp.",xs1+RunTime->scx[ 45],ys2+RunTime->scy[FRMTEXT_YPOS(4)]);

			DrawTitle(modrp,"Source"			,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"		,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Aliasing Filter"	,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"			,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(4));				/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,ANYFRM_HEIGHT(2));				/* Par */
			DrawGadWB(modrp,xs1,ys4,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel      (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			//SetSource      (&(instance->src ),modWin,modGadgets,GADIX_SRC);
			//SetInterpolator(&(instance->int1),modWin,modGadgets,GADIX_INTER);
			//SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);
			RefreshGUI(instance,modGadgets,modWin);
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
						SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
						RefreshGUI(instance,modGadgets,modWin);
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
								gh=HandleSource(&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode);
								if(gh && gnr==GADID_SRC) {		// SRCSEL=GADID_SRC+0
									RefreshGUI(instance,modGadgets,modWin);
									if(UC_GetParameterUnit(newslen,UGRP_ABSTIME)==ATME_SAMPLES) {
										sprintf(oldslen,"%ld sv",(ULONG)(instance->src.srcbuf->slen));
									}
									else {
										sprintf(param,"%lf ms",(ULONG)(instance->src.srcbuf->slen)*(1000.0/(double)instance->src.srcbuf->srat));
										UC_ConvertParameter(oldslen,param,UGRP_ABSTIME,UC_GetParameterUnit(newslen,UGRP_ABSTIME));
									}
									sprintf(param,"%ld Hz",(ULONG)(instance->src.srcbuf->srat));
									UC_ConvertParameter(oldsrat,param,UGRP_ABSFREQUENCY,UC_GetParameterUnit(newsrat,UGRP_ABSFREQUENCY));
									GT_SetGadgetAttrs(modGadgets[GADIX_SLENOLD],modWin,0l,GTTX_Text,oldslen,GTTX_CopyText,FALSE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_SRATOLD],modWin,0l,GTTX_Text,oldsrat,GTTX_CopyText,FALSE,TAG_DONE);
								}
								if(!gh) gh=HandleInterpolator(&(instance->int1 ),modWin,modGadgets,GADID_INTER,GADIX_INTER,gnr,icode);
								if(!gh) gh=HandleWinFunction (&(instance->wfkt1),modWin,modGadgets,GADID_WFKT, GADIX_WFKT, gnr,icode);
								if(!gh) gh=HandleCfgSel      (&cfgsel        ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic);
								if(!gh)	{
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_SLENNEW:
											HandleParameter(newslen,modWin,modGadgets[GADIX_SLENNEW]);
											if(UC_GetParameterUnit(newslen,UGRP_ABSTIME)==ATME_SAMPLES) {
												slen=atof(newslen);
											}
											else {
												UC_ConvertParameter(param,newslen,UGRP_ABSTIME,ATME_MS);
												slen=atof(param)*((double)instance->src.srcbuf->srat/1000.0);
											}
											instance->factor=(double)slen/(double)instance->src.srcbuf->slen;
											if(instance->factor>1.0) {
												instance->aliasflt=FALSE;
												GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,TRUE,TAG_DONE);
											}
											else GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,FALSE,TAG_DONE);
											sprintf(param,"%lf",instance->factor);
											UC_ConvertParameter(instance->p_factor,param,UGRP_PROPOTION,UC_GetParameterUnit(instance->p_factor,UGRP_PROPOTION));
											sprintf(param,"%ld Hz",(ULONG)(instance->src.srcbuf->srat*instance->factor));
											UC_ConvertParameter(newsrat,param,UGRP_ABSFREQUENCY,UC_GetParameterUnit(newsrat,UGRP_ABSFREQUENCY));
											GT_SetGadgetAttrs(modGadgets[GADIX_FC],modWin,0l,GTST_String,instance->p_factor,TAG_DONE);
											GT_SetGadgetAttrs(modGadgets[GADIX_SRATNEW],modWin,0l,GTST_String,newsrat,TAG_DONE);
											break;
										case GADID_SRATNEW:
											HandleParameter(newsrat,modWin,modGadgets[GADIX_SRATNEW]);
											UC_ConvertParameter(param,newsrat,UGRP_ABSFREQUENCY,AFRQ_HZ);
											srat=atof(param);
											instance->factor=(double)srat/(double)instance->src.srcbuf->srat;
											if(instance->factor>1.0) {
												instance->aliasflt=FALSE;
												GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,TRUE,TAG_DONE);
											}
											else GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,FALSE,TAG_DONE);
											sprintf(param,"%lf",instance->factor);
											UC_ConvertParameter(instance->p_factor,param,UGRP_PROPOTION,UC_GetParameterUnit(instance->p_factor,UGRP_PROPOTION));
											if(UC_GetParameterUnit(newslen,UGRP_ABSTIME)==ATME_SAMPLES) {
												sprintf(newslen,"%ld sv",(ULONG)(instance->src.srcbuf->slen*instance->factor));
											}
											else {
												sprintf(param,"%lf ms",(ULONG)(instance->src.srcbuf->slen*instance->factor)*(1000.0/(double)instance->src.srcbuf->srat));
												UC_ConvertParameter(newslen,param,UGRP_ABSTIME,UC_GetParameterUnit(newslen,UGRP_ABSTIME));
											}
											GT_SetGadgetAttrs(modGadgets[GADIX_FC],modWin,0l,GTST_String,instance->p_factor,TAG_DONE);
											GT_SetGadgetAttrs(modGadgets[GADIX_SLENNEW],modWin,0l,GTST_String,newslen,TAG_DONE);
											break;
										case GADID_FC:
											HandleParameter(instance->p_factor,modWin,modGadgets[GADIX_FC]);
											UC_ConvertParameter(param,instance->p_factor,UGRP_PROPOTION,PROP_FC);
											instance->factor=atof(param);
											if(instance->factor>1.0) {
												instance->aliasflt=FALSE;
												GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,TRUE,TAG_DONE);
											}
											else GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,FALSE,TAG_DONE);
											if(UC_GetParameterUnit(newslen,UGRP_ABSTIME)==ATME_SAMPLES) {
												sprintf(newslen,"%ld sv",(ULONG)(instance->src.srcbuf->slen*instance->factor));
											}
											else {
												sprintf(param,"%lf ms",(ULONG)(instance->src.srcbuf->slen*instance->factor)*(1000.0/(double)instance->src.srcbuf->srat));
												UC_ConvertParameter(newslen,param,UGRP_ABSTIME,UC_GetParameterUnit(newslen,UGRP_ABSTIME));
											}
											sprintf(param,"%ld Hz",(ULONG)(instance->src.srcbuf->srat*instance->factor));
											UC_ConvertParameter(newsrat,param,UGRP_ABSFREQUENCY,UC_GetParameterUnit(newsrat,UGRP_ABSFREQUENCY));
											GT_SetGadgetAttrs(modGadgets[GADIX_SLENNEW],modWin,0l,GTST_String,newslen,TAG_DONE);
											GT_SetGadgetAttrs(modGadgets[GADIX_SRATNEW],modWin,0l,GTST_String,newsrat,TAG_DONE);
											break;
										case GADID_SRATSEL:
											if(srat=GetPeriode((ULONG)atoi(newsrat),0l)) {
												instance->factor=(double)srat/(double)instance->src.srcbuf->srat;
												if(instance->factor>1.0) {
													instance->aliasflt=FALSE;
													GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,TRUE,TAG_DONE);
												}
												else GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,FALSE,TAG_DONE);
												sprintf(param,"%lf",instance->factor);
												UC_ConvertParameter(instance->p_factor,param,UGRP_PROPOTION,UC_GetParameterUnit(instance->p_factor,UGRP_PROPOTION));
												if(UC_GetParameterUnit(newslen,UGRP_ABSTIME)==ATME_SAMPLES) {
													sprintf(newslen,"%ld sv",(ULONG)(instance->src.srcbuf->slen*instance->factor));
												}
												else {
													sprintf(param,"%lf ms",(ULONG)(instance->src.srcbuf->slen*instance->factor)*(1000.0/(double)instance->src.srcbuf->srat));
													UC_ConvertParameter(newslen,param,UGRP_ABSTIME,UC_GetParameterUnit(newslen,UGRP_ABSTIME));
												}
												sprintf(param,"%ld Hz",(ULONG)(instance->src.srcbuf->srat*instance->factor));
												UC_ConvertParameter(newsrat,param,UGRP_ABSFREQUENCY,UC_GetParameterUnit(newsrat,UGRP_ABSFREQUENCY));
												GT_SetGadgetAttrs(modGadgets[GADIX_FC],modWin,0l,GTST_String,instance->p_factor,TAG_DONE);
												GT_SetGadgetAttrs(modGadgets[GADIX_SLENNEW],modWin,0l,GTST_String,newslen,TAG_DONE);
												GT_SetGadgetAttrs(modGadgets[GADIX_SRATNEW],modWin,0l,GTST_String,newsrat,TAG_DONE);
											}
											break;
										case GADID_LOCK:
											instance->lock=icode;
											break;
										case GADID_ANZ:
											HandleParameter(instance->p_anz,modWin,modGadgets[GADIX_ANZ]);
											break;
										case GADID_AFLT:
											instance->aliasflt=modGadgets[GADIX_AFLT]->Flags&GFLG_SELECTED;;
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_FC],modWin,0l);
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
	UBYTE l;
	//-- prowin
	PWData *pwd;
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
	RecalcModParams(instance);

	srcbuf=instance->src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);
	instance->slen=rm_len;
	instance->srat=srcbuf->srat;

	UC_ConvertParameter(param,instance->p_factor,UGRP_PROPOTION,PROP_FC);
	instance->factor=atof(param);

	if(instance->aliasflt) {
		UC_ConvertParameter(param,instance->p_anz,UGRP_COUNT,COUN_ABS);
		instance->anz=atoi(param);
	}

	if(!dstbuf) {
		if(dstbuf=NewSample()) {
			BufferFill(srcbuf,dstbuf);
			dstbuf->slen=srcbuf->slen*instance->factor;
			dstbuf->srat=srcbuf->srat*instance->factor;
			dstbuf->zoomxl*=instance->factor;
			dstbuf->zoomxs*=instance->factor;
			if(dstbuf->mark) {
				dstbuf->markxs*=instance->factor;
				dstbuf->markxl*=instance->factor;
			}
			if(dstbuf->loop) {
				dstbuf->loops*=instance->factor;
				dstbuf->loopl*=instance->factor;
			}
			if(AllocSampleMem(dstbuf,dstbuf->slen,CH_COUNT(srcbuf->channels))) {
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
					srcsa=SAInit(srcbuf,l,0,srcbuf->slen,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					dstsa=SAInit(dstbuf,l,0,dstbuf->slen,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					if(srcsa && dstsa) {
						SARead =srcsa->safunc;
						SAWrite=dstsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
						instance->dbuf=(SAMPLE *)dstsa->buffer1;						// Pointer auf dst-Block
						if(instance->aliasflt) {									// mit Aliasingfilter
							for(srcsa->seg=dstsa->seg=0;((srcsa->seg<=srcsa->seganz) && (instance->curlen<instance->slen));) {
								if(instance->spos==0) {
									instance->sbuf=(SAMPLE *)srcsa->buffer1;			// Pointer auf src-Block holen
									instance->spos=blen;
									SARead(srcsa,srcsa->blen);srcsa->seg++;				// nächstes Read starten
								}
								process_flt((SFXCoreInstance *)instance,blen);
								if(instance->dpos==blen) {
									SAWrite(dstsa,dstsa->blen);dstsa->seg++;			// akt Block schreiben
									instance->dbuf=(SAMPLE *)dstsa->buffer1;			// Pointer auf dst-Block
									instance->dpos=0;
								}
							}
						}
						else {														// ohne Aliasingfilter
							for(srcsa->seg=dstsa->seg=0;((srcsa->seg<=srcsa->seganz) && (instance->curlen<instance->slen));) {
								if(instance->spos==0) {
									instance->sbuf=(SAMPLE *)srcsa->buffer1;			// Pointer auf src-Block holen
									instance->spos=blen;
									SARead(srcsa,srcsa->blen);srcsa->seg++;				// nächstes Read starten
								}
								process_noflt((SFXCoreInstance *)instance,blen);
								if(instance->dpos==blen) {
									SAWrite(dstsa,dstsa->blen);dstsa->seg++;			// akt Block schreiben
									instance->dbuf=(SAMPLE *)dstsa->buffer1;			// Pointer auf dst-Block
									instance->dpos=0;
								}
							}
						}
						if(!ProWinAborted(instance->curlen)) {											// process rest (because of irange delay)
							process_rest((SFXCoreInstance *)instance,blen);
							if(instance->dpos) { 
								SAWrite(dstsa,dstsa->blen);dstsa->seg++;				// akt Block schreiben
								instance->dbuf=(SAMPLE *)dstsa->buffer1;			// Pointer auf dst-Block
								instance->dpos=0;
							}
						}
					}
					else okay=FALSE;
					if(srcsa) SADone(srcsa,srcbuf);
					if(dstsa) SADone(dstsa,dstbuf);
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
		return(FALSE);
	}
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;	pf=ParseWinFunctionSet (&(instance->wfkt1),"W1",params);
		if(!pf) pf=ParseInterpolatorSet(&(instance->int1), "I1",params);
		if(!pf && !stricmp(RexxPar2,"Factor"))		{ strntcpy(instance->p_factor,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(instance->p_anz   ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"AliasFlt"))	{ instance->aliasflt=atoi(RexxPar3)%2;pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseWinFunctionGet (&(instance->wfkt1),"W1",params,ret);
		if(!pf) pf=ParseInterpolatorGet(&(instance->int1), "I1",params,ret);
		if(!pf && !stricmp(RexxPar2,"Factor"))		{ strntcpy(ret,instance->p_factor,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(ret,instance->p_anz   ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"AliasFlt"))	{ sprintf(ret,"%d",instance->aliasflt);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	instance->lock	=CfgIO_MatchString(
					 CfgIO_ReadString		(cfg,"General"	,"Lock"		,LockLabels[LOCK_FC]),LockLabels);
	switch(instance->lock) {
		case LOCK_FC:	strcpy(instance->p_factor,CfgIO_ReadString	(cfg,"General"	,"Factor"	,instance->p_factor));break;
		case LOCK_SRAT:	strcpy(newsrat        ,CfgIO_ReadString	(cfg,"General"	,"SRat"		,newsrat));break;
		case LOCK_SLEN:	strcpy(newslen        ,CfgIO_ReadString	(cfg,"General"	,"SLen"		,newslen));break;
	}
	strcpy(instance->p_anz   ,CfgIO_ReadString	(cfg,"General"	,"Length"	,instance->p_anz));
	instance->aliasflt=	   CfgIO_ReadBool	(cfg,"General"	,"AliasFlt"	,instance->aliasflt);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Lock"		,LockLabels[instance->lock]);
	switch(instance->lock) {
		case LOCK_FC:	CfgIO_WriteString		  (cfg,"General"	,"Factor"	,instance->p_factor);break;
		case LOCK_SRAT: CfgIO_WriteString		  (cfg,"General"	,"SRat"		,newsrat);break;
		case LOCK_SLEN: CfgIO_WriteString		  (cfg,"General"	,"SLen"		,newslen);break;
	}
	CfgIO_WriteString		  (cfg,"General"	,"Length"	,instance->p_anz);
	CfgIO_WriteBool			  (cfg,"General"	,"AliasFlt"	,instance->aliasflt);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->int1.range=2.0;
	instance->int1.type=INT_TYPE_LIN;
	instance->int1.desc[0]='\0';

	instance->wfkt1.win=WIN_HAMMING;
	instance->wfkt1.par=0.54;
	instance->wfkt1.desc[0]='\0';

	UC_ConvertParameter(instance->p_factor,"1.0",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(newsrat,"44100 Hz"   ,UGRP_ABSFREQUENCY,AFRQ_HZ);
	UC_ConvertParameter(newslen,"100 sv"     ,UGRP_ABSTIME,ATME_SAMPLES);
	
	UC_ConvertParameter(instance->p_anz,"64",UGRP_COUNT,COUN_ABS);
	instance->aliasflt=0;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	Param param;

	UC_ConvertParameter(param,instance->p_factor,UGRP_PROPOTION,PROP_FC);
	instance->factor=atof(param);
	MSG1("factor = %lf",instance->factor);

	SetSource      (&(instance->src),  modWin,modGadgets,GADIX_SRC);
	SetInterpolator(&(instance->int1), modWin,modGadgets,GADIX_INTER);
	SetWinFunction (&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);
	GT_SetGadgetAttrs(modGadgets[GADIX_ANZ ],modWin,0l,GTST_String ,instance->p_anz,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_LOCK],modWin,0l,GTCY_Active ,instance->lock,TAG_DONE);

	RecalcModParams(instance);
	GT_SetGadgetAttrs(modGadgets[GADIX_FC  ],modWin,0l,GTST_String ,instance->p_factor,TAG_DONE);
	if(instance->factor<1.0) GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,FALSE,TAG_DONE);
	else					 GT_SetGadgetAttrs(modGadgets[GADIX_AFLT],modWin,0l,GTCB_Checked,instance->aliasflt,GA_Disabled,TRUE,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_SLENNEW],modWin,0l,GTST_String,newslen,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_SRATNEW],modWin,0l,GTST_String,newsrat,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

void RecalcModParams(struct Instance *instance) {
	Param param;

	if(instance->src.srcbuf) {
		switch(instance->lock) {
			case LOCK_FC:
				//MSG("LOCK_FC");
				UC_ConvertParameter(param,instance->p_factor,UGRP_PROPOTION,PROP_FC);instance->factor=atof(param);
				sprintf(newslen,"%ld sv",(ULONG)(instance->src.srcbuf->slen*instance->factor));
				sprintf(newsrat,"%ld Hz",(ULONG)(instance->src.srcbuf->srat*instance->factor));
				break;
			case LOCK_SRAT:
				//MSG("LOCK_SRAT");
				UC_ConvertParameter(param,newsrat  ,UGRP_ABSFREQUENCY,AFRQ_HZ);srat=atof(param);
				instance->factor=(double)srat/(double)instance->src.srcbuf->srat;
				if(instance->factor>1.0) instance->aliasflt=FALSE;
				sprintf(instance->p_factor,"%lf",instance->factor);
				sprintf(newslen,"%ld sv",(ULONG)(instance->src.srcbuf->slen*instance->factor));				
				break;
			case LOCK_SLEN:
				//MSG("LOCK_SLEN");
				if(UC_GetParameterUnit(newslen,UGRP_ABSTIME)==ATME_SAMPLES) {
					slen=atof(newslen);
				}
				else {
					UC_ConvertParameter(param,newslen,UGRP_ABSTIME,ATME_MS);
					slen=atof(param)*((double)instance->src.srcbuf->srat/1000.0);
				}
				instance->factor=(double)slen/(double)instance->src.srcbuf->slen;
				if(instance->factor>1.0) instance->aliasflt=FALSE;
				sprintf(instance->p_factor,"%lf",instance->factor);
				sprintf(newsrat,"%ld Hz",(ULONG)(instance->src.srcbuf->srat*instance->factor));
				break;
		}
	}
	else {
		switch(instance->lock) {
			case LOCK_FC:
				UC_ConvertParameter(param,instance->p_factor,UGRP_PROPOTION,PROP_FC);instance->factor=atof(param);
				break;
			case LOCK_SRAT:
			case LOCK_SLEN:
				instance->factor=1.0;
				sprintf(instance->p_factor,"1.0");
				break;
		}
		sprintf(newslen,"0 sv");
		sprintf(newsrat,"0 Hz");
	}
	MSG1("factor=%lf",instance->factor);
}

//-- eof ----------------------------------------------------------------------
