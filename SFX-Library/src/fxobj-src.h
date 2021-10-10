/****h* sfxsupport.library/fxobj-src.h [4.1] *
*
*  NAME
*    fxobj-src.h
*  COPYRIGHT
*    $VER: fxobj-src.h 4.1 (27.08.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    source object - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    27.Aug.2000	V 4.1	most recent version
*    29.Apr.1999	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef FXOBJ_SRC_H
#define FXOBJ_SRC_H

//-- prototypes ---------------------------------------------------------------

#ifndef FXOBJ_SRC_C
	extern struct Gadget *	ASM LIB_EXPORT(AddSource(REG(a0,Source *src),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y)));
	extern void				ASM LIB_EXPORT(SetSource(REG(a0,Source *src),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
	extern UBYTE			ASM LIB_EXPORT(HandleSource(REG(a0,Source *src),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode)));
#endif

//-- globals ------------------------------------------------------------------

#ifndef FXOBJ_SRC_C
	extern UBYTE *RModeLabels[];
#endif

#endif /* FXOBJ_SRC_H */

//-- eof ----------------------------------------------------------------------
