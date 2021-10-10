/****h*SoundFX/SFX-CheckSum1.c [3.70] *
*
*  NAME
*    SFX-CheckSum1.c
*  COPYRIGHT
*    $VER: SFX-CheckSum1.c 3.70 (14.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    keyfile checking (old) - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_CheckSum1_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UWORD CheckSum1(UWORD check);

//-- definitions --------------------------------------------------------------

UWORD CheckSum1(UWORD chk)
{
	return((UWORD)((chk&0xFFFF+chk&0xFFF+chk&0xFF+chk&0xF)&0xFFFF));
}

//-- eof ----------------------------------------------------------------------
