/****h* Subtract/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (31.10.01) ? by Stefan Kost 1998-2001
*  FUNCTION
*    Subtract Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    31.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define SUBTRACT_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Subtract.h"

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

#define GADID_DELAY2	(GADID_SRC2+SRC_IDCT)
#define GADIX_DELAY2	(GADIX_SRC2+SRC_IXCT)

#define LAST_GADIX		(GADIX_DELAY2+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[3];
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
		SFXMod_ModDescInit(0,PD_SOURCE      ,"S1",&(instance->src1));
		SFXMod_ModDescInit(1,PD_SOURCE      ,"S2",&(instance->src2));
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
	UWORD ys3=ys2+1+SRCSEL_HEIGHT;
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

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

		g=AddSource(&(instance->src1),modGadgets,g,GADID_SRC1,GADIX_SRC1,xs1,ys1);
		g=AddSource(&(instance->src2),modGadgets,g,GADID_SRC2,GADIX_SRC2,xs1,ys2);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="Delay";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_DELAY2;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_delay2,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DELAY2]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source 1"			,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Source 2"			,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Delay Source 2"	,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"			,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys3,xh,ANYFRM_HEIGHT(1));				/* Go */
			DrawGadWB(modrp,xs1,ys4,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src1),modWin,modGadgets,GADIX_SRC1);
			SetSource   (&(instance->src2),modWin,modGadgets,GADIX_SRC2);

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
								if(!HandleSource(&(instance->src1),modWin,modGadgets,GADID_SRC1,GADIX_SRC1,gnr,icode)
								&& !HandleSource(&(instance->src2),modWin,modGadgets,GADID_SRC2,GADIX_SRC2,gnr,icode)
								&& !HandleCfgSel(&cfgsel       ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_DELAY2:
											HandleParameter(instance->p_delay2,modWin,modGadgets[GADIX_DELAY2]);
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_DELAY2],modWin,0l);
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
	ULONG rm_start1,rm_len1,rm_start2,rm_len2;
	//-- src & dst buffers
	SInfo *dstbuf=NULL,*srcbuf1,*srcbuf2;
	//-- buffer access
	SmpAccess *srcsa1,*srcsa2,*dstsa;
	SAFuncPtr SARead1,SARead2,SAWrite;
	ULONG blen;
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
	SetRngs(srcbuf1,&rm_start1,&rm_len1,instance->src1.rmode);
	SetRngs(srcbuf2,&rm_start2,&rm_len2,instance->src2.rmode);
	instance->slen=rm_len1;
	instance->srat=srcbuf1->srat;

	if(UC_GetParameterUnit(instance->p_delay2,UGRP_ABSTIME)==ATME_SAMPLES) {
		instance->delay2=atoi(instance->p_delay2);
	}
	else {
		UC_ConvertParameter(param,instance->p_delay2,UGRP_ABSTIME,ATME_MS);
		instance->delay2=(LONG)(atof(param)*((double)srcbuf2->srat/1000.0));
	}

	if(!dstbuf) {
		if(dstbuf=NewSample()) {
			BufferFill(srcbuf1,dstbuf);
			dstbuf->channels=max(srcbuf1->channels,srcbuf2->channels);
			dstbuf->channelmask=CH_MASK(dstbuf->channels);
			if(AllocSampleMem(dstbuf,srcbuf1->slen,CH_COUNT(dstbuf->channels))) {
				newdst=TRUE;
			}
			else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
	}
	if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster ?ffnen
		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;l<CH_COUNT(dstbuf->channels);l++) {
				//if(srcbuf1->channelmask&(1<<l) && srcbuf2->channelmask&(1<<l)) {
					activate_ch((SFXCoreInstance *)instance,l);
					if(rm_start1>0) CopyDataCh(srcbuf1,dstbuf,l,0,0,rm_start1);
					srcsa1=SAInit(srcbuf1,l,rm_start1,rm_len1,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl?cke konstanter Gr??e lesen
					srcsa2=SAInit(srcbuf2,l,rm_start2,rm_len2,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl?cke konstanter Gr??e lesen
					dstsa =SAInit(dstbuf ,l,rm_start1,rm_len1,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl?cke konstanter Gr??e schreiben
					if(srcsa1 && srcsa2 && dstsa) {
						SARead1=srcsa1->safunc;
						SARead2=srcsa2->safunc;
						SAWrite=dstsa ->safunc;
						blen=Bytes2Samples(srcsa1->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(dstbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len1);					// den Statusbalken starten
						if(!instance->delay2) {
							if(rm_len1==rm_len2) {
								for(srcsa1->seg=srcsa2->seg=dstsa->seg=0;((srcsa1->seg<srcsa1->seganz) && (instance->curlen<rm_len1));srcsa1->seg++,srcsa2->seg++,dstsa->seg++) {
									instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead1(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & n?chstes Read starten
									instance->sbuf2=(SAMPLE *)srcsa2->buffer1;SARead2(srcsa2,srcsa2->blen);	// Pointer auf src-Block holen & n?chstes Read starten
									instance->dbuf =(SAMPLE *)dstsa ->buffer1;								// Pointer auf dst-Block
									process_0((SFXCoreInstance *)instance,min(blen,(rm_len1-instance->curlen)));
									SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
								}
							}
							else {
								for(srcsa1->seg=srcsa2->seg=dstsa->seg=0;srcsa1->seg<srcsa1->seganz;srcsa1->seg++,srcsa2->seg++,dstsa->seg++) {
									instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead1(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & n?chstes Read starten
									instance->dbuf =(SAMPLE *)dstsa ->buffer1;								// Pointer auf dst-Block
									if(instance->curlen<rm_len2) {
										instance->sbuf2=(SAMPLE *)srcsa2->buffer1;SARead2(srcsa2,srcsa2->blen);// Pointer auf src-Block holen & n?chstes Read starten
										process_0((SFXCoreInstance *)instance,min(blen,(rm_len1-instance->curlen)));
									}
									else {
										process_1((SFXCoreInstance *)instance,min(blen,(rm_len1-instance->curlen)));
									}
									SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
								}
							}
						}
						else {		// delay is not null
							if(rm_len1==rm_len2) {
								for(srcsa1->seg=srcsa2->seg=dstsa->seg=0;srcsa1->seg<srcsa1->seganz;srcsa1->seg++,srcsa2->seg++,dstsa->seg++) {
									instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead1(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & n?chstes Read starten
									instance->sbuf2=(SAMPLE *)srcsa2->buffer1;SARead2(srcsa2,srcsa2->blen);	// Pointer auf src-Block holen & n?chstes Read starten
									instance->dbuf =(SAMPLE *)dstsa ->buffer1;								// Pointer auf dst-Block
									process_2((SFXCoreInstance *)instance,min(blen,(rm_len1-instance->curlen)));
									SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
								}
							}
							else {
								for(srcsa1->seg=srcsa2->seg=dstsa->seg=0;srcsa1->seg<srcsa1->seganz;srcsa1->seg++,srcsa2->seg++,dstsa->seg++) {
									instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead1(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & n?chstes Read starten
									instance->dbuf =(SAMPLE *)dstsa ->buffer1;								// Pointer auf dst-Block
									if(instance->curlen<rm_len2) {
										instance->sbuf2=(SAMPLE *)srcsa2->buffer1;SARead2(srcsa2,srcsa2->blen);// Pointer auf src-Block holen & n?chstes Read starten
										process_2((SFXCoreInstance *)instance,min(blen,(rm_len1-instance->curlen)));
									}
									else {
										process_1((SFXCoreInstance *)instance,min(blen,(rm_len1-instance->curlen)));
									}
									SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
								}
							}
						}
					}
					else okay=FALSE;
					if(srcsa1) SADone(srcsa1,srcbuf1);
					if(srcsa2) SADone(srcsa2,srcbuf2);
					if(dstsa)  SADone(dstsa ,dstbuf);
					if(rm_start1+rm_len1<srcbuf1->slen) CopyDataCh(srcbuf1,dstbuf,l,(rm_start1+rm_len1),(rm_start1+rm_len1),(srcbuf1->slen-(rm_start1+rm_len1)));
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }										  // es wurde abgebrochen
				//}
				//else CopyDataCh(srcbuf1,dstbuf,l,0,0,srcbuf1->slen);
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
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"Delay2"))		{ strntcpy(instance->p_delay2,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;/*pf=0;*/
		if(       !stricmp(RexxPar2,"Delay2"))		{ strntcpy(ret,instance->p_delay2,PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_delay2,CfgIO_ReadString	(cfg,"General"	,"Delay2"	,"0 sv"));
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		 (cfg,"General"	,"Delay2"		,instance->p_delay2);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src1.srcbuf=0L;
	instance->src1.rmode=RNGMD_ALL;

	//instance->src2.srcbuf=0L;
	instance->src2.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->p_delay2,"0 sv",UGRP_ABSTIME,ATME_SAMPLES);
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src1),modWin,modGadgets,GADIX_SRC1);
	SetSource   (&(instance->src2),modWin,modGadgets,GADIX_SRC2);
	GT_SetGadgetAttrs(modGadgets[GADIX_DELAY2],modWin,0l,GTST_String,instance->p_delay2,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
