/****h* sfxsupport.library/mwin.h [1.20] *
*
*  NAME
*    mwin.h
*  COPYRIGHT
*    $VER: mwin.h 1.20 (18.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    multiport-windows - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    18.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef MWIN_H
#define MWIN_H

//-- prototypes ---------------------------------------------------------------

#ifndef MWIN_C
	//-- MultiPortWindows

	extern struct Window *	ASM LIB_EXPORT(OpenMWindow(REG(a0,struct TagItem *wintags),REG(a1,struct MsgPort *userport),REG(a2,UBYTE *ct),REG(d0,ULONG idcmps)));
	extern void				ASM LIB_EXPORT(CloseMWindow(REG(a0,struct Window *win),REG(a1,UBYTE *ct)));
#endif

#endif /* MWIN_H */

//-- eof ----------------------------------------------------------------------
