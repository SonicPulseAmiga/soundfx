/****h* SoundFX/SFX-PlayAhi.h [3.70] *
*
*  NAME
*    SFX-PlayAhi.h
*  COPYRIGHT
*    $VER: SFX-PlayAhi.h 3.70 (18.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    sample playroutines for ahi - declarations
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

#ifndef SFX_PlayAhi_H
#define SFX_PlayAhi_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_PlayAhi_C
	extern void PlayAhiASample_All(void);
	extern void PlayAhiASample_Rng(void);
	extern void StopAhiASample(void);

	extern UBYTE InitAhiAPlayer(void);
	extern void  FreeAhiAPlayer(void);
#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_PlayAhi_C
	extern struct AHIAudioCtrl *audioctrl;
#endif

#endif /* SFX_PlayAhi_H */

//-- eof ----------------------------------------------------------------------
