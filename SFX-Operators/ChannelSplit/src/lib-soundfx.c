/****h* ChannelSplit/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (29.10.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    ChannelSplit Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    29.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    01.Aug.2000	V 1.0	most recent version
*    07.Nov.1999	V 0.0	initial version
*  NOTES
*
*  IDEAS
*******
*/

#define ChannelSplit_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "ChannelSplit.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_SRC		(GADID_BASE+1)
#define GADIX_SRC		(GADIX_BASE+1)

#define LAST_GADIX		(GADIX_SRC+SRC_IXCT)

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
	UWORD winh=ys2+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+STDFRM_WIDTH;
	UWORD xh;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[104];
		ng.ng_TopEdge		=ys2+RunTime->scy[2];
		ng.ng_Width			=RunTime->scx[100];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	=GetString(btnProcess);
		ng.ng_TextAttr		=RunTime->ScrFontB;
		ng.ng_GadgetID		=GADID_GO;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime->VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_GO]=g;

		g=AddSource(&(instance->src),modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,CMDFRM_HEIGHT);					/* Go */

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
								if(!HandleSource(&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)) {
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
	SInfo *dstbuf0=NULL,*dstbuf1=NULL,*dstbuf2=NULL,*dstbuf3=NULL,*srcbuf;
	//-- administrative
	SampleName name;
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
			dstbuf0=pdata->dst[0];
			dstbuf1=pdata->dst[1];
			dstbuf2=pdata->dst[2];
			dstbuf3=pdata->dst[3];
			break;
		case CM_PREHEAR:
			break;
	}

	srcbuf=instance->src.srcbuf;
	instance->slen=srcbuf->slen;
	instance->srat=srcbuf->srat;

	switch(srcbuf->channels) {
		case CH_MONO:
			Message(errSourceStereoQuadro,NULL,__FILE__,__LINE__);
			okay=FALSE;
			break;
		case CH_STEREO:
			if(!(dstbuf0=NewSample()) || !(dstbuf1=NewSample())) { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);okay=FALSE; }
			if(okay) {
				BufferFill(srcbuf,dstbuf0);
				BufferFill(srcbuf,dstbuf1);
				dstbuf0->channels=dstbuf1->channels=CH_MONO;
				dstbuf0->channelmask=dstbuf1->channelmask=CH_CENTER;
				dstbuf0->aktch=dstbuf0->channels+1;
				dstbuf1->aktch=dstbuf1->channels+1;
				if(!AllocSampleMem(dstbuf0,dstbuf0->slen,1) || !AllocSampleMem(dstbuf1,dstbuf1->slen,1)) { Message(errAllocateMemory,NULL,__FILE__,__LINE__);okay=FALSE; }
			}
			if(okay) {
				if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
					if(activate((SFXCoreInstance *)instance)) {
						activate_ch((SFXCoreInstance *)instance,0);
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,2);					// den Statusbalken starten
						CopyDataCh(srcbuf,dstbuf0,0,0,0,srcbuf->slen);instance->curlen++;
						CopyDataCh(srcbuf,dstbuf1,1,0,0,srcbuf->slen);instance->curlen++;
						deactivate_ch((SFXCoreInstance *)instance,0);
						sprintf(name,"%s.0",((struct Node *)srcbuf)->ln_Name);AddSample(dstbuf0,name);
						sprintf(name,"%s.1",((struct Node *)srcbuf)->ln_Name);AddSample(dstbuf1,name);
					}
					else okay=FALSE;
					deactivate((SFXCoreInstance *)instance);
					DelProWin(pwd);
				}
				else okay=FALSE;
			}
			break;
		case CH_QUADRO:
			if(!(dstbuf0=NewSample()) || !(dstbuf1=NewSample()) || !(dstbuf2=NewSample()) || !(dstbuf3=NewSample())) { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);okay=FALSE; }
			if(okay) {
				BufferFill(srcbuf,dstbuf0);
				BufferFill(srcbuf,dstbuf1);
				BufferFill(srcbuf,dstbuf2);
				BufferFill(srcbuf,dstbuf3);
				dstbuf0->channels=dstbuf1->channels=dstbuf2->channels=dstbuf3->channels=CH_MONO;
				dstbuf0->channelmask=dstbuf1->channelmask=dstbuf2->channelmask=dstbuf3->channelmask=CH_CENTER;
				dstbuf0->aktch=dstbuf0->channels+1;
				dstbuf1->aktch=dstbuf1->channels+1;
				dstbuf2->aktch=dstbuf2->channels+1;
				dstbuf3->aktch=dstbuf3->channels+1;
				if(!AllocSampleMem(dstbuf0,dstbuf0->slen,1) || !AllocSampleMem(dstbuf1,dstbuf1->slen,1) || !AllocSampleMem(dstbuf2,dstbuf2->slen,1) || !AllocSampleMem(dstbuf3,dstbuf3->slen,1)) { Message(errAllocateMemory,NULL,__FILE__,__LINE__);okay=FALSE; }
			}
			if(okay) {
				if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
					if(activate((SFXCoreInstance *)instance)) {
						activate_ch((SFXCoreInstance *)instance,0);
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,4);					// den Statusbalken starten
						CopyDataCh(srcbuf,dstbuf0,0,0,0,srcbuf->slen);instance->curlen++;
						CopyDataCh(srcbuf,dstbuf1,1,0,0,srcbuf->slen);instance->curlen++;
						CopyDataCh(srcbuf,dstbuf2,2,0,0,srcbuf->slen);instance->curlen++;
						CopyDataCh(srcbuf,dstbuf3,3,0,0,srcbuf->slen);instance->curlen++;
						deactivate_ch((SFXCoreInstance *)instance,0);
						sprintf(name,"%s.0",((struct Node *)srcbuf)->ln_Name);AddSample(dstbuf0,name);
						sprintf(name,"%s.1",((struct Node *)srcbuf)->ln_Name);AddSample(dstbuf1,name);
						sprintf(name,"%s.2",((struct Node *)srcbuf)->ln_Name);AddSample(dstbuf2,name);
						sprintf(name,"%s.3",((struct Node *)srcbuf)->ln_Name);AddSample(dstbuf3,name);
					}
					else okay=FALSE;
					deactivate((SFXCoreInstance *)instance);
					DelProWin(pwd);
				}
				else okay=FALSE;
			}
			break;
		default:
			okay=FALSE;
			break;
	}
	if(!okay) {
		if(dstbuf0) RemSample(dstbuf0);
		if(dstbuf1) RemSample(dstbuf1);
		if(dstbuf2) RemSample(dstbuf2);
		if(dstbuf3) RemSample(dstbuf3);
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
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
