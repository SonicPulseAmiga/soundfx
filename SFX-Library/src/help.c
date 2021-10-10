/****h* sfxsupport.library/help.c [4.3] *
*
*  NAME
*    help.c
*  COPYRIGHT
*    $VER: help.c 4.3 (08.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    online help routines - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    22.Aug.2000
*  MODIFICATION HISTORY
*    08.Apr.2004    V 4.3   most recent version
*    08.Feb.2002	V 4.2	most recent version
*							C: now does just nothing if there is no OpenURL.library
*    22.Aug.2000	V 4.1	most recent version
*    22.Aug.2000	V 4.1	initial version
*  NOTES
*
*******
*/

#define HELP_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- online help routines

void		ASM LIB_EXPORT(ShowHelp(REG(a0,STRPTR nodename)));

//-- definitions --------------------------------------------------------------

//-- online help routines

void SAVEDS ASM LIB_EXPORT(ShowHelp(REG(a0,STRPTR nodename))) {
	char path[FILENAME_MAX],*node;
	BPTR dirlock;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(nodename,__FUNC__,"1.par",4));
	//MSG1("html : [%s]",nodename);

	if(OpenURLBase) {
		if(strncmp(nodename,"http://",7)
		&& strncmp(nodename,"file://",7)
		&& strncmp(nodename,"mailto:",7)) {
			if((dirlock=Lock("progdir:",ACCESS_READ))) {
				NameFromLock(dirlock,path,FILENAME_MAX-1);
				UnLock(dirlock);
				if((node=AllocVec(strlen(path)+strlen(nodename)+20,MEMF_ANY))) {
					sprintf(node,"file:///%s/%s",path,nodename);
					//SetDefaultPubScreen(SFX_PUBSCRNAME);
					URL_Open(node,URL_PubScreenName,SFX_PUBSCRNAME,TAG_END);
					//SetDefaultPubScreen(NULL);
					FreeVec(node);
				}
				else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));
			}
		}
		else {
			URL_Open(nodename,URL_PubScreenName,SFX_PUBSCRNAME,TAG_END);
		}
	}
}

//-- eof ----------------------------------------------------------------------
