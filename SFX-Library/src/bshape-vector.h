/****h* sfxsupport.library/bshape-vector.h [4.1] *
*
*  NAME
*    bshape-vector.h
*  COPYRIGHT
*    $VER: bshape-vector.h 4.1 (25.04.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Modulator BlendShapes : Vector - declarations
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

#ifndef BSHAPE_VECTOR_H
#define BSHAPE_VECTOR_H

//-- prototypes ---------------------------------------------------------------

#ifndef BSHAPE_VECTOR_C
	//-- Modulator Vector

	extern void Describe_ModVector(Modulator *mod);

	extern double		ASM BShape_Vector	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
#endif

#endif /* BSHAPE_VECTOR_H */

//-- eof ----------------------------------------------------------------------
