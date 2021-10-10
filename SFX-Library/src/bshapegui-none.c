/****h* sfxsupport.library/bshapegui-none.c [4.2] *
*
*  NAME
*    bshapegui-none.c
*  COPYRIGHT
*    $VER: bshapegui-none.c 4.2 (03.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Modulator BlendShapes GUI : None - definitions
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

#define BSHAPEGUI_NONE_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator None

void ShowWin_ModNone	(Modulator *mod,Source *src1,Source *src2);

//-- definitions --------------------------------------------------------------

//-- Modulator None

void ShowWin_ModNone(Modulator *mod,Source *src1,Source *src2) {
	Describe_ModNone(mod);
}

//-- eof ----------------------------------------------------------------------
