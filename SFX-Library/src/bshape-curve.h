/****h* sfxsupport.library/bshape-curve.h [4.1] *
*
*  NAME
*    bshape-curve.h
*  COPYRIGHT
*    $VER: bshape-curve.h 4.1 (25.04.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Modulator BlendShapes : Curve - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    25.Apr.2001	V 4.1	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef BSHAPE_CURVE_H
#define BSHAPE_CURVE_H

//-- prototypes ---------------------------------------------------------------

#ifndef BSHAPE_CURVE_C
	//-- Modulator Curve

	extern void Describe_ModCurve (Modulator *mod);
	
	extern double		ASM BShape_Linear	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_Curve	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
#endif

#endif /* BSHAPE_CURVE_H */

//-- eof ----------------------------------------------------------------------
