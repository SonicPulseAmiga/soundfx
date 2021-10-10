/****h* Amplify/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (03.06.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Amplify Operator module for SoundFX
*  AUTHOR
*    Stefan Kost            sk      kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    03.Jun.2002    V 4.2   most recent version
*    25.Apr.2001    V 4.1   most recent version
*    01.Aug.2000    V 1.2   most recent version
*                           * fixed maxvol-bug
*    19.Jul.1998    V 1.2   initial version
*  NOTES
*
*******
*/

#define AMPLIFY_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Amplify.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

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

#define GADID_MOD       (GADID_SRC+SRC_IDCT)
#define GADIX_MOD       (GADIX_SRC+SRC_IXCT)

#define GADID_MAXVOL    (GADID_MOD+MOD_IDCT)
#define GADIX_MAXVOL    (GADIX_MOD+MOD_IXCT)

#define GADID_WRAPMODE  (GADID_MAXVOL+1)
#define GADIX_WRAPMODE  (GADIX_MAXVOL+1)

#define LAST_GADIX      (GADIX_WRAPMODE+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[3];
};

UBYTE *WrapLabels[]=WRAP_LABELS;

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
	if(cfg)             CfgIO_RemConfig(cfg);
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
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;
	struct Requester NullReq;	//-- window input blocker

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge      =xs1+RunTime->scx[(308-100)>>1];
		ng.ng_TopEdge       =ys4+RunTime->scy[2];
		ng.ng_Width         =RunTime->scx[100];
		ng.ng_Height        =RunTime->scy[11];
		ng.ng_GadgetText    =GetString(btnProcess);
		ng.ng_TextAttr      =RunTime->ScrFontB;
		ng.ng_GadgetID      =GADID_GO;
		ng.ng_Flags         =PLACETEXT_IN;
		ng.ng_VisualInfo    =RunTime->VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_GO]=g;

		InitCfgSel(&cfgsel,PROGDIR"_operators/"PRJ_NAME);
		g=AddCfgSel(&cfgsel,modGadgets,g,GADID_CFGSEL,GADIX_CFGSEL,xs2,ys1,winh-2);

		g=AddSource(&(instance->src),modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		g=AddModulator(&(instance->mod),modGadgets,g,GADID_MOD,GADIX_MOD,xs1,ys2);

		ng.ng_LeftEdge      =xs1+RunTime->scx[37];
		ng.ng_TopEdge       =ys3+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width         =RunTime->scx[80];
		ng.ng_GadgetText    ="MaxVol";
		ng.ng_TextAttr      =RunTime->ScrFont;
		ng.ng_GadgetID      =GADID_MAXVOL;
		ng.ng_Flags         =PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_MAXVOL]=g;

		ng.ng_LeftEdge      =xs1+RunTime->scx[214];
		ng.ng_Width         =RunTime->scx[90];
		ng.ng_GadgetText    ="Wrap";
		ng.ng_GadgetID      =GADID_WRAPMODE;
		ng.ng_Flags         =PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,WrapLabels,GTCY_Active,instance->wrap,TAG_DONE);
		modGadgets[GADIX_WRAPMODE]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"        ,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Amplification" ,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous" ,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"       ,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);                  /* Source */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);                  /* Par */
			DrawGadWB(modrp,xs1,ys3,xh,ANYFRM_HEIGHT(1));               /* Misc */
			DrawGadWB(modrp,xs1,ys4,xh,CMDFRM_HEIGHT);                  /* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);        /* CfgSel */

			SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
			SetModulator(&(instance->mod),modWin,modGadgets,GADIX_MOD);

			if(!enabled) {
				GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOL],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
			}

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			InitRequester(&NullReq);	/* BlockReq initialisieren */

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=TRUE;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOL],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						instance->src.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src.rmode=SetRngMode(instance->src.srcbuf);
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=FALSE;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
						GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOL],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					}
					SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
				}
				if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(!HandleSource   (              &(instance->src),   modWin,modGadgets,GADID_SRC,   GADIX_SRC,   gnr,icode)
								&& !HandleModulator(&(instance->mod),&(instance->src),0l,modWin,modGadgets,GADID_MOD,   GADIX_MOD,   gnr,icode)
								&& !HandleCfgSel   (&cfgsel      ,                 modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_MAXVOL:
											instance->maxvol=TRUE;
											if(instance->src.srcbuf) {
												Request(&NullReq,modWin);
												SetWindowPointer(modWin,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
												if(GetMaxVol(instance)) SetModulator(&(instance->mod),modWin,modGadgets,GADIX_MOD);
												EndRequest(&NullReq,modWin);
												SetWindowPointer(modWin,TAG_DONE);
											}
											break;
										case GADID_WRAPMODE:
											instance->wrap=icode;
											break;
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
	UBYTE l;   // channels
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

	INTRO;

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:             // wir wurden e.g. per ARexx gestartet
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
	SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);       // gibt den zu bearbeitenden Bereich zurück
	instance->slen=rm_len;
	instance->srat=srcbuf->srat;

	if(instance->maxvol && (pdata->callmd==CM_DIRECT)) {
		if(!GetMaxVol(instance)) return(FALSE);							   // maxvol suchen (wenn von rexx aus gestartet)
	}

	UC_ConvertParameter(param,instance->mod.vals,UGRP_AMPLITUDE,AMP_FC); // e.g. Lautstärke von was-auch-immer (db/%/%%/Faktor/...) nach Faktor
	instance->amps=atof(param);
	UC_ConvertParameter(param,instance->mod.vale,UGRP_AMPLITUDE,AMP_FC);
	instance->ampe=atof(param);
	instance->ampd=instance->ampe-instance->amps;                        // Modulationsdiffernz

	if(!dstbuf) {
		if(dstbuf=NewSample()) {                                            // wir benötigen einen neuen Samplepuffer für das Resultat
			if(AllocSampleMem(dstbuf,srcbuf->slen,CH_COUNT(srcbuf->channels))) {      // dann noch etwas wo das Ergebnis hingespeichert werden kann
				BufferFill(srcbuf,dstbuf);                                          // kopieren die Buffereinstellungen des Quellsamples
				newdst=TRUE;
			}
			else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
	}
	if((pwd=NewProWin())) {                                     // ein neues asynchrones Statusfenster öffnen
		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;l<CH_COUNT(srcbuf->channels);l++) {                       // bearbeite jeden Kanal ...
				if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {                            // ... der aktive ist
					activate_ch((SFXCoreInstance *)instance,l);
					if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);    // Bereich von 0...Bereichsstart kopieren
																				// Die Samplezugriffshandles besorgen
					srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);    // sequentiell Blöcke konstanter Größe lesen
					dstsa=SAInit(dstbuf,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);    // sequentiell Blöcke konstanter Größe schreiben
					if(srcsa && dstsa) {                                        // hat's geklappt ?
						SARead =srcsa->safunc;                                  // die Zugriffsfunktionspointer cachen
						SAWrite=dstsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));     // Titel für Statusfenster
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);                  // den Statusbalken starten
						switch(instance->wrap) {                                 // je nach wrap-mode
							case WRAP_NOCLIP:                                       // und jetzt berechnen
								for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) { // für jedes Segment
									instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);    // Pointer auf src-Block holen & nächstes Read starten
									instance->dbuf=(SAMPLE *)dstsa->buffer1;                              // Pointer auf dst-Block
									process_noclip((SFXCoreInstance *)instance,blen);
									SAWrite(dstsa,dstsa->blen);                                    // akt Block schreiben
								}
								break;
							case WRAP_CLIP:
								for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
									instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
									instance->dbuf=(SAMPLE *)dstsa->buffer1;
									process_clip((SFXCoreInstance *)instance,blen);
									SAWrite(dstsa,dstsa->blen);
								}
								break;
							case WRAP_WRAP1:
								for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
									instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
									instance->dbuf=(SAMPLE *)dstsa->buffer1;
									process_wrap1((SFXCoreInstance *)instance,blen);
									SAWrite(dstsa,dstsa->blen);
								}
								break;
							case WRAP_WRAP2:
								for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
									instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
									instance->dbuf=(SAMPLE *)dstsa->buffer1;
									process_wrap2((SFXCoreInstance *)instance,blen);
									SAWrite(dstsa,dstsa->blen);
								}
								break;
							//default:
								//break;
						}
					}
					else okay=FALSE;
					if(srcsa) SADone(srcsa,srcbuf);                 // Samplezugiffshandles freigeben
					if(dstsa) SADone(dstsa,dstbuf);
																	// Bereich von Bereichsende..Samplenende kopieren
					if(rm_start+rm_len<srcbuf->slen) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }			// es wurde abgebrochen
				}
				else CopyDataCh(srcbuf,dstbuf,l,0,0,srcbuf->slen);  // den kompletten kanal unverändert übernehmen
			}
			if(okay && newdst) AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);      // das Sample öffentlich zugänglich machen (Fenster öffnen, in Liste aufnehmen)
		}
		else okay=FALSE;
		deactivate((SFXCoreInstance *)instance);
		DelProWin(pwd);                                         // das Statusbalkenfenster wieder freigeben
	}
	else okay=FALSE;
	if(!okay) {
		if(newdst) RemSample(dstbuf);
		return(FALSE);
	}
	OUTRO;
	return(TRUE);                                                    // everything went right
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;pf=ParseModulatorSet(&(instance->mod),"P1",params);
		if(!pf && !stricmp(RexxPar2,"MaxVol"))  { instance->maxvol=atoi(RexxPar3)&1;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Wrap"))    { instance->wrap  =CfgIO_MatchString(RexxPar3,WrapLabels);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=ParseModulatorGet(&(instance->mod),"P1",params,ret);
		if(!pf && !stricmp(RexxPar2,"MaxVol"))  { sprintf(ret,"%d",instance->maxvol);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Wrap"))    { sprintf(ret,"%s",WrapLabels[instance->wrap]);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	instance->wrap  =CfgIO_MatchString(
					 CfgIO_ReadString           (cfg,"General"  ,"Wrap"     ,WrapLabels[WRAP_NOCLIP]),WrapLabels);
	instance->maxvol=CfgIO_ReadBool             (cfg,"General"  ,"MaxVol"   ,TRUE);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString         (cfg,"General"    ,"Wrap"     ,WrapLabels[instance->wrap]);
	CfgIO_WriteBool           (cfg,"General"    ,"MaxVol"   ,instance->maxvol);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=NULL;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod.vals,"1.0",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->mod.vale,"1.0",UGRP_AMPLITUDE,AMP_FC);
	instance->mod.bshape=BSHP_NONE;
	instance->mod.desc[0]='\0';

	instance->wrap=WRAP_NOCLIP;
	instance->maxvol=FALSE;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
	SetModulator(&(instance->mod),modWin,modGadgets,GADIX_MOD);
	GT_SetGadgetAttrs(modGadgets[GADIX_WRAPMODE],modWin,0l,GTCY_Active,instance->wrap,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

UBYTE __inline GetMaxVol(struct Instance *instance) {
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
	ULONG rm_start,rm_len;
	Param param;
	UBYTE srctyp;
	UBYTE okay=TRUE;

	if((pwd=NewProWin())) {                 // ein neues asynchrones Statusfenster öffnen
		SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);       // gibt den zu bearbeitenden Bereich zurück
		instance->slen=rm_len;

		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;(okay && (l<(CH_COUNT(srcbuf->channels))));l++) {
				if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {
					activate_ch((SFXCoreInstance *)instance,l);
					if(srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN)) {   // sequentiell Blöcke konstanter Größe lesen
						SARead =srcsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME"/MaxVol",l+1,CH_COUNT(srcbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);
						for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++) {
							instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
							process_pre((SFXCoreInstance *)instance,blen);
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
	if(okay && (instance->maxvalp || instance->maxvaln)) {
		ModUser *mus;

		instance->maxvaln=-instance->maxvaln;
		// old version, seems to overdrive sometimes, due IEEE errors
		//if(instance->maxvaln==32768 || instance->maxvalp==32767) strcpy(param,"1.0");
		//else sprintf(param,"%6.4lf",32767.0/(double)(max(instance->maxvalp,instance->maxvaln)));
		sprintf(param,"%6.4lf",32766.0/(double)(max(instance->maxvalp,instance->maxvaln)));
		srctyp=UC_GetParameterUnit(instance->mod.vals,UGRP_AMPLITUDE);
		UC_ConvertParameter(instance->mod.vals,param,UGRP_AMPLITUDE,srctyp);
		UC_ConvertParameter(instance->mod.vale,param,UGRP_AMPLITUDE,srctyp);
		// free old modulator
		switch(instance->mod.bshape) {
			case BSHP_USER:
				mus=(ModUser *)(instance->mod.bshpdata);
				UnlockBuffer(mus->modbuf);
				//if(mus->envdata) FreeVec(mus->envdata);
				break;
		}
		if(instance->mod.bshpdata) { FreeVec(instance->mod.bshpdata);instance->mod.bshpdata=NULL; }
		// set new modulator
		instance->mod.bshape=BSHP_NONE;
	}
	return(okay);
}

//-- eof ----------------------------------------------------------------------
