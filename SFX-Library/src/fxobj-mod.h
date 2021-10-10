/****h* sfxsupport.library/fxobj-mod.h [1.20] *
*
*  NAME
*    fxobj-mod.h
*  COPYRIGHT
*    $VER: fxobj-mod.h 1.20 (19.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    modulator object - declarations
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

#ifndef FXOBJ_MOD_H
#define FXOBJ_MOD_H

//-- prototypes ---------------------------------------------------------------

#ifndef FXOBJ_MOD_C
	//-- GUI Handling
	extern struct Gadget *	ASM LIB_EXPORT(AddModulator(REG(a0,Modulator *mod),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y)));
	extern void				ASM LIB_EXPORT(SetModulator(REG(a0,Modulator *mod),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
	extern UBYTE			ASM LIB_EXPORT(HandleModulator(REG(a0,Modulator *mod),REG(a1,Source *src1),REG(a2,Source *src2),REG(a3,struct Window *win),REG(a4,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode)));

	//-- ARexx Parsing
	extern UBYTE			ASM LIB_EXPORT(ParseModulatorSet(REG(a0,Modulator *mod),REG(a1,STRPTR prefix),REG(a2,STRPTR params)));
	extern UBYTE			ASM LIB_EXPORT(ParseModulatorGet(REG(a0,Modulator *mod),REG(a1,STRPTR prefix),REG(a2,STRPTR params),REG(a3,STRPTR ret)));
#endif

//-- globals ------------------------------------------------------------------

#ifndef FXOBJ_MOD_C
	extern UBYTE *BShapeLabels[];
#endif

#endif /* FXOBJ_MOD_H */

//-- eof ----------------------------------------------------------------------
