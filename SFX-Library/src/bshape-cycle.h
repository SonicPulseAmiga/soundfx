/****h* sfxsupport.library/bshape-cycle.h [4.3] *
*
*  NAME
*    bshape-cycle.h
*  COPYRIGHT
*    $VER: bshape-cycle.h 4.3 (14.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Modulator BlendShapes : Cycle - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    14.Oct.2003	V 4.3	most recent version
*    25.Apr.2001	V 4.1	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef BSHAPE_CYCLE_H
#define BSHAPE_CYCLE_H

//-- prototypes ---------------------------------------------------------------

#ifndef BSHAPE_CYCLE_C
	//-- Modulator Cycle

	extern void Describe_ModCycle (Modulator *mod);

	extern double		ASM BShape_Cycle_Saw (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_Cycle_Sin (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_Cycle_Sqr (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_Cycle_Tri (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_Cycle_Rnd (REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_Cycle_SRnd(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
#endif

//-- globals ------------------------------------------------------------------

#ifndef BSHAPE_C
	extern UBYTE *OsziLabels[];
#endif

#endif /* BSHAPE_CYCLE_H */

//-- eof ----------------------------------------------------------------------
