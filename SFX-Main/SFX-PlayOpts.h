/****h* SoundFX/SFX-PlayOpts.h [3.70] *
*
*  NAME
*    SFX-PlayOpts.h
*  COPYRIGHT
*    $VER: SFX-PlayOpts.h 3.70 (13.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    options windows for player modules - declarations
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

#ifndef SFX_PlayOpts_H
#define SFX_PlayOpts_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_PlayOpts_C
	extern void OPT_Player(void);

	extern void OptP_AHI(void);
	extern void OptP_Amiga8bit(void);
	extern void OptP_Amiga14bit(void);
	extern void OptP_Amiga14cbit(void);
#endif

#endif /* SFX_PlayOpts_H */

//-- eof ----------------------------------------------------------------------
