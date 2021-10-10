/****h* sfxsupport.library/bshape.h [4.1] *
*
*  NAME
*    bshape.h
*  COPYRIGHT
*    $VER: bshape.h 4.1 (25.04.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Modulator BlendShapes - declarations
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

#ifndef BSHAPE_H
#define BSHAPE_H

//-- prototypes ---------------------------------------------------------------

#ifndef BSHAPE_C
	//-- BShape to Modulator assign and initialisation

	extern void ASM LIB_EXPORT(AssignBShape(REG(a0,Modulator *mod)));
#endif

#endif /* BSHAPE_H */

//-- eof ----------------------------------------------------------------------
