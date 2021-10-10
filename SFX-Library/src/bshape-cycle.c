/****h* sfxsupport.library/bshape-cycle.c [4.3] *
*
*  NAME
*    bshape-cycle.c
*  COPYRIGHT
*    $VER: bshape-cycle.c 4.3 (14.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Modulator BlendShapes : Cycle - definitions
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
*  NOTES
*
*******
*/

#define BSHAPE_CYCLE_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator Cycle

void Describe_ModCycle (Modulator *mod);

double		ASM BShape_Cycle_Saw (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_Cycle_Sin (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_Cycle_Sqr (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_Cycle_Tri (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_Cycle_Rnd (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_Cycle_SRnd(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));

//-- globals ------------------------------------------------------------------

UBYTE *OsziLabels[]={ "Saw","Sin","Sqr","Tri","Rnd","SRnd",0l };

//-- definitions --------------------------------------------------------------

//-- Modulator Cycle

void Describe_ModCycle(Modulator *mod) {
	ModCycle *mc=(ModCycle *)(mod->bshpdata);

	sprintf(mod->desc,"%s : %s %s",OsziLabels[mc->oszi],mc->frq,mc->pha);
}

double ALIGNED ASM BShape_Cycle_Saw(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	double angle=((ModCycle *)md)->angle+((double)pos*((ModCycle *)md)->anglest);

	angle-=(PIM2*(double)((ULONG)(angle/PIM2)));
	return(angle/PIM2);
}

double ALIGNED ASM BShape_Cycle_Sin(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	return(
		0.5+(
			0.5*__sin(
				((ModCycle *)md)->angle+(
					(double)pos*((ModCycle *)md)->anglest
				)
			)
		)
	);
}

double ALIGNED ASM BShape_Cycle_Sqr(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	double angle=((ModCycle *)md)->angle+((double)pos*((ModCycle *)md)->anglest);

	angle-=(PIM2*(double)((ULONG)(angle/PIM2)));
	return((angle<PI) ? 0.0 : 1.0);
}

double ALIGNED ASM BShape_Cycle_Tri(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	double angle=((ModCycle *)md)->angle+((double)pos*((ModCycle *)md)->anglest);

	angle-=(PIM2*(double)((ULONG)(angle/PIM2)));
	return((angle<PI) ? (angle/PI) : ((PIM2-angle)/PI));
}

double ALIGNED ASM BShape_Cycle_Rnd(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	if(((ModCycle *)md)->anglest>0.0) ((ModCycle *)md)->anglest-=1.0;
	else {
		((ModCycle *)md)->anglest+=((ModCycle *)md)->hold;
		((ModCycle *)md)->angle=(double)rand()/(double)RAND_MAX;
	}
	return(((ModCycle *)md)->angle);
}

double ALIGNED ASM BShape_Cycle_SRnd(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	double fade;

	if(((ModCycle *)md)->anglest>0.0) {
		((ModCycle *)md)->anglest-=1.0;
		fade=((ModCycle *)md)->diff*(1.0-(((ModCycle *)md)->anglest/((ModCycle *)md)->hold));
	}
	else {
		((ModCycle *)md)->anglest+=((ModCycle *)md)->hold;
		((ModCycle *)md)->angle=((ModCycle *)md)->newrnd;
		((ModCycle *)md)->newrnd=(double)rand()/(double)RAND_MAX;
		((ModCycle *)md)->diff=((ModCycle *)md)->newrnd-((ModCycle *)md)->angle;
		fade=0.0;
	}
	return(((ModCycle *)md)->angle+fade);
}

//-- eof ----------------------------------------------------------------------
