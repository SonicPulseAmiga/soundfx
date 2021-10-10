/****h*SoundFX/SFX-ToolsRange.h [4.2] *
*
*  NAME
*    SFX-ToolsRange.h
*  COPYRIGHT
*    $VER: SFX-ToolsRange.h 4.2 (15.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    range procession tools - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    15.May.2002	V 4.2	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_ToolsRange_H
#define SFX_ToolsRange_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_ToolsRange_C
	extern void NewRangeStart (ULONG numNewStart ,ULONG numUpperBound,ULONG *numStart,ULONG *numLength);
	extern void NewRangeEnd   (ULONG numNewEnd   ,ULONG numUpperBound,ULONG *numStart,ULONG *numLength);
	extern void NewRangeLength(ULONG numNewLength,ULONG numUpperBound,ULONG *numStart,ULONG *numLength);

	extern void RangeMoveHorizontal(void);
	extern void RangeMoveVertical(void);
#endif

#endif /* SFX_ToolsRange_H */

//-- eof ----------------------------------------------------------------------
