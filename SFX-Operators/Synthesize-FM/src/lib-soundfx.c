/****h* Synthesize-FM/lib-soundfx.c [4.3] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.3 (04.12.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Synthesize-FM Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    08.Nov.1998
*  MODIFICATION HISTORY
*    04.Dec.2003	V 4.3	most recent version
*    31.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    24.Jan.1999	V 1.00	most recent version
*    08.Nov.1998	V 1.00	initial version
*  NOTES
*
*******
*/

#define SYNTHESIZE_FM_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasSource

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Synthesize-FM.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

void ReadFMSynth(struct Instance *instance,char *fname);

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

#define GADID_SLEN		(GADID_CFGSEL+CFGSEL_IDCT)
#define GADIX_SLEN		(GADIX_CFGSEL+CFGSEL_IXCT)

#define GADID_RATE		(GADID_SLEN+1)
#define GADIX_RATE		(GADIX_SLEN+1)

#define GADID_RATESEL	(GADID_RATE+1)

#define GADID_VOL		(GADID_RATESEL+1)
#define GADIX_VOL		(GADIX_RATE+1)

#define GADID_FRQ		(GADID_VOL+1)
#define GADIX_FRQ		(GADIX_VOL+1)

#define GADID_FRQSEL	(GADID_FRQ+1)

#define GADID_OPSEL		(GADID_FRQSEL+1)
#define GADIX_OPSEL		(GADIX_FRQ+1)

#define GADID_MOD1		(GADID_OPSEL+1)
#define GADIX_MOD1		(GADIX_OPSEL+1)

#define GADID_MOD2		(GADID_MOD1+MOD_IDCT)
#define GADIX_MOD2		(GADIX_MOD1+MOD_IXCT)

#define GADID_WAVE		(GADID_MOD2+MOD_IDCT)
#define GADIX_WAVE		(GADIX_MOD2+MOD_IXCT)

#define GADID_PHASE		(GADID_WAVE+1)
#define GADIX_PHASE		(GADIX_WAVE+1)

#define GADID_MATRIX	(GADID_PHASE+1)
#define GADIX_MATRIX	(GADIX_PHASE+1)

#define LAST_GADIX		(GADIX_MATRIX+(ANZ_OSZIS*ANZ_OSZIS)+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[14];
};

UBYTE *OsziLabels[]={ "Sin","Tri","Saw","Sqr",0l };
UBYTE *OpLabels[]={"1","2","3","4","5","6",0l };

UBYTE aktop;				// wegen RefreshGUI

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
		SFXMod_ModDescInit( 0,PD_COREINSTANCE,0l   , instance);
		SFXMod_ModDescInit( 1,PD_MODULATOR   ,"M11",&(instance->mod1[0]));
		SFXMod_ModDescInit( 2,PD_MODULATOR   ,"M12",&(instance->mod2[0]));
		SFXMod_ModDescInit( 3,PD_MODULATOR   ,"M21",&(instance->mod1[1]));
		SFXMod_ModDescInit( 4,PD_MODULATOR   ,"M22",&(instance->mod2[1]));
		SFXMod_ModDescInit( 5,PD_MODULATOR   ,"M31",&(instance->mod1[2]));
		SFXMod_ModDescInit( 6,PD_MODULATOR   ,"M32",&(instance->mod2[2]));
		SFXMod_ModDescInit( 7,PD_MODULATOR   ,"M41",&(instance->mod1[3]));
		SFXMod_ModDescInit( 8,PD_MODULATOR   ,"M42",&(instance->mod2[3]));
		SFXMod_ModDescInit( 9,PD_MODULATOR   ,"M51",&(instance->mod1[4]));
		SFXMod_ModDescInit(10,PD_MODULATOR   ,"M52",&(instance->mod2[4]));
		SFXMod_ModDescInit(11,PD_MODULATOR   ,"M61",&(instance->mod1[5]));
		SFXMod_ModDescInit(12,PD_MODULATOR   ,"M62",&(instance->mod2[5]));
		SFXMod_ModDescInit(13,PD_END         ,0l  ,0l);	
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
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(1);				// select operator, global options
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(1);				// wave, phase
	UWORD ys4=ys3+1+MODSEL_HEIGHT;					// amplitude, modulation matrix
	UWORD ys5=ys4+1+MODSEL_HEIGHT;					// freqency,  modulation matrix
	UWORD winh=ys5+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD xs3=xs2+1+STDFRM_WIDTH;
	UWORD winw=xs3+3+CFGSEL_WIDTH;
	UWORD xh,yh1,yh2;

	register UBYTE i,j,k,o;
	LONG rate;
	ULONG slen,srat;
	double frq;
	Param param;
	Source			dummysrc;
	SInfo			dummysi={0};

	aktop=0;

	dummysrc.srcbuf=&dummysi;
	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	dummysi.srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) dummysi.slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);dummysi.slen=atof(param)*((double)dummysi.srat/1000.0); }
	if(dummysi.slen<2) dummysi.slen=2;

	SFXMod_OpenGUILibs;
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[258];
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
		g=AddCfgSel(&cfgsel,modGadgets,g,GADID_CFGSEL,GADIX_CFGSEL,xs3,ys1,winh-2);

		g=AddModulator(&(instance->mod1[aktop]),modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys3);
		g=AddModulator(&(instance->mod2[aktop]),modGadgets,g,GADID_MOD2,GADIX_MOD2,xs1,ys4);

		ng.ng_LeftEdge		=xs1+RunTime->scx[42];				/* Länge */
		ng.ng_TopEdge		=ys1+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[65];
		ng.ng_GadgetText	="SLen";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_SLEN;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_slen,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_SLEN]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[155];				/* Rate */
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_RATESEL;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[171];
		ng.ng_Width			=RunTime->scx[65];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_RATE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_srat,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_RATE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[270];				/* Volume */
		ng.ng_Width			=RunTime->scx[75];
		ng.ng_GadgetText	="Vol";
		ng.ng_GadgetID		=GADID_VOL;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_ampf,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_VOL]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[392];					/* Pitch */
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_FRQSEL;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[408];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_FRQ;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_pitch,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FRQ]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[565];					/* Operatorselect */
		ng.ng_Width			=RunTime->scx[45];
		ng.ng_GadgetText	="Operator";
		ng.ng_GadgetID		=GADID_OPSEL;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,OpLabels,GTCY_Active,aktop,TAG_DONE);
		modGadgets[GADIX_OPSEL]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[42];					/* Wave <n> */					
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[65];
		ng.ng_GadgetText	="Wave";
		ng.ng_GadgetID		=GADID_WAVE;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,OsziLabels,GTCY_Active,instance->oszi[aktop],TAG_DONE);
		modGadgets[GADIX_WAVE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[155];					/* Phase <n> */
		ng.ng_GadgetText	="Pha.";
		ng.ng_GadgetID		=GADID_PHASE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_phase[aktop],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_PHASE]=g;

		//ng.ng_LeftEdge		=xs1+RunTime->scx[250];					/* Amount <n> */
		//ng.ng_Width			=RunTime->scx[45];
		//ng.ng_GadgetText	="Amt";
		//ng.ng_GadgetID		=GADID_AMOUNT;
		//g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->amount[aktop],TAG_DONE);
		//((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		//modGadgets[GADIX_AMOUNT]=g;

		xh=RunTime->scx[26];yh1=RunTime->scy[11];						/* ModMatrix */
		ng.ng_Width			=RunTime->scx[26];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_Flags			=PLACETEXT_LEFT;
		for(k=j=0;j<ANZ_OSZIS;j++) {
			ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET]+j*yh1;
			ng.ng_GadgetText	=OpLabels[j];
			for(i=0;i<ANZ_OSZIS;i++,k++) {
				ng.ng_LeftEdge		=xs2+RunTime->scx[50]+i*xh;
				ng.ng_GadgetID		=GADID_MATRIX+k;
				g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,instance->matrix[j][i],GTCB_Scaled,TRUE,TAG_DONE);
				modGadgets[GADIX_MATRIX+k]=g;
				ng.ng_GadgetText	="";
			}
		}

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Rate",xs1+RunTime->scx[145],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Frq",xs1+RunTime->scx[384],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[ 29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[ 29],ys4+RunTime->scy[FRMTEXT_YPOS(2)]);
			for(j=0;j<ANZ_OSZIS;j++) { Move(modrp,xs2+RunTime->scx[60]+j*xh,ys2+RunTime->scy[FRMCONTENT_YOFFSET+6]+(6*yh1));Text(modrp,OpLabels[j],1); }
			RText(modrp,"Src",xs2+RunTime->scx[29],ys2+RunTime->scy[FRMCONTENT_YOFFSET]+(3*yh1));
			Move(modrp,xs2+RunTime->scx[50-9]+(3*xh),ys2+RunTime->scy[FRMCONTENT_YOFFSET+12]+(6*yh1));Text(modrp,"Dst",3);

			xh=STDFRM_WIDTH;
			DrawTitle(modrp,"Wave"				,xs1,ys2,xh);
			DrawTitle(modrp,"Frequency"			,xs1,ys3,xh);
			DrawTitle(modrp,"Amplitude"			,xs1,ys4,xh);
			DrawTitle(modrp,"Modulation-Matrix"	,xs2,ys2,xh);
			xh=xh+xh+1;
			DrawTitle(modrp,"Miscellaneous"		,xs1,ys1,xh);
			DrawTitle(modrp,"Presets"			,xs3,ys1,CFGSEL_WIDTH);

			yh1=ANYFRM_HEIGHT(1);
			yh2=MODSEL_HEIGHT;
			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys2,xh,yh1);							/* Wave,Pha */
			DrawGadWB(modrp,xs1,ys3,xh,yh2);							/* Frq */
			DrawGadWB(modrp,xs1,ys4,xh,yh2);							/* Amp */
			DrawGadWB(modrp,xs2,ys2,xh,yh2+yh2+yh1+2);					/* ModMat */
			xh+=1+xh;
			DrawGadWB(modrp,xs1,ys1,xh,yh1);							/* Par */
			DrawGadWB(modrp,xs1,ys5,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs3,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetModulator(&(instance->mod1[aktop]),modWin,modGadgets,GADIX_MOD1);
			SetModulator(&(instance->mod2[aktop]),modWin,modGadgets,GADIX_MOD2);

			while(runing) {
				WaitPort(modWin->UserPort);
				while(imsg=GT_GetIMsg(modWin->UserPort)) {
					SFXMod_Interface_AnalyseIMsg
					switch(iclass) {
						case IDCMP_GADGETUP:
							gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
							if(!HandleModulator(&(instance->mod1[aktop]),&dummysrc,0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode)
							&& !HandleModulator(&(instance->mod2[aktop]),&dummysrc,0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode)
							&& !HandleCfgSel   (&cfgsel       ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
								switch(gnr) {
									SFXMod_Interface_GadgetUpEvents
									case GADID_RATESEL:
										UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										rate=atof(param);
										if(rate=GetPeriode(rate,NULL)) {
											sprintf(param,"%ld hz",rate);
											UC_ConvertParameter(instance->p_srat,param,UGRP_ABSFREQUENCY,UC_GetParameterUnit(instance->p_srat,UGRP_ABSFREQUENCY));
											GT_SetGadgetAttrs(modGadgets[GADIX_RATE],modWin,0l,GTST_String,instance->p_srat,TAG_DONE);
											UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
											srat=atof(param);
											if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(instance->p_slen);
											else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);slen=atof(param)*((double)srat/1000.0); }
											if(slen<2) slen=2;
											for(o=0;o<ANZ_OSZIS;o++) {		// init all oszis
												instance->mod1[o].slen=slen;
												instance->mod1[o].srat=srat;
												instance->mod2[o].slen=slen;
												instance->mod2[o].srat=srat;
											}
										}
										break;
									case GADID_RATE:
										HandleParameter(instance->p_srat,modWin,modGadgets[GADIX_RATE]);
										UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										srat=atof(param);
										if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(instance->p_slen);
										else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);slen=atof(param)*((double)srat/1000.0); }
										if(slen<2) slen=2;
										for(o=0;o<ANZ_OSZIS;o++) {		// init all oszis
											instance->mod1[o].slen=slen;
											instance->mod1[o].srat=srat;
											instance->mod2[o].slen=slen;
											instance->mod2[o].srat=srat;
										}
										break;
									case GADID_SLEN:
										HandleParameter(instance->p_slen,modWin,modGadgets[GADIX_SLEN]);
										UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										srat=atof(param);
										if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(instance->p_slen);
										else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);slen=atof(param)*((double)srat/1000.0); }
										if(slen<2) slen=2;
										for(o=0;o<ANZ_OSZIS;o++) {		// init all oszis
											instance->mod1[o].slen=slen;
											instance->mod1[o].srat=srat;
											instance->mod2[o].slen=slen;
											instance->mod2[o].srat=srat;
										}
										break;
									case GADID_FRQSEL:
										UC_ConvertParameter(param,instance->p_pitch,UGRP_ABSFREQUENCY,AFRQ_HZ);
										frq=atof(param);
										Frq2RateKey(&frq,(ULONG *)(&rate),0l);
										if(rate=GetPeriode(rate,NULL)) {
											if(Rate2KeyFrq(rate,0l,&frq)) {
												sprintf(param,"%lf hz",frq);
												UC_ConvertParameter(instance->p_pitch,param,UGRP_ABSFREQUENCY,UC_GetParameterUnit(instance->p_pitch,UGRP_ABSFREQUENCY));
												GT_SetGadgetAttrs(modGadgets[GADIX_FRQ],modWin,0l,GTST_String,instance->p_pitch,TAG_DONE);
											}
										}
										break;
									case GADID_FRQ:
										HandleParameter(instance->p_pitch,modWin,modGadgets[GADIX_FRQ]);
										break;
									case GADID_VOL:
										HandleParameter(instance->p_ampf,modWin,modGadgets[GADIX_VOL]);
										break;

									case GADID_OPSEL:
										aktop=icode;
										SetModulator(&(instance->mod1[aktop]),modWin,modGadgets,GADIX_MOD1);
										SetModulator(&(instance->mod2[aktop]),modWin,modGadgets,GADIX_MOD2);
										GT_SetGadgetAttrs(modGadgets[GADIX_WAVE  ],modWin,0l,GTCY_Active,instance->oszi  [aktop],TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_PHASE ],modWin,0l,GTST_String,instance->phase [aktop],TAG_DONE);
										//GT_SetGadgetAttrs(modGadgets[GADIX_AMOUNT],modWin,0l,GTST_String,instance->amount[aktop],TAG_DONE);
										break;
									case GADID_WAVE:
										instance->oszi[aktop]=icode;
										break;
									case GADID_PHASE:
										HandleParameter(instance->p_phase[aktop],modWin,modGadgets[GADIX_PHASE]);
										break;
									//case GADID_AMOUNT:
										//HandleParameter(instance->amount[aktop],modWin,modGadgets[GADIX_AMOUNT]);
										//break;
									default:
										if((gnr>=GADID_MATRIX) && (gnr<(GADID_MATRIX+(ANZ_OSZIS*ANZ_OSZIS)))) {
											k=gnr-GADID_MATRIX;
											j=k/ANZ_OSZIS;
											i=k-(j*ANZ_OSZIS);
											// häääääääää ???
											//instance->matrix[j][i]=(UBYTE)(modGadgets[GADIX_MATRIX+k]->Flags&GFLG_SELECTED==GFLG_SELECTED);
											//MSG3("0x%08lx 0x%08lx ->%d",GFLG_SELECTED,modGadgets[GADIX_MATRIX+k]->Flags,(modGadgets[GADIX_MATRIX+k]->Flags&GFLG_SELECTED==GFLG_SELECTED));
											// wieso das nicht funzt - keine Ahnung
											instance->matrix[j][i]=((modGadgets[GADIX_MATRIX+k]->Flags&GFLG_SELECTED) ? 1 : 0);
											//MSG4("%d -> matrix[%d][%d]=%d",k,j,i,instance->matrix[j][i]);
										}
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
			SFXMod_Interface_WinDone
			SFXMod_Interface_FReqDone
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		FreeGadgets(modGList);
		DoneCfgSel(&cfgsel);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	OUTRO_BOOL(ret);
	return(ret);
	Error:
	OUTRO_BOOL(FALSE);
	return(FALSE);
}

UBYTE SAVEDS ASM LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	//-- loop
	UBYTE o;
	//-- prowin
	PWData *pwd;
	ProWinTitle title;
	//-- procesing length
	//-- src & dst buffers
	SInfo *dstbuf=NULL;
	//-- buffer access
	SmpAccess *dstsa;
	SAFuncPtr SAWrite;
	ULONG blen;
	//-- administrative
	Param param;
	UBYTE newdst=FALSE;
	UBYTE okay=TRUE;

	INTRO;

	for(o=0;o<ANZ_OSZIS;o++) {
		UC_ConvertParameter(param,instance->mod1[o].vals,UGRP_RELFREQUENCY,RFRQ_FC);
		instance->frq1s[o]=atof(param);
		UC_ConvertParameter(param,instance->mod1[o].vale,UGRP_RELFREQUENCY,RFRQ_FC);
		instance->frq1e[o]=atof(param);
		instance->frq1d[o]=instance->frq1e[o]-instance->frq1s[o];

		//UC_ConvertParameter(param,instance->amount[o],UGRP_RELFREQUENCY,RFRQ_FC);
		//instance->frq2s[o]=atof(param);
		//MSG1("  Freqency Variation : %lf",instance->frq2s[o]);

		UC_ConvertParameter(param,instance->mod2[o].vals,UGRP_AMPLITUDE,AMP_FC);
		instance->amps[o]=atof(param);
		UC_ConvertParameter(param,instance->mod2[o].vale,UGRP_AMPLITUDE,AMP_FC);
		instance->ampe[o]=atof(param);
		instance->ampd[o]=instance->ampe[o]-instance->amps[o];

		//-- modulations matrix
		UC_ConvertParameter(param,instance->p_phase[o],UGRP_PHASE,PHA_RAD);
		instance->phase[o]=atof(param);
	}

	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
	if(instance->slen<2) instance->slen=2;
	UC_ConvertParameter(param,instance->p_pitch,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->pitch=atof(param);

	UC_ConvertParameter(param,instance->p_ampf,UGRP_AMPLITUDE,AMP_FC);
	instance->ampf=atof(param)*32767.0;

	if(!dstbuf) {
		if(dstbuf=NewSample()) {
			if(AllocSampleMem(dstbuf,instance->slen,(CH_MONO+1))) {
				dstbuf->srat=instance->srat;
				dstbuf->slen=instance->slen;
				dstbuf->channels=CH_MONO;
				dstbuf->channelmask=CH_CENTER;
				dstbuf->aktch=dstbuf->channels+1;
				dstbuf->zoomxl=instance->slen;
				newdst=TRUE;
			}
			else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
	}
	if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
		if(activate((SFXCoreInstance *)instance)) {
			//for(l=0;l<1;l++) {
				activate_ch((SFXCoreInstance *)instance,0);
				if((dstsa=SAInit(dstbuf,0,0,instance->slen,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {	// sequentiell Blöcke konstanter Größe schreiben
					SAWrite=dstsa->safunc;
					blen=Bytes2Samples(dstsa->blen);
					sprintf(title,GetString(msgProgress),PRJ_NAME,1,1);
					StartProWin(pwd,(ULONG *)(&instance->curlen),title,instance->slen);					// den Statusbalken starten
					for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && (instance->curlen<instance->slen));dstsa->seg++) {
						instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
						process((SFXCoreInstance *)instance,blen);
						SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
					}
					SADone(dstsa,dstbuf);
				}
				else okay=FALSE;
				deactivate_ch((SFXCoreInstance *)instance,0);
				if(ProWinAborted(instance->curlen)) { okay=FALSE; }										  // es wurde abgebrochen
			//}
			if(okay && newdst) AddSample(dstbuf,"SynthFM");
		}
		else okay=FALSE;
		deactivate((SFXCoreInstance *)instance);
		DelProWin(pwd);
	}
	else okay=FALSE;
	if(!okay) {
		if(newdst) RemSample(dstbuf);
		OUTRO_BOOL(FALSE);
		return(FALSE);
	}
	OUTRO_BOOL(TRUE);
	return(TRUE);
}

void SAVEDS ASM LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf,ix;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorSet  (&(instance->mod1[0]),"P11",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod2[0]),"P12",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod1[1]),"P22",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod2[1]),"P22",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod1[2]),"P32",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod2[2]),"P32",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod1[3]),"P42",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod2[3]),"P42",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod1[4]),"P52",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod2[4]),"P52",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod1[5]),"P62",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod2[5]),"P62",params);
		if(!pf && !stricmp(RexxPar2,"Oszilator"))	{ instance->oszi[atoi(RexxPar3)]	=CfgIO_MatchString(RexxPar4,OsziLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Phase"))		{ ix=atoi(RexxPar3);strntcpy(instance->p_phase[ix],RexxPar4,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(instance->p_slen ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Rate"))		{ strntcpy(instance->p_srat ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Pitch"))		{ strntcpy(instance->p_pitch,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strntcpy(instance->p_ampf ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet  (&(instance->mod1[0]),"P11",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod2[0]),"P12",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod1[1]),"P21",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod2[1]),"P22",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod1[2]),"P31",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod2[2]),"P32",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod1[3]),"P41",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod2[3]),"P42",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod1[4]),"P51",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod2[4]),"P52",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod1[5]),"P61",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod2[5]),"P62",params,ret);
		if(!pf && !stricmp(RexxPar2,"Oszilator"))	{ strcpy(ret,OsziLabels[instance->oszi[atoi(RexxPar3)]]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Phase"))		{ ix=atoi(RexxPar3);strntcpy(ret,instance->p_phase[ix],PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(ret,instance->p_slen ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Rate"))		{ strntcpy(ret,instance->p_srat ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Pitch"))		{ strntcpy(ret,instance->p_pitch,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strntcpy(ret,instance->p_ampf ,PARBUFLEN); }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	register UBYTE o,p;
	char strSectName[10],strItemName[10],ext[FESIZE];
	Param param;

	strsfn(fname,NULL,NULL,NULL,ext);
	if(!strcmp(ext,"cfg")) {
		CfgIO_ReadConfig(cfg,fname);

		LoadModSettings(cfg,instance->moddesc);
		strcpy(instance->p_ampf,	CfgIO_ReadString	(cfg,"General"	,"Ampf"			,instance->p_ampf));
		strcpy(instance->p_pitch,	CfgIO_ReadString	(cfg,"General"	,"Pitch"		,instance->p_pitch));
		strcpy(instance->p_slen,	CfgIO_ReadString	(cfg,"General"	,"Length"		,instance->p_slen));
		strcpy(instance->p_srat,	CfgIO_ReadString	(cfg,"General"	,"Rate"			,instance->p_srat));

		UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
		instance->srat=atof(param);
		if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
		else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
		if(instance->slen<2) instance->slen=2;

		for(o=0;o<ANZ_OSZIS;o++) {
			for(p=0;p<ANZ_OSZIS;p++) {
				sprintf(strItemName,"Matrix%d%d",o,p);
				instance->matrix[o][p]=(UBYTE)CfgIO_ReadBool		(cfg,"Matrix",	strItemName		,(BOOL)instance->matrix[o][p]);
			}
			sprintf(strSectName,"Oszi%d",o);
			instance->oszi[o]=				CfgIO_MatchString(
											CfgIO_ReadString	(cfg,strSectName,"Oszilator"	,OsziLabels[instance->oszi[o]]),OsziLabels);
			strcpy(instance->p_phase[o],	CfgIO_ReadString	(cfg,strSectName,"Phase"		,instance->p_phase[o]));
			//strcpy(instance->amount[o],	CfgIO_ReadString	(cfg,strSectName,"Amount"		,instance->amount[o]));
		}
	}
	else if(!strcmp(ext,"voice")) ReadFMSynth(instance,fname);
}

void SaveSettings(struct Instance *instance,char *fname) {
	register UBYTE o,p;
	char strSectName[10],strItemName[10];

	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Ampf"		,instance->p_ampf);
	CfgIO_WriteString		  (cfg,"General"	,"Pitch"	,instance->p_pitch);
	CfgIO_WriteString		  (cfg,"General"	,"Length"	,instance->p_slen);
	CfgIO_WriteString		  (cfg,"General"	,"Rate"		,instance->p_srat);
	for(o=0;o<ANZ_OSZIS;o++) {
		for(p=0;p<ANZ_OSZIS;p++) {
			sprintf(strItemName,"Matrix%d%d",o,p);
			CfgIO_WriteBool		(cfg,"Matrix",	strItemName	,(BOOL)instance->matrix[o][p]);
		}
		sprintf(strSectName,"Oszi%d",o);
		CfgIO_WriteString		(cfg,strSectName,"Oszilator",OsziLabels[instance->oszi[o]]);
		CfgIO_WriteString		(cfg,strSectName,"Phase"	,instance->p_phase[o]);
		//CfgIO_WriteString		(cfg,strSectName,"Amount"	,instance->amount[o]);
	}
	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	register UBYTE o,p;
	Param param;

	for(o=0;o<ANZ_OSZIS;o++) {		// init all oszis
		UC_ConvertParameter(instance->mod1[o].vals,"1.0",UGRP_RELFREQUENCY,RFRQ_FC);
		UC_ConvertParameter(instance->mod1[o].vale,"1.0",UGRP_RELFREQUENCY,RFRQ_FC);
		instance->mod1[o].bshape=BSHP_NONE;
		instance->mod1[o].desc[0]='\0';

		if(o) {
			UC_ConvertParameter(instance->mod2[o].vals,"0.0",UGRP_AMPLITUDE,AMP_FC);
			UC_ConvertParameter(instance->mod2[o].vale,"0.0",UGRP_AMPLITUDE,AMP_FC);
		}
		else {
			UC_ConvertParameter(instance->mod2[o].vals,"1.0",UGRP_AMPLITUDE,AMP_FC);
			UC_ConvertParameter(instance->mod2[o].vale,"1.0",UGRP_AMPLITUDE,AMP_FC);
		}
		instance->mod2[o].bshape=BSHP_NONE;
		instance->mod2[o].desc[0]='\0';

		UC_ConvertParameter(instance->p_phase[o],"0.0 °",UGRP_PHASE,PHA_D);
		instance->oszi[o]=0;
		//UC_ConvertParameter(instance->amount[o],"+1 st",UGRP_RELFREQUENCY,RFRQ_SEMITONES);

		for(p=0;p<ANZ_OSZIS;p++) instance->matrix[o][p]=FALSE;
	}

	UC_ConvertParameter(instance->p_slen ,"1 s",UGRP_ABSTIME,ATME_S);
	UC_ConvertParameter(instance->p_srat ,"22050 hz",UGRP_ABSFREQUENCY,AFRQ_HZ);
	UC_ConvertParameter(instance->p_pitch,"261.6255653 hz",UGRP_ABSFREQUENCY,AFRQ_HZ);
	UC_ConvertParameter(instance->p_ampf,"1.0",UGRP_AMPLITUDE,AMP_FC);

	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
	if(instance->slen<2) instance->slen=2;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	UBYTE i,j,k;
	aktop=0;

	SetModulator(&(instance->mod1[aktop]),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2[aktop]),modWin,modGadgets,GADIX_MOD2);
	GT_SetGadgetAttrs(modGadgets[GADIX_WAVE  ],modWin,0l,GTCY_Active,instance->oszi  [aktop],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_PHASE ],modWin,0l,GTST_String,instance->phase [aktop],TAG_DONE);
	//GT_SetGadgetAttrs(modGadgets[GADIX_AMOUNT],modWin,0l,GTST_String,instance->amount[aktop],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_VOL   ],modWin,0l,GTST_String,instance->p_ampf,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_FRQ   ],modWin,0l,GTST_String,instance->p_pitch,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_SLEN  ],modWin,0l,GTST_String,instance->p_slen,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_RATE  ],modWin,0l,GTST_String,instance->p_srat,TAG_DONE);

	for(k=j=0;j<ANZ_OSZIS;j++) {
		for(i=0;i<ANZ_OSZIS;i++,k++) {
			GT_SetGadgetAttrs(modGadgets[GADIX_MATRIX+k],modWin,0l,GTCB_Checked,instance->matrix[j][i],TAG_DONE);
		}
	}
}

//-- private Stuff ------------------------------------------------------------

// see fmsynth.mod/ReadVoice/1267
// see fmsynth.mod/LoadVoice/1346

void ReadFMSynth(struct Instance *instance,char *fname) {
	BPTR in;
	UBYTE buf[20];
	register UBYTE i,o;
	WORD numLines;
	Modulator *mod;
	ModUser *mus;
	ModVector *mve;
	double t1,t2;
	struct fmsy_operator	// for each operator
	{
		BYTE	scR,scL;		// KeyRate Scaling (rechts,links) (0..127)
		UBYTE	r[4],l[4];		// Amplituden Envelope (0..127)
		float	freq;			// Basefrequency
		BYTE	outp;			// Output-Level (0..127)
		BYTE	mode;
		BYTE	flags;
		BYTE	wave;			// Waveform (Sin=0,Tri,Saw,Sqr)
		BYTE	phase;			// Phase(-64..63)
		BYTE	detune;
	} fmsy_op;
	BYTE algo[30];				// mod0,car0,mod1,car1,...
	Param param;

	MSG1("importing FMSynth voice : %s",fname);

	if((in=Open(fname,MODE_OLDFILE))) {
		Read(in,buf,8);buf[8]='\0';
		MSG1("<%s>",buf);
		if(!strcmp(buf,"FMsy 1.3")) {
			MSG("valid file opened");
			for(o=0;o<6;o++) {
				MSG1("reading operator %d",o);
				Read(in,(APTR)(&fmsy_op),sizeof(struct fmsy_operator));
				mod=&instance->mod2[o];
				mod->desc[0]='\0';
				switch(mod->bshape) {
					case BSHP_USER:
						mus=(ModUser *)(mod->bshpdata);
						if(mus->envdata) FreeVec(mus->envdata);
						break;
				}
				if(mod->bshpdata) FreeVec(mod->bshpdata);
				if((mod->bshpdata=AllocVec(sizeof(ModVector),MEMF_ANY|MEMF_CLEAR))) {
					//-- convert dx7-volume envelope
					mod->bshape=BSHP_VECTOR;
					mve=(ModVector *)(mod->bshpdata);
					MSG4("%3d %3d %3d %3d",fmsy_op.l[0],fmsy_op.l[1],fmsy_op.l[2],fmsy_op.l[3]);
					MSG4("%3d %3d %3d %3d",fmsy_op.r[0],fmsy_op.r[1],fmsy_op.r[2],fmsy_op.r[3]);
					fmsy_op.r[0]=127-fmsy_op.r[0];/*if(fmsy_op.r[0]==0) fmsy_op.r[0]=128;*/		// see fmtools.mod/EG2Knobs/1329
					fmsy_op.r[1]=127-fmsy_op.r[1];/*if(fmsy_op.r[1]==0) fmsy_op.r[1]=128;*/
					fmsy_op.r[2]=127-fmsy_op.r[2];/*if(fmsy_op.r[2]==0) fmsy_op.r[2]=128;*/
					fmsy_op.r[3]=127-fmsy_op.r[3];/*if(fmsy_op.r[3]==0) fmsy_op.r[3]=128;*/
					MSG4("%3d %3d %3d %3d",fmsy_op.r[0],fmsy_op.r[1],fmsy_op.r[2],fmsy_op.r[3]);
					/*
					t1=(double)fmsy_op.r[0]+(double)fmsy_op.r[1]+(double)fmsy_op.r[2]+(double)fmsy_op.r[3];
					MSG1("%lf",t1);
					*/
					/* 127 is the longes possible length of one envelope part*/
					/* we have 4.0 envelope parts */
					t1=127.0*4.0;
					t2=0.5/t1;
					mve->anz=6;
					mve->lev[0]=0.0;                         mve->pos[0]=0.0;
					mve->lev[1]=((double)fmsy_op.l[0]/127.0);mve->pos[1]=mve->pos[0]+(fmsy_op.r[0]?((double)fmsy_op.r[0]/t1):t2);
					mve->lev[2]=((double)fmsy_op.l[1]/127.0);mve->pos[2]=mve->pos[1]+(fmsy_op.r[1]?((double)fmsy_op.r[1]/t1):t2);
					mve->lev[3]=((double)fmsy_op.l[2]/127.0);mve->pos[3]=mve->pos[2]+(fmsy_op.r[2]?((double)fmsy_op.r[2]/t1):t2);
					mve->lev[4]=((double)fmsy_op.l[3]/127.0);mve->pos[4]=mve->pos[3]+(fmsy_op.r[3]?((double)fmsy_op.r[3]/t1):t2);
					//mve->lev[5]=0.0;                         mve->pos[5]=1.0;
					mve->lev[5]=((double)fmsy_op.l[3]/127.0);mve->pos[5]=1.0;
					//for(i=1;i<5;i++) mve->pos[i]=pow(mve->pos[i],2.0);
				}
				else { mod->bshape=BSHP_NONE;strcpy(mod->desc,"None"); }
				AssignBShape(mod);
				sprintf(mod->vals,"0.0");
				sprintf(mod->vale,"%lf",(double)fmsy_op.outp/127.0);						// amplitude
				sprintf(instance->p_phase[o],"%lf °",(double)(fmsy_op.phase)*(360.0/128.0));	// phase
				instance->oszi[o]=fmsy_op.wave;												// waveform
			}
			Seek(in,8,OFFSET_CURRENT);								// OneShotHi,RepeatHi
			Read(in,&numLines,2);									// Algorithm
			Read(in,algo,30);
			memset(instance->matrix,0,ANZ_OSZIS*ANZ_OSZIS);
			for(i=0;i<(numLines<<1);i+=2) {
				instance->matrix[algo[i]][algo[i+1]]=1;
			}

			UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
			instance->srat=atof(param);
			if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
			else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
			if(instance->slen<2) instance->slen=2;
		}
		else {
			MSG1("version <%s> not yet supported",buf);
			Message(strNoID,"SoundFX does not support this file version yet.",__FILE__,__LINE__);
		}
		Close(in);
	}
	else MSG("open failed");
}

//-- eof ----------------------------------------------------------------------
