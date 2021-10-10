/****h* Panorama-4Ch/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (05.02.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Panorama-4Ch Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    05.Feb.2002	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*  IDEAS
*    different encodings
*      * L,R,F,B     : Left,Right,Front,Back
*      * FL,FR,BL,BR : FrontLeft,FrontRight,BackLeft,BackRight
*
*******
*/

#define PANORAMA_4CH_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Panorama-4Ch.h"

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

#define GADID_MODE		(GADID_MOD2+MOD_IDCT)
#define GADIX_MODE		(GADIX_MOD2+MOD_IXCT)

#define LAST_GADIX		(GADIX_MODE+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[4];
};

UBYTE *ModeLabels[]={ "Audio","Video",NULL };

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

		ng.ng_LeftEdge      =xs1+RunTime->scx[54];
		ng.ng_TopEdge       =ys4+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText    ="Mode";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID      =GADID_MODE;
		ng.ng_Flags         =PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ModeLabels,GTCY_Active,instance->mode,TAG_DONE);
		modGadgets[GADIX_MODE]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"				,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Left-Right Position"	,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Front-Back Position"	,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Speaker Layout"        ,xs1,ys4,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"				,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* L-R Pos */
			DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);					/* V-H Pos */
			DrawGadWB(modrp,xs1,ys4,xh,ANYFRM_HEIGHT(1));               /* Par */
			DrawGadWB(modrp,xs1,ys5,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src ),modWin,modGadgets,GADIX_SRC);
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
								if(!HandleSource   (&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)
								&& !HandleModulator(&(instance->mod1),&(instance->src),0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode)
								&& !HandleModulator(&(instance->mod2),&(instance->src),0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode)
								&& !HandleCfgSel   (&cfgsel      ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
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
	//UBYTE l;
	//-- prowin
	PWData *pwd;
	ProWinTitle title;
	//-- procesing length
	ULONG rm_start,rm_len;
	//-- src & dst buffers
	SInfo *dstbuf=NULL,*srcbuf;
	//-- buffer access
	SmpAccess *srcsa,*dstsa0,*dstsa1,*dstsa2,*dstsa3;
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

	UC_ConvertParameter(param,instance->mod1.vals,UGRP_PROPOTION,PROP_FC);
	instance->lrs=atof(param);
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_PROPOTION,PROP_FC);
	instance->lre=atof(param);
	instance->lrd=instance->lre-instance->lrs;

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_PROPOTION,PROP_FC);
	instance->vhs=atof(param);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_PROPOTION,PROP_FC);
	instance->vhe=atof(param);
	instance->vhd=instance->vhe-instance->vhs;

	if(srcbuf->channels==CH_MONO) {			// it's a mono sample
		if(!dstbuf) {
			if(dstbuf=NewSample()) {
				if(AllocSampleMem(dstbuf,srcbuf->slen,CH_COUNT(CH_QUADRO))) {
					BufferFill(srcbuf,dstbuf);
					dstbuf->channels=CH_QUADRO;
					dstbuf->aktch=CH_COUNT(CH_QUADRO);
					dstbuf->channelmask=CH_MASK(CH_QUADRO);;
					newdst=TRUE;
				}
				else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
			}
			else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
			if(activate((SFXCoreInstance *)instance)) {
				if(IS_CHANNEL_ACTIVE(0,srcbuf->channelmask)) {
					activate_ch((SFXCoreInstance *)instance,0);
					if(rm_start>0) {
						CopyDataCh(srcbuf,dstbuf,0,0,0,rm_start);
						CopyDataCh(srcbuf,dstbuf,1,0,0,rm_start);
						CopyDataCh(srcbuf,dstbuf,2,0,0,rm_start);
						CopyDataCh(srcbuf,dstbuf,3,0,0,rm_start);
					}
					srcsa =SAInit(srcbuf,0,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					dstsa0=SAInit(dstbuf,0,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					dstsa1=SAInit(dstbuf,1,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					dstsa2=SAInit(dstbuf,2,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					dstsa3=SAInit(dstbuf,3,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					if(srcsa && dstsa0 && dstsa1 && dstsa2 && dstsa3) {
						SARead =srcsa ->safunc;
						SAWrite=dstsa0->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,1,1);
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
						switch(instance->mode) {                                 // je nach speaker-mode
							case MODE_AUDIO:
								for(srcsa->seg=dstsa0->seg=dstsa1->seg=dstsa2->seg=dstsa3->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa0->seg++,dstsa1->seg++,dstsa2->seg++,dstsa3->seg++) {
									instance->sbuf =(SAMPLE *)srcsa ->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
									instance->dbuf0=(SAMPLE *)dstsa0->buffer1;								// Pointer auf dst-Block
									instance->dbuf1=(SAMPLE *)dstsa1->buffer1;								// Pointer auf dst-Block
									instance->dbuf2=(SAMPLE *)dstsa2->buffer1;								// Pointer auf dst-Block
									instance->dbuf3=(SAMPLE *)dstsa3->buffer1;								// Pointer auf dst-Block
									process_audio((SFXCoreInstance *)instance,blen);
									SAWrite(dstsa0,dstsa0->blen);									// akt Block schreiben
									SAWrite(dstsa1,dstsa1->blen);									// akt Block schreiben
									SAWrite(dstsa2,dstsa2->blen);									// akt Block schreiben
									SAWrite(dstsa3,dstsa3->blen);									// akt Block schreiben
								}
								break;
							case MODE_VIDEO:
								for(srcsa->seg=dstsa0->seg=dstsa1->seg=dstsa2->seg=dstsa3->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa0->seg++,dstsa1->seg++,dstsa2->seg++,dstsa3->seg++) {
									instance->sbuf =(SAMPLE *)srcsa ->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
									instance->dbuf0=(SAMPLE *)dstsa0->buffer1;								// Pointer auf dst-Block
									instance->dbuf1=(SAMPLE *)dstsa1->buffer1;								// Pointer auf dst-Block
									instance->dbuf2=(SAMPLE *)dstsa2->buffer1;								// Pointer auf dst-Block
									instance->dbuf3=(SAMPLE *)dstsa3->buffer1;								// Pointer auf dst-Block
									process_video((SFXCoreInstance *)instance,blen);
									SAWrite(dstsa0,dstsa0->blen);									// akt Block schreiben
									SAWrite(dstsa1,dstsa1->blen);									// akt Block schreiben
									SAWrite(dstsa2,dstsa2->blen);									// akt Block schreiben
									SAWrite(dstsa3,dstsa3->blen);									// akt Block schreiben
								}
								break;
						}
					}
					else okay=FALSE;
					if(srcsa ) SADone(srcsa ,srcbuf);
					if(dstsa0) SADone(dstsa0,dstbuf);
					if(dstsa1) SADone(dstsa1,dstbuf);
					if(dstsa2) SADone(dstsa2,dstbuf);
					if(dstsa3) SADone(dstsa3,dstbuf);
					if(rm_start+rm_len<srcbuf->slen) {
						CopyDataCh(srcbuf,dstbuf,0,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
						CopyDataCh(srcbuf,dstbuf,1,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
						CopyDataCh(srcbuf,dstbuf,2,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
						CopyDataCh(srcbuf,dstbuf,3,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
					}
					deactivate_ch((SFXCoreInstance *)instance,0);
					if(ProWinAborted(instance->curlen)) { okay=FALSE; }										  // es wurde abgebrochen
				}
				else {
					CopyDataCh(srcbuf,dstbuf,0,0,0,srcbuf->slen);
					CopyDataCh(srcbuf,dstbuf,1,0,0,srcbuf->slen);
					CopyDataCh(srcbuf,dstbuf,2,0,0,srcbuf->slen);
					CopyDataCh(srcbuf,dstbuf,3,0,0,srcbuf->slen);
				}
				if(okay && newdst) AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);
			}
			else okay=FALSE;
			deactivate((SFXCoreInstance *)instance);
			DelProWin(pwd);
		}
		else okay=FALSE;
	}
	else { Message(errSourceMono,NULL,__FILE__,__LINE__);okay=FALSE; }
	if(!okay) {
		if(newdst) RemSample(dstbuf);
		return(FALSE);
	}
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)){ 
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;pf=ParseModulatorSet   (&(instance->mod1), "P1",params);
		if(!pf) pf=ParseModulatorSet   (&(instance->mod2), "P2",params);
		if(!pf && !stricmp(RexxPar2,"Mode"))    { instance->mode  =CfgIO_MatchString(RexxPar3,ModeLabels);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=ParseModulatorGet   (&(instance->mod1), "P1",params,ret);
		if(!pf) ParseModulatorGet   (&(instance->mod2), "P2",params,ret);
		if(!pf && !stricmp(RexxPar2,"Mode"))    { sprintf(ret,"%s",ModeLabels[instance->mode]);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname){ 
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	instance->mode  =CfgIO_MatchString(
					 CfgIO_ReadString           (cfg,"General"  ,"Mode"     ,ModeLabels[MODE_VIDEO]),ModeLabels);
}

void SaveSettings(struct Instance *instance,char *fname){ 
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString         (cfg,"General"    ,"Mode"     ,ModeLabels[instance->mode]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src.srcbuf=NULL;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod1.vals,"0.5",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod1.vale,"0.5",UGRP_PROPOTION,PROP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"0.5",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod2.vale,"0.5",UGRP_PROPOTION,PROP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	instance->mode=MODE_VIDEO;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) { 
	SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
	SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
