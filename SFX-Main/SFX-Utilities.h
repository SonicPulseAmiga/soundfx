/****h* SoundFX/SFX-Utilities.h [4.2] *
*
*  NAME
*    SFX-Utilities.h
*  COPYRIGHT
*    $VER: SFX-Utilities.h 4.2 (04.02.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    utility functions - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    04.Feb.2002
*  MODIFICATION HISTORY
*    04.Feb.2002	V 4.2 	most recent version
*    04.Feb.2002	V 4.2 	initial version
*  NOTES
*
*******
*/

#ifndef SFX_Utilities_H
#define SFX_Utilities_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_Utilities_C

	extern void UtilitiesSwapEndian(SInfo *entry);
	extern void UtilitiesSwapSign(SInfo *entry);
	extern void UtilitiesInterleave(SInfo *entry);
	extern void UtilitiesDeInterleave(SInfo *entry);

#endif

#endif /* SFX_Utilities_H */

//-- eof ----------------------------------------------------------------------

