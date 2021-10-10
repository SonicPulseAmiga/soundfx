/****h* SoundFX/SFX-Zoom.h [3.70] *
*
*  NAME
*    SFX-Zoom.h
*  COPYRIGHT
*    $VER: SFX-Zoom.h 3.70 (12.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    Zoom functions - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    11.Aug.1998
*  MODIFICATION HISTORY
*    12.Aug.1998	V 3.70	most recent version
*    11.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_Zoom_H
#define SFX_Zoom_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_Zoom_C
	extern void ZoomIn(SInfo *entry,UBYTE mode);
	extern void ZoomOut(SInfo *entry,UBYTE mode);
	extern void ZoomPixel(SInfo *entry,UBYTE mode);
	extern void ZoomFull(SInfo *entry,UBYTE mode);
#endif

#endif /* SFX_Zoom_H */

//-- eof ----------------------------------------------------------------------
