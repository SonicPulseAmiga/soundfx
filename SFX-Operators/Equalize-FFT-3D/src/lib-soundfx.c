/****h* Equalize-FFT-3D/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (18.07.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Equalize-FFT-3D Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    18.Jul.2002	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define EQUALIZE_FFT_3D_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Equalize-FFT-3D.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

void DrawWay(struct Instance *instance,struct RastPort *rp,UBYTE view,UBYTE prec,Modulator *mod1,Modulator *mod2,Modulator *mod3,UWORD xs,UWORD ys);
void HandleEQSel(struct Instance *instance,struct FileRequester *modFRequest,struct TagItem *MODFReqtags,UBYTE index,struct Gadget *modGadgets[],struct Window *modWin);
void LoadEqfs(char *fname,double *eqf);

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

#define GADID_EQSEL1	(GADID_SRC+SRC_IDCT)

#define GADID_EQNAME1	(GADID_EQSEL1+1)
#define GADIX_EQNAME1	(GADIX_SRC+SRC_IXCT)

#define GADID_EQSEL2	(GADID_EQNAME1+1)

#define GADID_EQNAME2	(GADID_EQSEL2+1)
#define GADIX_EQNAME2	(GADIX_EQNAME1+1)

#define GADID_EQSEL3	(GADID_EQNAME2+1)

#define GADID_EQNAME3	(GADID_EQSEL3+1)
#define GADIX_EQNAME3	(GADIX_EQNAME2+1)

#define GADID_EQSEL4	(GADID_EQNAME3+1)

#define GADID_EQNAME4	(GADID_EQSEL4+1)
#define GADIX_EQNAME4	(GADIX_EQNAME3+1)

#define GADID_EQSEL5	(GADID_EQNAME4+1)

#define GADID_EQNAME5	(GADID_EQSEL5+1)
#define GADIX_EQNAME5	(GADIX_EQNAME4+1)

#define GADID_EQSEL6	(GADID_EQNAME5+1)

#define GADID_EQNAME6	(GADID_EQSEL6+1)
#define GADIX_EQNAME6	(GADIX_EQNAME5+1)

#define GADID_EQSEL7	(GADID_EQNAME6+1)

#define GADID_EQNAME7	(GADID_EQSEL7+1)
#define GADIX_EQNAME7	(GADIX_EQNAME6+1)

#define GADID_EQSEL8	(GADID_EQNAME7+1)

#define GADID_EQNAME8	(GADID_EQSEL8+1)
#define GADIX_EQNAME8	(GADIX_EQNAME7+1)

#define GADID_MOD1		(GADID_EQNAME8+1)
#define GADIX_MOD1		(GADIX_EQNAME8+1)

#define GADID_MOD2		(GADID_MOD1+MOD_IDCT)
#define GADIX_MOD2		(GADIX_MOD1+MOD_IXCT)

#define GADID_MOD3		(GADID_MOD2+MOD_IDCT)
#define GADIX_MOD3		(GADIX_MOD2+MOD_IXCT)

#define GADID_VIEW		(GADID_MOD3+MOD_IDCT)
#define GADIX_VIEW		(GADIX_MOD3+MOD_IXCT)

#define GADID_PREC		(GADID_VIEW+1)
#define GADIX_PREC		(GADIX_VIEW+1)

#define GADID_WFKT		(GADID_PREC+1)
#define GADIX_WFKT		(GADIX_PREC+1)

#define GADID_BANDS		(GADID_WFKT+WFKT_IDCT)
#define GADIX_BANDS		(GADIX_WFKT+WFKT_IXCT)

#define GADID_STEPS		(GADID_BANDS+1)
#define GADIX_STEPS		(GADIX_BANDS+1)

#define LAST_GADIX		(GADIX_STEPS+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[6];
};

UBYTE *BandLabels[]={ "4","8","16","32","64","128","256","512",0l };
UBYTE *StepLabels[]={ "1","2","4","8","16","32","64","128","256",0l };
UBYTE *ViewLabels[]={ "Front","Back","Right","Left","Top","Bottom",0l };
UBYTE *PrecLabels[]={ "Low","Medium","High",0l };

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
		SFXMod_ModDescInit(4,PD_WINFUNCTION ,"W1",&(instance->wfkt1));
		SFXMod_ModDescInit(5,PD_END         ,0l  ,0l);	
		instance->name[0]=instance->name1;instance->eqf[0]=instance->eqf1;
		instance->name[1]=instance->name2;instance->eqf[1]=instance->eqf2;
		instance->name[2]=instance->name3;instance->eqf[2]=instance->eqf3;
		instance->name[3]=instance->name4;instance->eqf[3]=instance->eqf4;
		instance->name[4]=instance->name5;instance->eqf[4]=instance->eqf5;
		instance->name[5]=instance->name6;instance->eqf[5]=instance->eqf6;
		instance->name[6]=instance->name7;instance->eqf[6]=instance->eqf7;
		instance->name[7]=instance->name8;instance->eqf[7]=instance->eqf8;
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
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(4);
	UWORD ys4=ys3+1+MODSEL_HEIGHT;
	UWORD ys5=ys4+1+MODSEL_HEIGHT;
	UWORD ys6=ys5+1+MODSEL_HEIGHT;
	UWORD ys7=ys6+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys7+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD xs3=xs2+3+STDFRM_WIDTH;
	UWORD winw=xs3+3+CFGSEL_WIDTH;
	UWORD xh,yh;

	UBYTE gh;

	if(RunTime->aktsamples) instance->slen=instance->src.srcbuf->slen;
	else instance->slen=1000;

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

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];			/* Src1 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_EQSEL1;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_Width			=RunTime->scx[235];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_EQNAME1;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,instance->name1,TAG_DONE);
		modGadgets[GADIX_EQNAME1]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];			/* Src2 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_EQSEL2;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_Width			=RunTime->scx[235];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_EQNAME2;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,instance->name2,TAG_DONE);
		modGadgets[GADIX_EQNAME2]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];			/* Src3 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_EQSEL3;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_Width			=RunTime->scx[235];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_EQNAME3;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,instance->name3,TRUE,TAG_DONE);
		modGadgets[GADIX_EQNAME3]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];			/* Src4 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+36];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_EQSEL4;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[53];
		ng.ng_Width			=RunTime->scx[235];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_EQNAME4;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,instance->name4,TRUE,TAG_DONE);
		modGadgets[GADIX_EQNAME4]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[37];			/* Src5 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_EQSEL5;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime->scx[53];
		ng.ng_Width			=RunTime->scx[235];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_EQNAME5;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,instance->name5,TRUE,TAG_DONE);
		modGadgets[GADIX_EQNAME5]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[37];			/* Src6 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_EQSEL6;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime->scx[53];
		ng.ng_Width			=RunTime->scx[235];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_EQNAME6;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,instance->name6,TAG_DONE);
		modGadgets[GADIX_EQNAME6]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[37];			/* Src7 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_EQSEL7;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime->scx[53];
		ng.ng_Width			=RunTime->scx[235];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_EQNAME7;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,instance->name7,TAG_DONE);
		modGadgets[GADIX_EQNAME7]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[37];			/* Src8 */
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+36];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_EQSEL8;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs2+RunTime->scx[53];
		ng.ng_Width			=RunTime->scx[235];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_EQNAME8;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,instance->name8,TAG_DONE);
		modGadgets[GADIX_EQNAME8]=g;

		g=AddModulator(&(instance->mod1),modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys3);
		g=AddModulator(&(instance->mod2),modGadgets,g,GADID_MOD2,GADIX_MOD2,xs1,ys4);
		g=AddModulator(&(instance->mod3),modGadgets,g,GADID_MOD3,GADIX_MOD3,xs1,ys5);

		ng.ng_LeftEdge		=xs2+RunTime->scx[53];
		ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="View";
		ng.ng_GadgetID		=GADID_VIEW;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ViewLabels,GTCY_Active,instance->view,TAG_DONE);
		modGadgets[GADIX_VIEW]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[195];
		ng.ng_GadgetText	="Prec.";
		ng.ng_GadgetID		=GADID_PREC;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,PrecLabels,GTCY_Active,instance->prec,TAG_DONE);
		modGadgets[GADIX_PREC]=g;

		g=AddWinFunction(&(instance->wfkt1),modGadgets,g,GADID_WFKT,GADIX_WFKT,xs1,ys6+RunTime->scy[FRMCONTENT_YOFFSET]);

		ng.ng_LeftEdge		=xs1+RunTime->scx[380];				/* Anzahl Frq.bänder */
		ng.ng_TopEdge		=ys6+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[60];
		ng.ng_GadgetText	="Bands";
		ng.ng_Flags			=PLACETEXT_LEFT;
		ng.ng_GadgetID		=GADID_BANDS;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,BandLabels,GTCY_Active,instance->bands,TAG_DONE);
		modGadgets[GADIX_BANDS]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[495];				/* Schrittweite */
		ng.ng_GadgetText	="Steps";
		ng.ng_GadgetID		=GADID_STEPS;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,StepLabels,GTCY_Active,instance->steps,TAG_DONE);
		modGadgets[GADIX_STEPS]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Src1",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Src2",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Src3",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(3)]);
			RText(modrp,"Src4",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(4)]);
			RText(modrp,"Src5",xs2+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Src6",xs2+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Src7",xs2+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(3)]);
			RText(modrp,"Src8",xs2+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(4)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys4+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys5+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Window",xs1+RunTime->scx[45],ys6+RunTime->scy[FRMTEXT_YPOS(1)]);

			xh=STDFRM_WIDTH;
			DrawTitle(modrp,"X-Axis"			,xs1,ys3,xh);
			DrawTitle(modrp,"Y-Axis"			,xs1,ys4,xh);
			DrawTitle(modrp,"Z-Axis"			,xs1,ys5,xh);
			DrawTitle(modrp,"Path"				,xs2,ys3,xh);
			xh+=1+STDFRM_WIDTH;
			DrawTitle(modrp,"Source"			,xs1,ys1,xh);
			DrawTitle(modrp,"Frequency-Curves"	,xs1,ys2,xh);
			DrawTitle(modrp,"FFT-Parameters"	,xs1,ys6,xh);
			DrawTitle(modrp,"Presets"			,xs3,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;yh=MODSEL_HEIGHT;
			DrawGadWB(modrp,xs1,ys3,xh,yh);								/* X-Achse */
			DrawGadWB(modrp,xs1,ys4,xh,yh);								/* Y-Achse */
			DrawGadWB(modrp,xs1,ys5,xh,yh);								/* Z-Achse */
			DrawGadWB(modrp,xs2,ys3,xh,2+yh+yh+yh);						/* Cube */
			xh+=1+STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Src */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(4));				/* Curves */
			DrawGadWB(modrp,xs1,ys6,xh,ANYFRM_HEIGHT(1));				/* Par */
			DrawGadWB(modrp,xs1,ys7,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs3,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel     (&cfgsel        ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource     (&(instance->src  ),modWin,modGadgets,GADIX_SRC );
			SetModulator  (&(instance->mod1 ),modWin,modGadgets,GADIX_MOD1);
			SetModulator  (&(instance->mod2 ),modWin,modGadgets,GADIX_MOD2);
			SetModulator  (&(instance->mod3 ),modWin,modGadgets,GADIX_MOD3);
			SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);

			if(!enabled) GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);

			xh=xs2+RunTime->scx[37];
			yh=ys3+RunTime->scy[18];
			DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=1;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						instance->src.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src.rmode=SetRngMode(instance->src.srcbuf);
						instance->slen=instance->src.srcbuf->slen;
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
								if(gh=HandleSource(&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)) {
									if(instance->src.srcbuf->slen!=instance->slen) {
										instance->slen=instance->src.srcbuf->slen;
										DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
									}
								}
								if(!gh && (gh=HandleModulator(&(instance->mod1),0l,0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode))) {
									DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
								}
								if(!gh && (gh=HandleModulator(&(instance->mod2),0l,0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode))) {
									DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
								}
								if(!gh && (gh=HandleModulator(&(instance->mod3),0l,0l,modWin,modGadgets,GADID_MOD3,GADIX_MOD3,gnr,icode))) {
									DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
								}
								if(!gh) gh=HandleWinFunction(&(instance->wfkt1),modWin,modGadgets,GADID_WFKT,GADIX_WFKT,gnr,icode);
								if(!gh) gh=HandleCfgSel     (&cfgsel        ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic);
								if(!gh)	{
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_VIEW:
											instance->view=icode;
											DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
											break;
										case GADID_PREC:
											instance->prec=icode;
											DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
											break;
										case GADID_BANDS:
											instance->bands=icode;
											break;
										case GADID_STEPS:
											instance->steps=icode;
											break;
										case GADID_EQSEL1:
											HandleEQSel(instance,modFRequest,MODFReqtags,0,modGadgets,modWin);
											break;
										case GADID_EQSEL2:
											HandleEQSel(instance,modFRequest,MODFReqtags,1,modGadgets,modWin);
											break;
										case GADID_EQSEL3:
											HandleEQSel(instance,modFRequest,MODFReqtags,2,modGadgets,modWin);
											break;
										case GADID_EQSEL4:
											HandleEQSel(instance,modFRequest,MODFReqtags,3,modGadgets,modWin);
											break;
										case GADID_EQSEL5:
											HandleEQSel(instance,modFRequest,MODFReqtags,4,modGadgets,modWin);
											break;
										case GADID_EQSEL6:
											HandleEQSel(instance,modFRequest,MODFReqtags,5,modGadgets,modWin);
											break;
										case GADID_EQSEL7:
											HandleEQSel(instance,modFRequest,MODFReqtags,6,modGadgets,modWin);
											break;
										case GADID_EQSEL8:
											HandleEQSel(instance,modFRequest,MODFReqtags,7,modGadgets,modWin);
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
	//-- buffer access
	Param param;
	UBYTE newdst=FALSE;
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
	instance->axis_xs=atof(param);
	instance->axis_xs=RangeI(instance->axis_xs,0.0,1.0);
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_PROPOTION,PROP_FC);
	instance->axis_xe=atof(param);
	instance->axis_xe=RangeI(instance->axis_xe,0.0,1.0);
	instance->axis_xd=instance->axis_xe-instance->axis_xs;

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_PROPOTION,PROP_FC);
	instance->axis_ys=atof(param);
	instance->axis_ys=RangeI(instance->axis_ys,0.0,1.0);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_PROPOTION,PROP_FC);
	instance->axis_ye=atof(param);
	instance->axis_ye=RangeI(instance->axis_ye,0.0,1.0);
	instance->axis_yd=instance->axis_ye-instance->axis_ys;

	UC_ConvertParameter(param,instance->mod3.vals,UGRP_PROPOTION,PROP_FC);
	instance->axis_zs=atof(param);
	instance->axis_zs=RangeI(instance->axis_zs,0.0,1.0);
	UC_ConvertParameter(param,instance->mod3.vale,UGRP_PROPOTION,PROP_FC);
	instance->axis_ze=atof(param);
	instance->axis_ze=RangeI(instance->axis_ze,0.0,1.0);
	instance->axis_zd=instance->axis_ze-instance->axis_zs;

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
		if(!pf) pf=ParseWinFunctionSet(&(instance->wfkt1),"W1",params);
		if(!pf && !stricmp(RexxPar2,"Lev1"))		{ instance->eqf1[atoi(RexxPar3)%65]=(double)(atoi(RexxPar4)%100)/100.0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Lev2"))		{ instance->eqf2[atoi(RexxPar3)%65]=(double)(atoi(RexxPar4)%100)/100.0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Lev3"))		{ instance->eqf3[atoi(RexxPar3)%65]=(double)(atoi(RexxPar4)%100)/100.0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Lev4"))		{ instance->eqf4[atoi(RexxPar3)%65]=(double)(atoi(RexxPar4)%100)/100.0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Lev5"))		{ instance->eqf5[atoi(RexxPar3)%65]=(double)(atoi(RexxPar4)%100)/100.0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Lev6"))		{ instance->eqf6[atoi(RexxPar3)%65]=(double)(atoi(RexxPar4)%100)/100.0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Lev7"))		{ instance->eqf7[atoi(RexxPar3)%65]=(double)(atoi(RexxPar4)%100)/100.0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Lev8"))		{ instance->eqf8[atoi(RexxPar3)%65]=(double)(atoi(RexxPar4)%100)/100.0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Name1"))		{ LoadEqfs(RexxPar3,instance->eqf1);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Name2"))		{ LoadEqfs(RexxPar3,instance->eqf2);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Name3"))		{ LoadEqfs(RexxPar3,instance->eqf3);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Name4"))		{ LoadEqfs(RexxPar3,instance->eqf4);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Name5"))		{ LoadEqfs(RexxPar3,instance->eqf5);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Name6"))		{ LoadEqfs(RexxPar3,instance->eqf6);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Name7"))		{ LoadEqfs(RexxPar3,instance->eqf7);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Name8"))		{ LoadEqfs(RexxPar3,instance->eqf8);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ instance->bands	=CfgIO_MatchString(RexxPar3,BandLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Steps"))		{ instance->steps	=CfgIO_MatchString(RexxPar3,StepLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"View"))		{ instance->view		=CfgIO_MatchString(RexxPar3,ViewLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Prec"))		{ instance->prec		=CfgIO_MatchString(RexxPar3,PrecLabels);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet  (&(instance->mod1),"P1",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod2),"P2",params,ret);
		if(!pf) pf=ParseModulatorGet  (&(instance->mod3),"P3",params,ret);
		if(!pf) pf=ParseWinFunctionGet(&(instance->wfkt1),"W1",params,ret);
		if(!pf && !stricmp(RexxPar2,"Bands"))		{ strcpy(ret,BandLabels[instance->bands]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Steps"))		{ strcpy(ret,StepLabels[instance->steps]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"View"))		{ strcpy(ret,ViewLabels[instance->view]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Prec"))		{ strcpy(ret,PrecLabels[instance->prec]);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	register UBYTE i;
	char strItemName[10];

	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	for(i=0;i<65;i++) {
		sprintf(strItemName,"Eqf1_%d",i);instance->eqf1[i]=atof(CfgIO_ReadString	(cfg,"General"	,strItemName,"1.0"));
		sprintf(strItemName,"Eqf2_%d",i);instance->eqf2[i]=atof(CfgIO_ReadString	(cfg,"General"	,strItemName,"1.0"));
		sprintf(strItemName,"Eqf3_%d",i);instance->eqf3[i]=atof(CfgIO_ReadString	(cfg,"General"	,strItemName,"1.0"));
		sprintf(strItemName,"Eqf4_%d",i);instance->eqf4[i]=atof(CfgIO_ReadString	(cfg,"General"	,strItemName,"1.0"));
		sprintf(strItemName,"Eqf5_%d",i);instance->eqf5[i]=atof(CfgIO_ReadString	(cfg,"General"	,strItemName,"1.0"));
		sprintf(strItemName,"Eqf6_%d",i);instance->eqf6[i]=atof(CfgIO_ReadString	(cfg,"General"	,strItemName,"1.0"));
		sprintf(strItemName,"Eqf7_%d",i);instance->eqf7[i]=atof(CfgIO_ReadString	(cfg,"General"	,strItemName,"1.0"));
		sprintf(strItemName,"Eqf8_%d",i);instance->eqf8[i]=atof(CfgIO_ReadString	(cfg,"General"	,strItemName,"1.0"));
	}
	strItemName[0]='\0';
	strcpy(instance->name1,CfgIO_ReadString	(cfg,"General"	,"Eqf1",strItemName));
	strcpy(instance->name2,CfgIO_ReadString	(cfg,"General"	,"Eqf2",strItemName));
	strcpy(instance->name3,CfgIO_ReadString	(cfg,"General"	,"Eqf3",strItemName));
	strcpy(instance->name4,CfgIO_ReadString	(cfg,"General"	,"Eqf4",strItemName));
	strcpy(instance->name5,CfgIO_ReadString	(cfg,"General"	,"Eqf5",strItemName));
	strcpy(instance->name6,CfgIO_ReadString	(cfg,"General"	,"Eqf6",strItemName));
	strcpy(instance->name7,CfgIO_ReadString	(cfg,"General"	,"Eqf7",strItemName));
	strcpy(instance->name8,CfgIO_ReadString	(cfg,"General"	,"Eqf8",strItemName));
	
	instance->bands=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Bands",BandLabels[instance->bands]),BandLabels);
	instance->steps=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Steps",StepLabels[instance->steps]),StepLabels);
	instance->view=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"View", ViewLabels[instance->view]), ViewLabels);
	instance->prec=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Prec", PrecLabels[instance->prec]), PrecLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	register UBYTE i;
	char strItemName[10];

	SaveModSettings(cfg,instance->moddesc);
	for(i=0;i<65;i++) {
		sprintf(strItemName,"Eqf1_%d",i);CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->eqf1[i]);
		sprintf(strItemName,"Eqf2_%d",i);CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->eqf2[i]);
		sprintf(strItemName,"Eqf3_%d",i);CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->eqf3[i]);
		sprintf(strItemName,"Eqf4_%d",i);CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->eqf4[i]);
		sprintf(strItemName,"Eqf5_%d",i);CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->eqf5[i]);
		sprintf(strItemName,"Eqf6_%d",i);CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->eqf6[i]);
		sprintf(strItemName,"Eqf7_%d",i);CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->eqf7[i]);
		sprintf(strItemName,"Eqf8_%d",i);CfgIO_WriteFloat	  (cfg,"General"	,strItemName	,&instance->eqf8[i]);
	}
	CfgIO_WriteString		  (cfg,"General"	,"Eqf1"			,instance->name1);
	CfgIO_WriteString		  (cfg,"General"	,"Eqf2"			,instance->name2);
	CfgIO_WriteString		  (cfg,"General"	,"Eqf3"			,instance->name3);
	CfgIO_WriteString		  (cfg,"General"	,"Eqf4"			,instance->name4);
	CfgIO_WriteString		  (cfg,"General"	,"Eqf5"			,instance->name5);
	CfgIO_WriteString		  (cfg,"General"	,"Eqf6"			,instance->name6);
	CfgIO_WriteString		  (cfg,"General"	,"Eqf7"			,instance->name7);
	CfgIO_WriteString		  (cfg,"General"	,"Eqf8"			,instance->name8);

	CfgIO_WriteString		  (cfg,"General"	,"Bands"		,BandLabels[instance->bands]);
	CfgIO_WriteString		  (cfg,"General"	,"Steps"		,StepLabels[instance->steps]);
	CfgIO_WriteString		  (cfg,"General"	,"View"			,ViewLabels[instance->view]);
	CfgIO_WriteString		  (cfg,"General"	,"Prec"			,PrecLabels[instance->prec]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	register UBYTE i;

	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod1.vals,"0.5",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod1.vale,"0.5",UGRP_PROPOTION,PROP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"0.5",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod2.vale,"0.5",UGRP_PROPOTION,PROP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	UC_ConvertParameter(instance->mod3.vals,"0.5",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod3.vale,"0.5",UGRP_PROPOTION,PROP_FC);
	instance->mod3.bshape=BSHP_NONE;
	instance->mod3.desc[0]='\0';

	instance->wfkt1.win=WIN_HAMMING;
	instance->wfkt1.par=0.54;
	instance->wfkt1.desc[0]='\0';

	for(i=0;i<65;i++) instance->eqf1[i]=instance->eqf2[i]=instance->eqf3[i]=instance->eqf4[i]=instance->eqf5[i]=instance->eqf6[i]=instance->eqf7[i]=instance->eqf8[i]=0;
	for(i=0;i<40;i++) instance->name1[i]=instance->name2[i]=instance->name3[i]=instance->name4[i]=instance->name5[i]=instance->name6[i]=instance->name7[i]=instance->name8[i]=0;
	instance->view=0;
	instance->prec=1;
	instance->slen=1000;
	instance->bands=4;
	instance->steps=5;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource     (&(instance->src),  modWin,modGadgets,GADIX_SRC);
	SetModulator  (&(instance->mod1), modWin,modGadgets,GADIX_MOD1);
	SetModulator  (&(instance->mod2), modWin,modGadgets,GADIX_MOD2);
	SetModulator  (&(instance->mod3), modWin,modGadgets,GADIX_MOD3);
	SetWinFunction(&(instance->wfkt1),modWin,modGadgets,GADIX_WFKT);
	GT_SetGadgetAttrs(modGadgets[GADIX_VIEW   ],modWin,0l,GTCY_Active,instance->view,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_PREC   ],modWin,0l,GTCY_Active,instance->prec,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME1],modWin,0l,GTTX_Text,instance->name1,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME2],modWin,0l,GTTX_Text,instance->name2,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME3],modWin,0l,GTTX_Text,instance->name3,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME4],modWin,0l,GTTX_Text,instance->name4,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME5],modWin,0l,GTTX_Text,instance->name5,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME6],modWin,0l,GTTX_Text,instance->name6,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME7],modWin,0l,GTTX_Text,instance->name7,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME8],modWin,0l,GTTX_Text,instance->name8,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_BANDS  ],modWin,0l,GTCY_Active,instance->bands,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_STEPS  ],modWin,0l,GTCY_Active,instance->steps,TAG_DONE);

	{
		// this is a very ugly hack, the cube should realy be a custom boopsi gadget
		UWORD ys1=2;
		UWORD ys2=ys1+1+SRCSEL_HEIGHT;
		UWORD ys3=ys2+1+ANYFRM_HEIGHT(4);
		UWORD xs1=2;
		UWORD xs2=xs1+1+STDFRM_WIDTH;

		DrawWay(instance,modWin->RPort,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xs2+RunTime->scx[37],ys3+RunTime->scy[18]);
	}
}

//-- private Stuff ------------------------------------------------------------

void DrawWay(struct Instance *instance,struct RastPort *rp,UBYTE view,UBYTE prec,Modulator *mod1,Modulator *mod2,Modulator *mod3,UWORD xs,UWORD ys) {
	register ULONG i;
	double axis_xe,axis_xs,axis_ye,axis_ys,axis_ze,axis_zs;
	double axis_x,axis_xd,axis_y,axis_yd,axis_z,axis_zd;
	UWORD rx,ry;
	UWORD step=0;
	Param param;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;

	if(!instance->slen) return;

	switch(prec) {
		case 0: step=instance->slen/ 100;break;
		case 1: step=instance->slen/ 500;break;
		case 2: step=instance->slen/1000;break;
	}
	if(!step) step=1;

	mod1->slen=mod2->slen=mod3->slen=instance->slen;
	AssignBShape(mod1);
	moddata1=(void *)(mod1->bshpdata);
	modfunc1=mod1->modfunc;
	AssignBShape(mod2);
	moddata2=(void *)(mod2->bshpdata);
	modfunc2=mod2->modfunc;
	AssignBShape(mod3);
	moddata3=(void *)(mod3->bshpdata);
	modfunc3=mod3->modfunc;

	SetAPen(rp,0);RectFill(rp,xs,ys+RunTime->scy[3],xs+RunTime->scx[170],ys+RunTime->scy[82]);SetAPen(rp,1);		/* Löschen */

	UC_ConvertParameter(param,instance->mod1.vals,UGRP_PROPOTION,PROP_FC);
	axis_xs=atof(param);
	axis_xs=RangeI(axis_xs,0.0,1.0);
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_PROPOTION,PROP_FC);
	axis_xe=atof(param);
	axis_xe=RangeI(axis_xe,0.0,1.0);
	axis_xd=axis_xe-axis_xs;

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_PROPOTION,PROP_FC);
	axis_ys=atof(param);
	axis_ys=RangeI(axis_ys,0.0,1.0);
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_PROPOTION,PROP_FC);
	axis_ye=atof(param);
	axis_ye=RangeI(axis_ye,0.0,1.0);
	axis_yd=axis_ye-axis_ys;

	UC_ConvertParameter(param,instance->mod3.vals,UGRP_PROPOTION,PROP_FC);
	axis_zs=atof(param);
	axis_zs=RangeI(axis_zs,0.0,1.0);
	UC_ConvertParameter(param,instance->mod3.vale,UGRP_PROPOTION,PROP_FC);
	axis_ze=atof(param);
	axis_ze=RangeI(axis_ze,0.0,1.0);
	axis_zd=axis_ze-axis_zs;

	SetAPen(rp,6);																/* Würfel malen (verdeckt) */
	Move(rp,xs+RunTime->scx[145],ys+RunTime->scy[55]);
	Draw(rp,xs+RunTime->scx[ 45],ys+RunTime->scy[55]);
	Draw(rp,xs+RunTime->scx[ 45],ys+RunTime->scy[ 5]);
	Move(rp,xs+RunTime->scx[ 45],ys+RunTime->scy[55]);
	Draw(rp,xs+RunTime->scx[  5],ys+RunTime->scy[75]);

	SetAPen(rp,1);
	switch(view) {
		case 0:			/* vorne */
			RectFill(rp,xs+RunTime->scx[3],ys+RunTime->scy[74],xs+RunTime->scx[7],ys+RunTime->scy[76]);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[82]);Text(rp,"S1",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[82]);Text(rp,"S2",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[62]);Text(rp,"S4",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[62]);Text(rp,"S3",2);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[32]);Text(rp,"S5",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[32]);Text(rp,"S6",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[12]);Text(rp,"S8",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[12]);Text(rp,"S7",2);
			SetAPen(rp,7);
			axis_x=axis_xs+axis_xd*modfunc1(0,instance->slen,moddata1);
			axis_y=axis_ys+axis_yd*modfunc2(0,instance->slen,moddata2);
			axis_z=axis_zs+axis_zd*modfunc3(0,instance->slen,moddata3);
			rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
			Move(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			for(i=step;i<instance->slen;i+=step)
			{
				axis_x=axis_xs+axis_xd*modfunc1(i,instance->slen,moddata1);
				axis_y=axis_ys+axis_yd*modfunc2(i,instance->slen,moddata2);
				axis_z=axis_zs+axis_zd*modfunc3(i,instance->slen,moddata3);
				rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
				Draw(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			}
			break;
		case 1:			/* hinten */
			RectFill(rp,xs+RunTime->scx[143],ys+RunTime->scy[54],xs+RunTime->scx[147],ys+RunTime->scy[56]);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[82]);Text(rp,"S3",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[82]);Text(rp,"S4",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[62]);Text(rp,"S2",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[62]);Text(rp,"S1",2);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[32]);Text(rp,"S7",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[32]);Text(rp,"S8",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[12]);Text(rp,"S6",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[12]);Text(rp,"S5",2);
			SetAPen(rp,7);
			axis_x=1.0-(axis_xs+axis_xd*modfunc1(0,instance->slen,moddata1));
			axis_y=     axis_ys+axis_yd*modfunc2(0,instance->slen,moddata2);
			axis_z=1.0-(axis_zs+axis_zd*modfunc3(0,instance->slen,moddata3));
			rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
			Move(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			for(i=step;i<instance->slen;i+=step)
			{
				axis_x=1.0-(axis_xs+axis_xd*modfunc1(i,instance->slen,moddata1));
				axis_y=     axis_ys+axis_yd*modfunc2(i,instance->slen,moddata2);
				axis_z=1.0-(axis_zs+axis_zd*modfunc3(i,instance->slen,moddata3));
				rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
				Draw(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			}
			break;
		case 2:			/* rechts */
			RectFill(rp,xs+RunTime->scx[43],ys+RunTime->scy[54],xs+RunTime->scx[47],ys+RunTime->scy[56]);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[82]);Text(rp,"S2",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[82]);Text(rp,"S3",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[62]);Text(rp,"S1",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[62]);Text(rp,"S4",2);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[32]);Text(rp,"S6",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[32]);Text(rp,"S7",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[12]);Text(rp,"S5",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[12]);Text(rp,"S8",2);
			SetAPen(rp,7);
			axis_z=1.0-(axis_xs+axis_xd*modfunc1(0,instance->slen,moddata1));
			axis_y=     axis_ys+axis_yd*modfunc2(0,instance->slen,moddata2);
			axis_x=     axis_zs+axis_zd*modfunc3(0,instance->slen,moddata3);
			rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
			Move(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			for(i=step;i<instance->slen;i+=step)
			{
				axis_z=1.0-(axis_xs+axis_xd*modfunc1(i,instance->slen,moddata1));
				axis_y=     axis_ys+axis_yd*modfunc2(i,instance->slen,moddata2);
				axis_x=     axis_zs+axis_zd*modfunc3(i,instance->slen,moddata3);
				rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
				Draw(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			}
			break;
		case 3:			/* links */
			RectFill(rp,xs+RunTime->scx[103],ys+RunTime->scy[74],xs+RunTime->scx[107],ys+RunTime->scy[76]);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[82]);Text(rp,"S4",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[82]);Text(rp,"S1",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[62]);Text(rp,"S3",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[62]);Text(rp,"S2",2);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[32]);Text(rp,"S8",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[32]);Text(rp,"S5",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[12]);Text(rp,"S7",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[12]);Text(rp,"S6",2);
			axis_z=     axis_xs+axis_xd*modfunc1(0,instance->slen,moddata1);
			axis_y=     axis_ys+axis_yd*modfunc2(0,instance->slen,moddata2);
			axis_x=1.0-(axis_zs+axis_zd*modfunc3(0,instance->slen,moddata3));
			rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
			Move(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			SetAPen(rp,7);
			for(i=step;i<instance->slen;i+=step)
			{
				axis_z=     axis_xs+axis_xd*modfunc1(i,instance->slen,moddata1);
				axis_y=     axis_ys+axis_yd*modfunc2(i,instance->slen,moddata2);
				axis_x=1.0-(axis_zs+axis_zd*modfunc3(i,instance->slen,moddata3));
				rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
				Draw(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			}
			break;
		case 4:			/* oben */
			RectFill(rp,xs+RunTime->scx[43],ys+RunTime->scy[54],xs+RunTime->scx[47],ys+RunTime->scy[56]);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[82]);Text(rp,"S5",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[82]);Text(rp,"S6",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[62]);Text(rp,"S1",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[62]);Text(rp,"S2",2);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[32]);Text(rp,"S8",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[32]);Text(rp,"S7",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[12]);Text(rp,"S4",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[12]);Text(rp,"S3",2);
			SetAPen(rp,7);
			axis_x=     axis_xs+axis_xd*modfunc1(0,instance->slen,moddata1);
			axis_z=1.0-(axis_ys+axis_yd*modfunc2(0,instance->slen,moddata2));
			axis_y=     axis_zs+axis_zd*modfunc3(0,instance->slen,moddata3);
			rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
			Move(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			for(i=step;i<instance->slen;i+=step)
			{
				axis_x=     axis_xs+axis_xd*modfunc1(i,instance->slen,moddata1);
				axis_z=1.0-(axis_ys+axis_yd*modfunc2(i,instance->slen,moddata2));
				axis_y=     axis_zs+axis_zd*modfunc3(i,instance->slen,moddata3);
				rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
				Draw(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			}
			break;
		case 5:			/* unten */
			RectFill(rp,xs+RunTime->scx[3],ys+RunTime->scy[24],xs+RunTime->scx[7],ys+RunTime->scy[26]);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[82]);Text(rp,"S4",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[82]);Text(rp,"S3",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[62]);Text(rp,"S8",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[62]);Text(rp,"S7",2);
			Move(rp,xs+RunTime->scx[  7],ys+RunTime->scy[32]);Text(rp,"S1",2);
			Move(rp,xs+RunTime->scx[107],ys+RunTime->scy[32]);Text(rp,"S2",2);
			Move(rp,xs+RunTime->scx[ 47],ys+RunTime->scy[12]);Text(rp,"S5",2);
			Move(rp,xs+RunTime->scx[147],ys+RunTime->scy[12]);Text(rp,"S6",2);
			SetAPen(rp,7);
			axis_x=     axis_xs+axis_xd*modfunc1(0,instance->slen,moddata1);
			axis_z=     axis_ys+axis_yd*modfunc2(0,instance->slen,moddata2);
			axis_y=1.0-(axis_zs+axis_zd*modfunc3(0,instance->slen,moddata3));
			rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
			Move(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			for(i=step;i<instance->slen;i+=step)
			{
				axis_x=     axis_xs+axis_xd*modfunc1(i,instance->slen,moddata1);
				axis_z=     axis_ys+axis_yd*modfunc2(i,instance->slen,moddata2);
				axis_y=1.0-(axis_zs+axis_zd*modfunc3(i,instance->slen,moddata3));
				rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
				Draw(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			}
			break;
	}
																/* Würfel malen */
	SetAPen(rp,1);
	Move(rp,xs+RunTime->scx[105],ys+RunTime->scy[75]);
	Draw(rp,xs+RunTime->scx[145],ys+RunTime->scy[55]);
	Draw(rp,xs+RunTime->scx[145],ys+RunTime->scy[05]);
	Draw(rp,xs+RunTime->scx[ 45],ys+RunTime->scy[05]);
	Draw(rp,xs+RunTime->scx[  5],ys+RunTime->scy[25]);
	Draw(rp,xs+RunTime->scx[  5],ys+RunTime->scy[75]);
	Draw(rp,xs+RunTime->scx[105],ys+RunTime->scy[75]);
	Draw(rp,xs+RunTime->scx[105],ys+RunTime->scy[25]);
	Draw(rp,xs+RunTime->scx[  5],ys+RunTime->scy[25]);

	Move(rp,xs+RunTime->scx[105],ys+RunTime->scy[25]);
	Draw(rp,xs+RunTime->scx[145],ys+RunTime->scy[05]);
}

void HandleEQSel(struct Instance *instance,struct FileRequester *modFRequest,struct TagItem *MODFReqtags,UBYTE index,struct Gadget *modGadgets[],struct Window *modWin) {
	UBYTE anz;
	register UBYTE i;
	char fn[FILENAME_MAX];
	
	MODFReqtags_InitialDrawer=(ULONG)PROGDIR"_operators/Equalize-FFT";
	MODFReqtags_DoSaveMode=FALSE;
	MODFReqtags_DoMultiSelect=TRUE;
	if(AslRequest(modFRequest,MODFReqtags)) {
		if((anz=modFRequest->rf_NumArgs)>1) {
			//-- multiselect
			struct WBArg *args=modFRequest->rf_ArgList;
			for(i=0;(i<anz) && (index+i<8);i++) {                                // load more than one preset
				strmfp(fn,modFRequest->fr_Drawer,args[i].wa_Name);
				LoadEqfs(fn,instance->eqf[i]);
				strncpy(instance->name[i],args[i].wa_Name,39);instance->name[i][39]=0;
				GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME1+i],modWin,0l,GTTX_Text,instance->name[i],TAG_DONE);
			}
		}
		else {
			strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File);
			LoadEqfs(fn,instance->eqf[i]);
			strncpy(instance->name1,modFRequest->fr_File,39);instance->name1[39]=0;
			GT_SetGadgetAttrs(modGadgets[GADIX_EQNAME1+i],modWin,0l,GTTX_Text,instance->name1,TAG_DONE);
		}
	}
}

void LoadEqfs(char *fname,double *eqf){ 
	APTR 	 cfgEQ;
	register UBYTE i;
	char strItemName[10];

	if((cfgEQ=CfgIO_NewConfig("Equalize-FFT","$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) {
		CfgIO_ReadConfig(cfgEQ,fname);
		for(i=0;i<65;i++) {
			sprintf(strItemName,"Eqf%d",i);
			eqf[i]=atof(CfgIO_ReadString	(cfgEQ,"General"	,strItemName,"1.0"));
		}
		CfgIO_RemConfig(cfgEQ);
	}
}

//-- eof ----------------------------------------------------------------------
