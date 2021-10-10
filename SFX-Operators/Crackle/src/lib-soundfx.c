/****h* Crackle/lib-soundfx.c [4.3] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.3 (06.03.03) � by Stefan Kost 1998-2003
*  FUNCTION
*    Crackle Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    14.Aug.2003
*  MODIFICATION HISTORY
*    14.Aug.2003	V 4.3	most recent version
*    14.Aug.2003	V 4.3	initial version
*  NOTES
*
*******
*/

#define CRACKLE_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Crackle.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_CFGSEL    (GADID_BASE+1)
#define GADIX_CFGSEL    (GADIX_BASE+1)
#define GADID_CFGLIST   (GADID_CFGSEL)
#define GADIX_CFGLIST   (GADIX_CFGSEL)
#define GADID_CFGNAME   (GADID_CFGLIST+1)
#define GADIX_CFGNAME   (GADIX_CFGLIST+1)
#define GADID_CFGADD    (GADID_CFGNAME+1)
#define GADIX_CFGADD    (GADIX_CFGNAME+1)
#define GADID_CFGDEL    (GADID_CFGADD+1)
#define GADIX_CFGDEL    (GADIX_CFGADD+1)

#define GADID_SRC       (GADID_CFGSEL+CFGSEL_IDCT)
#define GADIX_SRC       (GADIX_CFGSEL+CFGSEL_IXCT)

#define GADID_MOD		(GADID_SRC+SRC_IDCT)
#define GADIX_MOD		(GADIX_SRC+SRC_IXCT)

#define LAST_GADIX		(GADIX_MOD+MOD_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData		moddesc[3];
};

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int SAVEDS ASM __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) � by Stefan Kost 1993-%s"))) goto Error;
	init();							// initialize plugin core
	return(FALSE);
	Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void SAVEDS ASM __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	done();
	if(cfg)				CfgIO_RemConfig(cfg);
	SFXMod_CloseCommonAndGUILibs;
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

void SAVEDS ASM *LIBSFXMod_ClientDataInit(REG(a0,RTime *RunTime_)) {
	struct Instance *instance;
	
	if(instance=(struct Instance *)AllocVec(sizeof(struct Instance),MEMF_ANY|MEMF_CLEAR)) {
		SFXMod_ModDescInit(0,PD_SOURCE      ,"S1",&(instance->src));
		SFXMod_ModDescInit(1,PD_MODULATOR   ,"M1",&(instance->mod));
		SFXMod_ModDescInit(2,PD_END         ,0l  ,0l);
		instantiate((SFXCoreInstance *)instance);
		SFXMod_ClientDataInitCommon;
	}
	return((void *)instance);
}

void SAVEDS ASM LIBSFXMod_ClientDataDone(REG(a0,struct Instance *instance)) {
	if(instance) {
		SFXMod_ClientDataDoneCommon;
		destroy((SFXCoreInstance *)instance);
		FreeVec((void *)instance);
	}
}

//-- Modulroutinen

UBYTE SAVEDS ASM LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	SFXMod_Interface_GUIVariables
	SFXMod_Interface_CfgSelVariables
	SFXMod_Interface_NotifyVariables

	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+MODSEL_HEIGHT;
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

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

		g=AddModulator(&(instance->mod),modGadgets,g,GADID_MOD,GADIX_MOD,xs1,ys2);

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Density"       ,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Eff */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
			SetModulator(&(instance->mod),modWin,modGadgets,GADIX_MOD);

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
								&& !HandleModulator(&(instance->mod),&(instance->src),0l,modWin,modGadgets,GADID_MOD,GADIX_MOD,gnr,icode)
								&& !HandleCfgSel	  (&cfgsel,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_MOD],modWin,0l);
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
	UBYTE pu;
	Param param;
	UBYTE newdst=FALSE;
	double crd;
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
	
	srcbuf=instance->src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);
	instance->slen=rm_len;
	instance->srat=srcbuf->srat;

	//-- set params
	if(UC_GetParameterUnit(instance->mod.vals,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
		UC_ConvertParameter(param,instance->mod.vals,UGRP_ABSFREQUENCY,AFRQ_HZ);if((crd=atof(param))<1.0) crd=1.0;
		instance->crds=(double)srcbuf->srat/crd;
	}
	else {
		if((pu=UC_GetParameterUnit(instance->mod.vals,UGRP_ABSTIME))!=INVALID_UNIT) {
			if(pu==ATME_SAMPLES) instance->crds=atof(instance->mod.vals);
			else { UC_ConvertParameter(param,instance->mod.vals,UGRP_ABSTIME,ATME_MS);instance->crds=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,instance->mod.vals,UGRP_RELTIME,RTME_FC);instance->crds=(double)srcbuf->slen*atof(param); }
	}
	if(instance->crds<1.0) instance->crds=1.0;
	if(UC_GetParameterUnit(instance->mod.vale,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
		UC_ConvertParameter(param,instance->mod.vale,UGRP_ABSFREQUENCY,AFRQ_HZ);if((crd=atof(param))<1.0) crd=1.0;
		instance->crde=(double)srcbuf->srat/crd;
	}
	else {
		if((pu=UC_GetParameterUnit(instance->mod.vale,UGRP_ABSTIME))!=INVALID_UNIT) {
			if(pu==ATME_SAMPLES) instance->crde=atof(instance->mod.vale);
			else { UC_ConvertParameter(param,instance->mod.vale,UGRP_ABSTIME,ATME_MS);instance->crde=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,instance->mod.vale,UGRP_RELTIME,RTME_FC);instance->crde=(double)srcbuf->slen*atof(param); }
	}
	if(instance->crde<1.0) instance->crde=1.0;
	instance->crdd=instance->crde-instance->crds;

	MSG2("crds/e %lf ... %lf",instance->crds,instance->crde);


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
	if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster �ffnen
		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
				if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {
					activate_ch((SFXCoreInstance *)instance,l);
					if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);
					srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl�cke konstanter Gr��e lesen
					dstsa=SAInit(dstbuf,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl�cke konstanter Gr��e schreiben
					if(srcsa && dstsa) {
						SARead =srcsa->safunc;
						SAWrite=dstsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					  // den Statusbalken starten
						for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
							instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);		// Pointer auf src-Block holen & n�chstes Read starten
							instance->dbuf=(SAMPLE *)dstsa->buffer1;									// Pointer auf dst-Block
							process((SFXCoreInstance *)instance,blen);
							SAWrite(dstsa,dstsa->blen);										// akt Block schreiben
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
		return(FALSE);
	}
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;pf=ParseModulatorSet  (&(instance->mod),"P1",params);
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;
		ParseModulatorGet  (&(instance->mod),"P1",params,ret);
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src.srcbuf=NULL;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod.vals,"10 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->mod.vale,"200 ms",UGRP_ABSTIME,ATME_MS);
	instance->mod.bshape=BSHP_CYCLE;
	if((instance->mod.bshpdata=AllocVec(sizeof(ModCycle),MEMF_ANY|MEMF_CLEAR)))	{
		((ModCycle *)instance->mod.bshpdata)->oszi=MC_OSZI_RND;
		strcpy(((ModCycle *)instance->mod.bshpdata)->frq,"10 Hz");
		strcpy(((ModCycle *)instance->mod.bshpdata)->pha,"0.0 �");
	}
	else instance->mod.bshape=BSHP_NONE;
	instance->mod.desc[0]='\0';
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
	SetModulator(&(instance->mod),modWin,modGadgets,GADIX_MOD);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
