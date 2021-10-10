/****h* DeCrackle/lib-soundfx.c [4.3] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.3 (05.02.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Decrackle Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    05.Feb.2004	V 4.3	most recent version
*    29.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define DECRACKLE_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "DeCrackle.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

UBYTE TestDeCrackle(struct Instance *instance);

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

#define GADID_DIF		(GADID_SRC+SRC_IDCT)
#define GADIX_DIF		(GADIX_SRC+SRC_IXCT)

#define GADID_AMP		(GADID_DIF+1)
#define GADIX_AMP		(GADIX_DIF+1)

#define GADID_ADJUST	(GADID_AMP+1)
#define GADIX_ADJUST	(GADIX_AMP+1)

#define GADID_CRSIZE	(GADID_ADJUST+1)
#define GADIX_CRSIZE	(GADIX_ADJUST+1)

#define GADID_TEST		(GADID_CRSIZE+1)
#define GADIX_TEST		(GADIX_CRSIZE+1)

#define GADID_TESTRES	(GADID_TEST+1)
#define GADIX_TESTRES	(GADIX_TEST+1)

#define LAST_GADIX		(GADIX_TESTRES+1)

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
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(2);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;
	char strTestRes[CH_MAX*15];
	struct Requester NullReq;	//-- window input blocker

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

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Dif.";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_DIF;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_threshd,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DIF]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[224];
		ng.ng_GadgetText	="Adjust";
		ng.ng_GadgetID		=GADID_ADJUST;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_adj,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_ADJUST]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Amp.";
		ng.ng_GadgetID		=GADID_AMP;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_thresha,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMP]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[224];
		ng.ng_GadgetText	="Size";
		ng.ng_GadgetID		=GADID_CRSIZE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_cracklesize,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_CRSIZE]=g;


		ng.ng_LeftEdge      =xs1+RunTime->scx[53];
		ng.ng_TopEdge       =ys3+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width         =RunTime->scx[60];
		ng.ng_GadgetText    ="Test";
		ng.ng_TextAttr      =RunTime->ScrFont;
		ng.ng_GadgetID      =GADID_TEST;
		ng.ng_Flags         =PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_TEST]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge       =ys3+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width         =RunTime->scx[STDFRM_WIDTH-(53+4)];
		ng.ng_GadgetText	="Stat.";
		ng.ng_GadgetID		=GADID_TESTRES;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,NULL,TAG_DONE);
		modGadgets[GADIX_TESTRES]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Sensitivity"  	,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Dry-Run"       ,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(2));				/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,ANYFRM_HEIGHT(2));				/* Par */
			DrawGadWB(modrp,xs1,ys4,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);

			if(!enabled) {
				GT_SetGadgetAttrs(modGadgets[GADIX_GO  ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(modGadgets[GADIX_TEST],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
			}

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			InitRequester(&NullReq);	/* BlockReq initialisieren */

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=1;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO  ],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_TEST],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						instance->src.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src.rmode=SetRngMode(instance->src.srcbuf);
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=0;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO  ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_TEST],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					}
					SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
				}
				if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(!HandleSource(&(instance->src),modWin,modGadgets,GADID_SRC,   GADIX_SRC,   gnr,icode)
								&& !HandleCfgSel(&cfgsel      ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_DIF:
											HandleParameter(instance->p_threshd,modWin,modGadgets[GADIX_DIF]);
											break;
										case GADID_AMP:
											HandleParameter(instance->p_thresha,modWin,modGadgets[GADIX_AMP]);
											break;
										case GADID_ADJUST:
											HandleParameter(instance->p_adj,modWin,modGadgets[GADIX_ADJUST]);
											break;
										case GADID_CRSIZE:
											HandleParameter(instance->p_cracklesize,modWin,modGadgets[GADIX_CRSIZE]);
											break;
										case GADID_TEST:
											if(instance->src.srcbuf) {
												Request(&NullReq,modWin);
												SetWindowPointer(modWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
												if(TestDeCrackle(instance)) {
													/** @todo display as '%' as well */
													sprintf(strTestRes,"%6ld (%3d%%) %6ld (%3d%%) %6ld (%3d%%) %6ld (%3d%%)",
														instance->crackle_cts[0],(WORD)((instance->crackle_cts[0]*100.0)/instance->slen),
														instance->crackle_cts[1],(WORD)((instance->crackle_cts[1]*100.0)/instance->slen),
														instance->crackle_cts[2],(WORD)((instance->crackle_cts[2]*100.0)/instance->slen),
														instance->crackle_cts[3],(WORD)((instance->crackle_cts[3]*100.0)/instance->slen));
													GT_SetGadgetAttrs(modGadgets[GADIX_TESTRES],modWin,0l,GTTX_Text,strTestRes,GTTX_CopyText,TRUE,TAG_DONE);
												}
												EndRequest(&NullReq,modWin);
												SetWindowPointer(modWin,TAG_DONE);
											}
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_DIF],modWin,0l);
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

	srcbuf=instance->src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);
	instance->slen=rm_len;
	instance->srat=srcbuf->srat;

	UC_ConvertParameter(param,instance->p_threshd,UGRP_PROPOTION,PROP_FC);			// Schwellwert Deltas
	instance->threshd=1.0+atof(param);

	UC_ConvertParameter(param,instance->p_thresha,UGRP_PROPOTION,PROP_FC);			// Schwellwert Amplitude
	instance->thresha=1.0+atof(param);

	UC_ConvertParameter(param,instance->p_adj,UGRP_PROPOTION,PROP_FC);			// Anpassung
	instance->adj=atof(param);

	if(UC_GetParameterUnit(instance->p_cracklesize,UGRP_ABSTIME)==ATME_SAMPLES) {
		instance->cracklesize=atof(instance->p_cracklesize);
	}
	else {
		UC_ConvertParameter(param,instance->p_cracklesize,UGRP_ABSTIME,ATME_MS);
		instance->cracklesize=atof(param)*((double)srcbuf->srat/1000.0);
	}
	if(instance->cracklesize<2) instance->cracklesize=2;						// check that range is not empty
	if(instance->cracklesize>srcbuf->slen) instance->cracklesize=srcbuf->slen;	// and that our range isn't larger than the whole thing

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
						instance->dbuf=(SAMPLE *)dstsa->buffer1;									// Pointer auf dst-Block
						//for(srcsa->seg=dstsa->seg=0;(((srcsa->seg<srcsa->seganz) || (dstsa->seg<dstsa->seganz)) && (instance->curlen<instance->slen));) {
						for(srcsa->seg=dstsa->seg=0;instance->curlen<instance->slen;) {
							//MSG1("curlen=%8ld",instance->curlen);
							if(instance->spos==0) {
								if(srcsa->seg==srcsa->seganz) break;
								instance->sbuf=(SAMPLE *)srcsa->buffer1;			// Pointer auf src-Block holen
								instance->spos=blen;
								SARead(srcsa,srcsa->blen);srcsa->seg++;				// nächstes Read starten
							}
							//MSG2("< spos=%7ld dpos=%7ld",instance->spos,instance->dpos);
							process((SFXCoreInstance *)instance,blen);
							//MSG2("> spos=%7ld dpos=%7ld",instance->spos,instance->dpos);
							if(instance->dpos==blen) {
								if(dstsa->seg==dstsa->seganz) break;
								SAWrite(dstsa,dstsa->blen);dstsa->seg++;			// akt Block schreiben
								instance->dbuf=(SAMPLE *)dstsa->buffer1;			// Pointer auf dst-Block
								instance->dpos=0;
							}
						}
						//MSG6("curlen/slen=%8ld/%8ld  scrseg=%ld/%ld  dstseg=%ld/%ld",instance->curlen,instance->slen,srcsa->seg,srcsa->seganz,dstsa->seg,dstsa->seganz);
						if(!ProWinAborted(instance->curlen)) {											// process rest (because of cracklesize delay)
							instance->rest=instance->cracklesize;
							while(instance->rest>0) {
								//MSG2("curlen=%ld,rest=%ld",instance->curlen,instance->rest);
								process_rest((SFXCoreInstance *)instance,blen);
								if(instance->dpos) { 
									SAWrite(dstsa,dstsa->blen);dstsa->seg++;				// akt Block schreiben
									instance->dbuf=(SAMPLE *)dstsa->buffer1;				// Pointer auf dst-Block
									instance->dpos=0;
								}
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
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"Dif"))			{ strntcpy(instance->p_threshd,		RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Amp"))			{ strntcpy(instance->p_thresha,		RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Adjust"))		{ strntcpy(instance->p_adj,			RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Size"))		{ strntcpy(instance->p_cracklesize,	RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"Dif"))			{ strntcpy(ret,instance->p_threshd,		PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Amp"))			{ strntcpy(ret,instance->p_thresha,		PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Adjust"))		{ strntcpy(ret,instance->p_adj,			PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Size"))		{ strntcpy(ret,instance->p_cracklesize,	PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_threshd		,CfgIO_ReadString	(cfg,"General"	,"Dif"		,"80 %"));
	strcpy(instance->p_thresha		,CfgIO_ReadString	(cfg,"General"	,"Amp"		,"30 %"));
	strcpy(instance->p_adj			,CfgIO_ReadString	(cfg,"General"	,"Adjust"	,"90 %"));
	strcpy(instance->p_cracklesize	,CfgIO_ReadString	(cfg,"General"	,"Size"		,"2 ms"));
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Dif"		,instance->p_threshd);
	CfgIO_WriteString		  (cfg,"General"	,"Amp"		,instance->p_thresha);
	CfgIO_WriteString		  (cfg,"General"	,"Adjust"	,instance->p_adj);
	CfgIO_WriteString		  (cfg,"General"	,"Size"		,instance->p_cracklesize);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->p_threshd,	"1000 %",UGRP_PROPOTION,PROP_PROZ);
	UC_ConvertParameter(instance->p_thresha,	"1000 %",UGRP_PROPOTION,PROP_PROZ);
	UC_ConvertParameter(instance->p_adj,		"90 %",UGRP_PROPOTION,PROP_PROZ);
	UC_ConvertParameter(instance->p_cracklesize,"4 ms",UGRP_ABSTIME,ATME_MS);
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_DIF],modWin,0l,GTST_String,instance->dif,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_AMP],modWin,0l,GTST_String,instance->amp,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_ADJUST],modWin,0l,GTST_String,instance->adj,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

UBYTE __inline TestDeCrackle(struct Instance *instance) {
	//--loop
	UBYTE l;
	//-- prowin
	PWData *pwd;
	ProWinTitle title;
	//-- src & dst buffers
	SInfo *srcbuf=instance->src.srcbuf;
	//-- buffer access
	SmpAccess *srcsa;
	SAFuncPtr SARead;
	ULONG blen;
	//-- local
	Param param;
	ULONG rm_start,rm_len;
	UBYTE okay=TRUE;

	INTRO;

	if((pwd=NewProWin())) {                 // ein neues asynchrones Statusfenster öffnen
		SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);       // gibt den zu bearbeitenden Bereich zurück
		instance->slen=rm_len;
		instance->srat=srcbuf->srat;

		UC_ConvertParameter(param,instance->p_threshd,UGRP_PROPOTION,PROP_FC);			// Schwellwert Deltas
		instance->threshd=1.0+atof(param);

		UC_ConvertParameter(param,instance->p_thresha,UGRP_PROPOTION,PROP_FC);			// Schwellwert Amplitude
		instance->thresha=1.0+atof(param);

		MSG2("threshd=%6.4lf  thresha=%6.4lf",instance->threshd,instance->thresha);

		UC_ConvertParameter(param,instance->p_adj,UGRP_PROPOTION,PROP_FC);			// Anpassung
		instance->adj=atof(param);

		if(UC_GetParameterUnit(instance->p_cracklesize,UGRP_ABSTIME)==ATME_SAMPLES) {
			instance->cracklesize=atof(instance->p_cracklesize);
		}
		else {
			UC_ConvertParameter(param,instance->p_cracklesize,UGRP_ABSTIME,ATME_MS);
			instance->cracklesize=atof(param)*((double)srcbuf->srat/1000.0);
		}
		if(instance->cracklesize<2) instance->cracklesize=2;						// check that range is not empty
		if(instance->cracklesize>srcbuf->slen) instance->cracklesize=srcbuf->slen;	// and that our range isn't larger than the whole thing

		MSG1("max cracklesize=%d",instance->cracklesize);

		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;(okay && (l<(CH_COUNT(srcbuf->channels))));l++) {
				if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {
					activate_ch((SFXCoreInstance *)instance,l);
					if(srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN)) {   // sequentiell Blöcke konstanter Größe lesen
						MSG1("  scanning channel %d",l);
						SARead =srcsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME"/Test",l+1,CH_COUNT(srcbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);
						for(srcsa->seg=0;instance->curlen<instance->slen;) {
							if(instance->spos==0) {
								if(srcsa->seg==srcsa->seganz) break;
								instance->sbuf=(SAMPLE *)srcsa->buffer1;			// Pointer auf src-Block holen
								instance->spos=blen;
								SARead(srcsa,srcsa->blen);srcsa->seg++;				// nächstes Read starten
							}
							process_dry((SFXCoreInstance *)instance,blen);
							if(instance->dpos==blen) instance->dpos=0;
						}
						if(!ProWinAborted(instance->curlen)) {											// process rest (because of cracklesize delay)
							instance->rest=instance->cracklesize;
							while(instance->rest>0) {
								process_dry_rest((SFXCoreInstance *)instance,blen);
								if(instance->dpos) instance->dpos=0;
							}
						}
						SADone(srcsa,srcbuf);
					}
					else okay=FALSE;
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) okay=FALSE;                  // es wurde abgebrochen
				}
			}
		}
		deactivate((SFXCoreInstance *)instance);
		DelProWin(pwd);
	}
	else okay=FALSE;
	return(okay);
}

//-- eof ----------------------------------------------------------------------
