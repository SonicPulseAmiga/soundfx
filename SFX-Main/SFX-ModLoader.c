/****h* SoundFX/SFX-ModLoader.c [4.3] *
*
*  NAME
*    SFX-ModLoader.c
*  COPYRIGHT
*    $VER: SFX-ModLoader.c 4.3 (01.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    sample loader interface - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    01.Mar.2004	V 4.3	most recent version
*                           A: HandleLoaderCmd
*    14.Sep.2002    V 4.2   most recent version
*    19.Nov.2000    V 4.1   most recent version
*                           A: MRU support
*    22.Jul.2000    V 4.0   most recent version
*    03.Oct.1999    V 3.81  most recent version
*    24.Sep.1998    V 3.71  most recent version
*    14.Aug.1998    V 3.7   most recent version
*    19.Jul.1998    V 3.7   initial version
*  NOTES
*
*  IDEAS
*    * check for dummy chunk info
*      * like samplename="NoName" or "Unnamed" -> use filename
*        function IsStopWord(string);
*
*******
*/

#define SFX_ModLoader_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"
#include "SFX-Filedefs.h"

//-- prototypes ---------------------------------------------------------------

void LoadSampleInterface(void);
BOOL LoadSample(void);
BOOL LoadSampleRexx(char *fn);

//-- local helper

UBYTE LoadSampleDirect(SInfo *si,char *fn);
UBYTE FindExt(ModInfo *mi,char *ext);

//-- internal universal loader module

void LoadUnivSample(SInfo *si,char *fn);

//-- helper for arexx cmds

void HandleLoaderCmd(STRPTR params,STRPTR ret);


//-- definitions --------------------------------------------------------------

void LoadSampleInterface(void) {
	struct Library *SFXModBase;
	void *instance;
	char name[FILENAME_MAX];

	sprintf(name,"progdir:_loaders/%s_L.sfxmod",GetNameByIndex(&LoaderList,RunTime.aloader));
	if(SFXModBase=OpenLibrary(name,PRJ_VERSION)) {
		if(instance=SFXMod_ClientDataInit(&RunTime)) {
			if(SFXMod_Interface(instance)) {
				SFXMod_ClientDataDone(instance);
				CloseLibrary(SFXModBase);
				LoadSample();
			}
			else {
				SFXMod_ClientDataDone(instance);
				CloseLibrary(SFXModBase);
			}
		}
		else MSG(__FUNC__" ClientDateInit() failed");
	}
}

BOOL LoadSample(void) {
	register UBYTE i;
	UBYTE anz;
	char fn[FILENAME_MAX];
	BOOL res=TRUE;

	INTRO;

	*fn='\0';
	if(((ModNode *)GetNodeByIndex(&LoaderList,RunTime.aloader))->info.fsel) {
		//-- with fileselection
		File_Reqtags_TitleText		=(ULONG)GetString(strLoad);
		File_Reqtags_InitialDrawer	=(ULONG)lopath;
		File_Reqtags_InitialFile	=(ULONG)fn;
		File_Reqtags_DoSaveMode		=FALSE;
		File_Reqtags_DoMultiSelect	=TRUE;
		File_Reqtags_DrawersOnly	=FALSE;

#ifdef CHECK_REG
		if(!(keycounter%17)) {
			SetKeyStatus_1;
			ERRC(!IsKeyStatusOkay,"!!! DEMO !!!");
		}
#endif
		if(AslRequest(File_Request,File_Reqtags)) {
			if((anz=File_Request->rf_NumArgs)>1) {
				//-- multiselect
				struct WBArg *args;

				strcpy(lopath,File_Request->fr_Drawer);			// remember path
				args=File_Request->rf_ArgList;
				for(i=0;i<anz;i++) {                                // load more than one sample
					strmfp(fn,File_Request->fr_Drawer,args[i].wa_Name);
					if(access(fn,F_OK|R_OK)==0) {
						res=LoadSampleRexx(fn);
					}
					else { StdCError(fn,errAccessFile,errno,__FILE__,__LINE__);res=FALSE; }
				}
#ifdef CHECK_REG
				SetKeyStatus_5;
				ERRC(!IsKeyStatusOkay,"!!! DEMO !!!");
#endif
			}
			else {
				//-- single select
				strcpy(lopath,File_Request->fr_Drawer);   // remember path
				strmfp(fn,File_Request->fr_Drawer,File_Request->fr_File);
				if(access(fn,F_OK|R_OK)==0) {
					res=LoadSampleRexx(fn);
				}
				else { StdCError(fn,errAccessFile,errno,__FILE__,__LINE__);res=FALSE; }
			}
		}
		else res=FALSE;
	}
	else {
		//-- without fileselection
		res=LoadSampleRexx(fn);
	}
	OUTRO_BOOL(res);
	return(res);
}

BOOL LoadSampleRexx(char *fn) {
	SInfo *si;
	UBYTE aloader;
	char ext[FILENAME_MAX],fn1[FILENAME_MAX],fn2[FILENAME_MAX];
	ModInfo *mi;
	BOOL res;

	INTRO;
	MSG1("fn=\"%s\"",fn);
	if(si=NewSample()) {
		aloader=RunTime.aloader;
		if(!strnicmp(GetNameByIndex(&LoaderList,RunTime.aloader),"UNIVERSAL",9)) LoadUnivSample(si,fn);
		if(res=LoadSampleDirect(si,fn)) {
#ifdef CHECK_REG
			SetKeyStatus_5;
			ERRC(!IsKeyStatusOkay,"!!! DEMO !!!");
#endif
			si->saved=TRUE;
			MRU_Add(fn);
			if(si->title && si->title[0]) AddSample(si,si->title);
			else {
				if(fn && fn[0]) {	// for e.g. CDDA-Direct fn is NULLPtr
					mi=&(((ModNode *)GetNodeByIndex(&LoaderList,RunTime.aloader))->info);   // check if the extension is one of the standard ones
					strsfn(fn,NULL,NULL,fn1,ext);
					if(!strlen(ext) || FindExt(mi,ext)) AddSample(si,fn1);              // leave it off
					else { sprintf(fn2,"%s.%s",fn1,ext);AddSample(si,fn2); }            // tack it on
				}
				else AddSample(si,"Buffer");
			}
			//DrawSample(si,0);       // we refresh via IDCMP_NEWSIZE
			/** @TODO is it okay to do notifies when getting called from ARexx */
			NotifyUser();
		}
		else { RemSample(si);res=FALSE; }
		RunTime.aloader=aloader;
	}
	else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);res=FALSE; }
	OUTRO_BOOL(res);
	return(res);
}

//-- helper

UBYTE LoadSampleDirect(SInfo *si,char *fn) {
	struct Library *SFXModBase;
	void *instance;
	ProcessData pdata={0};
	char name[FILENAME_MAX];
	UBYTE ret=FALSE;

	INTRO;

	sprintf(name,"progdir:_loaders/%s_L.sfxmod",GetNameByIndex(&LoaderList,RunTime.aloader));
	MSG2("Loader \"%s\" will load file \"%s\"",name,SaveString(fn));
	if(SFXModBase=OpenLibrary(name,PRJ_VERSION)) {
		if(instance=SFXMod_ClientDataInit(&RunTime)) {
			pdata.callmd=CM_DIRECT;
			pdata.dst[0]=si;
			pdata.fn=fn;
			ret=SFXMod_Process(instance,&pdata);
			SFXMod_ClientDataDone(instance);
		}
		else MSG(__FUNC__" ClientDateInit() failed");
		CloseLibrary(SFXModBase);
	}

	OUTRO_BOOL(ret);
	return(ret);
}

// returns 0 when no extension could be matched

UBYTE FindExt(ModInfo *mi,char *ext) {
	register UBYTE j;
	UBYTE extchk=0;

	for(j=0;j<10;j++) {         // check all extensions
		if(mi->ext[j][0]) {
			extchk|=(!stricmp(ext,mi->ext[j]));
		}
		else j=MAXBYTEP;
	}
	return(extchk);
}

//-- internal universal loader module

/****u* _loaders/Universal_L *
*  Function.eng
*    The universal-loader tries to identify the sampleformat and loads the sample
*    with the refering loader. It does it in the following way:
*      1.) At first it tries to identify the sample on the basis of its extension.
*      2.) If this is not successful, it tries to find specific strings in the file.
*      3.) If this fails too, it is probably a RAW-Sample and it will be loaded as such.
*    If a sample is not loaded correctly and you know the format, try invoking the
*    respective loader directly. 
*  Funktion.ger
*    Der Universalloader versucht die Sampleformate zu identifizieren und l‰d dann
*    mittels des entsprechenden Lademoduls. Dabei geht er folgendermaﬂen vor:
*      1.) Zuerst versucht er die Samples anhand ihrer Extension (Namenserweiterung) zu erkennen.
*      2.) Falls dies nicht gelingt, versucht er das Format anhand spezifischer Zeichenketten zu identifizieren.
*      3.) Insofern auch dies nichts bringt handelt es sich warscheinlich um ein RAW-Sample und es wird als solches geladen.
*    Wenn ein Sample nicht korrekt geladen wurde und sie das Format kennen, versuchen
*    sie auch mal den entsprechenden Loader direkt aufzurufen.
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

void LoadUnivSample(SInfo *si,char *fn) {
	FILE *infile;
	register UBYTE i,j,k;
	UBYTE done=FALSE,extchk,coochk1,coochk2;
	char ext[FESIZE],temp[256];
	//ModNode *mn;
	ModInfo *mi;

	//INTRO;

	strsfn(fn,NULL,NULL,NULL,ext);
	//MSG2("check : <%s> ext=<%s>",fn,ext);
	if(ext[0]) {
		if((infile=fopen(fn,"rb"))) {
			for(i=0;(i<RunTime.anzlo && !done);i++) {
				//MSG1("\tcheck extensions %ld",(ULONG)i); 
				if((mi=&(((ModNode *)GetNodeByIndex(&LoaderList,i))->info))) {
					if((extchk=FindExt(mi,ext))) {       // one at least fits
						coochk1=1;
						if(mi->magic_str[0][0][0]) {         // if there are magic cookies
							for(j=0;(j<5 && mi->magic_str[j][0][0] && coochk1);j++) { // check for the cookies
								//MSG1("\tcheck cookies1 %ld",(ULONG)i); 
								coochk2=0;
								for(k=0;k<5;k++) {
									if(mi->magic_str[j][k][0]) {
										fseek(infile,mi->magic_off[j],SEEK_SET);
										fread(&temp,strlen(mi->magic_str[j][k]),1,infile);
										coochk2|=(!strncmp(temp,mi->magic_str[j][k],strlen(mi->magic_str[j][k])));
										//MSG2("\t\t\tco2 %1ld <%s>",(ULONG)coochk2,mi->magic_str[j][k]); 
									}
									else k=MAXBYTEP;
								}
								coochk1&=coochk2;
								//MSG1("\t\tco1 %1ld",(ULONG)coochk1); 
							}
						}
						if(coochk1) { RunTime.aloader=i;done=TRUE; }       // all magic cookies found
					}
				}
			}
			fclose(infile);
		}
		else { StdCError(fn,errOpenFile,errno,__FILE__,__LINE__);done=TRUE; }
	}
	if(!done) {                                             /* no extension to check against */
		if((infile=fopen(fn,"rb"))) {
			for(i=0;(i<RunTime.anzlo && !done);i++) {
				//MSG1("check cookies2 %ld",(ULONG)i); 
				if((mi=&(((ModNode *)GetNodeByIndex(&LoaderList,i))->info))) {
					//MSG1("  got info : %08lx",mi);
					coochk1=0;
					if(mi->magic_str[0][0][0]) {         // if there are magic cookies
						coochk1=1;
						for(j=0;(j<5 && mi->magic_str[j][0][0] && coochk1);j++) { // check for the cookies
							coochk2=0;
							for(k=0;k<5;k++) {
								if(mi->magic_str[j][k][0]) {
									fseek(infile,mi->magic_off[j],SEEK_SET);
									fread(&temp,strlen(mi->magic_str[j][k]),1,infile);
									coochk2|=(!strncmp(temp,mi->magic_str[j][k],strlen(mi->magic_str[j][k])));
									//MSG2("    co2 %1ld <%s>",(ULONG)coochk2,mi->magic_str[j][k]); 
								}
								else k=MAXBYTEP;
							}
							coochk1&=coochk2;
							//MSG1("    tco1 %1ld",(ULONG)coochk1); 
						}
					}
					if(coochk1) { RunTime.aloader=i;done=TRUE; }              // all magic cookies found
				}
			}
			fclose(infile);
		}
		else { StdCError(fn,errOpenFile,errno,__FILE__,__LINE__);done=TRUE; }
	}
	if(!done) RunTime.aloader=GetIndexByName(&LoaderList,"RAW");    // because, we have no idea what this is

	//OUTRO;
}


//-- helper for arexx cmds

void HandleLoaderCmd(STRPTR params,STRPTR ret) {
	struct Library *SFXModBase;
	void *instance;
	char name[FILENAME_MAX];
	WORD ix;

	if((ix=GetIndexByName(&LoaderList,RexxPar1))>-1) {
		sprintf(name,"progdir:_loaders/%s_L.sfxmod",RexxPar1);
		if(SFXModBase=OpenLibrary(name,PRJ_VERSION)) {
			if(instance=SFXMod_ClientDataInit(&RunTime)) {
				SFXMod_HandleARexxCmd(instance,params,ret);
				SFXMod_ClientDataDone(instance);
			}
			else MSG(__FUNC__" ClientDateInit() failed");
			CloseLibrary(SFXModBase);
		}
		else MSG(__FUNC__" invalid loader version");
	}
	else MSG(__FUNC__" invalid loader name");
}

//-- eof ----------------------------------------------------------------------
