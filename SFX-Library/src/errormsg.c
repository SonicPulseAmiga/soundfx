/****h* sfxsupport.library/errormsg.c [4.3] *
*
*  NAME
*    errormsg.c
*  COPYRIGHT
*    $VER: errormsg.c 4.3 (31.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    error message helper - definitions
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

#define ERRORMSG_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- error message helper

//-- definitions --------------------------------------------------------------

//-- error message helper

void SAVEDS	ASM LIB_EXPORT(ADosError(REG(a0,STRPTR filename),REG(d0,ULONG messid),REG(d1,ULONG adoserror),REG(a1,STRPTR file),REG(d2,ULONG line))) {
	char msg[500],err[200];

	Fault(adoserror,NULL,err,199);
	sprintf(msg,"%s: %s\n%d: %s",LIB_INTCALL(GetString(messid)),filename,adoserror,err);
	LIB_INTCALL(Message(messid,msg,file,line));
}

void SAVEDS ASM LIB_EXPORT(StdCError(REG(a0,STRPTR filename),REG(d0,ULONG messid),REG(d1,ULONG stdcerror),REG(a1,STRPTR file),REG(d2,ULONG line))) {
	char msg[500];

	sprintf(msg,"%s: %s\n%d: %s",LIB_INTCALL(GetString(messid)),filename,stdcerror,strerror(stdcerror));
	LIB_INTCALL(Message(messid,msg,file,line));
}

//-- eof ----------------------------------------------------------------------
