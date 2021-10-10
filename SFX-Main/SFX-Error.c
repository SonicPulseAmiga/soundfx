/****h* SoundFX/SFX-Error.c [4.1] *
*
*  NAME
*    SFX-Error.c
*  COPYRIGHT
*    $VER: SFX-Error.c 4.1 (02.08.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    Error helper - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    02.Aug.2000
*  MODIFICATION HISTORY
*    02.Aug.2000	V 4.1	most recent version
*    02.Aug.2000	V 4.1	initial version
*  NOTES
*
*******
*/

#define SFX_Error_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

STRPTR ErrorInfo(void);
STRPTR stroserror(int errornum);

//-- definitions --------------------------------------------------------------

STRPTR ErrorInfo(void)
{
	static char msg[1024],str[256];
	int C_Error=errno;
	long IOError=IoErr();
	int OSError=_OSERR;
	ULONG pos;

	if(C_Error) pos=sprintf(msg,"  errno : %s\n",strerror(C_Error));
	if(OSError) pos+=sprintf(&msg[pos],"  oserr : %s\n",stroserror(OSError));
	if(IOError) { Fault(IOError,"  ioerr ",str,255);sprintf(&msg[pos],"%s\n",str); }
	return(msg);
}

STRPTR stroserror(int errornum)
{
	static char *deferr="unknown",*err;
	long i=0;

	err=deferr;
	while(i!=-1 && __os_errlist[i].errnum) {
		if(__os_errlist[i].errnum==errornum) {
			err=__os_errlist[i].msg;i=-1;
		}
		else i++;
	}
	return(err);
}

//-- eof ----------------------------------------------------------------------
