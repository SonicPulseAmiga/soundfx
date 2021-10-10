/****h* sfxsupport.library/interpol.h [1.20] *
*
*  NAME
*    interpol.h
*  COPYRIGHT
*    $VER: interpol.h 1.20 (19.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    Interpolation routines - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    19.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef INTERPOL_H
#define INTERPOL_H

//-- prototypes ---------------------------------------------------------------

#ifndef INTERPOL_C
	extern void ASM LIB_EXPORT(AssignInterpolFunc(REG(d0,UBYTE typ),REG(a0,InterpolFuncPtr *InterpolFunc)));
#endif

#endif /* INTERPOL_H */

//-- eof ----------------------------------------------------------------------
