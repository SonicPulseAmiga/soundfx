/****h* SoundFX/SFX-Tools.c [4.3] *
*
*  NAME
*    SFX-Tools.c
*  COPYRIGHT
*    $VER: SFX-Tools.c 4.3 (28.01.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    various routines - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    28.Jan.2004	V 4.3	most recent version
*							A: SetInitialFileAndDrawer()
*							A: SendRexxCmd()
*    21.Feb.2003	V 4.2	most recent version
*    12.Jun.2000	V 4.00	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_Tools_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"
#include "SFX-Periodes.h"

//-- prototypes ---------------------------------------------------------------

//-- sample tools

void SetSampleLength(SInfo *si,ULONG newlen);

void TemporaryHideAllSounds(void);
void ShowAllTemporaryHiddenSounds(void);

//-- key helper ;-)

void KillAllTasks(void);

//-- arexx helper

void SendRexxCmd(STRPTR port,STRPTR cmd);

//-- file-requester helper

void SetInitialFileAndDrawer(STRPTR fn);

//-- definitions --------------------------------------------------------------

//-- sample tools

void SetSampleLength(SInfo *si,ULONG newlen) {
	if(RunTime.play && si==RunTime.plsi) StopSample();
	if(AllocSampleMem(si,newlen,CH_COUNT(si->channels))) {
		si->zoomxl=newlen;si->zoomxs=0;
		if(si->loop) {
			if(newlen<=si->loops) { si->loops=0;si->loopl=0;si->loop=0; }							// nl ls le
			if(newlen>si->loops && newlen<=si->loops+si->loopl) { si->loopl=newlen-si->loops; }		// ls nl le
			// ls le nl
		}
		if(si->mark) {
			if(newlen<=si->markxs) { si->markxs=0;si->markxl=0;si->mark=0; }							// nl ms me
			if(newlen>si->markxs && newlen<=si->markxs+si->markxl) { si->markxl=newlen-si->markxs; }	// ms nl me
			// ms me nl
		}
		SetSlider(si,1);SetTitle(si);
		DrawSample(si,0);
	}
	else {
		Message(errAllocateMemory,NULL,__FILE__,__LINE__);RemSample(si);
	}
}

/* temporary hidde all show samples
 */
void TemporaryHideAllSounds(void) {
	struct Node *snd;

	foreach(snd,&BufferList) {
		if(SFX_SOUND_IS_SHOWN((SInfo *)snd)) {
			RunTime.aktbuf=(SInfo *)snd;
			HideSample();
			((SInfo *)snd)->visibillity=SFX_SOUND_VISIBILLITY_TEMPHIDDEN;
		}
	}

}

/* show all temporarily hidden sounds
 */
void ShowAllTemporaryHiddenSounds(void) {
	struct Node *snd;

	foreach(snd,&BufferList) {                               /* alle versteckten Samples wieder anzeigen */
		if(SFX_SOUND_IS_TEMPHIDDEN((SInfo *)snd)) {
			RunTime.aktbuf=(SInfo *)snd;
			ShowSample();
		}
	}
}


//-- key helper ;-)

void KillAllTasks(void)		// I hope no machine will survive this
{
	ULONG *ptr=(ULONG *)4L;

	Forbid();
	NewList(&SysBase->TaskWait);
	NewList(&SysBase->TaskReady);
	Permit();
	*ptr=(ULONG)(rand()|1);
	Delay(100);

}

//-- arexx helper

/****** SoundFX/SendRexxCmd [0.20] *
*
*  NAME
*    SendRexxCmd -- sends a rexx command to a port
*  SYNOPSIS
*    SendRexxCmd(port,cmd)
*    void SendRexxCmd(REG(STRPTR,STRPTR);
*  FUNCTION
*    Sends the supplied comand out to the port and waits until executions is finished..
*  INPUTS
*    port - the port to contact
*    cmd  - the command to send
*  EXAMPLE
*    SendRexxCmd("PPAINT","Draw Line 10 10 20 20");
*  SEE ALSO
*
**********
*/
void SendRexxCmd(STRPTR port,STRPTR cmd)
{
	struct MsgPort *msg_rport;
	struct RexxMsg *msg_rxmsg;
	struct MsgPort *msg_port;

	if(msg_rport=CreateMsgPort()) {		/* Create the REXX reply port */
		if(msg_rxmsg=CreateRexxMsg(msg_rport,"sfx_tmp","SFX_TMP")) {
			Forbid();		// We do this under Forbid() to prevent the destination from exiting before our message gets there
			if(msg_port=FindPort(port))	{	// Get the destination port
				msg_rxmsg->rm_Action=RXCOMM;						// Fill in message
 				msg_rxmsg->rm_Args[0]=cmd;
				msg_rxmsg->rm_Args[1]=(void *)strlen(cmd);
				PutMsg(msg_port,(struct Message *)msg_rxmsg);	// Send it out
				Permit();
				WaitPort(msg_rport);							// Wait for destination's reply
				while(GetMsg(msg_rport)!=NULL);					// Get replys
			}
			else Permit();
			DeleteRexxMsg(msg_rxmsg);
		}
		DeleteMsgPort(msg_rport);
	}
}

//-- file-requester helper

void SetInitialFileAndDrawer(STRPTR fn) {
	char drive[FNSIZE],path[FMSIZE],node[FNSIZE],ext[FESIZE];
	static char idrawer[FMSIZE],ifile[FMSIZE];

	strsfn(fn,drive,path,node,ext);
	strmfn(idrawer,drive,path,NULL,NULL);
	strmfn(ifile,NULL,NULL,node,ext);

	File_Reqtags_InitialDrawer	=(ULONG)idrawer;
	File_Reqtags_InitialFile	=(ULONG)ifile;
}


//-- eof ----------------------------------------------------------------------
