/****h* SoundFX/SFX-bootwin.h [3.70] *
*
*  NAME
*    SFX-bootwin.h
*  COPYRIGHT
*    $VER: SFX-bootwin.h 3.70 (10.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    bootprogress-window for SFX - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    10.Aug.1998
*  MODIFICATION HISTORY
*    10.Aug.1998	V 3.70	most recent version
*    10.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_BootWin_H
#define SFX_BootWin_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_BootWin_C

	//-- bootwin functions

	extern void OpenGFX(void);
	extern void CloseGFX(void);
	extern void UpdateGFX(char *text,UBYTE proz);
#endif

#endif /* SFX_BootWin_H */

//-- eof ----------------------------------------------------------------------
