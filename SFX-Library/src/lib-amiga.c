/****h* sfxsupport.library/lib-amiga.c [4.2] *
*
*  NAME
*    lib-amiga.c
*  COPYRIGHT
*    $VER: lib-amiga.c 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    amiga library init/done routines - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    15.Jul.1998
*  MODIFICATION HISTORY
*    01.Aug.2002	V 4.2	most recent version
*							C: no use of amigaguide.libary, asl.library, diskfont.library anymore
*							C: doesn't *requires* locale.library, openurl.library anymore, just uses them if available
*    27.Aug.2000	V 4.1	most recent version
*    02.Aug.2000	V 4.0	most recent version
*    01.Jul.2000	V 1.3	most recent version
*    20.Sep.1998	V 1.2	most recent version
*    15.Jul.1998	V 1.2	initial version
*  NOTES
*
*******
*/

#define LIB_AMIGA_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Initialisation and Destruction

int				ASM __UserLibInit(REG(a6,struct Library *SFXSupportBase));
void			ASM __UserLibCleanup(REG(a6,struct Library *SFXSupportBase));

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int SAVEDS ASM __UserLibInit(REG(a6,struct Library *SFXSupportBase)) {
	register UWORD j;

	INITDEBUG;
	SysBase=(struct ExecBase *)*((ULONG *)4L);
	INTRO;

#ifndef CONF_CASSABLANCA
	//-- open at first because of DisplayAlert()
	if(!(IntuitionBase  =(struct IntuitionBase *)OpenLibrary("intuition.library",50L))) {
		if(!(IntuitionBase  =(struct IntuitionBase *)OpenLibrary("intuition.library",39L))) goto Error;
		else os_version=39;
	}
	else os_version=50;
	if(!(IntuitionBase	=(struct IntuitionBase *)OpenLibrary("intuition.library",37))) goto Error;
	//-- try to get the rom libs - should work
	if(!(GadToolsBase	=OpenLibrary("gadtools.library",37L)))					{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open gadtools.library >=V37.0" "\x00\x00",44);goto Error; }
	if(!(GfxBase		=(struct GfxBase *)OpenLibrary("graphics.library",37)))	{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open graphics.library >=V37.0" "\x00\x00",44);goto Error; }
#endif
	if(!(DOSBase		=(struct DosLibrary *)OpenLibrary("dos.library",37)))	{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open dos.library >=V37.0" "\x00\x00",44);goto Error; }
	//-- open my own libs
	if(!(ConfigIOBase	=OpenLibrary(ConfigIOName,ConfigIOVersion)))			{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open " ConfigIOName " >=V" NUM2STR(ConfigIOVersion) "\x00\x00",44);goto Error; }
	if(!(ListToolsBase	=OpenLibrary(ListToolsName,ListToolsVersion)))			{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open " ListToolsName " >=V" NUM2STR(ListToolsVersion) "\x00\x00",44);goto Error; }
	if(!(UnitConvBase	=OpenLibrary(UnitConvName,UnitConvVersion)))			{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open " UnitConvName " >=V" NUM2STR(UnitConvVersion) "\x00\x00",44);goto Error; }
	//-- open libs we can do without (maybe send a message to the user)
	LocaleBase			=(struct LocaleBase *)OpenLibrary("locale.library",38L);
	OpenURLBase			=OpenLibrary("openurl.library",3L);
	MSG("Libs ok");
	//-- open locale catalog
	if(LocaleBase) {
		struct TagItem catalogtags[]={
			{ OC_BuiltInLanguage,	(ULONG)"english" },
			{ OC_Version,			((MAIN_VERSION*10)+MAIN_REVISION) },
			{ TAG_DONE }
		};
		if(sfxcat=OpenCatalogA(NULL,(STRPTR)"SoundFX.catalog",(struct TagItem *)catalogtags)) {
			MSG("Catalog ok");
		}
		else {
			ERR("Catalog failed, using fallback");
		}
	}
	//-- init window function and interpolator list
	NewList(&wfktList);for(j=0;j<WIN_ANZ;j++) { wfktNodes[j].ln_Name=WFktLabels[j];AddTail(&wfktList,&wfktNodes[j]); }
	MSG("wfkt-list ok");
	NewList(&interpolList);for(j=0;j<INT_TYPE_ANZ;j++) { interpolNodes[j].ln_Name=InterpolLabels[j];AddTail(&interpolList,&interpolNodes[j]); }
	MSG("interpol-list ok");
	//-- init locale-string-id to help-node-id translation table
	msgid2hlpid[errOpenFile]			= 8;
	msgid2hlpid[errOpenLibrary]			=13;
	msgid2hlpid[errOpenScreen]			=12;
	msgid2hlpid[errOpenWindow]			=12;
	msgid2hlpid[errAllocateMemory]		=12;
	msgid2hlpid[errAllocateFileRequest]	=12;
	msgid2hlpid[errAllocateFontRequest]	=12;
	msgid2hlpid[errAllocateScrmdRequest]=12;
	msgid2hlpid[errAllocateAudioRequest]=12;
	msgid2hlpid[errAllocateSignal]		=12;
	msgid2hlpid[errCreateContext]		=12;
	msgid2hlpid[errCreateGadgets]		=12;
	msgid2hlpid[errCreateMenus]			=12;
	msgid2hlpid[errLayoutMenus]			=12;
	msgid2hlpid[errCreatePort]			=12;
	msgid2hlpid[errVisitorWindows]		=14;
	msgid2hlpid[errMakeScreenPublic]	=15;
	msgid2hlpid[errCloseSample]			=16;
	msgid2hlpid[errEmptyClip]			=17;
	msgid2hlpid[errAccessFile]			=11;
	msgid2hlpid[errAddAppIcon]			=12;
	msgid2hlpid[errAddSampleBuffer]		=12;
	msgid2hlpid[errGetVisualInfo]		=12;
	msgid2hlpid[errAllocateAudio]		=12;
	msgid2hlpid[errNoAHI]				=18;
	msgid2hlpid[errFail]				=19;
	msgid2hlpid[errSampleAttributes]	=12;
	msgid2hlpid[errSampleObject]		=12;
	msgid2hlpid[errReadData]			= 9;
	msgid2hlpid[errWriteData]			=10;
	msgid2hlpid[errWrongFiletype]		=20;
	msgid2hlpid[errUnsupportedSubtype]	=21;
	msgid2hlpid[errWrongDatafile]		= 0;		/* @TODO: add help */
	msgid2hlpid[errNoSampleRate]		=22;
	msgid2hlpid[errSampleToLong]		=23;
	msgid2hlpid[errDamagedFormat]		=24;
	msgid2hlpid[errSourceMono]			=25;
	msgid2hlpid[errSourceStereo]		=25;
	msgid2hlpid[errSourceQuadro]		=25;
	msgid2hlpid[errSourceStereoQuadro]	=25;
	msgid2hlpid[errNotImplemented]		= 6;
	msgid2hlpid[errNoVMemYet]			= 7;
	msgid2hlpid[msgUnregVersionStart]	= 1;
	msgid2hlpid[msgUnregVersionEnd]		= 2;
	msgid2hlpid[msgDemoNoSave]			= 3;
	msgid2hlpid[msgDemoNoARexx]			= 4;
	msgid2hlpid[msgInstIncomplete]		= 5;
	msgid2hlpid[msgPrefsHaveChanged]	= 0;		/* @TODO: add help */
	msgid2hlpid[msgSetPlayerOpts]		= 0;		// no req msgs
	msgid2hlpid[msgSetDefaultGlobals]	= 0;
	msgid2hlpid[msgSetLocalTexts]		= 0;
	msgid2hlpid[msgScanOperators]		= 0;
	msgid2hlpid[msgScanLoaders]			= 0;
	msgid2hlpid[msgScanPlayers]			= 0;
	msgid2hlpid[msgScanSavers]			= 0;
	msgid2hlpid[msgScanRexxmacros]		= 0;
	msgid2hlpid[msgBuildHelp]			= 0;
	msgid2hlpid[msgCreatePorts]			= 0;
	msgid2hlpid[msgInitGUI]				= 0;
	msgid2hlpid[msgProgress]			= 0;
	msgid2hlpid[msgProgressLoad]		= 0;
	msgid2hlpid[msgProgressSave]		= 0;

	msgid2hlpid[reqFileExists]			= 1;
	msgid2hlpid[reqReallyQuit]			= 2;
	msgid2hlpid[reqRunAgain]			= 3;
	msgid2hlpid[reqRemoveAll]			= 4;
	msgid2hlpid[reqCloseSample]			= 5;
	msgid2hlpid[reqRemEntry]			= 6;

	OUTRO;

	return(FALSE);
Error:
	__UserLibCleanup(SFXSupportBase);
	OUTRO;
	return(TRUE);
}

void SAVEDS ASM __UserLibCleanup(REG(a6,struct Library *SFXSupportBase)) {
	//-- close libaries
	if(OpenURLBase)		{ CloseLibrary(OpenURLBase);OpenURLBase=NULL; }
	if(LocaleBase)		{
		if(sfxcat) { CloseCatalog(sfxcat);sfxcat=NULL; }
		CloseLibrary(LocaleBase);LocaleBase=NULL;
	}
	//-- close own libraries
	if(UnitConvBase)	{ CloseLibrary(UnitConvBase);UnitConvBase=NULL; }
	if(ListToolsBase)	{ CloseLibrary(ListToolsBase);ListToolsBase=NULL; }
	if(ConfigIOBase)	{ CloseLibrary(ConfigIOBase);ConfigIOBase=NULL; }
	//-- close rom libraries
	if(GfxBase)			{ CloseLibrary((struct Library *)GfxBase);GfxBase=NULL; }
	if(GadToolsBase)	{ CloseLibrary(GadToolsBase);GadToolsBase=NULL; }
	if(DOSBase)			{ CloseLibrary((struct Library *)DOSBase);DOSBase=NULL; }
	if(IntuitionBase)	{ CloseLibrary((struct Library *)IntuitionBase);IntuitionBase=NULL; }

	DONEDEBUG;
}

//-- eof ----------------------------------------------------------------------
