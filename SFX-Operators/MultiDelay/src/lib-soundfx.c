/****h* MultiDelay/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (19.07.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    MultiDelay Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    22.Oct.1998
*  MODIFICATION HISTORY
*    19.Jul.2002	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    22.Oct.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define MULTIDELAY_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "MultiDelay.h"

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

#define GADID_DEL1		(GADID_SRC+SRC_IDCT)
#define GADIX_DEL1		(GADIX_SRC+SRC_IXCT)

#define GADID_FBL1		(GADID_DEL1+1)
#define GADIX_FBL1		(GADIX_DEL1+1)

#define GADID_AMP1		(GADID_FBL1+1)
#define GADIX_AMP1		(GADIX_FBL1+1)

#define GADID_FBG1		(GADID_AMP1+1)
#define GADIX_FBG1		(GADIX_AMP1+1)

#define GADID_DEL2		(GADID_FBG1+1)
#define GADIX_DEL2		(GADIX_FBG1+1)

#define GADID_FBL2		(GADID_DEL2+1)
#define GADIX_FBL2		(GADIX_DEL2+1)

#define GADID_AMP2		(GADID_FBL2+1)
#define GADIX_AMP2		(GADIX_FBL2+1)

#define GADID_FBG2		(GADID_AMP2+1)
#define GADIX_FBG2		(GADIX_AMP2+1)

#define GADID_DEL3		(GADID_FBG2+1)
#define GADIX_DEL3		(GADIX_FBG2+1)

#define GADID_FBL3		(GADID_DEL3+1)
#define GADIX_FBL3		(GADIX_DEL3+1)

#define GADID_AMP3		(GADID_FBL3+1)
#define GADIX_AMP3		(GADIX_FBL3+1)

#define GADID_FBG3		(GADID_AMP3+1)
#define GADIX_FBG3		(GADIX_AMP3+1)

#define GADID_DEL4		(GADID_FBG3+1)
#define GADIX_DEL4		(GADIX_FBG3+1)

#define GADID_FBL4		(GADID_DEL4+1)
#define GADIX_FBL4		(GADIX_DEL4+1)

#define GADID_AMP4		(GADID_FBL4+1)
#define GADIX_AMP4		(GADIX_FBL4+1)

#define GADID_FBG4		(GADID_AMP4+1)
#define GADIX_FBG4		(GADIX_AMP4+1)

#define GADID_DEL5		(GADID_FBG4+1)
#define GADIX_DEL5		(GADIX_FBG4+1)

#define GADID_FBL5		(GADID_DEL5+1)
#define GADIX_FBL5		(GADIX_DEL5+1)

#define GADID_AMP5		(GADID_FBL5+1)
#define GADIX_AMP5		(GADIX_FBL5+1)

#define GADID_FBG5		(GADID_AMP5+1)
#define GADIX_FBG5		(GADIX_AMP5+1)

#define GADID_DEL6		(GADID_FBG5+1)
#define GADIX_DEL6		(GADIX_FBG5+1)

#define GADID_FBL6		(GADID_DEL6+1)
#define GADIX_FBL6		(GADIX_DEL6+1)

#define GADID_AMP6		(GADID_FBL6+1)
#define GADIX_AMP6		(GADIX_FBL6+1)

#define GADID_FBG6		(GADID_AMP6+1)
#define GADIX_FBG6		(GADIX_AMP6+1)

#define GADID_DEL7		(GADID_FBG6+1)
#define GADIX_DEL7		(GADIX_FBG6+1)

#define GADID_FBL7		(GADID_DEL7+1)
#define GADIX_FBL7		(GADIX_DEL7+1)

#define GADID_AMP7		(GADID_FBL7+1)
#define GADIX_AMP7		(GADIX_FBL7+1)

#define GADID_FBG7		(GADID_AMP7+1)
#define GADIX_FBG7		(GADIX_AMP7+1)

#define GADID_DEL8		(GADID_FBG7+1)
#define GADIX_DEL8		(GADIX_FBG7+1)

#define GADID_FBL8		(GADID_DEL8+1)
#define GADIX_FBL8		(GADIX_DEL8+1)

#define GADID_AMP8		(GADID_FBL8+1)
#define GADIX_AMP8		(GADIX_FBL8+1)

#define GADID_FBG8		(GADID_AMP8+1)
#define GADIX_FBG8		(GADIX_AMP8+1)

#define GADID_AMPF		(GADID_FBG8+1)
#define GADIX_AMPF		(GADIX_FBG8+1)

#define GADID_DRY		(GADID_AMPF+1)
#define GADIX_DRY		(GADIX_AMPF+1)

#define GADID_NR		(GADID_DRY+1)
#define GADIX_NR		(GADIX_DRY+1)

#define LAST_GADIX		(GADIX_NR+1)

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
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(2);
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(2);
	UWORD ys5=ys4+1+ANYFRM_HEIGHT(2);
	UWORD ys6=ys5+1+ANYFRM_HEIGHT(2);
	UWORD ys7=ys6+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys7+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD xs3=xs2+1+STDFRM_WIDTH;
	UWORD winw=xs3+3+CFGSEL_WIDTH;
	UWORD xh,yh;

	register UBYTE k;
	LONG val1;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[258];
		ng.ng_TopEdge		=ys7+RunTime->scy[2];
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

		g=AddSource(&(instance->src),modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		ng.ng_LeftEdge		=xs1+RunTime->scx[62];				/* Delay1 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Delay";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_DEL1;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[0].del,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DEL1]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Local";
		ng.ng_GadgetID		=GADID_FBL1;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[0].fbl,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBL1]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[62];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Volume";
		ng.ng_GadgetID		=GADID_AMP1;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[0].amp,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMP1]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Global";
		ng.ng_GadgetID		=GADID_FBG1;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[0].fbg,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBG1]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[62];				/* Delay2 */
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Delay";
		ng.ng_GadgetID		=GADID_DEL2;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[1].del,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DEL2]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Local";
		ng.ng_GadgetID		=GADID_FBL2;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[1].fbl,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBL2]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[62];
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Volume";
		ng.ng_GadgetID		=GADID_AMP2;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[1].amp,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMP2]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Global";
		ng.ng_GadgetID		=GADID_FBG2;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[1].fbg,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBG2]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[62];				/* Delay3 */
		ng.ng_TopEdge		=ys4+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Delay";
		ng.ng_GadgetID		=GADID_DEL3;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[2].del,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DEL3]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Local";
		ng.ng_GadgetID		=GADID_FBL3;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[2].fbl,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBL3]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[62];
		ng.ng_TopEdge		=ys4+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Volume";
		ng.ng_GadgetID		=GADID_AMP3;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[2].amp,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMP3]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Global";
		ng.ng_GadgetID		=GADID_FBG3;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[2].fbg,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBG3]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[62];				/* Delay4 */
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Delay";
		ng.ng_GadgetID		=GADID_DEL4;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[3].del,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DEL4]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Local";
		ng.ng_GadgetID		=GADID_FBL4;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[3].fbl,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBL4]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[62];
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Volume";
		ng.ng_GadgetID		=GADID_AMP4;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[3].amp,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMP4]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Global";
		ng.ng_GadgetID		=GADID_FBG4;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[3].fbg,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBG4]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[62];				/* Delay5 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Delay";
		ng.ng_GadgetID		=GADID_DEL5;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[4].del,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DEL5]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Local";
		ng.ng_GadgetID		=GADID_FBL5;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[4].fbl,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBL5]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[62];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Volume";
		ng.ng_GadgetID		=GADID_AMP5;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[4].amp,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMP5]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Global";
		ng.ng_GadgetID		=GADID_FBG5;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[4].fbg,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBG5]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[62];				/* Delay6 */
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Delay";
		ng.ng_GadgetID		=GADID_DEL6;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[5].del,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DEL6]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Local";
		ng.ng_GadgetID		=GADID_FBL6;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[5].fbl,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBL6]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[62];
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Volume";
		ng.ng_GadgetID		=GADID_AMP6;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[5].amp,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMP6]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Global";
		ng.ng_GadgetID		=GADID_FBG6;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[5].fbg,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBG6]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[62];				/* Delay7 */
		ng.ng_TopEdge		=ys4+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Delay";
		ng.ng_GadgetID		=GADID_DEL7;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[6].del,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DEL7]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Local";
		ng.ng_GadgetID		=GADID_FBL7;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[6].fbl,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBL7]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[62];
		ng.ng_TopEdge		=ys4+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Volume";
		ng.ng_GadgetID		=GADID_AMP7;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[6].amp,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMP7]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Global";
		ng.ng_GadgetID		=GADID_FBG7;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[6].fbg,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBG7]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[62];				/* Delay8 */
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Delay";
		ng.ng_GadgetID		=GADID_DEL8;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[7].del,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DEL8]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Local";
		ng.ng_GadgetID		=GADID_FBL8;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[7].fbl,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBL8]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[62];
		ng.ng_TopEdge		=ys5+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Volume";
		ng.ng_GadgetID		=GADID_AMP8;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[7].amp,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMP8]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[215];
		ng.ng_GadgetText	="Fb Global";
		ng.ng_GadgetID		=GADID_FBG8;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->delays[7].fbg,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_FBG8]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[62];				/* Other */
		ng.ng_TopEdge		=ys6+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="Amp";
		ng.ng_GadgetID		=GADID_AMPF;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_ampf,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_AMPF]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_GadgetText	="Dry";
		ng.ng_GadgetID		=GADID_DRY;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->p_dry,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_DRY]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[62];
		ng.ng_GadgetText	="Num";
		ng.ng_GadgetID		=GADID_NR;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,2,GTIN_Number,instance->anz,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_NR]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			xh=STDFRM_WIDTH;
			DrawTitle(modrp,"Delay 1"		,xs1,ys2,xh);
			DrawTitle(modrp,"Delay 2"		,xs1,ys3,xh);
			DrawTitle(modrp,"Delay 3"		,xs1,ys4,xh);
			DrawTitle(modrp,"Delay 4"		,xs1,ys5,xh);
			DrawTitle(modrp,"Delay 5"		,xs2,ys2,xh);
			DrawTitle(modrp,"Delay 6"		,xs2,ys3,xh);
			DrawTitle(modrp,"Delay 7"		,xs2,ys4,xh);
			DrawTitle(modrp,"Delay 8"		,xs2,ys5,xh);
			xh=xh+xh+1;
			DrawTitle(modrp,"Source"		,xs1,ys1,xh);
			DrawTitle(modrp,"Miscellaneous"	,xs1,ys6,xh);
			DrawTitle(modrp,"Presets"		,xs3,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;yh=ANYFRM_HEIGHT(2);
			DrawGadWB(modrp,xs1,ys2,xh,yh);							/* Delay1 */
			DrawGadWB(modrp,xs1,ys3,xh,yh);							/* Delay2 */
			DrawGadWB(modrp,xs1,ys4,xh,yh);							/* Delay3 */
			DrawGadWB(modrp,xs1,ys5,xh,yh);							/* Delay4 */
			DrawGadWB(modrp,xs2,ys2,xh,yh);							/* Delay5 */
			DrawGadWB(modrp,xs2,ys3,xh,yh);							/* Delay6 */
			DrawGadWB(modrp,xs2,ys4,xh,yh);							/* Delay7 */
			DrawGadWB(modrp,xs2,ys5,xh,yh);							/* Delay8 */
			xh=1+xh+xh;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);				/* Source */
			DrawGadWB(modrp,xs1,ys6,xh,ANYFRM_HEIGHT(1));			/* Par */
			DrawGadWB(modrp,xs1,ys7,xh,CMDFRM_HEIGHT);				/* Go */
			DrawGadWB(modrp,xs3,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);	/* CfgSel */

			SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);

			if(!enabled) GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);

			for(k=0;k<MAX_DELAYS;k++) {
				if(k>=instance->anz) {
					GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_DEL1],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBL1],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_AMP1],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBG1],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
				}
			}

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
								&& !HandleCfgSel   (&cfgsel      ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_DEL1: HandleParameter(instance->delays[0].del,modWin,modGadgets[GADIX_DEL1]);break;
										case GADID_FBL1: HandleParameter(instance->delays[0].fbl,modWin,modGadgets[GADIX_FBL1]);break;
										case GADID_AMP1: HandleParameter(instance->delays[0].amp,modWin,modGadgets[GADIX_AMP1]);break;
										case GADID_FBG1: HandleParameter(instance->delays[0].fbg,modWin,modGadgets[GADIX_FBG1]);break;
										case GADID_DEL2: HandleParameter(instance->delays[1].del,modWin,modGadgets[GADIX_DEL2]);break;
										case GADID_FBL2: HandleParameter(instance->delays[1].fbl,modWin,modGadgets[GADIX_FBL2]);break;
										case GADID_AMP2: HandleParameter(instance->delays[1].amp,modWin,modGadgets[GADIX_AMP2]);break;
										case GADID_FBG2: HandleParameter(instance->delays[1].fbg,modWin,modGadgets[GADIX_FBG2]);break;
										case GADID_DEL3: HandleParameter(instance->delays[2].del,modWin,modGadgets[GADIX_DEL3]);break;
										case GADID_FBL3: HandleParameter(instance->delays[2].fbl,modWin,modGadgets[GADIX_FBL3]);break;
										case GADID_AMP3: HandleParameter(instance->delays[2].amp,modWin,modGadgets[GADIX_AMP3]);break;
										case GADID_FBG3: HandleParameter(instance->delays[2].fbg,modWin,modGadgets[GADIX_FBG3]);break;
										case GADID_DEL4: HandleParameter(instance->delays[3].del,modWin,modGadgets[GADIX_DEL4]);break;
										case GADID_FBL4: HandleParameter(instance->delays[3].fbl,modWin,modGadgets[GADIX_FBL4]);break;
										case GADID_AMP4: HandleParameter(instance->delays[3].amp,modWin,modGadgets[GADIX_AMP4]);break;
										case GADID_FBG4: HandleParameter(instance->delays[3].fbg,modWin,modGadgets[GADIX_FBG4]);break;
										case GADID_DEL5: HandleParameter(instance->delays[4].del,modWin,modGadgets[GADIX_DEL5]);break;
										case GADID_FBL5: HandleParameter(instance->delays[4].fbl,modWin,modGadgets[GADIX_FBL5]);break;
										case GADID_AMP5: HandleParameter(instance->delays[4].amp,modWin,modGadgets[GADIX_AMP5]);break;
										case GADID_FBG5: HandleParameter(instance->delays[4].fbg,modWin,modGadgets[GADIX_FBG5]);break;
										case GADID_DEL6: HandleParameter(instance->delays[5].del,modWin,modGadgets[GADIX_DEL6]);break;
										case GADID_FBL6: HandleParameter(instance->delays[5].fbl,modWin,modGadgets[GADIX_FBL6]);break;
										case GADID_AMP6: HandleParameter(instance->delays[5].amp,modWin,modGadgets[GADIX_AMP6]);break;
										case GADID_FBG6: HandleParameter(instance->delays[5].fbg,modWin,modGadgets[GADIX_FBG6]);break;
										case GADID_DEL7: HandleParameter(instance->delays[6].del,modWin,modGadgets[GADIX_DEL7]);break;
										case GADID_FBL7: HandleParameter(instance->delays[6].fbl,modWin,modGadgets[GADIX_FBL7]);break;
										case GADID_AMP7: HandleParameter(instance->delays[6].amp,modWin,modGadgets[GADIX_AMP7]);break;
										case GADID_FBG7: HandleParameter(instance->delays[6].fbg,modWin,modGadgets[GADIX_FBG7]);break;
										case GADID_DEL8: HandleParameter(instance->delays[7].del,modWin,modGadgets[GADIX_DEL8]);break;
										case GADID_FBL8: HandleParameter(instance->delays[7].fbl,modWin,modGadgets[GADIX_FBL8]);break;
										case GADID_AMP8: HandleParameter(instance->delays[7].amp,modWin,modGadgets[GADIX_AMP8]);break;
										case GADID_FBG8: HandleParameter(instance->delays[7].fbg,modWin,modGadgets[GADIX_FBG8]);break;
										case GADID_AMPF: HandleParameter(instance->p_ampf,modWin,modGadgets[GADIX_AMPF]);break;
										case GADID_DRY:  HandleParameter(instance->p_dry ,modWin,modGadgets[GADIX_DRY ]);break;
										case GADID_NR:
											CheckGad_int(modGadgets[GADIX_NR],modWin,&val1,1,1,MAX_DELAYS);instance->anz=val1;
											for(k=0;k<MAX_DELAYS;k++) {
												if(k>=instance->anz) {
													GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_DEL1],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
													GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBL1],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
													GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_AMP1],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
													GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBG1],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
												}
												else {
													GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_DEL1],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
													GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBL1],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
													GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_AMP1],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
													GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBG1],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
												}
											}
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADID_DEL1],modWin,0l);
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
	UBYTE k;
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

	UC_ConvertParameter(param,instance->p_ampf,UGRP_AMPLITUDE,AMP_FC);
	instance->ampf=atof(param)/(instance->anz+1);								// because we amplify each delay
	UC_ConvertParameter(param,instance->p_dry ,UGRP_PROPOTION,PROP_FC);
	instance->dry =atof(param);

	if(instance->anz>MAX_DELAYS) instance->anz=MAX_DELAYS;

	for(k=0;k<instance->anz;k++) {
		if(UC_GetParameterUnit(instance->delays[k].del,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
			UC_ConvertParameter(param,instance->delays[k].del,UGRP_ABSFREQUENCY,AFRQ_HZ);
			if((del=atof(param))<1.0) del=1.0;
			instance->delay[k]=(ULONG)((double)srcbuf->srat/del);
		}
		else {
			if((pu=UC_GetParameterUnit(instance->delays[k].del,UGRP_ABSTIME))!=INVALID_UNIT) {
				if(pu==ATME_SAMPLES) instance->delay[k]=atof(instance->delays[k].del);
				else {
					UC_ConvertParameter(param,instance->delays[k].del,UGRP_ABSTIME,ATME_MS);
					instance->delay[k]=(ULONG)(atof(param)*((double)srcbuf->srat/1000.0));
				}
			}
			else {
				UC_ConvertParameter(param,instance->delays[k].del,UGRP_RELTIME,RTME_FC);
				instance->delay[k]=(ULONG)((double)srcbuf->slen*atof(param));
			}
		}
		if(instance->delay[k]<1) instance->delay[k]=1;
		
		UC_ConvertParameter(param,instance->delays[k].fbl,UGRP_PROPOTION,PROP_FC);
		if((instance->fbl[k]=atof(param))>=0.0) instance->dr[k]=1.0-instance->fbl[k];
		else instance->dr[k]=1.0+instance->fbl[k];
		UC_ConvertParameter(param,instance->delays[k].fbg,UGRP_PROPOTION,PROP_FC);
		instance->fbg[k]=atof(param);

		UC_ConvertParameter(param,instance->delays[k].amp,UGRP_AMPLITUDE,AMP_FC);
		instance->amp[k]=atof(param)*instance->ampf;
	}

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
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }										  // es wurde abgebrochen
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
	UBYTE cf=FALSE,pf,ix;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"Del"))			{ ix=atoi(RexxPar3)&7;strntcpy(instance->delays[ix].del,RexxPar4,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Fbl"))			{ ix=atoi(RexxPar3)&7;strntcpy(instance->delays[ix].fbl,RexxPar4,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Amp"))			{ ix=atoi(RexxPar3)&7;strntcpy(instance->delays[ix].amp,RexxPar4,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Fbg"))			{ ix=atoi(RexxPar3)&7;strntcpy(instance->delays[ix].fbg,RexxPar4,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strntcpy(instance->p_ampf,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Dry" ))		{ strntcpy(instance->p_dry ,RexxPar3,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Nr" ))			{ instance->anz=atoi(RexxPar3);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"Del"))			{ ix=atoi(RexxPar3)&7;strntcpy(ret,instance->delays[ix].del,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Fbl"))			{ ix=atoi(RexxPar3)&7;strntcpy(ret,instance->delays[ix].fbl,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Amp"))			{ ix=atoi(RexxPar3)&7;strntcpy(ret,instance->delays[ix].amp,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Fbg"))			{ ix=atoi(RexxPar3)&7;strntcpy(ret,instance->delays[ix].fbg,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strntcpy(ret,instance->p_ampf,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Dry" ))		{ strntcpy(ret,instance->p_dry ,PARBUFLEN);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Nr" ))			{ sprintf(ret,"%d",instance->anz);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	register UBYTE k;
	char strItem[10];

	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->p_ampf,CfgIO_ReadString			(cfg,"General"	,"Ampf"	,instance->p_ampf));
	strcpy(instance->p_dry ,CfgIO_ReadString			(cfg,"General"	,"Dry"	,instance->p_ampf));
	instance->anz=		 CfgIO_ReadUnsignedInteger	(cfg,"General"	,"Nr"	,instance->anz);
	if(!instance->anz || instance->anz>MAX_DELAYS) instance->anz=MAX_DELAYS;
	for(k=0;k<instance->anz;k++) {
		sprintf(strItem,"Del%d",k);strcpy(instance->delays[k].del,CfgIO_ReadString			(cfg,"General"	,strItem	,instance->delays[k].del));
		sprintf(strItem,"Fbl%d",k);strcpy(instance->delays[k].fbl,CfgIO_ReadString			(cfg,"General"	,strItem	,instance->delays[k].fbl));
		sprintf(strItem,"Amp%d",k);strcpy(instance->delays[k].amp,CfgIO_ReadString			(cfg,"General"	,strItem	,instance->delays[k].amp));
		sprintf(strItem,"Fbg%d",k);strcpy(instance->delays[k].fbg,CfgIO_ReadString			(cfg,"General"	,strItem	,instance->delays[k].fbg));
	}
}

void SaveSettings(struct Instance *instance,char *fname) {
	register UBYTE k;
	char strItem[10];

	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Ampf"		,instance->p_ampf);
	CfgIO_WriteString		  (cfg,"General"	,"Dry"		,instance->p_dry);
	CfgIO_WriteUnsignedInteger(cfg,"General"	,"Nr"		,instance->anz);
	for(k=0;k<instance->anz;k++) {
		sprintf(strItem,"Del%d",k);CfgIO_WriteString			(cfg,"General"	,strItem	,instance->delays[k].del);
		sprintf(strItem,"Fbl%d",k);CfgIO_WriteString			(cfg,"General"	,strItem	,instance->delays[k].fbl);
		sprintf(strItem,"Amp%d",k);CfgIO_WriteString			(cfg,"General"	,strItem	,instance->delays[k].amp);
		sprintf(strItem,"Fbg%d",k);CfgIO_WriteString			(cfg,"General"	,strItem	,instance->delays[k].fbg);
	}

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src.srcbuf=NULL;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->delays[0].del," 30 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->delays[0].fbl,"0.8"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[0].amp,"0.4"   ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->delays[0].fbg,"0.8"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[1].del," 70 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->delays[1].fbl,"0.7"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[1].amp,"0.35"  ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->delays[1].fbg,"0.7"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[2].del,"110 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->delays[2].fbl,"0.6"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[2].amp,"0.3"   ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->delays[2].fbg,"0.6"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[3].del,"155 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->delays[3].fbl,"0.5"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[3].amp,"0.25"  ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->delays[3].fbg,"0.5"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[4].del,"213 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->delays[4].fbl,"0.4"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[4].amp,"0.2"   ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->delays[4].fbg,"0.4"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[5].del,"297 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->delays[5].fbl,"0.3"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[5].amp,"0.15"  ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->delays[5].fbg,"0.3"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[6].del,"342 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->delays[6].fbl,"0.2"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[6].amp,"0.1"   ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->delays[6].fbg,"0.2"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[7].del,"401 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(instance->delays[7].fbl,"0.1"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->delays[7].amp,"0.05"  ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->delays[7].fbg,"0.1"   ,UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->p_ampf ,"1.0"  ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->p_dry  ,"0.8"  ,UGRP_PROPOTION,PROP_FC);
	instance->anz=MAX_DELAYS;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	register UBYTE k;

	SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
	for(k=0;k<MAX_DELAYS;k++) {
		if(k<instance->anz) {
			GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_DEL1],modWin,0l,GTST_String,instance->delays[k].del,GA_Disabled,FALSE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBL1],modWin,0l,GTST_String,instance->delays[k].fbl,GA_Disabled,FALSE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_AMP1],modWin,0l,GTST_String,instance->delays[k].amp,GA_Disabled,FALSE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBG1],modWin,0l,GTST_String,instance->delays[k].fbg,GA_Disabled,FALSE,TAG_DONE);
		}
		else {
			GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_DEL1],modWin,0l,GTST_String,instance->delays[k].del,GA_Disabled,TRUE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBL1],modWin,0l,GTST_String,instance->delays[k].fbl,GA_Disabled,TRUE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_AMP1],modWin,0l,GTST_String,instance->delays[k].amp,GA_Disabled,TRUE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[(k<<2)+GADIX_FBG1],modWin,0l,GTST_String,instance->delays[k].fbg,GA_Disabled,TRUE,TAG_DONE);
		}
	}
	GT_SetGadgetAttrs(modGadgets[GADIX_NR  ],modWin,0l,GTIN_Number,instance->anz ,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_AMPF],modWin,0l,GTST_String,instance->p_ampf,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_DRY ],modWin,0l,GTST_String,instance->p_dry ,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
