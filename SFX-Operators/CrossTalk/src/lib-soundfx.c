/****h* CrossTalk/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (29.10.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    CrossTalk Operator module for SoundFX
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
* 	* should probably enable/disable gadgets, depending on source=quadro/stereo
*******
*/
 
#define CROSSTALK_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "CrossTalk.h"

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

#define GADID_AMPF		(GADID_MOD2+MOD_IDCT)
#define GADIX_AMPF		(GADIX_MOD2+MOD_IXCT)

#define LAST_GADIX		(GADIX_AMPF+1)

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

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys4+RunTime->scy[FRMCONTENT_YOFFSET];
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
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+  RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+3+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Width"			,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Depth"			,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"	,xs1,ys4,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Width */
			DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);					/* Depth */
			DrawGadWB(modrp,xs1,ys4,xh,ANYFRM_HEIGHT(1));				/* Par */
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
								if(!HandleSource   (               &(instance->src),   modWin,modGadgets,GADID_SRC,   GADIX_SRC,   gnr,icode)
								&& !HandleModulator(&(instance->mod1),&(instance->src),0l,modWin,modGadgets,GADID_MOD1,  GADIX_MOD1,  gnr,icode)
								&& !HandleModulator(&(instance->mod2),&(instance->src),0l,modWin,modGadgets,GADID_MOD2,  GADIX_MOD2,  gnr,icode)
								&& !HandleCfgSel   (&cfgsel       ,                 modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
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
	//-- prowin
	PWData *pwd;
	ProWinTitle title;
	//-- procesing length
	ULONG rm_start,rm_len,blen;
	//-- src & dst buffers
	SInfo *dstbuf=NULL,*srcbuf;
	//-- buffer access
	SmpAccess *srcsa0,*srcsa1,*srcsa2,*srcsa3;
	SmpAccess *dstsa0,*dstsa1,*dstsa2,*dstsa3;
	SAFuncPtr SARead,SAWrite;
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
	instance->wids=atof(param);
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_PROPOTION,PROP_FC);
	instance->wide=atof(param);
	instance->widd=instance->wide-instance->wids;

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_PROPOTION,PROP_FC);
	instance->deps=atof(param);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_PROPOTION,PROP_FC);
	instance->depe=atof(param);
	instance->depd=instance->depe-instance->deps;

	UC_ConvertParameter(param,instance->p_ampf,UGRP_AMPLITUDE,AMP_FC);
	instance->ampf=atof(param);

	if(srcbuf->channels==CH_STEREO || srcbuf->channels==CH_QUADRO) {
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
		if(pwd=NewProWin()) {
			if(activate((SFXCoreInstance *)instance)) {
				activate_ch((SFXCoreInstance *)instance,0);
				if(rm_start>0) CopyData(srcbuf,dstbuf,0,0,rm_start);
				sprintf(title,GetString(msgProgress),PRJ_NAME,1,1);
				StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
				if(srcbuf->channels==CH_STEREO) {
					srcsa0=SAInit(srcbuf,0,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa1=SAInit(srcbuf,1,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					dstsa0=SAInit(dstbuf,0,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					dstsa1=SAInit(dstbuf,1,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					if(srcsa0 && srcsa1 && dstsa0 && dstsa1) {
						SARead =srcsa0->safunc;
						SAWrite=dstsa0->safunc;
						blen=Bytes2Samples(srcsa0->blen);
						for(srcsa0->seg=srcsa1->seg=dstsa0->seg=dstsa1->seg=0;((srcsa0->seg<srcsa0->seganz) && (instance->curlen<instance->slen));srcsa0->seg++,srcsa1->seg++,dstsa0->seg++,dstsa1->seg++) {
							instance->sbuf0=(SAMPLE *)srcsa0->buffer1;SARead(srcsa0,srcsa0->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->dbuf0=(SAMPLE *)dstsa0->buffer1;								// Pointer auf dst-Block
							instance->dbuf1=(SAMPLE *)dstsa1->buffer1;								// Pointer auf dst-Block
							process_s((SFXCoreInstance *)instance,blen);
							SAWrite(dstsa0,dstsa0->blen);									// akt Block schreiben
							SAWrite(dstsa1,dstsa1->blen);									// akt Block schreiben
						}
					}
					else okay=FALSE;
					if(srcsa0) SADone(srcsa0,srcbuf);
					if(srcsa1) SADone(srcsa1,srcbuf);
					if(dstsa0) SADone(dstsa0,dstbuf);
					if(dstsa1) SADone(dstsa1,dstbuf);
				}
				if(srcbuf->channels==CH_QUADRO) {
					srcsa0=SAInit(srcbuf,0,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa1=SAInit(srcbuf,1,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa2=SAInit(srcbuf,2,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa3=SAInit(srcbuf,3,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					dstsa0=SAInit(dstbuf,0,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					dstsa1=SAInit(dstbuf,1,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					dstsa2=SAInit(dstbuf,2,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					dstsa3=SAInit(dstbuf,3,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					if(srcsa0 && srcsa1 && srcsa2 && srcsa3 && dstsa0 && dstsa1 && dstsa2 && dstsa3) {
						SARead =srcsa0->safunc;
						SAWrite=dstsa0->safunc;
						blen=Bytes2Samples(srcsa0->blen);
						for(srcsa0->seg=srcsa1->seg=srcsa2->seg=srcsa3->seg=dstsa0->seg=dstsa1->seg=dstsa2->seg=dstsa3->seg=0;((srcsa0->seg<srcsa0->seganz) && (instance->curlen<instance->slen));srcsa0->seg++,srcsa1->seg++,srcsa2->seg++,srcsa3->seg++,dstsa0->seg++,dstsa1->seg++,dstsa2->seg++,dstsa3->seg++) {
							instance->sbuf0=(SAMPLE *)srcsa0->buffer1;SARead(srcsa0,srcsa0->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->sbuf2=(SAMPLE *)srcsa2->buffer1;SARead(srcsa2,srcsa2->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->sbuf3=(SAMPLE *)srcsa3->buffer1;SARead(srcsa3,srcsa3->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->dbuf0=(SAMPLE *)dstsa0->buffer1;								// Pointer auf dst-Block
							instance->dbuf1=(SAMPLE *)dstsa1->buffer1;								// Pointer auf dst-Block
							instance->dbuf2=(SAMPLE *)dstsa2->buffer1;								// Pointer auf dst-Block
							instance->dbuf3=(SAMPLE *)dstsa3->buffer1;								// Pointer auf dst-Block
							process_q((SFXCoreInstance *)instance,blen);
							SAWrite(dstsa0,dstsa0->blen);									// akt Block schreiben
							SAWrite(dstsa1,dstsa1->blen);									// akt Block schreiben
							SAWrite(dstsa2,dstsa2->blen);									// akt Block schreiben
							SAWrite(dstsa3,dstsa3->blen);									// akt Block schreiben
						}
					}
					else okay=FALSE;
					if(srcsa0) SADone(srcsa0,srcbuf);
					if(srcsa1) SADone(srcsa1,srcbuf);
					if(srcsa2) SADone(srcsa2,srcbuf);
					if(srcsa3) SADone(srcsa3,srcbuf);
					if(dstsa0) SADone(dstsa0,dstbuf);
					if(dstsa1) SADone(dstsa1,dstbuf);
					if(dstsa2) SADone(dstsa2,dstbuf);
					if(dstsa3) SADone(dstsa3,dstbuf);
				}
				if(rm_start+rm_len<srcbuf->slen) CopyData(srcbuf,dstbuf,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
				deactivate_ch((SFXCoreInstance *)instance,0);
				if(ProWinAborted(instance->curlen)) { okay=FALSE; }
				if(okay && newdst) AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);
			}
			else okay=FALSE;
			deactivate((SFXCoreInstance *)instance);
			DelProWin(pwd);
		}
		else okay=FALSE;
	}
	else { Message(errSourceStereoQuadro,NULL,__FILE__,__LINE__);okay=FALSE; }
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
		if(!pf && !stricmp(RexxPar2,"Ampf"))			{ strntcpy(instance->p_ampf ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet(&(instance->mod1),"P1",params,ret);
		if(!pf) pf=ParseModulatorGet(&(instance->mod2),"P2",params,ret);
		if(!pf && !stricmp(RexxPar2,"Ampf"))			{ strntcpy(ret,instance->p_ampf ,PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_ampf,CfgIO_ReadString			(cfg,"General"	,"Ampf"	,"1.25"));
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Ampf"	,instance->p_ampf);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod1.vals,"0.5",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod1.vale,"0.5",UGRP_PROPOTION,PROP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"0.5",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod2.vale,"0.5",UGRP_PROPOTION,PROP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	UC_ConvertParameter(instance->p_ampf ,"1.25"   ,UGRP_AMPLITUDE,AMP_FC);
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src ),modWin,modGadgets,GADIX_SRC);
	SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
	GT_SetGadgetAttrs(modGadgets[GADIX_AMPF],modWin,0l,GTST_String,instance->p_ampf,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
