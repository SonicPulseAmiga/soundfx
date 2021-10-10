/****h* sfxsupport.library/req-lstentry.h [4.1] *
*
*  NAME
*    req-lstentry.h
*  COPYRIGHT
*    $VER: req-lstentry.h 4.1 (26.08.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    get list entry requester - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Sep.1998
*  MODIFICATION HISTORY
*    26.Aug.2000	V 4.1	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    19.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef REQ_LSTENTRY_H
#define REQ_LSTENTRY_H

//-- prototypes ---------------------------------------------------------------

#ifndef REQ_LSTENTRY_C
	extern SInfo * 		ASM LIB_EXPORT(GetListEntry(void));
	extern struct Node *ASM LIB_EXPORT(GetNodeInteractive(REG(a0,struct List *list),REG(d0,ULONG index)));
#endif

#endif /* REQ_LSTENTRY_H */

//-- eof ----------------------------------------------------------------------
