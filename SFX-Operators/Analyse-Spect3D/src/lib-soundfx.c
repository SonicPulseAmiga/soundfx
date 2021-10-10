/****h* Analyse-Spect3D/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (31.10.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Analyse-Spect3D Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    31.Oct.2001	V 4.2	most recent version
*    17.Jan.2001	V 4.1	most recent version
*    01.Aug.2000	V 1.2	most recent version
*    18.Jul.1998	V 1.2	initial version
*  NOTES
*
*******
*/

#define ANALYSE_SPECT3D_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Analyse-Spect3D.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

UBYTE AnalyseFFT(struct Instance *instance,SInfo *srcbuf,ULONG rm_start,ULONG rm_len,UBYTE win,double par,UBYTE hmax);
void DisplayResults(struct Instance *instance,SInfo *srcbuf,ULONG rm_start,ULONG rm_len);

//-- defines ------------------------------------------------------------------

#define HEIGHT_SCL	150

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

#define GADID_LINES		(GADID_SRC+SRC_IDCT)
#define GADIX_LINES		(GADIX_SRC+SRC_IXCT)

#define GADID_MAXLINES	(GADID_LINES+1)
#define GADIX_MAXLINES	(GADIX_LINES+1)

#define GADID_BANDS		(GADID_MAXLINES+1)
#define GADIX_BANDS		(GADIX_MAXLINES+1)

#define GADID_GAMMA		(GADID_BANDS+1)
#define GADIX_GAMMA		(GADIX_BANDS+1)

#define GADID_DIR		(GADID_GAMMA+1)
#define GADIX_DIR		(GADIX_GAMMA+1)

#define GADID_WFKT		(GADID_DIR+1)
#define GADIX_WFKT		(GADIX_DIR+1)

#define LAST_GADIX		(GADIX_WFKT+WFKT_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[3];
};

UBYTE *DirLabels[]={ "front","back",0l };
UBYTE *BandLabels[]={ "4","8","16","32","64","128","256","512",0l };

UWORD maxlines;

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
	UWORD winxr=14,winyr=32+RunTime->scy[18];
	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(4);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;
	UWORD maxlines2;
	UBYTE as_yd=max(2,(4.0*RunTime->yres)/RunTime->xres);

	Param param;

	UC_ConvertParameter(param,instance->p_ml,UGRP_COUNT,COUN_ABS);
	instance->ml=atoi(param);

	maxlines= (RunTime->xres-512-winxr)>>2;
	maxlines2=(RunTime->yres-HEIGHT_SCL-winyr)/as_yd;
	if(maxlines2<maxlines) maxlines=maxlines2;
	if(instance->ml>maxlines) {
		sprintf(param,"%ld",maxlines);
		UC_ConvertParameter(instance->p_ml,param,UGRP_COUNT,COUN_ABS);
	}

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
		ng.ng_GadgetText	="Lines";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_Flags			=PLACETEXT_LEFT;
		ng.ng_GadgetID		=GADID_LINES;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_ml,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_LINES]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="MaxLines";
		ng.ng_GadgetID		=GADID_MAXLINES;
		sprintf(param,"%d",maxlines);
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,param,GTTX_CopyText,TRUE,TAG_DONE);
		modGadgets[GADIX_MAXLINES]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Bands";
		ng.ng_GadgetID		=GADID_BANDS;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,BandLabels,GTCY_Active,instance->bands,TAG_DONE);
		modGadgets[GADIX_BANDS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="Gamma";
		ng.ng_GadgetID		=GADID_GAMMA;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_gamma,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_GAMMA]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_GadgetText	="Dir.";
		ng.ng_GadgetID		=GADID_DIR;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,DirLabels,GTCY_Active,instance->dir,TAG_DONE);
		modGadgets[GADIX_DIR]=g;

		g=AddWinFunction(&(instance->wfkt1),modGadgets,g,GADID_WFKT,GADIX_WFKT,xs1,ys2+RunTime->scy[FRMCONTENT_YOFFSET+36]);

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
								if(!HandleSource     (&(instance->src),  modWin,modGadgets,GADID_SRC,   GADIX_SRC,   gnr,icode)
								&& !HandleWinFunction(&(instance->wfkt1),modWin,modGadgets,GADID_WFKT,  GADIX_WFKT,  gnr,icode)
								&& !HandleCfgSel     (&cfgsel        ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_DIR:
											instance->dir=icode;
											break;
										case GADID_LINES:
											HandleParameter(instance->p_ml,modWin,modGadgets[GADIX_LINES]);
											break;
										case GADID_BANDS:
											instance->bands=icode;
											break;
										case GADID_GAMMA:
											HandleParameter(instance->p_gamma,modWin,modGadgets[GADIX_GAMMA]);
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_MAXLINES],modWin,0l);
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
	//-- procesing length
	ULONG rm_start,rm_len;
	//-- src & dst buffers
	SInfo *srcbuf;
	//-- administrative
	Param param;
	UBYTE okay=TRUE;

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:				// wir wurden e.g. per ARexx gestartet
		case CM_STREAM:				// no difference
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

	UC_ConvertParameter(param,instance->p_ml,UGRP_COUNT,COUN_ABS);
	instance->ml=atoi(param);

	UC_ConvertParameter(param,instance->p_gamma,UGRP_PROPOTION,PROP_FC);
	instance->gamma=atof(param);

	instance->m=1L<<(instance->bands+2);

	if(activate((SFXCoreInstance *)instance)) {
		activate_ch((SFXCoreInstance *)instance,0);
	
		if(AnalyseFFT(instance,srcbuf,rm_start,rm_len,instance->wfkt1.win,instance->wfkt1.par,HEIGHT_SCL)) {
			/** @todo if(pdata->callmd==CM_DIRECT) SaveImage(); */
			DisplayResults(instance,srcbuf,rm_start,rm_len);
		}
		deactivate_ch((SFXCoreInstance *)instance,0);
	}
	else okay=FALSE;
	deactivate((SFXCoreInstance *)instance);
	if(!okay) {
		return(FALSE);
	}
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;pf=ParseWinFunctionSet(&(instance->wfkt1),"W1",params);
		if(!pf && !stricmp(RexxPar2,"Lines"))		{ strntcpy(instance->p_ml   ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Gamma"))		{ strntcpy(instance->p_gamma,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Dir"))			{ instance->dir   =atoi(RexxPar3)%2;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ instance->bands =atoi(RexxPar3)%5;pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=ParseWinFunctionGet(&(instance->wfkt1),"W1",params,ret);
		if(!pf && !stricmp(RexxPar2,"Lines"))		{ strntcpy(ret,instance->p_ml   ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Gamma"))		{ strntcpy(ret,instance->p_gamma,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Dir"))			{ sprintf(ret,"%d",instance->dir);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ sprintf(ret,"%d",instance->bands);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_ml,		CfgIO_ReadString		(cfg,"General"	,"Maxlines"	,"300"));
	strcpy(instance->p_gamma,	CfgIO_ReadString		(cfg,"General"	,"Gamma"	,"0.75"));
	instance->bands=			CfgIO_MatchString(
					 		CfgIO_ReadString		(cfg,"General"	,"Bands"	,BandLabels[5]),BandLabels);
	instance->dir=			CfgIO_MatchString(
					 		CfgIO_ReadString		(cfg,"General"	,"Dir"		,DirLabels[0]),DirLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Maxlines"	,instance->p_ml);
	CfgIO_WriteString		  (cfg,"General"	,"Gamma"	,instance->p_gamma);
	CfgIO_WriteString		  (cfg,"General"	,"Bands"	,BandLabels[instance->bands]);
	CfgIO_WriteString		  (cfg,"General"	,"Dir"		,DirLabels[instance->dir]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->wfkt1.win=WIN_HAMMING;
	instance->wfkt1.par=0.54;
	instance->wfkt1.desc[0]='\0';

	UC_ConvertParameter(instance->p_ml   ,"50" ,UGRP_COUNT,COUN_ABS);
	UC_ConvertParameter(instance->p_gamma,"0.75",UGRP_PROPOTION,PROP_FC);
	instance->dir=0;									/* vorn */
	instance->bands=5;								/* => 128 */
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	Param param;

	SetSource     (&(instance->src),  modWin,modGadgets,GADIX_SRC);
	SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);
	GT_SetGadgetAttrs(modGadgets[GADIX_DIR     ],modWin,0l,GTCY_Active,instance->dir,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_LINES   ],modWin,0l,GTST_String,instance->p_ml,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_BANDS   ],modWin,0l,GTCY_Active,instance->bands,TAG_DONE);
	sprintf(param,"%d",maxlines);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAXLINES],modWin,0l,GTTX_Text  ,param,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_GAMMA   ],modWin,0l,GTST_String,instance->p_gamma,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

UBYTE AnalyseFFT(struct Instance *instance,SInfo *srcbuf,ULONG rm_start,ULONG rm_len,UBYTE win,double par,UBYTE hmax) {
	//-- loop
	register ULONG j;
	register double t;				/* Laufvariable Zeit */
	register UWORD o;				/* Laufvariable Bänder */
	//-- prowin
	PWData *pwd=NULL;
	ProWinTitle title;
	//-- buffer access
	WORD *sbuf;
	SmpAccess *srcsa;
	SAFuncPtr SARead;
	ULONG blen;
	//-- local
	ULONG size;
	UWORD m=instance->m,mm=m+m;
	WORD *rbufw,*rbufr;
	SAMPLE *rbuf=NULL,*rbufbase;
	register float *rasbuf;
	ULONG rbufmask,rbufsize;
	double *flbuf_r=0l,*flbuf_i=0l;
	double *win_fkt=0l;
	double rmax=0.0;				/* Variablen für Scalierung */
	register double *flp1_r,*flp1_i,*winfp;
	UWORD *bitrevtab=0l;
	double *sintab1=0l,*costab=0l;
	UBYTE notquit=TRUE;
	double st;
	UBYTE ch=0;

	st=(double)rm_len/(double)instance->ml;
	size=(mm+2)*sizeof(double);
	if(	(flbuf_r=(double *)AllocVec(size,MEMF_ANY))
	&&	(flbuf_i=(double *)AllocVec(size,MEMF_ANY))
	&&	(win_fkt=NewWindowFkt((mm+2),win,&par))
	&&			 NewFFTTabs(&bitrevtab,&sintab1,NULL,&costab,m)
	&&	(rbuf	=NewRingBuf(mm,&rbufbase,&rbufmask,&rbufsize))
	&&	(pwd	=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
		for(o=0;o<CH_COUNT(srcbuf->channels);o++) {					// select first active channel
			if(srcbuf->channelmask&(1<<o)) { ch=o;o=4; }
		}
		if((srcsa=SAInit(srcbuf,ch,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {	// sequentiell Blöcke konstanter Größe lesen
			SARead =srcsa->safunc;
			t=0.0;rasbuf=instance->raster;
			memset(rbuf,0,(rbufsize<<1));
			rbufw=rbufbase;
			blen=Bytes2Samples(srcsa->blen);
			sprintf(title,GetString(msgProgress),PRJ_NAME,1,3);
			StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
			for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++) {
				sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
				for(j=0;((j<blen) && (instance->curlen<instance->slen));j++,instance->curlen++) {	// die Blocklänge durchgehen
					*rbufw=*(sbuf++);										// get current value
					rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

					if(instance->curlen==(ULONG)t) {									// process a timeslice
						rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-mm))&rbufmask));
						flp1_r=flbuf_r;flp1_i=flbuf_i;winfp=win_fkt;
						for(o=0;o<=mm;o++) {
							*(flp1_r++)=(*(winfp++))*((double)*rbufr/32768.0);
							*(flp1_i++)=0.0;
							rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
						}

						Transform(flbuf_r,flbuf_i,bitrevtab,sintab1,costab,-1,m);	/* in Frequenzdarstellung transformieren */

						flp1_r=&flbuf_r[1];flp1_i=&flbuf_i[1];						// 0 is DC-Offset
						for(o=0;o<m;o++) {
							//*(rasbuf++)=sqrt((*flp1_r)*(*(flp1_r++))+(*flp1_i)*(*(flp1_i++)));	// Bug in SAS ??
							*(rasbuf++)=sqrt((*flp1_r)*(*flp1_r)+(*flp1_i)*(*flp1_i));		// really for(o=1;o<=m;o++)
							flp1_r++;flp1_i++;
						}
						t+=st;
					}
				}
			}
			if(ProWinAborted(instance->curlen)) notquit=FALSE;					// es wurde abgebrochen
			SADone(srcsa,srcbuf);
		}

		if(notquit) {
			rm_len=m*instance->ml;
			rasbuf=instance->raster;										// Maxima
			sprintf(title,GetString(msgProgress),PRJ_NAME,2,3);
			StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
			for(;instance->curlen<rm_len;instance->curlen++,rasbuf++) if(*rasbuf>rmax) rmax=*rasbuf;
			if(ProWinAborted(instance->curlen)) notquit=FALSE;					// es wurde abgebrochen
		}
		if(notquit) {
			rasbuf=instance->raster;										// Skalieren
			sprintf(title,GetString(msgProgress),PRJ_NAME,3,3);
			StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					// den Statusbalken starten
			for(;instance->curlen<rm_len;instance->curlen++,rasbuf++) *rasbuf=(double)hmax*mypow(((double)*rasbuf/(double)rmax),instance->gamma);
			if(ProWinAborted(instance->curlen)) notquit=FALSE;					// es wurde abgebrochen
		}
	}
	else { Message(errAllocateMemory,NULL,__FILE__,__LINE__);notquit=FALSE; }
	size=(mm+2)*sizeof(double);
	if(pwd)			DelProWin(pwd);
	if(flbuf_r)		FreeVec((ULONG *)flbuf_r);
	if(flbuf_i)		FreeVec((ULONG *)flbuf_i);
					DelFFTTabs(bitrevtab,sintab1,NULL,costab);
	if(win_fkt)		DelWindowFkt(win_fkt);
	if(rbuf)		DelRingBuf(rbuf);
	return(notquit);
}

void DisplayResults(struct Instance *instance,SInfo *srcbuf,ULONG rm_start,ULONG rm_len) {
	char title[100];
	UWORD winx,winy,winxr=14,winyr=32+RunTime->scy[18];
	struct TagItem as_wintags[]={
		WA_Left,			0,
		WA_Top,				8,
		WA_Width,			700,
		WA_Height,			512,
		WA_IDCMP,			IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_INTUITICKS,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_CLOSEGADGET|WFLG_DEPTHGADGET|WFLG_GIMMEZEROZERO,
		WA_Title,			0l,
		WA_ScreenTitle,		(ULONG)PRJ_VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_MinWidth,		700,
		WA_MinHeight,		512,
		WA_MaxWidth,		700,
		WA_MaxHeight,		512,
		TAG_DONE
	};
	struct Window		*as_win;
	struct RastPort		*as_rp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	//USHORT icode;
	UBYTE runing=TRUE;
	float *rbuf1,*rbuf2,tfl;		/* Puffer für Spectrogammergebisse */
	ULONG rptr;						/* Zeiger auf akt. Wert */
	register UWORD t;				/* Laufvariable Zeit */
	register UWORD n;				/* Laufvariable Bänder */
	register ULONG i,o;				/* andere Laufvariablen */
	UWORD m=instance->m;
	Param param;

	UWORD maxy[1280];				/* Variablen für Darstellung */
	UWORD as_xs,as_ys,as_xp,as_yp,as_xe,as_ye,as_yo;
	UBYTE as_xd,as_yd=max(2,(4.0*RunTime->yres)/RunTime->xres);
	double step,st2,frq;
	UWORD sl;

	as_xd=512/m;
	if(instance->dir) {				/* eventuell Werte vertauschen */
		rbuf1=instance->raster;
		for(t=0;t<(instance->ml>>1);t++) {
			rbuf2=&instance->raster[(instance->ml-t-1)*m];
			for(n=0;n<m;n++) { tfl=*rbuf1;*(rbuf1++)=*rbuf2;*(rbuf2++)=tfl; }
		}
	}
	if(instance->ml<8) winx=winxr+512+28;			// Windowwidth
	else winx=winxr+512+(instance->ml<<2);
	as_xs=5;
	winy=winyr+instance->ml*as_yd+HEIGHT_SCL;			// Windowheight
	as_ys=5+instance->ml*as_yd+HEIGHT_SCL;
	for(i=0;i<1280;i++) maxy[i]=9999;		/* Initialisieren */
	as_wintags[1].ti_Data=RunTime->scy[6]+4;
	as_wintags[11].ti_Data=as_wintags[ 9].ti_Data=as_wintags[2].ti_Data=winx;
	as_wintags[12].ti_Data=as_wintags[10].ti_Data=as_wintags[3].ti_Data=winy;
	sprintf(title,PRJ_NAME" : %s",((struct Node *)srcbuf)->ln_Name);
	as_wintags[ 6].ti_Data=(ULONG)title;
	if(RunTime->sfxpubscr==IntuitionBase->FirstScreen) as_wintags[5].ti_Data|=WFLG_ACTIVATE;
	if(as_win=OpenWindowTagList(0l,as_wintags)) {
		as_rp=as_win->RPort;
		SetFont(as_rp,RunTime->scrfont);
		DrawGadWB(as_rp,2,2,winx-12,winy-(RunTime->scy[6]+12));
		SetAPen(as_rp,1);RectFill(as_rp,4,4,winx-13,winy-(RunTime->scy[6]+13));
		SetAPen(as_rp,2);SetBPen(as_rp,1);SetDrMd(as_rp,JAM1);

		Move(as_rp,as_xs,as_ys);					/* Beschriftung der Frequenzachse */
		Draw(as_rp,as_xs+(513-as_xd),as_ys);
		Draw(as_rp,as_xs+(513-as_xd)+((instance->ml-1)<<2),as_ys-((instance->ml-1)*as_yd));			/* ml-1*4,ml-1*3 */
		frq=(double)(srcbuf->srat>>4);
		for(i=0;i<=8;i++) {
			Move(as_rp,as_xs+(i<<6),as_ys);
			Draw(as_rp,as_xs+(i<<6),as_ys+8);
			if(i) {
				sprintf(param,"%ld",(ULONG)(frq*(double)i));sl=strlen(param);
				Move(as_rp,as_xs+(i<<6)-(sl*3),as_ys+10+RunTime->scy[6]);
				Text(as_rp,param,sl);
			}
			else {
				Move(as_rp,as_xs+(i<<6),as_ys+10+RunTime->scy[6]);
				Text(as_rp,"0",1);
			}
		}

		Move(as_rp,as_xs,as_ys+12+RunTime->scy[12]);Text(as_rp,"Frequency",9);
		if(instance->ml>7) Move(as_rp,as_xs+509+(24),as_ys-(as_yd));Text(as_rp,"Time",4);

		as_yp=as_ys;as_xp=as_xs;rptr=0;					/* the first timeslice */

		as_ye=as_yp-(int)instance->raster[rptr];					/* the first band */
		Move(as_rp,as_xp-1,as_yp);Draw(as_rp,as_xp-1,as_ye);
		//WritePixel(as_rp,as_xp,as_ye);
		maxy[as_xp]=as_ye;as_yo=as_ye;

		rptr++;
		for(n=1;n<m;n++) {								/* the other bands */
			st2=step=(instance->raster[rptr]-instance->raster[rptr-1])/(double)as_xd;
			as_xe=as_xp;
			for(o=0;o<as_xd;o++) {
				as_ye=as_yp-(int)(instance->raster[rptr-1]+st2);
				if(as_ye<=maxy[as_xe]) {
					Move(as_rp,as_xe-1,as_yo);Draw(as_rp,as_xe,as_ye);
					maxy[as_xe]=as_ye;as_yo=as_ye;
				}
				else {
					Move(as_rp,as_xe-1,as_yo);Draw(as_rp,as_xe,maxy[as_xe]);
					as_yo=maxy[as_xe];
				}
				as_xe++;
				st2+=step;
			}
			as_xp+=as_xd;rptr++;
		}
		as_yp-=as_yd;as_xs+=4;
		for(t=1;t<instance->ml;t++) {								/* the other timeslices */
			as_xp=as_xs;
			as_ye=as_yp-(int)instance->raster[rptr];				/* the first band */
			if(as_ye<=maxy[as_xp]) {
				Move(as_rp,as_xp-1,as_yp);Draw(as_rp,as_xp-1,as_ye);
				//WritePixel(as_rp,as_xp,as_ye);
				maxy[as_xp]=as_ye;as_yo=as_ye;
			}
			else as_yo=maxy[as_xp];
			rptr++;

			for(n=1;n<m;n++) {							/* the other bands */
				st2=step=(instance->raster[rptr]-instance->raster[rptr-1])/(double)as_xd;
				as_xe=as_xp;
				for(o=0;o<as_xd;o++) {
					as_ye=as_yp-(int)(instance->raster[rptr-1]+st2);
					if(as_ye<=maxy[as_xe]) {
						Move(as_rp,as_xe-1,as_yo);Draw(as_rp,as_xe,as_ye);
						maxy[as_xe]=as_ye;as_yo=as_ye;
					}
					else {
						Move(as_rp,as_xe-1,as_yo);Draw(as_rp,as_xe,maxy[as_xe]);
						as_yo=maxy[as_xe];
					}
					as_xe++;
					st2+=step;
				}
				as_xp+=as_xd;rptr++;
			}
			as_yp-=as_yd;as_xs+=4;
		}
		SetDrMd(as_rp,COMPLEMENT);SetAPen(as_rp,3);
		while(runing) {
			WaitPort(as_win->UserPort);
			while(imsg=(struct IntuiMessage *)GetMsg(as_win->UserPort)) {
				iclass	=imsg->Class;
				//icode	=imsg->Code;
				ReplyMsg(imsg);
				switch(iclass) {
					case IDCMP_CLOSEWINDOW:
						runing=FALSE;break;
				}
			}
		}
		CloseWindow(as_win);
	}
	else Message(errOpenWindow,NULL,__FILE__,__LINE__);
}
//-- eof ----------------------------------------------------------------------
