/****h* SoundFX/SFX-SmpGfx.h [4.2] *
*
*  NAME
*    SFX-SmpGfx.h
*  COPYRIGHT
*    $VER: SFX-SmpGfx.h 4.2 (30.10.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    sample graphic routines - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    12.Aug.1998
*  MODIFICATION HISTORY
*    30.Oct.2002	V 4.2	most recent version
*    12.Aug.1998	V 3.70	most recent version
*    12.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/


#ifndef SFX_SmpGfx_H
#define SFX_SmpGfx_H

//-- prototypes ---------------------------------------------------------------
#ifndef SFX_SmpGfx_C

	//-- sample drawing

	extern void LayoutSample(SInfo *si,UBYTE mode);
	extern void DrawSample(SInfo *si,UBYTE mode);

	//-- helplines drawing

	extern void DrawSampleExtra(SInfo *si,UBYTE channel,ULONG y,ULONG h);

	//-- Markers, Ranges, etc.

	extern void DrawLoop (SInfo *si);
	extern void DrawMark (SInfo *si);
	extern void DrawCross(SInfo *si,UWORD xpos,UWORD ypos);
	extern void DrawPPos (SInfo *si,ULONG pos);

	//-- Axis drawing

	extern void DrawAxisX(SInfo *si);
	extern void DrawAxisY(SInfo *si);
#endif

#endif /* SFX_SmpGfx_H */

//-- eof ----------------------------------------------------------------------
