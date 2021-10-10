/****h* DeNoise-FIR/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (29.10.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    DeNoise-FIR Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    29.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define DENOISE_FIR_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "DeNoise-FIR.h"

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

#define GADID_ATTACK	(GADID_SRC+SRC_IDCT)
#define GADIX_ATTACK	(GADIX_SRC+SRC_IXCT)

#define GADID_THRESHS	(GADID_ATTACK+1)
#define GADIX_THRESHS	(GADIX_ATTACK+1)

#define GADID_THRESHE	(GADID_THRESHS+1)
#define GADIX_THRESHE	(GADIX_THRESHS+1)

#define GADID_NR		(GADID_THRESHE+1)
#define GADIX_NR		(GADIX_THRESHE+1)

#define GADID_SHAPE		(GADID_NR+1)
#define GADIX_SHAPE		(GADIX_NR+1)

#define GADID_BANDS		(GADID_SHAPE+1)
#define GADIX_BANDS		(GADIX_SHAPE+1)

#define GADID_WFKT		(GADID_BANDS+1)
#define GADIX_WFKT		(GADIX_BANDS+1)

#define LAST_GADIX		(GADIX_WFKT+WFKT_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[3];
};

UBYTE *ShapeLabels[]={ "Linear","SpeedUp","SlowDown",0l };

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
		SFXMod_ModDescInit(1,PD_WINFUNCTION ,"W1",&(instance->wfkt1));
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
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(4);
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
		ng.ng_GadgetText	="Attack";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_ATTACK;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_attack,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_ATTACK]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="Shape";
		ng.ng_GadgetID		=GADID_SHAPE;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ShapeLabels,GTCY_Active,instance->shape,TAG_DONE);
		modGadgets[GADIX_SHAPE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="ThreshS";
		ng.ng_GadgetID		=GADID_THRESHS;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_threshs,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_THRESHS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="ThreshE";
		ng.ng_GadgetID		=GADID_THRESHE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_threshe,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_THRESHE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_GadgetText	="Bands";
		ng.ng_GadgetID		=GADID_BANDS;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_bands,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_BANDS]=g;

		g=AddWinFunction(&(instance->wfkt1),modGadgets,g,GADID_WFKT,GADIX_WFKT,xs1,ys2+RunTime->scy[FRMCONTENT_YOFFSET+36]);

		ng.ng_LeftEdge		=xs1+RunTime->scx[254];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+36];
		ng.ng_Width			=RunTime->scx[50];
		ng.ng_GadgetText	="Num";
		ng.ng_GadgetID		=GADID_NR;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_anz,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_NR]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Window",xs1+RunTime->scx[45],ys2+RunTime->scy[FRMTEXT_YPOS(4)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"	,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(4));				/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource     (&(instance->src),  modWin,modGadgets,GADIX_SRC);
			SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);

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
								if(!HandleSource     (&(instance->src),  modWin,modGadgets,GADID_SRC,	 GADIX_SRC,   gnr,icode)
								&& !HandleWinFunction(&(instance->wfkt1),modWin,modGadgets,GADID_WFKT,	 GADIX_WFKT,  gnr,icode)
								&& !HandleCfgSel	 (&cfgsel		 ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_ATTACK:
											HandleParameter(instance->p_attack,modWin,modGadgets[GADIX_ATTACK]);
											break;
										case GADID_THRESHS:
											HandleParameter(instance->p_threshs,modWin,modGadgets[GADIX_THRESHS]);
											break;
										case GADID_THRESHE:
											HandleParameter(instance->p_threshe,modWin,modGadgets[GADIX_THRESHE]);
											break;
										case GADID_SHAPE:
											instance->shape=icode;
											break;
										case GADID_BANDS:
											HandleParameter(instance->p_bands,modWin,modGadgets[GADIX_BANDS]);
											break;
										case GADID_NR:
											HandleParameter(instance->p_anz,modWin,modGadgets[GADIX_NR]);
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_ATTACK],modWin,0l);
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

	UC_ConvertParameter(param,instance->p_threshs,UGRP_AMPLITUDE,AMP_FC);
	instance->threshs=atof(param);
	UC_ConvertParameter(param,instance->p_threshe,UGRP_AMPLITUDE,AMP_FC);
	instance->threshe=atof(param);

	if(UC_GetParameterUnit(instance->p_attack,UGRP_ABSTIME)==ATME_SAMPLES) {
		instance->attack=atoi(instance->p_attack);
	}
	else {
		UC_ConvertParameter(param,instance->p_attack,UGRP_ABSTIME,ATME_MS);
		instance->attack=(ULONG)(0.5+atof(param)*((double)srcbuf->srat/1000.0));
	}
	if(instance->attack<2) instance->attack=2;

	UC_ConvertParameter(param,instance->p_bands,UGRP_COUNT,COUN_ABS);
	instance->bands=atoi(param);
	if(instance->bands>MAX_BANDS) instance->bands=MAX_BANDS;

	UC_ConvertParameter(param,instance->p_anz,UGRP_COUNT,COUN_ABS);
	instance->anz=atoi(param);
	if(instance->anz&1) instance->anz++;		// make it an even number
	if(instance->anz<2) instance->anz=2;
	else if(instance->anz>1024) instance->anz=1024;
	
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
	if((pwd=NewProWin())) {									// ein neues asynchrones Statusfenster öffnen
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
						for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
							instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
							process((SFXCoreInstance *)instance,blen);
							SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
						}
					}
					else okay=FALSE;
					if(srcsa) SADone(srcsa,srcbuf);
					if(dstsa) SADone(dstsa,dstbuf);
					if(rm_start+rm_len<srcbuf->slen) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) l=CH_MAX;					// es wurde abgebrochen
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
		cf=TRUE;pf=ParseWinFunctionSet(&(instance->wfkt1),"W1",params);
		if(!pf && !stricmp(RexxPar2,"ThreshS"))		{ strntcpy(instance->p_threshs,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"ThreshE"))		{ strntcpy(instance->p_threshe,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Attack"))		{ strntcpy(instance->p_attack,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Shape"))		{ instance->shape=CfgIO_MatchString(RexxPar3,ShapeLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ strntcpy(instance->p_bands,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(instance->p_anz  ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=ParseWinFunctionGet(&(instance->wfkt1),"W1",params,ret);
		if(!pf && !stricmp(RexxPar2,"ThreshS"))		{ strntcpy(ret,instance->p_threshs,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"ThreshE"))		{ strntcpy(ret,instance->p_threshe,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Attack"))		{ strntcpy(ret,instance->p_attack,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Shape"))		{ strcpy(ret,ShapeLabels[instance->shape]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ strntcpy(ret,instance->p_bands,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(ret,instance->p_anz  ,PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_threshs,CfgIO_ReadString	(cfg,"General"	,"Thresh"	,"5 %"));				// Compatibillity
	strcpy(instance->p_threshs,CfgIO_ReadString	(cfg,"General"	,"ThreshS"	,instance->p_threshs));	// to previous version
	strcpy(instance->p_threshe,CfgIO_ReadString	(cfg,"General"	,"ThreshE"	,"2 %"));
	strcpy(instance->p_attack,	CfgIO_ReadString	(cfg,"General"	,"Attack"	,"0.5 ms"));
	instance->shape=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Shape"	,ShapeLabels[instance->shape]),ShapeLabels);
	strcpy(instance->p_bands,	CfgIO_ReadString	(cfg,"General"	,"Bands"	,"8"));
	strcpy(instance->p_anz,	CfgIO_ReadString	(cfg,"General"	,"Length"	,"32"));
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"ThreshS"	,instance->p_threshs);
	CfgIO_WriteString		  (cfg,"General"	,"ThreshE"	,instance->p_threshe);
	CfgIO_WriteString		  (cfg,"General"	,"Attack"	,instance->p_attack);
	CfgIO_WriteString		  (cfg,"General"	,"Shape"	,ShapeLabels[instance->shape]);
	CfgIO_WriteString		  (cfg,"General"	,"Bands"	,instance->p_bands);
	CfgIO_WriteString		  (cfg,"General"	,"Length"	,instance->p_anz);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->wfkt1.win=WIN_HAMMING;
	instance->wfkt1.par=0.54;
	instance->wfkt1.desc[0]='\0';

	UC_ConvertParameter(instance->p_attack ,"0.5 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->p_threshs,"5 %",UGRP_AMPLITUDE,AMP_ABSPROM);
	UC_ConvertParameter(instance->p_threshe,"2 %",UGRP_AMPLITUDE,AMP_ABSPROM);
	instance->shape=0;
	UC_ConvertParameter(instance->p_bands, "8",UGRP_COUNT,COUN_ABS);
	UC_ConvertParameter(instance->p_anz   ,"32",UGRP_COUNT,COUN_ABS);
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource     (&(instance->src),  modWin,modGadgets,GADIX_SRC);
	SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);
	GT_SetGadgetAttrs(modGadgets[GADIX_ATTACK ],modWin,0l,GTST_String,instance->p_attack ,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_THRESHS],modWin,0l,GTST_String,instance->p_threshs,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_THRESHE],modWin,0l,GTST_String,instance->p_threshe,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_SHAPE  ],modWin,0l,GTCY_Active,instance->shape  ,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_BANDS  ],modWin,0l,GTST_String,instance->p_bands  ,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_NR     ],modWin,0l,GTST_String,instance->p_anz    ,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
