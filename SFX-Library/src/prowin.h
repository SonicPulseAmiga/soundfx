/****h* sfxsupport.library/prowin.h [4.1] *
*
*  NAME
*    prowin.h
*  COPYRIGHT
*    $VER: prowin.h 4.1 (16.01.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    progress bar windows - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Sep.1998
*  MODIFICATION HISTORY
*    16.Jan.2001	V 4.1	most recent version
*    21.Sep.1998	V 1.20	most recent version
*    19.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef PROWIN_H
#define PROWIN_H

//-- prototypes ---------------------------------------------------------------

#ifndef PROWIN_C
	//-- asynchronous

	extern PWData * ASM LIB_EXPORT(NewProWin(void));
	extern void		ASM LIB_EXPORT(DelProWin(REG(a0,PWData *pwd)));
	extern void     ASM LIB_EXPORT(StartProWin(REG(a0,PWData *pwd),REG(a1,ULONG *curlen),REG(a2,STRPTR title),REG(d0,ULONG maxlen)));
#endif

#endif /* PROWIN_H */

//-- eof ----------------------------------------------------------------------
