/****h* SoundFX/SFX.c [4.3] *
*
*  NAME
*    SFX.c
*  COPYRIGHT
*    $VER: SFX.c 4.3 (14.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    main source code for sfx - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    06.Aug.1998
*  MODIFICATION HISTORY
*    14.Apr.2004	V 4.3	most recent version
*    21.Feb.2003	V 4.2	most recent version
*    16.Apr.2002    V 4.2   most recent version
*                           * new menu entries
*                           * quiet mode (arexx,batch)
*    01.Aug.2000    V 4.00  most recent version
*                           * samplewin i/o externalised
*    30.Jan.2000    V 3.82  most recent version
*    03.Oct.1999    V 3.81  most recent version
*    02.Mar.1999    V 3.80  most recent version
*    06.Nov.1998    V 3.71  most recent version
*                           * stronger key checking
*                           * better assign add for fonts
*    24.Aug.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#define SFX_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void OpenLibs(void);
void OpenAll(void);
void CloseAll(UBYTE level,UBYTE return_code);

void SetDefs(void);
void SetLocaleTexts(void);

//-- globals ------------------------------------------------------------------

//-- Versionsstring

UBYTE *VersTag="\0$VER: "PRJ_VERS;

//-- Libarybasese

struct Library              *AslBase=NULL;
struct Library              *ConfigIOBase=NULL;
struct Library              *DiskfontBase=NULL;
struct Library              *GadToolsBase=NULL;
struct GfxBase              *GfxBase=NULL;
struct Library              *IconBase=NULL;
struct Library				*IdentifyBase=NULL;
struct IntuitionBase        *IntuitionBase=NULL;
struct Library              *ListToolsBase=NULL;
struct PopupMenuBase        *PopupMenuBase=NULL;
struct RxsLib				*RexxSysBase=NULL;
struct Library              *SFXSupportBase=NULL;
struct Library              *UnitConvBase=NULL;
struct Library              *UtilityBase=NULL;
struct Library              *WorkbenchBase=NULL;
#ifdef CHECK_DELFINA
		struct Library *DelfinaBase=NULL;
#endif

//-- Appicon

struct DiskObject           *SFX_DiskObject=NULL;
struct AppIcon              *SFX_AppIcon=NULL;

//-- temorary assign fonts: progdir:fonts add

BPTR fontslock=NULL;

//-- the shared memory structure

RTime RunTime={0};

//-- OPT-Structuren

OPTP_Amiga_8bit         optp_Amiga_8bit;
OPTP_Amiga_14bit        optp_Amiga_14bit;
OPTP_Amiga_14cbit       optp_Amiga_14cbit;
OPTP_Ahi                optp_Ahi;

//-- Modullisten

struct List                 EffektList;
struct List                 LoaderList;
struct List                 SaverList;
struct List                 PlayerList;
struct List                 RexxList;
struct List                 BufferList;

//-- Multiwindow-Messageports

struct MsgPort              *swin_up=NULL;
struct MsgPort              *gwin_up=NULL;

//-- Arexx-Port

struct MsgPort              *arexxport=NULL;
char                        *arexxname=SFX_AREXXNAME;

//-- AppIcon-Port

struct MsgPort              *appiconport=NULL;

//-- shifted MsgPort Signals

ULONG apsig,bwsig,gwsig,swsig,rxsig;

//-- sfxscreentitle (containing memory status)

char sfxtitle[256];

//-- keyfile related

KeyFile Key;
struct Library *SFXKeyBase=NULL,*SFXKeyBase1=(APTR)0x03FE;
UWORD keycounter=0;

//-- used for error messages

char sfxmsg[256];

//-- the configuration

APTR cfg=NULL;

//-- path for loader/saver ASL requests

char lopath[FILENAME_MAX],sapath[FILENAME_MAX];

//-- for Edit/Utillities

SInfo *clip=NULL,temp;

//-- startup args (initial filesnames)

ULONG num_args=0;
STRPTR file_args[MAX_ARGS]={ NULL };

//-- private

ULONG srclist[20];                                  /* sourcebufferliste */

void ASM (*CrashFuncPtr)(REG(a4,ULONG val1),REG(a6,ULONG val2));

//-- BuiltIn Modules

ArrangeList pllist[playeranz]= {
	"AHI",			0,
	"Amiga-14bit",	1,
	"Amiga-14cbit",	2,
	"Amiga-8bit",	3,
};

//-- externals ----------------------------------------------------------------

// set some params for init code

#pragma msg 72 ignore
extern long __near __stack=60000L;
extern long __oslibversion=38;
#pragma msg 72 warning

//-- definitions --------------------------------------------------------------

void OpenLibs(void) {
	UBYTE i;

	INTRO;
//-- open essential libs
	if(!(IntuitionBase  =(struct IntuitionBase *)OpenLibrary("intuition.library",39L)))         { printf("Can't open intuition.library >=V39 !\n%s"                                     	,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	/** @todo use DisplayAlert() below */
	if(!(AslBase        =OpenLibrary(AslName,39L)))                                             { printf("Can't open asl.library >=V39 !\n%s"                                               ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(ConfigIOBase   =OpenLibrary(ConfigIOName,ConfigIOVersion)))                            { printf("Can't open "ConfigIOName" >=V"NUM2STR(ConfigIOVersion)" !\n%s"                    ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(DiskfontBase   =OpenLibrary("diskfont.library",39L)))                                  { printf("Can't open diskfont.library >=V39 !\n%s"                                          ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
//  if(!(DOSBase        =(struct DosLibrary *)OpenLibrary("dos.library",39L)))                  { printf("Can't open dos.library >=V39 !\n%s"                                               ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }   // weil wir mit c.o linken
	if(!(GadToolsBase   =OpenLibrary("gadtools.library",39L)))                                  { printf("Can't open gadtools.library >=V39 !\n%s"                                          ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(GfxBase        =(struct GfxBase *)OpenLibrary("graphics.library",39L)))                { printf("Can't open graphics.library >=V39 !\n%s"                                          ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(IconBase       =OpenLibrary("icon.library",39L)))                                      { printf("Can't open icon.library >=V39 !\n%s"                                              ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(ListToolsBase  =OpenLibrary(ListToolsName,ListToolsVersion)))                          { printf("Can't open "ListToolsName" >=V"NUM2STR(ListToolsVersion)" !\n%s"                  ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(RexxSysBase    =(struct RxsLib *)OpenLibrary("rexxsyslib.library",34L)))				{ printf("Can't open rexxsyslib.library >=V34 !\n%s"                                        ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(UnitConvBase   =OpenLibrary(UnitConvName,UnitConvVersion)))                            { printf("Can't open "UnitConvName" >=V"NUM2STR(UnitConvVersion)" !\n%s"                    ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(UtilityBase    =OpenLibrary("utility.library",39L)))                                   { printf("Can't open utility.library >=V39 !\n%s"                                           ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(WorkbenchBase  =OpenLibrary("workbench.library",39L)))                                 { printf("Can't open workbench.library >=V39 !\n%s"                                         ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(!(SFXSupportBase =OpenLibrary("progdir:libs/"SFXSupportName,SFXSupportVersion)))         { printf("Can't open progdir:libs/"SFXSupportName" >=V"NUM2STR(SFXSupportVersion)" !\n%s"   ,ErrorInfo());CloseAll(CLLEV_LIBS,RETURN_FAIL); }
	if(GetSFXVersion()!=MAKE_VERSION_STAMP(PRJ_VERSION,PRJ_REVISION))                           { printf("progdir:libs/"SFXSupportName" has not the requested version V"NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)"!\n");CloseAll(CLLEV_LIBS,RETURN_FAIL); }
//-- open libs we can do without (maybe send a message to the user)
	IdentifyBase		=OpenLibrary("identify.library",IDENTIFYVERSION);
	PopupMenuBase       =(struct PopupMenuBase *)OpenLibrary(POPUPMENU_NAME,POPUPMENU_VERSION);

//-- testen ob Programm schon läuft
	// we can run it just once, because of the shared library
	if(!ServerDataInit(&RunTime)) {
		//printf("SoundFX is already running !\n");
		struct MsgPort *msg_rport;
		struct RexxMsg *msg_rxmsg;
		struct MsgPort *msg_port;
		char cmd[FILENAME_MAX+20];

		MSG("we have initial files to submit to running sfx");
		//-- send arexx messages : SFX_LoadSample sfilename
		if(msg_rport=CreateMsgPort()) {		/* Create the REXX reply port */
			if(msg_rxmsg=CreateRexxMsg(msg_rport,"sfx_rexxtmp","SFX_REXXTMP")) {
				Forbid();		// We do this under Forbid() to prevent the destination from exiting before our message gets there
				if(msg_port=FindPort("SFX_REXX"))	{	// Get the destination port
					msg_rxmsg->rm_Action=RXCOMM;						// Fill in message
					for(i=1;i<num_args;i++) {
						sprintf(cmd,"SFX_LoadSample '%s'",file_args[i]);
		 				msg_rxmsg->rm_Args[0]=cmd;
						msg_rxmsg->rm_Args[1]=(void *)strlen(cmd);
						PutMsg(msg_port,(struct Message *)msg_rxmsg);	// Send it out
						Permit();
						WaitPort(msg_rport);							// Wait for destination's reply
						while(GetMsg(msg_rport)!=NULL);					// Get replys
					}
					if(num_args==1) {
						sprintf(cmd,"SFX_Activate");
		 				msg_rxmsg->rm_Args[0]=cmd;
						msg_rxmsg->rm_Args[1]=(void *)strlen(cmd);
						PutMsg(msg_port,(struct Message *)msg_rxmsg);	// Send it out
						Permit();
						WaitPort(msg_rport);							// Wait for destination's reply
						while(GetMsg(msg_rport)!=NULL);					// Get replys
					}
				}
				else Permit();
				DeleteRexxMsg(msg_rxmsg);
			}
			DeleteMsgPort(msg_rport);
		}
		CloseAll(CLLEV_LIBS,RETURN_ERROR);
	}
//  MSG("  sfxsupport.library initialized");
//
#pragma msg 147 ignore
	CrashFuncPtr=((void *)(rand()%524288));
#pragma msg 147 warn
//
	OUTRO;
}

void OpenAll(void) {
	INTRO;
//-- default options setzen

//  UpdateGFX(GetString(msgSetPlayerOpts    ), 7);for(i=0;i<5;i++) SetDefPOptions(i);       // mit in ReadPrefs
	UpdateGFX(GetString(msgSetDefaultGlobals), 8);
	SetDefs();
	GenIOTables();	// SFX-library/lib-packt.c

	UpdateGFX(GetString(msgSetLocalTexts    ),16);
	SetLocaleTexts();

//-- main config anlegen & einlesen

	if(!(cfg=CfgIO_NewConfig("SoundFX"," %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) CloseAll(CLLEV_GUI,RETURN_FAIL);

	if(access("ENV:sfx.cfg",F_OK|R_OK)) ReadPrefs("ENVARC:sfx.cfg");        /* Prefs müssen nicht unbedingt ins ENV: umkopiert werden */
	else ReadPrefs("ENV:sfx.cfg");

#ifdef CHECK_REG
	if(IsKeyStatusIllegal_1 || IsKeyStatusIllegal_2) RunTime.status=STAT_ILLEGAL;
	ERRC(!IsKeyStatusOkay,"!!! DEMO !!!");
#endif

//-- mru config anlegen & einlesen

	if(!MRU_Init()) CloseAll(CLLEV_GUI,RETURN_FAIL);

#ifdef CHECK_REG
	if(RunTime.status==STAT_ILLEGAL) {
		char fn[256];

		MSG3("KeyTrap : %ld,%ld,%ld",RunTime.status,RunTime.status2,RunTime.status3);
		OpenLibrary("audioplugin.library ",19);
		sprintf(fn,"audioplugin.library >= V 19.3 (errcode : %08lx)",RunTime.keyerr);
		StdCError(fn,errAccessFile,ENOENT,__FILE__,87+(RunTime.keynr^465));      // to make it not too obvious
#ifndef CHECK_NOCRASH
		KillAllTasks();
		CrashFuncPtr(rand(),rand());
#endif
//      CloseAll(CLLEV_GUI,RETURN_FAIL);
	}
#endif

//-- ModInfos lesen

	UpdateGFX(GetString(msgScanLoaders      ),20);RunTime.anzlo  =GetModuleInfos("Loaders"  ,"PROGDIR:_loaders"  ,&LoaderList,1);
	UpdateGFX(GetString(msgScanOperators    ),28);RunTime.anzfx  =GetModuleInfos("Operators","PROGDIR:_operators",&EffektList,0);
	UpdateGFX(GetString(msgScanPlayers      ),43);RunTime.anzpl  =GetModuleInfos("Players"  ,"PROGDIR:_players"  ,&PlayerList,0);SortAList(RunTime.anzpl,pllist);
	UpdateGFX(GetString(msgScanRexxmacros   ),46);RunTime.anzrxop=GetModuleInfos("Rexx"     ,"PROGDIR:_rexx"     ,&RexxList  ,0);
	UpdateGFX(GetString(msgScanSavers       ),52);RunTime.anzsa  =GetModuleInfos("Savers"   ,"PROGDIR:_savers"   ,&SaverList ,1);
	if(!RunTime.anzfx || !RunTime.anzlo || !RunTime.anzsa || !RunTime.anzpl || !RunTime.anzrxop) Message(msgInstIncomplete,NULL,NULL,0);

//-- MsgPorts anlegen

	UpdateGFX(GetString(msgCreatePorts      ),65);
	if(!(arexxport=CreateMsgPort())) { Message(errCreatePort,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }   /* ArexxPort anlegen */
	arexxport->mp_Node.ln_Name=arexxname;arexxport->mp_Node.ln_Pri=0;AddPort(arexxport);
	if(!(gwin_up=CreateMsgPort()))   { Message(errCreatePort,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }   /* Ports für MultiWindows */
	if(!(swin_up=CreateMsgPort()))   { Message(errCreatePort,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	RunTime.swin_up=swin_up;

//-- AppIcon erzeugen

	if(FindTask("« IPrefs »")) {                                            /* seems to be no workbench open */
		char sfxname[FILENAME_MAX];

		if(!(appiconport=CreateMsgPort())) { Message(errCreatePort,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); } /* AppiconInitialisation */
		if(!(SFX_DiskObject=GetDiskObject(file_args[0]))) {
			sprintf(sfxname,"%s.info",file_args[0]);ADosError(sfxname,errAccessFile,IoErr(),__FILE__,__LINE__);
			CloseAll(CLLEV_GUI,RETURN_FAIL);
		}
		//if(!(SFX_AppIcon=AddAppIcon(0L,0L,"SoundFX",appiconport,NULL,SFX_DiskObject,WBAPPICONA_SupportsInformation,TRUE,TAG_END))) {
		if(!(SFX_AppIcon=AddAppIcon(0L,0L,"SoundFX",appiconport,NULL,SFX_DiskObject,TAG_END))) {
			sprintf(sfxname,"%s.info",file_args[0]);ADosError(sfxname,errAddAppIcon,IoErr(),__FILE__,__LINE__);
			CloseAll(CLLEV_GUI,RETURN_FAIL);
		}
	}

//-- initialize Player
//	UpdateGFX(GetString(msgInitPlayers      ),70);		/* !!! LOCALIZE !!! */
	RunTime.boolPaulaOkay   =InitPaulaPlayers();
	RunTime.boolCaCaOkay    =InitCaCaPlayer();
	RunTime.boolAHIOkay     =InitAhiAPlayer();


//-- Mousepointer lesen
	UpdateGFX(GetString(msgInitGUI          ),75);
	RunTime.MPtrCrossHair=ReadPointerImage("progdir:data/pointer_crosshair.prefs");
	RunTime.MPtrMove     =ReadPointerImage("progdir:data/pointer_move.prefs");
	RunTime.MPtrRange    =ReadPointerImage("progdir:data/pointer_range.prefs");
	RunTime.MPtrSize64   =ReadPointerImage("progdir:data/pointer_size64.prefs");
	RunTime.MPtrSize73   =ReadPointerImage("progdir:data/pointer_size73.prefs");
	RunTime.MPtrSize82   =ReadPointerImage("progdir:data/pointer_size82.prefs");
	RunTime.MPtrSize91   =ReadPointerImage("progdir:data/pointer_size91.prefs");

	UpdateGFX(GetString(msgInitGUI          ),80);

	if((fontslock=Lock("progdir:fonts",ACCESS_READ))) {
		if(!AssignAdd("fonts",fontslock)) { UnLock(fontslock);fontslock=NULL; }
	}

	GUI_Init();                                                         /* GUI */

	InitCopyBuffer();

	InitRequester(&NullReq);                                            /* BlockReq initialisieren */

	UpdateGFX(GetString(strReady),100);
	OUTRO;
}

void CloseAll(UBYTE level,UBYTE return_code) {
	UBYTE i;

	INTRO;

	if(level>=CLLEV_GUI) {
		MSG("  CLLEV_GUI");
		CloseGFX();                                         /* only if failed during bootup */

		FreeCopyBuffer();

		GUI_Done();

		if(fontslock) 		RemAssignList("fonts",fontslock);	/* do not unlock the lock, it gets unlocked by RemAssignList() */

		if(RunTime.MPtrCrossHair)   FreePointerImage(RunTime.MPtrCrossHair);
		if(RunTime.MPtrMove)        FreePointerImage(RunTime.MPtrMove);
		if(RunTime.MPtrRange)       FreePointerImage(RunTime.MPtrRange);
		if(RunTime.MPtrSize64)      FreePointerImage(RunTime.MPtrSize64);
		if(RunTime.MPtrSize73)      FreePointerImage(RunTime.MPtrSize73);
		if(RunTime.MPtrSize82)      FreePointerImage(RunTime.MPtrSize82);
		if(RunTime.MPtrSize91)      FreePointerImage(RunTime.MPtrSize91);

		FreePaulaPlayers();																/* free Players */
		FreeCaCaPlayer();
		FreeAhiAPlayer();
		//MSG("    after freeing playerstuff");

		if(SFX_AppIcon)     RemoveAppIcon(SFX_AppIcon);
		if(SFX_DiskObject)  FreeDiskObject(SFX_DiskObject);

		if(appiconport) { FlushMsgPort(appiconport);DeleteMsgPort(appiconport); }       /* AppIconPort freigeben */
		//MSG("after freeing appiconport");
		if(swin_up)     { FlushMsgPort(swin_up);    DeleteMsgPort(swin_up); }           /* swin_up freigeben */
		//MSG("after freeing swin_up");
		if(gwin_up)     { FlushMsgPort(gwin_up);    DeleteMsgPort(gwin_up); }           /* gwin_up freigeben */
		//MSG("after freeing gwin_up");
		if(arexxport)   { RemPort(arexxport);FlushMsgPort(arexxport);DeleteMsgPort(arexxport); }    /* Arexxport freigeben */
		//MSG("after freeing arexxport");

		if(RunTime.barray) { FreeVec(RunTime.barray);RunTime.barray=NULL; }

		if(!IsListEmpty(&EffektList))   FreeModList(&EffektList);
		if(!IsListEmpty(&LoaderList))   FreeModList(&LoaderList);
		if(!IsListEmpty(&PlayerList))   FreeModList(&PlayerList);
		if(!IsListEmpty(&RexxList))     FreeModList(&RexxList);
		if(!IsListEmpty(&SaverList))    FreeModList(&SaverList);

		if(!IsListEmpty(&RunTime.gblist)) FreeList(&RunTime.gblist,sizeof(NotifyTask));

		MRU_Done();
		if(cfg)                         CfgIO_RemConfig(cfg);
	}
	if(level>=CLLEV_SERVER) {
		MSG("  CLLEV_SERVER");
		ServerDataDone();
	}
	if(level>=CLLEV_LIBS) {
		MSG("  CLLEV_LIBS");
		if(WorkbenchBase)   CloseLibrary(WorkbenchBase);
		if(UtilityBase)     CloseLibrary(UtilityBase);
		if(UnitConvBase)    CloseLibrary(UnitConvBase);
		if(SFXKeyBase)      CloseLibrary(SFXKeyBase);
		if(SFXSupportBase)  CloseLibrary(SFXSupportBase);
		if(RexxSysBase)     CloseLibrary(RexxSysBase);
		if(PopupMenuBase)   CloseLibrary(PopupMenuBase);
		if(ListToolsBase)   CloseLibrary(ListToolsBase);
		if(IntuitionBase)   CloseLibrary(IntuitionBase);        // crashes here when using debugtools and some library could not be opened
		if(IdentifyBase)	CloseLibrary(IdentifyBase);
		if(IconBase)        CloseLibrary(IconBase);
#pragma msg 88 ignore
		if(GfxBase)         CloseLibrary(GfxBase);
#pragma msg 88 warn
		if(GadToolsBase)    CloseLibrary(GadToolsBase);
		//if(DOSBase)         CloseLibrary(DOSBase);
		if(DiskfontBase)    CloseLibrary(DiskfontBase);
		if(ConfigIOBase)    CloseLibrary(ConfigIOBase);
		if(AslBase)         CloseLibrary(AslBase);

		for(i=0;i<num_args;i++) free(file_args[i]);

		MSG("CLLEV_LIBS okay");
	}
	OUTRO_BYTE(return_code);
	exit(return_code);
}

void SetDefs(void) {
	NewList(&BufferList);                       /* initialize bufferlist */

	RunTime.blist=&BufferList;

	RunTime.plst=&plst;
	RunTime.plst2=&plst2;
	RunTime.AHIBase=AHIBase;
	RunTime.audioctrl=audioctrl;
	RunTime.optp_Amiga_8bit=&optp_Amiga_8bit;
	RunTime.optp_Amiga_14bit=&optp_Amiga_14bit;
	RunTime.optp_Amiga_14cbit=&optp_Amiga_14cbit;
	RunTime.optp_Ahi=&optp_Ahi;
	RunTime.plsa=plsa;

	RunTime.sfxprefs_sbuf=&sfxprefs_sbuf;
	RunTime.sfxprefs_vmem=&sfxprefs_vmem;
	RunTime.sfxprefs_gui =&sfxprefs_gui;
	RunTime.sfxprefs_misc=&sfxprefs_misc;

	NewList(&RunTime.gblist);
	RunTime.samplestat=SMPSTAT_FIRSTNEW;
	sprintf(RunTime.username,"SoundFX");
//  sprintf(RunTime.username,"Registered to Amy Resource");

	RunTime.stringextend=&stringextend;
	RunTime.tablelvhook=&tablelvhook;
	RunTime.colorlvhook=&colorlvhook;

	RunTime.quiet=FALSE;

#ifdef CHECK_REG
	SetKeyStatus_0;                                             /* Keyfile testen */
	ERRC4(!IsKeyStatusOkay,"!!! DEMO !!! : %d %d %d %d",RunTime.status,RunTime.status2,RunTime.status3,RunTime.keyerr);
#endif

	srand(time(NULL));                          /* init the random-number-generator */

	UnLock(CreateDir("env:sfx"));               /* create paths for configs */
	UnLock(CreateDir("env:sfx/operators"));
	UnLock(CreateDir("env:sfx/savers"));
	UnLock(CreateDir("env:sfx/loaders"));
//  UnLock(CreateDir("env:sfx/players"));

#ifdef CHECK_REG
	SetKeyStatus2_0;                                         /* Keyfile testen */
	SetKeyStatus_3;
	SetKeyStatus3_0;
	ERRC4(!IsKeyStatusOkay,"!!! DEMO !!! : %d %d %d %d",RunTime.status,RunTime.status2,RunTime.status3,RunTime.keyerr);
#endif
}

void SetLocaleTexts(void) {
	//File_Reqtags_TitleText	=(ULONG)GetString(strLoad);
	Font_Reqtags_TitleText	=(ULONG)GetString(strSelectFont);
	ScrMd_Reqtags_TitleText	=(ULONG)GetString(strSelectScrMd);

	//-- main menu
	SFXMenu[MAIN_MENIX_PROJECT  + 0].nm_Label=GetString(menProject);
	SFXMenu[MAIN_MENIX_PROJECT  + 1].nm_Label=GetString(menPrjNew);
	SFXMenu[MAIN_MENIX_PROJECT  + 2].nm_Label=GetString(menPrjLoad);
	SFXMenu[MAIN_MENIX_PROJECT  + 3].nm_Label=GetString(menPrjSave);
	SFXMenu[MAIN_MENIX_PROJECT  + 5].nm_Label=GetString(menPrjFlush);
	SFXMenu[MAIN_MENIX_PROJECT  + 6].nm_Label=GetString(menPrjFlushAll);
	SFXMenu[MAIN_MENIX_PROJECT  + 7].nm_Label=GetString(menPrjFlushHidden);
	SFXMenu[MAIN_MENIX_PROJECT  + 8].nm_Label=GetString(menPrjFlushShown);
	SFXMenu[MAIN_MENIX_PROJECT  +10].nm_Label=GetString(menPrjRunOperator);
	SFXMenu[MAIN_MENIX_PROJECT  +11].nm_Label=GetString(menPrjRunRexx);
	SFXMenu[MAIN_MENIX_PROJECT  +13].nm_Label=GetString(menPrjPlayAll);
	SFXMenu[MAIN_MENIX_PROJECT  +14].nm_Label=GetString(menPrjPlayRange);
	SFXMenu[MAIN_MENIX_PROJECT  +15].nm_Label=GetString(menPrjStop);
	SFXMenu[MAIN_MENIX_PROJECT  +16].nm_Label=GetString(menPrjRecord);			/* !!! LOCALIZE separately ?, or rename to menPrjRecord */
	SFXMenu[MAIN_MENIX_PROJECT  +18].nm_Label=GetString(menPrjBatchProc);
	SFXMenu[MAIN_MENIX_PROJECT  +20].nm_Label=GetString(menPrjInfo);
	SFXMenu[MAIN_MENIX_PROJECT  +28].nm_Label=GetString(menPrjQuit);

	SFXMenu[MAIN_MENIX_EDIT     + 0].nm_Label=GetString(menEdit);
	SFXMenu[MAIN_MENIX_EDIT     + 1].nm_Label=GetString(menEditCut);
	SFXMenu[MAIN_MENIX_EDIT     + 2].nm_Label=GetString(menEditCutBeg);
	SFXMenu[MAIN_MENIX_EDIT     + 3].nm_Label=GetString(menEditCutEnd);
	SFXMenu[MAIN_MENIX_EDIT     + 4].nm_Label=GetString(menEditCopy);
	SFXMenu[MAIN_MENIX_EDIT     + 5].nm_Label=GetString(menEditCopyBeg);
	SFXMenu[MAIN_MENIX_EDIT     + 6].nm_Label=GetString(menEditCopyEnd);
	SFXMenu[MAIN_MENIX_EDIT     + 7].nm_Label=GetString(menEditGrab);
	SFXMenu[MAIN_MENIX_EDIT     + 8].nm_Label=GetString(menEditGrabBeg);
	SFXMenu[MAIN_MENIX_EDIT     + 9].nm_Label=GetString(menEditGrabEnd);
	SFXMenu[MAIN_MENIX_EDIT     +10].nm_Label=GetString(menEditPaste);
	SFXMenu[MAIN_MENIX_EDIT     +11].nm_Label=GetString(menEditPasteBeg);
	SFXMenu[MAIN_MENIX_EDIT     +12].nm_Label=GetString(menEditPasteEnd);
	SFXMenu[MAIN_MENIX_EDIT     +13].nm_Label=GetString(menEditErase);
	SFXMenu[MAIN_MENIX_EDIT     +14].nm_Label=GetString(menEditEraseBeg);
	SFXMenu[MAIN_MENIX_EDIT     +15].nm_Label=GetString(menEditEraseEnd);
	SFXMenu[MAIN_MENIX_EDIT     +16].nm_Label=GetString(menEditZero);
	SFXMenu[MAIN_MENIX_EDIT     +17].nm_Label=GetString(menEditZeroBeg);
	SFXMenu[MAIN_MENIX_EDIT     +18].nm_Label=GetString(menEditZeroEnd);
	SFXMenu[MAIN_MENIX_EDIT     +19].nm_Label=GetString(menEditOverwrite);
	SFXMenu[MAIN_MENIX_EDIT     +20].nm_Label=GetString(menEditOverwriteBeg);
	SFXMenu[MAIN_MENIX_EDIT     +22].nm_Label=GetString(menEditExchangeMarkAndClip);
	SFXMenu[MAIN_MENIX_EDIT     +23].nm_Label=GetString(menEditReplaceMarkByClip);

	SFXMenu[MAIN_MENIX_RANGE    + 0].nm_Label=GetString(menRange);
	SFXMenu[MAIN_MENIX_RANGE    + 1].nm_Label=GetString(menRangeMark2Loop);
	SFXMenu[MAIN_MENIX_RANGE    + 2].nm_Label=GetString(menRangeLoop2Mark);
	SFXMenu[MAIN_MENIX_RANGE    + 3].nm_Label=GetString(menRangeZoom2Loop);
	SFXMenu[MAIN_MENIX_RANGE    + 4].nm_Label=GetString(menRangeZoom2Mark);
	SFXMenu[MAIN_MENIX_RANGE    + 6].nm_Label=GetString(menRangeMarkOff);
	SFXMenu[MAIN_MENIX_RANGE    + 7].nm_Label=GetString(menRangeMarkAll);

	SFXMenu[MAIN_MENIX_ZOOM     + 0].nm_Label=GetString(menZoom);
	SFXMenu[MAIN_MENIX_ZOOM     + 1].nm_Label=GetString(menZoomIn);
	SFXMenu[MAIN_MENIX_ZOOM     + 2].nm_Label=GetString(menZoomOut);
	SFXMenu[MAIN_MENIX_ZOOM     + 3].nm_Label=GetString(menZoomPixel);
	SFXMenu[MAIN_MENIX_ZOOM     + 4].nm_Label=GetString(menZoomFull);

	SFXMenu[MAIN_MENIX_CLEANUP  + 0].nm_Label=GetString(menCleanup);
	SFXMenu[MAIN_MENIX_CLEANUP  + 1].nm_Label=GetString(menClCur);
	SFXMenu[MAIN_MENIX_CLEANUP  + 2].nm_Label=GetString(menClAll);
	SFXMenu[MAIN_MENIX_CLEANUP  + 3].nm_Label=GetString(menClAllNorm);
	SFXMenu[MAIN_MENIX_CLEANUP  + 4].nm_Label=GetString(menClAllZoom);

	SFXMenu[MAIN_MENIX_UTILITIES+ 0].nm_Label=GetString(menUtilities);
	SFXMenu[MAIN_MENIX_UTILITIES+ 1].nm_Label=GetString(menUtSwapEnd);
	SFXMenu[MAIN_MENIX_UTILITIES+ 2].nm_Label=GetString(menUtSwapSign);
	SFXMenu[MAIN_MENIX_UTILITIES+ 4].nm_Label=GetString(menUtInterleave);
	SFXMenu[MAIN_MENIX_UTILITIES+ 5].nm_Label=GetString(menUtDeInterleave);

	SFXMenu[MAIN_MENIX_PREFS    + 0].nm_Label=GetString(menPrefs);
	SFXMenu[MAIN_MENIX_PREFS    + 1].nm_Label=GetString(menPrefsGUI);
	SFXMenu[MAIN_MENIX_PREFS    + 2].nm_Label=GetString(menPrefsSample);
	SFXMenu[MAIN_MENIX_PREFS    + 3].nm_Label=GetString(menPrefsVMem);
	SFXMenu[MAIN_MENIX_PREFS    + 4].nm_Label=GetString(menPrefsMisc);
	SFXMenu[MAIN_MENIX_PREFS    + 6].nm_Label=GetString(menPrefsUse);
	SFXMenu[MAIN_MENIX_PREFS    + 7].nm_Label=GetString(menPrefsSave);
	SFXMenu[MAIN_MENIX_PREFS    + 9].nm_Label=GetString(menPrefsRestoreUsed);
	SFXMenu[MAIN_MENIX_PREFS    +10].nm_Label=GetString(menPrefsRestoreSaved);
	SFXMenu[MAIN_MENIX_PREFS    +11].nm_Label=GetString(menPrefsDefault);

	SFXMenu[MAIN_MENIX_HELP     + 0].nm_Label=GetString(menHelp);
	SFXMenu[MAIN_MENIX_HELP     + 1].nm_Label=GetString(menHelpContents);
	SFXMenu[MAIN_MENIX_HELP     + 2].nm_Label=GetString(menHelpIndex);
	SFXMenu[MAIN_MENIX_HELP     + 3].nm_Label=GetString(menHelpWorkshop);
	SFXMenu[MAIN_MENIX_HELP     + 5].nm_Label=GetString(menHelpOnline);
	SFXMenu[MAIN_MENIX_HELP     + 6].nm_Label=GetString(menHelpMail);
	SFXMenu[MAIN_MENIX_HELP     + 8].nm_Label=GetString(menHelpShowTips);
	SFXMenu[MAIN_MENIX_HELP     +10].nm_Label=GetString(menAbout);

	//-- modul menu
	FXMenu[MOD_MENIX_PROJECT  + 0].nm_Label=GetString(menProject);
	FXMenu[MOD_MENIX_PROJECT  + 1].nm_Label=GetString(menPrefsLoad);
	FXMenu[MOD_MENIX_PROJECT  + 2].nm_Label=GetString(menPrefsSave);
	FXMenu[MOD_MENIX_PROJECT  + 3].nm_Label=GetString(menPrefsRestoreSaved);
	FXMenu[MOD_MENIX_PROJECT  + 4].nm_Label=GetString(menPrefsDefault);
	FXMenu[MOD_MENIX_PROJECT  + 6].nm_Label=GetString(menStart);
	FXMenu[MOD_MENIX_PROJECT  + 8].nm_Label=GetString(menPrjQuit);
	
	FXMenu[MOD_MENIX_HELP     + 0].nm_Label=GetString(menHelp);
	FXMenu[MOD_MENIX_HELP     + 1].nm_Label=GetString(menHelp);
	FXMenu[MOD_MENIX_HELP     + 3].nm_Label=GetString(menAbout);

	//-- toolbar
	SFXToolbar[GADIX_TB_LOADER      ].nt_Label=
	SFXToolbar[GADIX_TB_CHLOADER    ].nt_Label=GetString(tbbLoaderChoice);
	SFXToolbar[GADIX_TB_PREFLO      ].nt_Label=GetString(tbbLoaderOptions);
	SFXToolbar[GADIX_TB_LOAD        ].nt_Label=GetString(tbbLoadFile);

	SFXToolbar[GADIX_TB_SAVER       ].nt_Label=
	SFXToolbar[GADIX_TB_CHSAVER     ].nt_Label=GetString(tbbSaverChoice);
	SFXToolbar[GADIX_TB_PREFSA      ].nt_Label=GetString(tbbSaverOptions);
	SFXToolbar[GADIX_TB_SAVE        ].nt_Label=GetString(tbbSaveFile);

	SFXToolbar[GADIX_TB_OPERATOR    ].nt_Label=
	SFXToolbar[GADIX_TB_CHOP        ].nt_Label=GetString(tbbOperatorChoice);
	SFXToolbar[GADIX_TB_RUNOP       ].nt_Label=GetString(tbbRunOperator);

	SFXToolbar[GADIX_TB_REXX        ].nt_Label=
	SFXToolbar[GADIX_TB_CHREXX      ].nt_Label=GetString(tbbArexxMacroChoice);
	SFXToolbar[GADIX_TB_RUNREXX     ].nt_Label=GetString(tbbRunArexxMacro);

	SFXToolbar[GADIX_TB_PLAYER      ].nt_Label=
	SFXToolbar[GADIX_TB_CHPLAYER    ].nt_Label=GetString(tbbPlayerChoice);
	SFXToolbar[GADIX_TB_PREFPL      ].nt_Label=GetString(tbbPlayerOptions);
	SFXToolbar[GADIX_TB_PLAYALL     ].nt_Label=GetString(tbbPlayAll);
	SFXToolbar[GADIX_TB_PLAYRNG     ].nt_Label=GetString(tbbPlayRange);
	SFXToolbar[GADIX_TB_STOP        ].nt_Label=GetString(tbbStopPlaying);
	SFXToolbar[GADIX_TB_RECORD      ].nt_Label=GetString(tbbRecord);

	SFXToolbar[GADIX_TB_BUFFER      ].nt_Label=
	SFXToolbar[GADIX_TB_CHBUF       ].nt_Label=GetString(tbbBufferChoice);
	SFXToolbar[GADIX_TB_PREFBUF     ].nt_Label=GetString(tbbBufferOptions);
	SFXToolbar[GADIX_TB_HIDESHOW    ].nt_Label=GetString(tbbHideShow0);		 /* multiple texts depending on state */

	SFXToolbar[GADIX_TB_CUTMEN      ].nt_Label=GetString(tbbCutMen);
	SFXToolbar[GADIX_TB_COPYMEN     ].nt_Label=GetString(tbbCopyMen);
	SFXToolbar[GADIX_TB_PASTEMEN    ].nt_Label=GetString(tbbPasteMen);
	SFXToolbar[GADIX_TB_ERASEMEN    ].nt_Label=GetString(tbbEraseMen);
	SFXToolbar[GADIX_TB_GRABMEN     ].nt_Label=GetString(tbbGrabMen);
	SFXToolbar[GADIX_TB_ZEROMEN     ].nt_Label=GetString(tbbZeroMen);
	SFXToolbar[GADIX_TB_OVERWRMEN   ].nt_Label=GetString(tbbOverwrMen);

	SFXToolbar[GADIX_TB_ZOOMMODE    ].nt_Label=GetString(tbbZoomMode0);		 /* multiple texts depending on state */
	SFXToolbar[GADIX_TB_ZOOMIN      ].nt_Label=GetString(tbbZoomIn);
	SFXToolbar[GADIX_TB_ZOOMOUT     ].nt_Label=GetString(tbbZoomOut);
	SFXToolbar[GADIX_TB_ZOOMPIXEL   ].nt_Label=GetString(tbbZoomEq);
	SFXToolbar[GADIX_TB_ZOOMFULL    ].nt_Label=GetString(tbbZoomOutFull);

	SFXToolbar[GADIX_TB_RNGMODE     ].nt_Label=GetString(tbbRangeMode0);	 /* multiple texts depending on state */
	SFXToolbar[GADIX_TB_LOCKLEN     ].nt_Label=GetString(tbbLockLength);
	SFXToolbar[GADIX_TB_MOVEMODE    ].nt_Label=GetString(tbbChBegEnd);
	SFXToolbar[GADIX_TB_MOVEBEG     ].nt_Label=GetString(tbbMoveBeg);
	SFXToolbar[GADIX_TB_MOVELEFTF   ].nt_Label=GetString(tbbMoveLeftF);
	SFXToolbar[GADIX_TB_MOVELEFTS   ].nt_Label=GetString(tbbMoveLeftS);
	SFXToolbar[GADIX_TB_MOVELEFTN   ].nt_Label=GetString(tbbMoveLeftN);
	SFXToolbar[GADIX_TB_MOVERIGHTN  ].nt_Label=GetString(tbbMoveRightN);
	SFXToolbar[GADIX_TB_MOVERIGHTS  ].nt_Label=GetString(tbbMoveRightS);
	SFXToolbar[GADIX_TB_MOVERIGHTF  ].nt_Label=GetString(tbbMoveRightF);
	SFXToolbar[GADIX_TB_MOVEEND     ].nt_Label=GetString(tbbMoveEnd);
	SFXToolbar[GADIX_TB_MOVETOP     ].nt_Label=GetString(tbbMoveTop);
	SFXToolbar[GADIX_TB_MOVEUPF     ].nt_Label=GetString(tbbMoveUpF);
	SFXToolbar[GADIX_TB_MOVEUPS     ].nt_Label=GetString(tbbMoveUpS);
	SFXToolbar[GADIX_TB_MOVEUPP     ].nt_Label=GetString(tbbMoveUpP);
	SFXToolbar[GADIX_TB_MOVEDOWNP   ].nt_Label=GetString(tbbMoveDownP);
	SFXToolbar[GADIX_TB_MOVEDOWNS   ].nt_Label=GetString(tbbMoveDownS);
	SFXToolbar[GADIX_TB_MOVEDOWNF   ].nt_Label=GetString(tbbMoveDownF);
	SFXToolbar[GADIX_TB_MOVEBOTTOM  ].nt_Label=GetString(tbbMoveBottom);

	SFXToolbar[GADIX_TB_WINMODE     ].nt_Label=GetString(tbbWindowMode0);				/* multiple texts depending on state */

	SFXToolbarCut[0].nt_Label=GetString(tbbCut);
	SFXToolbarCut[1].nt_Label=GetString(tbbCutB);
	SFXToolbarCut[2].nt_Label=GetString(tbbCutE);

	SFXToolbarCopy[0].nt_Label=GetString(tbbCopy);
	SFXToolbarCopy[1].nt_Label=GetString(tbbCopyB);
	SFXToolbarCopy[2].nt_Label=GetString(tbbCopyE);

	SFXToolbarPaste[0].nt_Label=GetString(tbbPaste);
	SFXToolbarPaste[1].nt_Label=GetString(tbbPasteB);
	SFXToolbarPaste[2].nt_Label=GetString(tbbPasteE);

	SFXToolbarErase[0].nt_Label=GetString(tbbErase);
	SFXToolbarErase[1].nt_Label=GetString(tbbEraseB);
	SFXToolbarErase[2].nt_Label=GetString(tbbEraseE);

	SFXToolbarGrab[0].nt_Label=GetString(tbbGrab);
	SFXToolbarGrab[1].nt_Label=GetString(tbbGrabB);
	SFXToolbarGrab[2].nt_Label=GetString(tbbGrabE);

	SFXToolbarZero[0].nt_Label=GetString(tbbZero);
	SFXToolbarZero[1].nt_Label=GetString(tbbZeroB);
	SFXToolbarZero[2].nt_Label=GetString(tbbZeroE);

	SFXToolbarOverwr[0].nt_Label=GetString(tbbOverwr);
	SFXToolbarOverwr[1].nt_Label=GetString(tbbOverwrB);
}

int main(int argc,char *argv[]) {
	struct IntuiMessage *imsg,*imsg2;
	struct Window       *iwin,*owin;
	ULONG iclass;
	APTR iadr;
	UWORD icode,iqual;
	int mousex,mousey;
	ULONG sig_out;
	SInfo *si,*clsi=NULL,*osi=NULL;
	BOOL closedd;
	struct Node *node;
	UBYTE i,j;

	INTRO;

	//-- check commandline
	RunTime.sfxproc=(struct Process *)FindTask(NULL);
	MSG("checking commandline args");	// something in the startup code crashes if we are trying to print argc/num_args/...
	if(argc) {
		char drive[FNSIZE],path[FNSIZE],fn[FILENAME_MAX];

		MSG("-> startup from shell");
		if((argv[1]) && (*argv[1]=='?')) printf("SoundFX does not support any shell parameters.\n");
		num_args=min(argc,MAX_ARGS);
		for(i=0,j=0;i<num_args;i++) {
			if(argv[i] && *argv[i]) {
				strsfn(argv[i],drive,NULL,NULL,NULL);
				if(strlen(drive)==0) { 
					GetCurrentDirName(path,FNSIZE-1);
					sprintf(fn,"%s/%s",path,argv[i]);
					file_args[j++]=strdup(fn);
				}
				else {
					file_args[j++]=strdup(argv[i]);
				}
			}
		}
		num_args=j;
	}
	else {
		struct WBStartup *startup;
		struct WBArg *arg;
		char fn[FILENAME_MAX],path[FNSIZE];

		MSG("-> startup from wb");
		startup=(struct WBStartup *)argv;
		num_args=min(startup->sm_NumArgs,MAX_ARGS);
		//puts(startup->sm_ToolWindow); // whatever should be in there, it is empty

		sprintf(fn,"progdir:%s",RunTime.sfxproc->pr_Task.tc_Node.ln_Name);
		file_args[0]=strdup(fn);

		for(i=0,j=1;i<num_args;i++) {
			arg=&startup->sm_ArgList[i];
			if(arg) {
				NameFromLock(arg->wa_Lock,path,FNSIZE);
				if(arg->wa_Name) {
					strmfp(fn,path,arg->wa_Name);
					file_args[j++]=strdup(fn);
				}
				else file_args[j++]=strdup(path);
			}
		}
		num_args=j;
	}

	//-- normal startup
	OpenLibs();
	OpenGFX();                                      /* opens bootup-window */
	OpenAll();
	CloseGFX();                                     /* closes bootup-window */
	ScreenToFront(RunTime.sfxpubscr);
	CheckCrashMark();								/* tests if previous sfx has finished properly */
	if(writeprefs) Message(msgPrefsHaveChanged,NULL,NULL,0);

#ifdef CHECK_REG
	if(!IsKeyStatusOkay) {
		ERR4("!!! DEMO !!! : %d %d %d %d",RunTime.status,RunTime.status2,RunTime.status3,RunTime.keyerr);
		Message(msgUnregVersionStart,NULL,NULL,0);
	}
#endif
	if(sfxprefs_misc.showtips) {										/* Show Tips */
		SystemTags("showtip "SFX_PUBSCRNAME,SYS_Asynch,TRUE,SYS_Output,NULL,SYS_Input,NULL,TAG_DONE);
	}

	if(num_args) {
		/** @ todo check here for initial files to load */
		MSG("we have initial files to load");
		MSG1("  num_args=%ld",num_args);
		for(i=1;i<num_args;i++) {
			MSG2("  file_args[%ld]=\"%s\"",i,file_args[i]);
			LoadSampleRexx(file_args[i]);
		}
	}

	while(!RunTime.quitsfx) {
		closedd=FALSE;
		sig_out=Wait(RunTime.sig_in);
		//MSG1("sigout : 0x%08lx",sig_out);

#ifdef CHECK_REG
		//-- a timer would be better (arexx-scripts produce a lot of events
		keycounter++;
		if(!RunTime.play && !(keycounter%2142)) {                   // do not interfere with play-routines
			SetKeyStatus_4;
			if(IsKeyStatusIllegal_1 || IsKeyStatusIllegal_2) RunTime.status=STAT_DEMO;
			SetKeyStatus2_1;
			SetKeyStatus3_0;
			ERRC4(!IsKeyStatusOkay,"!!! DEMO !!! : %d %d %d %d",RunTime.status,RunTime.status2,RunTime.status3,RunTime.keyerr);
		}
#endif
		if(sig_out&apsig)         DoIO_App();
		if(sig_out&rxsig) {       DoIO_Rexx();keycounter--; }
		if(sig_out&bwsig) {
			while(imsg=GT_GetIMsg(RunTime.winBackDrop->UserPort)) {
				iclass  =imsg->Class;
				iadr    =imsg->IAddress;
				icode   =imsg->Code;
				iqual   =imsg->Qualifier;
				mousex  =imsg->MouseX;
				mousey  =imsg->MouseY;
				GT_ReplyIMsg(imsg);
				NG_HandleEvents(iclass,icode,mousex,mousey,iadr,RunTime.winBackDrop,&closedd);
				Test_BDWin(iclass,icode,mousex,mousey,iqual,iadr);
			}
		}
		if(sig_out&gwsig) {
			while(imsg=GT_GetIMsg(gwin_up)) {
				iwin    =imsg->IDCMPWindow;
				iclass  =imsg->Class;
				iadr    =imsg->IAddress;
				icode   =imsg->Code;
				iqual   =imsg->Qualifier;
				mousex  =imsg->MouseX;
				mousey  =imsg->MouseY;
				GT_ReplyIMsg(imsg);
				if(iwin==RunTime.winStatus) {
					NG_HandleEvents(iclass,icode,mousex,mousey,iadr,RunTime.winStatus,&closedd);
					Test_StatusWin(iclass,icode,mousex,mousey,iqual,iadr);
				}
				else if(iwin==RunTime.winToolbar) {
					NG_HandleEvents(iclass,icode,mousex,mousey,iadr,RunTime.winToolbar,&closedd);
					Test_ToolbarWin(iclass,icode,mousex,mousey,iqual,iadr);
				}
				else if(iwin==RunTime.winDropDown) {
					NG_HandleEvents(iclass,icode,mousex,mousey,iadr,RunTime.winDropDown,&closedd);
					Test_DropDownWin(iclass,icode,mousex,mousey,iqual,iadr,&closedd);
				}
			}
		}
		if(sig_out&swsig) {
			//MSG("SampleWinMessage begin");
			imsg2=NULL;owin=NULL;

			//-- alternative implementation : fails because 'imgs2=NULL' gets not executed
			//while((imsg2 && ((imsg=imsg2) || (imsg2=NULL))) || (imsg=GT_GetIMsg(swin_up)))

			while((imsg2) || (imsg2=GT_GetIMsg(swin_up))) {
				imsg=imsg2;imsg2=NULL;

				iwin    =imsg->IDCMPWindow;
				iclass  =imsg->Class;
				iadr    =imsg->IAddress;
				icode   =imsg->Code;
				iqual   =imsg->Qualifier;
				mousex  =imsg->MouseX;
				mousey  =imsg->MouseY;
				GT_ReplyIMsg(imsg);
				/** @todo if(RunTime.owsi.win) */
				if(owin==iwin) si=osi;                      // msg for the same window
				else {                                      // msg for another window, lookup sound-buffer
					si=NULL;
					foreach(node,&BufferList) {                 // search for window
						if(iwin==((SInfo *)node)->win) { owin=iwin;osi=si=(SInfo *)node;break; }
					}
				}
				if(si) {
					NG_HandleEvents(iclass,icode,mousex,mousey,iadr,iwin,&closedd);
					Test_SampleWin(iclass,icode,mousex,mousey,iqual,iadr,si,&clsi,&imsg2);

					if(RunTime.play && RunTime.plsi	&& SFX_SOUND_IS_SHOWN(RunTime.plsi)	&& (RunTime.plptr<RunTime.plsi->slen)) SetPlayStatus();
				}
				else MSG("message for invalid window");
			}
			if(clsi) {
				//MSG("closing window ...");
				RemSample(clsi);clsi=NULL;
				if(!RunTime.aktsamples) RunTime.samplestat=SMPSTAT_LASTOLD;
				else if(RunTime.aktbuf && SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) ActivateWindow(RunTime.aktbuf->win);
				//MSG("done ...");
			}
			//MSG("SampleWinMessage end");
		}

		//-- out of eventloop processing

		if(RunTime.rngmove==RNGMOVE_X) RangeMoveHorizontal();	// Left, Right
		if(RunTime.rngmove==RNGMOVE_Y) RangeMoveVertical();		// Up, Down

		if(RunTime.play) {
			if(RunTime.plsi	&& SFX_SOUND_IS_SHOWN(RunTime.plsi)	&& (RunTime.plptr<RunTime.plsi->slen)) SetPlayStatus();
			if(RunTime.plstop>1) StopSample();
		}
		if(RunTime.tryquit) {
			while(RunTime.aktsamples) RemSample(RunTime.aktbuf);
			NewList(&BufferList);
			// try to make the screen private
			if(!(PubScreenStatus(RunTime.sfxpubscr,PSNF_PRIVATE)&(UWORD)0x1)) {                 // noch visitor windows  da ?
				Message(errVisitorWindows,NULL,__FILE__,__LINE__);RunTime.tryquit=FALSE;
			}
			else RunTime.quitsfx=TRUE;
		}
		if(RunTime.tryreopen) {
			GUI_ReOpen();
			RunTime.tryreopen=FALSE;
		}
		if(RunTime.changesamples) {
			//MSG2("ChangeSamples %2ld,%1ld",RunTime.aktsamples,RunTime.samplestat);
			RunTime.changesamples=FALSE;ChangeSamples();
			foreach(node,&RunTime.gblist) {
				Signal(((NotifyTask *)node)->task,(1L<<((NotifyTask *)node)->sig));
			}
		}
		if(RunTime.winDropDown && closedd) {
			CloseMWindow(RunTime.winDropDown,&RunTime.gwin_upct);RunTime.winDropDown=NULL;
			FreeGadgets(gadDropDown);gadDropDown=NULL;
			closedd=FALSE;
			if(RunTime.aktbuf && (SFX_SOUND_IS_SHOWN(RunTime.aktbuf))) ActivateWindow(RunTime.aktbuf->win);
		}
	}
#ifdef CHECK_REG
	if(!IsKeyStatusOkay) {
		ERR4("!!! DEMO !!! : %d %d %d %d",RunTime.status,RunTime.status2,RunTime.status3,RunTime.keyerr);
		Message(msgUnregVersionEnd,NULL,NULL,0);
	}
#endif
	//++++++++++++++++++++++++++++++++++++ only in Version for Aminet
	//#ifdef RESTRICT
	//  Message(RunTime.mess[37],NULL,0);
	//#endif
	//++++++++++++++++++++++++++++++++++++ only in Version for Aminet

	DeleteCrashMark();

	CloseAll(CLLEV_ALL,RETURN_OK);
	// this is just for the compiler
	return(0);
}

//-- eof ----------------------------------------------------------------------
