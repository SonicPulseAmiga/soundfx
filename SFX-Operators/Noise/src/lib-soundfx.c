/****h* Noise/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (31.10.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Noise Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    31.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define NOISE_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasSource

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Noise.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

#ifdef DEBUG
void DumpFXParams(void);
#endif

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

#define GADID_MOD1		(GADID_CFGSEL+CFGSEL_IDCT)
#define GADIX_MOD1		(GADIX_CFGSEL+CFGSEL_IXCT)

#define GADID_MOD2		(GADID_MOD1+MOD_IDCT)
#define GADIX_MOD2		(GADIX_MOD1+MOD_IXCT)

#define GADID_RATE		(GADID_MOD2+MOD_IDCT)
#define GADIX_RATE		(GADIX_MOD2+MOD_IXCT)

#define GADID_RATESEL	(GADID_RATE+1)

#define GADID_LENGTH	(GADID_RATESEL+1)
#define GADIX_LENGTH	(GADIX_RATE+1)

#define LAST_GADIX		(GADIX_LENGTH+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[4];
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
		SFXMod_ModDescInit(0,PD_COREINSTANCE,0l  , instance);
		SFXMod_ModDescInit(1,PD_MODULATOR   ,"M1",&(instance->mod1));
		SFXMod_ModDescInit(2,PD_MODULATOR   ,"M2",&(instance->mod2));
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
	UWORD ys2=ys1+1+MODSEL_HEIGHT;
	UWORD ys3=ys2+1+MODSEL_HEIGHT;
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

	ULONG slen,srat;
	LONG rate;
	Param param;
	Source			dummysrc;
	SInfo			dummysi={0};

	dummysrc.srcbuf=&dummysi;
	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	dummysi.srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) dummysi.slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);dummysi.slen=atof(param)*((double)dummysi.srat/1000.0); }
	if(dummysi.slen<2) dummysi.slen=2;

	SFXMod_OpenGUILibs;
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

		g=AddModulator(&(instance->mod1),modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys1);
		g=AddModulator(&(instance->mod2),modGadgets,g,GADID_MOD2,GADIX_MOD2,xs1,ys2);

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];				/* Rate */
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_RATESEL;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_RATE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_srat,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_RATE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="Length";
		ng.ng_GadgetID		=GADID_LENGTH;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_slen,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_LENGTH]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Rate",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Minimum Change",xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Maximum Change",xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"	,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,MODSEL_HEIGHT);					/* Par */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,ANYFRM_HEIGHT(1));				/* Par */
			DrawGadWB(modrp,xs1,ys4,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
			SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);

			while(runing) {
				WaitPort(modWin->UserPort);
				while(imsg=GT_GetIMsg(modWin->UserPort)) {
					SFXMod_Interface_AnalyseIMsg
					switch(iclass) {
						case IDCMP_GADGETUP:
							gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
							if(!HandleModulator(&(instance->mod1),&dummysrc,0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode)
							&& !HandleModulator(&(instance->mod2),&dummysrc,0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode)
							&& !HandleCfgSel   (&cfgsel      ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
								switch(gnr) {
									SFXMod_Interface_GadgetUpEvents
									case GADID_RATESEL:
										UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										rate=atof(param);
										if(rate=GetPeriode(rate,NULL)) {
											sprintf(param,"%ld hz",rate);
											UC_ConvertParameter(instance->p_srat,param,UGRP_ABSFREQUENCY,UC_GetParameterUnit(instance->p_srat,UGRP_ABSFREQUENCY));
											GT_SetGadgetAttrs(modGadgets[GADIX_RATE],modWin,0l,GTST_String,instance->p_srat,TAG_DONE);
											UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
											srat=atof(param);
											if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(instance->p_slen);
											else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);slen=atof(param)*((double)srat/1000.0); }
											if(slen<2) slen=2;
											instance->mod1.slen=slen;
											instance->mod1.srat=srat;
											instance->mod2.slen=slen;
											instance->mod2.srat=srat;
										}
										break;
									case GADID_RATE:
										HandleParameter(instance->p_srat,modWin,modGadgets[GADIX_RATE]);
										UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										srat=atof(param);
										if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(instance->p_slen);
										else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);slen=atof(param)*((double)srat/1000.0); }
										if(slen<2) slen=2;
										instance->mod1.slen=slen;
										instance->mod1.srat=srat;
										instance->mod2.slen=slen;
										instance->mod2.srat=srat;
										break;
									case GADID_LENGTH:
										HandleParameter(instance->p_slen,modWin,modGadgets[GADIX_LENGTH]);
										UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										srat=atof(param);
										if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(instance->p_slen);
										else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);slen=atof(param)*((double)srat/1000.0); }
										if(slen<2) slen=2;
										instance->mod1.slen=slen;
										instance->mod1.srat=srat;
										instance->mod2.slen=slen;
										instance->mod2.srat=srat;
										break;
								}
							}
							break;
						case IDCMP_VANILLAKEY:
							switch(icode) {
								SFXMod_Interface_VKeyEvents
								case KEY_TAB:
									ActivateGadget(modGadgets[GADIX_MOD1],modWin,0l);
									break;
							}
							break;
						SFXMod_Interface_Events
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
	return(ret);
	Error:
	return(FALSE);
}

UBYTE SAVEDS ASM LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	//-- loop
	//UBYTE l;
	//-- prowin
	PWData *pwd;
	ProWinTitle title;
	//-- procesing length
	//-- src & dst buffers
	SInfo *dstbuf=NULL;
	//-- buffer access
	SmpAccess *dstsa;
	SAFuncPtr SAWrite;
	ULONG blen;
	//-- administrative
	Param param;
	UBYTE newdst=FALSE;
	UBYTE okay=TRUE;

#ifdef DEBUG
//	DumpFXParams();
#endif

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:				// wir wurden e.g. per ARexx gestartet
			break;
		case CM_STREAM:
			dstbuf=pdata->dst[0];
			break;
		case CM_PREHEAR:
			break;
	}

	UC_ConvertParameter(param,instance->mod1.vals,UGRP_AMPLITUDE,AMP_LEVEL);
	instance->mis=atof(param)+atof(param);
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_AMPLITUDE,AMP_LEVEL);
	instance->mie=atof(param)+atof(param);
	instance->mid=instance->mie-instance->mis;
		
	UC_ConvertParameter(param,instance->mod2.vals,UGRP_AMPLITUDE,AMP_LEVEL);
	instance->mas=atof(param)+atof(param);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_AMPLITUDE,AMP_LEVEL);
	instance->mae=atof(param)+atof(param);
	instance->mad=instance->mae-instance->mas;

	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
	if(instance->slen<2) instance->slen=2;

	if(!dstbuf) {
		if(dstbuf=NewSample()) {
			if(AllocSampleMem(dstbuf,instance->slen,(CH_MONO+1))) {
				dstbuf->srat=instance->srat;
				dstbuf->slen=instance->slen;
				dstbuf->channels=CH_MONO;
				dstbuf->channelmask=CH_CENTER;
				dstbuf->aktch=dstbuf->channels+1;
				dstbuf->zoomxl=instance->slen;
				newdst=TRUE;
			}
			else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
	}
	if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
		if(activate((SFXCoreInstance *)instance)) {
			//for(l=0;l<1;l++) {
				activate_ch((SFXCoreInstance *)instance,0);
				if((dstsa=SAInit(dstbuf,0,0,instance->slen,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {	// sequentiell Blöcke konstanter Größe schreiben
					SAWrite=dstsa->safunc;
					blen=Bytes2Samples(dstsa->blen);
					sprintf(title,GetString(msgProgress),PRJ_NAME,1,1);
					StartProWin(pwd,(ULONG *)(&instance->curlen),title,instance->slen);					// den Statusbalken starten
					for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && (instance->curlen<instance->slen));dstsa->seg++) {
						instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
						process((SFXCoreInstance *)instance,blen);
						SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
					}
					SADone(dstsa,dstbuf);
				}
				else okay=FALSE;
				deactivate_ch((SFXCoreInstance *)instance,0);
				if(ProWinAborted(instance->curlen)) { okay=FALSE; }										  // es wurde abgebrochen
			//}
			if(okay && newdst) AddSample(dstbuf,"Noise");
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
		cf=TRUE;	pf=ParseModulatorSet   (&(instance->mod1), "P1",params);
		if(!pf) pf=ParseModulatorSet   (&(instance->mod2), "P2",params);
		if(!pf && !stricmp(RexxPar2,"Rate"))		{ strntcpy(instance->p_srat,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(instance->p_slen,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet   (&(instance->mod1), "P1",params,ret);
		if(!pf) pf=ParseModulatorGet   (&(instance->mod2), "P2",params,ret);
		if(!pf && !stricmp(RexxPar2,"Rate"))		{ strntcpy(ret,instance->p_srat,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(ret,instance->p_slen,PARBUFLEN); }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

void LoadSettings(struct Instance *instance,char *fname) {
	Param param;

	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_srat	,CfgIO_ReadString	(cfg,"General"	,"Rate"		,instance->p_srat));
	strcpy(instance->p_slen	,CfgIO_ReadString	(cfg,"General"	,"Length"	,instance->p_slen));

	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
	if(instance->slen<2) instance->slen=2;
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Rate"		,instance->p_srat);
	CfgIO_WriteString		  (cfg,"General"	,"Length"	,instance->p_slen);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	Param param;

	UC_ConvertParameter(instance->mod1.vals,"0.0",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->mod1.vale,"0.0",UGRP_AMPLITUDE,AMP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"1.0",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->mod2.vale,"1.0",UGRP_AMPLITUDE,AMP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	UC_ConvertParameter(instance->p_srat,"22050 hz",UGRP_ABSFREQUENCY,AFRQ_HZ);
	UC_ConvertParameter(instance->p_slen,"1 s",UGRP_ABSTIME,ATME_S);

	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
	if(instance->slen<2) instance->slen=2;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
	GT_SetGadgetAttrs(modGadgets[GADIX_RATE],modWin,0l,GTST_String,instance->p_srat,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH],modWin,0l,GTST_String,instance->p_slen,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

#ifdef uncomment_me
void DumpFXParams(void) {
	MSG ("structure dump");
	MSG ("  Modulator mod1 =");
	MSG2("    Param         vals,vale = '%s','%s'",instance->mod1.vals,instance->mod1.vale);
	MSG1("    UBYTE         bshape    = %d"       ,instance->mod1.bshape);
	MSG1("    BShapeFuncPtr modfunc   = 0x%08lx"  ,instance->mod1.modfunc);
	MSG1("    char          desc      = '%s'"     ,instance->mod1.desc);
	MSG2("    ULONG         slen,srat = %ld,%ld"  ,instance->mod1.slen,instance->mod1.srat);
	MSG1("    APTR          bshpdata  = 0x%08lx"  ,instance->mod1.bshpdata);
	switch(instance->mod1.bshape) {
		case BSHP_NONE: break;
		case BSHP_CURVE:
			MSG1("      double exp = %08lf",((ModCurve *)(instance->mod1.bshpdata))->exp);
			break;
		case BSHP_CYCLE:
			MSG1("      UBYTE oszi = %d",((ModCycle *)(instance->mod1.bshpdata))->oszi);
			break;
	}
	MSG ("  Modulator mod2");
	MSG2("    Param         vals,vale = '%s','%s'",instance->mod2.vals,instance->mod2.vale);
	MSG1("    UBYTE         bshape    = %d"       ,instance->mod2.bshape);
	MSG1("    BShapeFuncPtr modfunc   = 0x%08lx"  ,instance->mod2.modfunc);
	MSG1("    char          desc      = '%s'"     ,instance->mod2.desc);
	MSG2("    ULONG         slen,srat = %ld,%ld"  ,instance->mod2.slen,instance->mod2.srat);
	MSG1("    APTR          bshpdata  = 0x%08lx"  ,instance->mod2.bshpdata);
	switch(instance->mod2.bshape) {
		case BSHP_NONE: break;
		case BSHP_CURVE:
			MSG1("      double exp = %08lf",((ModCurve *)(instance->mod2.bshpdata))->exp);
			break;
		case BSHP_CYCLE:
			MSG1("      UBYTE oszi = %d",((ModCycle *)(instance->mod2.bshpdata))->oszi);
			break;
	}
	
}
#endif

//-- eof ----------------------------------------------------------------------
