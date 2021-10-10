/****h* sfxsupport.library/fxtools.h [4.2] *
*
*  NAME
*    fxtools.h
*  COPYRIGHT
*    $VER: fxtools.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    some fx helpers
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    01.Aug.2002	V 4.2	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef FXTOOLS_H
#define FXTOOLS_H

//-- prototypes ---------------------------------------------------------------

#ifndef FXTOOLS_C
	//-- FXTools

	extern UBYTE		ASM LIB_EXPORT(SetRngMode(REG(a0,SInfo *si)));
	extern void			ASM LIB_EXPORT(SetRngs(REG(a0,SInfo *si),REG(a1,ULONG *rm_start),REG(a2,ULONG *rm_len),REG(d0,UBYTE rmode)));
	extern ULONG		ASM LIB_EXPORT(ZeroSearch(REG(a0,SInfo *si),REG(d0,ULONG aktpos),REG(d1,BYTE dir),REG(d2,BYTE dir2),REG(d3,UBYTE chan)));
	extern void			ASM LIB_EXPORT(SetNewPlaybackRate(void));

	extern SInfo *		SetValidBuffer(ULONG *modid);

#endif

#endif /* FXTOOLS_H */

//-- eof ----------------------------------------------------------------------
