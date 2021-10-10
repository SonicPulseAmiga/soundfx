/****h* ChannelJoin/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (29.10.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    ChannelJoin Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    29.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    01.Aug.2000	V 1.0	most recent version
*    09.Nov.1999	V 0.0	initial version
*  NOTES
*
*  IDEAS
*******
*/

#define ChannelJoin_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "ChannelJoin.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_SRC1		(GADID_BASE+1)
#define GADIX_SRC1		(GADIX_BASE+1)
#define GADID_SRC2		(GADID_SRC1+SRC_IDCT)
#define GADIX_SRC2		(GADIX_SRC1+SRC_IXCT)

#define LAST_GADIX		(GADIX_SRC2+SRC_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[3];
};

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int SAVEDS ASM __UserLibInit(REG(a6,struct Library *SFXModBase)){
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
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+STDFRM_WIDTH;
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

		g=AddSource(&(instance->src1),modGadgets,g,GADID_SRC1,GADIX_SRC1,xs1,ys1);
		g=AddSource(&(instance->src2),modGadgets,g,GADID_SRC2,GADIX_SRC2,xs1,ys2);

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source 1"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Source 2"		,xs1,ys2,STDFRM_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);					/* Go */

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
								&& !HandleSource(&(instance->src2),modWin,modGadgets,GADID_SRC2,GADIX_SRC2,gnr,icode)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
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
	//-- src & dst buffers
	SInfo *dstbuf=NULL,*srcbuf1,*srcbuf2;
	//-- administrative
	UBYTE dstch;
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
	instance->slen=srcbuf1->slen;
	instance->srat=srcbuf1->srat;

	if((srcbuf1->channels==srcbuf2->channels) && (srcbuf1->slen==srcbuf2->slen) && (srcbuf1->channels<CH_QUADRO)) {
		dstch=CH_DOUBLE(srcbuf1->channels);
		if(!dstbuf) {
			if(dstbuf=NewSample()) {
				if(AllocSampleMem(dstbuf,srcbuf1->slen,dstch+1)) {
					BufferFill(srcbuf1,dstbuf);
					dstbuf->channels=dstch;
					dstbuf->aktch=dstbuf->channels+1;
					newdst=TRUE;
				}
				else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
			}
			else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
			if(activate((SFXCoreInstance *)instance)) {
				activate_ch((SFXCoreInstance *)instance,0);
				sprintf(title,GetString(msgProgress),PRJ_NAME,1,1);
				StartProWin(pwd,(ULONG *)(&instance->curlen),title,(dstch+1));					// den Statusbalken starten
				switch(dstch) {
					case CH_STEREO:
						dstbuf->channelmask=CH_LEFT|CH_RIGHT;
						CopyDataCh(srcbuf1,dstbuf,0,0,0,srcbuf1->slen);instance->curlen++;
						CopyDataCh(srcbuf2,dstbuf,1,0,0,srcbuf2->slen);instance->curlen++;
						break;
					case CH_QUADRO:
						dstbuf->channelmask=CH_LEFT|CH_RIGHT|CH_FRONT|CH_BACK;
						CopyDataCh(srcbuf1,dstbuf,0,0,0,srcbuf1->slen);instance->curlen++;
						CopyDataCh(srcbuf1,dstbuf,1,0,0,srcbuf1->slen);instance->curlen++;
						CopyDataCh(srcbuf2,dstbuf,2,0,0,srcbuf2->slen);instance->curlen++;
						CopyDataCh(srcbuf2,dstbuf,3,0,0,srcbuf2->slen);instance->curlen++;
						break;
				}
				deactivate_ch((SFXCoreInstance *)instance,0);
				if(okay && newdst) AddSample(dstbuf,JoinSampleNames(((struct Node *)srcbuf1)->ln_Name,((struct Node *)srcbuf2)->ln_Name));
			}
			else okay=FALSE;
			deactivate((SFXCoreInstance *)instance);
			DelProWin(pwd);
		}
		else okay=FALSE;
	}
	else { /* @todo Message(); */okay=FALSE; }
	if(!okay) {
		if(newdst) RemSample(dstbuf);
		return(FALSE);
	}
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=0/*,pf*/;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;/*pf=0;*/
		//if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;/*pf=0;*/
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src1.srcbuf=0L;
	instance->src1.rmode=RNGMD_ALL;
	instance->src2.srcbuf=0L;
	instance->src2.rmode=RNGMD_ALL;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src1),modWin,modGadgets,GADIX_SRC1);
	SetSource   (&(instance->src2),modWin,modGadgets,GADIX_SRC2);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
