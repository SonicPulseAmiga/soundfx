/****h* Synthesize-Add/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (13.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Synthesize-Add Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    13.May.2002	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*  IDEAS
*    * parameter to control wave
*    * new waves
*       * y=exp(x%1.0,par);  (par=1.0 -> saw)
*       * y=pulse(x%PI,par); (par=0.5 -> sqr)
*
*******
*/

#define SYNTHESIZE_ADD_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasSource

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Synthesize-Add.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

void DrawWave(struct Instance *instance,struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye);
void DrawTones(struct Instance *instance,struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye);
void DrawTone(struct Instance *instance,struct RastPort *rp,UBYTE akttone,UWORD xs,UWORD ys,UWORD xe,UWORD ye);

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

#define GADID_MOD1		(GADID_CFGSEL+CFGSEL_IDCT)
#define GADIX_MOD1		(GADIX_CFGSEL+CFGSEL_IXCT)

#define GADID_MOD2		(GADID_MOD1+MOD_IDCT)
#define GADIX_MOD2		(GADIX_MOD1+MOD_IXCT)

#define GADID_SLEN		(GADID_MOD2+MOD_IDCT)
#define GADIX_SLEN		(GADIX_MOD2+MOD_IXCT)

#define GADID_ONEPER	(GADID_SLEN+1)

#define GADID_RATE		(GADID_ONEPER+1)
#define GADIX_RATE		(GADIX_SLEN+1)

#define GADID_RATESEL	(GADID_RATE+1)

#define GADID_VOL		(GADID_RATESEL+1)
#define GADIX_VOL		(GADIX_RATE+1)

#define GADID_MAXVOL	(GADID_VOL+1)

#define GADID_FRQ		(GADID_MAXVOL+1)
#define GADIX_FRQ		(GADIX_VOL+1)

#define GADID_FRQSEL	(GADID_FRQ+1)

#define GADID_RANGE		(GADID_FRQSEL+1)

#define GADID_MOVEMODE	(GADID_RANGE+1)

#define GADID_WAVE		(GADID_MOVEMODE+1)
#define GADIX_WAVE		(GADIX_FRQ+1)

#define GADID_NR		(GADID_WAVE+1)
#define GADIX_NR		(GADIX_WAVE+1)

#define GADID_VAL		(GADID_NR+1)
#define GADIX_VAL		(GADIX_NR+1)

#define GADID_PHASE		(GADID_VAL+1)
#define GADIX_PHASE		(GADID_VAL+1)

#define GADID_UP2		(GADID_PHASE+1)
#define GADID_UP		(GADID_UP2+1)
#define GADID_VERTFLIP	(GADID_UP+1)
#define GADID_DOWN		(GADID_VERTFLIP+1)
#define GADID_DOWN2		(GADID_DOWN+1)

#define GADID_LEFT2		(GADID_DOWN2+1)
#define GADID_LEFT		(GADID_LEFT2+1)
#define GADID_RIGHT		(GADID_LEFT+1)
#define GADID_RIGHT2	(GADID_RIGHT+1)

#define LAST_GADIX		(GADIX_PHASE+1)

//-- Gfx Size

#define GFX_XRES		128
#define GFX_YRES		 64

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[4];
};

UBYTE akttone=0;			// wegen RefrehGUI

UBYTE *ModeLabels[]={ "Cur","All","Pos","Neg",0l };
UBYTE *OsziLabels[]={ "Sin","Tri","Saw","Sqr",0l };

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
		SFXMod_ModDescInit(0,PD_COREINSTANCE,0l  , instance);
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
	UWORD ys2=ys1+1+RunTime->scy[117+FRMTITLE_HEIGHT];
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(1);
	UWORD ys4=ys3+1+MODSEL_HEIGHT;
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD ys5=ys1+1+RunTime->scy[GFX_YRES+4+FRMTITLE_HEIGHT+FRMLINE_HEIGHT];
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD xs3=xs2+1+STDFRM_WIDTH;
	UWORD winw=xs3+3+CFGSEL_WIDTH;
	UWORD xs5=xs1+1+RunTime->scx[170];		/* wave-preview */
	UWORD xh,yh;

	UWORD xsg=xs1+RunTime->scx[       6],ysg=ys1+RunTime->scy[FRMCONTENT_YOFFSET]+1;	/* wave-preview */
	UWORD xlg=2  +RunTime->scx[GFX_XRES],ylg=2+  RunTime->scy[GFX_YRES];
	UWORD xeg=xsg+xlg,yeg=ysg+ylg;
	UWORD xs =xs1+RunTime->scx[176],ys =ys1+RunTime->scy[FRMCONTENT_YOFFSET];	/* obertones */
	UWORD xl =2  +RunTime->scx[384],yl =2  +RunTime->scy[100];
	UWORD xe =xs +xl ,ye =ys +yl;

	register ULONG i;
	register UWORD j;
	register UBYTE k;
	double aktval,oldval;
	double mscale,mval;
	UBYTE mouseb=FALSE,range=FALSE;
	UBYTE omptr=MPTR_STANDARD;
	double angle,anglediff,an;
	double frq;
	double si0,si1,fc;
	ULONG slen,srat;
	LONG rate,oneper;
	UWORD mousex,mousey;
	UBYTE oldtone;
	double rdif,rval1,rval2;
	UBYTE mode=0;
	double *sv=instance->svals;
	Param param1,param2;
	LONG val1;
	Source			dummysrc;
	SInfo			dummysi={0};

	akttone=0;
	wintags[4].ti_Data|=IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS;
	wintags[5].ti_Data|=WFLG_REPORTMOUSE;

	dummysrc.srcbuf=&dummysi;
	UC_ConvertParameter(param1,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	dummysi.srat=atof(param1);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) dummysi.slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param1,instance->p_slen,UGRP_ABSTIME,ATME_MS);dummysi.slen=atof(param1)*((double)dummysi.srat/1000.0); }
	if(dummysi.slen<2) dummysi.slen=2;

	SFXMod_OpenGUILibs;
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[258];
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
		g=AddCfgSel(&cfgsel,modGadgets,g,GADID_CFGSEL,GADIX_CFGSEL,xs3,ys1,winh-2);

		g=AddModulator(&(instance->mod1),modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys3);
		g=AddModulator(&(instance->mod2),modGadgets,g,GADID_MOD2,GADIX_MOD2,xs2,ys3);

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];				/* Länge */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[65];
		ng.ng_GadgetText	="SLen";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_SLEN;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_slen,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_SLEN]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[102];				/* Converting Länge/OnePer */
		ng.ng_GadgetText	="OnePer";
		ng.ng_GadgetID		=GADID_ONEPER;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[210];				/* Rate */
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_RATESEL;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[226];
		ng.ng_Width			=RunTime->scx[65];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_RATE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_srat,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_RATE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[330];				/* Volume */
		ng.ng_Width			=RunTime->scx[75];
		ng.ng_GadgetText	="Vol";
		ng.ng_GadgetID		=GADID_VOL;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->scale,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_VOL]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[406];
		ng.ng_Width			=RunTime->scx[55];
		ng.ng_GadgetText	="MaxVol";
		ng.ng_GadgetID		=GADID_MAXVOL;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[502];					/* Pitch */
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_FRQSEL;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[518];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_FRQ;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_pitch,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FRQ]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[6];					/* Wave-Params */
		ng.ng_TopEdge		=yeg+RunTime->scy[3];
		ng.ng_Width			=RunTime->scx[60];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_WAVE;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,OsziLabels,GTCY_Active,instance->oszi,TAG_DONE);
		modGadgets[GADIX_WAVE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[110];
		ng.ng_Width			=RunTime->scx[50];
		ng.ng_GadgetText	="Pha.";
		ng.ng_GadgetID		=GADID_PHASE;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_phase,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_PHASE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[6];					// curve processing
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[60];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	="Range";
		ng.ng_GadgetID		=GADID_RANGE;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_MOVEMODE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ModeLabels,GTCY_Active,mode,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[110];
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[50];
		ng.ng_GadgetText	="Nr.";
		ng.ng_GadgetID		=GADID_NR;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,3,GTIN_Number,akttone,TAG_DONE);
		modGadgets[GADIX_NR]=g;
		/** @TODO change !!! ? */
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Val.";
		ng.ng_GadgetID		=GADID_VAL;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,4,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
		modGadgets[GADIX_VAL]=g;
		/** @TODO change !!! ? */

		ng.ng_LeftEdge		=xe+1;								/* Arrows vert.*/
		ng.ng_TopEdge		=ys;
		ng.ng_Width			=RunTime->scx[19];
		ng.ng_Height		=RunTime->scy[20];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_ArrowUp2;
		ng.ng_GadgetID		=GADID_UP2;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_TopEdge		=ys+RunTime->scy[20];
		ng.ng_GadgetText	=IMAGE_ArrowUp;
		ng.ng_GadgetID		=GADID_UP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_TopEdge		=ys+RunTime->scy[40];
		ng.ng_Height		=RunTime->scy[22];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="F";
		ng.ng_GadgetID		=GADID_VERTFLIP;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_TopEdge		=ys+RunTime->scy[62];
		ng.ng_Height		=RunTime->scy[20];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_ArrowDown;	
		ng.ng_GadgetID		=GADID_DOWN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_TopEdge		=ys+RunTime->scy[82];
		ng.ng_GadgetText	=IMAGE_ArrowDown2;
		ng.ng_GadgetID		=GADID_DOWN2;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs;								/* Arrows horiz. */
		ng.ng_TopEdge		=ye+1;
		ng.ng_Width			=RunTime->scx[97];
		ng.ng_Height		=RunTime->scy[10];
		ng.ng_GadgetText	=IMAGE_ArrowLeft2;
		ng.ng_GadgetID		=GADID_LEFT2;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs+RunTime->scx[97];
		ng.ng_GadgetText	=IMAGE_ArrowLeft;
		ng.ng_GadgetID		=GADID_LEFT;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs+RunTime->scx[194];
		ng.ng_GadgetText	=IMAGE_ArrowRight;
		ng.ng_GadgetID		=GADID_RIGHT;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs+RunTime->scx[291];
		ng.ng_GadgetText	=IMAGE_ArrowRight2;	
		ng.ng_GadgetID		=GADID_RIGHT2;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Rate",xs1+RunTime->scx[202],ys2+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Frq",xs1+RunTime->scx[494],ys2+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[ 29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs2+RunTime->scx[ 29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Wave"				,xs1,ys1,RunTime->scx[170]);
			DrawTitle(modrp,"Curve-Editing"		,xs1,ys5,RunTime->scx[170]);
			DrawTitle(modrp,"Harmonics"			,xs5,ys1,RunTime->scx[616-170]);
			xh=STDFRM_WIDTH;
			DrawTitle(modrp,"Frequency"			,xs1,ys3,xh);
			DrawTitle(modrp,"Amplitude"			,xs2,ys3,xh);
			xh=xh+xh+1;
			DrawTitle(modrp,"Miscellaneous"		,xs1,ys2,xh);
			DrawTitle(modrp,"Presets"			,xs3,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;yh=MODSEL_HEIGHT;
			DrawGadWB(modrp,xs1,ys3,xh,yh);									/* Frq */
			DrawGadWB(modrp,xs2,ys3,xh,yh);									/* Amp */
			yh=RunTime->scy[GFX_YRES+4+FRMTITLE_HEIGHT+FRMLINE_HEIGHT];
			DrawGadWB(modrp,xs1,ys1,RunTime->scx[170],yh);					/* Wave */
			yh=RunTime->scy[117+FRMTITLE_HEIGHT]-(yh+1);
			DrawGadWB(modrp,xs1,ys5,RunTime->scx[170],yh);					/* Curve-Ed. */
			xh=(STDFRM_WIDTH+STDFRM_WIDTH)-RunTime->scx[170];
			DrawGadWB(modrp,xs5,ys1,xh,RunTime->scy[117+FRMTITLE_HEIGHT]);	/* Obertones */
			xh=STDFRM_WIDTH+STDFRM_WIDTH+1;
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(1));					/* Par */
			DrawGadWB(modrp,xs1,ys4,xh,CMDFRM_HEIGHT);						/* Go */
			DrawGadBW(modrp,xsg-1,ysg-1,xlg+2,ylg+2);						/* Wave */
			DrawGadBW(modrp,xs,ys,xl,yl);									/* Tones */
			DrawGadWB(modrp,xs3,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);			/* CfgSel */

			DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
			DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);

			SetCfgSel   (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
			SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);

			while(runing) {
				WaitPort(modWin->UserPort);
				while(imsg=GT_GetIMsg(modWin->UserPort)) {
					mousex	=(imsg->MouseX)-(modWin->BorderLeft);
					mousey	=(imsg->MouseY)-(modWin->BorderTop);
					SFXMod_Interface_AnalyseIMsg
					switch(iclass) {
						case IDCMP_GADGETUP:
							gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
							if(!HandleModulator(&(instance->mod1),&dummysrc,0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode)
							&& !HandleModulator(&(instance->mod2),&dummysrc,0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode)
							&& !HandleCfgSel   (&cfgsel       ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
								switch(gnr) {
									SFXMod_Interface_GadgetUpEvents
									case GADID_RATESEL:
										UC_ConvertParameter(param1,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										rate=atof(param1);
										if(rate=GetPeriode(rate,NULL)) {
											sprintf(param1,"%ld hz",rate);
											UC_ConvertParameter(instance->p_srat,param1,UGRP_ABSFREQUENCY,UC_GetParameterUnit(instance->p_srat,UGRP_ABSFREQUENCY));
											GT_SetGadgetAttrs(modGadgets[GADIX_RATE],modWin,0l,GTST_String,instance->p_srat,TAG_DONE);
											UC_ConvertParameter(param1,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
											srat=atof(param1);
											if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(instance->p_slen);
											else { UC_ConvertParameter(param1,instance->p_slen,UGRP_ABSTIME,ATME_MS);slen=atof(param1)*((double)srat/1000.0); }
											if(slen<2) slen=2;
											instance->mod1.slen=slen;
											instance->mod1.srat=srat;
											instance->mod2.slen=slen;
											instance->mod2.srat=srat;
										}
										break;
									case GADID_RATE:
										HandleParameter(instance->p_srat,modWin,modGadgets[GADIX_RATE]);
										UC_ConvertParameter(param1,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										srat=atof(param1);
										if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(instance->p_slen);
										else { UC_ConvertParameter(param1,instance->p_slen,UGRP_ABSTIME,ATME_MS);slen=atof(param1)*((double)srat/1000.0); }
										if(slen<2) slen=2;
										instance->mod1.slen=slen;
										instance->mod1.srat=srat;
										instance->mod2.slen=slen;
										instance->mod2.srat=srat;
										break;
									case GADID_SLEN:
										HandleParameter(instance->p_slen,modWin,modGadgets[GADIX_SLEN]);
										UC_ConvertParameter(param1,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										srat=atof(param1);
										if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(instance->p_slen);
										else { UC_ConvertParameter(param1,instance->p_slen,UGRP_ABSTIME,ATME_MS);slen=atof(param1)*((double)srat/1000.0); }
										if(slen<2) slen=2;
										instance->mod1.slen=slen;
										instance->mod1.srat=srat;
										instance->mod2.slen=slen;
										instance->mod2.srat=srat;
										break;
									case GADID_ONEPER:
										UC_ConvertParameter(param1,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										UC_ConvertParameter(param2,instance->p_pitch,UGRP_ABSFREQUENCY,AFRQ_HZ);
										oneper=(LONG)(atof(param1)/atof(param2));
										oneper=max(oneper,2);oneper&=0xFFFFFFFE;
										if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) {
											sprintf(param2,"%ld",oneper);
											UC_ConvertParameter(instance->p_slen,param2,UGRP_ABSTIME,ATME_SAMPLES);
										}
										else {
											sprintf(param2,"%lf ms",(((double)oneper*1000.0)/atof(param1)));
											UC_ConvertParameter(instance->p_slen,param2,UGRP_ABSTIME,UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME));
										}
										GT_SetGadgetAttrs(modGadgets[GADIX_SLEN],modWin,0l,GTST_String,instance->p_slen,TAG_DONE);
										break;
									case GADID_FRQSEL:
										UC_ConvertParameter(param1,instance->p_pitch,UGRP_ABSFREQUENCY,AFRQ_HZ);
										frq=atof(param1);
										Frq2RateKey(&frq,(ULONG *)(&rate),0l);
										if(rate=GetPeriode(rate,NULL)) {
											if(Rate2KeyFrq(rate,0l,&frq)) {
												sprintf(param1,"%lf hz",frq);
												UC_ConvertParameter(instance->p_pitch,param1,UGRP_ABSFREQUENCY,UC_GetParameterUnit(instance->p_pitch,UGRP_ABSFREQUENCY));
												GT_SetGadgetAttrs(modGadgets[GADIX_FRQ],modWin,0l,GTST_String,instance->p_pitch,TAG_DONE);
											}
										}
										break;
									case GADID_FRQ:
										HandleParameter(instance->p_pitch,modWin,modGadgets[GADIX_FRQ]);
										break;
									case GADID_RANGE:
										range=1;
										break;
									case GADID_MOVEMODE:
										mode=icode;
										break;
									case GADID_WAVE:
										instance->oszi=icode;
										DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										break;
									case GADID_NR:
										CheckGad_int(modGadgets[GADIX_NR],modWin,&val1,1,0,64);akttone=(UBYTE)val1;
										GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
										ActivateGadget(modGadgets[GADIX_VAL],modWin,0L);
										break;
									case GADID_VAL:
										DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
										val1=atoi(((struct StringInfo *)(modGadgets[GADIX_VAL]->SpecialInfo))->Buffer);
										if(val1>100) val1=100;
										else if(val1<-100) val1=-100;
										sv[akttone]=(double)val1/100.0;
										DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
										ActivateGadget(modGadgets[GADIX_NR],modWin,0L);
										break;
									case GADID_PHASE:
										HandleParameter(instance->p_phase,modWin,modGadgets[GADIX_PHASE]);
										DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										break;
									case GADID_VOL:
										HandleParameter(instance->scale,modWin,modGadgets[GADIX_VOL]);
										DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										break;
									case GADID_MAXVOL:				/* MaxVol */
										UC_ConvertParameter(param1,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
										UC_ConvertParameter(param2,instance->p_pitch,UGRP_ABSFREQUENCY,AFRQ_HZ);
										oneper=(LONG)(atof(param1)/atof(param2));
										oneper=max(oneper,2);oneper&=0xFFFFFFFE;
										UC_ConvertParameter(param1,instance->p_phase,UGRP_PHASE,PHA_RAD);
										angle=atof(param1);
										anglediff=PIM2/(double)oneper;
										mval=0.0;
										switch(instance->oszi) {
											case WAVE_SIN:
												for(i=0;i<oneper;i++) {
													mscale=0L;an=angle;
													si0=sin(-(angle+angle)+an);
													si1=sin(-angle+an);
													fc=cos(angle);fc+=fc;
													for(j=0;j<64;) {
														si0=fc*si1-si0;
														if(sv[j]!=0.0) mscale+=(si0*sv[j]);
														j++;
														si1=fc*si0-si1;
														if(sv[j]!=0.0) mscale+=(si1*sv[j]);
														j++;
													}
													if(fabs(mscale)>mval) mval=fabs(mscale);
													angle+=anglediff;
												}
												break;
											case WAVE_TRI:
												for(i=0;i<oneper;i++) {
													mscale=0L;an=angle;
													for(j=0;j<64;j++) {
														if(sv[j]!=0.0) mscale+=(sfxtri(an)*sv[j]);
														an+=angle;
													}
													if(fabs(mscale)>mval) mval=fabs(mscale);
													angle+=anglediff;
												}
												break;
											case WAVE_SAW:
												for(i=0;i<oneper;i++) {
													mscale=0L;an=angle;
													for(j=0;j<64;j++) {
														if(sv[j]!=0.0) mscale+=(sfxsaw(an)*sv[j]);
														an+=angle;
													}
													if(fabs(mscale)>mval) mval=fabs(mscale);
													angle+=anglediff;
												}
												break;
											case WAVE_SQR:
												for(i=0;i<oneper;i++) {
													mscale=0L;an=angle;
													for(j=0;j<64;j++) {
														if(sv[j]!=0.0) mscale+=(sfxsqr(an)*sv[j]);
														an+=angle;
													}
													if(fabs(mscale)>mval) mval=fabs(mscale);
													angle+=anglediff;
												}
												break;
										}
										sprintf(param1,"%lf",(1.0/mval));
										UC_ConvertParameter(instance->scale,param1,UGRP_AMPLITUDE,UC_GetParameterUnit(instance->scale,UGRP_AMPLITUDE));
										GT_SetGadgetAttrs(modGadgets[GADIX_VOL],modWin,0l,GTST_String,instance->scale,TAG_DONE);
										DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										break;
									case GADID_UP:			/* 1 x hoch */
										if(mode>0) {
											for(k=0;k<64;k++) {
												if(sv[k]<1.0) {
													if((mode==1) || (mode==2 && sv[k]>=0.0) || (mode==3 && sv[k]<=0.0))
														sv[k]+=0.01;
												}
											}
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										}
										else {
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											if(sv[akttone]<1.0) sv[akttone]+=0.01;
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										}
										break;
									case GADID_UP2:			/* 5 x hoch */
										if(mode) {
											for(k=0;k<64;k++) {
												if(sv[k]<=0.95) {
													if((mode==1) || (mode==2 && sv[k]>=0.0) || (mode==3 && sv[k]<=0.0))
														sv[k]+=0.05;
												}
												else {
													if((mode==1) || (mode==2 && sv[k]>=0.0) || (mode==3 && sv[k]<=0.0))
														sv[k]=1.0;
												}
											}
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										}
										else {
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											if(sv[akttone]<=0.95) sv[akttone]+=0.05;
											else sv[akttone]=1.0;
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										}
										break;
									case GADID_VERTFLIP:			/* Flip */
										if(mode) {
											switch(mode) {
												case 1:
													for(k=0;k<64;k++)
														sv[k]*=-1.0;
													break;
												case 2:
													for(k=0;k<64;k++)
														if(sv[k]>0.0) sv[k]*=-1.0;
													break;
												case 3:
													for(k=0;k<64;k++)
														if(sv[k]<0.0) sv[k]*=-1.0;
													break;
											}
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										}
										else {
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											sv[akttone]*=-1.0;
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										}
										break;
									case GADID_DOWN2:			/* 5 x runter */
										if(mode) {
											for(k=0;k<64;k++) {
												if(sv[k]>=-0.95) {
													if((mode==1) || (mode==2 && sv[k]>=0.0) || (mode==3 && sv[k]<=0.0))
														sv[k]-=0.05;
												}
												else {
													if((mode==1) || (mode==2 && sv[k]>=0.0) || (mode==3 && sv[k]<=0.0))
														sv[k]=-1.0;
												}
											}
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										}
										else {
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											if(sv[akttone]>=-0.95) sv[akttone]-=0.05;
											else sv[akttone]=-1.0;
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										}
										break;
									case GADID_DOWN:			/* 1 x runter */
										if(mode>0) {
											for(k=0;k<64;k++) {
												if(sv[k]>-1.0) {
													if((mode==1) || (mode==2 && sv[k]>=0.0) || (mode==3 && sv[k]<=0.0))
														sv[k]-=0.01;
												}
											}
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										}
										else {
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											if(sv[akttone]>-1.0) sv[akttone]-=0.01;
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										}
										break;
									case GADID_LEFT2:		/* 5 x links */
										for(k=0;k<59;k++) sv[k]=sv[k+5];
										sv[63]=sv[62]=sv[61]=sv[60]=sv[59]=0.0;
										GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
										DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										break;
									case GADID_LEFT:		/* 1 x links */
										for(k=0;k<64;k++) sv[k]=sv[k+1];
										sv[63]=0.0;
										GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
										DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										break;
									case GADID_RIGHT:		/* 1 x rechts */
										for(k=63;k>0;k--) sv[k]=sv[k-1];
										sv[0]=0.0;
										GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
										DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										break;
									case GADID_RIGHT2:		/* 5 x rechts */
										for(k=63;k>4;k--) sv[k]=sv[k-5];
										sv[0]=sv[1]=sv[2]=sv[3]=sv[4]=0.0;
										GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
										DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										break;
								}
							}
							break;
						case IDCMP_MOUSEBUTTONS:
							if(icode==SELECTDOWN) mouseb=TRUE;
							if(icode==SELECTUP && mouseb) {
								mouseb=FALSE;
								GT_SetGadgetAttrs(modGadgets[GADIX_NR],modWin,0l,GTIN_Number,akttone,TAG_DONE);
								GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
							}		/* No break !!! */
						case IDCMP_MOUSEMOVE:
							if((mousex>xs) && (mousex<xe) && (mousey>ys) && (mousey<ye)) {
								if(mouseb) {
									if(range) {
										range=0;
										oldtone=akttone;akttone=(mousex-(xs+1))/(float)RunTime->scx[6];
										if(akttone>63) akttone=63;
										if(akttone!=oldtone) {
											rval1=sv[oldtone];
											rval2=sv[akttone];
											rdif=(rval2-rval1)/(double)labs(akttone-oldtone);
											if(oldtone<akttone) {
												for(j=oldtone;j<akttone;j++) {
													sv[j]=rval1;
													rval1+=rdif;
												}
											}
											else {
												for(j=oldtone;j>akttone;j--) {
													sv[j]=rval1;
													rval1+=rdif;
												}
											}
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);DrawTones(instance,modrp,xs+1,ys+1,xe-1,ye-1);
										}
									}
									else {
										oldtone=akttone;akttone=(mousex-(xs+1))/(float)RunTime->scx[6];
										if(akttone>63) akttone=63;
										if(akttone!=oldtone) {
											GT_SetGadgetAttrs(modGadgets[GADIX_NR],modWin,0l,GTIN_Number,akttone,TAG_DONE);
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)sv[akttone],TAG_DONE);
										}
										oldval=sv[akttone];
										aktval=((double)((ye-1)-mousey)/(float)RunTime->scy[50])-1.0;
										if(aktval>1.0) aktval=1.0;
										if(aktval<-1.0) aktval=-1.0;
										if(aktval!=oldval) {
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											sv[akttone]=aktval;
											DrawTone(instance,modrp,akttone,xs+1,ys+1,xe-1,ye-1);
											GT_SetGadgetAttrs(modGadgets[GADIX_VAL],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
											DrawWave(instance,modrp,xsg+1,ysg+1,xeg-1,yeg-1);
										}
									}
								}
								if(omptr!=MPTR_SIZE82) { SetWindowPointer(modWin,WA_Pointer,RunTime->MPtrSize82->Pointer,TAG_DONE);omptr=MPTR_SIZE82; }
							}
							else {
								if(omptr!=MPTR_STANDARD) { SetWindowPointer(modWin,TAG_DONE);omptr=MPTR_STANDARD; }
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
	return(ret);
	Error:
	return(FALSE);
}

UBYTE SAVEDS ASM LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	//-- loop
	//UBYTE l;
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

	UC_ConvertParameter(param,instance->mod1.vals,UGRP_RELFREQUENCY,RFRQ_FC);
	instance->frqs=atof(param);
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_RELFREQUENCY,RFRQ_FC);
	instance->frqe=atof(param);
	instance->frqd=instance->frqe-instance->frqs;

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_AMPLITUDE,AMP_FC);
	instance->amps=atof(param);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_AMPLITUDE,AMP_FC);
	instance->ampe=atof(param);
	instance->ampd=instance->ampe-instance->amps;

	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
	if(instance->slen<2) instance->slen=2;
	UC_ConvertParameter(param,instance->p_pitch,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->pitch=atof(param);

	UC_ConvertParameter(param,instance->p_phase,UGRP_PHASE,PHA_RAD);
	instance->phase=atof(param);

	UC_ConvertParameter(param,instance->scale,UGRP_AMPLITUDE,AMP_FC);
	instance->amp=atof(param)*32767.0;

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
					switch(instance->oszi) {
						case WAVE_SIN:
							for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && (instance->curlen<instance->slen));dstsa->seg++) {
								instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
								process_sin((SFXCoreInstance *)instance,blen);
								SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
							}
							break;
						case WAVE_TRI:
							for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && (instance->curlen<instance->slen));dstsa->seg++) {
								instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
								process_tri((SFXCoreInstance *)instance,blen);
								SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
							}
							break;
						case WAVE_SAW:
							for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && (instance->curlen<instance->slen));dstsa->seg++) {
								instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
								process_saw((SFXCoreInstance *)instance,blen);
								SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
							}
							break;
						case WAVE_SQR:
							for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && (instance->curlen<instance->slen));dstsa->seg++) {
								instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
								process_sqr((SFXCoreInstance *)instance,blen);
								SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
							}
							break;
					}
					SADone(dstsa,dstbuf);
				}
				else okay=FALSE;
				deactivate_ch((SFXCoreInstance *)instance,0);
				if(ProWinAborted(instance->curlen)) { okay=FALSE; }										  // es wurde abgebrochen
			//}
			if(okay && newdst) AddSample(dstbuf,"SynthAdd");
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
		if(!pf && !stricmp(RexxPar2,"SVal"))		{ instance->svals[atoi(RexxPar3)]=(double)(atof(RexxPar4));pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Oszilator"))	{ instance->oszi	=CfgIO_MatchString(RexxPar3,OsziLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Phase"))		{ strntcpy(instance->p_phase,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(instance->p_slen ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Rate"))		{ strntcpy(instance->p_srat ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Pitch"))		{ strntcpy(instance->p_pitch,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Scale"))		{ strntcpy(instance->scale,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet  (&(instance->mod1),"P1",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod2),"P2",params,ret);
		if(!pf && !stricmp(RexxPar2,"SVal"))		{ sprintf(ret,"%lf",instance->svals[atoi(RexxPar3)]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Oszilator"))	{ strcpy(ret,OsziLabels[instance->oszi]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Phase"))		{ strntcpy(ret,instance->p_phase,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strntcpy(ret,instance->p_slen ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Rate"))		{ strntcpy(ret,instance->p_srat ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Pitch"))		{ strntcpy(ret,instance->p_pitch,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Scale"))		{ strntcpy(ret,instance->scale,PARBUFLEN); }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	register UBYTE i;
	char strItemName[10];
	Param param;

	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	for(i=0;i<64;i++) {
		sprintf(strItemName,"SVal%d",i);
		instance->svals[i]=atof(CfgIO_ReadString		(cfg,"General"	,strItemName,"0.0"));
	}
	instance->oszi=				CfgIO_MatchString(
								CfgIO_ReadString	(cfg,"General"	,"Oszilator"	,OsziLabels[instance->oszi]),OsziLabels);
	strcpy(instance->p_phase,	CfgIO_ReadString	(cfg,"General"	,"Phase"		,instance->p_phase));
	strcpy(instance->p_slen,	CfgIO_ReadString	(cfg,"General"	,"Length"		,instance->p_slen));
	strcpy(instance->p_srat,	CfgIO_ReadString	(cfg,"General"	,"Rate"			,instance->p_srat));
	strcpy(instance->p_pitch,	CfgIO_ReadString	(cfg,"General"	,"Pitch"		,instance->p_pitch));
	strcpy(instance->scale,		CfgIO_ReadString	(cfg,"General"	,"Scale"		,instance->scale));

	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
	if(instance->slen<2) instance->slen=2;
}

void SaveSettings(struct Instance *instance,char *fname) {
	register UBYTE i;
	char strItemName[10];

	SaveModSettings(cfg,instance->moddesc);
	for(i=0;i<64;i++) {
		sprintf(strItemName,"SVal%d",i);
		CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->svals[i]);
	}
	CfgIO_WriteString		  (cfg,"General"	,"Oszilator",OsziLabels[instance->oszi]);
	CfgIO_WriteString		  (cfg,"General"	,"Phase"	,instance->p_phase);
	CfgIO_WriteString		  (cfg,"General"	,"Length"	,instance->p_slen);
	CfgIO_WriteString		  (cfg,"General"	,"Rate"		,instance->p_srat);
	CfgIO_WriteString		  (cfg,"General"	,"Pitch"	,instance->p_pitch);
	CfgIO_WriteString		  (cfg,"General"	,"Scale"	,instance->scale);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	register UBYTE i;
	Param param;
	
	UC_ConvertParameter(instance->mod1.vals,"1.0",UGRP_RELFREQUENCY,RFRQ_FC);
	UC_ConvertParameter(instance->mod1.vale,"1.0",UGRP_RELFREQUENCY,RFRQ_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"1.0",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->mod2.vale,"1.0",UGRP_AMPLITUDE,AMP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	UC_ConvertParameter(instance->p_slen ,"1 s",UGRP_ABSTIME,ATME_S);
	UC_ConvertParameter(instance->p_srat ,"22050 hz",UGRP_ABSFREQUENCY,AFRQ_HZ);
	UC_ConvertParameter(instance->p_pitch,"261.6255653 hz",UGRP_ABSFREQUENCY,AFRQ_HZ);
	UC_ConvertParameter(instance->scale  ,"1.0",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->p_phase,"0.0 °",UGRP_PHASE,PHA_D);

	instance->svals[0]=1.0;
	for(i=1;i<64;i++) instance->svals[i]=0.0;
	instance->oszi=WAVE_SIN;
	instance->phdist=0.5;

	UC_ConvertParameter(param,instance->p_srat,UGRP_ABSFREQUENCY,AFRQ_HZ);
	instance->srat=atof(param);
	if(UC_GetParameterUnit(instance->p_slen,UGRP_ABSTIME)==ATME_SAMPLES) instance->slen=atof(instance->p_slen);
	else { UC_ConvertParameter(param,instance->p_slen,UGRP_ABSTIME,ATME_MS);instance->slen=atof(param)*((double)instance->srat/1000.0); }
	if(instance->slen<2) instance->slen=2;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	double *sv=instance->svals;
	
	SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
	akttone=0;
	GT_SetGadgetAttrs(modGadgets[GADIX_NR   ],modWin,0l,GTIN_Number,akttone,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_VAL  ],modWin,0l,GTIN_Number,(int)(sv[akttone]*100.0),TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_VOL  ],modWin,0l,GTST_String,instance->scale,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_WAVE ],modWin,0l,GTCY_Active,instance->oszi,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_PHASE],modWin,0l,GTST_String,instance->p_phase,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_FRQ  ],modWin,0l,GTST_String,instance->p_pitch,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_SLEN ],modWin,0l,GTST_String,instance->p_slen,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_RATE ],modWin,0l,GTST_String,instance->p_srat,TAG_DONE);

	{
		/** @todo this is a very ugly hack, the slider and the wave fields should realy be a custom boopsi gadget */
		UWORD ys1=2;
		UWORD xs1=2;

		UWORD xsg=xs1+RunTime->scx[       6],ysg=ys1+RunTime->scy[FRMCONTENT_YOFFSET]+1;	/* wave-preview */
		UWORD xlg=2  +RunTime->scx[GFX_XRES],ylg=2+  RunTime->scy[GFX_YRES];
		UWORD xeg=xsg+xlg,yeg=ysg+ylg;

		UWORD xs =xs1+RunTime->scx[176],ys =ys1+RunTime->scy[FRMCONTENT_YOFFSET];	/* obertones */
		UWORD xl =2  +RunTime->scx[384],yl =2  +RunTime->scy[100];
		UWORD xe =xs +xl ,ye =ys +yl;

		DrawTones(instance,modWin->RPort,xs+1,ys+1,xe-1,ye-1);
		DrawWave(instance,modWin->RPort,xsg+1,ysg+1,xeg-1,yeg-1);
	}
}

//-- private Stuff ------------------------------------------------------------

void DrawWave(struct Instance *instance,struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye) {
	register UWORD x;
	register UBYTE j;
	double angle,anglediff,scale,an;
	LONG val1,oneper;
	UBYTE first=1;
	double *sv;
	double si0,si1,fc;
	double val2;
	Param param;

	SetAPen(rp,RunTime->sfxprefs_gui->Pens[PEN_SMP_BACK]);
	RectFill(rp,xs,ys,xe,ye);
	SetAPen(rp,RunTime->sfxprefs_gui->Pens[PEN_SMP_LINE]);
	oneper=RunTime->scx[GFX_XRES];
	UC_ConvertParameter(param,instance->p_phase,UGRP_PHASE,PHA_RAD);
	angle=atof(param);
	anglediff=PIM2/(double)oneper;
	UC_ConvertParameter(param,instance->scale,UGRP_AMPLITUDE,AMP_FC);
	scale=atof(param)*32.0;
	switch(instance->oszi) {
		case WAVE_SIN:
			for(x=xs;x<=xe;x++) {
				sv=instance->svals;
				val2=0.0;
				si0=sin(-angle);
				si1=0.0;
				fc=cos(angle);fc+=fc;
				for(j=0;j<64;) {
					si0=fc*si1-si0;
					if(*sv!=0.0) val2+=(scale*si0*(*sv));
					j++;sv++;
					si1=fc*si0-si1;
					if(*sv!=0.0) val2+=(scale*si1*(*sv));
					j++;sv++;
					//if(sv[j]!=0.0) val2+=(scale*(sin(an)*sv[j]));
					//an+=angle;
				}
				val1=(LONG)val2;
				if(val1>32) val1=32;
				if(val1<-32) val1=-32;
				if(!first) Draw(rp,x,ye-RunTime->scy[32+val1]);
				else { Move(rp,x,ye-RunTime->scy[32+val1]);first=0; }
				angle+=anglediff;
				//if(angle>PIM2) angle-=PIM2;
			}
			break;
		case WAVE_TRI:
			for(x=xs;x<=xe;x++) {
				sv=instance->svals;
				val2=0.0;an=angle;
				for(j=0;j<64;j++,sv++) {
					if(*sv!=0.0) val2+=(scale*(sfxtri(an)*(*sv)));
					an+=angle;
				}
				val1=(LONG)val2;
				if(val1>32) val1=32;
				if(val1<-32) val1=-32;
				if(!first) Draw(rp,x,ye-RunTime->scy[32+val1]);
				else { Move(rp,x,ye-RunTime->scy[32+val1]);first=0; }
				angle+=anglediff;
				//if(angle>PIM2) angle-=PIM2;
			}
			break;
		case WAVE_SAW:				/* Saw */
			for(x=xs;x<=xe;x++) {
				sv=instance->svals;
				val2=0.0;an=angle;
				for(j=0;j<64;j++,sv++) {
					if(*sv!=0.0) val2+=(LONG)(scale*(sfxsaw(an)*(*sv)));
					an+=angle;
				}
				val1=(LONG)val2;
				if(val1>32) val1=32;
				if(val1<-32) val1=-32;
				if(!first) Draw(rp,x,ye-RunTime->scy[32+val1]);
				else { Move(rp,x,ye-RunTime->scy[32+val1]);first=0; }
				angle+=anglediff;
				//if(angle>PIM2) angle-=PIM2;
			}
			break;
		case WAVE_SQR:				/* Sqr */
			for(x=xs;x<=xe;x++) {
				sv=instance->svals;
				val2=0.0;an=angle;
				for(j=0;j<64;j++,sv++) {
					if(*sv!=0.0) val2+=(scale*(sfxsqr(an)*(*sv)));
					an+=angle;
				}
				val1=(LONG)val2;
				if(val1>32) val1=32;
				if(val1<-32) val1=-32;
				if(!first) Draw(rp,x,ye-RunTime->scy[32+val1]);
				else { Move(rp,x,ye-RunTime->scy[32+val1]);first=0; }
				angle+=anglediff;
				//if(angle>PIM2) angle-=PIM2;
			}
			break;
	}
	SetAPen(rp,RunTime->sfxprefs_gui->Pens[PEN_SMP_GRID]);
	Move(rp,xs,ye-RunTime->scy[GFX_YRES>>1]);Draw(rp,xe,ye-RunTime->scy[GFX_YRES>>1]);
	Move(rp,xe-RunTime->scx[GFX_XRES>>1],ys);Draw(rp,xe-RunTime->scx[GFX_XRES>>1],ye);
}

void DrawTones(struct Instance *instance,struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye) {
	register UWORD j,y;
	double *sv=instance->svals;
	UBYTE col;

	SetAPen(rp,5);RectFill(rp,xs,ys,xe,ye);	/* Löschen */

	SetAPen(rp,0);									/* Balken */
	for(j=0;j<64;j+=2) RectFill(rp,xs+RunTime->scx[(j*6)],ys,xs+RunTime->scx[5+(j*6)],ye);

	SetAPen(rp,6);									/* vert. Linien */
	Move(rp,xs,ye-RunTime->scy[50]);Draw(rp,xe,ye-RunTime->scy[50]);
	SetDrPt(rp,43690);
	Move(rp,xs,ye-RunTime->scy[25]);Draw(rp,xe,ye-RunTime->scy[25]);
	Move(rp,xs,ye-RunTime->scy[75]);Draw(rp,xe,ye-RunTime->scy[75]);

	for(j=0;j<64;j+=10) {								/* horiz. Linien */
		Move(rp,xs+RunTime->scx[2+(j*6)],ys);
		Draw(rp,xs+RunTime->scx[2+(j*6)],ye);
	}
	SetDrPt(rp,65535);

	SetAPen(rp,RunTime->sfxprefs_gui->Pens[PEN_SMP_LOOP]);	/* Regler */
	SetDrMd(rp,COMPLEMENT);
	for(j=0;j<64;j++) {
		y=(UWORD)(50+(WORD)(sv[j]*50.0));
		col=RunTime->sfxprefs_gui->Pens[PEN_SMP_MARK]^(UBYTE)((j&1)?5:0);
		SetAPen(rp,col);SetWriteMask(rp,col);
		Move(rp,xs+RunTime->scx[(j*6)],ye-RunTime->scy[y]);Draw(rp,xs+RunTime->scx[5+(j*6)],ye-RunTime->scy[y]);
	}
	SetWriteMask(rp,0xFF);SetDrMd(rp,JAM2);
}

void DrawTone(struct Instance *instance,struct RastPort *rp,UBYTE akttone,UWORD xs,UWORD ys,UWORD xe,UWORD ye) {
	register UWORD j,y;
	UBYTE col=RunTime->sfxprefs_gui->Pens[PEN_SMP_MARK]^(UBYTE)((akttone&1)?5:0);

	SetAPen(rp,col);SetWriteMask(rp,col);SetDrMd(rp,COMPLEMENT);
	j=(UWORD)akttone;
	y=(UWORD)(50+(WORD)(instance->svals[j]*50.0));j*=6;
	Move(rp,xs+RunTime->scx[j],ye-RunTime->scy[y]);Draw(rp,xs+RunTime->scx[5+j],ye-RunTime->scy[y]);
	SetWriteMask(rp,0xFF);SetDrMd(rp,JAM2);
}

//-- eof ----------------------------------------------------------------------
