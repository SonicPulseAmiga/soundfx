/****h* sfxsupport.library/bshape-user.c [4.2] *
*
*  NAME
*    bshape-user.c
*  COPYRIGHT
*    $VER: bshape-user.c 4.2 (03.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Modulator BlendShapes : User - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    26.Nov.1995
*  MODIFICATION HISTORY
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
*  NOTES
*
*******
*/

#define BSHAPE_USER_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator User

void Describe_ModUser  (Modulator *mod);

double		ASM BShape_UserNormal_Single			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserNormal_Repeat			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserNormal_Stretch			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserAbsolut_Single			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserAbsolut_Repeat			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserAbsolut_Stretch			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserAmpEnv_MaxDecay_Single	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserAmpEnv_MaxDecay_Repeat	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserAmpEnv_MaxDecay_Stretch	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserFrqEnv_PeakSearch_Single	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserFrqEnv_PeakSearch_Repeat	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserFrqEnv_PeakSearch_Stretch(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserFrqEnv_Energy_Single		(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserFrqEnv_Energy_Repeat		(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_UserFrqEnv_Energy_Stretch	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));

//-- globals ------------------------------------------------------------------

UBYTE *TypeLabels[]={ "Normal","Absolut","AmpEnv","FrqEnv",0l };
UBYTE *ModeLabels[]={ "Single","Repeat","Stretch",0l };
UBYTE *EnvLabels[2][2]={ {"MaxDecay",0l},{"PeakSearch",0l} };
//BETA
//UBYTE *EnvLabels[2][3]={ {"MaxDecay",0l,0l},{"PeakSearch","Energy",0l} };
//BETA

//-- definitions --------------------------------------------------------------

//-- Modulator User

void Describe_ModUser(Modulator *mod) {
	ModUser *mu=(ModUser *)(mod->bshpdata);

	if(mu->modbuf) sprintf(mod->desc,"%s %s %s",((struct Node *)mu->modbuf)->ln_Name,TypeLabels[mu->type],ModeLabels[mu->bmode]);
	else mod->desc[0]=0;
}

double ALIGNED ASM BShape_UserNormal_Single(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	if(pos<((ModUser *)md)->modbuf->slen) return(0.5+((double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos]/65536.0));
	else return(0.5);
}

double ALIGNED ASM BShape_UserNormal_Repeat(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	return(0.5+((double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos%((ModUser *)md)->modbuf->slen]/65536.0));
}

double ALIGNED ASM BShape_UserNormal_Stretch(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	double ant1,ant2,spos;

	spos=(double)pos*(double)(((ModUser *)md)->modbuf->slen-1)/(double)max;
	ant1=1.0-(ant2=spos-(double)((LONG)spos));
	return(0.5+(((double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos]*ant1+(double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos+1L]*ant2)/65536.0));
}

double ALIGNED ASM BShape_UserAbsolut_Single(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	if(pos<((ModUser *)md)->modbuf->slen) return(__fabs((double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos])/32768.0);
	else return(0.0);
}

double ALIGNED ASM BShape_UserAbsolut_Repeat(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	return(__fabs((double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos%((ModUser *)md)->modbuf->slen])/32768.0);
}

double ALIGNED ASM BShape_UserAbsolut_Stretch(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	double ant1,ant2,spos;

	spos=(double)pos*(double)(((ModUser *)md)->modbuf->slen-1)/(double)max;
	ant1=1.0-(ant2=spos-(double)((LONG)spos));
	return(__fabs(((double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos]*ant1+(double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos+1L]*ant2)/32768.0));
}

double ALIGNED ASM BShape_UserAmpEnv_MaxDecay_Single(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	double aktval=0.0;
	AmpEnvMaxDecay *ed=(AmpEnvMaxDecay *)(((ModUser *)md)->envdata);

	if(pos<((ModUser *)md)->modbuf->slen) aktval=(double)labs(((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos]);

	if(aktval>ed->maxval) ed->maxval=aktval;
	else ed->maxval=(ed->fc1*ed->maxval)+(ed->fc2*aktval);
	return(ed->maxval/32768.0);
}

double ALIGNED ASM BShape_UserAmpEnv_MaxDecay_Repeat(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	double aktval;
	AmpEnvMaxDecay *ed=(AmpEnvMaxDecay *)(((ModUser *)md)->envdata);

	aktval=(double)labs(((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos%((ModUser *)md)->modbuf->slen]);

	if(aktval>ed->maxval) ed->maxval=aktval;
	else ed->maxval=(ed->fc1*ed->maxval)+(ed->fc2*aktval);
	return(ed->maxval/32768.0);
}

double ALIGNED ASM BShape_UserAmpEnv_MaxDecay_Stretch(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	double aktval;
	double ant1,ant2,spos;
	AmpEnvMaxDecay *ed=(AmpEnvMaxDecay *)(((ModUser *)md)->envdata);

	spos=(double)pos*(double)(((ModUser *)md)->modbuf->slen-1)/(double)max;
	ant1=1.0-(ant2=spos-(double)((LONG)spos));
	aktval=__fabs((double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos]*ant1+(double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos+1L]*ant2);

	if(aktval>ed->maxval) ed->maxval=aktval;
	else ed->maxval=(ed->fc1*ed->maxval)+(ed->fc2*aktval);
	return(ed->maxval/32768.0);
}

double ALIGNED ASM BShape_UserFrqEnv_PeakSearch_Single(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	WORD aktval=0;
	FrqEnvPeakSearch *ed=(FrqEnvPeakSearch *)(((ModUser *)md)->envdata);

	if(pos<((ModUser *)md)->modbuf->slen) aktval=(double)labs(((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos]);

	if(ed->maxs) { if(aktval<ed->lastval) { ed->maxs=FALSE;ed->mins=TRUE; } }
	else {
		if(ed->mins) {
			if(aktval>ed->lastval) {
				ed->curlen=pos-ed->lastpos;
				ed->lastpos=pos;
				ed->mins=FALSE;ed->maxs=TRUE;
			}
		}
	}
	ed->lastval=aktval;
	return(2.0/(double)ed->curlen);		/* curlen is *at least* 2 */
}

double ALIGNED ASM BShape_UserFrqEnv_PeakSearch_Repeat(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	WORD aktval;
	FrqEnvPeakSearch *ed=(FrqEnvPeakSearch *)(((ModUser *)md)->envdata);

	pos%=((ModUser *)md)->modbuf->slen;
	aktval=labs(((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos]);

	if(ed->maxs) { if(aktval<ed->lastval) { ed->maxs=FALSE;ed->mins=TRUE; } }
	else {
		if(ed->mins) {
			if(aktval>ed->lastval) {
				if(pos>ed->lastpos) ed->curlen=pos-ed->lastpos;
				else ed->curlen=pos;
				ed->lastpos=pos;
				ed->mins=FALSE;ed->maxs=TRUE;
			}
		}
	}
	ed->lastval=aktval;
	return(2.0/(double)ed->curlen);		/* curlen is *at least* 2 */
}

double ALIGNED ASM BShape_UserFrqEnv_PeakSearch_Stretch(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	WORD aktval;
	double ant1,ant2,spos;
	FrqEnvPeakSearch *ed=(FrqEnvPeakSearch *)(((ModUser *)md)->envdata);

	spos=(double)pos*(double)(((ModUser *)md)->modbuf->slen-1)/(double)max;
	ant1=1.0-(ant2=spos-(double)((LONG)spos));
	aktval=labs((WORD)((double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos]*ant1+(double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos+1L]*ant2));
	pos=(ULONG)spos;

	if(ed->maxs) { if(aktval<ed->lastval) { ed->maxs=FALSE;ed->mins=TRUE; } }
	else {
		if(ed->mins && aktval>ed->lastval) {
			ed->curlen=pos-ed->lastpos;
			ed->lastpos=pos;
			ed->mins=FALSE;ed->maxs=TRUE;
		}
	}
	ed->lastval=aktval;
	return(2.0/(double)ed->curlen);		/* curlen is *at least* 2 */
}

double ALIGNED ASM BShape_UserFrqEnv_Energy_Single(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	WORD aktval=0;
	FrqEnvEnergy *ed=(FrqEnvEnergy *)(((ModUser *)md)->envdata);

	if(pos<((ModUser *)md)->modbuf->slen) aktval=(double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos];

	ed->energy+=aktval;
	if((aktval<0) && (ed->energy<10)) ed->mins=TRUE;
	if((aktval>0) && (ed->energy>10) && ed->mins) {
		ed->curlen=pos-ed->lastpos;
		ed->lastpos=pos;
		ed->mins=FALSE;
	}
	return(2.0/(double)ed->curlen);		/* curlen is *at least* 2 */
}

double ALIGNED ASM BShape_UserFrqEnv_Energy_Repeat(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	WORD aktval;
	FrqEnvEnergy *ed=(FrqEnvEnergy *)(((ModUser *)md)->envdata);

	pos%=((ModUser *)md)->modbuf->slen;
	aktval=((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][pos];

	ed->energy+=aktval;
	if((aktval<0) && (ed->energy<10)) ed->mins=TRUE;
	if((aktval>0) && (ed->energy>10) && ed->mins) {
		ed->curlen=pos-ed->lastpos;
		ed->lastpos=pos;
		ed->mins=FALSE;
	}
	return(2.0/(double)ed->curlen);
}

double ALIGNED ASM BShape_UserFrqEnv_Energy_Stretch(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	WORD aktval;
	double ant1,ant2,spos;
	FrqEnvEnergy *ed=(FrqEnvEnergy *)(((ModUser *)md)->envdata);

	spos=(double)pos*(double)(((ModUser *)md)->modbuf->slen-1)/(double)max;
	ant1=1.0-(ant2=spos-(double)((LONG)spos));
	aktval=(WORD)((double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos]*ant1+(double)((ModUser *)md)->modbuf->vbuf[((ModUser *)md)->channel][(LONG)spos+1L]*ant2);
	pos=(ULONG)spos;

	ed->energy+=aktval;
	if((aktval<0) && (ed->energy<10)) ed->mins=TRUE;
	if((aktval>0) && (ed->energy>10) && ed->mins) {
		if(pos>ed->lastpos) ed->curlen=pos-ed->lastpos;
		else ed->curlen=pos;
		ed->lastpos=pos;
		ed->mins=FALSE;
	}
	return(2.0/(double)ed->curlen);		/* curlen is *at least* 2 */
}

//-- eof ----------------------------------------------------------------------
