/****h* sfxsupport.library/fxobj-cfgsel.h [4.2] *
*
*  NAME
*    fxobj-cfgsel.h
*  COPYRIGHT
*    $VER: fxobj-cfgsel.h 4.2 (07.07.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    configuration - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    27.Apr.1999
*  MODIFICATION HISTORY
*    07.Jul.2003	V 4.2	most recent version
*    27.Apr.1999	V 1.20	most recent version
*    27.Apr.1999	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef FXOBJ_CFGSEL_H
#define FXOBJ_CFGSEL_H

//-- prototypes ---------------------------------------------------------------

#ifndef FXOBJ_CFGSEL_C
	extern void			    ASM LIB_EXPORT(InitCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,STRPTR path)));
	extern void			    ASM LIB_EXPORT(DoneCfgSel(REG(a0,CfgSel *cfgsel)));

	extern struct Gadget *	ASM LIB_EXPORT(AddCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y),REG(d4,UWORD h)));
	extern void				ASM LIB_EXPORT(SetCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
	extern UBYTE			ASM LIB_EXPORT(HandleCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode),REG(d4,ULONG sec),REG(d5,ULONG mic)));
#endif

//-- globals ------------------------------------------------------------------

#ifndef FXOBJ_CFGSEL_C
#endif

#endif /* FXOBJ_CFGSEL_H */

//-- eof ----------------------------------------------------------------------
