/****h* Analyse-Data/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (29.10.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Analyse-Data Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    17.Jul.1998
*  MODIFICATION HISTORY
*    29.Oct.2001	V 4.2	most recent version
*    09.Apr.2001	V 4.1	most recent version
*							A: when callmd==CM_REXX save result to logfile t:analyse-data.log
*    01.Aug.2000	V 1.2	most recent version
*    17.Jul.1998	V 1.2	initial version
*  NOTES
*  IDEAS
*    * Tabs over diagraph
*      * Amplitude/Delta Histographs
*      * Amplitude/Delta Envelopes
*    * Average Delta
*
*******
*/

#define ANALYSE_DATA_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Analyse-Data.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

char *ToYUnit(UBYTE unity,double val);
void StoreResults(struct Instance *instance,SInfo *srcbuf);
void DisplayResults(struct Instance *instance,SInfo *srcbuf);

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_SRC		(GADID_BASE+1)
#define GADIX_SRC		(GADIX_BASE+1)

#define LAST_GADIX		(GADIX_SRC+SRC_IXCT)


#define GADID_HEAD		0
#define GADID_MAXA		(GADID_HEAD+1)
#define GADID_MAXD		(GADID_MAXA+1)
#define GADID_MINS		(GADID_MAXD+1)
#define GADID_MAXS		(GADID_MINS+1)
#define GADID_AVGA		(GADID_MAXS+1)
#define GADID_RMSA		(GADID_AVGA+1)
#define GADID_AVGD		(GADID_RMSA+1)
#define GADID_DC		(GADID_AVGD+1)
#define GADID_PCS		(GADID_DC+1)

#define	GADID_CHANNEL	0

#define GADID_CH0		(GADID_CHANNEL+1)
#define GADID_CH1		(GADID_CH0+GADID_PCS+1)
#define GADID_CH2		(GADID_CH1+GADID_PCS+1)
#define GADID_CH3		(GADID_CH2+GADID_PCS+1)
#define GADID_CHA		(GADID_CH3+GADID_PCS+1)

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
	//-- loop
	UBYTE l;	// channels
	//-- prowin
	PWData *pwd;
	ProWinTitle title;
	//-- procesing length
	ULONG rm_start,rm_len;
	//-- src & dst buffers
	SInfo *srcbuf;
	//-- buffer access
	SmpAccess *srcsa;
	SAFuncPtr SARead;
	ULONG blen;
	//-- administrative
	UBYTE okay=TRUE;

	INTRO;

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

	if((pwd=NewProWin())) {					// ein neues asynchrones Statusfenster öffnen
		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
				if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {
					activate_ch((SFXCoreInstance *)instance,l);
					if((srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {	// sequentiell Blöcke konstanter Größe lesen
						SARead =srcsa->safunc;
						blen=Bytes2Samples(srcsa->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(srcbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);
						for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++) {
							instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
							process((SFXCoreInstance *)instance,blen);
						}
						SADone(srcsa,srcbuf);
					}
					else okay=FALSE;
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }					// es wurde abgebrochen
				}
			}
		}
		else okay=FALSE;
		deactivate((SFXCoreInstance *)instance);
		DelProWin(pwd);

		if(okay) {
			MSG("calc ready, going to output the results");
			//-- which method is better ?
			//if(RunTime->quiet) {
			if(pdata->callmd==CM_DIRECT) StoreResults(instance,srcbuf);
			else DisplayResults(instance,srcbuf);
		}
	}
	else okay=FALSE;
	if(!okay) {
		return(FALSE);
	}
	OUTRO;
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

char *ToYUnit(UBYTE unity,double val) {
	static char text[PARBUFLEN+1];

	switch(unity) {
		case AMP_FC:		sprintf(text,"%+6.4lf",val/32768.0);break;
		case AMP_ABSPROZ:	sprintf(text,"%+6ld %",(LONG)(val/327.68));break;
		case AMP_ABSPROM:	sprintf(text,"%+6ld %%",(LONG)(val/32.768));break;
		case AMP_LEVEL:		sprintf(text,"%+6ld",(LONG)val);break;
		case AMP_DEZIBEL:
			if(val!=0.0) sprintf(text,"%+6.4lf db",(3.0*log(fabs(val/32768.0)))/log(1.41421356237));
			else sprintf(text,"   inf db");
			break;
	}
	return(text);
}

void StoreResults(struct Instance *instance,SInfo *srcbuf) { 
	FILE *log;
	UBYTE newfile=TRUE;
	UBYTE l;
	char fn[FILENAME_MAX];

	strmfp(fn,RunTime->sfxprefs_sbuf->sample_sa_path,"Analyse-Data.log");

	if(log=fopen(fn,"rb")) {
		newfile=FALSE;
		fclose(log);
	}

	// save data to logfile t:analyse-data.log
	if(log=fopen(fn,"ab")) {
		if(newfile) {
			fprintf(log,"+------------------------+----------+----------+----------+----------+----------+----------+----------+----------+\n");
			fprintf(log,"| Sample                 | MaxAmp   | MaxDiff  | MinSamp  | MaxSamp  | AvgAmp   | RmsAmp   | AvgDiff  | DC-Off   |\n");
			fprintf(log,"+------------------------+----------+----------+----------+----------+----------+----------+----------+----------+\n");
		}
		/** @todo should we use db values as well? */
		for(l=0;l<CH_COUNT(srcbuf->channels);l++) {
			if(IS_CHANNEL_ACTIVE(l,srcbuf->channelmask)) {
				fprintf(log,"| %20s.%1d | %+8ld | %+8ld | %+8ld | %+8ld | %+8ld | %+8ld | %+8ld | %+8ld |\n",
					((struct Node *)srcbuf)->ln_Name,l,
					instance->maxa[l],instance->maxd[l],
					instance->mins[l],instance->maxs[l],
					(LONG)(32767.0*instance->avg[l]),(LONG)(32767.0*instance->rms[l]),
					instance->avgdif[l],(LONG)(32767.0*instance->dc[l]));
			}
		}
		fprintf(log,"+------------------------+----------+----------+----------+----------+----------+----------+----------+----------+\n");
		fclose(log);
	}
}

void DisplayResults(struct Instance *instance,SInfo *srcbuf) {
	struct NewGadget 	ng;
	struct Window		*modWin;
	struct Gadget		*g;
	struct Gadget		*modGList=0l;
	struct RastPort		*modrp;
	UWORD ys1=2,		yh1=8+64+(2*RunTime->scy[6])+8+64+(2*RunTime->scy[6])+8;
	UWORD ys2=ys1+1+yh1,yh2=RunTime->scy[3]+RunTime->scy[11];
	UWORD ys3=ys2+1+yh2,yh3=RunTime->scy[3]+10*RunTime->scy[11];						// text fields
	UWORD winh=ys3+2+yh3;
	UWORD xs1=2;						/* 4 pixel rahmen */
	UWORD winw=xs1+2+10+512+7+10;		/* 256 values * 2 + 7 pixel rand + 10 pixel linker rand + 10 pixel rechter rand */ 
	UWORD as_xs,as_ys,as_ys2;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	UBYTE runing=TRUE;
	char scala[100],title[200];
	ULONG *ptr;
	UWORD j;
	UBYTE ch;
	ULONG val;

	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_CLOSEGADGET|WFLG_DEPTHGADGET|WFLG_GIMMEZEROZERO,
		WA_Gadgets,			0l,
		WA_Title,			0l,
		WA_ScreenTitle,		(ULONG)PRJ_VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};
	UBYTE *ChanLabels[]={ "Channel 1","Channel 2","Channel 3","Channel 4","All Channels",0l };

	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[150];
		ng.ng_TopEdge		=ys2+RunTime->scy[2];
		ng.ng_Width			=RunTime->scx[150];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	="Graph for";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_CHANNEL;
		ng.ng_Flags			=PLACETEXT_LEFT;
		ng.ng_VisualInfo	=RunTime->VisualInfo;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ChanLabels,GTCY_Active,0,TAG_DONE);

		// Channel 0
			as_ys2=RunTime->scy[11];
			as_ys=ys3+RunTime->scy[2];
			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_Width			=RunTime->scx[70];
			ng.ng_GadgetText	="Channel";
			ng.ng_GadgetID		=GADID_CH0+GADID_HEAD;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,GTTX_Text,"0",GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetText	="MaxAmp";
			ng.ng_GadgetID		=GADID_CH0+GADID_MAXA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxa[0]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetText	="MaxDif";
			ng.ng_GadgetID		=GADID_CH0+GADID_MAXD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxd[0]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetText	="MinVal";
			ng.ng_GadgetID		=GADID_CH0+GADID_MINS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->mins[0]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetText	="MaxVal";
			ng.ng_GadgetID		=GADID_CH0+GADID_MAXS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxs[0]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetText	="AvgVol";
			ng.ng_GadgetID		=GADID_CH0+GADID_AVGA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->avg[0])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetText	="RMSVol";
			ng.ng_GadgetID		=GADID_CH0+GADID_RMSA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->rms[0])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetText	="AvgDif";
			ng.ng_GadgetID		=GADID_CH0+GADID_AVGD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->avgdif[0]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetText	="DC Offset";
			ng.ng_GadgetID		=GADID_CH0+GADID_DC;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->dc[0])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;
			ng.ng_GadgetText	="Prob. clipped Samples";
			ng.ng_GadgetID		=GADID_CH0+GADID_PCS;
			sprintf(scala,"%d",instance->pcs[0]);
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

		// Channel 1
			as_ys=ys3+RunTime->scy[2];
			ng.ng_LeftEdge		=xs1+RunTime->scx[225];
			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetText	=0l;
			ng.ng_GadgetID		=GADID_CH1+GADID_HEAD;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,GTTX_Text,"1",GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH1+GADID_MAXA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxa[1]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH1+GADID_MAXD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxd[1]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH1+GADID_MINS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->mins[1]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH1+GADID_MAXS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxs[1]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH1+GADID_AVGA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->avg[1])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH1+GADID_RMSA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->rms[1])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH1+GADID_AVGD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->avgdif[1]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH1+GADID_DC;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->dc[1])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;
			ng.ng_GadgetID		=GADID_CH1+GADID_PCS;
			sprintf(scala,"%d",instance->pcs[1]);
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

		// Channel 2
			as_ys=ys3+RunTime->scy[2];
			ng.ng_LeftEdge		=xs1+RunTime->scx[300];
			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH2+GADID_HEAD;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,GTTX_Text,"2",GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH2+GADID_MAXA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxa[2]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH2+GADID_MAXD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxd[2]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH2+GADID_MINS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->mins[2]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH2+GADID_MAXS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxs[2]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH2+GADID_AVGA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->avg[2])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH2+GADID_RMSA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->rms[2])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH2+GADID_AVGD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->avgdif[2]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH2+GADID_DC;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->dc[2])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;
			ng.ng_GadgetID		=GADID_CH2+GADID_PCS;
			sprintf(scala,"%d",instance->pcs[2]);
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

		// Channel 3
			as_ys=ys3+RunTime->scy[2];
			ng.ng_LeftEdge		=xs1+RunTime->scx[375];
			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH3+GADID_HEAD;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,GTTX_Text,"3",GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH3+GADID_MAXA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxa[3]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH3+GADID_MAXD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxd[3]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH3+GADID_MINS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->mins[3]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH3+GADID_MAXS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->maxs[3]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH3+GADID_AVGA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->avg[3])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH3+GADID_RMSA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->rms[3])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH3+GADID_AVGD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)instance->avgdif[3]));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CH3+GADID_DC;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*instance->dc[3])));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;
			ng.ng_GadgetID		=GADID_CH3+GADID_PCS;
			sprintf(scala,"%d",instance->pcs[3]);
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

		// All Channels
			as_ys=ys3+RunTime->scy[2];
			ng.ng_LeftEdge		=xs1+RunTime->scx[450];
			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CHA+GADID_HEAD;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,GTTX_Text,"All",GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CHA+GADID_MAXA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)(instance->maxa[0]+instance->maxa[1]+instance->maxa[2]+instance->maxa[3])/instance->numch));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CHA+GADID_MAXD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)(instance->maxd[1]+instance->maxd[0]+instance->maxd[2]+instance->maxd[3])/instance->numch));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CHA+GADID_MINS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)(instance->mins[0]+instance->mins[1]+instance->mins[2]+instance->mins[3])/instance->numch));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CHA+GADID_MAXS;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)(instance->maxs[0]+instance->maxs[1]+instance->maxs[2]+instance->maxs[3])/instance->numch));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CHA+GADID_AVGA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*(instance->avg[0]+instance->avg[1]+instance->avg[2]+instance->avg[3])/instance->numch)));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CHA+GADID_RMSA;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*(instance->rms[0]+instance->rms[1]+instance->rms[2]+instance->rms[3])/instance->numch)));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CHA+GADID_AVGD;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(double)(instance->avgdif[0]+instance->avgdif[1]+instance->avgdif[2]+instance->avgdif[3])/instance->numch));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;as_ys+=as_ys2;
			ng.ng_GadgetID		=GADID_CHA+GADID_DC;
			sprintf(scala,"%s",ToYUnit(srcbuf->unity,(32767.0*(instance->dc[0]+instance->dc[1]+instance->dc[2]+instance->dc[3])/instance->numch)));
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

			ng.ng_TopEdge		=as_ys;
			ng.ng_GadgetID		=GADID_CHA+GADID_PCS;
			sprintf(scala,"%d",instance->pcs[0]+instance->pcs[1]+instance->pcs[2]+instance->pcs[3]);
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,scala,GTTX_CopyText,TRUE,TAG_DONE);

		if(g) {
			wintags[0].ti_Data=(RunTime->xres-winw)>>1;						/* Displaying data */
			wintags[1].ti_Data=(RunTime->yres-winh)>>1;
			wintags[2].ti_Data=winw;
			wintags[3].ti_Data=winh;
			wintags[6].ti_Data=(ULONG)modGList;
			sprintf(title,PRJ_NAME" : %s",((struct Node *)srcbuf)->ln_Name);
			wintags[7].ti_Data=(ULONG)title;
			if(RunTime->sfxpubscr==IntuitionBase->FirstScreen) wintags[5].ti_Data|=WFLG_ACTIVATE;
			if(modWin=OpenWindowTagList(0l,wintags)) {
				GT_RefreshWindow(modWin,0l);
				modrp=modWin->RPort;
				SetFont(modrp,RunTime->scrfont);
				DrawGadWB(modrp,xs1,ys1,winw-4,yh1);
				DrawGadWB(modrp,xs1,ys2,winw-4,yh2);
				DrawGadWB(modrp,xs1,ys3,winw-4,yh3);
				SetAPen(modrp,1);RectFill(modrp,xs1+2,ys1+2,winw-5,ys1+(yh1-3));
				SetABPenDrMd(modrp,2,1,JAM1);

				as_xs=10;as_ys=8;
				as_ys2=as_ys+64;													// Amp Histogram
				SetAPen(modrp,5);
				Move(modrp,as_xs,as_ys2+ 1);Draw(modrp,as_xs+519,as_ys2+ 1);
				Move(modrp,as_xs,as_ys2-32);Draw(modrp,as_xs+519,as_ys2-32);
				Move(modrp,as_xs,as_ys2-64);Draw(modrp,as_xs+519,as_ys2-64);
				SetAPen(modrp,2);
				Move(modrp,as_xs,as_ys2+2+RunTime->scy[6]);Text(modrp,"0",1);
				Move(modrp,as_xs+(519-RunTime->scx[18]),as_ys2+2+RunTime->scy[6]);Text(modrp,"100",3);
				Move(modrp,as_xs+(519-RunTime->scx[54]),as_ys2+2+RunTime->scy[12]);Text(modrp,"Amplitude",9);
				ptr=instance->hista[0];
				for(j=0;j<128;j++,ptr++) {
					if(*ptr>0) RectFill(modrp,as_xs+(j<<2),as_ys2-*ptr,as_xs+(j<<2)+2,as_ys2);
					else { Move(modrp,as_xs+(j<<2),as_ys+64);Draw(modrp,as_xs+(j<<2)+2,as_ys+64); }
				}
				as_ys2=as_ys+138+RunTime->scy[12];									// Delta Histogram
				SetAPen(modrp,5);
				Move(modrp,as_xs,as_ys2+ 1);Draw(modrp,as_xs+519,as_ys2+ 1);
				Move(modrp,as_xs,as_ys2-32);Draw(modrp,as_xs+519,as_ys2-32);
				Move(modrp,as_xs,as_ys2-64);Draw(modrp,as_xs+519,as_ys2-64);
				SetAPen(modrp,2);
				Move(modrp,as_xs,as_ys2+2+RunTime->scy[6]);Text(modrp,"0",1);
				Move(modrp,as_xs+(519-RunTime->scx[18]),as_ys2+2+RunTime->scy[6]);Text(modrp,"100",3);
				Move(modrp,as_xs+(519-RunTime->scx[30]),as_ys2+2+RunTime->scy[12]);Text(modrp,"Delta",5);
				ptr=instance->histd[0];
				for(j=0;j<256;j++) {
					Move(modrp,as_xs+(j<<1),as_ys2-*(ptr++));Draw(modrp,as_xs+(j<<1),as_ys2);
				}

				while(runing) {
					WaitPort(modWin->UserPort);
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						switch(iclass) {
							case IDCMP_CLOSEWINDOW:	runing=FALSE;break;
							case IDCMP_GADGETUP:
								switch((ULONG)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_CHANNEL:
										ch=icode;
										if(ch<4) {
											as_xs=10;as_ys=8;
											as_ys2=as_ys+64;													// Amp Histogram
											SetAPen(modrp,1);RectFill(modrp,as_xs+1,as_ys2-63,as_xs+(128<<2),as_ys2-1);
											SetAPen(modrp,5);
											Move(modrp,as_xs,as_ys2-32);Draw(modrp,as_xs+519,as_ys2-32);
											SetAPen(modrp,2);
											ptr=instance->hista[ch];
											for(j=0;j<128;j++,ptr++) {
												if(*ptr>0) RectFill(modrp,as_xs+(j<<2),as_ys2-*ptr,as_xs+(j<<2)+2,as_ys2);
												else { Move(modrp,as_xs+(j<<2),as_ys+64);Draw(modrp,as_xs+(j<<2)+2,as_ys+64); }
											}
											as_ys2=as_ys+138+RunTime->scy[12];									// Delta Histogram
											SetAPen(modrp,1);RectFill(modrp,as_xs+1,as_ys2-63,as_xs+(256<<1),as_ys2-1);
											SetAPen(modrp,5);
											Move(modrp,as_xs,as_ys2-32);Draw(modrp,as_xs+519,as_ys2-32);
											SetAPen(modrp,2);
											ptr=instance->histd[ch];
											for(j=0;j<256;j++) {
												Move(modrp,as_xs+(j<<1),as_ys2-*(ptr++));Draw(modrp,as_xs+(j<<1),as_ys2);
											}
										}
										else {
											as_xs=10;as_ys=8;
											as_ys2=as_ys+64;													// Amp Histogram
											SetAPen(modrp,1);RectFill(modrp,as_xs+1,as_ys2-63,as_xs+(128<<2),as_ys2-1);
											SetAPen(modrp,5);
											Move(modrp,as_xs,as_ys2-32);Draw(modrp,as_xs+519,as_ys2-32);
											SetAPen(modrp,2);
											for(j=0;j<128;j++) {
												val=(instance->hista[0][j]+instance->hista[1][j]+instance->hista[2][j]+instance->hista[3][j])/instance->numch;
												if(val>0) RectFill(modrp,as_xs+(j<<2),as_ys2-val,as_xs+(j<<2)+2,as_ys2);
												else { Move(modrp,as_xs+(j<<2),as_ys+64);Draw(modrp,as_xs+(j<<2)+2,as_ys+64); }
											}
											as_ys2=as_ys+138+RunTime->scy[12];									// Delta Histogram
											SetAPen(modrp,1);RectFill(modrp,as_xs+1,as_ys2-63,as_xs+(256<<1),as_ys2-1);
											SetAPen(modrp,5);
											Move(modrp,as_xs,as_ys2-32);Draw(modrp,as_xs+519,as_ys2-32);
											SetAPen(modrp,2);
											for(j=0;j<256;j++) {
												val=(instance->histd[0][j]+instance->histd[1][j]+instance->histd[2][j]+instance->histd[3][j])/instance->numch;
												Move(modrp,as_xs+(j<<1),as_ys2-val);Draw(modrp,as_xs+(j<<1),as_ys2);
											}							
										}
										break;
								}
								break;
						}
					}
				}
				CloseWindow(modWin);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		FreeGadgets(modGList);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
}
//-- eof ----------------------------------------------------------------------
