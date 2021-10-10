/****h* PitchShift/lib-soundfx.c [4.3] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.3 (16.02.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    PitchShift Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    16.Feb.2004	V 4.3	most recent version
*    05.Nov.2001	V 4.2	most recent version
*    04.Jan.2001	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*  IDEAS
*    * Formant-correction via FormatDefinitionFiles
*    * dynamic windowing (for modulated fx), real_winsize=winsize/pitch;
*    * Dynamic-correction
*      * generate an volume-envelope of the source (not that easy in this case)
*        * we need the envelope at readpos and writepos
*      * before writing a value to destination normalise it : val=val/env[readpos]
*      * and apply the valume of writepos : val=val*env[writepos]
*      * short : val*=(env[writepos]/env[readpos]);
*    * Tone-correction
*	   * like dynamic-correction
*      * pitch-analysis is still problematic
*    * Spectral correction
*      * spectral envelopes
*    * skip-back-seeking
*      * joint-channel processing
*      * autocorrellation
*
*******
*/

#define PITCHSHIFT_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "PitchShift.h"

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

#define GADID_WINSIZE	(GADID_MOD2+MOD_IDCT)
#define GADIX_WINSIZE	(GADIX_MOD2+MOD_IXCT)

#define GADID_SMOOTH	(GADID_WINSIZE+1)
#define GADIX_SMOOTH	(GADIX_WINSIZE+1)

#define GADID_INTER		(GADID_SMOOTH+1)
#define GADIX_INTER		(GADIX_SMOOTH+1)

#define GADID_CHANLINK	(GADID_INTER+INT_IDCT)
#define GADIX_CHANLINK	(GADIX_INTER+INT_IXCT)

#define LAST_GADIX		(GADIX_CHANLINK+1)

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
		SFXMod_ModDescInit(0,PD_SOURCE      ,"S1",&(instance->src));
		SFXMod_ModDescInit(1,PD_MODULATOR   ,"M1",&(instance->mod1));
		SFXMod_ModDescInit(2,PD_MODULATOR   ,"M2",&(instance->mod2));
		SFXMod_ModDescInit(3,PD_INTERPOLATOR,"I1",&(instance->int1));
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
	UWORD ys3=ys2+1+MODSEL_HEIGHT;
	UWORD ys4=ys3+1+MODSEL_HEIGHT;
	UWORD ys5=ys4+1+ANYFRM_HEIGHT(2);
	UWORD winh=ys5+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

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

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];					/* andere */
		ng.ng_TopEdge		=ys4+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Window";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_WINSIZE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_winsize,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_WINSIZE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[224];
		ng.ng_GadgetText	="Smooth";
		ng.ng_GadgetID		=GADID_SMOOTH;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_smooth,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_SMOOTH]=g;

		g=AddInterpolator(&(instance->int1),modGadgets,g,GADID_INTER,GADIX_INTER,xs1,ys4+RunTime->scy[FRMCONTENT_YOFFSET+12]);

		ng.ng_LeftEdge		=xs1+RunTime->scx[278];
		ng.ng_TopEdge		=ys4+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime->scx[26];
		ng.ng_GadgetText	="ChannelLink";
		ng.ng_GadgetID		=GADID_CHANLINK;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,instance->chanlink,GTCB_Scaled,TRUE,TAG_DONE);
		modGadgets[GADIX_CHANLINK]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Interp.",xs1+RunTime->scx[45],ys4+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"			,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Effect"			,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"PitchShift Factor"	,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"		,xs1,ys4,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"			,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Eff */
			DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);					/* Pitch */
			DrawGadWB(modrp,xs1,ys4,xh,ANYFRM_HEIGHT(2));				/* Par */
			DrawGadWB(modrp,xs1,ys5,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel      (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource      (&(instance->src ),modWin,modGadgets,GADIX_SRC);
			SetModulator   (&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
			SetModulator   (&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
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
								if(!HandleSource      (&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)
								&& !HandleModulator   (&(instance->mod1),&(instance->src),0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode)
								&& !HandleModulator   (&(instance->mod2),&(instance->src),0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode)
								&& !HandleInterpolator(&(instance->int1),modWin,modGadgets,GADID_INTER,GADIX_INTER,gnr,icode)
								&& !HandleCfgSel      (&cfgsel       ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_WINSIZE:
											HandleParameter(instance->p_winsize,modWin,modGadgets[GADIX_WINSIZE]);
											break;
										case GADID_SMOOTH:
											HandleParameter(instance->p_smooth,modWin,modGadgets[GADIX_SMOOTH]);
											break;
										case GADID_CHANLINK:
											instance->chanlink=((modGadgets[GADIX_CHANLINK]->Flags&GFLG_SELECTED)==GFLG_SELECTED);
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
	double smfc;

	INTRO;

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

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_RELFREQUENCY,RFRQ_FC);
	instance->pitchs=atof(param);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_RELFREQUENCY,RFRQ_FC);
	instance->pitche=atof(param);
	instance->pitchd=instance->pitche-instance->pitchs;
	// DEBUG
	MSG2("pitch : %6.4lf ... %6.4lf",instance->pitchs,instance->pitche);
	// DEBUG

	if(UC_GetParameterUnit(instance->p_winsize,UGRP_ABSTIME)==ATME_SAMPLES) {
		instance->winsize=atof(instance->p_winsize);
	}
	else {
		UC_ConvertParameter(param,instance->p_winsize,UGRP_ABSTIME,ATME_MS);
		instance->winsize=atof(param)*((double)srcbuf->srat/1000.0);
	}
	if(instance->winsize<10) instance->winsize=10;				// make sure we have some range to seek for an optimal continuation point
	if(instance->winsize>(srcbuf->slen>>2)) instance->winsize=(srcbuf->slen>>2);	// and that our range isn't the whole thing
	UC_ConvertParameter(param,instance->p_smooth,UGRP_PROPOTION,PROP_FC);
	smfc=atof(param);
	if(smfc<0.0) smfc=0.0;
	else if(smfc>1.0) smfc=1.0;
	instance->smrng=(ULONG)((double)instance->winsize*smfc*0.25);
	MSG1("smrng %ld",instance->smrng);

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
					srcsa=SAInit(srcbuf,l,0,srcbuf->slen,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					dstsa=SAInit(dstbuf,l,0,dstbuf->slen,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					if(srcsa && dstsa) {
						SARead =srcsa->safunc;
						SAWrite=dstsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
						instance->dbuf=(SAMPLE *)dstsa->buffer1;									// Pointer auf dst-Block
						if(instance->chanlink) {
							MSG("0 seeking is off");
							for(srcsa->seg=dstsa->seg=0;((srcsa->seg<=srcsa->seganz) && (instance->curlen<instance->slen));) {
								if(instance->spos==0) {
									instance->sbuf=(SAMPLE *)srcsa->buffer1;			// Pointer auf src-Block holen
									instance->spos=blen;
									SARead(srcsa,srcsa->blen);srcsa->seg++;				// nächstes Read starten
								}
								process_noseek((SFXCoreInstance *)instance,blen);
								if(instance->dpos==blen) {
									SAWrite(dstsa,dstsa->blen);dstsa->seg++;			// akt Block schreiben
									instance->dbuf=(SAMPLE *)dstsa->buffer1;			// Pointer auf dst-Block
									instance->dpos=0;
								}
							}
							if(!ProWinAborted(instance->curlen)) {											// process rest (because of winsize delay)
								instance->rest=instance->winsize;
								while(instance->rest>0) {
									process_noseek_rest((SFXCoreInstance *)instance,blen);
									if(instance->dpos) { 
										SAWrite(dstsa,dstsa->blen);dstsa->seg++;				// akt Block schreiben
										instance->dbuf=(SAMPLE *)dstsa->buffer1;				// Pointer auf dst-Block
										instance->dpos=0;
									}
								}
							}
						}
						else {
							MSG("1 seeking is on");
							for(srcsa->seg=dstsa->seg=0;((srcsa->seg<=srcsa->seganz) && (instance->curlen<instance->slen));) {
								if(instance->spos==0) {
									instance->sbuf=(SAMPLE *)srcsa->buffer1;			// Pointer auf src-Block holen
									instance->spos=blen;
									SARead(srcsa,srcsa->blen);srcsa->seg++;				// nächstes Read starten
								}
								process_seek((SFXCoreInstance *)instance,blen);
								if(instance->dpos==blen) {
									SAWrite(dstsa,dstsa->blen);dstsa->seg++;			// akt Block schreiben
									instance->dbuf=(SAMPLE *)dstsa->buffer1;			// Pointer auf dst-Block
									instance->dpos=0;
								}
							}
							if(!ProWinAborted(instance->curlen)) {											// process rest (because of winsize delay)
								instance->rest=instance->winsize;
								while(instance->rest>0) {
									process_seek_rest((SFXCoreInstance *)instance,blen);
									if(instance->dpos) {
										SAWrite(dstsa,dstsa->blen);dstsa->seg++;				// akt Block schreiben
										instance->dbuf=(SAMPLE *)dstsa->buffer1;				// Pointer auf dst-Block
										instance->dpos=0;
									}
								}
							}
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

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)){ 
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorSet   (&(instance->mod1),"P1",params);
		if(!pf) pf=ParseModulatorSet   (&(instance->mod2),"P2",params);
		if(!pf) pf=ParseInterpolatorSet(&(instance->int1),"I1",params);
		if(!pf && !stricmp(RexxPar2,"WinSize"))	{ strntcpy(instance->p_winsize,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Smooth"))	{ strntcpy(instance->p_smooth ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"ChanLink"))	{ instance->chanlink=atoi(RexxPar3)&0x1;pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet   (&(instance->mod1),"P1",params,ret);
		if(!pf) pf=ParseModulatorGet   (&(instance->mod2),"P2",params,ret);
		if(!pf) pf=ParseInterpolatorGet(&(instance->int1),"I1",params,ret);
		if(!pf && !stricmp(RexxPar2,"WinSize"))	{ strntcpy(ret,instance->p_winsize,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Smooth"))	{ strntcpy(ret,instance->p_smooth ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"ChanLink"))	{ sprintf(ret,"%d",instance->chanlink);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname){ 
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_winsize,CfgIO_ReadString	(cfg,"General"	,"WinSize"	,instance->p_winsize));
	strcpy(instance->p_smooth,	CfgIO_ReadString	(cfg,"General"	,"Smooth"	,instance->p_smooth));

	instance->chanlink	=CfgIO_ReadBool	   (cfg,"General"  ,"ChanLink",FALSE);
}

void SaveSettings(struct Instance *instance,char *fname){ 
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"WinSize"	,instance->p_winsize);
	CfgIO_WriteString		  (cfg,"General"	,"Smooth"	,instance->p_smooth);

	CfgIO_WriteBool			  (cfg,"General"    ,"ChanLink",instance->chanlink);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src.srcbuf=NULL;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod1.vals,"1.0",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod1.vale,"1.0",UGRP_PROPOTION,PROP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"1.5",UGRP_RELFREQUENCY,RFRQ_FC);
	UC_ConvertParameter(instance->mod2.vale,"1.5",UGRP_RELFREQUENCY,RFRQ_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	instance->int1.range=2.0;
	instance->int1.type=INT_TYPE_LIN;
	instance->int1.desc[0]='\0';

	UC_ConvertParameter(instance->p_winsize,"40 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->p_smooth ,"35 %",UGRP_PROPOTION,PROP_PROZ);

	instance->chanlink=FALSE;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin){ 
	SetSource      (&(instance->src), modWin,modGadgets,GADIX_SRC);
	SetModulator   (&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator   (&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
	SetInterpolator(&(instance->int1),modWin,modGadgets,GADIX_INTER);
	GT_SetGadgetAttrs(modGadgets[GADIX_WINSIZE],modWin,0l,GTST_String,instance->p_winsize,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_SMOOTH],modWin,0l,GTST_String,instance->p_smooth,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
