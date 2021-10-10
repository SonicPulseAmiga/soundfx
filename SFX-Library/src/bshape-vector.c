/****h* sfxsupport.library/bshape-vector.c [4.2] *
*
*  NAME
*    bshape-vector.c
*  COPYRIGHT
*    $VER: bshape-vector.c 4.2 (03.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Modulator BlendShapes : Vector - definitions
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

#define BSHAPE_VECTOR_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator Vector

void Describe_ModVector(Modulator *mod);

double		ASM BShape_Vector	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));

//-- definitions --------------------------------------------------------------

//-- Modulator Vector

void Describe_ModVector(Modulator *mod) {
	ModVector *mv=(ModVector *)(mod->bshpdata);

	sprintf(mod->desc,"Vector Points : %d",mv->anz);
}

double ALIGNED ASM BShape_Vector(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	//double erg;
	if(pos>=(ULONG)(max*((ModVector *)md)->pos[((ModVector *)md)->ix])) {					// Stützstelle ?
		while((pos>=(ULONG)(max*((ModVector *)md)->pos[((ModVector *)md)->ix])) && (((ModVector *)md)->ix<((ModVector *)md)->anz)) {
			((ModVector *)md)->lpos=((ModVector *)md)->pos[((ModVector *)md)->ix];
			((ModVector *)md)->llev=((ModVector *)md)->lev[((ModVector *)md)->ix];
			((ModVector *)md)->ix++;
		}
		((ModVector *)md)->dfc=(((ModVector *)md)->lev[((ModVector *)md)->ix]-((ModVector *)md)->llev)/
							   (((ModVector *)md)->pos[((ModVector *)md)->ix]-((ModVector *)md)->lpos);
	}
	//erg=((ModVector *)md)->llev+((ModVector *)md)->dfc*(((double)pos/(double)max)-((ModVector *)md)->lpos);
	//if(erg>1.0) erg=1.0;
	//elseif(erg<0.0) erg=0.0;
	return(((ModVector *)md)->llev+((ModVector *)md)->dfc*(((double)pos/(double)max)-((ModVector *)md)->lpos));
}

//-- eof ----------------------------------------------------------------------
