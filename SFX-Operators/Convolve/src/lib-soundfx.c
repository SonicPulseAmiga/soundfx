/****h* Convolve/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (29.10.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Convolve Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    29.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    01.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*  TODO
*
*******
*/

#define CONVOLVE_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Convolve.h"

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

#define GADID_SRC1		(GADID_CFGSEL+CFGSEL_IDCT)
#define GADIX_SRC1		(GADIX_CFGSEL+CFGSEL_IXCT)

#define GADID_SRC2		(GADID_SRC1+SRC_IDCT)
#define GADIX_SRC2		(GADIX_SRC1+SRC_IXCT)

#define GADID_MOD1		(GADID_SRC2+SRC_IDCT)
#define GADIX_MOD1		(GADIX_SRC2+SRC_IXCT)

#define GADID_MOD2		(GADID_MOD1+MOD_IDCT)
#define GADIX_MOD2		(GADIX_MOD1+MOD_IXCT)

#define GADID_AMPF		(GADID_MOD2+MOD_IDCT)
#define GADIX_AMPF		(GADIX_MOD2+MOD_IXCT)

#define LAST_GADIX		(GADIX_AMPF+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[5];
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
		SFXMod_ModDescInit(0,PD_SOURCE      ,"S1",&(instance->src1));
		SFXMod_ModDescInit(1,PD_SOURCE      ,"S2",&(instance->src2));
		SFXMod_ModDescInit(2,PD_MODULATOR   ,"M1",&(instance->mod1));
		SFXMod_ModDescInit(3,PD_MODULATOR   ,"M2",&(instance->mod2));
		SFXMod_ModDescInit(4,PD_END         ,0l  ,0l);	
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
	UWORD ys3=ys2+1+SRCSEL_HEIGHT;
	UWORD ys4=ys3+1+MODSEL_HEIGHT;
	UWORD ys5=ys4+1+MODSEL_HEIGHT;
	UWORD ys6=ys5+1+ANYFRM_HEIGHT(1);
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

		g=AddSource(&(instance->src1),modGadgets,g,GADID_SRC1,GADIX_SRC1,xs1,ys1);
		g=AddSource(&(instance->src2),modGadgets,g,GADID_SRC2,GADIX_SRC2,xs1,ys2);

		g=AddModulator(&(instance->mod1),modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys3);
		g=AddModulator(&(instance->mod2),modGadgets,g,GADID_MOD2,GADIX_MOD2,xs1,ys4);

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

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys4+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Impulsresponse",xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Effect"		,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Feedback"		,xs1,ys4,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"	,xs1,ys5,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);					/* Par */
			DrawGadWB(modrp,xs1,ys4,xh,MODSEL_HEIGHT);					/* Par */
			DrawGadWB(modrp,xs1,ys5,xh,ANYFRM_HEIGHT(1));				/* Par */
			DrawGadWB(modrp,xs1,ys6,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src1),modWin,modGadgets,GADIX_SRC1);
			SetSource   (&(instance->src2),modWin,modGadgets,GADIX_SRC2);
			SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
			SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);

			if(!enabled) GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=1;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						instance->src1.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src1.rmode=SetRngMode(instance->src1.srcbuf);
						instance->src2.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src2.rmode=SetRngMode(instance->src2.srcbuf);
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=0;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					}
					SetSource   (&(instance->src1),modWin,modGadgets,GADIX_SRC1);
					SetSource   (&(instance->src2),modWin,modGadgets,GADIX_SRC2);
				}
				if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(!HandleSource   (               &(instance->src1),               modWin,modGadgets,GADID_SRC1,  GADIX_SRC1,  gnr,icode)
								&& !HandleSource   (               &(instance->src2),               modWin,modGadgets,GADID_SRC2,  GADIX_SRC2,  gnr,icode)
								&& !HandleModulator(&(instance->mod1),&(instance->src1),&(instance->src2),modWin,modGadgets,GADID_MOD1,  GADIX_MOD1,  gnr,icode)
								&& !HandleModulator(&(instance->mod2),&(instance->src1),&(instance->src2),modWin,modGadgets,GADID_MOD2,  GADIX_MOD2,  gnr,icode)
								&& !HandleCfgSel   (&cfgsel       ,                              modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_AMPF:
											HandleParameter(instance->p_ampf,modWin,modGadgets[GADIX_AMPF]);
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
	PWData *pwd;
	ProWinTitle title;
	//-- procesing length
	ULONG rm_start,rm_len,blen;
	//-- src & dst buffers
	SInfo *dstbuf=NULL,*srcbuf1,*srcbuf2;
	//-- buffer access
	SmpAccess *srcsa,*dstsa;
	SAFuncPtr SARead,SAWrite;
	//-- administrative
	Param param;
	UBYTE newdst=FALSE;
	UBYTE okay=TRUE;

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:				// wir wurden e.g. per ARexx gestartet
			UnlockBuffer(instance->src1.srcbuf);
			if((instance->src1.srcbuf=LockBuffer(pdata->src[0]))==NULL) return(FALSE);
			instance->src1.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src2.srcbuf);
			if((instance->src2.srcbuf=LockBuffer(pdata->src[1]))==NULL) return(FALSE);
			instance->src2.rmode=RNGMD_ALL;
			break;
		case CM_STREAM:
			UnlockBuffer(instance->src1.srcbuf);
			if((instance->src1.srcbuf=LockBuffer(pdata->src[0]))==NULL) return(FALSE);
			instance->src1.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src2.srcbuf);
			if((instance->src2.srcbuf=LockBuffer(pdata->src[1]))==NULL) return(FALSE);
			instance->src2.rmode=RNGMD_ALL;
			dstbuf=pdata->dst[0];
			break;
		case CM_PREHEAR:
			break;
	}

	srcbuf1=instance->src1.srcbuf;
	srcbuf2=instance->src2.srcbuf;
	SetRngs(srcbuf1,&rm_start,&rm_len,instance->src1.rmode);
	instance->slen=rm_len;
	instance->srat=srcbuf1->srat;

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

	UC_ConvertParameter(param,instance->p_ampf,UGRP_AMPLITUDE,AMP_FC);
	instance->ampf=atof(param);

	instance->dellen=srcbuf2->slen&0xFFFFFFFC;		/* make it a dividable by 4 without rest */
	
	if(!dstbuf) {
		if(dstbuf=NewSample()) {
			if(AllocSampleMem(dstbuf,srcbuf1->slen,CH_COUNT(srcbuf1->channels))) {
				BufferFill(srcbuf1,dstbuf);
				newdst=TRUE;
			}
			else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
	}
	if(pwd=NewProWin()) {										// ein neues asynchrones Statusfenster öffnen
		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;l<CH_COUNT(dstbuf->channels);l++) {
				if(IS_CHANNEL_ACTIVE(l,srcbuf1->channelmask)) {
					activate_ch((SFXCoreInstance *)instance,l);
					if(rm_start>0) CopyDataCh(srcbuf1,dstbuf,l,0,0,rm_start);
					srcsa=SAInit(srcbuf1,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					dstsa=SAInit(dstbuf ,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					if(srcsa && dstsa && (srcbuf2->storage==VM_MEMORY)) {
						SARead =srcsa->safunc;
						SAWrite=dstsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(dstbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
						for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
							instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
							process((SFXCoreInstance *)instance,blen);
							SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
						}
					}
					else okay=FALSE;
					if(srcsa) SADone(srcsa,srcbuf1);
					if(dstsa) SADone(dstsa,dstbuf);
					if(rm_start+rm_len<dstbuf->slen) CopyDataCh(srcbuf1,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(dstbuf->slen-(rm_start+rm_len)));
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }					// es wurde abgebrochen
					deactivate_ch((SFXCoreInstance *)instance,l);
				}
				else CopyDataCh(srcbuf1,dstbuf,l,0,0,dstbuf->slen);
			}
			if(okay && newdst) AddSample(dstbuf,JoinSampleNames(((struct Node *)srcbuf1)->ln_Name,((struct Node *)srcbuf2)->ln_Name));
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
		cf=TRUE;pf=ParseModulatorSet  (&(instance->mod1),"P1",params);
		if(!pf) ParseModulatorSet  (&(instance->mod2),"P2",params);
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strntcpy(instance->p_ampf,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=ParseModulatorGet  (&(instance->mod1),"P1",params,ret);
		if(!pf) ParseModulatorGet  (&(instance->mod2),"P2",params,ret);
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strntcpy(ret,instance->p_ampf,PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_ampf,CfgIO_ReadString		(cfg,"General"	,"Ampf"	,"0.1"));
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Ampf"	,instance->p_ampf);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src1.srcbuf=0L;
	instance->src1.rmode=RNGMD_ALL;

	instance->src2.srcbuf=0L;
	instance->src2.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod1.vals,"0.5",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod1.vale,"0.5",UGRP_PROPOTION,PROP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"0.15",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod2.vale,"0.15",UGRP_PROPOTION,PROP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	UC_ConvertParameter(instance->p_ampf,"0.1",UGRP_AMPLITUDE,AMP_FC);
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src1),modWin,modGadgets,GADIX_SRC1);
	SetSource   (&(instance->src2),modWin,modGadgets,GADIX_SRC2);
	SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
	GT_SetGadgetAttrs(modGadgets[GADIX_AMPF],modWin,0l,GTST_String,instance->p_ampf,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
