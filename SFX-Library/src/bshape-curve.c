/****h* sfxsupport.library/bshape-curve.c [4.2] *
*
*  NAME
*    bshape-curve.c
*  COPYRIGHT
*    $VER: bshape-curve.c 4.2 (03.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Modulator BlendShapes : Curve - definitions
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

#define BSHAPE_CURVE_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator Curve

void Describe_ModCurve (Modulator *mod);

double		ASM BShape_Linear	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
double		ASM BShape_Curve	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));

//-- definitions --------------------------------------------------------------

//-- Modulator Curve

void Describe_ModCurve(Modulator *mod) {
	ModCurve *mc=(ModCurve *)(mod->bshpdata);

	if(mc->exp==1.0) sprintf(mod->desc,"Linear Exp. : 1.0");
	else if(mc->exp>1.0) sprintf(mod->desc,"SpeedUp Exp. : %f",mc->exp);
	else if(mc->exp<1.0) sprintf(mod->desc,"SlowDown Exp. : %f",mc->exp);
}

double ALIGNED ASM BShape_Linear(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	return((double)pos/(double)max);
}

double ALIGNED ASM BShape_Curve(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	return(__mypow_nn((double)pos/(double)max,((ModCurve *)md)->exp));
}

//-- eof ----------------------------------------------------------------------
