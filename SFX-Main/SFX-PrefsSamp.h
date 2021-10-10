/****h* SoundFX/SFX-PrefsSamp.h [3.71] *
*
*  NAME
*    SFX-PrefsSamp.h
*  COPYRIGHT
*    $VER: SFX-PrefsSamp.h 3.71 (15.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    sample related options window - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    15.Sep.1998	V 3.71	most recent version
*							* some globals, used by CfgIO
*    13.Aug.1998	V 3.70	release version
*    13.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_PrefsSamp_H
#define SFX_PrefsSamp_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_PrefsSamp_C
	extern UBYTE PrefsSamp(void);
#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_PrefsSamp_C
	extern UBYTE *DrawModeLabels[],*SafeLabels[],*StorageLabels[],*XUnitLabels[],*YUnitLabels[],*SizeModeLabels[];
#endif

#endif /* SFX_PrefsSamp_H */

//-- eof ----------------------------------------------------------------------
