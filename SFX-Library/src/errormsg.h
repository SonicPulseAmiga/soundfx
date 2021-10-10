/****h* sfxsupport.library/errormsg.h [4.3] *
*
*  NAME
*    errormsg.h
*  COPYRIGHT
*    $VER: errormsg.h 4.3 (31.10.03) © by Stefan Kost 1998-12003
*  FUNCTION
*    error message helper - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    31.Oct.2003
*  MODIFICATION HISTORY
*    31.Oct.2003	V 4.3	most recent version
*    31.Oct.2003	V 4.3  initial version
*  NOTES
*
*******
*/

#ifndef ERRORMSG_H
#define ERRORMSG_H

//-- prototypes ---------------------------------------------------------------

#ifndef ERRORMSG_C
	extern void ASM LIB_EXPORT(ADosError(REG(a0,STRPTR filename),REG(d0,ULONG messid),REG(d1,ULONG adoserror),REG(a1,STRPTR file),REG(d2,ULONG line)));
	extern void ASM LIB_EXPORT(StdCError(REG(a0,STRPTR filename),REG(d0,ULONG messid),REG(d1,ULONG stdcerror),REG(a1,STRPTR file),REG(d2,ULONG line)));
#endif

#endif /* ERRORMSG_H */

//-- eof ----------------------------------------------------------------------
