/****h* sfxsupport.library/fxobj-mod.c [4.3] *
*
*  NAME
*    fxobj-mod.c
*  COPYRIGHT
*    $VER: fxobj-mod.c 4.3 (23.11.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    modulator object - definitions
*  AUTHOR
*    Stefan Kost            sk      kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    23.Nov.2003	V 4.3	most recent version
*    26.Jun.2001    V 4.2   most recent version
*    25.Apr.2001    V 4.1   most recent version
*    27.Apr.1999    V 1.20  most recent version
*    18.Sep.1998    V 1.20  initial version
*  NOTES
*
*******
*/

#define FXOBJ_MOD_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- GUI Handling

struct Gadget * ASM LIB_EXPORT(AddModulator(REG(a0,Modulator *mod),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y)));
void            ASM LIB_EXPORT(SetModulator(REG(a0,Modulator *mod),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
UBYTE           ASM LIB_EXPORT(HandleModulator(REG(a0,Modulator *mod),REG(a1,Source *src1),REG(a2,Source *src2),REG(a3,struct Window *win),REG(a4,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode)));

//-- ARexx Parsing

UBYTE           ASM LIB_EXPORT(ParseModulatorSet(REG(a0,Modulator *mod),REG(a1,STRPTR prefix),REG(a2,STRPTR params)));
UBYTE           ASM LIB_EXPORT(ParseModulatorGet(REG(a0,Modulator *mod),REG(a1,STRPTR prefix),REG(a2,STRPTR params),REG(a3,STRPTR ret)));

//-- defines ------------------------------------------------------------------

#define GADID_VALS       0
#define GADIX_VALS       0
#define GADID_VALE       (GADID_VALS+1)
#define GADIX_VALE       (GADIX_VALS+1)
#define GADID_SWAP       (GADID_VALE+1)
#define GADID_MODSEL     (GADID_SWAP+1)
#define GADIX_MODSEL     (GADIX_VALE+1)
#define GADID_BSHAPE     (GADID_MODSEL+1)
#define GADIX_BSHAPE     (GADIX_MODSEL+1)
#define GADID_MODDESC    (GADID_BSHAPE+1)
#define GADIX_MODDESC    (GADIX_BSHAPE+1)

//-- globals ------------------------------------------------------------------

UBYTE *BShapeLabels[]={ "None","Curve","Cycle","Vector","User",0l };

//-- definitions --------------------------------------------------------------

//-- GUI Handling

struct Gadget * SAVEDS ASM LIB_EXPORT(AddModulator(REG(a0,Modulator *mod),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y))) {
	struct NewGadget ng;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(mod,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(g,__FUNC__,"3.par",4));

	ng.ng_LeftEdge      =x+GetRunTime(scx[37]);
	ng.ng_TopEdge       =y+GetRunTime(scy[10]);
	ng.ng_Width         =GetRunTime(scx[117]);
	ng.ng_Height        =GetRunTime(scy[11]);
	ng.ng_TextAttr      =GetRunTime(ScrFont);
	ng.ng_GadgetText    ="Par";
	ng.ng_GadgetID      =firstid+GADID_VALS;
	ng.ng_Flags         =PLACETEXT_LEFT;
	ng.ng_VisualInfo    =GetRunTime(VisualInfo);
	g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,mod->vals,TAG_DONE);
	((struct StringInfo*)g->SpecialInfo)->Extension=GetRunTime(stringextend);
	g->Activation|=GACT_STRINGEXTEND;
	gad[firstix+GADIX_VALS]=g;

	ng.ng_LeftEdge      =x+GetRunTime(scx[187]);
	ng.ng_GadgetText    ="";
	ng.ng_GadgetID      =firstid+GADID_VALE;
	g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,mod->vale,TAG_DONE);
	((struct StringInfo*)g->SpecialInfo)->Extension=GetRunTime(stringextend);
	g->Activation|=GACT_STRINGEXTEND;
	gad[firstix+GADIX_VALE]=g;

	ng.ng_LeftEdge      =x+GetRunTime(scx[155]);
	ng.ng_Width         =GetRunTime(scx[31]);
	ng.ng_GadgetText    ="<->";
	ng.ng_GadgetID      =firstid+GADID_SWAP;
	ng.ng_Flags         =PLACETEXT_IN;
	g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

	ng.ng_LeftEdge      =x+GetRunTime(scx[37]);
	ng.ng_TopEdge       =y+GetRunTime(scy[22]);
	ng.ng_Width         =GetRunTime(scx[15]);	/* max(GetRunTime(scx[15]),(GetRunTime(SigFont)->tf_XSize+4)); */
	ng.ng_TextAttr      =GetRunTime(SigFont);
	ng.ng_GadgetText    =IMAGE_PopUp;
	ng.ng_GadgetID      =firstid+GADID_MODSEL;
	ng.ng_Flags         =PLACETEXT_IN;
	g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
	gad[firstix+GADIX_MODSEL]=g;

	ng.ng_LeftEdge      =x+GetRunTime(scx[53]);
	ng.ng_Width         =GetRunTime(scx[90]);
	ng.ng_TextAttr      =GetRunTime(ScrFont);
	ng.ng_GadgetText    ="";
	ng.ng_GadgetID      =firstid+GADID_BSHAPE;
	ng.ng_Flags         =PLACETEXT_LEFT;
	g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,BShapeLabels,GTCY_Active,mod->bshape,TAG_DONE);
	gad[firstix+GADIX_BSHAPE]=g;

	ng.ng_LeftEdge      =x+GetRunTime(scx[144]);
	ng.ng_Width         =GetRunTime(scx[160]);
	ng.ng_GadgetText    ="";
	ng.ng_GadgetID      =firstid+GADID_MODDESC;
	g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,0l,GTTX_CopyText,TRUE,TAG_DONE);
	gad[firstix+GADIX_MODDESC]=g;

	return(g);
}

void SAVEDS ASM LIB_EXPORT(SetModulator(REG(a0,Modulator *mod),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(mod,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"3.par",4));

	GT_SetGadgetAttrs(gad[firstix+GADIX_VALS   ],win,0l,GTST_String,mod->vals  ,TAG_DONE);
	if(mod->bshape) GT_SetGadgetAttrs(gad[firstix+GADIX_VALE   ],win,0l,GTST_String,mod->vale  ,GA_Disabled,FALSE,TAG_DONE);
	else            GT_SetGadgetAttrs(gad[firstix+GADIX_VALE   ],win,0l,GTST_String,mod->vale  ,GA_Disabled,TRUE ,TAG_DONE);
	GT_SetGadgetAttrs(gad[firstix+GADIX_BSHAPE ],win,0l,GTCY_Active,mod->bshape,TAG_DONE);
	GT_SetGadgetAttrs(gad[firstix+GADIX_MODDESC],win,0l,GTTX_Text  ,mod->desc  ,TAG_DONE);
}

UBYTE SAVEDS ASM LIB_EXPORT(HandleModulator(REG(a0,Modulator *mod),REG(a1,Source *src1),REG(a2,Source *src2),REG(a3,struct Window *win),REG(a4,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode))) {
	Param param;
	UBYTE ret=0;
	ULONG bshape;
	ModCurve *mcu;
	ModCycle *mcy;
	ModVector *mve;
	ModUser *mus;
	struct Requester NullReq;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(mod,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(src1,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER(src2,__FUNC__,"3.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"4.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"5.par",4));

	switch(id-firstid) {
		case GADID_VALS:
			ret=1;
			LIB_INTCALL(HandleParameter(mod->vals,win,gad[firstix+GADIX_VALS]));
			break;
		case GADID_VALE:
			ret=1;
			LIB_INTCALL(HandleParameter(mod->vale,win,gad[firstix+GADIX_VALE]));
			break;
		case GADID_SWAP:
			ret=1;
			strncpy(param,mod->vals,PARBUFLEN);
			strncpy(mod->vals,mod->vale,PARBUFLEN);
			strncpy(mod->vale,param,PARBUFLEN);
			GT_SetGadgetAttrs(gad[firstix+GADIX_VALS],win,0l,GTST_String,mod->vals,TAG_DONE);
			GT_SetGadgetAttrs(gad[firstix+GADIX_VALE],win,0l,GTST_String,mod->vale,TAG_DONE);
			break;
		case GADID_BSHAPE:
			ret=1;
			break;
		case GADID_MODSEL:
			ret=1;
			GT_GetGadgetAttrs(gad[firstix+GADIX_BSHAPE],win,0l,GTCY_Active,&bshape,TAG_DONE);       // use this because it's the popupbutton
			if(mod->bshape!=bshape) {                                                               // which triggered this method
				//-- free old data
				switch(mod->bshape) {
					//case BSHP_NONE: break;
					//case BSHP_CURVE: break;
					//case BSHP_CYCLE: break;
					//case BSHP_VECTOR: break;
					case BSHP_USER:
						mus=(ModUser *)(mod->bshpdata);
						LIB_INTCALL(UnlockBuffer(mus->modbuf));
						if(mus->envdata) FreeVec(mus->envdata);
						break;
				}
				if(mod->bshpdata) { FreeVec(mod->bshpdata);mod->bshpdata=NULL; }
				//-- init new data
				mod->bshape=bshape;
				switch(bshape) {
					case BSHP_NONE: break;
					case BSHP_CURVE:
						if((mod->bshpdata=AllocVec(sizeof(ModCurve),MEMF_ANY|MEMF_CLEAR))) {
							mcu=(ModCurve *)(mod->bshpdata);
							mcu->exp=1.0;
						}
						else GT_SetGadgetAttrs(gad[firstix+GADIX_BSHAPE],win,0l,GTCY_Active,(mod->bshape=BSHP_NONE),TAG_DONE);
						break;
					case BSHP_CYCLE:
						if((mod->bshpdata=AllocVec(sizeof(ModCycle),MEMF_ANY|MEMF_CLEAR))) {
							mcy=(ModCycle *)(mod->bshpdata);
							mcy->oszi=MC_OSZI_SAW;
							UC_ConvertParameter(mcy->frq,"1.0 hz",UGRP_ABSFREQUENCY,AFRQ_HZ);
							UC_ConvertParameter(mcy->pha,"0.0 °",UGRP_PHASE,PHA_D);
						}
						else GT_SetGadgetAttrs(gad[firstix+GADIX_BSHAPE],win,0l,GTCY_Active,(mod->bshape=BSHP_NONE),TAG_DONE);
						break;
					case BSHP_VECTOR:
						if((mod->bshpdata=AllocVec(sizeof(ModVector),MEMF_ANY|MEMF_CLEAR))) {
							mve=(ModVector *)(mod->bshpdata);
							mve->anz=11;
							mve->pos[ 0]=0.00;mve->lev[ 0]=0.00;    /* Start */
							mve->pos[ 1]=0.01;mve->lev[ 1]=0.70;    /* Attack */
							mve->pos[ 2]=0.05;mve->lev[ 2]=1.00;
							mve->pos[ 3]=0.25;mve->lev[ 3]=1.00;
							mve->pos[ 4]=0.32;mve->lev[ 4]=0.60;    /* Decay */
							mve->pos[ 5]=0.41;mve->lev[ 5]=0.65;
							mve->pos[ 6]=0.55;mve->lev[ 6]=0.50;
							mve->pos[ 7]=0.70;mve->lev[ 7]=0.50;    /* Sustain */
							mve->pos[ 8]=0.82;mve->lev[ 8]=0.55;    /* Relase */
							mve->pos[ 9]=0.90;mve->lev[ 9]=0.10;
							mve->pos[10]=1.00;mve->lev[10]=0.00;    /* End */
						}
						else GT_SetGadgetAttrs(gad[firstix+GADIX_BSHAPE],win,0l,GTCY_Active,(mod->bshape=BSHP_NONE),TAG_DONE);
						break;
					case BSHP_USER:
						if((mod->bshpdata=AllocVec(sizeof(ModUser),MEMF_ANY|MEMF_CLEAR))) {
							mus=(ModUser *)(mod->bshpdata);
							mus->type   =MU_TYPE_NORMAL;
							mus->bmode  =MU_BMOD_REPEAT;
							//mus->env    =0;
							//mus->modid  =0l;
							mus->modbuf =LIB_INTCALL(LockBuffer(SetValidBuffer(&mus->modid)));
						}
						else GT_SetGadgetAttrs(gad[firstix+GADIX_BSHAPE],win,0l,GTCY_Active,(mod->bshape=BSHP_NONE),TAG_DONE);
						break;
				}
			}
			InitRequester(&NullReq);Request(&NullReq,win);SetWindowPointer(win,WA_BusyPointer,TRUE,TAG_DONE);
			switch(bshape) {
				case BSHP_NONE:     ShowWin_ModNone(mod,src1,src2);break;
				case BSHP_CURVE:    ShowWin_ModCurve(mod,src1,src2);break;
				case BSHP_CYCLE:    ShowWin_ModCycle(mod,src1,src2);break;
				case BSHP_VECTOR:   ShowWin_ModVector(mod,src1,src2);break;
				case BSHP_USER:     ShowWin_ModUser(mod,src1,src2);break;
			}
			EndRequest(&NullReq,win);SetWindowPointer(win,TAG_DONE);
			if(bshape)  GT_SetGadgetAttrs(gad[firstix+GADIX_VALE   ],win,0l,GA_Disabled,FALSE,TAG_DONE);
			else        GT_SetGadgetAttrs(gad[firstix+GADIX_VALE   ],win,0l,GA_Disabled,TRUE ,TAG_DONE);
			GT_SetGadgetAttrs(gad[firstix+GADIX_MODDESC],win,0l,GTTX_Text  ,mod->desc  ,TAG_DONE);
			LIB_INTCALL(AssignBShape(mod));
			break;
	}
	return(ret);
}

//-- ARexx Parsing

UBYTE SAVEDS ASM LIB_EXPORT(ParseModulatorSet(REG(a0,Modulator *mod),REG(a1,STRPTR prefix),REG(a2,STRPTR params))) {
	UBYTE pf=0;
	ULONG sl=strlen(prefix);
	UBYTE bshape;
	ModCurve *mcu;
	ModCycle *mcy;
	ModVector *mve;
	ModUser *mus;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(mod,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(prefix,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(params,__FUNC__,"3.par",4));

	if(!sl || (sl && !strnicmp(RexxPar2,prefix,sl))) {
		//MSG1("  parname=%s",&params[MaxParLen2+sl]);
		//MSG1("  parval =%s",RexxPar3);
		if(       !stricmp(&params[MaxParLen2+sl],"S"))         { strncpy(mod->vals,RexxPar3,PARBUFLEN);mod->vals[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(&params[MaxParLen2+sl],"E"))         { strncpy(mod->vale,RexxPar3,PARBUFLEN);mod->vale[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(&params[MaxParLen2+sl],"ModShape")) {
			bshape=CfgIO_MatchString(RexxPar3,BShapeLabels);pf=1;
			if(bshape!=mod->bshape) {
				switch(mod->bshape) {
					//case BSHP_NONE: break;
					//case BSHP_CURVE: break;
					//case BSHP_CYCLE: break;
					//case BSHP_VECTOR: break;
					case BSHP_USER:
						mus=(ModUser *)(mod->bshpdata);
						LIB_INTCALL(UnlockBuffer(mus->modbuf));
						if(mus->envdata) FreeVec(mus->envdata);
						break;
				}
				if(mod->bshpdata) { FreeVec(mod->bshpdata);mod->bshpdata=NULL; }
				mod->bshape=bshape;
				switch(bshape) {
					case BSHP_NONE: break;
					case BSHP_CURVE:
						if(!(mod->bshpdata=AllocVec(sizeof(ModCurve),MEMF_ANY|MEMF_CLEAR))) mod->bshape=BSHP_NONE;
						break;
					case BSHP_CYCLE:
						if(!(mod->bshpdata=AllocVec(sizeof(ModCycle),MEMF_ANY|MEMF_CLEAR))) mod->bshape=BSHP_NONE;
						break;
					case BSHP_VECTOR:
						if(!(mod->bshpdata=AllocVec(sizeof(ModVector),MEMF_ANY|MEMF_CLEAR))) mod->bshape=BSHP_NONE;
						break;
					case BSHP_USER:
						if(!(mod->bshpdata=AllocVec(sizeof(ModUser),MEMF_ANY|MEMF_CLEAR))) mod->bshape=BSHP_NONE;
						break;
				}
			}
		}
		switch(mod->bshape) {
			case BSHP_NONE: break;
			case BSHP_CURVE:
				mcu=(ModCurve *)(mod->bshpdata);
				if(!pf && !stricmp(&params[MaxParLen2+sl],"CurveExp")) { mcu->exp=atof(RexxPar3);pf=1; }
				break;
			case BSHP_CYCLE:
				mcy=(ModCycle *)(mod->bshpdata);
				if(!pf && !stricmp(&params[MaxParLen2+sl],"CycleOszi")) { mcy->oszi=CfgIO_MatchString(RexxPar3,OsziLabels);pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"CycleFrq"))  { strncpy(mcy->frq,RexxPar3,PARBUFLEN);mcy->frq[PARBUFLEN]=0;pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"CyclePhase")){ strncpy(mcy->pha,RexxPar3,PARBUFLEN);mcy->pha[PARBUFLEN]=0;pf=1; }
				break;
			case BSHP_VECTOR:
				mve=(ModVector *)(mod->bshpdata);
				if(!pf && !stricmp(&params[MaxParLen2+sl],"VectorAnz")) { mve->anz=atoi(RexxPar3)%MV_MAX_VECTORS;pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"VectorPos")) { mve->pos[atoi(RexxPar3)%(mve->anz)]=(float)(bound(atof(RexxPar4),0.0,1.0));pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"VectorLev")) { mve->lev[atoi(RexxPar3)%(mve->anz)]=(float)(bound(atof(RexxPar4),0.0,1.0));pf=1; }
				break;
			case BSHP_USER:
				mus=(ModUser *)(mod->bshpdata);
				if(!pf && !stricmp(&params[MaxParLen2+sl],"UserModBuf")){ mus->modid=atoi(RexxPar3);pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"UserType"))  { mus->type=CfgIO_MatchString(RexxPar3,TypeLabels);pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"UserMode"))  { mus->bmode=CfgIO_MatchString(RexxPar3,ModeLabels);pf=1; }
				if(mus->type==MU_TYPE_AMPENV)
					if(!pf && !stricmp(&params[MaxParLen2+sl],"UserAmpEnv")){ mus->env=CfgIO_MatchString(RexxPar3,EnvLabels[0]);pf=1; }
				if(mus->type==MU_TYPE_FRQENV)
					if(!pf && !stricmp(&params[MaxParLen2+sl],"UserFrqEnv")){ mus->env=CfgIO_MatchString(RexxPar3,EnvLabels[1]);pf=1; }
				break;
		}
	}
	return(pf);
}

UBYTE SAVEDS ASM LIB_EXPORT(ParseModulatorGet(REG(a0,Modulator *mod),REG(a1,STRPTR prefix),REG(a2,STRPTR params),REG(a3,STRPTR ret))) {
	UBYTE pf=0;
	ULONG sl=strlen(prefix);
	ModCurve *mcu;
	ModCycle *mcy;
	ModVector *mve;
	ModUser *mus;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(mod,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(prefix,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(params,__FUNC__,"3.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(ret,__FUNC__,"4.par",4));

	if(!sl || (sl && !strnicmp(&params[MaxParLen2],prefix,sl))) {
		if(       !stricmp(&params[MaxParLen2+sl],"S"))         { strncpy(ret,mod->vals,PARBUFLEN);ret[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(&params[MaxParLen2+sl],"E"))         { strncpy(ret,mod->vale,PARBUFLEN);ret[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(&params[MaxParLen2+sl],"ModShape"))  { strcpy(ret,BShapeLabels[mod->bshape]);pf=1; }
		switch(mod->bshape) {
			case BSHP_NONE: break;
			case BSHP_CURVE:
				mcu=(ModCurve *)(mod->bshpdata);
				if(!pf && !stricmp(&params[MaxParLen2+sl],"CurveExp"))  { sprintf(ret,"%lf",mcu->exp);pf=1; }
				break;
			case BSHP_CYCLE:
				mcy=(ModCycle *)(mod->bshpdata);
				if(!pf && !stricmp(&params[MaxParLen2+sl],"CycleOszi")) { strcpy(ret,OsziLabels[mcy->oszi]);pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"CycleFrq"))  { strncpy(ret,mcy->frq,PARBUFLEN);ret[PARBUFLEN]=0;pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"CyclePhase")){ strncpy(ret,mcy->pha,PARBUFLEN);ret[PARBUFLEN]=0;pf=1; }
				break;
			case BSHP_VECTOR:
				mve=(ModVector *)(mod->bshpdata);
				if(!pf && !stricmp(&params[MaxParLen2+sl],"VectorAnz")) { sprintf(ret,"%d",mve->anz);pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"VectorPos")) { sprintf(ret,"%lf",mve->pos[atoi(&params[MaxParLen3])%(mve->anz)]);pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"VectorLev")) { sprintf(ret,"%lf",mve->lev[atoi(&params[MaxParLen3])%(mve->anz)]);pf=1; }
				break;
			case BSHP_USER:
				mus=(ModUser *)(mod->bshpdata);
				if(!pf && !stricmp(&params[MaxParLen2+sl],"UserModBuf")){ sprintf(ret,"%d",mus->modid);pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"UserType"))  { strcpy(ret,TypeLabels[mus->type]);pf=1; }
				if(!pf && !stricmp(&params[MaxParLen2+sl],"UserMode"))  { strcpy(ret,ModeLabels[mus->bmode]);pf=1; }
				if(mus->type==MU_TYPE_AMPENV)
					if(!pf && !stricmp(&params[MaxParLen2+sl],"UserAmpEnv")){ strcpy(ret,EnvLabels[0][mus->env]);pf=1; }
				if(mus->type==MU_TYPE_FRQENV)
					if(!pf && !stricmp(&params[MaxParLen2+sl],"UserFrqEnv")){ strcpy(ret,EnvLabels[1][mus->env]);pf=1; }
				break;
		}
	}
	return(pf);
}

//-- eof ----------------------------------------------------------------------
