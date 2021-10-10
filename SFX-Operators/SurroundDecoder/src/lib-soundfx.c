/****h* SurroundDecoder/lib-soundfx.c [4.3] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.3 (22.08.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SurroundDecoder Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    22.Aug.2003
*  MODIFICATION HISTORY
*    22.Aug.2003	V 4.3	most recent version
*    22.Aug.2003	V 4.3	initial version
*  NOTES
*
*******
*/

#define SURROUNDDECODER_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "SurroundDecoder.h"

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

#define GADID_MODE		(GADID_SRC+SRC_IDCT)
#define GADIX_MODE		(GADIX_SRC+SRC_IXCT)

#define LAST_GADIX		(GADIX_MODE+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[2];
};

UBYTE *ModeLabels[]={ "Passive","Active",0l };

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

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="Mode";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_MODE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ModeLabels,GTCY_Active,instance->mode,TAG_DONE);
		modGadgets[GADIX_MODE]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"				,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Surround"				,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"				,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(1));				/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel     (&cfgsel        ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource     (&(instance->src  ),modWin,modGadgets,GADIX_SRC);

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
								if(!HandleSource     (&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)
								&& !HandleCfgSel     (&cfgsel        ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_MODE:
											instance->mode=icode;
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									//case KEY_TAB: break;
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
	SmpAccess *srcsa0,*srcsa1,*dstsa0,*dstsa1,*dstsa2,*dstsa3;
	SAFuncPtr SARead,SAWrite;
	ULONG blen;
	//-- administrative
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

	if(srcbuf->channels==CH_STEREO) {			// it's a stereo sample
		if(!dstbuf) {
			if(dstbuf=NewSample()) {
				if(AllocSampleMem(dstbuf,srcbuf->slen,CH_COUNT(CH_QUADRO))) {
					BufferFill(srcbuf,dstbuf);
					dstbuf->channels	=CH_QUADRO;
					dstbuf->channelmask	=CH_MASK(CH_QUADRO);
					dstbuf->aktch		=CH_COUNT(CH_QUADRO);
					newdst=TRUE;
				}
				else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
			}
			else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
			if(activate((SFXCoreInstance *)instance)) {
				activate_ch((SFXCoreInstance *)instance,0);
				if(rm_start>0) for(l=0;l<CH_COUNT(dstbuf->channels);l++) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);
				srcsa0=SAInit(srcbuf,0,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
				srcsa1=SAInit(srcbuf,1,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
				dstsa0=SAInit(dstbuf,0,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
				dstsa1=SAInit(dstbuf,1,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
				dstsa2=SAInit(dstbuf,2,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
				dstsa3=SAInit(dstbuf,3,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
				if(srcsa0 && srcsa1 && dstsa0 && dstsa1 && dstsa2 && dstsa3) {
					SARead =srcsa0->safunc;
					SAWrite=dstsa0->safunc;
					blen=Bytes2Samples(srcsa0->blen);
					sprintf(title,GetString(msgProgress),PRJ_NAME,1,1);
					StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
					switch(instance->mode) {
						case MODE_PASSIVE:
							for(srcsa0->seg=srcsa1->seg=dstsa0->seg=dstsa1->seg=dstsa2->seg=dstsa3->seg=0;((srcsa0->seg<srcsa0->seganz) && (instance->curlen<instance->slen));srcsa0->seg++,srcsa1->seg++,dstsa0->seg++,dstsa1->seg++,dstsa2->seg++,dstsa3->seg++) {
								instance->sbuf0=(SAMPLE *)srcsa0->buffer1;SARead(srcsa0,srcsa0->blen);	// Pointer auf src-Block holen & nächstes Read starten
								instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & nächstes Read starten
								instance->dbuf0=(SAMPLE *)dstsa0->buffer1;								// Pointer auf dst-Block
								instance->dbuf1=(SAMPLE *)dstsa1->buffer1;								// Pointer auf dst-Block
								instance->dbuf2=(SAMPLE *)dstsa2->buffer1;								// Pointer auf dst-Block
								instance->dbuf3=(SAMPLE *)dstsa3->buffer1;								// Pointer auf dst-Block
								process_passive((SFXCoreInstance *)instance,blen);
								SAWrite(dstsa0,dstsa0->blen);									// akt Block schreiben
								SAWrite(dstsa1,dstsa1->blen);									// akt Block schreiben
								SAWrite(dstsa2,dstsa1->blen);									// akt Block schreiben
								SAWrite(dstsa3,dstsa1->blen);									// akt Block schreiben
							}
							break;
						case MODE_ACTIVE:
							for(srcsa0->seg=srcsa1->seg=dstsa0->seg=dstsa1->seg=dstsa2->seg=dstsa3->seg=0;((srcsa0->seg<srcsa0->seganz) && (instance->curlen<instance->slen));srcsa0->seg++,srcsa1->seg++,dstsa0->seg++,dstsa1->seg++,dstsa2->seg++,dstsa3->seg++) {
								instance->sbuf0=(SAMPLE *)srcsa0->buffer1;SARead(srcsa0,srcsa0->blen);	// Pointer auf src-Block holen & nächstes Read starten
								instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & nächstes Read starten
								instance->dbuf0=(SAMPLE *)dstsa0->buffer1;								// Pointer auf dst-Block
								instance->dbuf1=(SAMPLE *)dstsa1->buffer1;								// Pointer auf dst-Block
								instance->dbuf2=(SAMPLE *)dstsa2->buffer1;								// Pointer auf dst-Block
								instance->dbuf3=(SAMPLE *)dstsa3->buffer1;								// Pointer auf dst-Block
								process_active((SFXCoreInstance *)instance,blen);
								SAWrite(dstsa0,dstsa0->blen);									// akt Block schreiben
								SAWrite(dstsa1,dstsa1->blen);									// akt Block schreiben
								SAWrite(dstsa2,dstsa1->blen);									// akt Block schreiben
								SAWrite(dstsa3,dstsa1->blen);									// akt Block schreiben
							}
							break;
					}
					deactivate_ch((SFXCoreInstance *)instance,0);
				}
				else okay=FALSE;
				if(srcsa0) SADone(srcsa0,srcbuf);
				if(srcsa1) SADone(srcsa1,srcbuf);
				if(dstsa0) SADone(dstsa0,dstbuf);
				if(dstsa1) SADone(dstsa1,dstbuf);
				if(dstsa2) SADone(dstsa2,dstbuf);
				if(dstsa3) SADone(dstsa3,dstbuf);
				if(rm_start+rm_len<srcbuf->slen) for(l=0;l<CH_COUNT(dstbuf->channels);l++) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
				if(okay && newdst) AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);
			}
			else okay=FALSE;
			deactivate((SFXCoreInstance *)instance);
			DelProWin(pwd);
		}
		else okay=FALSE;
	}
	else { Message(errSourceStereo,NULL,__FILE__,__LINE__);okay=FALSE; }
	if(!okay) {
		if(newdst) RemSample(dstbuf);
		return(FALSE);
	}
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;
		if(    !stricmp(RexxPar2,"Mode"))			{ instance->mode    =atoi(RexxPar3)%2;pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;
		if(    !stricmp(RexxPar2,"Mode"))			{ sprintf(ret,"%d",instance->mode);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	instance->mode		=CfgIO_ReadBool			(cfg,"General"	,"Mode"		,instance->mode);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteBool			  (cfg,"General"	,"Mode"		,instance->mode);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->mode=0;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource     (&(instance->src),  modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_MODE],modWin,0l,GTCY_Active,instance->mode,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
