/****h* sfxsupport.library/help.h [4.3] *
*
*  NAME
*    help.h
*  COPYRIGHT
*    $VER: help.h 4.3 (08.04.04) © by Stefan Kost 1998-12004
*  FUNCTION
*    online help routines - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    22.Aug.2000
*  MODIFICATION HISTORY
*    08.Apr.2004    V 4.3   most recent version
*    22.Aug.2000	V 4.1	most recent version
*    22.Aug.2000	V 4.1	initial version
*  NOTES
*
*******
*/

#ifndef HELP_H
#define HELP_H

//-- prototypes ---------------------------------------------------------------

#ifndef HELP_C
	//-- online help routines

	extern void		ASM LIB_EXPORT(ShowHelp(REG(a0,STRPTR nodename)));
#endif

#endif /* HELP_H */

//-- eof ----------------------------------------------------------------------
