/****h* sfxsupport.library/bshape-none.c [4.2] *
*
*  NAME
*    bshape-none.c
*  COPYRIGHT
*    $VER: bshape-none.c 4.2 (03.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Modulator BlendShapes : None - definitions
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

#define BSHAPE_NONE_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator None

void Describe_ModNone  (Modulator *mod);

double		ASM BShape_None		(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));

//-- definitions --------------------------------------------------------------

//-- Modulator None

void Describe_ModNone(Modulator *mod) {
	strcpy(mod->desc,"None");
}

double ALIGNED ASM BShape_None(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md)) {
	return(0.0);
}

//-- eof ----------------------------------------------------------------------
