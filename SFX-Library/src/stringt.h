/****h* sfxsupport.library/stringt.h [4.1] *
*
*  NAME
*    string.h
*  COPYRIGHT
*    $VER: string.h 4.1 (26.08.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    string related helpers - declarations
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

#ifndef STRINGT_H
#define STRINGT_H

//-- prototypes ---------------------------------------------------------------

#ifndef STRINGT_C
	extern void		ASM LIB_EXPORT(FilterString(REG(a0,STRPTR strbuf)));
	extern void		ASM LIB_EXPORT(StripString(REG(a0,STRPTR strbuf)));
	extern void 	ASM LIB_EXPORT(TrimString(REG(a0,STRPTR strbuf)));
	extern STRPTR 	ASM LIB_EXPORT(StringCopy(REG(a0,STRPTR src)));
#endif

#endif /* STRINGT_H */

//-- eof ----------------------------------------------------------------------
