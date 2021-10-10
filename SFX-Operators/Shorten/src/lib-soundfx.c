/****h* Shorten/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (31.10.01) � by Stefan Kost 1998-2001
*  FUNCTION
*    Shorten Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    31.Oct.2001	V 4.2	most recent version
*    15.Mar.2001	V 4.1	most recent version
*							A: ThreshE
*    01.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  IDEAS
*    * Parameter : BorderS/E - keep some space?
*  NOTES
*    * was mache ich, wenn nicht alle Kan�le bearbeitet werden ?
*      -> einfach nur in aktiven Kan�len suchen, aber alle beschneiden ?
*******
*/

#define SHORTEN_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Shorten.h"

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

#define GADID_THRESHS	(GADID_SRC+SRC_IDCT)
#define GADIX_THRESHS	(GADIX_SRC+SRC_IXCT)

#define GADID_THRESHE	(GADID_THRESHS+1)
#define GADIX_THRESHE	(GADIX_THRESHS+1)

#define LAST_GADIX		(GADIX_THRESHE+1)

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
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) � by Stefan Kost 1993-%s"))) goto Error;
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
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(1);
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

		ng.ng_LeftEdge		=xs1+RunTime->scx[64];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="ThreshS";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_THRESHS;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->threshs,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_THRESHS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="ThreshE";
		ng.ng_GadgetID		=GADID_THRESHE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->threshe,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_THRESHE]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Threshold"		,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(1));				/* Par */
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
										case GADID_THRESHS:
											HandleParameter(instance->threshs,modWin,modGadgets[GADIX_THRESHS]);
											break;
										case GADID_THRESHE:
											HandleParameter(instance->threshe,modWin,modGadgets[GADIX_THRESHE]);
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_THRESHS],modWin,0l);
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
	SmpAccess *srcsa;
	SAFuncPtr SARead;
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

	UC_ConvertParameter(param,instance->threshs,UGRP_AMPLITUDE,AMP_LEVEL);
	instance->tvals=atoi(param);
	UC_ConvertParameter(param,instance->threshe,UGRP_AMPLITUDE,AMP_LEVEL);
	instance->tvale=atoi(param);

	instance->rngs=rm_start+rm_len;instance->rnge=rm_start;	// nearly impossible

	if((pwd=NewProWin())) {					// ein neues asynchrones Statusfenster �ffnen
		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
				if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {
					activate_ch((SFXCoreInstance *)instance,l);
					if((srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {	// sequentiell Bl�cke konstanter Gr��e lesen
						SARead =srcsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
						for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++) {
							instance->sbuf=(SAMPLE *)srcsa->buffer1;
							SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & n�chstes Read starten
							process((SFXCoreInstance *)instance,blen);
						}
						SADone(srcsa,srcbuf);						// Samplezugiffshandles freigeben
					}
					else okay=FALSE;
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }										  // es wurde abgebrochen
					instance->rngst+=rm_start;instance->rnget+=rm_start;
					instance->rngs=min(instance->rngs,instance->rngst);		// choose the earliest start
					instance->rnge=max(instance->rnge,instance->rnget);		// choose the latest end
				}
			}
			if(okay && (instance->rngs<instance->rnge)) {		// es wurde nicht abgebrochen
				if(dstbuf=NewSample()) {
					ULONG diff;
					BufferFill(srcbuf,dstbuf);
					diff=(instance->rngs-rm_start)+((rm_start+rm_len)-instance->rnge);
					dstbuf->slen		=srcbuf->slen-diff;
					if(dstbuf->loop && instance->rngs>dstbuf->loops) {
						dstbuf->loops=srcbuf->loops-instance->rngs;
						if(dstbuf->loops<0) dstbuf->loops=0;
						if(dstbuf->loops+dstbuf->loopl>dstbuf->slen) dstbuf->loopl=dstbuf->slen-dstbuf->loops;
					}
					if(dstbuf->mark && instance->rngs>dstbuf->markxs) {
						dstbuf->markxs		=srcbuf->markxs-instance->rngs;
						if(dstbuf->markxs<0) dstbuf->markxs=0;
						if(dstbuf->markxs+dstbuf->markxl>dstbuf->slen) dstbuf->markxl=dstbuf->slen-dstbuf->markxs;
					}
					dstbuf->zoomxs		=0;
					dstbuf->zoomxl		=dstbuf->slen;
					if(AllocSampleMem(dstbuf,dstbuf->slen,CH_COUNT(srcbuf->channels))) {
						newdst=TRUE;
						for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
							//sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));
							//StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
							if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {							// ... der aktive ist
								if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);	// Bereich von 0...Bereichsstart kopieren
								CopyDataCh(srcbuf,dstbuf,l,instance->rngs,rm_start,(instance->rnge-instance->rngs));		// Mittenbereich  �bernehmen
								if(rm_start+rm_len<srcbuf->slen) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
							}
							else CopyDataCh(srcbuf,dstbuf,l,0,0,dstbuf->slen);	// den kompletten kanal unver�ndert �bernehmen (L�nge ?)
						}
						AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);
					}
					else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);okay=FALSE;dstbuf=NULL; }
				}
				else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);okay=FALSE; }
			}
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
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"ThreshS"))		{ strntcpy(instance->threshs,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf &&  !stricmp(RexxPar2,"ThreshE"))		{ strntcpy(instance->threshe,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"ThreshS"))		{ strntcpy(ret,instance->threshs,PARBUFLEN);pf=TRUE; }
		if(pf &&  !stricmp(RexxPar2,"ThreshE"))		{ strntcpy(ret,instance->threshe,PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->threshs,CfgIO_ReadString	(cfg,"General"	,"Thresh"	,instance->threshs));
	strcpy(instance->threshe,CfgIO_ReadString	(cfg,"General"	,"Thresh"	,instance->threshe));
	strcpy(instance->threshs,CfgIO_ReadString	(cfg,"General"	,"ThreshS"	,instance->threshs));
	strcpy(instance->threshe,CfgIO_ReadString	(cfg,"General"	,"ThreshE"	,instance->threshe));
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"ThreshS"	,instance->threshs);
	CfgIO_WriteString		  (cfg,"General"	,"ThreshE"	,instance->threshe);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->threshs,"2 %%",UGRP_AMPLITUDE,AMP_ABSPROM);
	UC_ConvertParameter(instance->threshe,"1 %%",UGRP_AMPLITUDE,AMP_ABSPROM);
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_THRESHS],modWin,0l,GTST_String,instance->threshs,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_THRESHE],modWin,0l,GTST_String,instance->threshe,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
