/*****************************************
  $VER: GetSysInfo 1.0 for SoundFX
  by Dan Jedlicka

  Compiled with StormC 3.0
  on 26 January 2002

  Requirements:
  - identify.library V13+

*****************************************/

#include <stdlib.h>

#include <proto/exec.h>
#include <proto/intuition.h>

#include <clib/identify_protos.h>
#include <pragmas/identify_pragmas.h>
#include <libraries/identify.h>


struct Library *IdentifyBase;

STRPTR string = "Computer System: $SYSTEM$\n"
				"CPU: $CPU$/$CPUCLOCK$, FPU: $FPU$/$FPUCLOCK$\n"
				"MMU: $MMU$, $CHIPSET$ chipset\n"
				"Memory: $CHIPRAM$ Chip, $FASTRAM$ Fast RAM\n"
				"\n"
				"AmigaOS $OSNR$ ($OSVER$), Workbench $WBVER$\n"
				"Exec $EXECVER$, SetPatch $SETPATCHVER$\n"
				"Graphic System: $GFXSYS$";
STRPTR buffer;  // buffer for storing the formatted string


STRPTR GetSysInfo(void)
{
 LONG   buflen = NULL;   // buffer length

 if (IdentifyBase = OpenLibrary("identify.library",13))
  {
   /*  Get the size of the buffer */
   buflen = IdEstimateFormatSize(string, NULL);

   /*  Allocate the buffer        */
   buffer = malloc(buflen);

   /*  Get the formatted string with system information
	   and return it
	*/
   IdFormatString(string, buffer, buflen, NULL);
   return buffer;
  }
 else return NULL;   /* error: couldn't open library */
}




int main (void)
{
 struct EasyStruct request;

 /*  Display a requester with the information
	 returned by GetSysInfo()
  */
 request.es_StructSize = sizeof(struct EasyStruct);
 request.es_Flags = NULL;
 request.es_Title = "System Information";
 request.es_TextFormat = GetSysInfo();
 request.es_GadgetFormat = "OK";

 EasyRequest(NULL, &request, NULL, NULL);



 /*  Free all resources
  */

 if (buffer)       free(buffer);
 if (IdentifyBase) CloseLibrary(IdentifyBase);

 return 0;
}

