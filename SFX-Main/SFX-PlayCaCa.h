/****h* SoundFX/SFX-PlayCaCa.h [3.70] *
*
*  NAME
*    SFX-PlayCaCa.h
*  COPYRIGHT
*    $VER: SFX-PlayCaCa.h 3.70 (18.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    sample playroutines for Paula 14 bit calibrated - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    18.Aug.1998	V 3.70	most recent version
*    13.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_PlayCaCa_H
#define SFX_PlayCaCa_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_PlayCaCa_C
	extern void PlayCaCaSample_All(void);
	extern void PlayCaCaSample_Rng(void);
	extern void StopCaCaSample(void);

	extern UBYTE InitCaCaPlayer(void);
	extern void FreeCaCaPlayer(void);
#endif

#endif /* SFX_PlayCaCa_H */

//-- includes -----------------------------------------------------------------

//-- prototypes ---------------------------------------------------------------

//-- defines ------------------------------------------------------------------

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
