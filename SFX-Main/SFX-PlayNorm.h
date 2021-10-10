/****h* SoundFX/SFX-PlayNorm.h [3.70] *
*
*  NAME
*    SFX-PlayNorm.h
*  COPYRIGHT
*    $VER: SFX-PlayNorm.h 3.70 (13.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    sample playroutienes for Paula 8 bit - declarations
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

#ifndef SFX_PlayNorm_H
#define SFX_PlayNorm_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_PlayNorm_C
	extern void PlayNormSample_All(void);
	extern void PlayNormSample_Rng(void);
	extern void StopNormSample(void);
#endif

#endif /* SFX_PlayNorm_H */

//-- eof ----------------------------------------------------------------------
