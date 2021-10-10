/****h* sfxsupport.library/fxobj-wfkt.h [1.20] *
*
*  NAME
*    fxobj-wfkt.h
*  COPYRIGHT
*    $VER: fxobj-wfkt.h 1.20 (20.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    windowfunction object - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    20.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef FXOBJ_WFKT_H
#define FXOBJ_WFKT_H

//-- prototypes ---------------------------------------------------------------

#ifndef FXOBJ_WFKT_C
	//-- GUI Handling

	extern struct Gadget *	ASM LIB_EXPORT(AddWinFunction(REG(a0,WinFunction *wfkt),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y)));
	extern void				ASM LIB_EXPORT(SetWinFunction(REG(a0,WinFunction *wfkt),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
	extern UBYTE			ASM LIB_EXPORT(HandleWinFunction(REG(a0,WinFunction *wfkt),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode)));

	//-- ARexx Parsing

	extern UBYTE			ASM LIB_EXPORT(ParseWinFunctionSet(REG(a0,WinFunction *wfkt),REG(a1,STRPTR prefix),REG(a2,STRPTR params)));
	extern UBYTE			ASM LIB_EXPORT(ParseWinFunctionGet(REG(a0,WinFunction *wfkt),REG(a1,STRPTR prefix),REG(a2,STRPTR params),REG(a3,STRPTR ret)));
#endif

//-- globals ------------------------------------------------------------------

#ifndef FXOBJ_WFKT_C
	extern UBYTE *WFktLabels[];
	extern struct Node wfktNodes[WIN_ANZ];
	extern struct List wfktList;
#endif

#endif /* FXOBJ_WFKT_H */

//-- eof ----------------------------------------------------------------------
