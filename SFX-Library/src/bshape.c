/****h* sfxsupport.library/bshape.c [4.3] *
*
*  NAME
*    bshape.c
*  COPYRIGHT
*    $VER: bshape.c 4.3 (14.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Modulator BlendShapes - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    26.Nov.1995
*  MODIFICATION HISTORY
*    14.Oct.2003	V 4.3	most recent version
*    03.Apr.2002	V 4.2	most recent version
*    25.Apr.2001	V 4.1	most recent version
*							C: ModWin*(),RefreshMW*() naming
*							A: Describe*() functions
*							A: User_FrqEnv_Energy (beta)
*							C: BShape naming
*    31.May.2000	V 1.30	most recent version
*    29.Apr.1999	V 1.20	most recent version
*    26.Nov.1995	V 1.00	initial version
*  IDEAS
*    * supply channel to AssignBShape for stereo/quadro modulations with bshape=User
*  NOTES
*
*******
*/

#define BSHAPE_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- BShape to Modulator assign and initialisation

void ASM LIB_EXPORT(AssignBShape(REG(a0,Modulator *mod)));

//-- definitions --------------------------------------------------------------

//-- BShape to Modulator assign and initialisation

void SAVEDS ASM LIB_EXPORT(AssignBShape(REG(a0,Modulator *mod))) {
	ModCurve *mcu;
	ModCycle *mcy;
	ModVector *mve;
	ModUser	*mus;
	AmpEnvMaxDecay *emd;
	FrqEnvPeakSearch *eps;
	FrqEnvEnergy *eeg;
	Param param;
	UBYTE pu;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(mod,__FUNC__,"1.par",4));

	MSG2("Modulator : slen/srat=%ld/%ld",mod->slen,mod->srat);

	switch(mod->bshape) {
		case BSHP_NONE:
			//MSG("  none");
			mod->modfunc=BShape_None;
			break;
		case BSHP_CURVE:
			//MSG("  curve");
			mcu=(ModCurve *)(mod->bshpdata);
			if((mcu->exp>=0.9999) && (mcu->exp<=1.0001)) {
				mod->modfunc=BShape_Linear;
				//MSG("    lin");
			}
			else {
				mod->modfunc=BShape_Curve;
				//MSG("    exp");
			}
			break;
		case BSHP_CYCLE:
			//MSG("  cycle");
			mcy=(ModCycle *)(mod->bshpdata);
			switch(mcy->oszi) {
				case MC_OSZI_SAW:	mod->modfunc=BShape_Cycle_Saw;break;
				case MC_OSZI_SIN:	mod->modfunc=BShape_Cycle_Sin;break;
				case MC_OSZI_SQR:	mod->modfunc=BShape_Cycle_Sqr;break;
				case MC_OSZI_TRI:	mod->modfunc=BShape_Cycle_Tri;break;
				case MC_OSZI_RND:	mod->modfunc=BShape_Cycle_Rnd;break;
				case MC_OSZI_SRND:	mod->modfunc=BShape_Cycle_SRnd;break;
			}
			if((mcy->oszi!=MC_OSZI_RND) && (mcy->oszi!=MC_OSZI_SRND)) {
				MSG("    not rnd/srnd");
				//-- do we already care about lower control frequency resolution ?????????????????????????????????
				UC_ConvertParameter(param,mcy->pha,UGRP_PHASE,PHA_RAD);
				mcy->angle=atof(param);
				//MSG1("    angle=%lf",mcy->angle);
				if(UC_GetParameterUnit(mcy->frq,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
					MSG("      type=1 (e.g. Hz)");
					UC_ConvertParameter(param,mcy->frq,UGRP_ABSFREQUENCY,AFRQ_HZ);
					mcy->anglest=(PIM2*atof(mcy->frq))/(double)mod->srat;
				}
				else {
					if((pu=UC_GetParameterUnit(mcy->frq,UGRP_ABSTIME))!=INVALID_UNIT) {
						if(pu==ATME_SAMPLES) {
							MSG("      type=2 (samples)");
							mcy->anglest=PIM2/atof(mcy->frq);
						}
						else {
							MSG("      type=3 (e.g. ms)");
							UC_ConvertParameter(param,mcy->frq,UGRP_ABSTIME,ATME_MS);
							mcy->anglest=(PIM2*1000.0)/(atof(param)*(double)mod->srat);
						}
					}
					else {
						MSG("      type=4 (factor)");
						UC_ConvertParameter(param,mcy->frq,UGRP_RELTIME,RTME_FC);
						mcy->anglest=PIM2/((double)mod->slen*atof(param));
					}
				}
			}
			else {
				MSG("    rnd/srnd");
				if(UC_GetParameterUnit(mcy->frq,UGRP_ABSFREQUENCY)!=INVALID_UNIT) {
					UC_ConvertParameter(param,mcy->frq,UGRP_ABSFREQUENCY,AFRQ_HZ);
					//mcy->anglest=(atof(mcy->frq)*(double)mod->slen)/(double)mod->srat;
					mcy->anglest=atof(mcy->frq)*(double)mod->srat;
				}
				else {
					if((pu=UC_GetParameterUnit(mcy->frq,UGRP_ABSTIME))!=INVALID_UNIT) {
						if(pu==ATME_SAMPLES) mcy->anglest=atof(mcy->frq);
						else {
							UC_ConvertParameter(param,mcy->frq,UGRP_ABSTIME,ATME_MS);
							//mcy->anglest=(1000.0*atof(param))/(double)mod->srat;
							mcy->anglest=(atof(param)*(double)mod->srat)/1000.0;
						}
					}
					else {
						UC_ConvertParameter(param,mcy->frq,UGRP_RELTIME,RTME_FC);
						mcy->anglest=((double)mod->slen*atof(param));
					}
				}
				mcy->angle=(double)rand()/(double)RAND_MAX;
				mcy->newrnd=(double)rand()/(double)RAND_MAX;
				mcy->diff=mcy->newrnd-mcy->angle;
				mcy->hold=mcy->anglest;
				MSG4("Modulator : Cycle.Rnd.hold=%lf  Cycle.Rnd.diff=%lf  slen/srat=%ld/%ld",mcy->hold,mcy->diff,mod->slen,mod->srat);
			}
			//MSG1("    anglest=%lf",mcy->anglest);
			break;
		case BSHP_VECTOR:
			//MSG("  vector");
			mve=(ModVector *)(mod->bshpdata);
			mve->ix=0;
			mve->dfc=0.0;
			mod->modfunc=BShape_Vector;
			break;
		case BSHP_USER:
			//MSG("  user");
			mus=(ModUser *)(mod->bshpdata);
			mus->channel=0;
			switch(mus->type) {
				case MU_TYPE_NORMAL:
					switch(mus->bmode) {
						case MU_BMOD_SINGLE:	mod->modfunc=BShape_UserNormal_Single;break;
						case MU_BMOD_REPEAT:	mod->modfunc=BShape_UserNormal_Repeat;break;
						case MU_BMOD_STRETCH:	mod->modfunc=BShape_UserNormal_Stretch;break;
					}
					break;
				case MU_TYPE_ABSOLUT:
					switch(mus->bmode) {
						case MU_BMOD_SINGLE:	mod->modfunc=BShape_UserAbsolut_Single;break;
						case MU_BMOD_REPEAT:	mod->modfunc=BShape_UserAbsolut_Repeat;break;
						case MU_BMOD_STRETCH:	mod->modfunc=BShape_UserAbsolut_Stretch;break;
					}
					break;
				case MU_TYPE_AMPENV:
					switch(mus->env) {
						case MU_AENV_MAXDECAY:
							emd=(AmpEnvMaxDecay *)(mus->envdata);
							emd->maxval=0.0;
							emd->fc1=0.9995;						// maybe dependent from srat
							emd->fc2=1.0-emd->fc1;
							switch(mus->bmode) {
								case MU_BMOD_SINGLE:	mod->modfunc=BShape_UserAmpEnv_MaxDecay_Single;break;
								case MU_BMOD_REPEAT:	mod->modfunc=BShape_UserAmpEnv_MaxDecay_Repeat;break;
								case MU_BMOD_STRETCH:	mod->modfunc=BShape_UserAmpEnv_MaxDecay_Stretch;break;
							}
							break;
					}
					break;
				case MU_TYPE_FRQENV:
					switch(mus->env) {
						case MU_PENV_PEAKSEARCH:
							eps=(FrqEnvPeakSearch *)(mus->envdata);
							eps->lastpos=0;
							eps->curlen=100;							// maybe seek one period initially
							eps->lastval=0;
							eps->maxs=TRUE;eps->mins=FALSE;
							switch(mus->bmode) {
								case MU_BMOD_SINGLE:	mod->modfunc=BShape_UserFrqEnv_PeakSearch_Single;break;
								case MU_BMOD_REPEAT:	mod->modfunc=BShape_UserFrqEnv_PeakSearch_Repeat;break;
								case MU_BMOD_STRETCH:	mod->modfunc=BShape_UserFrqEnv_PeakSearch_Stretch;break;
							}
							break;
						case MU_PENV_ENERGY:
							eeg=(FrqEnvEnergy *)(mus->envdata);
							eeg->lastpos=0;
							eeg->energy=0;
							eeg->curlen=100;							// maybe seek one period initially
							eeg->mins=FALSE;
							switch(mus->bmode) {
								case MU_BMOD_SINGLE:	mod->modfunc=BShape_UserFrqEnv_Energy_Single;break;
								case MU_BMOD_REPEAT:	mod->modfunc=BShape_UserFrqEnv_Energy_Repeat;break;
								case MU_BMOD_STRETCH:	mod->modfunc=BShape_UserFrqEnv_Energy_Stretch;break;
							}
							break;
					}
					break;
			}
			break;
	}
}

//-- eof ----------------------------------------------------------------------
