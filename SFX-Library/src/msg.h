/****h* sfxsupport.library/msg.h [1.20] *
*
*  NAME
*    msg.hc
*  COPYRIGHT
*    $VER: msg.h 1.20 (18.09.98) © by Stefan Kost 1998-11998
*  FUNCTION
*    error & message windows - declarations
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

#ifndef MSG_H
#define MSG_H

//-- prototypes ---------------------------------------------------------------

#ifndef MSG_C
	//-- Errors && Messages

	extern void		ASM LIB_EXPORT(Message (REG(d0,ULONG messid),REG(a0,STRPTR mess),REG(a1,STRPTR file),REG(d1,ULONG line)));
	extern long		ASM LIB_EXPORT(Question(REG(d0,ULONG messid),REG(a0,STRPTR gadfmt)));

	//-- Locale

	extern STRPTR 	ASM LIB_EXPORT(GetString(REG(d0,ULONG id)));
#endif

#endif /* MSG_C */

//-- eof ----------------------------------------------------------------------
