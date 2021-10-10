/****h* sfxsupport.library/fxtools-gui.h [4.2] *
*
*  NAME
*    fxtools-gui.h
*  COPYRIGHT
*    $VER: fxtools-gui.h 4.2 (07.07.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    some fx gui helpers
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    24.Mar.2003
*  MODIFICATION HISTORY
*    07.Jul.2003    V 4.2   most recent version
*    24.Mar.2003    V 4.2   initial version
*  NOTES
*
*******
*/

#ifndef FXTOOLS_GUI_H
#define FXTOOLS_GUI_H

//-- prototypes ---------------------------------------------------------------

#ifndef FXTOOLS_GUI_C
	//-- GUI-Tools / Gadgetabfrage,-verwaltung

	extern void 		ASM LIB_EXPORT(DrawGadWB(REG(a0,struct RastPort *rp),REG(d0,UWORD x),REG(d1,UWORD y),REG(d2,UWORD w),REG(d3,UWORD h)));
	extern void 		ASM LIB_EXPORT(DrawGadBW(REG(a0,struct RastPort *rp),REG(d0,UWORD x),REG(d1,UWORD y),REG(d2,UWORD w),REG(d3,UWORD h)));
	extern void         ASM LIB_EXPORT(DrawTitle(REG(a0,struct RastPort *rp),REG(a1,STRPTR title),REG(d0,UWORD x),REG(d1,UWORD y),REG(d2,UWORD w)));
	extern void			ASM LIB_EXPORT(RText(REG(a0,struct RastPort *rp),REG(a1,STRPTR text),REG(d0,UWORD x),REG(d1,UWORD y)));
	extern void			ASM LIB_EXPORT(CheckGad_int(REG(a0,struct Gadget *g),REG(a1,struct Window *win),REG(a2,LONG   *val),REG(d0,UBYTE chkrng),REG(d1,LONG minv),REG(d2,LONG maxv)));
	extern void			ASM LIB_EXPORT(CheckGad_dbl(REG(a0,struct Gadget *g),REG(a1,struct Window *win),REG(a2,double *val),REG(d0,UBYTE chkrng),REG(d1,LONG minv),REG(d2,LONG maxv)));
	extern void			ASM LIB_EXPORT(HandleParameter(REG(a0,STRPTR src),REG(a1,struct Window *win),REG(a2,struct Gadget *g)));

#endif

#endif /* FXTOOLS_GUI_H */

//-- eof ----------------------------------------------------------------------
