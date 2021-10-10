/****h* SampleSplit/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (18.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    SampleSplit Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    18.Apr.2002	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    04.Jul.1999	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*  IDEAS
*******
*/

#define SAMPLESPLIT_SOUNDFX_C

//-- includes -----------------------------------------------------------------


#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "SampleSplit.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

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

#define GADID_SPLITPOS	(GADID_SRC+SRC_IDCT)
#define GADIX_SPLITPOS	(GADIX_SRC+SRC_IXCT)

#define GADID_GRABMARK	(GADID_SPLITPOS+1)
#define GADIX_GRABMARK	(GADIX_SPLITPOS+1)

#define GADID_SPLITS	(GADID_GRABMARK+1)
#define GADIX_SPLITS	(GADIX_GRABMARK+1)

#define LAST_GADIX		(GADIX_SPLITS+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[2];
};

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
		SFXMod_ModDescInit(1,PD_END         ,0l  ,0l);	
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
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(2);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

	UBYTE srctyp;
	Param param;

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
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	="Pos";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_SPLITPOS;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_splitpos,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_SPLITPOS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[150];
		ng.ng_Width			=RunTime->scx[100];
		ng.ng_GadgetText	="Grab Mark";
		ng.ng_GadgetID		=GADID_GRABMARK;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_GRABMARK]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Splits";
		ng.ng_GadgetID		=GADID_SPLITS;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_splits,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_SPLITS]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Splits"		,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(2));				/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);

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
								if(!HandleSource(&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)
								&& !HandleCfgSel   (&cfgsel      ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_SPLITPOS:
											HandleParameter(instance->p_splitpos,modWin,modGadgets[GADIX_SPLITPOS]);
											break;
										case GADID_GRABMARK:
											if(instance->src.srcbuf->mark) {
												if((srctyp=UC_GetParameterUnit(instance->p_splitpos,UGRP_ABSTIME))!=INVALID_UNIT) {
													if(srctyp==ATME_SAMPLES) {
														sprintf(param,"%ld",instance->src.srcbuf->markxs);
														UC_ConvertParameter(instance->p_splitpos,param,UGRP_ABSTIME,ATME_SAMPLES);
													}
													else {
														sprintf(param,"%lf ms",(double)instance->src.srcbuf->markxs/((double)instance->src.srcbuf->srat/1000.0));
														UC_ConvertParameter(param,instance->p_splitpos,UGRP_ABSTIME,srctyp);
													}

												}
												else {
													if((srctyp=UC_GetParameterUnit(instance->p_splitpos,UGRP_RELTIME))!=INVALID_UNIT) {
														sprintf(param,"%lf",(double)instance->src.srcbuf->markxs/((double)instance->src.srcbuf->slen));
														UC_ConvertParameter(instance->p_splitpos,param,UGRP_RELTIME,srctyp);
													}
												}
												GT_SetGadgetAttrs(modGadgets[GADIX_SPLITPOS],modWin,0l,GTST_String,instance->p_splitpos,TAG_DONE);
											}
											break;
										case GADID_SPLITS:
											HandleParameter(instance->p_splits,modWin,modGadgets[GADIX_SPLITS]);
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_SPLITPOS],modWin,0l);
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
	UWORD j;
	UBYTE l;
	//-- prowin
	PWData *pwd;
	ProWinTitle title;
	//-- procesing length
	//-- src & dst buffers
	SInfo *dstbuf,*srcbuf,*dstbufs[100]={NULL};
	//-- administrative
	Param param;
	UBYTE pu;
	UBYTE newdst=FALSE;
	UBYTE okay=TRUE;
	ULONG len;

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
			/** @TODO use this to return results (array handles max 10 results right now) */
			//dstbuf=pdata->dst;
			break;
		case CM_PREHEAR:
			break;
	}

	srcbuf=instance->src.srcbuf;
	instance->slen=srcbuf->slen;				// the whole length
	instance->srat=srcbuf->srat;

	if((pu=UC_GetParameterUnit(instance->p_splitpos,UGRP_ABSTIME))!=INVALID_UNIT) {
		if(pu==ATME_SAMPLES) instance->splitpos=atof(instance->p_splitpos);
		else { UC_ConvertParameter(param,instance->p_splitpos,UGRP_ABSTIME,ATME_MS);instance->splitpos=atof(param)*((double)srcbuf->srat/1000.0); }
	}
	else { UC_ConvertParameter(param,instance->p_splitpos,UGRP_RELTIME,RTME_FC);instance->splitpos=(double)srcbuf->slen*atof(param); }
	if(instance->splitpos<0) instance->splitpos=0;

	UC_ConvertParameter(param,instance->p_splits,UGRP_COUNT,COUN_ABS);
	instance->splits=atoi(param);
	if(instance->splits>98) instance->splits=98;
	if(((instance->splits+1)*instance->splitpos)>instance->slen) instance->splits=(instance->slen/instance->splitpos)-1;

	for(j=0,len=instance->slen;j<instance->splits;j++) {
		if(!(dstbufs[j]=NewSample())) { okay=FALSE;break; }
		else {
			dstbuf=dstbufs[j];
			BufferFill(srcbuf,dstbuf);
			dstbuf->slen=instance->splitpos;len-=instance->splitpos;
			dstbuf->zoomxl=dstbuf->slen;
			if(!AllocSampleMem(dstbuf,dstbuf->slen,CH_COUNT(dstbuf->channels))) {
				okay=FALSE;
				break;
			}
		}
	}
	if(okay) {
		if(!(dstbufs[instance->splits]=NewSample())) okay=FALSE;
		else {
			dstbuf=dstbufs[instance->splits];
			BufferFill(srcbuf,dstbuf);
			dstbuf->slen=len;
			dstbuf->zoomxl=dstbuf->slen;
			if(!AllocSampleMem(dstbuf,dstbuf->slen,CH_COUNT(dstbuf->channels))) okay=FALSE;
		}
	}
	if(okay) {
		/** @todo support passing destbuffers via pdata->dst[] */
		newdst=TRUE;
		if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
			if(activate((SFXCoreInstance *)instance)) {
				for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
					activate_ch((SFXCoreInstance *)instance,l);
					sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));
					StartProWin(pwd,(ULONG *)(&instance->curlen),title,srcbuf->slen);					// den Statusbalken starten
					for(j=0;((j<=instance->splits) && (instance->curlen<instance->slen));j++) {										// for each split
						dstbuf=dstbufs[j];
						CopyDataCh(srcbuf,dstbuf,l,instance->curlen,0,dstbuf->slen);
						if(!ProWinAborted(instance->curlen)) instance->curlen+=dstbuf->slen;
					}
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }										  // es wurde abgebrochen
				}
				if(okay && newdst) {
					SampleName name;
					for(j=0;j<=instance->splits;j++) {										// for each split
						if(instance->splits<10) sprintf(name,"%s.%1d",((struct Node *)srcbuf)->ln_Name,j);
						else sprintf(name,"%s.%02d",((struct Node *)srcbuf)->ln_Name,j);
						AddSample(dstbufs[j],name);
					}
				}
			}
			else okay=FALSE;
			deactivate((SFXCoreInstance *)instance);
			DelProWin(pwd);
		}
		else okay=FALSE;
	}
	else okay=FALSE;
	if(!okay) {
		for(j=0;j<=instance->splits;j++) if(dstbufs[j]) RemSample(dstbufs[j]);
		return(FALSE);
	}
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)){
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"SplitPos"))	{ strntcpy(instance->p_splitpos,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Splits"))		{ strntcpy(instance->p_splits  ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"SplitPos"))	{ strntcpy(ret,instance->p_splitpos,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Splits"))		{ strntcpy(ret,instance->p_splits  ,PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_splitpos,CfgIO_ReadString	(cfg,"General"	,"SplitPos"	,"50 %"));
	strcpy(instance->p_splits  ,CfgIO_ReadString	(cfg,"General"	,"Splits"	,"1"));
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"SplitPos"	,instance->p_splitpos);
	CfgIO_WriteString		  (cfg,"General"	,"Splits"	,instance->p_splits);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->p_splitpos,"50 %",UGRP_RELTIME,RTME_PROZ);
	UC_ConvertParameter(instance->p_splits  ,"1"   ,UGRP_COUNT,COUN_ABS);
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_SPLITPOS],modWin,0l,GTST_String,instance->p_splitpos,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_SPLITS  ],modWin,0l,GTST_String,instance->p_splits  ,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
