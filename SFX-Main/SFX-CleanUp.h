/****h* SoundFX/SFX-CleanUp.h [3.35] *
*
*  NAME
*    SFX-CleanUp.h
*  COPYRIGHT
*    $VER: SFX-CleanUp.h 3.70 (12.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    CleanUp functions - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    22.Feb.1997
*  MODIFICATION HISTORY
*    12.Aug.1998	V 3.70	most recent version
*    22.Feb.1998	V 3.35	initial version
*  NOTES
*
*******
*/

#ifndef SFX_CleanUp_H
#define SFX_CleanUp_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_CleanUp_C
	extern void CleanUpCurr(void);
	extern void CleanUpAll(void);
	extern void CleanUpAllNorm(void);
	extern void CleanUpAllZoom(void);
#endif

#endif /* SFX_CleanUp_H */

//-- eof ----------------------------------------------------------------------
