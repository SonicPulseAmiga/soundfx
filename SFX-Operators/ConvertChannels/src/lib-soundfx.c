/****h* ConvertChannels/lib-soundfx.c [4.2] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.2 (15.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    ConvertChannels Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    16.Jul.1998
*  MODIFICATION HISTORY
*    15.May.2002	V 4.2	most recent version
*    16.Apr.2002	V 0.0	most recent version
*    11.Feb.2002	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    01.Aug.2000	V 1.20	most recent version
*    16.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define CONVERTCHANNELS_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Convertchannels.h"

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

#define GADID_MAT0_0	(GADID_SRC+SRC_IDCT)
#define GADIX_MAT0_0	(GADIX_SRC+SRC_IXCT)

#define GADID_MAT0_1	(GADID_MAT0_0+1)
#define GADIX_MAT0_1	(GADIX_MAT0_0+1)

#define GADID_MAT0_2	(GADID_MAT0_1+1)
#define GADIX_MAT0_2	(GADIX_MAT0_1+1)

#define GADID_MAT0_3	(GADID_MAT0_2+1)
#define GADIX_MAT0_3	(GADIX_MAT0_2+1)

#define GADID_MAT1_0	(GADID_MAT0_3+1)
#define GADIX_MAT1_0	(GADIX_MAT0_3+1)

#define GADID_MAT1_1	(GADID_MAT1_0+1)
#define GADIX_MAT1_1	(GADIX_MAT1_0+1)

#define GADID_MAT1_2	(GADID_MAT1_1+1)
#define GADIX_MAT1_2	(GADIX_MAT1_1+1)

#define GADID_MAT1_3	(GADID_MAT1_2+1)
#define GADIX_MAT1_3	(GADIX_MAT1_2+1)

#define GADID_MAT2_0	(GADID_MAT1_3+1)
#define GADIX_MAT2_0	(GADIX_MAT1_3+1)

#define GADID_MAT2_1	(GADID_MAT2_0+1)
#define GADIX_MAT2_1	(GADIX_MAT2_0+1)

#define GADID_MAT2_2	(GADID_MAT2_1+1)
#define GADIX_MAT2_2	(GADIX_MAT2_1+1)

#define GADID_MAT2_3	(GADID_MAT2_2+1)
#define GADIX_MAT2_3	(GADIX_MAT2_2+1)

#define GADID_MAT3_0	(GADID_MAT2_3+1)
#define GADIX_MAT3_0	(GADIX_MAT2_3+1)

#define GADID_MAT3_1	(GADID_MAT3_0+1)
#define GADIX_MAT3_1	(GADIX_MAT3_0+1)

#define GADID_MAT3_2	(GADID_MAT3_1+1)
#define GADIX_MAT3_2	(GADIX_MAT3_1+1)

#define GADID_MAT3_3	(GADID_MAT3_2+1)
#define GADIX_MAT3_3	(GADIX_MAT3_2+1)

#define LAST_GADIX		(GADIX_MAT3_3+1)

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
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(4)+RunTime->scy[8+8];
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh,xh2,yh,yh2;

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

		ng.ng_LeftEdge		=xs1+RunTime->scx[60];				// row 0
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[60];
		ng.ng_GadgetText	="0";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_MAT0_0;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[0][0],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT0_0]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="1";
		ng.ng_GadgetID		=GADID_MAT0_1;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[0][1],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT0_1]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_GadgetText	="2";
		ng.ng_GadgetID		=GADID_MAT0_2;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[0][2],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT0_2]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+36];
		ng.ng_GadgetText	="3";
		ng.ng_GadgetID		=GADID_MAT0_3;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[0][3],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT0_3]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[120];				// row 1
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_MAT1_0;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[1][0],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT1_0]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetID		=GADID_MAT1_1;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[1][1],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT1_1]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_GadgetID		=GADID_MAT1_2;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[1][2],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT1_2]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+36];
		ng.ng_GadgetID		=GADID_MAT1_3;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[1][3],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT1_3]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[180];				// row 2
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_GadgetID		=GADID_MAT2_0;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[2][0],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT2_0]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetID		=GADID_MAT2_1;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[2][1],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT2_1]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_GadgetID		=GADID_MAT2_2;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[2][2],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT2_2]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+36];
		ng.ng_GadgetID		=GADID_MAT2_3;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[2][3],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT2_3]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[240];				// row 3
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_GadgetID		=GADID_MAT3_0;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[3][0],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT3_0]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetID		=GADID_MAT3_1;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[3][1],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT3_1]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_GadgetID		=GADID_MAT3_2;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[3][2],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT3_2]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+36];
		ng.ng_GadgetID		=GADID_MAT3_3;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->mat[3][3],TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_MAT3_3]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			Move(modrp,xs1+RunTime->scx[ 65],ys2+RunTime->scy[FRMTEXT_YPOS(4)+18]);Text(modrp,"Dst",3);
			Move(modrp,xs1+RunTime->scx[ 65],ys2+RunTime->scy[FRMTEXT_YPOS(4)+10]);Text(modrp,"0",1);
			Move(modrp,xs1+RunTime->scx[125],ys2+RunTime->scy[FRMTEXT_YPOS(4)+10]);Text(modrp,"1",1);
			Move(modrp,xs1+RunTime->scx[185],ys2+RunTime->scy[FRMTEXT_YPOS(4)+10]);Text(modrp,"2",1);
			Move(modrp,xs1+RunTime->scx[245],ys2+RunTime->scy[FRMTEXT_YPOS(4)+10]);Text(modrp,"3",1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(4)]);

			DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Matrix"		,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);						/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(4)+RunTime->scy[8+8]);	/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);						/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);			/* CfgSel */

			yh=ys2+RunTime->scy[FRMCONTENT_YOFFSET];yh2=RunTime->scy[4*12];
			xh=xs1+RunTime->scx[2];					xh2=RunTime->scx[ 60]-xs1;
			DrawGadBW(modrp,xh    ,yh    ,xh2-RunTime->scx[2],yh2-RunTime->scy[2]);		/* Matrix-Src */
			DrawGadBW(modrp,xh+xh2,yh+yh2,RunTime->scx[240]  ,RunTime->scy[16-1]);		/* Matrix-Dst */

			SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
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
								if(!HandleSource(&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)
								&& !HandleCfgSel(&cfgsel      ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_MAT0_0: HandleParameter(instance->mat[0][0],modWin,modGadgets[GADIX_MAT0_0]);break;
										case GADID_MAT0_1: HandleParameter(instance->mat[0][1],modWin,modGadgets[GADIX_MAT0_1]);break;
										case GADID_MAT0_2: HandleParameter(instance->mat[0][2],modWin,modGadgets[GADIX_MAT0_2]);break;
										case GADID_MAT0_3: HandleParameter(instance->mat[0][3],modWin,modGadgets[GADIX_MAT0_3]);break;
										case GADID_MAT1_0: HandleParameter(instance->mat[1][0],modWin,modGadgets[GADIX_MAT1_0]);break;
										case GADID_MAT1_1: HandleParameter(instance->mat[1][1],modWin,modGadgets[GADIX_MAT1_1]);break;
										case GADID_MAT1_2: HandleParameter(instance->mat[1][2],modWin,modGadgets[GADIX_MAT1_2]);break;
										case GADID_MAT1_3: HandleParameter(instance->mat[1][3],modWin,modGadgets[GADIX_MAT1_3]);break;
										case GADID_MAT2_0: HandleParameter(instance->mat[2][0],modWin,modGadgets[GADIX_MAT2_0]);break;
										case GADID_MAT2_1: HandleParameter(instance->mat[2][1],modWin,modGadgets[GADIX_MAT2_1]);break;
										case GADID_MAT2_2: HandleParameter(instance->mat[2][2],modWin,modGadgets[GADIX_MAT2_2]);break;
										case GADID_MAT2_3: HandleParameter(instance->mat[2][3],modWin,modGadgets[GADIX_MAT2_3]);break;
										case GADID_MAT3_0: HandleParameter(instance->mat[3][0],modWin,modGadgets[GADIX_MAT3_0]);break;
										case GADID_MAT3_1: HandleParameter(instance->mat[3][1],modWin,modGadgets[GADIX_MAT3_1]);break;
										case GADID_MAT3_2: HandleParameter(instance->mat[3][2],modWin,modGadgets[GADIX_MAT3_2]);break;
										case GADID_MAT3_3: HandleParameter(instance->mat[3][3],modWin,modGadgets[GADIX_MAT3_3]);break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB:
										ActivateGadget(modGadgets[GADIX_MAT0_0],modWin,0l);
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
	SmpAccess *srcsa[4]={NULL},*dstsa[4]={NULL};
	SAFuncPtr SARead,SAWrite;
	ULONG blen;
	//-- administrative
	Param param;
	UBYTE dstch;
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

	for(l=0;l<4;l++) { UC_ConvertParameter(param,instance->mat[0][l],UGRP_AMPLITUDE,AMP_FC);instance->mat0[l]=atof(param); }
	for(l=0;l<4;l++) { UC_ConvertParameter(param,instance->mat[1][l],UGRP_AMPLITUDE,AMP_FC);instance->mat1[l]=atof(param); }
	for(l=0;l<4;l++) { UC_ConvertParameter(param,instance->mat[2][l],UGRP_AMPLITUDE,AMP_FC);instance->mat2[l]=atof(param); }
	for(l=0;l<4;l++) { UC_ConvertParameter(param,instance->mat[3][l],UGRP_AMPLITUDE,AMP_FC);instance->mat3[l]=atof(param); }

	instance->srcch=srcbuf->channels;
	dstch=255;
	if((instance->mat0[0]!=0.0) || (instance->mat0[1]!=0.0) || (instance->mat0[2]!=0.0) || (instance->mat0[3]!=0.0)) dstch=CH_MONO;
	if((instance->mat1[0]!=0.0) || (instance->mat1[1]!=0.0) || (instance->mat1[2]!=0.0) || (instance->mat1[3]!=0.0)) dstch=CH_STEREO;
	if((instance->mat2[0]!=0.0) || (instance->mat2[1]!=0.0) || (instance->mat2[2]!=0.0) || (instance->mat2[3]!=0.0)) dstch=CH_QUADRO;
	if((instance->mat3[0]!=0.0) || (instance->mat3[1]!=0.0) || (instance->mat3[2]!=0.0) || (instance->mat3[3]!=0.0)) dstch=CH_QUADRO;

	if(dstch!=255) {
		if(!dstbuf) {
			if(dstbuf=NewSample()) {
				if(AllocSampleMem(dstbuf,srcbuf->slen,CH_COUNT(dstch))) {
					BufferFill(srcbuf,dstbuf);
					dstbuf->channels=dstch;
					dstbuf->aktch=CH_COUNT(dstch);
					dstbuf->channelmask=CH_MASK(dstch);
					newdst=TRUE;
				}
				else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
			}
			else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);return(FALSE); }
		}
		for(l=0;((l<=instance->srcch) && okay);l++) {
			if(!(srcsa[l]=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) okay=FALSE;	// sequentiell Blöcke konstanter Größe lesen
			else srcsa[l]->seg=0;
		}
		for(l=0;((l<=dstch) && okay);l++) {
			if(!(dstsa[l]=SAInit(dstbuf,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) okay=FALSE;	// sequentiell Blöcke konstanter Größe schreiben
			else dstsa[l]->seg=0;
		}
		if(okay) {
			if(pwd=NewProWin()) {
				if(activate((SFXCoreInstance *)instance)) {
					activate_ch((SFXCoreInstance *)instance,0);
					if(rm_start>0) for(l=0;l<CH_COUNT(dstch);l++) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);    // Bereich von 0...Bereichsstart kopieren
					SARead =srcsa[0]->safunc;
					SAWrite=dstsa[0]->safunc;
					blen=Bytes2Samples(srcsa[0]->blen);
					sprintf(title,GetString(msgProgress),PRJ_NAME,1,1);
					StartProWin(pwd,(ULONG *)(&instance->curlen),title,rm_len);					  // den Statusbalken starten
					switch(dstch) {
						case CH_MONO:		// to mono
							while((srcsa[0]->seg<srcsa[0]->seganz) && (instance->curlen<instance->slen)) {
								for(l=0;l<=instance->srcch;l++) { instance->sbuf[l]=(SAMPLE *)srcsa[l]->buffer1;SARead(srcsa[l],srcsa[l]->blen);srcsa[l]->seg++; }
								for(l=0;l<1;l++)	instance->dbuf[l]=(SAMPLE *)dstsa[l]->buffer1;
								process_m((SFXCoreInstance *)instance,blen);
								for(l=0;l<1;l++)	{ SAWrite(dstsa[l],dstsa[l]->blen);dstsa[l]->seg++; }
							}
							break;
						case CH_STEREO:		// to stereo
							while((srcsa[0]->seg<srcsa[0]->seganz) && (instance->curlen<instance->slen)) {
								for(l=0;l<=instance->srcch;l++) { instance->sbuf[l]=(SAMPLE *)srcsa[l]->buffer1;SARead(srcsa[l],srcsa[l]->blen);srcsa[l]->seg++; }
								for(l=0;l<2;l++)	instance->dbuf[l]=(SAMPLE *)dstsa[l]->buffer1;
								process_s((SFXCoreInstance *)instance,blen);
								for(l=0;l<2;l++)	{ SAWrite(dstsa[l],dstsa[l]->blen);dstsa[l]->seg++; }
							}
						case CH_QUADRO:		// to quattro
							while((srcsa[0]->seg<srcsa[0]->seganz) && (instance->curlen<instance->slen)) {
								for(l=0;l<=instance->srcch;l++) { instance->sbuf[l]=(SAMPLE *)srcsa[l]->buffer1;SARead(srcsa[l],srcsa[l]->blen);srcsa[l]->seg++; }
								for(l=0;l<4;l++)	instance->dbuf[l]=(SAMPLE *)dstsa[l]->buffer1;
								process_q((SFXCoreInstance *)instance,blen);
								for(l=0;l<4;l++)	{ SAWrite(dstsa[l],dstsa[l]->blen);dstsa[l]->seg++; }
							}
							break;
					}
					if(rm_start+rm_len<srcbuf->slen) for(l=0;l<CH_COUNT(dstch);l++) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
					deactivate_ch((SFXCoreInstance *)instance,0);
					if(ProWinAborted(instance->curlen)) { okay=FALSE; }					// es wurde abgebrochen
				}
				else okay=FALSE;
				deactivate((SFXCoreInstance *)instance);
				DelProWin(pwd);
			}
			else okay=FALSE;
		}
		for(l=0;l<=instance->srcch;l++) if(srcsa[l]) SADone(srcsa[l],srcbuf);
		for(l=0;l<=dstch;l++) if(dstsa[l]) SADone(dstsa[l],srcbuf);
		if(okay && newdst) AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);
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
	char *param;

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam")) {
		cf=TRUE;pf=0;
		if(       !stricmp(RexxPar2,"Mat"))			{ param=instance->mat[atoi(RexxPar3)][atoi(RexxPar4)];strntcpy(param,RexxPar5,PARBUFLEN);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;/*pf=0;*/
		if(       !stricmp(RexxPar2,"Mat"))			{ param=instance->mat[atoi(RexxPar3)][atoi(RexxPar4)];strntcpy(ret,param,PARBUFLEN);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	strcpy(instance->mat[0][0],CfgIO_ReadString			(cfg,"General"	,"Mat00"	,"1.0"));
	strcpy(instance->mat[0][1],CfgIO_ReadString			(cfg,"General"	,"Mat01"	,"0.0"));
	strcpy(instance->mat[0][2],CfgIO_ReadString			(cfg,"General"	,"Mat02"	,"0.0"));
	strcpy(instance->mat[0][3],CfgIO_ReadString			(cfg,"General"	,"Mat03"	,"0.0"));
	strcpy(instance->mat[1][0],CfgIO_ReadString			(cfg,"General"	,"Mat10"	,"0.0"));
	strcpy(instance->mat[1][1],CfgIO_ReadString			(cfg,"General"	,"Mat11"	,"1.0"));
	strcpy(instance->mat[1][2],CfgIO_ReadString			(cfg,"General"	,"Mat12"	,"0.0"));
	strcpy(instance->mat[1][3],CfgIO_ReadString			(cfg,"General"	,"Mat13"	,"0.0"));
	strcpy(instance->mat[2][0],CfgIO_ReadString			(cfg,"General"	,"Mat20"	,"0.0"));
	strcpy(instance->mat[2][1],CfgIO_ReadString			(cfg,"General"	,"Mat21"	,"0.0"));
	strcpy(instance->mat[2][2],CfgIO_ReadString			(cfg,"General"	,"Mat22"	,"1.0"));
	strcpy(instance->mat[2][3],CfgIO_ReadString			(cfg,"General"	,"Mat23"	,"0.0"));
	strcpy(instance->mat[3][0],CfgIO_ReadString			(cfg,"General"	,"Mat30"	,"0.0"));
	strcpy(instance->mat[3][1],CfgIO_ReadString			(cfg,"General"	,"Mat31"	,"0.0"));
	strcpy(instance->mat[3][2],CfgIO_ReadString			(cfg,"General"	,"Mat32"	,"0.0"));
	strcpy(instance->mat[3][3],CfgIO_ReadString			(cfg,"General"	,"Mat33"	,"1.0"));
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Mat00"	,instance->mat[0][0]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat01"	,instance->mat[0][1]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat02"	,instance->mat[0][2]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat03"	,instance->mat[0][3]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat10"	,instance->mat[1][0]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat11"	,instance->mat[1][1]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat12"	,instance->mat[1][2]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat13"	,instance->mat[1][3]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat20"	,instance->mat[2][0]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat21"	,instance->mat[2][1]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat22"	,instance->mat[2][2]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat23"	,instance->mat[2][3]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat30"	,instance->mat[3][0]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat31"	,instance->mat[3][1]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat32"	,instance->mat[3][2]);
	CfgIO_WriteString		  (cfg,"General"	,"Mat33"	,instance->mat[3][3]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	strcpy(instance->mat[0][0],"1.0");strcpy(instance->mat[1][0],"0.0");strcpy(instance->mat[2][0],"0.0");strcpy(instance->mat[3][0],"0.0");
	strcpy(instance->mat[0][1],"0.0");strcpy(instance->mat[1][1],"1.0");strcpy(instance->mat[2][1],"0.0");strcpy(instance->mat[3][1],"0.0");
	strcpy(instance->mat[0][2],"0.0");strcpy(instance->mat[1][2],"0.0");strcpy(instance->mat[2][2],"1.0");strcpy(instance->mat[3][2],"0.0");
	strcpy(instance->mat[0][3],"0.0");strcpy(instance->mat[1][3],"0.0");strcpy(instance->mat[2][3],"0.0");strcpy(instance->mat[3][3],"1.0");
}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src),modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT0_0],modWin,0l,GTST_String,instance->mat[0][0],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT0_1],modWin,0l,GTST_String,instance->mat[0][1],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT0_2],modWin,0l,GTST_String,instance->mat[0][2],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT0_3],modWin,0l,GTST_String,instance->mat[0][3],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT1_0],modWin,0l,GTST_String,instance->mat[1][0],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT1_1],modWin,0l,GTST_String,instance->mat[1][1],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT1_2],modWin,0l,GTST_String,instance->mat[1][2],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT1_3],modWin,0l,GTST_String,instance->mat[1][3],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT2_0],modWin,0l,GTST_String,instance->mat[2][0],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT2_1],modWin,0l,GTST_String,instance->mat[2][1],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT2_2],modWin,0l,GTST_String,instance->mat[2][2],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT2_3],modWin,0l,GTST_String,instance->mat[2][3],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT3_0],modWin,0l,GTST_String,instance->mat[3][0],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT3_1],modWin,0l,GTST_String,instance->mat[3][1],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT3_2],modWin,0l,GTST_String,instance->mat[3][2],TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MAT3_3],modWin,0l,GTST_String,instance->mat[3][3],TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
