/****h*SoundFX/SFX-CheckKeyHelper.c [4.1] *
*
*  NAME
*    SFX-CheckKeyHelper.c
*  COPYRIGHT
*    $VER: SFX-CheckKeyHelper.c 4.1 (20.02.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    keyfile checking helper - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    20.Feb.2001
*  MODIFICATION HISTORY
*    20.Feb.2001	V 4.1	most recent version
*    20.Feb.2001	V 4.1	initial version
*  NOTES
*
*******
*/

#define SFX_CheckKeyHelper_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

STRPTR DecryptString(STRPTR str);

//-- definitions --------------------------------------------------------------

STRPTR DecryptString(STRPTR str)
{
	static char res[256];
	STRPTR pres;
	register UWORD i,j;

	pres=res;j=strlen(str);
	for(i=0;i<min(j,255);i++) {
		*(pres++)=*(str++)^0x05;
	}
	*pres='\0';
	return(res);
}

//-- eof ----------------------------------------------------------------------
