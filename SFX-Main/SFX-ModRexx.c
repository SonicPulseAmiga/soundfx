/****h* SoundFX/SFX-ModRexx.c [4.2] *
*
*  NAME
*    SFX-ModRexx.c
*  COPYRIGHT
*    $VER: SFX-ModRexx.c 4.2 (09.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    sample rexx operator interface - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    18.Feb.2002
*  MODIFICATION HISTORY
*    09.May.2002	V 4.2	most recent version
*    18.Feb.2002	V 4.2	initial version
*  NOTES
*
*******
*/

#define SFX_ModRexx_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void StartRexx(void);

//-- asyncronous arexx task

void __asm ExecuteRexxTask(void);

//-- definitions --------------------------------------------------------------

void StartRexx(void)
{
#ifdef CHECK_REG
	if(IsKeyStatusOkay) {
#endif
		char *modName;
		if(modName=GetNameByIndex(&RexxList,RunTime.arxop)) {
			/* Version #1 *//*
			char cmdln[150];

			sprintf(cmdln,"sys:rexxc/rx _rexx/%s.rexx",modName);
			SystemTags(cmdln,SYS_Asynch,TRUE,SYS_Output,0L,SYS_Input,0L,TAG_DONE);
			*/
			/* Version #2 */
			static char name[100];
			UBYTE nr=0;
			struct TagItem ProcTags[]=
			{
			/* 0 */	NP_Entry,		(ULONG)ExecuteRexxTask,
			/* 1 */	NP_StackSize,	20000,
			/* 2 */	NP_Name,		0L,
			/* 3 */	NP_Priority,	0L,
			/* 4 */	TAG_DONE
			};
			struct Process *ThisTask=(struct Process *)FindTask(NULL);

			//-- check somehow that only one macro is running
			sprintf(name,SFX_PREFIX"%s.%d",modName,nr);			 // eindeutigen Tasknamen generieren
			Forbid();
			while(FindTask(name)) sprintf(name,SFX_PREFIX"%s.%d",modName,++nr);
			Permit();

			ProcTags[2].ti_Data=(ULONG)name;			// better allocate it ?, but when to free it
			ProcTags[3].ti_Data=(ULONG)(((struct Node *)ThisTask)->ln_Pri-2);	// make this two lower than calling task
			MSG("starting process");
			CreateNewProc(ProcTags);					// create new process & pass parameters
		}
#ifdef CHECK_REG
	}
	else Message(msgDemoNoARexx,NULL,NULL,0);
#endif
}

//-- asyncronous operator task

void __saveds __asm ExecuteRexxTask(void)
{
	char name[FILENAME_MAX],*tname;
	char *np,*tnp;
	struct Process *ThisTask=(struct Process *)FindTask(NULL);
	struct Window *oldWinPtr;
	struct MsgPort *srcPort,*dstPort;
	struct RexxMsg *srcMsg, *dstMsg;

	oldWinPtr=ThisTask->pr_WindowPtr;
	ThisTask->pr_WindowPtr=(APTR)RunTime.winBackDrop;

	tname=((struct Node *)ThisTask)->ln_Name;

//	  strcpy(name,"progdir:_rexx/");
//	  np=&name[14];							  // skip "progdir:_operators/"

	strcpy(name,"_rexx/");
	np=&name[6];						   // skip "progdir:_operators/"
	
	tnp=&tname[4];							// skip "SFX_" prefix
	while(*tnp!='.') *(np++)=*(tnp++);		// copy plugin name
	strcpy(np,".rexx");

	MSG1("about to run : \"%s\"",name);
	if(dstPort = FindPort("REXX")) {
		if((srcPort = CreatePort(NULL,0))) {
			if(srcMsg=CreateRexxMsg(srcPort,NULL,SFX_AREXXNAME)) {
				srcMsg->rm_Action=RXCOMM;
				srcMsg->rm_Args[0]=CreateArgstring(name, strlen(name));

				MSG("Sending message");
				PutMsg(dstPort, (struct Message *)srcMsg);
				MSG("Waiting for reply");
				WaitPort(srcPort);

				if (dstMsg = (struct RexxMsg *)GetMsg(srcPort)) {
					MSG("Got reply message");
					if(IsRexxMsg(dstMsg)) {
						MSG("okay");
					}			
					else MSG("not okay");
				}
				DeleteArgstring(srcMsg->rm_Args[0]);
				DeleteRexxMsg(srcMsg);
			}
			else Message(strNoID,"Can't create ARexx message !",__FILE__,__LINE__); /* LOCALIZE */
			DeletePort(srcPort);
		}
		else Message(errCreatePort,NULL,__FILE__,__LINE__);
	}
	else Message(strNoID,"No ARexx available !\nStart RexxMast in s:user-startup.",__FILE__,__LINE__); /* LOCALIZE */

	ThisTask->pr_WindowPtr=oldWinPtr;
}

//-- eof ----------------------------------------------------------------------

