/****h* sfxsupport.library/asyncio.c [4.2] *
*
*  NAME
*    asyncio.c
*  COPYRIGHT
*    $VER: asyncio.c 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    asynchronous disk IO - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    20.Jul.1998
*  MODIFICATION HISTORY
*    06.Mar.2003	V 4.2	most recent version
*    26.Sep.2000	V 4.1	most recent version
*    12.Jun.2000	V 1.30	most recent version
*    31.Oct.1998	V 1.20	most recent version
*    20.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define ASYNCIO_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- asynchronous disk io

UBYTE	ASM LIB_EXPORT(ASOpen (REG(a0,SmpAccess *sa),REG(a1,STRPTR fn),REG(d0,int file_mode)));
void	ASM LIB_EXPORT(ASClose(REG(a0,SmpAccess *sa)));

UBYTE	ASM LIB_EXPORT(ASRead	(REG(a0,SmpAccess *sa),REG(d0,ULONG length)));
UBYTE	ASM LIB_EXPORT(ASReadIRQ(REG(a0,SmpAccess *sa),REG(d0,ULONG length)));
UBYTE	ASM LIB_EXPORT(ASWrite	(REG(a0,SmpAccess *sa),REG(d0,ULONG length)));

void	ASM ASReadTask (void);
void	ASM ASWriteTask(void);

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

//-- asynchronous disk io

UBYTE SAVEDS ASM LIB_EXPORT(ASOpen(REG(a0,SmpAccess *sa),REG(a1,STRPTR fn),REG(d0,int file_mode))) {
	UBYTE fail=FALSE;
	static char name[100];
	UBYTE nr=0;
	struct TagItem ProcTags[]= {
	/* 0 */	NP_Entry,		0l,
	/* 1 */	NP_StackSize,	10000,
	/* 2 */	NP_Name,		0L,
	/* 3 */	NP_Priority,	0L,
	/* 4 */	TAG_DONE
	};
	struct Process *ThisTask=(struct Process *)FindTask(NULL);
	struct Process *ThatTask;

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(fn,__FUNC__,"2.par",4));
	//MSG2("fn=<%s>  file_mode=%d",fn,file_mode);

	if((sa->mainsig=AllocSignal(-1))==-1) fail=TRUE;
	if(!fail) {
		//MSG("  Signal okay");
		
		sa->maintask	=(struct Process *)FindTask(NULL);		// set up parameters for IO Tasks
		sa->worktask	=0l;
		sa->worksig		=-1;
		sa->state		=STATE_READY;
		sa->aseg=sa->seg=0;
		sa->fn			=fn;
		sa->file_mode	=file_mode;
		sa->seek		=-1;		// no seek
		sa->last_error_no=0;

		if(file_mode==O_RDONLY) {
			//MSG("  use Readtask");
			ProcTags[0].ti_Data=(ULONG)ASReadTask;					// which task
			sprintf(name,SFX_PREFIX"IORead.%d",nr);					// generate unique task name
			Forbid();
			while(FindTask(name)) sprintf(name,SFX_PREFIX"IORead.%d",++nr);
			Permit();
		}
		else if(file_mode==(O_RDWR|O_CREAT|O_APPEND)) {
			//MSG("  use Writetask");
			ProcTags[0].ti_Data=(ULONG)ASWriteTask;
			sprintf(name,SFX_PREFIX"IOWrite.%d",nr);					// generate unique task name
			Forbid();
			while(FindTask(name)) sprintf(name,SFX_PREFIX"IOWrite.%d",++nr);
			Permit();
		}
		else {
			ERR1("Invalid file_mode ! : %ld",file_mode);
			fail=TRUE;
		}

		if(!fail) {
			ProcTags[2].ti_Data=(ULONG)name;			// better allocate it ?, but when to free it
			ProcTags[3].ti_Data=(ULONG)(((struct Node *)ThisTask)->ln_Pri+1);	// make this one higher than calling task
			Forbid();
			if(ThatTask=CreateNewProc(ProcTags)) ThatTask->pr_Task.tc_UserData=sa;		// create Task & pass parameters
			Permit();

			Wait(1L<<sa->mainsig);														// wait for task to say hello
			if(sa->worksig!=-1 && sa->worktask!=NULL && sa->fn!=NULL) { 				// is task okay ?
				//MSG("  WorkSignal && Task okay");
				if(file_mode==O_RDONLY && sa->seganz) {
					//MSG("# PreRead #");
					LIB_INTCALL(ASRead(sa,sa->blen));					// start first read, but only if seganz>0
				}
			}																				// if seganz=0 then we work with segments of variing size
			else fail=TRUE;
		}
	}
	if(fail) LIB_INTCALL(ASClose(sa));

	OUTRO_BOOL(fail);
	return(fail);
}

void SAVEDS ASM LIB_EXPORT(ASClose(REG(a0,SmpAccess *sa))) {
	//INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));

	if(sa->worksig!=-1 && sa->worktask) {	// subtask is running
		Forbid();								// we don't have to be interrupted
		//MSG1("  sa->state %ld",sa->state);
		while(sa->state!=STATE_READY) {			// because of ReadIRQ, we may have to do this twice (or perhaps even more)
			sa->state=STATE_NOTIFY;
			Permit();
			//MSG("    ASClose wait for notification");
			Wait(1L<<sa->mainsig);
			Forbid();							// we don't have to be interrupted
		}
		Permit();
		//MSG("  ASClose middle");
		sa->state=STATE_FINISH;
		Signal(sa->worktask,(1L<<sa->worksig));					// {M->S:01} start request
		//MSG("  ASClose waiting");
		Wait(1L<<sa->mainsig);									// {S->M:04} wait for task to say he's ready to shutdown
		//MSG("  ASClose ready");
	}
	if(sa->mainsig!=-1) { FreeSignal(sa->mainsig);sa->mainsig=-1; }

	//OUTRO;
}

UBYTE SAVEDS ASM LIB_EXPORT(ASRead(REG(a0,SmpAccess *sa),REG(d0,ULONG length))) {
	__builtin_putreg(REG_A4,sa->a4);
	__builtin_putreg(REG_A6,sa->a6);

	//INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));
	// length=0 is allowed for last read
	//ASSERT(length>0);

	Forbid();							// we don't have to be interrupted
	if(sa->state==STATE_WORKING) {
		sa->state=STATE_NOTIFY;
		Permit();
		//MSG("      ASRead wait for notification");
		Wait(1L<<sa->mainsig);					// {S->M:03} for for last read to finish
	}
	else Permit();

	//MSG1("      ASRead fpos %6ld",lseek(sa->fh,0,SEEK_CUR));
	memcpy(sa->buffer1,sa->buffer2,sa->cblen);			// copy data already read
	//MSG("      after memcopy");
	ASSERT_RESET;
	ASSERT(length<=sa->blen);
	if(ASSERT_OKAY) sa->cblen=length;
	else {
		sa->cblen=sa->blen;
		ERR2("length=%5d > sa->blen=%5d",length,sa->blen);
	}
	if(length) {
		Signal(sa->worktask,(1L<<sa->worksig));				// {M->S:01} start next read
		Wait(1L<<sa->mainsig);								// {S->M:02} wait for task to get information
	}
	//OUTRO;
	if(sa->last_error_no) return(FALSE);
	else return(TRUE);
}

UBYTE SAVEDS ASM LIB_EXPORT(ASReadIRQ(REG(a0,SmpAccess *sa),REG(d0,ULONG length))) {
	__builtin_putreg(REG_A4,sa->a4);
	__builtin_putreg(REG_A6,sa->a6);

	//INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));
	ASSERT(length>0);

	__Forbid();							// we don't have to be interrupted
	if(sa->state!=STATE_WORKING) {
		__Permit();

		//MSG2("      ASRead seg,aseg %ld  %ld",sa->seg,sa->aseg);
		memcpy(sa->buffer1,sa->buffer2,sa->cblen);			// copy data already read
		//MSG("      after memcopy");
		ASSERT_RESET;
		ASSERT(length<=sa->blen);
		if(ASSERT_OKAY) sa->cblen=length;
		else {
			sa->cblen=sa->blen;
			ERR2("length=%5d > sa->blen=%5d",length,sa->blen);
		}
		if(length) {
			__Signal(sa->worktask,(1L<<sa->worksig));				// {M->S:01} start next read
			//__Wait(1L<<sa->mainsig);								// {S->M:02} wait for task to get information
		}
	}
	else __Permit();
	//OUTRO;
	if(sa->last_error_no) return(FALSE);
	else return(TRUE);
}

UBYTE SAVEDS ASM LIB_EXPORT(ASWrite(REG(a0,SmpAccess *sa),REG(d0,ULONG length))) {
	__builtin_putreg(REG_A4,sa->a4);
	__builtin_putreg(REG_A6,sa->a6);

	//INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));
	ASSERT(length>0);

	Forbid();							// we don't have to be interrupted
	if(sa->state==STATE_WORKING) {
		sa->state=STATE_NOTIFY;
		Permit();
		//MSG("      ASWrite wait for notification");
		Wait(1L<<sa->mainsig);
	}
	else Permit();

	//MSG2("      ASRead seg,aseg %ld  %ld",sa->seg,sa->aseg);

	ASSERT_RESET;
	ASSERT(length<=sa->blen);
	if(ASSERT_OKAY) sa->cblen=length;
	else {
		sa->cblen=sa->blen;
		ERR2("length=%5d > sa->blen=%5d",length,sa->blen);
	}
	memcpy(sa->buffer2,sa->buffer1,sa->cblen);			// copy data to write
	Signal(sa->worktask,(1L<<sa->worksig));				// start next write
	Wait(1L<<sa->mainsig);								// wait for task to get information

	//OUTRO;
	if(sa->last_error_no) return(FALSE);
	else return(TRUE);
}

void ASM ASReadTask(void) {
	struct Process *ThisTask;
	SmpAccess *sa;
	UBYTE fail,running;
	int count;

	ThisTask=(struct Process *)FindTask(NULL);
	if(sa=ThisTask->pr_Task.tc_UserData) {
		__builtin_putreg(REG_A4,sa->a4);
		__builtin_putreg(REG_A6,sa->a6);

		ThisTask->pr_WindowPtr=(APTR)GetRunTime(winBackDrop);
		fail=FALSE;running=TRUE;
		//MSG("  RT is running");
		sa->worktask=ThisTask;								// maintask will check those fields to see if we're okay
		if((sa->worksig=AllocSignal(-1))==-1) fail=TRUE;
		if((sa->fh=open(sa->fn,sa->file_mode,0l))==-1) {
			fail=TRUE;
			sa->last_error_no=errno;
		}
		//kprintf("    flags=0x%lx fn=%s\n",__ufbs[sa->fh].ufbflg,__ufbs[sa->fh].ufbfn);
		//kprintf("    fh=%6ld err=%3ld,%3ld fpos=%6ld\n",sa->fh,errno,_OSERR,lseek(sa->fh,0l,SEEK_CUR));
		Signal(sa->maintask,(1L<<sa->mainsig));			// say hello
		if(!fail) {
			if(sa->start) {
				//kprintf("  seek to startingpoint\n");
				lseek(sa->fh,sa->start,SEEK_SET);
			}
			//kprintf("    RT Resources okay\n");
			while(running) {
				Wait(1L<<sa->worksig);					// {M->S:01} wait for request
				//kprintf("    RT[%ld] got request %ld %ld\n",sa->aseg,sa->state,sa->cblen);
				Forbid();
				if(sa->state==STATE_FINISH) { running=0;Permit(); }			// we're ready
				else {														// we've got to work
					sa->state=STATE_WORKING;
					Permit();
					if(sa->seek>-1) { lseek(sa->fh,sa->seek,SEEK_SET);sa->seek=-1; }
					Signal(sa->maintask,(1L<<sa->mainsig));			// {S->M:02} we got all information
					//MSG3("    before read fh %08lx  buf %08lx  len %ld",sa->fh,sa->buffer2,sa->cblen);
					count=read(sa->fh,sa->buffer2,sa->cblen);
					if((count<1) || (count>sa->cblen)) sa->last_error_no=errno;				// read block
					//kprintf("    RT[%ld] err=%3ld,%3ld fpos=%6ld cnt=%6ld\n",sa->aseg,errno,_OSERR,lseek(sa->fh,0l,SEEK_CUR),count);
					//MSG3("    RT[%ld] err : %ld %ld",sa->aseg,errno,_OSERR);
					//sa->aseg++;
					Forbid();
					if(sa->state==STATE_NOTIFY)	{					// is main already waiting for the data
						sa->state=STATE_READY;
						Permit();
						Signal(sa->maintask,(1L<<sa->mainsig));		// {S->M:03} tell main that we've got it
					}
					else { sa->state=STATE_READY;Permit(); }
				}
			}
			//MSG("  RT is cleaning up");
		}
		else {
			//kprintf("ReadTask.init failed\n");
		}
		sa->worktask=0l;
		if(sa->worksig!=-1) { FreeSignal(sa->worksig);sa->worksig=-1; }
		if(sa->fh!=-1) {
			if(close(sa->fh)==-1) {
				ERR3("WT : error closing file \"%s\" : %ld %s\n",sa->fn,errno,strerror(errno));
				sa->last_error_no=errno;
			}
		}
		//MSG("  RT finishes");
		if(!fail) Signal(sa->maintask,(1L<<sa->mainsig));			// {S->M:04} tell main that we're ready
	}
	else Signal(sa->maintask,(1L<<sa->mainsig));
}

void ASM ASWriteTask(void) {
	struct Process *ThisTask;
	SmpAccess *sa;
	UBYTE fail,running;
	
	ThisTask=(struct Process *)FindTask(NULL);
	if(sa=ThisTask->pr_Task.tc_UserData) {
		__builtin_putreg(REG_A4,sa->a4);
		__builtin_putreg(REG_A6,sa->a6);

		ThisTask->pr_WindowPtr=(APTR)GetRunTime(winBackDrop);
		fail=FALSE;running=TRUE;
		//MSG("  WT is running");
		sa->worktask=ThisTask;								// maintask will check those fields to see if we're okay
		if((sa->worksig=AllocSignal(-1))==-1) fail=TRUE;
		if((sa->fh=open(sa->fn,sa->file_mode,0l))==-1) {
			fail=TRUE;
			sa->last_error_no=errno;
		}
		Signal(sa->maintask,(1L<<sa->mainsig));			// say hello
		if(!fail) {
			if(sa->start) {
				//kprintf("  seek to startingpoint\n");
				//if(lseek(sa->fh,sa->start,SEEK_SET)<0) kprintf("WT lseek : %s\n",strerror(errno));
				lseek(sa->fh,sa->start,SEEK_SET);
			}
			while(running) {
				Wait(1L<<sa->worksig);					// wait for request
				Forbid();
				if(sa->state==STATE_FINISH) { running=0;Permit(); }		// we're ready
				else {													// we've got to work
					sa->state=STATE_WORKING;
					Permit();
					Signal(sa->maintask,(1L<<sa->mainsig));
					if(write(sa->fh,sa->buffer2,sa->cblen)==-1) sa->last_error_no=errno;	// write block
					Forbid();
					if(sa->state==STATE_NOTIFY)	{				// is main already waiting for writing the next block
						sa->state=STATE_READY;
						Permit();
						Signal(sa->maintask,(1L<<sa->mainsig));	// tell main that we've done it
					}
					else { sa->state=STATE_READY;Permit(); }
				}
			}
		}
		else {
			//kprintf("WriteTask.init failed\n");
		}
		sa->worktask=0l;
		if(sa->worksig!=-1) { FreeSignal(sa->worksig);sa->worksig=-1; }
		//MSG1("WT : before close() , %ld",sa->fh);
		if(sa->fh!=-1) {
			if(close(sa->fh)==-1) {
				ERR3("WT : error closing file \"%s\" : %ld %s\n",sa->fn,errno,strerror(errno));
				sa->last_error_no=errno;
			}
		}
		//MSG("WT : after close()");
		if(!fail) Signal(sa->maintask,(1L<<sa->mainsig));			// {S->M:04} tell main that we're ready
	}
	else Signal(sa->maintask,(1L<<sa->mainsig));
}

//-- eof ----------------------------------------------------------------------

