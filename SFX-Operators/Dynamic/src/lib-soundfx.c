/****h* Dynamic/lib-soundfx.c [4.3] *
*
*  NAME
*    lib-soundfx.c
*  COPYRIGHT
*    $VER: lib-soundfx.c 4.3 (13.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Dynamic Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    13.Mar.2004	V 4.3	most recent version
*    30.Oct.2001	V 4.2	most recent version
*    29.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define DYNAMIC_SOUNDFX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Dynamic.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

static void DrawCharacteristics(struct Instance *instance,struct RastPort *rp,UWORD x1s,UWORD x1e,UWORD x2s,UWORD x2e,UWORD ys,UWORD ye,UBYTE which);
static void DrawCommon(struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye,double *datax,double *datay);
static void InitCurvePoints(double *datax,double *datay,double rqu,double rlo,double ext);
static void DrawCharacteristicHard(struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye,double *datax,double *datay,double rqu,double rlo);
static void DrawCharacteristicSoft(struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye,double *datax,double *datay);
static void GetHandlePosition(double datax,double datay,UWORD xl,UWORD yl,WORD *x1,WORD *y1,WORD *x2,WORD *y2);
static UBYTE GetHandleIx(double *datax,double *datay,UWORD xs,UWORD ys,UWORD xe,UWORD ye,UWORD mousex,UWORD mousey);

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

#define GADID_KNEE    	(GADID_MOD3+MOD_IDCT)
#define GADIX_KNEE    	(GADIX_MOD3+MOD_IXCT)

#define LAST_GADIX		(GADIX_KNEE+1)

//-- Gfx Size

#define GFX_XRES		140
#define GFX_YRES		 80

#define CHARACTERISTICS_S	0x1
#define CHARACTERISTICS_E	0x2

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	SFXCoreInstanceFields		/* super object */
	PrefsData       moddesc[5];
};

UBYTE *KneeLabels[]=KNEE_LABELS;

// curve points
double dataxs[3],datays[3],dataxe[3],dataye[3];

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
		SFXMod_ModDescInit(4,PD_END         ,0l  ,0l);	
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
	UWORD ys6=ys5+1+ANYFRM_HEIGHT(1);
	UWORD ys7=ys6+1+RunTime->scy[GFX_YRES+5+FRMTITLE_HEIGHT];
	UWORD winh=ys7+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;

	WORD mousex,mousey;
	UBYTE mouseb=FALSE,mouseo=FALSE,knobix=0,which=0;
	UBYTE omptr=MPTR_STANDARD;
	double *datax,*datay;
	double mx,my;
	Param param1,param2,param3;
	UWORD xs,xe;
	UWORD x1s=xs1+RunTime->scx[       5],x2s=xs1+RunTime->scx[307-5-GFX_XRES],ys=ys6+RunTime->scy[FRMCONTENT_YOFFSET];
	UWORD xl =2  +RunTime->scx[GFX_XRES],                                     yl=2  +RunTime->scy[GFX_YRES];
	UWORD x1e=x1s+xl,                    x2e=x2s+xl,                          ye=ys+yl;

	wintags[4].ti_Data|=IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS;
	wintags[5].ti_Data|=WFLG_REPORTMOUSE;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[104];
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
		g=AddCfgSel(&cfgsel,modGadgets,g,GADID_CFGSEL,GADIX_CFGSEL,xs2,ys1,winh-2);

		g=AddSource(&(instance->src),modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		g=AddModulator(&(instance->mod1),modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys2);
		g=AddModulator(&(instance->mod2),modGadgets,g,GADID_MOD2,GADIX_MOD2,xs1,ys3);
		g=AddModulator(&(instance->mod3),modGadgets,g,GADID_MOD3,GADIX_MOD3,xs1,ys4);

		ng.ng_LeftEdge      =xs1+RunTime->scx[37];
		ng.ng_TopEdge       =ys5+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width         =RunTime->scx[90];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText    ="Knee";
		ng.ng_GadgetID      =GADID_KNEE;
		ng.ng_Flags         =PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,KneeLabels,GTCY_Active,instance->knee,TAG_DONE);
		modGadgets[GADIX_KNEE]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+RunTime->scy[FRMTEXT_YPOS(2)]);
			RText(modrp,"Mod",xs1+RunTime->scx[29],ys4+RunTime->scy[FRMTEXT_YPOS(2)]);

			DrawTitle(modrp,"Source"				,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Ratio for loud values"	,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Ratio for quiet values",xs1,ys3,STDFRM_WIDTH);
			DrawTitle(modrp,"Threshold"             ,xs1,ys4,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous"         ,xs1,ys5,STDFRM_WIDTH);
			DrawTitle(modrp,"Characteristics"       ,xs1,ys6,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"				,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);								/* Source */
			DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);								/* Par */
			DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);								/* Par */
			DrawGadWB(modrp,xs1,ys4,xh,MODSEL_HEIGHT);								/* Par */
			DrawGadWB(modrp,xs1,ys5,xh,ANYFRM_HEIGHT(1));			               	/* Misc */
			DrawGadWB(modrp,xs1,ys6,xh,RunTime->scy[GFX_YRES+5+FRMTITLE_HEIGHT]);	/* Gfx */
			DrawGadWB(modrp,xs1,ys7,xh,CMDFRM_HEIGHT);								/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);					/* CfgSel */

			DrawGadBW(modrp,x1s,ys,xl,yl);
			DrawGadBW(modrp,x2s,ys,xl,yl);

			SetCfgSel   (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
			RefreshGUI(instance,modGadgets,modWin);

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
						mousex	=(imsg->MouseX)-(modWin->BorderLeft);
						mousey	=(imsg->MouseY)-(modWin->BorderTop);
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(!HandleSource(&(instance->src),modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)
								&& !HandleModulator(&(instance->mod1),&(instance->src),0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode)
								&& !HandleModulator(&(instance->mod2),&(instance->src),0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode)
								&& !HandleModulator(&(instance->mod3),&(instance->src),0l,modWin,modGadgets,GADID_MOD3,GADIX_MOD3,gnr,icode)
								&& !HandleCfgSel   (&cfgsel       ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_KNEE:
											instance->knee=icode;
											DrawCharacteristics(instance,modrp,x1s,x1e,x2s,x2e,ys,ye,CHARACTERISTICS_S|CHARACTERISTICS_E);
											break;
									}
								}
								else {
									DrawCharacteristics(instance,modrp,x1s,x1e,x2s,x2e,ys,ye,CHARACTERISTICS_S|CHARACTERISTICS_E);
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
							case IDCMP_MOUSEBUTTONS:
								if(icode==SELECTDOWN) {
									mouseb=FALSE;
									// left or right ? => set bounds
									if(mousey>=ys && mousey<=ye) {
										which=0;
										if     (mousex>=x1s && mousex<=x1e) { which=CHARACTERISTICS_S;xs=x1s;xe=x1e;datax=dataxs;datay=datays; }
										else if(mousex>=x2s && mousex<=x2e) { which=CHARACTERISTICS_E;xs=x2s;xe=x2e;datax=dataxe;datay=dataye; }
										if(which) {
											if(knobix=GetHandleIx(datax,datay,xs,ys,xe,ye,mousex,mousey)) mouseb=TRUE;
										}
									}
								}
								if(icode==SELECTUP) {
									mouseb=FALSE;
									DrawCharacteristics(instance,modrp,x1s,x1e,x2s,x2e,ys,ye,which);
								}		/* No break !!! */
							case IDCMP_MOUSEMOVE:
								if(mouseb) {
									if(mousex<xs) mousex=xs;
									else if(mousex>xe) mousex=xe;
									if(mousey<ys) mousey=ys;
									else if(mousey>ye) mousey=ye;
									mx=(double)(mousex-xs)/(double)xl;
									my=(double)(ye-mousey)/(double)yl;
									switch(knobix) {
										case 1: // limmit movement to be below end
											if(mx>=datax[2]) mx=datax[2]-0.0001;
											else if(mx<=datax[0]) mx=datax[0]+0.0001;
											if(my>=datay[2]) my=datay[2]-0.0001;
											else if(my<=datay[0]) my=datay[0]+0.0001;
											sprintf(param1,"%f",((datay[2]-my)/(datax[2]-mx)));	// rlo
											sprintf(param2,"%f",(my          /mx));				// rqu
											sprintf(param3,"%f",mx);							// ext
											switch(which) {
												case CHARACTERISTICS_S:
													UC_ConvertParameter(instance->mod1.vals,param1,UGRP_PROPOTION,PROP_FC);
													UC_ConvertParameter(instance->mod2.vals,param2,UGRP_PROPOTION,PROP_FC);
													UC_ConvertParameter(instance->mod3.vals,param3,UGRP_PROPOTION,PROP_FC);
													break;
												case CHARACTERISTICS_E:
													UC_ConvertParameter(instance->mod1.vale,param1,UGRP_PROPOTION,PROP_FC);
													UC_ConvertParameter(instance->mod2.vale,param2,UGRP_PROPOTION,PROP_FC);
													UC_ConvertParameter(instance->mod3.vale,param3,UGRP_PROPOTION,PROP_FC);
													break;
											}
											SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
											SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
											SetModulator(&(instance->mod3),modWin,modGadgets,GADIX_MOD3);
											break;
										case 2: // limmit movement to left or upper side
											if(mx<=datax[1]) mx=datax[1]+0.0001;
											if(my<=datay[1]) my=datay[1]+0.0001;
											if((1.0-mx)<(1.0-my)) {
												// move up & down
												if(omptr!=MPTR_SIZE82) { SetWindowPointer(modWin,WA_Pointer,RunTime->MPtrSize82->Pointer,TAG_DONE);omptr=MPTR_SIZE82; }
												sprintf(param1,"%f",((my-datay[1])/(1.0-datax[1])));	// rlo
											}
											else {
												// move left & right
												if(omptr!=MPTR_SIZE64) { SetWindowPointer(modWin,WA_Pointer,RunTime->MPtrSize64->Pointer,TAG_DONE);omptr=MPTR_SIZE64; }
												sprintf(param1,"%f",((1.0-datay[1])/(mx-datax[1])));	// rlo
											}													
											switch(which) {
												case CHARACTERISTICS_S:
													UC_ConvertParameter(instance->mod1.vals,param1,UGRP_PROPOTION,PROP_FC);
													break;
												case CHARACTERISTICS_E:
													UC_ConvertParameter(instance->mod1.vale,param1,UGRP_PROPOTION,PROP_FC);
													break;
											}
											SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
											break;
									}
									DrawCharacteristics(instance,modrp,x1s,x1e,x2s,x2e,ys,ye,which);
								}
								else {
									mouseo=FALSE;
									if(mousey>=ys && mousey<=ye) {
										if     (mousex>=x1s && mousex<=x1e) { xs=x1s;xe=x1e;datax=dataxs;datay=datays;mouseo=TRUE; }
										else if(mousex>=x2s && mousex<=x2e) { xs=x2s;xe=x2e;datax=dataxe;datay=dataye;mouseo=TRUE; }
										if(mouseo) {
											knobix=GetHandleIx(datax,datay,xs,ys,xe,ye,mousex,mousey);
											switch(knobix) {
												case 1:
													if(omptr!=MPTR_MOVE) { SetWindowPointer(modWin,WA_Pointer,RunTime->MPtrMove->Pointer,TAG_DONE);omptr=MPTR_MOVE; }
													break;
												case 2:
													if((datax[2]==1.0) && (datay[2]==1.0)) {
														if(omptr!=MPTR_MOVE) { SetWindowPointer(modWin,WA_Pointer,RunTime->MPtrMove->Pointer,TAG_DONE);omptr=MPTR_MOVE; }
													}
													else if(datax[2]==1.0) {
														if(omptr!=MPTR_SIZE82) { SetWindowPointer(modWin,WA_Pointer,RunTime->MPtrSize82->Pointer,TAG_DONE);omptr=MPTR_SIZE82; }
													}
													else {
														if(omptr!=MPTR_SIZE64) { SetWindowPointer(modWin,WA_Pointer,RunTime->MPtrSize64->Pointer,TAG_DONE);omptr=MPTR_SIZE64; }
													}
													break;
												default:
													if(omptr!=MPTR_STANDARD) { SetWindowPointer(modWin,TAG_DONE);omptr=MPTR_STANDARD; }
													break;
											}
										}
									}
									if(!mouseo) {
										if(omptr!=MPTR_STANDARD) { SetWindowPointer(modWin,TAG_DONE);omptr=MPTR_STANDARD; }
									}
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

	UC_ConvertParameter(param,instance->mod1.vals,UGRP_AMPLITUDE,AMP_FC);
	instance->rlos=atof(param);
	if(instance->rlos<0.0) instance->rlos=0.0; 
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_AMPLITUDE,AMP_FC);
	instance->rloe=atof(param);
	if(instance->rloe<0.0) instance->rloe=0.0; 
	instance->rlod=instance->rloe-instance->rlos;

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_AMPLITUDE,AMP_FC);
	instance->rqus=atof(param);
	if(instance->rqus<0.0) instance->rqus=0.0; 
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_AMPLITUDE,AMP_FC);
	instance->rque=atof(param);
	if(instance->rque<0.0) instance->rque=0.0; 
	instance->rqud=instance->rque-instance->rqus;

	UC_ConvertParameter(param,instance->mod3.vals,UGRP_PROPOTION,PROP_FC);
	instance->exts=atof(param);
	if(instance->exts<0.0) instance->exts=0.0;
	if(instance->exts>1.0) instance->exts=1.0;
	UC_ConvertParameter(param,instance->mod3.vale,UGRP_PROPOTION,PROP_FC);
	instance->exte=atof(param);
	if(instance->exte<0.0) instance->exte=0.0;
	if(instance->exte>1.0) instance->exte=1.0;
	instance->extd=instance->exte-instance->exts;

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
						switch(instance->knee) {                                 // je nach wrap-mode
							case KNEE_HARD:                                       // und jetzt berechnen
								for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
									instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
									instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
									process_hard((SFXCoreInstance *)instance,blen);
									SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
								}
								break;
							case KNEE_SOFT:
								for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (instance->curlen<instance->slen));srcsa->seg++,dstsa->seg++) {
									instance->sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
									instance->dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
									process_soft((SFXCoreInstance *)instance,blen);
									SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
								}
								break;
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
		cf=TRUE;	pf=ParseModulatorSet   (&(instance->mod1),"P1",params);
		if(!pf) pf=ParseModulatorSet   (&(instance->mod2),"P2",params);
		if(!pf) pf=ParseModulatorSet   (&(instance->mod3),"P3",params);
		if(!pf && !stricmp(RexxPar2,"Knee"))    { instance->knee  =CfgIO_MatchString(RexxPar3,KneeLabels);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam")) {
		cf=TRUE;	pf=ParseModulatorGet   (&(instance->mod1),"P1",params,ret);
		if(!pf)    ParseModulatorGet   (&(instance->mod2),"P2",params,ret);
		if(!pf)    ParseModulatorGet   (&(instance->mod3),"P3",params,ret);
		if(!pf && !stricmp(RexxPar2,"Knee"))    { sprintf(ret,"%s",KneeLabels[instance->knee]);/*pf=TRUE;*/ }
	}
	SFXMod_HandleARexxCmd_SetOperatorPreset
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,instance->moddesc);
	instance->knee  =CfgIO_MatchString(
					 CfgIO_ReadString           (cfg,"General"  ,"Knee"     ,KneeLabels[KNEE_SOFT]),KneeLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString         (cfg,"General"    ,"Knee"     ,KneeLabels[instance->knee]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	UC_ConvertParameter(instance->mod1.vals,"0.75",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->mod1.vale,"0.75",UGRP_AMPLITUDE,AMP_FC);
	instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';

	UC_ConvertParameter(instance->mod2.vals,"1.5",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(instance->mod2.vale,"1.5",UGRP_AMPLITUDE,AMP_FC);
	instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';

	UC_ConvertParameter(instance->mod3.vals,"0.35",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(instance->mod3.vale,"0.35",UGRP_PROPOTION,PROP_FC);
	instance->mod3.bshape=BSHP_NONE;
	instance->mod3.desc[0]='\0';

	instance->knee=KNEE_SOFT;

}

SFXMod_RecallLastSettings

SFXMod_RecallDefaultSettings

//-- private Stuff ------------------------------------------------------------

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource   (&(instance->src) ,modWin,modGadgets,GADIX_SRC);
	SetModulator(&(instance->mod1),modWin,modGadgets,GADIX_MOD1);
	SetModulator(&(instance->mod2),modWin,modGadgets,GADIX_MOD2);
	SetModulator(&(instance->mod3),modWin,modGadgets,GADIX_MOD3);
	
	{
		// this is a very ugly hack, the slider field should realy be a custom boopsi gadget
		UWORD ys6=2 + 1+SRCSEL_HEIGHT + 3*(1+MODSEL_HEIGHT) + 1+ANYFRM_HEIGHT(1);
		UWORD xs1=2;

		UWORD x1s=xs1+RunTime->scx[       5],x2s=xs1+RunTime->scx[307-5-GFX_XRES],ys=ys6+RunTime->scy[FRMCONTENT_YOFFSET];
		UWORD xl =2  +RunTime->scx[GFX_XRES],                                     yl=2  +RunTime->scy[GFX_YRES];
		UWORD x1e=x1s+xl,                    x2e=x2s+xl,                          ye=ys+yl;

		DrawCharacteristics(instance,modWin->RPort,x1s,x1e,x2s,x2e,ys,ye,CHARACTERISTICS_S|CHARACTERISTICS_E);
	}
}

static void DrawCharacteristics(struct Instance *instance,struct RastPort *rp,UWORD x1s,UWORD x1e,UWORD x2s,UWORD x2e,UWORD ys,UWORD ye,UBYTE which) {
	Param param;

	//MSG("----");

	UC_ConvertParameter(param,instance->mod1.vals,UGRP_AMPLITUDE,AMP_FC);
	instance->rlos=atof(param);
	if(instance->rlos<0.0) instance->rlos=0.0; 
	UC_ConvertParameter(param,instance->mod1.vale,UGRP_AMPLITUDE,AMP_FC);
	instance->rloe=atof(param);
	if(instance->rloe<0.0) instance->rloe=0.0; 
	instance->rlod=instance->rloe-instance->rlos;

	UC_ConvertParameter(param,instance->mod2.vals,UGRP_AMPLITUDE,AMP_FC);
	instance->rqus=atof(param);
	if(instance->rqus<0.0) instance->rqus=0.0; 
	UC_ConvertParameter(param,instance->mod2.vale,UGRP_AMPLITUDE,AMP_FC);
	instance->rque=atof(param);
	if(instance->rque<0.0) instance->rque=0.0; 
	instance->rqud=instance->rque-instance->rqus;

	UC_ConvertParameter(param,instance->mod3.vals,UGRP_PROPOTION,PROP_FC);
	instance->exts=atof(param);
	if(instance->exts<0.0) instance->exts=0.0;
	if(instance->exts>1.0) instance->exts=1.0;
	UC_ConvertParameter(param,instance->mod3.vale,UGRP_PROPOTION,PROP_FC);
	instance->exte=atof(param);
	if(instance->exte<0.0) instance->exte=0.0;
	if(instance->exte>1.0) instance->exte=1.0;
	instance->extd=instance->exte-instance->exts;

	if(which&CHARACTERISTICS_S) InitCurvePoints(dataxs,datays,instance->rqus,instance->rlos,instance->exts);
	if(which&CHARACTERISTICS_E) InitCurvePoints(dataxe,dataye,instance->rque,instance->rloe,instance->exte);
	switch(instance->knee) {
		case KNEE_HARD:
			if(which&CHARACTERISTICS_S) DrawCharacteristicHard(rp,x1s+1,ys+1,x1e-1,ye-1,dataxs,datays,instance->rqus,instance->rlos);
			if(which&CHARACTERISTICS_E) DrawCharacteristicHard(rp,x2s+1,ys+1,x2e-1,ye-1,dataxe,dataye,instance->rque,instance->rloe);
			break;
		case KNEE_SOFT:
			if(which&CHARACTERISTICS_S) DrawCharacteristicSoft(rp,x1s+1,ys+1,x1e-1,ye-1,dataxs,datays);
			if(which&CHARACTERISTICS_E) DrawCharacteristicSoft(rp,x2s+1,ys+1,x2e-1,ye-1,dataxe,dataye);
			break;
	}

}

static void InitCurvePoints(double *datax,double *datay,double rqu,double rlo,double ext) {
	double val,thrx,thry;

	thrx=ext;thry=thrx*rqu;
	if(thry>1.0) {
		thry=1.0;
		thrx=thry/rqu;
		//MSG3("        threshold %6.4f -> %6.4f, %6.4f",ext,thrx,thry);
	}
	//else MSG2("        threshold %6.4f, %6.4f",thrx,thry);

	datax[0]= 0.0;	datay[0]=0.0;
	datax[1]= thrx;	datay[1]=thry;
	val=thry+(1.0-thrx)*rlo;	// end gain
	if(val<=1.0) {
		datax[2]= 1.0;	datay[2]=val;
	}
	else {
		val=thrx+(1.0-thry)/rlo;	// new end gain
		datax[2]=val;	datay[2]=1.0;
	}
}

static void DrawCommon(struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye,double *datax,double *datay) {
	UWORD xl=xe-xs,yl=ye-ys;
	WORD x1,x2,y1,y2;
	//UBYTE col=RunTime->sfxprefs_gui->Pens[PEN_SMP_LINE]^RunTime->sfxprefs_gui->Pens[PEN_SMP_BACK];

	SetAPen(rp,RunTime->sfxprefs_gui->Pens[PEN_SMP_BACK]);
	RectFill(rp,xs,ys,xe,ye);
	SetAPen(rp,RunTime->sfxprefs_gui->Pens[PEN_SMP_GRID]);
	Move(rp,xs,ye-RunTime->scy[GFX_YRES>>1]);Draw(rp,xe,ye-RunTime->scy[GFX_YRES>>1]);
	Move(rp,xe-RunTime->scx[GFX_XRES>>1],ys);Draw(rp,xe-RunTime->scx[GFX_XRES>>1],ye);
	Move(rp,xs,ye);Draw(rp,xe,ys);
	SetAPen(rp,RunTime->sfxprefs_gui->Pens[PEN_SMP_LINE]);
	//SetABPenDrMd(rp,col,col,COMPLEMENT);SetWriteMask(rp,col);

	// middle knob
	GetHandlePosition(datax[1],datay[1],xl,yl,&x1,&y1,&x2,&y2);
	RectFill(rp,xs+x1,ye-y2,xs+x2,ye-y1);

	// end knob
	GetHandlePosition(datax[2],datay[2],xl,yl,&x1,&y1,&x2,&y2);
	RectFill(rp,xs+x1,ye-y2,xs+x2,ye-y1);

}

// version 1 (with this logic we can't simulate hard-knee characteristics, it will always be soft)
/*
	ratio=ival;
	if((ratio!=0.0) && (ext!=1.0)) ratio=mypow(ratio,ext);
	ratio=rqu+(ratio*(rlo-rqu));
	oval=ival*ratio;
*/


static void DrawCharacteristicHard(struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye,double *datax,double *datay,double rqu,double rlo) {
	register UWORD x,y;
	UWORD xl=xe-xs,yl=ye-ys;
	double ival,oval;

	DrawCommon(rp,xs,ys,xe,ye,datax,datay);

	Move(rp,xs,ye);
	for(x=0;x<xl;x++) {
		ival=(double)x/(double)xl;

		if(ival<datax[1]) {	// apply rqu
			oval=ival*rqu;
		}
		else {			// apply rlo
			oval=datay[1]+((ival-datax[1])*rlo);
		}

		y=(UWORD)((double)yl*oval);
		if(y>yl) y=yl;
		Draw(rp,xs+x,ye-y);
	}
}

static void DrawCharacteristicSoft(struct RastPort *rp,UWORD xs,UWORD ys,UWORD xe,UWORD ye,double *datax,double *datay) {
	register UWORD x,y;
	UWORD xl=xe-xs,yl=ye-ys;
	double t0,t1;
	double midx,midy;

	DrawCommon(rp,xs,ys,xe,ye,datax,datay);

	Move(rp,xs,ye);
	for(t0=0.0;t0<(1.0+(1/200.0));t0+=(1/200.0)) {
		// bezier with 3 control points
		t1=1.0-t0;
		midx=datax[0]*t1*t1 + datax[1]*2.0*t0*t1 + datax[2]*t0*t0;
		midy=datay[0]*t1*t1 + datay[1]*2.0*t0*t1 + datay[2]*t0*t0;

		x=(UWORD)((double)xl*midx);
		y=(UWORD)((double)yl*midy);if(y>yl) y=yl;
		Draw(rp,xs+x,ye-y);
	}
}

static void GetHandlePosition(double datax,double datay,UWORD xl,UWORD yl,WORD *x1,WORD *y1,WORD *x2,WORD *y2) {
	WORD p;

	p=(WORD)(xl*datax)-3;*x1=max(p,0);p+=6;*x2=min(p,(WORD)xl);
	p=(WORD)(yl*datay)-2;*y1=max(p,0);p+=4;*y2=min(p,(WORD)yl);
}

static UBYTE GetHandleIx(double *datax,double *datay,UWORD xs,UWORD ys,UWORD xe,UWORD ye,UWORD mousex,UWORD mousey) {
	register UBYTE i;
	UWORD xl=xe-xs,yl=ye-ys;
	WORD x1,x2,y1,y2;
	UBYTE ix=0;

	for(i=1;i<3;i++) {
		GetHandlePosition(datax[i],datay[i],xl,yl,&x1,&y1,&x2,&y2);
		if(mousex>=xs+x1 && mousex<=xs+x2 && mousey>=ye-y2 && mousey<=ye-y1) {
			ix=i;break;
		}
	}
	return(ix);
}

//-- eof ----------------------------------------------------------------------
