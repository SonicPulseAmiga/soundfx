/****h* Hall/lib-soundfx.c [4.3] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.3 (29.01.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Hall Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    29.Jan.2004	V 4.3	most recent version
*    31.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*  IDEAS
*    * problems
*      * low feedback - audible looping
*      * high feedback - resonating
*    * use StateVariable-Filters and redesign them, depending on input-sampling rate
*
*******
*/

#define HALL_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Hall.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

//-- Initialisation and Destruction

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

#define GADID_MOD3		(GADID_MOD2+MOD_IDCT)
#define GADIX_MOD3		(GADIX_MOD2+MOD_IXCT)

#define GADID_MOD4		(GADID_MOD3+MOD_IDCT)
#define GADIX_MOD4		(GADIX_MOD3+MOD_IXCT)

#define GADID_ERDELS	(GADID_MOD4+MOD_IDCT)
#define GADIX_ERDELS	(GADIX_MOD4+MOD_IXCT)

#define GADID_ERDELE	(GADID_ERDELS+1)
#define GADIX_ERDELE	(GADIX_ERDELS+1)

#define GADID_ERNR		(GADID_ERDELE+1)
#define GADIX_ERNR		(GADIX_ERDELE+1)

#define GADID_MRDELS	(GADID_ERNR+1)
#define GADIX_MRDELS	(GADIX_ERNR+1)

#define GADID_MRDELE	(GADID_MRDELS+1)
#define GADIX_MRDELE	(GADIX_MRDELS+1)

#define GADID_MRNR		(GADID_MRDELE+1)
#define GADIX_MRNR		(GADIX_MRDELE+1)

#define GADID_DRFC		(GADID_MRNR+1)
#define GADIX_DRFC		(GADIX_MRNR+1)

#define GADID_AMPF		(GADID_DRFC+1)
#define GADIX_AMPF		(GADIX_DRFC+1)

#define GADID_BRIGHT	(GADID_AMPF+1)
#define GADIX_BRIGHT	(GADIX_AMPF+1)

#define LAST_GADIX		(GADIX_BRIGHT+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[6];
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
		SFXMod_ModDescInit(3,PD_MODULATOR   ,"M3",&(instance->mod3));
		SFXMod_ModDescInit(4,PD_MODULATOR   ,"M4",&(instance->mod4));
		SFXMod_ModDescInit(5,PD_END         ,0l  ,0l);	
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
	UWORD ys5=ys4+1+MODSEL_HEIGHT;
	UWORD ys6=ys5+1+ANYFRM_HEIGHT(2);
	UWORD ys7=ys6+1+MODSEL_HEIGHT;
	UWORD ys8=ys7+1+ANYFRM_HEIGHT(2);
	UWORD ys9=ys8+1+ANYFRM_HEIGHT(2);
	UWORD winh=ys9+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[104];
		ng.ng_TopEdge		=ys9+RunTime->scy[2];
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
		g=AddModulator(&(instance->mod3),modGadgets,g,GADID_MOD3,GADIX_MOD3,xs1,ys4);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="Start";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_ERDELS;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_erdels,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_ERDELS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="End";
		ng.ng_GadgetID		=GADID_ERDELE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_erdele,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_ERDELE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Num.";
		ng.ng_GadgetID		=GADID_ERNR;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_ernr,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_ERNR]=g;

		g=AddModulator(&(instance->mod4),modGadgets,g,GADID_MOD4,GADIX_MOD4,xs1,ys6);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys7+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="Start";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_MRDELS;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_mrdels,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MRDELS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="End";
		ng.ng_GadgetID		=GADID_MRDELE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_mrdele,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MRDELE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys7+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Num.";
		ng.ng_GadgetID		=GADID_MRNR;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_mrnr,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MRNR]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys8+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_GadgetText	="Diff";
		ng.ng_GadgetID		=GADID_DRFC;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_drfc,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DRFC]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[214];
		ng.ng_GadgetText	="Ampf";
		ng.ng_GadgetID		=GADID_AMPF;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_ampf,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMPF]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_TopEdge		=ys8+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Bright";
		ng.ng_GadgetID		=GADID_BRIGHT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_bright,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_BRIGHT]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys4+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys6+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"						,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Effect"						,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Feedback, Early Reflections"	,xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Volume, Early Reflections"		,xs1,ys4,STDFRM_WIDTH);
			DrawTitle(modrp,"Delay, Early Reflections"		,xs1,ys5,STDFRM_WIDTH);
			DrawTitle(modrp,"Feedback, Main Reflections"	,xs1,ys6,STDFRM_WIDTH);
			DrawTitle(modrp,"Delay, Main Reflections"		,xs1,ys7,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"					,xs1,ys8,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"						,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Eff */
			DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);					/* Par */
			DrawGadWB(modrp,xs1,ys4,xh,MODSEL_HEIGHT);					/* Par */
			DrawGadWB(modrp,xs1,ys5,xh,ANYFRM_HEIGHT(2));				/* Par */
			DrawGadWB(modrp,xs1,ys6,xh,MODSEL_HEIGHT);					/* Par */
			DrawGadWB(modrp,xs1,ys7,xh,ANYFRM_HEIGHT(2));				/* Par */
			DrawGadWB(modrp,xs1,ys8,xh,ANYFRM_HEIGHT(2));				/* Par */
			DrawGadWB(modrp,xs1,ys9,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src ),modWin,modGadgets,GADIX_SRC);
			SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
			SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
			SetModulator(&(instance->mod3),modWin,modGadgets,GADIX_MOD3);
			SetModulator(&(instance->mod4),modWin,modGadgets,GADIX_MOD4);

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
								if(!HandleSource(&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)
								&& !HandleModulator(&(instance->mod1),&(instance->src),0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode)
								&& !HandleModulator(&(instance->mod2),&(instance->src),0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode)
								&& !HandleModulator(&(instance->mod3),&(instance->src),0l,modWin,modGadgets,GADID_MOD3,GADIX_MOD3,gnr,icode)
								&& !HandleModulator(&(instance->mod4),&(instance->src),0l,modWin,modGadgets,GADID_MOD4,GADIX_MOD4,gnr,icode)
								&& !HandleCfgSel   (&cfgsel       ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_ERDELS:
											HandleParameter(instance->p_erdels,modWin,modGadgets[GADIX_ERDELS]);
											break;
										case GADID_ERDELE:
											HandleParameter(instance->p_erdele,modWin,modGadgets[GADIX_ERDELE]);
											break;
										case GADID_ERNR:
											HandleParameter(instance->p_ernr,modWin,modGadgets[GADIX_ERNR]);
											break;
										case GADID_MRDELS:
											HandleParameter(instance->p_mrdels,modWin,modGadgets[GADIX_MRDELS]);
											break;
										case GADID_MRDELE:
											HandleParameter(instance->p_mrdele,modWin,modGadgets[GADIX_MRDELE]);
											break;
										case GADID_MRNR:
											HandleParameter(instance->p_mrnr,modWin,modGadgets[GADIX_MRNR]);
											break;
										case GADID_DRFC:
											HandleParameter(instance->p_drfc,modWin,modGadgets[GADIX_DRFC]);
											break;
										case GADID_AMPF:
											HandleParameter(instance->p_ampf,modWin,modGadgets[GADIX_AMPF]);
											break;
										case GADID_BRIGHT:
											HandleParameter(instance->p_bright,modWin,modGadgets[GADIX_BRIGHT]);
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
	UBYTE l;
	//-- prowin
	PWData *pwd;
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
	UBYTE pu;
	Param param;
	UBYTE newdst=FALSE;
	UBYTE okay=TRUE;
	double del;

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
	instance->effs=atof(param);
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_PROPOTION,PROP_FC);
	instance->effe=atof(param);
	instance->effd=instance->effe-instance->effs;

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_PROPOTION,PROP_FC);
	instance->erfbs=atof(param);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_PROPOTION,PROP_FC);
	instance->erfbe=atof(param);
	instance->erfbd=instance->erfbe-instance->erfbs;

	UC_ConvertParameter(param,instance->mod3.vals,UGRP_AMPLITUDE,AMP_FC);
	instance->ervols=atof(param);
	UC_ConvertParameter(param,instance->mod3.vale,UGRP_AMPLITUDE,AMP_FC);
	instance->ervole=atof(param);
	instance->ervold=instance->ervole-instance->ervols;

	if(UC_GetParameterUnit(instance->p_erdels,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
		UC_ConvertParameter(param,instance->p_erdels,UGRP_ABSFREQUENCY,AFRQ_HZ);if((del=atof(param))<1.0) del=1.0;
		instance->erdels=(double)srcbuf->srat/del;
	}
	else {
		if((pu=UC_GetParameterUnit(instance->p_erdels,UGRP_ABSTIME))!=INVALID_UNIT) {
			if(pu==ATME_SAMPLES) instance->erdels=atof(instance->p_erdels);
			else { UC_ConvertParameter(param,instance->p_erdels,UGRP_ABSTIME,ATME_MS);instance->erdels=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,instance->p_erdels,UGRP_RELTIME,RTME_FC);instance->erdels=(double)srcbuf->slen*atof(param); }
	}
	if(instance->erdels<1.0) instance->erdels=1.0;
	if(UC_GetParameterUnit(instance->p_erdele,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
		UC_ConvertParameter(param,instance->p_erdele,UGRP_ABSFREQUENCY,AFRQ_HZ);if((del=atof(param))<1.0) del=1.0;
		instance->erdele=(double)srcbuf->srat/del;
	}
	else {
		if((pu=UC_GetParameterUnit(instance->p_erdele,UGRP_ABSTIME))!=INVALID_UNIT) {
			if(pu==ATME_SAMPLES) instance->erdele=atof(instance->p_erdele);
			else { UC_ConvertParameter(param,instance->p_erdele,UGRP_ABSTIME,ATME_MS);instance->erdele=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,instance->p_erdele,UGRP_RELTIME,RTME_FC);instance->erdele=(double)srcbuf->slen*atof(param); }
	}
	if(instance->erdele<1.0) instance->erdele=1.0;
	if(instance->erdele<instance->erdels) { instance->erdeld=instance->erdele;instance->erdele=instance->erdels;instance->erdels=instance->erdeld; }
	instance->erdeld=instance->erdele-instance->erdels;
	UC_ConvertParameter(param,instance->p_ernr,UGRP_COUNT,COUN_ABS);
	instance->vanz=atoi(param);
	if(instance->vanz<1) instance->vanz=1;
	
	UC_ConvertParameter(param,instance->mod4.vals,UGRP_PROPOTION,PROP_FC);
	instance->mrfbs=atof(param);
	UC_ConvertParameter(param,instance->mod4.vale,UGRP_PROPOTION,PROP_FC);
	instance->mrfbe=atof(param);
	instance->mrfbd=instance->mrfbe-instance->mrfbs;

	if(UC_GetParameterUnit(instance->p_mrdels,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
		UC_ConvertParameter(param,instance->p_mrdels,UGRP_ABSFREQUENCY,AFRQ_HZ);if((del=atof(param))<1.0) del=1.0;
		instance->mrdels=(double)srcbuf->srat/del;
	}
	else {
		if((pu=UC_GetParameterUnit(instance->p_mrdels,UGRP_ABSTIME))!=INVALID_UNIT) {
			if(pu==ATME_SAMPLES) instance->mrdels=atof(instance->p_mrdels);
			else { UC_ConvertParameter(param,instance->p_mrdels,UGRP_ABSTIME,ATME_MS);instance->mrdels=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,instance->p_mrdels,UGRP_RELTIME,RTME_FC);instance->mrdels=(double)srcbuf->slen*atof(param); }
	}
	if(instance->mrdels<1.0) instance->mrdels=1.0;
	if(UC_GetParameterUnit(instance->p_mrdele,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
		UC_ConvertParameter(param,instance->p_mrdele,UGRP_ABSFREQUENCY,AFRQ_HZ);if((del=atof(param))<1.0) del=1.0;
		instance->mrdele=(double)srcbuf->srat/del;
	}
	else {
		if((pu=UC_GetParameterUnit(instance->p_mrdele,UGRP_ABSTIME))!=INVALID_UNIT) {
			if(pu==ATME_SAMPLES) instance->mrdele=atof(instance->p_mrdele);
			else { UC_ConvertParameter(param,instance->p_mrdele,UGRP_ABSTIME,ATME_MS);instance->mrdele=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,instance->p_mrdele,UGRP_RELTIME,RTME_FC);instance->mrdele=(double)srcbuf->slen*atof(param); }
	}
	if(instance->mrdele<1.0) instance->mrdele=1.0;
	if(instance->mrdele<instance->mrdels) { instance->mrdeld=instance->mrdele;instance->mrdele=instance->mrdels;instance->mrdels=instance->mrdeld; }
	instance->mrdeld=instance->mrdele-instance->mrdels;
	UC_ConvertParameter(param,instance->p_mrnr,UGRP_COUNT,COUN_ABS);
	instance->kanz=atoi(param);
	if(instance->kanz<1) instance->kanz=1;

	UC_ConvertParameter(param,instance->p_drfc,UGRP_PROPOTION,PROP_FC);
	instance->drfc=atof(param);

	UC_ConvertParameter(param,instance->p_ampf,UGRP_AMPLITUDE,AMP_FC);
	instance->ampf=atof(param);

	UC_ConvertParameter(param,instance->p_bright,UGRP_PROPOTION,PROP_FC);
	instance->bright=atof(param);

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
	if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
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
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }					// es wurde abgebrochen
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
		cf=TRUE;	pf=ParseModulatorSet  (&(instance->mod1),"P1",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod2),"P2",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod3),"P3",params);
		if(!pf) pf=ParseModulatorSet  (&(instance->mod4),"P4",params);
		if(!pf && !stricmp(RexxPar2,"ErdelS"))			{ strntcpy(instance->p_erdels,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"ErdelE"))			{ strntcpy(instance->p_erdele,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"ErNr"))			{ strntcpy(instance->p_ernr  ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"MrdelS"))			{ strntcpy(instance->p_mrdels,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"MrdelE"))			{ strntcpy(instance->p_mrdele,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"MrNr"))			{ strntcpy(instance->p_mrnr  ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Drfc"))			{ strntcpy(instance->p_drfc  ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Ampf"))			{ strntcpy(instance->p_ampf  ,RexxPar3,PARBUFLEN);pf=TRUE; }
		//if(!pf && !stricmp(RexxPar2,"Fb"))				{ strntcpy(instance->fb    ,RexxPar3,PARBUFLEN);pf=TRUE; }	/* P2/P4 */
		if(!pf && !stricmp(RexxPar2,"Bright"))			{ strntcpy(instance->p_bright,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet  (&(instance->mod1),"P1",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod2),"P2",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod3),"P3",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod4),"P4",params,ret);
		if(!pf && !stricmp(RexxPar2,"ErdelS"))			{ strntcpy(ret,instance->p_erdels,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"ErdelE"))			{ strntcpy(ret,instance->p_erdele,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"ErNr"))			{ strntcpy(ret,instance->p_ernr  ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"MrdelS"))			{ strntcpy(ret,instance->p_mrdels,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"MrdelE"))			{ strntcpy(ret,instance->p_mrdele,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"MrNr"))			{ strntcpy(ret,instance->p_mrnr  ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Drfc"))			{ strntcpy(ret,instance->p_drfc  ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Ampf"))			{ strntcpy(ret,instance->p_ampf  ,PARBUFLEN);pf=TRUE; }
		//if(!pf && !stricmp(RexxPar2,"Fb"))				{ strntcpy(ret,instance->fb    ,PARBUFLEN);/*pf=TRUE;*/ }	/* P2/P4 */
		if(!pf && !stricmp(RexxPar2,"Bright"))			{ strntcpy(ret,instance->p_bright,PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	Param param1,param2;
	UBYTE conv=FALSE; 

	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_erdels,CfgIO_ReadString	(cfg,"General"	,"ErdelS"	,"20 ms"));
	strcpy(instance->p_erdele,CfgIO_ReadString	(cfg,"General"	,"ErdelE"	,"150 ms"));
	strcpy(instance->p_ernr  ,CfgIO_ReadString	(cfg,"General"	,"ErNr"		,"10"));
	strcpy(instance->p_mrdels,CfgIO_ReadString	(cfg,"General"	,"MrdelS"	,"100 ms"));
	strcpy(instance->p_mrdele,CfgIO_ReadString	(cfg,"General"	,"MrdelE"	,"300 ms"));
	strcpy(instance->p_mrnr  ,CfgIO_ReadString	(cfg,"General"	,"MrNr"		,"16"));
	strcpy(instance->p_drfc  ,CfgIO_ReadString	(cfg,"General"	,"Drfc"		,"100 %"));
	strcpy(instance->p_ampf  ,CfgIO_ReadString	(cfg,"General"	,"Ampf"		,"1.35"));
	strcpy(instance->p_bright,CfgIO_ReadString	(cfg,"General"	,"Bright"	,"80 %"));

	// compatibillity with pre V4.1 
	strcpy(param1,CfgIO_ReadString	(cfg,"General"	,"Fb"	,"---"));
	if(strcmp(param1,"---")) {
		CfgIO_DeleteItem(cfg,"General"	,"Fb");
		//MSG1("fb : %s",param1);
		UC_ConvertParameter(instance->mod4.vals,param1,UGRP_PROPOTION,PROP_FC);
		UC_ConvertParameter(instance->mod4.vale,param1,UGRP_PROPOTION,PROP_FC);
		conv=TRUE;
	}
	strcpy(param1,CfgIO_ReadString	(cfg,"General"	,"Erfc"	,"---"));
	if(strcmp(param1,"---")) {
		double fc;
		CfgIO_DeleteItem(cfg,"General"	,"Erfc");
		UC_ConvertParameter(param2,param1,UGRP_PROPOTION,PROP_FC);fc=atof(param2);
		//MSG3("erfc : %s -> %s -> %f",param1,param2,fc);
		sprintf(param1,"%d ms",(ULONG)( 20.0*fc));UC_ConvertParameter(instance->p_erdels,param1,UGRP_ABSTIME,ATME_MS);
		sprintf(param1,"%d ms",(ULONG)(100.0*fc));UC_ConvertParameter(instance->p_erdele,param1,UGRP_ABSTIME,ATME_MS);
		conv=TRUE;
	}
	strcpy(param1,CfgIO_ReadString	(cfg,"General"	,"Mrfc"	,"---"));
	if(strcmp(param1,"---")) {
		double fc;
		CfgIO_DeleteItem(cfg,"General"	,"Mrfc");
		UC_ConvertParameter(param2,param1,UGRP_PROPOTION,PROP_FC);fc=atof(param2);
		//MSG3("mrfc : %s -> %s -> %f",param1,param2,fc);
		sprintf(param1,"%d ms",(ULONG)( 50.0*fc));UC_ConvertParameter(instance->p_mrdels,param1,UGRP_ABSTIME,ATME_MS);
		sprintf(param1,"%d ms",(ULONG)( 90.0*fc));UC_ConvertParameter(instance->p_mrdele,param1,UGRP_ABSTIME,ATME_MS);
		conv=TRUE;
	}
	if(conv) {
		//MSG1("mod2-defaults : [%s]",instance->mod2.vals);
		UC_ConvertParameter(instance->mod2.vals,"50 %",UGRP_PROPOTION,PROP_FC);
		UC_ConvertParameter(instance->mod2.vale,"50 %",UGRP_PROPOTION,PROP_FC);

		//MSG1("mod3-defaults : [%s]",instance->mod3.vals);
		UC_ConvertParameter(instance->mod3.vals,"1.0",UGRP_AMPLITUDE,AMP_FC);
		UC_ConvertParameter(instance->mod3.vale,"0.0",UGRP_AMPLITUDE,AMP_FC);
		instance->mod3.bshape=BSHP_CURVE;
		if((instance->mod3.bshpdata=AllocVec(sizeof(ModCurve),MEMF_ANY|MEMF_CLEAR))) {
			ModCurve *mcu=(ModCurve *)(instance->mod3.bshpdata);
			mcu->exp=1.0;
		}
		else instance->mod3.bshape=BSHP_NONE;
	}
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"ErdelS"	,instance->p_erdels);
	CfgIO_WriteString		  (cfg,"General"	,"ErdelE"	,instance->p_erdele);
	CfgIO_WriteString		  (cfg,"General"	,"ErNr"		,instance->p_ernr);
	CfgIO_WriteString		  (cfg,"General"	,"MrdelS"	,instance->p_mrdels);
	CfgIO_WriteString		  (cfg,"General"	,"MrdelE"	,instance->p_mrdele);
	CfgIO_WriteString		  (cfg,"General"	,"MrNr"		,instance->p_mrnr);
	CfgIO_WriteString		  (cfg,"General"	,"Drfc"		,instance->p_drfc);
	CfgIO_WriteString		  (cfg,"General"	,"Ampf"		,instance->p_ampf);
	CfgIO_WriteString		  (cfg,"General"	,"Bright"	,instance->p_bright);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	// Dry/Wet
	UC_ConvertParameter(instance->mod1.vals,"0.4",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod1.vale,"0.4",UGRP_PROPOTION,PROP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	// ER, Feedback
	UC_ConvertParameter(instance->mod2.vals,"50 %",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod2.vale,"50 %",UGRP_PROPOTION,PROP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	// ER,Volume
	UC_ConvertParameter(instance->mod3.vals,"1.0",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->mod3.vale,"0.0",UGRP_AMPLITUDE,AMP_FC);
	instance->mod3.bshape=BSHP_CURVE;
	if((instance->mod3.bshpdata=AllocVec(sizeof(ModCurve),MEMF_ANY|MEMF_CLEAR))) {
		ModCurve *mcu=(ModCurve *)(instance->mod3.bshpdata);
		mcu->exp=1.0;
	}
	else instance->mod3.bshape=BSHP_NONE;
	instance->mod3.desc[0]='\0';

	// ER, Delay
	UC_ConvertParameter(instance->p_erdels,"20 ms"	,UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->p_erdele,"150 ms"	,UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->p_ernr  ,"10"		 ,UGRP_COUNT,COUN_ABS);

	// MR, Feedback
	UC_ConvertParameter(instance->mod4.vals,"70 %",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod4.vale,"70 %",UGRP_PROPOTION,PROP_FC);
	instance->mod4.bshape=BSHP_NONE;
	instance->mod4.desc[0]='\0';

	// MR, Delay
	UC_ConvertParameter(instance->p_mrdels,"100 ms"	,UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->p_mrdele,"300 ms"	,UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->p_mrnr  ,"16"		,UGRP_COUNT,COUN_ABS);

	UC_ConvertParameter(instance->p_drfc  ,"100 %"	,UGRP_PROPOTION,PROP_PROZ);
	UC_ConvertParameter(instance->p_ampf  ,"1.35"	,UGRP_AMPLITUDE,AMP_FC);
	//UC_ConvertParameter(instance->fb    ,"15 %"	,UGRP_PROPOTION,PROP_PROZ);
	UC_ConvertParameter(instance->p_bright,"80 %"	,UGRP_PROPOTION,PROP_PROZ);
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin){ 
	SetSource   (&(instance->src ),modWin,modGadgets,GADIX_SRC);
	SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
	SetModulator(&(instance->mod3),modWin,modGadgets,GADIX_MOD3);
	SetModulator(&(instance->mod4),modWin,modGadgets,GADIX_MOD4);
	GT_SetGadgetAttrs(modGadgets[GADIX_ERDELS],modWin,0l,GTST_String,instance->p_erdels,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_ERDELE],modWin,0l,GTST_String,instance->p_erdele,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_ERNR  ],modWin,0l,GTST_String,instance->p_ernr  ,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MRDELS],modWin,0l,GTST_String,instance->p_mrdels,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MRDELE],modWin,0l,GTST_String,instance->p_mrdele,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MRNR  ],modWin,0l,GTST_String,instance->p_mrnr  ,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_DRFC  ],modWin,0l,GTST_String,instance->p_drfc  ,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_AMPF  ],modWin,0l,GTST_String,instance->p_ampf  ,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_BRIGHT],modWin,0l,GTST_String,instance->p_bright,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
