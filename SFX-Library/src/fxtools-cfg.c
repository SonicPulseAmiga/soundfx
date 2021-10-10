/****h* sfxsupport.library/fxtools-cfg.c [4.2] *
*
*  NAME
*    fxtools-cfg.c
*  COPYRIGHT
*    $VER: fxtools-cfg.c 4.2 (18.02.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    some fx cfg helpers
*  AUTHOR
*    Stefan Kost            sk      kost@imn.htwk-leipzig.de
*  CREATION DATE
*    24.Mar.2003
*  MODIFICATION HISTORY
*    24.Mar.2003    V 4.2   most recent version
*    24.Mar.2003    V 4.2   initial version
*  NOTES
*
*******
*/

#define FXTOOLS_CFG_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Config Tools

void            ASM LIB_EXPORT(InitFXParams(REG(a0,PrefsData *fxdesc)));
void            ASM LIB_EXPORT(DoneFXParams(REG(a0,PrefsData *fxdesc)));
void            ASM LIB_EXPORT(FreeFXParams(REG(a0,PrefsData *fxdesc)));

void            ASM LIB_EXPORT(SaveModSettings(REG(a0,APTR cfg),REG(a1,PrefsData *fxdesc)));
void            ASM LIB_EXPORT(LoadModSettings(REG(a0,APTR cfg),REG(a1,PrefsData *fxdesc)));

//-- definitions --------------------------------------------------------------

//-- Config Tools

void SAVEDS ASM LIB_EXPORT(InitFXParams(REG(a0,PrefsData *fxdesc))) {
	register UBYTE i=0;
	Source *src;
	Modulator *mod;
	ModUser *mus;
	Interpolator *inter;
	WinFunction *wfkt;
	ULONG *ci;
	ULONG slen=0,srat=0;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(fxdesc,__FUNC__,"1.par",4));

	while(fxdesc[i].type) {
		switch(fxdesc[i].type) {
			case PD_COREINSTANCE:
				//MSG("  coreinstance");
				ci=((ULONG *)(fxdesc[i].data));
				srat=*ci;ci++;
				slen=*ci;
				break;
			case PD_SOURCE:
				//MSG("  source");
				src=((Source *)(fxdesc[i].data));
				src->srcbuf=LIB_INTCALL(LockBuffer(GetRunTime(aktbuf)));
				src->rmode=LIB_INTCALL(SetRngMode(src->srcbuf));
				if(!slen && !srat && src->srcbuf) {
					slen=src->srcbuf->slen;
					srat=src->srcbuf->srat;
				}
				break;
			case PD_MODULATOR:
				//MSG("  modulator");
				mod=((Modulator *)(fxdesc[i].data));
				//mod->desc[0]=0l;
				mod->slen=slen;
				mod->srat=srat;
				//MSG1("    bshpdata=0x%08lx",mod->bshpdata);
				switch(mod->bshape) {
					case BSHP_NONE:   Describe_ModNone(mod);break;
					case BSHP_CURVE:  Describe_ModCurve(mod);break;
					case BSHP_CYCLE:  Describe_ModCycle(mod);break;
					case BSHP_VECTOR: Describe_ModVector(mod);break;
					case BSHP_USER:
						mus=(ModUser *)(mod->bshpdata);
						mus->modbuf=SetValidBuffer(&mus->modid);
						LIB_INTCALL(LockBuffer(mus->modbuf));
						Describe_ModUser(mod);
						break;
				}
				LIB_INTCALL(AssignBShape(mod));
				break;
			case PD_INTERPOLATOR:
				//MSG("  interpolator");
				inter=((Interpolator *)(fxdesc[i].data));
				GetInterpol(inter);
				break;
			case PD_WINFUNCTION:
				//MSG("  winfunction");
				wfkt=((WinFunction *)(fxdesc[i].data));
				GetWFkt(wfkt);
				break;
		}
		i++;
	}
}

void SAVEDS ASM LIB_EXPORT(DoneFXParams(REG(a0,PrefsData *fxdesc))) {
	register UBYTE i=0;
	Source *src;
	Modulator *mod;
	ModUser *mus;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(fxdesc,__FUNC__,"1.par",4));

	while(fxdesc[i].type) {
		switch(fxdesc[i].type) {
			case PD_SOURCE:
				src=((Source *)(fxdesc[i].data));
				LIB_INTCALL(UnlockBuffer(src->srcbuf));
				break;
			case PD_MODULATOR:
				mod=((Modulator *)(fxdesc[i].data));
				switch(mod->bshape) {
					//case BSHP_NONE: break;
					//case BSHP_CURVE: break;
					//case BSHP_CYCLE: break;
					//case BSHP_VECTOR: break;
					case BSHP_USER:
						mus=(ModUser *)(mod->bshpdata);
						LIB_INTCALL(UnlockBuffer(mus->modbuf));
						//if(mus->envdata) FreeVec(mus->envdata);
						break;
				}
				//if(mod->bshpdata) { FreeVec(mod->bshpdata);mod->bshpdata=NULL; }
				break;
		}
		i++;
	}
}

/****** sfxsupport.library/FreeFXParams [1.20] *
*
*  NAME
*    FreeFXParams
*  SYNOPSIS
*
*  FUNCTION
*    frees memory allocated by objects of fx-modules
*  INPUTS
*
*  RESULT
*
*  NOTES
*
*  SEE ALSO
*
**********
*/
void SAVEDS ASM LIB_EXPORT(FreeFXParams(REG(a0,PrefsData *fxdesc))) {
	register UBYTE i=0;
	Modulator *mod;
	ModUser *mus;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(fxdesc,__FUNC__,"1.par",4));

	while(fxdesc[i].type) {
		switch(fxdesc[i].type) {
			//case PD_SOURCE: break;
			case PD_MODULATOR:
				mod=((Modulator *)(fxdesc[i].data));
				switch(mod->bshape) {
					//case BSHP_NONE: break;
					//case BSHP_CURVE: break;
					//case BSHP_CYCLE: break;
					//case BSHP_VECTOR: break;
					case BSHP_USER:
						mus=(ModUser *)(mod->bshpdata);
						if(mus->envdata) FreeVec(mus->envdata);
						break;
				}
				if(mod->bshpdata) { FreeVec(mod->bshpdata);mod->bshpdata=NULL; }
				break;
			//case PD_INTERPOLATOR: break;
			//case PD_WINFUNCTION: break;
		}
		i++;
	}
}

void SAVEDS ASM LIB_EXPORT(SaveModSettings(REG(a0,APTR cfg),REG(a1,PrefsData *fxdesc))) {
	register UBYTE i=0;
	register UWORD j;
	ULONG *ci;
	ULONG slen=0,srat=0;
	char strItemName[20],strItemData[50];
	Source *src;
	Modulator *mod;
	ModCurve *mcu;
	ModCycle *mcy;
	ModVector *mve;
	ModUser *mus;
	Interpolator *inter;
	WinFunction *wfkt;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(cfg,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(fxdesc,__FUNC__,"2.par",4));

	while(fxdesc[i].type) {
		switch(fxdesc[i].type) {
			case PD_COREINSTANCE:
				ci=((ULONG *)(fxdesc[i].data));
				srat=*ci;ci++;
				slen=*ci;
				break;
			case PD_SOURCE:
				src=((Source *)(fxdesc[i].data));
				if(!slen && !srat && src->srcbuf) {
					slen=src->srcbuf->slen;
					srat=src->srcbuf->srat;
				}
				CfgIO_WriteString         (cfg,fxdesc[i].prefix ,"RangeMode"        ,RModeLabels[src->rmode]);
				break;
			case PD_MODULATOR:
				mod=((Modulator *)(fxdesc[i].data));
				mod->slen=slen;
				mod->srat=srat;
				CfgIO_DeleteSection       (cfg,fxdesc[i].prefix);
				CfgIO_WriteString         (cfg,fxdesc[i].prefix,"Value1"            ,mod->vals);
				CfgIO_WriteString         (cfg,fxdesc[i].prefix,"Value2"            ,mod->vale);
				CfgIO_WriteString         (cfg,fxdesc[i].prefix,"BShape"            ,BShapeLabels[mod->bshape]);
				switch(mod->bshape) {
					//case BSHP_NONE: break;
					case BSHP_CURVE:
						mcu=(ModCurve *)(mod->bshpdata);
						CfgIO_WriteFloat          (cfg,fxdesc[i].prefix,"Cu_Exp"    ,&mcu->exp);
						break;
					case BSHP_CYCLE:
						mcy=(ModCycle *)(mod->bshpdata);
						CfgIO_WriteString         (cfg,fxdesc[i].prefix,"Cy_Oszi"   ,OsziLabels[mcy->oszi]);
						CfgIO_WriteString         (cfg,fxdesc[i].prefix,"Cy_Frq"    ,mcy->frq);
						CfgIO_WriteString         (cfg,fxdesc[i].prefix,"Cy_Pha"    ,mcy->pha);
						break;
					case BSHP_VECTOR:
						mve=(ModVector *)(mod->bshpdata);
						CfgIO_WriteUnsignedInteger(cfg,fxdesc[i].prefix,"Ve_Anz"    ,mve->anz);
						for(j=0;j<mve->anz;j++) {
							sprintf(strItemName,"Ve_Point%03d",j);
							sprintf(strItemData,"%8.6lf,%8.6lf",mve->pos[j],mve->lev[j]);
							CfgIO_WriteString     (cfg,fxdesc[i].prefix,strItemName,strItemData);
						}
					//CfgIO_WriteBool           (cfg,fxdesc[i].prefix,"Ve_LockFirst"  ,mve->LockFirst);
					//CfgIO_WriteBool           (cfg,fxdesc[i].prefix,"Ve_LockLast"   ,mve->LockLast);
						break;
					case BSHP_USER:
						mus=(ModUser *)(mod->bshpdata);
						CfgIO_WriteUnsignedInteger(cfg,fxdesc[i].prefix,"Us_ModId"      ,mus->modid);
						CfgIO_WriteString         (cfg,fxdesc[i].prefix,"Us_Type"       ,TypeLabels[mus->type]);
						CfgIO_WriteString         (cfg,fxdesc[i].prefix,"Us_Mode"       ,ModeLabels[mus->bmode]);
						switch(mus->type) {
					//case MU_TYPE_NORMAL: break;
					//case MU_TYPE_ABS: break;
							case MU_TYPE_AMPENV:
							case MU_TYPE_FRQENV:
								CfgIO_WriteString (cfg,fxdesc[i].prefix,"Us_Env"        ,EnvLabels[mus->type-MU_TYPE_AMPENV][mus->env]);
								break;
						}
						break;
				}
				break;
			case PD_INTERPOLATOR:
				inter=((Interpolator *)(fxdesc[i].data));
				CfgIO_WriteString   (cfg,fxdesc[i].prefix,"Type"        ,InterpolLabels[inter->type]);
				CfgIO_WriteFloat    (cfg,fxdesc[i].prefix,"Range"       ,&inter->range);
				break;
			case PD_WINFUNCTION:
				wfkt=((WinFunction *)(fxdesc[i].data));
				CfgIO_WriteString   (cfg,fxdesc[i].prefix,"Type"        ,WFktLabels[wfkt->win]);
				CfgIO_WriteFloat    (cfg,fxdesc[i].prefix,"Param"       ,&wfkt->par);
				break;
		}
		i++;
	}
}

void SAVEDS ASM LIB_EXPORT(LoadModSettings(REG(a0,APTR cfg),REG(a1,PrefsData *fxdesc))) {
	register UBYTE i=0;
	register UWORD j;
	//ULONG slen=0,srat=0;
	char strItemName[20],*strItemData;
	Source *src;
	Modulator *mod;
	ModCurve *mcu;
	ModCycle *mcy;
	ModVector *mve;
	ModUser *mus;
	Interpolator *inter;
	WinFunction *wfkt;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(cfg,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(fxdesc,__FUNC__,"2.par",4));

	while(fxdesc[i].type) {
		switch(fxdesc[i].type) {
			case PD_SOURCE:
				//MSG("  source");
				src=((Source *)(fxdesc[i].data));
				//if(src->srcbuf!=GetRunTime(aktbuf)) {
					//LIB_INTCALL(UnlockBuffer(src->srcbuf));
					//src->srcbuf=LIB_INTCALL(LockBuffer(GetRunTime(aktbuf)));
				//}
				//if(!slen && !srat && src->srcbuf) {
					//slen=src->srcbuf->slen;
					//srat=src->srcbuf->srat;
				//}
				// should we avoid loading it ?
				// it is disturbing, when switching presets
				// currently I only load it if it is there, otherwise it remains unchanged
				src->rmode= CfgIO_MatchString(
							CfgIO_ReadString  (cfg,fxdesc[i].prefix ,"RangeMode"        ,RModeLabels[src->rmode]),RModeLabels);
				break;
			case PD_MODULATOR:
				//MSG("  modulator");
				mod=(Modulator *)(fxdesc[i].data);
				//mod->slen=slen;
				//mod->srat=srat;
				strncpy(mod->vals,CfgIO_ReadString          (cfg,fxdesc[i].prefix,"Value1"          ,"1"),PARBUFLEN);mod->vals[PARBUFLEN]='\0';
				strncpy(mod->vale,CfgIO_ReadString          (cfg,fxdesc[i].prefix,"Value2"          ,"1"),PARBUFLEN);mod->vale[PARBUFLEN]='\0';
				mod->bshape=      CfgIO_MatchString(
								  CfgIO_ReadString          (cfg,fxdesc[i].prefix,"BShape"          ,BShapeLabels[BSHP_NONE]),BShapeLabels);
				mod->desc[0]='\0';
				switch(mod->bshape) {
					case BSHP_NONE:
						//MSG("    none");
						mod->bshpdata=NULL;
						break;
					case BSHP_CURVE:
						//MSG("    curve");
						if((mod->bshpdata=AllocVec(sizeof(ModCurve),MEMF_ANY|MEMF_CLEAR))) {
							mcu=(ModCurve *)(mod->bshpdata);
							mcu->exp=atof(CfgIO_ReadString  (cfg,fxdesc[i].prefix,"Cu_Exp"      ,"1.0"));
						}
						else { mod->bshape=BSHP_NONE; }
						break;
					case BSHP_CYCLE:
						//MSG("    cycle");
						if((mod->bshpdata=AllocVec(sizeof(ModCycle),MEMF_ANY|MEMF_CLEAR))) {
							mcy=(ModCycle *)(mod->bshpdata);
							mcy->oszi=  CfgIO_MatchString       (
										CfgIO_ReadString        (cfg,fxdesc[i].prefix,"Cy_Oszi" ,OsziLabels[MC_OSZI_SIN]),OsziLabels);
							strncpy(mcy->frq,CfgIO_ReadString   (cfg,fxdesc[i].prefix,"Cy_Frq"  ,"1.0 Hz"),PARBUFLEN);mcy->frq[PARBUFLEN]='\0';
							strncpy(mcy->pha,CfgIO_ReadString   (cfg,fxdesc[i].prefix,"Cy_Pha"  ,"0.0 °"),PARBUFLEN);mcy->pha[PARBUFLEN]='\0';
							//MSG2("      mcy->frq=%s,0x%08lx",mcy->frq,mcy->frq);
							//MSG2("      mcy->pha=%s,0x%08lx",mcy->pha,mcy->pha);
						}
						else { mod->bshape=BSHP_NONE; }
						break;
					case BSHP_VECTOR:
						//MSG("    vector");
						if((mod->bshpdata=AllocVec(sizeof(ModVector),MEMF_ANY|MEMF_CLEAR))) {
							mve=(ModVector *)(mod->bshpdata);
							mve->anz=CfgIO_ReadUnsignedInteger(cfg,fxdesc[i].prefix,"Ve_Anz"    ,11);
							for(j=0;j<mve->anz;j++) {
								sprintf(strItemName,"Ve_Point%03d",j);
								strItemData=CfgIO_ReadString      (cfg,fxdesc[i].prefix,strItemName,"1.0,0.0");
								mve->pos[j]=atof(CfgIO_GetSubItem(strItemData,&strItemData,','));
								mve->lev[j]=atof(CfgIO_GetSubItem(strItemData,&strItemData,','));
							}
							////mve->LockFirst=CfgIO_ReadBool           (cfg,fxdesc[i].prefix,"Ve_LockFirst",FALSE);
							////mve->LockLast =CfgIO_ReadBool           (cfg,fxdesc[i].prefix,"Ve_LockLast" ,FALSE);
						}
						else { mod->bshape=BSHP_NONE; }
						break;
					case BSHP_USER:
						//MSG("    user");
						if((mod->bshpdata=AllocVec(sizeof(ModVector),MEMF_ANY|MEMF_CLEAR))) {
							mus=(ModUser *)(mod->bshpdata);
							mus->modid=     CfgIO_ReadUnsignedInteger(cfg,fxdesc[i].prefix,"Us_ModId"       ,mus->modid);
							mus->modbuf=SetValidBuffer(&mus->modid);
							mus->type=      CfgIO_MatchString(
											CfgIO_ReadString (cfg,fxdesc[i].prefix,"Us_Type"        ,TypeLabels[MU_TYPE_NORMAL]),TypeLabels);
							mus->bmode=     CfgIO_MatchString(
											CfgIO_ReadString (cfg,fxdesc[i].prefix,"Us_Mode"        ,ModeLabels[MU_BMOD_REPEAT]),ModeLabels);
							switch(mus->type) {
								//case MU_TYPE_NORMAL: break;
								//case MU_TYPE_ABS: break;
								case MU_TYPE_AMPENV:
									mus->env=CfgIO_MatchString(
											 CfgIO_ReadString (cfg,fxdesc[i].prefix,"Us_Env"        ,EnvLabels[mus->type-MU_TYPE_AMPENV][MU_AENV_MAXDECAY]),EnvLabels[mus->type-MU_TYPE_AMPENV]);
									if(!(mus->envdata=AllocVec(sizeof(AmpEnvMaxDecay),MEMF_ANY|MEMF_CLEAR))) mus->type=MU_TYPE_NORMAL;
									break;
								case MU_TYPE_FRQENV:
									mus->env=CfgIO_MatchString(
											 CfgIO_ReadString (cfg,fxdesc[i].prefix,"Us_Env"        ,EnvLabels[mus->type-MU_TYPE_AMPENV][MU_PENV_PEAKSEARCH]),EnvLabels[mus->type-MU_TYPE_AMPENV]);
									if(!(mus->envdata=AllocVec(sizeof(FrqEnvPeakSearch),MEMF_ANY|MEMF_CLEAR))) mus->type=MU_TYPE_NORMAL;
									break;
							}
						}
						else { mod->bshape=BSHP_NONE; }
						break;
				}
				//MSG1("      bshpdata=0x%08lx",mod->bshpdata);
				break;
			case PD_INTERPOLATOR:
				//MSG("  interpolator");
				inter=((Interpolator *)(fxdesc[i].data));
				inter->type=    CfgIO_MatchString(
								CfgIO_ReadString (cfg,fxdesc[i].prefix,"Type"       ,InterpolLabels[INT_TYPE_NONE]),InterpolLabels);
				inter->range=atof(CfgIO_ReadString   (cfg,fxdesc[i].prefix,"Range"      ,"1.0"));
				inter->desc[0]='\0';/*GetInterpol(inter);*/
				break;
			case PD_WINFUNCTION:
				//MSG("  winfunction");
				wfkt=((WinFunction *)(fxdesc[i].data));
				wfkt->win=      CfgIO_MatchString(
								CfgIO_ReadString (cfg,fxdesc[i].prefix,"Type"       ,WFktLabels[WIN_HANNING]),WFktLabels);
				wfkt->par=atof( CfgIO_ReadString (cfg,fxdesc[i].prefix,"Param"      ,"1.0"));
				wfkt->desc[0]='\0';/*GetWFkt(wfkt);*/
				break;
		}
		i++;
	}
}

//-- eof ----------------------------------------------------------------------
