/****h*SoundFX/SFX-CheckOldKey2.c_ [4.2] *
*
*  NAME
*    SFX-CheckOldKey2.c_
*  COPYRIGHT
*    $VER: SFX-CheckOldKey2.c_ 4.2 (24.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    keyfile checking (old) - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    24.Apr.2002	V 4.2	most recent version
*    20.Feb.2001	V 4.1	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_CheckOldKey2_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE CheckOldKeyFile2(void);

//-- definitions --------------------------------------------------------------

// returns 0xFF on success and 0 on failure
// can be used as a mask with &

UBYTE CheckOldKeyFile2(void)
{
	UBYTE ret=0xFF;
#ifdef CHECK_KEY
	FILE *file;

	INTRO;

	file=fopen(DecryptString("A@SV?n`|cli`v*vc}+n`|"),"rb");							// "DEVS:keyfiles/sfx.key"
	if(!file) file=fopen(DecryptString("uwjbalw?n`|cli`v*vc}+n`|"),"rb");				// "progdir:keyfiles/sfx.key"
	if(!file) file=fopen(DecryptString("uwjbalw?vc}+n`|"),"rb");						// "progdir:sfx.key"
	if(!file) {
		struct DosList *dl;
		dl=LockDosList(LDF_ALL|LDF_READ);
		if(FindDosEntry(dl,DecryptString("N@\\V"),LDF_ALL)) {							// "KEYS:"
			file=fopen(DecryptString("N@\\V?vc}+n`|"),"rb");							// "KEYS:sfx.key"
		}
		UnLockDosList(LDF_ALL|LDF_READ);
	}
	if(!file) {
		char path[FILENAME_MAX+10];
		if(GetVar(DecryptString("N@\\UDQM"),path,FILENAME_MAX,GVF_GLOBAL_ONLY)!=-1) {	// "KEYPATH"
			AddPart(path,DecryptString("vc}+n`|"),FILENAME_MAX+10);						// "sfx.key"
			file=fopen(path,"rb");
		}
	}
	if(file) {
		if(fread(&Key,sizeof(Key),1,file)) {
			fclose(file);
		}
		else ret=0;
	}
	else ret=0;

	OUTRO;
#endif
	return(ret);
}

//-- eof ----------------------------------------------------------------------
