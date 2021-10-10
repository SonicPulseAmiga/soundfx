/****h*SoundFX/SFX-PrefsMisc.h [4.3] *
*
*  NAME
*    SFX-PrefsMisc.h
*  COPYRIGHT
*    $VER: SFX-PrefsMisc.h 4.3 (14.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    miscelaneous options window - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    14.Oct.2003	V 4.3	most recent version
*    11.Jan.2001	V 4.1	most recent version
*    16.Jul.2000	V 4.00	most recent version
*    27.Apr.1999	V 3.81	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_PrefsMisc_H
#define SFX_PrefsMisc_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_PrefsMisc_C
	extern UBYTE PrefsMisc(void);
#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_PrefsMisc_C
	extern UBYTE *SrcSelLabels[];
	extern UBYTE *ContextButtonLabels[];
	extern UBYTE *AskExitLabels[];
	extern UBYTE *LogModeLabels[];
	extern UBYTE *DefToolLabels[];
#endif

#endif /* SFX_PrefsMisc_H */

//-- eof ----------------------------------------------------------------------
