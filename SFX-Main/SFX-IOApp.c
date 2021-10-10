/****h* SoundFX/SFX-IOApp.c [4.3] *
*
*  NAME
*    SFX-IOApp.c
*  COPYRIGHT
*    $VER: SFX-IOApp.c 4.3 (15.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    AppIcon port for SoundFX - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    02.Mar.1999
*  MODIFICATION HISTORY
*    15.Oct.2003	V 4.3	most recent version
*    02.Mar.1999	V 3.80	most recent version
*    02.Mar.1999	V 3.80	initial version
*  NOTES
*
*******
*/

#define SFX_IOApp_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void DoIO_App(void);

//-- definitions --------------------------------------------------------------

void DoIO_App(void)
{
	struct AppMessage *appmsg;

	while(appmsg=(struct AppMessage *)GetMsg(appiconport)) {
		switch(appmsg->am_Class) {
			case AMCLASSICON_Open:
				MSG1("APPICON->Open %d",appmsg->am_NumArgs);
				if(!appmsg->am_NumArgs) ScreenToFront(RunTime.sfxpubscr);			// später testen, ob iconified oder nicht
				else {
					register UBYTE i;
					char fn[FILENAME_MAX],path[FNSIZE];

					for(i=0;i<appmsg->am_NumArgs;i++) {
						NameFromLock(appmsg->am_ArgList[i].wa_Lock,path,FNSIZE);
						strmfp(fn,path,appmsg->am_ArgList[i].wa_Name);
						LoadSampleRexx(fn);
					}
				}
				break;
			case AMCLASSICON_Information:
				MSG("APPICON->Information has been called");
				/** @todo display information about SFX, like about, but on WbScreen  */
				break;
		}
		ReplyMsg((struct Message *)appmsg);
	}
}

//-- eof ----------------------------------------------------------------------
