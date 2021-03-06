/****h* AmplifySplit/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (29.10.01) ? by Stefan Kost 1998-2001
*  FUNCTION
*    AmplifySplit Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    29.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    01.Aug.2000	V 1.2	most recent version
*    19.Jul.1998	V 1.0	initial version
*  NOTES
*
*******
*/

#define AMPLIFYSPLIT_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "AmplifySplit.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

UBYTE GetMaxVol(struct Instance *instance);

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

#define GADID_MOD1		(GADID_SRC+SRC_IDCT)
#define GADIX_MOD1		(GADIX_SRC+SRC_IXCT)

#define GADID_MOD2		(GADID_MOD1+MOD_IDCT)
#define GADIX_MOD2		(GADIX_MOD1+MOD_IXCT)

#define GADID_MAXVOLU	(GADID_MOD2+MOD_IDCT)
#define GADIX_MAXVOLU	(GADIX_MOD2+MOD_IXCT)

#define GADID_MAXVOLL	(GADID_MAXVOLU+1)
#define GADIX_MAXVOLL	(GADIX_MAXVOLU+1)

#define LAST_GADIX		(GADIX_MAXVOLL+1)

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
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) ? by Stefan Kost 1993-%s"))) goto Error;
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
	UWORD ys3=ys2+1+MODSEL_HEIGHT;
	UWORD ys4=ys3+1+MODSEL_HEIGHT;
	UWORD ys5=ys4+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys5+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;
	struct Requester NullReq;	//-- window input blocker
	UBYTE maxvol;
	
	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[104];
		ng.ng_TopEdge		=ys5+RunTime->scy[2];
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

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];
		ng.ng_TopEdge		=ys4+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[120];
		ng.ng_GadgetText	="MaxVolUpper";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_MAXVOLU;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_MAXVOLU]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[174];
		ng.ng_GadgetText	="MaxVolLower";
		ng.ng_GadgetID		=GADID_MAXVOLL;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_MAXVOLL]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"				,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Upper Amplification"	,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Lower Amplification"	,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"			,xs1,ys4,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"				,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);					/* Par */
			DrawGadWB(modrp,xs1,ys4,xh,ANYFRM_HEIGHT(1));				/* Par */
			DrawGadWB(modrp,xs1,ys5,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
			SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
			SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);

			if(!enabled) {
				GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOLU],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOLL],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
			}

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=1;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOLU],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOLL],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						instance->src.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src.rmode=SetRngMode(instance->src.srcbuf);
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=0;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOLU],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOLL],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					}
					SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
				}
				if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(!HandleSource   (               &(instance->src),   modWin,modGadgets,GADID_SRC,   GADIX_SRC,   gnr,icode)
								&& !HandleModulator(&(instance->mod1),&(instance->src),0l,modWin,modGadgets,GADID_MOD1,  GADIX_MOD1,  gnr,icode)
								&& !HandleModulator(&(instance->mod2),&(instance->src),0l,modWin,modGadgets,GADID_MOD2,  GADIX_MOD2,  gnr,icode)
								&& !HandleCfgSel   (&cfgsel       ,                 modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_MAXVOLU:
											maxvol=instance->maxvoll;instance->maxvoll=FALSE;
											instance->maxvolu=TRUE;
											if(instance->src.srcbuf) {
												Request(&NullReq,modWin);
												SetWindowPointer(modWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
												if(GetMaxVol(instance)) SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
												EndRequest(&NullReq,modWin);
												SetWindowPointer(modWin,TAG_DONE);
											}
											instance->maxvoll=maxvol;
											break;
										case GADID_MAXVOLL:
											maxvol=instance->maxvolu;instance->maxvolu=FALSE;
											instance->maxvoll=TRUE;
											if(instance->src.srcbuf) {
												Request(&NullReq,modWin);
												SetWindowPointer(modWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
												if(GetMaxVol(instance)) SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
												EndRequest(&NullReq,modWin);
												SetWindowPointer(modWin,TAG_DONE);
											}
											instance->maxvolu=maxvol;
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
	UBYTE l;	// channels
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
	
	srcbuf=instance->src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);
	instance->slen=rm_len;
	instance->srat=srcbuf->srat;

	if(instance->maxvolu || instance->maxvoll) {
		if(!GetMaxVol(instance)) return(FALSE);							   // maxvol suchen (wenn von rexx aus gestartet)
	}

	UC_ConvertParameter(param,instance->mod1.vals,UGRP_AMPLITUDE,AMP_FC);
	instance->ampus=atof(param);
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_AMPLITUDE,AMP_FC);
	instance->ampue=atof(param);
	instance->ampud=instance->ampue-instance->ampus;
	
	UC_ConvertParameter(param,instance->mod2.vals,UGRP_AMPLITUDE,AMP_FC);
	instance->ampls=atof(param);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_AMPLITUDE,AMP_FC);
	instance->ample=atof(param);
	instance->ampld=instance->ample-instance->ampls;

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
	if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster ?ffnen
		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
				if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {
					activate_ch((SFXCoreInstance *)instance,l);
					if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);
					srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl?cke konstanter Gr??e lesen
					dstsa=SAInit(dstbuf,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl?cke konstanter Gr??e schreiben
					if(srcsa && dstsa) {
						SARead =srcsa->safunc;
						SAWrite=dstsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
						for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
							instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & n?chstes Read starten
							instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
							process((SFXCoreInstance *)instance,blen);
							SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
						}
					}
					else okay=FALSE;
					if(srcsa) SADone(srcsa,srcbuf);
					if(dstsa) SADone(dstsa,dstbuf);
					if(rm_start+rm_len<srcbuf->slen) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
				if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }					// es wurde abgebrochen
					deactivate_ch((SFXCoreInstance *)instance,l);
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
		cf=TRUE;	pf=ParseModulatorSet(&(instance->mod1),"P1",params);
		if(!pf) pf=ParseModulatorSet(&(instance->mod2),"P2",params);
		if(!pf && !stricmp(RexxPar2,"MaxVolU"))			{ instance->maxvolu=atoi(RexxPar3)&1;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"MaxVolL"))			{ instance->maxvoll=atoi(RexxPar3)&1;pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet(&(instance->mod1),"P1",params,ret);
		if(!pf) pf=ParseModulatorGet(&(instance->mod2),"P2",params,ret);
		if(!pf && !stricmp(RexxPar2,"MaxVolU"))			{ sprintf(ret,"%d",instance->maxvolu);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"MaxVolL"))			{ sprintf(ret,"%d",instance->maxvoll);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	instance->maxvolu=CfgIO_ReadBool				(cfg,"General"	,"MaxVolU"	,TRUE);
	instance->maxvoll=CfgIO_ReadBool				(cfg,"General"	,"MaxVolL"	,TRUE);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteBool			  (cfg,"General"	,"MaxVolU"	,instance->maxvolu);
	CfgIO_WriteBool			  (cfg,"General"	,"MaxVolL"	,instance->maxvoll);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod1.vals,"1.0",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->mod1.vale,"1.0",UGRP_AMPLITUDE,AMP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"1.0",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->mod2.vale,"1.0",UGRP_AMPLITUDE,AMP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	instance->maxvolu=FALSE;
	instance->maxvoll=FALSE;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src ),modWin,modGadgets,GADIX_SRC);
	SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
}

//-- private Stuff ------------------------------------------------------------

UBYTE __inline GetMaxVol(struct Instance *instance) {
	//--loop
	register ULONG j;
	UBYTE l;
	//-- prowin
	PWData *pwd;
	ProWinTitle title;
	//-- src & dst buffers
	SInfo *srcbuf=instance->src.srcbuf;
	//-- buffer access
	SAMPLE *sbuf;
	SmpAccess *srcsa;
	SAFuncPtr SARead;
	//-- local
	ULONG rm_start,rm_len;
	Param param;
	UBYTE srctyp;
	LONG maxvalp=0,maxvaln=0;

	if((pwd=NewProWin())) {                 // ein neues asynchrones Statusfenster ?ffnen
		SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);       // gibt den zu bearbeitenden Bereich zur?ck
		instance->slen=rm_len;

		for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
			if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {
				if(srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN)) {   // sequentiell Bl?cke konstanter Gr??e lesen
					SARead =srcsa->safunc;
					sprintf(title,GetString(msgProgress),PRJ_NAME"/MaxVol",l+1,CH_COUNT(srcbuf->channels));
					StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);
					for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++) {
						sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
						for(j=0;(j<Bytes2Samples(srcsa->blen) && (instance->curlen<instance->slen));j++,instance->curlen++) {
							if(*sbuf>maxvalp) maxvalp=*sbuf;
							else if(*sbuf<maxvaln) maxvaln=*sbuf;
							sbuf++;
						}
					}
					if(ProWinAborted(instance->curlen)) l=CH_MAX+1;                  // es wurde abgebrochen
					SADone(srcsa,srcbuf);
				}
				else l=CH_MAX+1;
			}
		}
		DelProWin(pwd);
		if(l>CH_MAX) return(FALSE);
	}
	else return(FALSE);
	if(instance->maxvolu && maxvalp>0) {
		// old version, seems to overdrive sometimes, due IEEE errors
		//sprintf(param,"%6.4lf",32767.0/(double)maxvalp);
		sprintf(param,"%6.4lf",32766.0/(double)maxvalp);
		srctyp=UC_GetParameterUnit(instance->mod1.vals,UGRP_AMPLITUDE);
		UC_ConvertParameter(instance->mod1.vals,param,UGRP_AMPLITUDE,srctyp);
		UC_ConvertParameter(instance->mod1.vale,param,UGRP_AMPLITUDE,srctyp);
		instance->mod1.bshape=BSHP_NONE;
	}
	if(instance->maxvoll && maxvaln<0) {
		// old version, seems to overdrive sometimes, due IEEE errors
		//sprintf(param,"%6.4lf",-32768.0/(double)maxvaln);
		sprintf(param,"%6.4lf",-32767.0/(double)maxvaln);
		srctyp=UC_GetParameterUnit(instance->mod2.vals,UGRP_AMPLITUDE);
		UC_ConvertParameter(instance->mod2.vals,param,UGRP_AMPLITUDE,srctyp);
		UC_ConvertParameter(instance->mod2.vale,param,UGRP_AMPLITUDE,srctyp);
		instance->mod2.bshape=BSHP_NONE;
	}
	return(TRUE);
}

//-- eof ----------------------------------------------------------------------
