/****h* Mix-3D/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (18.07.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Mix-3D Operator module for SoundFX
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

#define MIX_3D_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Mix-3D.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

void DrawWay(struct Instance *instance,struct RastPort *rp,UBYTE view,UBYTE prec,Modulator *mod1,Modulator *mod2,Modulator *mod3,UWORD xs,UWORD ys);

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

#define GADID_SRC1		(GADID_CFGSEL+CFGSEL_IDCT)
#define GADIX_SRC1		(GADIX_CFGSEL+CFGSEL_IXCT)

#define GADID_SRC2		(GADID_SRC1+SRC_IDCT)
#define GADIX_SRC2		(GADIX_SRC1+SRC_IXCT)

#define GADID_SRC3		(GADID_SRC2+SRC_IDCT)
#define GADIX_SRC3		(GADIX_SRC2+SRC_IXCT)

#define GADID_SRC4		(GADID_SRC3+SRC_IDCT)
#define GADIX_SRC4		(GADIX_SRC3+SRC_IXCT)

#define GADID_SRC5		(GADID_SRC4+SRC_IDCT)
#define GADIX_SRC5		(GADIX_SRC4+SRC_IXCT)

#define GADID_SRC6		(GADID_SRC5+SRC_IDCT)
#define GADIX_SRC6		(GADIX_SRC5+SRC_IXCT)

#define GADID_SRC7		(GADID_SRC6+SRC_IDCT)
#define GADIX_SRC7		(GADIX_SRC6+SRC_IXCT)

#define GADID_SRC8		(GADID_SRC7+SRC_IDCT)
#define GADIX_SRC8		(GADIX_SRC7+SRC_IXCT)

#define GADID_MOD1		(GADID_SRC8+SRC_IDCT)
#define GADIX_MOD1		(GADIX_SRC8+SRC_IXCT)

#define GADID_MOD2		(GADID_MOD1+MOD_IDCT)
#define GADIX_MOD2		(GADIX_MOD1+MOD_IXCT)

#define GADID_MOD3		(GADID_MOD2+MOD_IDCT)
#define GADIX_MOD3		(GADIX_MOD2+MOD_IXCT)

#define GADID_VIEW		(GADID_MOD3+MOD_IDCT)
#define GADIX_VIEW		(GADIX_MOD3+MOD_IXCT)

#define GADID_PREC		(GADID_VIEW+1)
#define GADIX_PREC		(GADIX_VIEW+1)

#define LAST_GADIX		(GADIX_PREC+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[12];
};

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
		SFXMod_ModDescInit( 0,PD_SOURCE      ,"S1",&(instance->src1));
		SFXMod_ModDescInit( 1,PD_SOURCE      ,"S2",&(instance->src2));
		SFXMod_ModDescInit( 2,PD_SOURCE      ,"S3",&(instance->src3));
		SFXMod_ModDescInit( 3,PD_SOURCE      ,"S4",&(instance->src4));
		SFXMod_ModDescInit( 4,PD_SOURCE      ,"S5",&(instance->src5));
		SFXMod_ModDescInit( 5,PD_SOURCE      ,"S6",&(instance->src6));
		SFXMod_ModDescInit( 6,PD_SOURCE      ,"S7",&(instance->src7));
		SFXMod_ModDescInit( 7,PD_SOURCE      ,"S8",&(instance->src8));
		SFXMod_ModDescInit( 8,PD_MODULATOR   ,"M1",&(instance->mod1));
		SFXMod_ModDescInit( 9,PD_MODULATOR   ,"M2",&(instance->mod2));
		SFXMod_ModDescInit(10,PD_MODULATOR   ,"M3",&(instance->mod3));
		SFXMod_ModDescInit(11,PD_END         ,0l  ,0l);	
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
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(4);
	UWORD ys3=ys2+1+MODSEL_HEIGHT;
	UWORD ys4=ys3+1+MODSEL_HEIGHT;
	UWORD ys5=ys4+1+MODSEL_HEIGHT;
	UWORD winh=ys5+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD xs3=xs2+3+STDFRM_WIDTH;
	UWORD winw=xs3+3+CFGSEL_WIDTH;
	UWORD xh,yh;

	UBYTE gh;

	if(RunTime->aktsamples) {
		instance->slen=instance->src1.srcbuf->slen;
		if(instance->src2.srcbuf->slen>instance->slen) instance->slen=instance->src2.srcbuf->slen;
		if(instance->src3.srcbuf->slen>instance->slen) instance->slen=instance->src3.srcbuf->slen;
		if(instance->src4.srcbuf->slen>instance->slen) instance->slen=instance->src4.srcbuf->slen;
		if(instance->src5.srcbuf->slen>instance->slen) instance->slen=instance->src5.srcbuf->slen;
		if(instance->src6.srcbuf->slen>instance->slen) instance->slen=instance->src6.srcbuf->slen;
		if(instance->src7.srcbuf->slen>instance->slen) instance->slen=instance->src7.srcbuf->slen;
		if(instance->src8.srcbuf->slen>instance->slen) instance->slen=instance->src8.srcbuf->slen;
	}
	else instance->slen=1000;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
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

		g=AddSource(&(instance->src1),modGadgets,g,GADID_SRC1,GADIX_SRC1,xs1,ys1);
		g=AddSource(&(instance->src2),modGadgets,g,GADID_SRC2,GADIX_SRC2,xs1,ys1+RunTime->scy[12]);
		g=AddSource(&(instance->src3),modGadgets,g,GADID_SRC3,GADIX_SRC3,xs1,ys1+RunTime->scy[24]);
		g=AddSource(&(instance->src4),modGadgets,g,GADID_SRC4,GADIX_SRC4,xs1,ys1+RunTime->scy[36]);
		g=AddSource(&(instance->src5),modGadgets,g,GADID_SRC5,GADIX_SRC5,xs2,ys1);
		g=AddSource(&(instance->src6),modGadgets,g,GADID_SRC6,GADIX_SRC6,xs2,ys1+RunTime->scy[12]);
		g=AddSource(&(instance->src7),modGadgets,g,GADID_SRC7,GADIX_SRC7,xs2,ys1+RunTime->scy[24]);
		g=AddSource(&(instance->src8),modGadgets,g,GADID_SRC8,GADIX_SRC8,xs2,ys1+RunTime->scy[36]);

		g=AddModulator(&(instance->mod1),modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys2);
		g=AddModulator(&(instance->mod2),modGadgets,g,GADID_MOD2,GADIX_MOD2,xs1,ys3);
		g=AddModulator(&(instance->mod3),modGadgets,g,GADID_MOD3,GADIX_MOD3,xs1,ys4);

		ng.ng_LeftEdge		=xs2+RunTime->scx[53];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_GadgetText	="View";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_VIEW;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ViewLabels,GTCY_Active,instance->view,TAG_DONE);
		modGadgets[GADIX_VIEW]=g;

		ng.ng_LeftEdge		=xs2+RunTime->scx[195];
		ng.ng_GadgetText	="Prec.";
		ng.ng_GadgetID		=GADID_PREC;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,PrecLabels,GTCY_Active,instance->prec,TAG_DONE);
		modGadgets[GADIX_PREC]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src1",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Src2",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Src3",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(3)]);
			RText(modrp,"Src4",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(4)]);
			RText(modrp,"Src5",xs2+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Src6",xs2+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Src7",xs2+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(3)]);
			RText(modrp,"Src8",xs2+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(4)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys4+RunTime->scy[FRMTEXT_YPOS(2)]);

			xh=STDFRM_WIDTH;
			DrawTitle(modrp,"X-Axis"			,xs1,ys2,xh);
			DrawTitle(modrp,"Y-Axis"			,xs1,ys3,xh);
			DrawTitle(modrp,"Z-Axis"			,xs1,ys4,xh);
			DrawTitle(modrp,"Path"				,xs2,ys2,xh);
			xh+=1+STDFRM_WIDTH;
			DrawTitle(modrp,"Sources"			,xs1,ys1,xh);
			DrawTitle(modrp,"Presets"			,xs3,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;yh=MODSEL_HEIGHT;
			DrawGadWB(modrp,xs1,ys2,xh,yh);								/* X-Achse */
			DrawGadWB(modrp,xs1,ys3,xh,yh);								/* Y-Achse */
			DrawGadWB(modrp,xs1,ys4,xh,yh);								/* Z-Achse */
			DrawGadWB(modrp,xs2,ys2,xh,2+yh+yh+yh);						/* Cube */
			xh+=1+STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,ANYFRM_HEIGHT(4));				/* Src`s */
			DrawGadWB(modrp,xs1,ys5,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs3,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

			SetCfgSel   (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			SetSource   (&(instance->src1),modWin,modGadgets,GADIX_SRC1);
			SetSource   (&(instance->src2),modWin,modGadgets,GADIX_SRC2);
			SetSource   (&(instance->src3),modWin,modGadgets,GADIX_SRC3);
			SetSource   (&(instance->src4),modWin,modGadgets,GADIX_SRC4);
			SetSource   (&(instance->src5),modWin,modGadgets,GADIX_SRC5);
			SetSource   (&(instance->src6),modWin,modGadgets,GADIX_SRC6);
			SetSource   (&(instance->src7),modWin,modGadgets,GADIX_SRC7);
			SetSource   (&(instance->src8),modWin,modGadgets,GADIX_SRC8);
			SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
			SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
			SetModulator(&(instance->mod3),modWin,modGadgets,GADIX_MOD3);

			xh=xs2+RunTime->scx[37];
			yh=ys2+RunTime->scy[18];
			DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);

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
						instance->src3.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src3.rmode=SetRngMode(instance->src3.srcbuf);
						instance->src4.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src4.rmode=SetRngMode(instance->src4.srcbuf);
						instance->src5.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src5.rmode=SetRngMode(instance->src5.srcbuf);
						instance->src6.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src6.rmode=SetRngMode(instance->src6.srcbuf);
						instance->src7.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src7.rmode=SetRngMode(instance->src7.srcbuf);
						instance->src8.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src8.rmode=SetRngMode(instance->src8.srcbuf);
						instance->slen=instance->src1.srcbuf->slen;
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=0;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					}
					SetSource   (&(instance->src1),modWin,modGadgets,GADIX_SRC1);
					SetSource   (&(instance->src2),modWin,modGadgets,GADIX_SRC2);
					SetSource   (&(instance->src3),modWin,modGadgets,GADIX_SRC3);
					SetSource   (&(instance->src4),modWin,modGadgets,GADIX_SRC4);
					SetSource   (&(instance->src5),modWin,modGadgets,GADIX_SRC5);
					SetSource   (&(instance->src6),modWin,modGadgets,GADIX_SRC6);
					SetSource   (&(instance->src7),modWin,modGadgets,GADIX_SRC7);
					SetSource   (&(instance->src8),modWin,modGadgets,GADIX_SRC8);
				}
				if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(gh=HandleSource(&(instance->src1),modWin,modGadgets,GADID_SRC1,GADIX_SRC1,gnr,icode)) {
									if(instance->src1.srcbuf->slen>instance->slen) {
										instance->slen=instance->src1.srcbuf->slen;
										DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
									}
								}
								if(!gh && (gh=HandleSource(&(instance->src2),modWin,modGadgets,GADID_SRC2,GADIX_SRC2,gnr,icode))) {
									if(instance->src2.srcbuf->slen>instance->slen) {
										instance->slen=instance->src2.srcbuf->slen;
										DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
									}
								}											
								if(!gh && (gh=HandleSource(&(instance->src3),modWin,modGadgets,GADID_SRC3,GADIX_SRC3,gnr,icode))) {
									if(instance->src3.srcbuf->slen>instance->slen) {
										instance->slen=instance->src3.srcbuf->slen;
										DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
									}
								}											
								if(!gh && (gh=HandleSource(&(instance->src4),modWin,modGadgets,GADID_SRC4,GADIX_SRC4,gnr,icode))) {
									if(instance->src4.srcbuf->slen>instance->slen) {
										instance->slen=instance->src4.srcbuf->slen;
										DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
									}
								}											
								if(!gh && (gh=HandleSource(&(instance->src5),modWin,modGadgets,GADID_SRC5,GADIX_SRC5,gnr,icode))) {
									if(instance->src5.srcbuf->slen>instance->slen) {
										instance->slen=instance->src5.srcbuf->slen;
										DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
									}
								}											
								if(!gh && (gh=HandleSource(&(instance->src6),modWin,modGadgets,GADID_SRC6,GADIX_SRC6,gnr,icode))) {
									if(instance->src6.srcbuf->slen>instance->slen) {
										instance->slen=instance->src6.srcbuf->slen;
										DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
									}
								}											
								if(!gh && (gh=HandleSource(&(instance->src7),modWin,modGadgets,GADID_SRC7,GADIX_SRC7,gnr,icode))) {
									if(instance->src7.srcbuf->slen>instance->slen) {
										instance->slen=instance->src7.srcbuf->slen;
										DrawWay(instance,modrp,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xh,yh);
									}
								}											
								if(!gh && (gh=HandleSource(&(instance->src8),modWin,modGadgets,GADID_SRC8,GADIX_SRC8,gnr,icode))) {
									if(instance->src8.srcbuf->slen>instance->slen) {
										instance->slen=instance->src8.srcbuf->slen;
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
								if(!gh) gh=HandleCfgSel     (&cfgsel        ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic);
								if(!gh) {
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
	//ULONG rm_start,rm_len;
	//-- src & dst buffers
	SInfo *dstbuf=NULL,*srcbuf1,*srcbuf2,*srcbuf3,*srcbuf4,*srcbuf5,*srcbuf6,*srcbuf7,*srcbuf8;
	//-- buffer access
	SmpAccess *srcsa1,*srcsa2,*srcsa3,*srcsa4,*srcsa5,*srcsa6,*srcsa7,*srcsa8,*dstsa;
	SAFuncPtr SARead1,SARead2,SARead3,SARead4,SARead5,SARead6,SARead7,SARead8,SAWrite;
	ULONG blen;
	//-- administrative
	ULONG st1,en1,st2,en2,st3,en3,st4,en4,st5,en5,st6,en6,st7,en7,st8,en8;
	UBYTE maxcha;
	Param param;
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
			UnlockBuffer(instance->src3.srcbuf);
			if((instance->src3.srcbuf=LockBuffer(pdata->src[2]))==NULL) return(FALSE);
			instance->src3.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src4.srcbuf);
			if((instance->src4.srcbuf=LockBuffer(pdata->src[3]))==NULL) return(FALSE);
			instance->src4.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src5.srcbuf);
			if((instance->src5.srcbuf=LockBuffer(pdata->src[4]))==NULL) return(FALSE);
			instance->src5.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src6.srcbuf);
			if((instance->src6.srcbuf=LockBuffer(pdata->src[5]))==NULL) return(FALSE);
			instance->src6.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src7.srcbuf);
			if((instance->src7.srcbuf=LockBuffer(pdata->src[6]))==NULL) return(FALSE);
			instance->src7.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src8.srcbuf);
			if((instance->src8.srcbuf=LockBuffer(pdata->src[7]))==NULL) return(FALSE);
			instance->src8.rmode=RNGMD_ALL;
			break;
		case CM_STREAM:
			UnlockBuffer(instance->src1.srcbuf);
			if((instance->src1.srcbuf=LockBuffer(pdata->src[0]))==NULL) return(FALSE);
			instance->src1.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src2.srcbuf);
			if((instance->src2.srcbuf=LockBuffer(pdata->src[1]))==NULL) return(FALSE);
			instance->src2.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src3.srcbuf);
			if((instance->src3.srcbuf=LockBuffer(pdata->src[2]))==NULL) return(FALSE);
			instance->src3.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src4.srcbuf);
			if((instance->src4.srcbuf=LockBuffer(pdata->src[3]))==NULL) return(FALSE);
			instance->src4.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src5.srcbuf);
			if((instance->src5.srcbuf=LockBuffer(pdata->src[4]))==NULL) return(FALSE);
			instance->src5.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src6.srcbuf);
			if((instance->src6.srcbuf=LockBuffer(pdata->src[5]))==NULL) return(FALSE);
			instance->src6.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src7.srcbuf);
			if((instance->src7.srcbuf=LockBuffer(pdata->src[6]))==NULL) return(FALSE);
			instance->src7.rmode=RNGMD_ALL;
			UnlockBuffer(instance->src8.srcbuf);
			if((instance->src8.srcbuf=LockBuffer(pdata->src[7]))==NULL) return(FALSE);
			instance->src8.rmode=RNGMD_ALL;
			dstbuf=pdata->dst[0];
			break;
		case CM_PREHEAR:
			break;
	}

	srcbuf1=instance->src1.srcbuf;SetRngs(srcbuf1,&st1,&en1,instance->src1.rmode);instance->en1=en1;
	srcbuf2=instance->src2.srcbuf;SetRngs(srcbuf2,&st2,&en2,instance->src2.rmode);instance->en2=en2;
	srcbuf3=instance->src3.srcbuf;SetRngs(srcbuf3,&st3,&en3,instance->src3.rmode);instance->en3=en3;
	srcbuf4=instance->src4.srcbuf;SetRngs(srcbuf4,&st4,&en4,instance->src4.rmode);instance->en4=en4;
	srcbuf5=instance->src5.srcbuf;SetRngs(srcbuf5,&st5,&en5,instance->src5.rmode);instance->en5=en5;
	srcbuf6=instance->src6.srcbuf;SetRngs(srcbuf6,&st6,&en6,instance->src6.rmode);instance->en6=en6;
	srcbuf7=instance->src7.srcbuf;SetRngs(srcbuf7,&st7,&en7,instance->src7.rmode);instance->en7=en7;
	srcbuf8=instance->src8.srcbuf;SetRngs(srcbuf8,&st8,&en8,instance->src8.rmode);instance->en8=en8;
	instance->slen=srcbuf1->slen;
	if(srcbuf2->slen>instance->slen) instance->slen=srcbuf2->slen;
	if(srcbuf3->slen>instance->slen) instance->slen=srcbuf3->slen;
	if(srcbuf4->slen>instance->slen) instance->slen=srcbuf4->slen;
	if(srcbuf5->slen>instance->slen) instance->slen=srcbuf5->slen;
	if(srcbuf6->slen>instance->slen) instance->slen=srcbuf6->slen;
	if(srcbuf7->slen>instance->slen) instance->slen=srcbuf7->slen;
	if(srcbuf8->slen>instance->slen) instance->slen=srcbuf8->slen;
	instance->srat=srcbuf1->srat;

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

	maxcha=srcbuf1->channels;
	if(srcbuf2->channels>maxcha) maxcha=srcbuf2->channels;
	if(srcbuf3->channels>maxcha) maxcha=srcbuf3->channels;
	if(srcbuf4->channels>maxcha) maxcha=srcbuf4->channels;
	if(srcbuf5->channels>maxcha) maxcha=srcbuf5->channels;
	if(srcbuf6->channels>maxcha) maxcha=srcbuf6->channels;
	if(srcbuf7->channels>maxcha) maxcha=srcbuf7->channels;
	if(srcbuf8->channels>maxcha) maxcha=srcbuf8->channels;

	if(!dstbuf) {
		if(dstbuf=NewSample()) {
			if(AllocSampleMem(dstbuf,instance->slen,(maxcha+1))) {
				BufferFill(srcbuf1,dstbuf);
				dstbuf->channels=maxcha;
				dstbuf->channelmask=(1L<<(maxcha+1))-1;
				dstbuf->zoomxl=dstbuf->slen=instance->slen;
				dstbuf->zoomxs=0;
				newdst=TRUE;
			}
			else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
	}
	if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
		if(activate((SFXCoreInstance *)instance)) {
			for(l=0;l<CH_COUNT(dstbuf->channels);l++) {
				//if(srcbuf1->channelmask&(1<<l) && srcbuf2->channelmask&(1<<l)) {
					activate_ch((SFXCoreInstance *)instance,l);
					//if(st1>0) CopyDataCh(srcbuf1,dstbuf,l,0,0,st1);
					srcsa1=SAInit(srcbuf1,l,st1,en1,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa2=SAInit(srcbuf2,l,st2,en2,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa3=SAInit(srcbuf3,l,st2,en3,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa4=SAInit(srcbuf4,l,st2,en4,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa5=SAInit(srcbuf5,l,st2,en5,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa6=SAInit(srcbuf6,l,st2,en6,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa7=SAInit(srcbuf7,l,st2,en7,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					srcsa8=SAInit(srcbuf8,l,st2,en8,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
					dstsa =SAInit(dstbuf ,l,st1,en1,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					if(srcsa1 && srcsa2 && srcsa3 && srcsa4 && srcsa5 && srcsa6 && srcsa7 && srcsa8 && dstsa) {
						SARead1=srcsa1->safunc;SARead2=srcsa2->safunc;SARead3=srcsa3->safunc;SARead4=srcsa4->safunc;
						SARead5=srcsa5->safunc;SARead6=srcsa6->safunc;SARead7=srcsa7->safunc;SARead8=srcsa8->safunc;
						SAWrite=dstsa ->safunc;
						blen=Bytes2Samples(srcsa1->blen);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1,CH_COUNT(dstbuf->channels));
						StartProWin(pwd,(ULONG *)(&instance->curlen),title,instance->slen);					// den Statusbalken starten
						for(srcsa1->seg=srcsa2->seg=dstsa->seg=0;((srcsa1->seg<srcsa1->seganz) && (instance->curlen<instance->slen));srcsa1->seg++,srcsa2->seg++,dstsa->seg++) {
							if(instance->curlen<en1) instance->sbuf1=(SAMPLE *)srcsa1->buffer1;SARead1(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & nächstes Read starten
							if(instance->curlen<en2) instance->sbuf2=(SAMPLE *)srcsa2->buffer1;SARead2(srcsa2,srcsa2->blen);	// Pointer auf src-Block holen & nächstes Read starten
							if(instance->curlen<en3) instance->sbuf3=(SAMPLE *)srcsa3->buffer1;SARead3(srcsa3,srcsa3->blen);	// Pointer auf src-Block holen & nächstes Read starten
							if(instance->curlen<en4) instance->sbuf4=(SAMPLE *)srcsa4->buffer1;SARead4(srcsa4,srcsa4->blen);	// Pointer auf src-Block holen & nächstes Read starten
							if(instance->curlen<en5) instance->sbuf5=(SAMPLE *)srcsa5->buffer1;SARead5(srcsa5,srcsa5->blen);	// Pointer auf src-Block holen & nächstes Read starten
							if(instance->curlen<en6) instance->sbuf6=(SAMPLE *)srcsa6->buffer1;SARead6(srcsa6,srcsa6->blen);	// Pointer auf src-Block holen & nächstes Read starten
							if(instance->curlen<en7) instance->sbuf7=(SAMPLE *)srcsa7->buffer1;SARead7(srcsa7,srcsa7->blen);	// Pointer auf src-Block holen & nächstes Read starten
							if(instance->curlen<en8) instance->sbuf8=(SAMPLE *)srcsa8->buffer1;SARead8(srcsa8,srcsa8->blen);	// Pointer auf src-Block holen & nächstes Read starten
							instance->dbuf =(SAMPLE *)dstsa ->buffer1;								// Pointer auf dst-Block
							process((SFXCoreInstance *)instance,blen);
							SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
						}
					}
					else okay=FALSE;
					if(srcsa1) SADone(srcsa1,srcbuf1);
					if(srcsa2) SADone(srcsa2,srcbuf2);
					if(srcsa3) SADone(srcsa3,srcbuf3);
					if(srcsa4) SADone(srcsa4,srcbuf4);
					if(srcsa5) SADone(srcsa5,srcbuf5);
					if(srcsa6) SADone(srcsa6,srcbuf6);
					if(srcsa7) SADone(srcsa7,srcbuf7);
					if(srcsa8) SADone(srcsa8,srcbuf8);
					if(dstsa)  SADone(dstsa ,dstbuf);
					//if(st1+en1<srcbuf1->slen) CopyDataCh(srcbuf1,dstbuf,l,(st1+en1),(st1+en1),(srcbuf1->slen-(st1+en1)));
					deactivate_ch((SFXCoreInstance *)instance,l);
					if(ProWinAborted(instance->curlen)) { l=CH_MAX;okay=FALSE; }					// es wurde abgebrochen
				//}
			}
			if(okay && newdst) AddSample(dstbuf,((struct Node *)srcbuf1)->ln_Name);			// temporary solution
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
		cf=TRUE;	pf=ParseModulatorSet   (&(instance->mod1), "P1",params);
		if(!pf) pf=ParseModulatorSet   (&(instance->mod2), "P2",params);
		if(!pf) pf=ParseModulatorSet   (&(instance->mod3), "P3",params);
		if(!pf && !stricmp(RexxPar2,"View"))			{ instance->view=CfgIO_MatchString(RexxPar3,ViewLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Prec"))			{ instance->prec=CfgIO_MatchString(RexxPar3,PrecLabels);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet   (&(instance->mod1), "P1",params,ret);
		if(!pf) pf=ParseModulatorGet   (&(instance->mod2), "P2",params,ret);
		if(!pf) pf=ParseModulatorGet   (&(instance->mod3), "P3",params,ret);
		if(!pf && !stricmp(RexxPar2,"View"))			{ strcpy(ret,ViewLabels[instance->view]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Prec"))			{ strcpy(ret,PrecLabels[instance->prec]);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	instance->view=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"View", ViewLabels[instance->view]), ViewLabels);
	instance->prec=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Prec", PrecLabels[instance->prec]), PrecLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"View"			,ViewLabels[instance->view]);
	CfgIO_WriteString		  (cfg,"General"	,"Prec"			,PrecLabels[instance->prec]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	//instance->src1.srcbuf=NULL;
	instance->src1.rmode=RNGMD_ALL;
	//instance->src2.srcbuf=NULL;
	instance->src2.rmode=RNGMD_ALL;
	//instance->src3.srcbuf=NULL;
	instance->src3.rmode=RNGMD_ALL;
	//instance->src4.srcbuf=NULL;
	instance->src4.rmode=RNGMD_ALL;
	//instance->src5.srcbuf=NULL;
	instance->src5.rmode=RNGMD_ALL;
	//instance->src6.srcbuf=NULL;
	instance->src6.rmode=RNGMD_ALL;
	//instance->src7.srcbuf=NULL;
	instance->src7.rmode=RNGMD_ALL;
	//instance->src8.srcbuf=NULL;
	instance->src8.rmode=RNGMD_ALL;

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

	instance->view=0;
	instance->prec=1;
	instance->slen=1000;
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin){ 
	SetSource   (&(instance->src1),modWin,modGadgets,GADIX_SRC1);
	SetSource   (&(instance->src2),modWin,modGadgets,GADIX_SRC2);
	SetSource   (&(instance->src3),modWin,modGadgets,GADIX_SRC3);
	SetSource   (&(instance->src4),modWin,modGadgets,GADIX_SRC4);
	SetSource   (&(instance->src5),modWin,modGadgets,GADIX_SRC5);
	SetSource   (&(instance->src6),modWin,modGadgets,GADIX_SRC6);
	SetSource   (&(instance->src7),modWin,modGadgets,GADIX_SRC7);
	SetSource   (&(instance->src8),modWin,modGadgets,GADIX_SRC8);
	SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
	SetModulator(&(instance->mod3),modWin,modGadgets,GADIX_MOD3);
	GT_SetGadgetAttrs(modGadgets[GADIX_VIEW],modWin,0l,GTCY_Active,instance->view,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_PREC],modWin,0l,GTCY_Active,instance->prec,TAG_DONE);
	instance->slen=0;
	if(instance->src1.srcbuf) instance->slen=instance->src1.srcbuf->slen;
	if(instance->src2.srcbuf && (instance->src2.srcbuf->slen>instance->slen)) instance->slen=instance->src2.srcbuf->slen;
	if(instance->src3.srcbuf && (instance->src3.srcbuf->slen>instance->slen)) instance->slen=instance->src3.srcbuf->slen;
	if(instance->src4.srcbuf && (instance->src4.srcbuf->slen>instance->slen)) instance->slen=instance->src4.srcbuf->slen;
	if(instance->src5.srcbuf && (instance->src5.srcbuf->slen>instance->slen)) instance->slen=instance->src5.srcbuf->slen;
	if(instance->src6.srcbuf && (instance->src6.srcbuf->slen>instance->slen)) instance->slen=instance->src6.srcbuf->slen;
	if(instance->src7.srcbuf && (instance->src7.srcbuf->slen>instance->slen)) instance->slen=instance->src7.srcbuf->slen;
	if(instance->src8.srcbuf && (instance->src8.srcbuf->slen>instance->slen)) instance->slen=instance->src8.srcbuf->slen;

	{
		// this is a very ugly hack, the cube should realy be a custom boopsi gadget
		UWORD ys1=2;
		UWORD ys2=ys1+1+ANYFRM_HEIGHT(4);
		UWORD xs1=2;
		UWORD xs2=xs1+1+STDFRM_WIDTH;

		DrawWay(instance,modWin->RPort,instance->view,instance->prec,&(instance->mod1),&(instance->mod2),&(instance->mod3),xs2+RunTime->scx[37],ys2+RunTime->scy[18]);
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
			for(i=step;i<instance->slen;i+=step) {
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
			for(i=step;i<instance->slen;i+=step) {
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
			for(i=step;i<instance->slen;i+=step) {
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
			SetAPen(rp,7);
			axis_z=     axis_xs+axis_xd*modfunc1(0,instance->slen,moddata1);
			axis_y=     axis_ys+axis_yd*modfunc2(0,instance->slen,moddata2);
			axis_x=1.0-(axis_zs+axis_zd*modfunc3(0,instance->slen,moddata3));
			rx=100*axis_x+40*axis_z;ry=50*axis_y+20*axis_z;
			Move(rp,xs+RunTime->scx[5+rx],ys+RunTime->scy[75-ry]);
			for(i=step;i<instance->slen;i+=step) {
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
			for(i=step;i<instance->slen;i+=step) {
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
			for(i=step;i<instance->slen;i+=step) {
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

//-- eof ----------------------------------------------------------------------
