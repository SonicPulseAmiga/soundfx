/****h* SoundFX/SFX-CheckKey4.c_ [4.1] *
*
*  NAME
*    SFX-CheckKey4.c_
*  COPYRIGHT
*    $VER: SFX-CheckKey4.c_ 4.1 (28.11.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    Keyfilechecking - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    28.Nov.2000    V 4.1   most recent version
*    24.May.1999    V 3.81  most recent version
*    21.Apr.1999    V 3.80  most recent version
*    13.Aug.1998    V 3.70  most recent version
*    13.Aug.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#define SFX_CheckKey4_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE CheckKeyFile4(void);

//-- definitions --------------------------------------------------------------

// returns 0xFF on success and 0 on failure

UBYTE CheckKeyFile4(void)
{
	UBYTE ret=0;
#ifdef CHECK_KEY
	struct Node *keylib;
	char *name;
	UBYTE tst,i;

	INTRO;

	if(SFXKeyBase1) {   // only does the check for new keys
		Forbid();
		foreach(keylib,&(SysBase->LibList)) {	// check each library
			if((name=keylib->ln_Name)) {			// which has a name
				tst=1;
				for(i=0;i<7;i++) tst&=(name[i]!=0);
				tst&=(name[7]==0);
				tst&=(name[6]>'x' && name[6]<'z');      // y
				tst&=(name[0]>'r' && name[0]<'t');      // s
				tst&=(name[5]<'f' && name[5]>'d');      // e
				tst&=(name[2]<'y' && name[2]>'w');      // x
				tst&=(name[4]>'j' && name[4]<'l');      // k
				tst&=(name[3]>'-' && name[3]<'/');      // .
				tst&=(name[1]>'e' && name[1]<'g');      // f
				if(tst) {
					ret=0xFF;break;
				}
			}
		}
		Permit();
		ERRC(!ret,"CheckKeyfile4() failed");
	}
	else ret=0xFF;
#else
	ret=0xFF;
#endif

	OUTRO;

	return(ret);
}

//-- eof ----------------------------------------------------------------------
