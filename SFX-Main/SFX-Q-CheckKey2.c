/****h* SoundFX/SFX-CheckKey2.c_ [4.2] *
*
*  NAME
*    SFX-CheckKey2.c_
*  COPYRIGHT
*    $VER: SFX-CheckKey2.c_ 4.2 (24.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Keyfilechecking - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    24.Apr.2002	V 4.2	most recent version
*    23.Feb.2001    V 4.1   most recent version
*    13.Aug.1998    V 3.70  most recent version
*    13.Aug.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#define SFX_CheckKey2_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE CheckKeyFile2(void);

//-- definitions --------------------------------------------------------------

// returns 0xFF on success and 0 on failure
// can be used as a mask with &

UBYTE CheckKeyFile2(void)
{
	UBYTE ret=0xFF;

#ifdef CHECK_KEY
	INTRO;

	if(!SFXKeyBase) ret=CheckOldKeyFile2();
	else {
		if(Key_GetKeyNr  (&RunTime.keynr)) { ret=0;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("GetKeyNr failed"); }
		if(Key_GetKeyType(&ret)) { ret=0;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("GetKeyType failed"); }
		if(Key_GetRv     (&RunTime.keyrv)) { ret=0;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("GetRv failed"); }
		if(Key_GetMinMax (&RunTime.keymi,&RunTime.keyma,&RunTime.keymabits)) { ret=0;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("GetKeyMinMax failed"); }
	}
#endif
#ifdef CHECK_DELFINA
	struct Library *DelfinaBase;

	INTRO;

	if((DelfinaBase=OpenLibrary("delfina.library",0))) {
		if((Delf_AvailMem(0l)==0l)) ret=0;
		CloseLibrary(DelfinaBase);
	}
	else ret=0;
#endif
#ifdef CHECK_PRELUDE
	struct Library *PreludeBase;

	INTRO;

	if((PreludeBase=OpenLibrary("prelude.library",0))) {
		CloseLibrary(PreludeBase);
	}
	else ret=0;
#endif
#ifdef CHECK_REPULSE
	struct Library *RepulseBase;

	INTRO;

	if((RepulseBase=OpenLibrary("repulse.library",0))) {
		CloseLibrary(RepulseBase);
	}
	else ret=0;
#endif

	OUTRO;

	return(ret);
}

//-- eof ----------------------------------------------------------------------
