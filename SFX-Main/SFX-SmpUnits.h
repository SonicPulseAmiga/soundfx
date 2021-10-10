/****h* SoundFX/SFX-SmpUnits.h [4.3] *
*
*  NAME
*    SFX-SmpUnits.h
*  COPYRIGHT
*    $VER: SFX-SmpUnits.h 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    unit conversion for sampleunits - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    12.Aug.1998
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    13.Sep.1998	V 3.71	most recent version
*    12.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_SmpUnits_H
#define SFX_SmpUnits_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_SmpUnits_C
	extern char  *ToXUnit (UBYTE unitx,ULONG val,ULONG srat);
	extern ULONG FromXUnit(UBYTE unitx,char *text,ULONG srat);
	extern char  *ToYUnit (UBYTE unity,LONG  val);
	extern LONG  FromYUnit(UBYTE unity,char *text);
#endif

#endif /* SFX_SmpUnits_H */

//-- eof ----------------------------------------------------------------------
