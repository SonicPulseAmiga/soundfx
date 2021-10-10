/****h* SoundFX/SFX-Range.h [4.00] *
*
*  NAME
*    SFX-Range.h
*  COPYRIGHT
*    $VER: SFX-Range.h 4.00 (16.07.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    Range functions - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    11.Aug.1998
*  MODIFICATION HISTORY
*    16.Jul.2000	V 4.00	most recent version
*    12.Aug.1998	V 3.70	most recent version
*    11.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_Range_H
#define SFX_Range_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_Range_C
	extern void RangeX1Beg(SInfo *entry);
	extern void RangeX2End(SInfo *entry);
	extern void RangeXZero(SInfo *entry,BYTE dir);

	extern void RangeY2Top(SInfo *entry);
	extern void RangeY1Bot(SInfo *entry);
	extern void RangeYPeak(SInfo *entry,BYTE dir);

	extern void Range_Loop2Mark(SInfo *entry);
	extern void Range_Mark2Loop(SInfo *entry);
	extern void Range_Zoom2Mark(SInfo *entry);
	extern void Range_Zoom2Loop(SInfo *entry);
	extern void Range_MarkOff(SInfo *entry);
	extern void Range_MarkAll(SInfo *entry);
#endif

#endif /* SFX_Range_H */

//-- eof ----------------------------------------------------------------------
