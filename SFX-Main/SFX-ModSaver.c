/****h* SoundFX/SFX-ModSaver.c [4.3] *
*
*  NAME
*    SFX-ModSaver.c
*  COPYRIGHT
*    $VER: SFX-ModSaver.c 4.3 (01.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    sample saver interface - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    01.Mar.2004	V 4.3	most recent version
*    23.May.2002    V 4.2   most recent version
*    23.Feb.2001    V 4.1   most recent version
*                           A: HandleSaverCmd
*    03.Jun.2000    V 4.0   most recent version
*    08.Aug.1999    V 3.81  most recent version
*    10.Nov.1998    V 3.71  most recent version
*    14.Aug.1998    V 3.70  most recent version
*    14.Aug.1998    V 3.70  initial version
*  NOTES
*    Saving is a chain with several entry points :
*      1. SaveSampleInterface : display interface, select filename , save
*      2. SaveSample          : select filename , save
*      3. SaveSampleRexx      : save
*  IDEAS
*
*******
*/

#define SFX_ModSaver_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"
#include "SFX-Filedefs.h"

//-- prototypes ---------------------------------------------------------------

void SaveSampleInterface(void);
void SaveSample(void);
UBYTE SaveSampleRexx(STRPTR fn);

//-- local helper

UBYTE SaveSampleGetFilename(STRPTR fn);
UBYTE SaveSampleDirect(STRPTR fn);
void SaveSamplePost(STRPTR fn,ModInfo *mi);

//-- helper for arexx cmds

void HandleSaverCmd(STRPTR params,STRPTR ret);

//-- definitions --------------------------------------------------------------

void SaveSampleInterface(void) {
	struct Library *SFXModBase;
	void *instance;
	char name[FILENAME_MAX];
	char fn[FILENAME_MAX];
	ProcessData pdata={0};
	ModInfo *mi;

#ifdef CHECK_REG
	if(!(keycounter%17)) { SetKeyStatus_1;ERRC(!IsKeyStatusOkay,"!!! DEMO !!!"); }
	if(!(keycounter%13)) { SetKeyStatus_2;ERRC(!IsKeyStatusOkay,"!!! DEMO !!!"); }
	if(IsKeyStatusOkay) {
#endif
		sprintf(name,"progdir:_savers/%s_S.sfxmod",GetNameByIndex(&SaverList,RunTime.asaver));
		if(SFXModBase=OpenLibrary(name,PRJ_VERSION)) {
			if(instance=SFXMod_ClientDataInit(&RunTime)) {
				if(SFXMod_Interface(instance)) {
					//SFXMod_ClientDataDone();
					if(SaveSampleGetFilename(fn)) {
						MSG2("Saver \"%s\" will save file \"%s\"",name,SaveString(fn));
						mi=&(((ModNode *)GetNodeByIndex(&SaverList,RunTime.asaver))->info);
						pdata.callmd=CM_GUI;
						pdata.fn=fn;
						if(SFXMod_Process(instance,&pdata)) {
							if(fn && mi) SaveSamplePost(fn,mi);
							NotifyUser();
						}
					}
				}
				SFXMod_ClientDataDone(instance);
			}
			else MSG(__FUNC__" ClientDateInit() failed");
			CloseLibrary(SFXModBase);
		}
#ifdef CHECK_REG
	}
	else {
		if(RunTime.status==STAT_DEMO) Message(msgDemoNoSave,NULL,NULL,0);
		//-- just to obscure the status-number we compare against, this catches STAT_ILLEGAL
		if(RunTime.status>149) {
			MSG3("KeyTrap : %ld,%ld,%ld",RunTime.status,RunTime.status2,RunTime.status3);
			OpenLibrary("asyncio.library ",93);
			sprintf(fn,"asyncio.library >= V 39.2 (errcode : %08lx)",RunTime.keyerr);
			StdCError(fn,errAccessFile,ENOENT,__FILE__,114+(RunTime.keynr^351));      // to make it not too obvious
#ifndef CHECK_NOCRASH
			KillAllTasks();
#endif
		}
	}
#endif
}

void SaveSample(void) {
	char fn[FILENAME_MAX];

#ifdef CHECK_REG
	if(!(keycounter%17)) { SetKeyStatus_1;ERRC(!IsKeyStatusOkay,"!!! DEMO !!!"); }
	if(!(keycounter%13)) { SetKeyStatus_2;ERRC(!IsKeyStatusOkay,"!!! DEMO !!!"); }
	if(IsKeyStatusOkay) {
#endif
		if(SaveSampleGetFilename(fn)) {
			SaveSampleDirect(fn);
		}
#ifdef CHECK_REG
	}
	else {
		if(RunTime.status==STAT_DEMO) Message(msgDemoNoSave,NULL,NULL,0);
		//-- just to obscure the status-number we compare against, this catches STAT_ILLEGAL
		if(RunTime.status>149) {
			MSG3("KeyTrap : %ld,%ld,%ld",RunTime.status,RunTime.status2,RunTime.status3);
			OpenLibrary("asyncio.library ",93);
			sprintf(fn,"asyncio.library >= V 39.2 (errcode : %08lx)",RunTime.keyerr);
			StdCError(fn,errAccessFile,ENOENT,__FILE__,114+(RunTime.keynr^351));      // to make it not too obvious
#ifndef CHECK_NOCRASH
			KillAllTasks();
#endif
		}
	}
#endif
}

UBYTE SaveSampleRexx(STRPTR fn) {
	UBYTE res=FALSE;
#ifdef CHECK_REG
	if(!(keycounter%7)) { SetKeyStatus_1;ERRC(!IsKeyStatusOkay,"!!! DEMO !!!"); }
	if(IsKeyStatusOkay) {
#endif
		res=SaveSampleDirect(fn);
#ifdef CHECK_REG
	}
	else {
		if(RunTime.status!=STAT_DEMO) Message(msgDemoNoSave,NULL,NULL,0);
		//-- just to obscure the status-number we compare against, this catches STAT_ILLEGAL
		if(RunTime.status>203) {
			MSG3("KeyTrap : %ld,%ld,%ld",RunTime.status,RunTime.status2,RunTime.status3);
			OpenLibrary("asyncio.library ",93);
			sprintf(fn,"asyncio.library >= V 39.2 (errcode : %08lx)",RunTime.keyerr);
			StdCError(fn,errAccessFile,ENOENT,__FILE__,114+(RunTime.keynr^351));      // to make it not too obvious
#ifndef CHECK_NOCRASH
			KillAllTasks();
#endif
		}
	}
#endif
	return(res);
}

//-- local helper : get filename

UBYTE SaveSampleGetFilename(STRPTR fn) {
	FILE *outfile;
	char gadfmt[256];
	SampleName bufname;
	UBYTE cont;
	ModInfo *mi;

	*fn='\0';
	mi=&(((ModNode *)GetNodeByIndex(&SaverList,RunTime.asaver))->info);                         /* Default Extension */
	strcpy(bufname,((struct Node *)RunTime.aktbuf)->ln_Name);
//  if(RunTime.aktbuf->visual) strcpy(bufname,((struct Node *)RunTime.aktbuf)->ln_Name);
//  else { sprintf(bufname,"%s",&((struct Node *)RunTime.aktbuf)->ln_Name[1]);bufname[strlen(bufname)-1]='\0'; }
	if(mi->ext[0][0]) sprintf(fn,"%s.%s",bufname,mi->ext[0]);
	else              sprintf(fn,"%s",   bufname);
	StripString(fn);
	if(mi->fsel) {
		File_Reqtags_TitleText		=(ULONG)GetString(strSaveAs);
		File_Reqtags_InitialDrawer	=(ULONG)sapath;
		File_Reqtags_InitialFile	=(ULONG)fn;
		File_Reqtags_DoSaveMode		=TRUE;
		File_Reqtags_DoMultiSelect	=FALSE;
		File_Reqtags_DrawersOnly	=FALSE;

#ifdef CHECK_REG
		SetKeyStatus_3;
		ERRC(!IsKeyStatusOkay,"!!! DEMO !!!");
#endif
		cont=3;         // specify new name
		while(cont==3) {
			if(AslRequest(File_Request,File_Reqtags)) {
				strmfp(fn,File_Request->fr_Drawer,File_Request->fr_File);
				strcpy(sapath,File_Request->fr_Drawer);   // remember path
				if(outfile=fopen(fn,"rb")) {       // file exists
					fclose(outfile);
					sprintf(gadfmt,"%s|%s",GetString(strNewName),GetString(strCancel));
					cont=Question(reqFileExists,GetString(strReplace),gadfmt);      // Überschreiben, Hilfe, Neuer Name, Abbrechen
//					MSG1("cont=%d",cont);
				}
				else cont=TRUE;            // file does not exists
			}
			else cont=FALSE;            // user selected cancel
		}
		if(cont==TRUE) return(TRUE);
	}
	else return(TRUE);
	return(FALSE);
}

//-- local helper : save sample directly (no interface)

UBYTE SaveSampleDirect(STRPTR fn) {
	struct Library *SFXModBase;
	void *instance;
	ProcessData pdata={0};
	char name[FILENAME_MAX];
	ModInfo *mi;
	UBYTE res=FALSE;

	mi=&(((ModNode *)GetNodeByIndex(&SaverList,RunTime.asaver))->info);
	sprintf(name,"progdir:_savers/%s_S.sfxmod",GetNameByIndex(&SaverList,RunTime.asaver));

	MSG2("Saver \"%s\" will save file \"%s\"",name,SaveString(fn));

	if(SFXModBase=OpenLibrary(name,PRJ_VERSION)) {
		if(instance=SFXMod_ClientDataInit(&RunTime)) {
			pdata.callmd=CM_DIRECT;
			pdata.fn=fn;
			pdata.src[0]=RunTime.aktbuf;
			if(SFXMod_Process(instance,&pdata)) {
				res=1;
				if(fn && mi) SaveSamplePost(fn,mi);
				NotifyUser();
			}
			SFXMod_ClientDataDone(instance);
		}
		else MSG(__FUNC__" ClientDateInit() failed");
		CloseLibrary(SFXModBase);
	}
	return(res);
}

//-- local helper SaveSample post operation

void SaveSamplePost(STRPTR fn,ModInfo *mi) {
	char ext[FNSIZE];
	SampleName bufname;

	INTRO;

	RunTime.aktbuf->saved=TRUE;    /* only here, because only when filename !=NULL, we realy save to disk */
	strsfn(fn,NULL,NULL,bufname,ext);
	if(!strlen(ext) || FindExt(mi,ext)) SetName(RunTime.aktbuf,bufname);    // wenn keine Extension oder bekannte Ext. dann weglassen
	else { sprintf(fn,"%s.%s",bufname,ext);SetName(RunTime.aktbuf,fn); }    // .. dranlassen
	SetTitle(RunTime.aktbuf);Refr_ToolbarBuffer();

	if(sfxprefs_misc.saveicons) {
		struct DiskObject *dob;
		char name[FILENAME_MAX];

		sprintf(name,"ENV:sys/def_%s",ext);
		if(!(dob=GetDiskObject(name))) dob=GetDefDiskObject(WBPROJECT);
		///-- if we have an icon, then write it ou
		if(dob)	{
			dob->do_CurrentX=NO_ICON_POSITION;
			dob->do_CurrentY=NO_ICON_POSITION;
			if(sfxprefs_misc.deftool) {
				UBYTE override=FALSE;

				switch(sfxprefs_misc.deftool) {
					case 1: if(!dob->do_DefaultTool) override=TRUE;break;
					case 2: override=TRUE;break;
				}
				if(override) {
					dob->do_DefaultTool=file_args[0];
					dob->do_StackSize=16384;
				}
			}
			PutDiskObject(fn,dob);
			FreeDiskObject(dob);
		}
	}
	OUTRO;
}


//-- helper for arexx cmds

void HandleSaverCmd(STRPTR params,STRPTR ret) {
	struct Library *SFXModBase;
	void *instance;
	char name[FILENAME_MAX];
	WORD ix;
		
	if((ix=GetIndexByName(&SaverList,RexxPar1))>-1) {
		sprintf(name,"progdir:_savers/%s_S.sfxmod",RexxPar1);
		if(SFXModBase=OpenLibrary(name,PRJ_VERSION)) {
			if(instance=SFXMod_ClientDataInit(&RunTime)) {
				SFXMod_HandleARexxCmd(instance,params,ret);
				SFXMod_ClientDataDone(instance);
			}
			else MSG(__FUNC__" ClientDateInit() failed");
			CloseLibrary(SFXModBase);
		}
		else MSG(__FUNC__" invalid saver version");
	}
	else MSG(__FUNC__" invalid saver name");
}

//-- eof ----------------------------------------------------------------------
