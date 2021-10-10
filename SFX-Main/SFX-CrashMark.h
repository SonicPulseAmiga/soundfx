/****h* SoundFX/SFX-CrashMark.h [4.2] *
*
*  NAME
*    SFX-CrashMark.h
*  COPYRIGHT
*    $VER: SFX-CrashMark.h 4.2 (14.07.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    crash recovery for sfx - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Jul.2003
*  MODIFICATION HISTORY
*    14.Jul.2003	V 4.2	most recent version
*    14.Jul.2003    V 4.2   initial version
*  NOTES
*
*******
*/

#ifndef SFX_CRASHMARK_H
#define SFX_CRASHMARK_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_CRASHMARK_C
	extern void CheckCrashMark(void);
	extern void DeleteCrashMark(void);
#endif

#endif /* SFX_CRASHMARK_H */

//-- eof ----------------------------------------------------------------------
