/****h* sfxsupport.library/fxobj-inter.h [1.20] *
*
*  NAME
*    fxobj-inter.h
*  COPYRIGHT
*    $VER: fxobj-inter.h 1.20 (19.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    interpolations object - declarations
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

#ifndef FXOBJ_INTER_H
#define FXOBJ_INTER_H

//-- prototypes ---------------------------------------------------------------

#ifndef FXOBJ_INTER_C
	//-- GUI Handling

	extern struct Gadget *	ASM LIB_EXPORT(AddInterpolator(REG(a0,Interpolator *inter),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y)));
	extern void				ASM LIB_EXPORT(SetInterpolator(REG(a0,Interpolator *inter),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
	extern UBYTE			ASM LIB_EXPORT(HandleInterpolator(REG(a0,Interpolator *inter),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode)));

	//-- ARexx Parsing

	extern UBYTE			ASM LIB_EXPORT(ParseInterpolatorSet(REG(a0,Interpolator *inter),REG(a1,STRPTR prefix),REG(a2,STRPTR params)));
	extern UBYTE			ASM LIB_EXPORT(ParseInterpolatorGet(REG(a0,Interpolator *inter),REG(a1,STRPTR prefix),REG(a2,STRPTR params),REG(a3,STRPTR ret)));
#endif

//-- globals ------------------------------------------------------------------

#ifndef FXOBJ_INTER_C
	extern UBYTE *InterpolLabels[];
	extern struct Node interpolNodes[INT_TYPE_ANZ];
	extern struct List interpolList;
#endif

#endif /* FXOBJ_INTER_H */

//-- eof ----------------------------------------------------------------------
