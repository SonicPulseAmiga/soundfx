/****h* SoundFX/SFX-ModOperator.c [4.2] *
*
*  NAME
*    SFX-ModOperator.c
*  COPYRIGHT
*    $VER: SFX-ModOperator.c 4.2 (02.05.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    sample operator interface - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    02.May.2003	V 4.2	most recent version
*    16.Jan.2001	V 4.1	most recent version
*    03.Oct.1999	V 3.81	most recent version
*    18.Sep.1998	V 3.71	most recent version
*                           * ClientDataDone for Operators
*    18.Aug.1998	V 3.70	release version
*    19.Jul.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_ModOperator_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void ProcessSample(void);
BOOL ProcessSampleRexx(char *params);

//-- asyncronous operator task

void __asm ExecuteOperatorTask(void);

//-- helper for arexx cmds

void HandleOperatorCmd(char *params,char *ret);

//-- definitions --------------------------------------------------------------

void ProcessSample(void) {
	char *modName;
	if(modName=GetNameByIndex(&EffektList,RunTime.aktfx)) {
		static char name[100];
		UBYTE nr=0;
		struct TagItem ProcTags[]=
		{
		/* 0 */	NP_Entry,		(ULONG)ExecuteOperatorTask,
		/* 1 */	NP_StackSize,	20000,
		/* 2 */	NP_Name,		0L,
		/* 3 */	NP_Priority,	0L,
		/* 4 */	TAG_DONE
		};
		struct Process *ThisTask=(struct Process *)FindTask(NULL);

		sprintf(name,SFX_PREFIX"%s.%d",modName,nr);			 // generate unique task name
		Forbid();
		while(FindTask(name)) sprintf(name,SFX_PREFIX"%s.%d",modName,++nr);
		Permit();
	
		ProcTags[2].ti_Data=(ULONG)name;			// better allocate it ?, but when to free it
		ProcTags[3].ti_Data=(ULONG)(((struct Node *)ThisTask)->ln_Pri-2);	// make this two lower than calling task
		CreateNewProc(ProcTags);					// create new process & pass parameters
	}
}

BOOL ProcessSampleRexx(char *params) {
	struct Library *SFXModBase;
	ProcessData pdata={0};
	char name[256];
	BOOL res=TRUE;
	void *instance;

	sprintf(name,"progdir:_operators/%s.sfxmod",GetNameByIndex(&EffektList,RunTime.aktfx));

	if(SFXModBase=OpenLibrary(name,PRJ_VERSION)) {
		if(instance=SFXMod_ClientDataInit(&RunTime)) {
			pdata.callmd=CM_DIRECT;
			GetSrcFromRexx(pdata.src,RexxPar1);
			if(SFXMod_Process(instance,&pdata)) {
				NotifyUser();
			}
			else { res=FALSE;MSG(__FUNC__" Process() failed"); }
			SFXMod_ClientDataDone(instance);
		}
		else MSG(__FUNC__" ClientDateInit() failed");
		CloseLibrary(SFXModBase);
	}
	else { res=FALSE;MSG("OpenLibary failed"); }
	return(res);
}

//-- asyncronous operator task

void __saveds __asm ExecuteOperatorTask(void) {
	struct Library *SFXModBase;
	ProcessData pdata={0};
	char name[FILENAME_MAX],*tname;
	char *np,*tnp;
	struct Process *ThisTask=(struct Process *)FindTask(NULL);
	struct Window *oldWinPtr;
	void *instance;

	INTRO;

	oldWinPtr=ThisTask->pr_WindowPtr;
	ThisTask->pr_WindowPtr=(APTR)RunTime.winBackDrop;

	tname=((struct Node *)ThisTask)->ln_Name;

	strcpy(name,"progdir:_operators/");
	np=&name[19];							// skip "progdir:_operators/"
	tnp=&tname[4];							// skip "SFX_" prefix
	while(*tnp!='.') *(np++)=*(tnp++);		// copy plugin name
	strcpy(np,".sfxmod");

	if(SFXModBase=OpenLibrary(name,PRJ_VERSION)) {
		if(instance=SFXMod_ClientDataInit(&RunTime)) {
			MSG("ClientDataInit okay");
			if(SFXMod_Interface(instance)) {
				MSG("Interface okay");
				pdata.callmd=CM_GUI;
				if(SFXMod_Process(instance,&pdata)) {
					NotifyUser();
				}
				else MSG(__FUNC__" Process() failed");
			}
			else MSG(__FUNC__" Interface() failed");
			SFXMod_ClientDataDone(instance);
		}
		else MSG(__FUNC__" ClientDateInit() failed");
		CloseLibrary(SFXModBase);
	}
	else MSG(__FUNC__" invalid fx version");

	ThisTask->pr_WindowPtr=oldWinPtr;
	OUTRO;
}

//-- helper for arexx cmds

void HandleOperatorCmd(char *params,char *ret) {
	struct Library *SFXModBase;
	char name[FILENAME_MAX];
	WORD ix;
	void *instance;
	
	if((ix=GetIndexByName(&EffektList,RexxPar1))>-1) {
		sprintf(name,"progdir:_operators/%s.sfxmod",RexxPar1);
		if(SFXModBase=OpenLibrary(name,PRJ_VERSION)) {
			if(instance=SFXMod_ClientDataInit(&RunTime)) {
				SFXMod_HandleARexxCmd(instance,params,ret);
				SFXMod_ClientDataDone(instance);
			}
			else MSG(__FUNC__" ClientDateInit() failed");
			CloseLibrary(SFXModBase);
		}
		else MSG(__FUNC__" invalid fx version");
	}
	else MSG(__FUNC__" invalid fx name");
}

//-- eof ----------------------------------------------------------------------

