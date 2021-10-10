/****h* Delay/lib-soundfx.c [4.3] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.3 (29.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Delay Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    29.Mar.2004    V 4.3   most recent version
*    29.Oct.2001	V 4.2	most recent version
*    17.Nov.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*							* precalc : blen=Byte2Samples(srcsa->blen)
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define DELAY_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Delay.h"

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

#define GADID_MOD1		(GADID_SRC+SRC_IDCT)
#define GADIX_MOD1		(GADIX_SRC+SRC_IXCT)

#define GADID_MOD2		(GADID_MOD1+MOD_IDCT)
#define GADIX_MOD2		(GADIX_MOD1+MOD_IXCT)

#define GADID_MOD3		(GADID_MOD2+MOD_IDCT)
#define GADIX_MOD3		(GADIX_MOD2+MOD_IXCT)

#define GADID_AMPF		(GADID_MOD3+MOD_IDCT)
#define GADIX_AMPF		(GADIX_MOD3+MOD_IXCT)

#define GADID_DRY		(GADID_AMPF+1)
#define GADIX_DRY		(GADIX_AMPF+1)

#define GADID_INTER		(GADID_DRY+1)
#define GADIX_INTER		(GADIX_DRY+1)

#define LAST_GADIX		(GADIX_INTER+INT_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[6];
};

char *DryLabels[]={ "Dry=1-Eff","Dry=-Eff",0l };

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
		SFXMod_ModDescInit(1,PD_MODULATOR   ,"M1",&(instance->mod1));
		SFXMod_ModDescInit(2,PD_MODULATOR   ,"M2",&(instance->mod2));
		SFXMod_ModDescInit(3,PD_MODULATOR   ,"M3",&(instance->mod3));
		SFXMod_ModDescInit(4,PD_INTERPOLATOR,"I1",&(instance->int1));
		SFXMod_ModDescInit(5,PD_END         ,0l  ,0l);
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
	UWORD ys3=ys2+1+MODSEL_HEIGHT;
	UWORD ys4=ys3+1+MODSEL_HEIGHT;
	UWORD ys5=ys4+1+MODSEL_HEIGHT;
	UWORD ys6=ys5+1+ANYFRM_HEIGHT(2);
	UWORD winh=ys6+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[104];
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

		g=AddModulator(&(instance->mod1),modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys2);
		g=AddModulator(&(instance->mod2),modGadgets,g,GADID_MOD2,GADIX_MOD2,xs1,ys3);
		g=AddModulator(&(instance->mod3),modGadgets,g,GADID_MOD3,GADIX_MOD3,xs1,ys4);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[60];
		ng.ng_GadgetText	="Ampf";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_AMPF;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_ampf,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMPF]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="Dry";
		ng.ng_GadgetID		=GADID_DRY;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,DryLabels,GTCY_Active,instance->drymode,TAG_DONE);
		modGadgets[GADIX_DRY]=g;

		g=AddInterpolator(&(instance->int1),modGadgets,g,GADID_INTER,GADIX_INTER,xs1,ys5+RunTime->scy[FRMCONTENT_YOFFSET+12]);

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys4+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Interp.",xs1+RunTime->scx[45],ys5+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Effect"		,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Feedback"		,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Delay"			,xs1,ys4,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"	,xs1,ys5,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Eff */
			DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);					/* Fb */
			DrawGadWB(modrp,xs1,ys4,xh,MODSEL_HEIGHT);					/* Delay */
			DrawGadWB(modrp,xs1,ys5,xh,ANYFRM_HEIGHT(2));				/* Par */
			DrawGadWB(modrp,xs1,ys6,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel      (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource      (&(instance->src ),modWin,modGadgets,GADIX_SRC);
			SetModulator   (&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
			SetModulator   (&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
			SetModulator   (&(instance->mod3),modWin,modGadgets,GADIX_MOD3);
			SetInterpolator(&(instance->int1),modWin,modGadgets,GADIX_INTER);

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
								if(!HandleSource      (&(instance->src),                  modWin,modGadgets,GADID_SRC,   GADIX_SRC,   gnr,icode)
								&& !HandleModulator   (&(instance->mod1),&(instance->src),0l,modWin,modGadgets,GADID_MOD1,  GADIX_MOD1,  gnr,icode)
								&& !HandleModulator   (&(instance->mod2),&(instance->src),0l,modWin,modGadgets,GADID_MOD2,  GADIX_MOD2,  gnr,icode)
								&& !HandleModulator   (&(instance->mod3),&(instance->src),0l,modWin,modGadgets,GADID_MOD3,  GADIX_MOD3,  gnr,icode)
								&& !HandleInterpolator(&(instance->int1),                 modWin,modGadgets,GADID_INTER, GADIX_INTER, gnr,icode)
								&& !HandleCfgSel	  (&cfgsel       ,                 modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_AMPF:
											HandleParameter(instance->p_ampf,modWin,modGadgets[GADIX_AMPF]);
											break;
										case GADID_DRY:
											instance->drymode=icode;
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
	PWData *pwd=NULL;
	ProWinTitle title;
	//-- procesing length
	ULONG rm_start,rm_len;
	//-- src & dst buffers
	SInfo *dstbuf=NULL,*srcbuf;						// sampleaccess
	//-- buffer access
	SmpAccess *srcsa,*dstsa;
	SAFuncPtr SARead,SAWrite;
	ULONG blen;
	//-- administrative
	UBYTE pu;
	Param param;
	UBYTE newdst=FALSE;
	double del;
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

	UC_ConvertParameter(param,instance->mod1.vals,UGRP_PROPOTION,PROP_FC);
	instance->effs=atof(param);
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_PROPOTION,PROP_FC);
	instance->effe=atof(param);
	instance->effd=instance->effe-instance->effs;

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_PROPOTION,PROP_FC);
	instance->fbs=atof(param);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_PROPOTION,PROP_FC);
	instance->fbe=atof(param);
	instance->fbd=instance->fbe-instance->fbs;

	if(UC_GetParameterUnit(instance->mod3.vals,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
		UC_ConvertParameter(param,instance->mod3.vals,UGRP_ABSFREQUENCY,AFRQ_HZ);if((del=atof(param))<1.0) del=1.0;
		instance->dels=(double)srcbuf->srat/del;
	}
	else {
		if((pu=UC_GetParameterUnit(instance->mod3.vals,UGRP_ABSTIME))!=INVALID_UNIT) {
			if(pu==ATME_SAMPLES) instance->dels=atof(instance->mod3.vals);
			else { UC_ConvertParameter(param,instance->mod3.vals,UGRP_ABSTIME,ATME_MS);instance->dels=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,instance->mod3.vals,UGRP_RELTIME,RTME_FC);instance->dels=(double)srcbuf->slen*atof(param); }
	}
	if(instance->dels<1.0) instance->dels=1.0;
	if(UC_GetParameterUnit(instance->mod3.vale,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
		UC_ConvertParameter(param,instance->mod3.vale,UGRP_ABSFREQUENCY,AFRQ_HZ);if((del=atof(param))<1.0) del=1.0;
		instance->dele=(double)srcbuf->srat/del;
	}
	else {
		if((pu=UC_GetParameterUnit(instance->mod3.vale,UGRP_ABSTIME))!=INVALID_UNIT) {
			if(pu==ATME_SAMPLES) instance->dele=atof(instance->mod3.vale);
			else { UC_ConvertParameter(param,instance->mod3.vale,UGRP_ABSTIME,ATME_MS);instance->dele=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,instance->mod3.vale,UGRP_RELTIME,RTME_FC);instance->dele=(double)srcbuf->slen*atof(param); }
	}
	if(instance->dele<1.0) instance->dele=1.0;
	instance->deld=instance->dele-instance->dels;

	UC_ConvertParameter(param,instance->p_ampf,UGRP_AMPLITUDE,AMP_FC);
	instance->ampf=atof(param);

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
	if(pwd=NewProWin()) {										// ein neues asynchrones Statusfenster öffnen
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
						if(!instance->drymode) {
							for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
								instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
								instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
								process_0((SFXCoreInstance *)instance,blen);
								SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
							}
						}
						else {
							for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
								instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
								instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
								process_1((SFXCoreInstance *)instance,blen);
								SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
							}
						}
					}
					else okay=FALSE;
					if(srcsa) SADone(srcsa,srcbuf);
					if(dstsa) SADone(dstsa,dstbuf);
					if(rm_start+rm_len<srcbuf->slen) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }					// es wurde abgebrochen
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
		cf=TRUE;	pf=ParseModulatorSet   (&(instance->mod1),"P1",params);
		if(!pf) pf=ParseModulatorSet   (&(instance->mod2),"P2",params);
		if(!pf) pf=ParseModulatorSet   (&(instance->mod3),"P3",params);
		if(!pf) pf=ParseInterpolatorSet(&(instance->int1),"I1",params);
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strntcpy(instance->p_ampf ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Dry"))			{ instance->drymode=CfgIO_MatchString(RexxPar3,DryLabels);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet   (&(instance->mod1),"P1",params,ret);
		if(!pf) pf=ParseModulatorGet   (&(instance->mod2),"P2",params,ret);
		if(!pf) pf=ParseModulatorGet   (&(instance->mod3),"P3",params,ret);
		if(!pf) pf=ParseInterpolatorGet(&(instance->int1),"I1",params,ret);
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strntcpy(ret,instance->p_ampf ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Dry"))			{ strcpy(ret,DryLabels[instance->drymode]);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);

	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_ampf,CfgIO_ReadString	(cfg,"General"	,"Ampf"	,instance->p_ampf));
	instance->drymode=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Dry"	,DryLabels[instance->drymode]),DryLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Ampf"		,instance->p_ampf);
	CfgIO_WriteString		  (cfg,"General"	,"Dry"		,DryLabels[instance->drymode]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src.srcbuf=NULL;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod1.vals,"0.4",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod1.vale,"0.4",UGRP_PROPOTION,PROP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"0.4",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod2.vale,"0.4",UGRP_PROPOTION,PROP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	UC_ConvertParameter(instance->mod3.vals,"100 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->mod3.vale,"100 ms",UGRP_ABSTIME,ATME_MS);
	instance->mod3.bshape=BSHP_NONE;
	instance->mod3.desc[0]='\0';

	instance->int1.range=2.0;
	instance->int1.type=INT_TYPE_LIN;
	instance->int1.desc[0]='\0';

	UC_ConvertParameter(instance->p_ampf ,"1.25",UGRP_AMPLITUDE,AMP_FC);
	instance->drymode=0;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin){
	SetSource      (&(instance->src ),modWin,modGadgets,GADIX_SRC);
	SetModulator   (&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator   (&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
	SetModulator   (&(instance->mod3),modWin,modGadgets,GADIX_MOD3);
	SetInterpolator(&(instance->int1),modWin,modGadgets,GADIX_INTER);
	GT_SetGadgetAttrs(modGadgets[GADIX_AMPF],modWin,0l,GTST_String,instance->p_ampf,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_DRY ],modWin,0l,GTCY_Active,instance->drymode ,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
