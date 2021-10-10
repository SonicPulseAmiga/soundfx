/****h* Analyse-Stereo/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (29.10.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Analyse-Stereo Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Apr.2000
*  MODIFICATION HISTORY
*    29.Oct.2001	V 4.2	most recent version
*    17.Jan.2001	V 4.1	most recent version
*    01.Aug.2000	V 1.0	most recent version
*    18.Apr.2000	V 1.0	initial version
*  NOTES
*
*******
*/

#define ANALYSE_STEREO_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Analyse-Stereo.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

void DisplayResults(struct Instance *instance,SInfo *srcbuf);

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
	//-- procesing length
	ULONG rm_start,rm_len;
	//-- src & dst buffers
	SInfo *srcbuf;
	//-- buffer access
	SmpAccess *srcsa0,*srcsa1;
	SAFuncPtr SARead;
	ULONG blen;
	//-- administrative
	UBYTE okay=TRUE;

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:				// wir wurden e.g. per ARexx gestartet
		case CM_STREAM:
			UnlockBuffer(instance->src.srcbuf);
			if((instance->src.srcbuf=LockBuffer(pdata->src[0]))==NULL) return(FALSE);
			instance->src.rmode=RNGMD_ALL;
			break;
		case CM_PREHEAR:
			break;
	}

	srcbuf=instance->src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,instance->src.rmode);
	instance->slen=rm_len;
	instance->srat=srcbuf->srat;

	if(srcbuf->channels==CH_STEREO) {
		if((pwd=NewProWin())) {					// ein neues asynchrones Statusfenster öffnen
			if(activate((SFXCoreInstance *)instance)) {
				activate_ch((SFXCoreInstance *)instance,0);
				srcsa0=SAInit(srcbuf,0,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
				srcsa1=SAInit(srcbuf,1,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
				if(srcsa0 && srcsa1) {
					SARead =srcsa0->safunc;
					blen=Bytes2Samples(srcsa0->blen);
					sprintf(title,GetString(msgProgress),PRJ_NAME,1,1);
					StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);
					for(srcsa0->seg=srcsa1->seg=0;((srcsa0->seg<srcsa0->seganz) && (instance->curlen<instance->slen));srcsa0->seg++,srcsa1->seg++) {
						instance->sbuf0=(SAMPLE *)srcsa0->buffer1;SARead(srcsa0,srcsa0->blen);	// Pointer auf src-Block holen & nächstes Read starten
						instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & nächstes Read starten
						process((SFXCoreInstance *)instance,blen);
					}
				}
				else okay=FALSE;
				if(srcsa0) SADone(srcsa0,srcbuf);
				if(srcsa1) SADone(srcsa1,srcbuf);
				deactivate_ch((SFXCoreInstance *)instance,0);
				if(ProWinAborted(instance->curlen)) { okay=FALSE; }					// es wurde abgebrochen
			}
			else okay=FALSE;
			deactivate((SFXCoreInstance *)instance);
			DelProWin(pwd);

			if(okay) {
				MSG("calc ready, going to output the results");
				/** @todo if(pdata->callmd==CM_DIRECT) SaveImage(); */
				DisplayResults(instance,srcbuf);
			}
		}
		else okay=FALSE;
	}
	else { Message(errSourceStereo,NULL,__FILE__,__LINE__);okay=FALSE; }
	if(!okay) {
		return(FALSE);
	}
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=0;/*,pf;*/

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;/*pf=0;*/
		//if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;/*pf=0;*/
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

/*-- local Stuff --------------------------------------------------------------------------------*/

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

void DisplayResults(struct Instance *instance,SInfo *srcbuf) {
	register ULONG j;
	register WORD k;
	ULONG *ptr,mv=0;
	struct Window		*modWin;
	struct RastPort		*modrp;
	UWORD ys1=2,		yh1=256+(4*RunTime->scy[6]);
	UWORD winh=ys1+2+yh1;
	UWORD xs1=2,		xh1=320+(4*RunTime->scx[6]);
	UWORD winw=xs1+2+xh1;
	UWORD as_xs,as_ys;
	WORD xp,yp;
	double ang,rad,radfc,angfc;
	struct IntuiMessage *imsg;
	ULONG iclass;
	UBYTE runing=TRUE;
	char title[200];

	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			IDCMP_RAWKEY|IDCMP_CLOSEWINDOW,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_CLOSEGADGET|WFLG_DEPTHGADGET|WFLG_GIMMEZEROZERO,
		WA_Title,			0l,
		WA_ScreenTitle,		(ULONG)PRJ_VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		TAG_DONE
	};

	// Normalizing data
	mv=0;ptr=instance->sum;
	for(j=0;j<513;j++,ptr++) if(*ptr>mv) mv=*ptr;

	if(mv) {
		wintags[0].ti_Data=(RunTime->xres-winw)>>1;						/* Displaying data */
		wintags[1].ti_Data=(RunTime->yres-winh)>>1;
		wintags[2].ti_Data=winw;
		wintags[3].ti_Data=winh;
		sprintf(title,PRJ_NAME" : %s",((struct Node *)srcbuf)->ln_Name);
		wintags[6].ti_Data=(ULONG)title;
		if(RunTime->sfxpubscr==IntuitionBase->FirstScreen) wintags[5].ti_Data|=WFLG_ACTIVATE;
		if(modWin=OpenWindowTagList(0l,wintags)) {
			modrp=modWin->RPort;
			SetFont(modrp,RunTime->scrfont);
			DrawGadWB(modrp,xs1,ys1,xh1,yh1);
			SetAPen(modrp,1);RectFill(modrp,xs1+2,ys1+2,winw-5,winh-5);
			SetABPenDrMd(modrp,2,1,JAM1);

			as_ys=ys1+(2*RunTime->scy[6])+128;								// draw axis
			as_xs=xs1+(2*RunTime->scx[6])+160;
			SetAPen(modrp,6);
			Move(modrp,as_xs,as_ys-128);Draw(modrp,as_xs,as_ys+128);
			Move(modrp,as_xs-160,as_ys);Draw(modrp,as_xs+160,as_ys);
			SetAPen(modrp,2);
			Move(modrp,as_xs-(2*RunTime->scx[6]),as_ys+4-(128+RunTime->scy[6]));Text(modrp,"Center",6);
			Move(modrp,as_xs-(3*RunTime->scx[6]),as_ys+4+(128+RunTime->scy[6]));Text(modrp,"Wide",4);
			Move(modrp,as_xs-(162+RunTime->scx[6]),as_ys+2);Text(modrp,"L",1);
			Move(modrp,as_xs+(162                ),as_ys+2);Text(modrp,"R",1);
			ptr=instance->sum;radfc=128.0/(double)mv;angfc=(PI/(double)RES);
			//MSG1("Maxval : %6ld",mv);
			for(k=-RES;k<=RES;k++,ptr++) {									// plot data
				//-- needles
				ang=((double)(k))*angfc;
				rad=((double)*ptr)*radfc;	/** @todo apply gamma */
				xp=(LONG)(rad*sin(ang)*1.25);
				yp=(LONG)(rad*cos(ang));
				Move(modrp,as_xs,as_ys);Draw(modrp,as_xs+xp,as_ys-yp);
				//-- outer circle
				/** @todo change color depending on value of 'rad' */
				xp=(LONG)(160.0*sin(ang));
				yp=(LONG)(128.0*cos(ang));
				WritePixel(modrp,as_xs+xp,as_ys-yp);
			}

			while(runing) {
				WaitPort(modWin->UserPort);
				while(imsg=GT_GetIMsg(modWin->UserPort)) {
					iclass	=imsg->Class;
					GT_ReplyIMsg(imsg);
					switch(iclass) {
						case IDCMP_CLOSEWINDOW:	runing=FALSE;break;
					}
				}
			}
			CloseWindow(modWin);
		}
		else Message(errOpenWindow,NULL,__FILE__,__LINE__);
	}
}
//-- eof ----------------------------------------------------------------------
