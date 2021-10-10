/****h* SoundFX/SFX-PlayCasc.h [3.70] *
*
*  NAME
*    SFX-PlayCasc.h
*  COPYRIGHT
*    $VER: SFX-PlayCasc.h 3.70 (13.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    samples playroutines for Paula 14 bit - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    13.Aug.1998	V 3.70	most recent version
*    13.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_PlayCasc_H
#define SFX_PlayCasc_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_PlayCasc_C
	extern void PlayCascSample_All(void);
	extern void PlayCascSample_Rng(void);
	extern void StopCascSample(void);
#endif

#endif /* SFX_PlayCasc_H */

//-- eof ----------------------------------------------------------------------
