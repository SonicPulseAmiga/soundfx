/****h* SoundFX/SFX-IORexx.c [4.3] *
*
*  NAME
*    SFX-IORexx.c
*  COPYRIGHT
*    $VER: SFX-IORexx.c 4.3 (31.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    ARexx port for SoundFX - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    16.Jul.1998
*  MODIFICATION HISTORY
*    31.Mar.2004	V 4.3	most recent version
*                           A: Get/SetLoaderParam, SetLoaderPreset
*                           A: Documentation for functions
*    12.Jun.2002    V 4.2   most recent version
*    24.Mar.2001    V 4.1   most recent version
*                           A: ToFront,ToBack
*                           A: Get/SetSaverParam, SetSaverPreset, Get/SetQuietMode
*    01.Feb.2000    V 3.82  most recent version
*    12.May.1999    V 3.81  most recent version
*    02.Mar.1999    V 3.80  most recent version
*                           A: GetList,GetProgDir,GetStorageType,GetUserInfo,GetVersion
*                           C: DisableChannel,EnableChannel
*    08.Dec.1998    V 3.71  most recent version
*    18.Aug.1998    V 3.70  most recent version
*    16.Jul.1998    V 3.70  initial version
*  NOTES
*    * the following functions have been removed
*      "Get/SetDrawMode"
*      "Get/SetMaxLines"
*      "Get/SetRasterX/Y"
*
*******
*/

#define SFX_IORexx_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void DoIO_Rexx(void);

//-- helper

WORD GetRexxStr(char *rstr,char *params);
void GetSrcFromRexx(SInfo **src,char *params);
enum RexxCmdId GetCmdId(const char * const strCmdName);
void RexxError(struct RexxMsg *rmsg,ULONG errnum);

//-- defines ------------------------------------------------------------------

#define REXXERR_CmdNotFound          1
#define REXXERR_ErrorInCmdString    11
#define REXXERR_WrongNrOfArgs       17
#define REXXERR_InvalidArg          18

//-- globals ------------------------------------------------------------------

enum RexxCmdId {
	REXXID_Activate,
	REXXID_CleanUp,
	REXXID_DisableChannel,
	REXXID_EditCopy,
	REXXID_EditCopyB,
	REXXID_EditCopyE,
	REXXID_EditCut,
	REXXID_EditCutB,
	REXXID_EditCutE,
	REXXID_EditErase,
	REXXID_EditEraseB,
	REXXID_EditEraseE,
	REXXID_EditGrab,
	REXXID_EditGrabB,
	REXXID_EditGrabE,
	REXXID_EditOverwrite,
	REXXID_EditOverwriteB,
	REXXID_EditPaste,
	REXXID_EditPasteB,
	REXXID_EditPasteE,
	REXXID_EditZero,
	REXXID_EditZeroB,
	REXXID_EditZeroE,
	REXXID_EnableChannel,
	REXXID_Exit,
	REXXID_GetActiveBuffer,
	REXXID_GetBufferName,
	REXXID_GetChannels,
	REXXID_GetLength,
	REXXID_GetList,
	REXXID_GetLoaderParam,              // 4.3
	REXXID_GetLoopEnd,
	REXXID_GetLoopLength,
	REXXID_GetLoopMode,
	REXXID_GetLoopStart,
	REXXID_GetMarkXEnd,
	REXXID_GetMarkXLength,
	REXXID_GetMarkXStart,
	REXXID_GetMarkYEnd,
	REXXID_GetMarkYLength,
	REXXID_GetMarkYStart,
	REXXID_GetOperatorParam,
	REXXID_GetProgDir,                  // 3.8
	REXXID_GetQuietMode,                // 4.1
	REXXID_GetRate,
	REXXID_GetSample,
	REXXID_GetSampleValue,
	REXXID_GetSaverParam,               // 4.1
	REXXID_GetStorageType,              // 3.8
	REXXID_GetUserInfo,                 // 3.8
	REXXID_GetVersion,                  // 3.8
	REXXID_GetZoomXEnd,
	REXXID_GetZoomXLength,
	REXXID_GetZoomXStart,
	REXXID_GetZoomYEnd,
	REXXID_GetZoomYLength,
	REXXID_GetZoomYStart,
	REXXID_HideBuffer,                  // 4.1
	REXXID_InitProWin,
	REXXID_IsChannelActive,
	REXXID_LoadSample,
	REXXID_Message,
	REXXID_NewBuffer,
	REXXID_ProcessSample,
	REXXID_PutSample,
	REXXID_PutSampleValue,
	REXXID_RedrawBuffer,
	REXXID_RefrProWin,
	REXXID_RemoveBuffer,
	REXXID_RemoveProWin,
	REXXID_RenameBuffer,
	REXXID_SaveSample,
	REXXID_SearchBuffer,
	REXXID_SelLoader,
	REXXID_SelOperator,
	REXXID_SelPlayer,
	REXXID_SelSaver,
	REXXID_SetActiveBuffer,
	REXXID_SetLength,
	REXXID_SetLoaderParam,             // 4.3
	REXXID_SetLoaderPreset,            // 4.3
	REXXID_SetLoopEnd,
	REXXID_SetLoopLength,
	REXXID_SetLoopMode,
	REXXID_SetLoopStart,
	REXXID_SetMarkXEnd,
	REXXID_SetMarkXLength,
	REXXID_SetMarkXStart,
	REXXID_SetMarkYEnd,
	REXXID_SetMarkYLength,
	REXXID_SetMarkYStart,
	REXXID_SetOperatorParam,
	REXXID_SetOperatorPreset,
	REXXID_SetQuietMode,                // 4.1
	REXXID_SetRate,
	REXXID_SetSaverParam,               // 4.1
	REXXID_SetSaverPreset,              // 4.1
	REXXID_SetZoomXEnd,
	REXXID_SetZoomXLength,
	REXXID_SetZoomXStart,
	REXXID_SetZoomYEnd,
	REXXID_SetZoomYLength,
	REXXID_SetZoomYStart,
	REXXID_ShowBuffer,                  // 4.1
	REXXID_ToBack,                      // 4.1
	REXXID_ToFront,                     // 4.1
	REXXCmdCount
};

struct RexxCmdTab {
	char            *strCmdName;
	enum RexxCmdId  idCmd;
	UBYTE           numParams; // incl. Cmd.
} RexxCmds[]= { // all commands have a prefix "SFX"
	"Activate",                 REXXID_Activate,            1,  /** # # Brings __SFX screen to front */
	"CleanUp",                  REXXID_CleanUp,             2,  /** # Mode[0=Cur,1=All,2=AllNormal,3=AllZoomed] # Reorder window(s) on __SFX screen */
	"DisableChannel",           REXXID_DisableChannel,      3,  /** # BufferId ChannelNo # Deactivate a given channel */
	"EditCopy",                 REXXID_EditCopy,            2,  /** # BufferId # Copies the selected region */
	"EditCopyB",                REXXID_EditCopyB,           2,  /** # BufferId # Copies the selected region (sample-begin to region-end) */
	"EditCopyE",                REXXID_EditCopyE,           2,  /** # BufferId # Copies the selected region (region-begin to sample-end) */
	"EditCut",                  REXXID_EditCut,             2,  /** # BufferId # Cuts the selected region */
	"EditCutB",                 REXXID_EditCutB,            2,  /** # BufferId # Cuts the selected region (sample-begin to region-end) */
	"EditCutE",                 REXXID_EditCutE,            2,  /** # BufferId # Cuts the selected region (region-begin to sample-end) */
	"EditErase",                REXXID_EditErase,           2,  /** # BufferId # Erases the selected region */
	"EditEraseB",               REXXID_EditEraseB,          2,  /** # BufferId # Erases the selected region (sample-begin to region-end) */
	"EditEraseE",               REXXID_EditEraseE,          2,  /** # BufferId # Erases the selected region (region-begin to sample-end) */
	"EditGrab",                 REXXID_EditGrab,            2,  /** # BufferId # Copies the selected region into a new buffer */
	"EditGrabB",                REXXID_EditGrabB,           2,  /** # BufferId # Copies the selected region into a new buffer (sample-begin to region-end) */
	"EditGrabE",                REXXID_EditGrabE,           2,  /** # BufferId # Copies the selected region into a new buffer (region-begin to sample-end) */
	"EditOverwrite",            REXXID_EditOverwrite,       2,  /** # BufferId # Overwrite samples with contents of the copy-buffer starting from the begin of the selected region */
	"EditOverwriteB",           REXXID_EditOverwriteB,      2,  /** # BufferId # Overwrite samples with contents of the copy-buffer starting from the begin of the sample */
	"EditPaste",                REXXID_EditPaste,           2,  /** # BufferId # Inserts the contents of the copy-buffer at the region marker */
	"EditPasteB",               REXXID_EditPasteB,          2,  /** # BufferId # Inserts the contents of the copy-buffer at the region begin */
	"EditPasteE",               REXXID_EditPasteE,          2,  /** # BufferId # Inserts the contents of the copy-buffer at the region end */
	"EditZero",                 REXXID_EditZero,            2,  /** # BufferId # Silences the selected region */
	"EditZeroB",                REXXID_EditZeroB,           2,  /** # BufferId # Silences the selected region (sample-begin to region-end) */
	"EditZeroE",                REXXID_EditZeroE,           2,  /** # BufferId # Silences the selected region (region-begin to sample-end) */
	"EnableChannel",            REXXID_EnableChannel,       3,  /** # BufferId ChannelNo # Activate a given channel */
	"Exit",                     REXXID_Exit,                1,  /** # # Leave __SFX without asking */
	"GetActiveBuffer",          REXXID_GetActiveBuffer,     1,  /** BufferId # # Return the Id of the currently active sample */
	"GetBufferName",            REXXID_GetBufferName,       2,  /** BuferName # BufferId # Returns a string containing the name of the sample-buffer */
	"GetChannels",              REXXID_GetChannels,         2,  /** NumChannels # BufferId # Returns the number of channels for the given buffer */
	"GetLength",                REXXID_GetLength,           2,  /** Length # BufferId # Returns the length of the specified sample-buffer */
	"GetList",                  REXXID_GetList,             2,  /** List # ListName[Buffers,Loaders,Operators,Players,Savers] # Returns a new-line delimited list of available modules in the respective category */
	"GetLoaderParam",			REXXID_GetLoaderParam,		3,  /** Value # LoaderName ParamName # Returns the value of the given parameter of the given loader */
	"GetLoopEnd",               REXXID_GetLoopEnd,          2,  /** LoopEnd # BufferId # Get the end position of the loop */
	"GetLoopLength",            REXXID_GetLoopLength,       2,  /** LoopLength # BufferId # Get the length of the loop */
	"GetLoopMode",              REXXID_GetLoopMode,         2,  /** LoopMode[0=Off,1=Forward] # BufferId # Get the loop mode for the specified buffer */
	"GetLoopStart",             REXXID_GetLoopStart,        2,  /** LoopStart # BufferId # Get the start position of the loop */
	"GetMarkXEnd",              REXXID_GetMarkXEnd,         2,  /** MarkXEnd # BufferId # Get the x-end position of the mark */
	"GetMarkXLength",           REXXID_GetMarkXLength,      2,  /** MarkXLength # BufferId # Get the x-length of the mark */
	"GetMarkXStart",            REXXID_GetMarkXStart,       2,  /** MarkXStart # BufferId # Get the x-start position of the mark */
	"GetMarkYEnd",              REXXID_GetMarkYEnd,         2,  /** MarkYEnd # BufferId # Get the y-end position of the mark */
	"GetMarkYLength",           REXXID_GetMarkYLength,      2,  /** MarkYLength # BufferId # Get the y-length of the mark */
	"GetMarkYStart",            REXXID_GetMarkYStart,       2,  /** MarkYStart # BufferId # Get the y-start position of the mark */
	"GetOperatorParam",         REXXID_GetOperatorParam,    3,  /** Value # OperatorName ParamName # Returns the value of the given parameter of the given operator */
	"GetProgDir",               REXXID_GetProgDir,          1,  /** ProgDir # # Returns the pathname of __SFX installation */
	"GetQuietMode",             REXXID_GetQuietMode,        1,  /** Mode # # Returns wheter __SFX is in quiet mode */
	"GetRate",                  REXXID_GetRate,             2,  /** SampleRate # BufferId # Returns the sampling rate of the specified sample-buffer */
	"GetSample",                REXXID_GetSample,           2,  /** # DstAddress # Stores the samples of the currently active buffer into the givven memory location as PCM-8 mono data */
	"GetSampleValue",           REXXID_GetSampleValue,      4,  /** Value # BufferId ChannelId Position # Retrieves one 16-bit sample value */
	"GetSaverParam",            REXXID_GetSaverParam,       3,  /** Value # SaverName ParamName # Returns the value of the given parameter of the given saver */
	"GetStorageType",           REXXID_GetStorageType,      2,  /** StorageType[1=Mem,2=Dev] # BufferId # Returns the type of storage of the specified sample-buffer */
	"GetUserInfo",              REXXID_GetUserInfo,         1,  /** UserInfo # # Returns a text string with information of registered user */
	"GetVersion",               REXXID_GetVersion,          2,  /** VersionInfo # ComponentName[SoundFX,...] # Returns the version of the specified component in the form X.Y */
	"GetZoomXEnd",              REXXID_GetZoomXEnd,         2,  /** ZoomXEnd # BufferId # Get the x-end position of the zoom */
	"GetZoomXLength",           REXXID_GetZoomXLength,      2,  /** ZoomXLength # BufferId # Get the x-length of the zoom */
	"GetZoomXStart",            REXXID_GetZoomXStart,       2,  /** ZoomXStart # BufferId # Get the x-start position of the zoom */
	"GetZoomYEnd",              REXXID_GetZoomYEnd,         2,  /** ZoomYEnd # BufferId # Get the y-end position of the zoom */
	"GetZoomYLength",           REXXID_GetZoomYLength,      2,  /** ZoomYLength # BufferId # Get the y-length of the zoom */
	"GetZoomYStart",            REXXID_GetZoomYStart,       2,  /** ZoomYStart # BufferId # Get the y-start position of the zoom */
	"HideBuffer",               REXXID_HideBuffer,          2,  /** # BufferId # Hides a visible sample */
	"InitProWin",               REXXID_InitProWin,          3,  /** ProWinId # MaxLength Title # Creates a new progress window */
	"IsChannelActive",          REXXID_IsChannelActive,     3,  /** ChannelActive # BufferId ChannelId # Returns a alue &gt; 0 if the given channel is active */
	"LoadSample",               REXXID_LoadSample,          2,  /** BufferId # FileName # Loads the specified file with the currently selected loader */
	"Message",                  REXXID_Message,             2,  /** # MessageText # Displays the supplied text as a message box on __SFX screen */
	"NewBuffer",                REXXID_NewBuffer,           4,  /** BufferId # Length SamplingRate Channels # Prepares a new empty buffer */
	"ProcessSample",            REXXID_ProcessSample,       1,  /** BufferId # # Apply the currently selected operator to the active sample */
	"PutSample",                REXXID_PutSample,           4,  /** # SrcAddress Length Name # Loads PCM-8 mono samples from the given memory location into __SFX and names the new sample-buffer */
	"PutSampleValue",           REXXID_PutSampleValue,      5,  /** # BufferId ChannelId Position Value # Stores one 16-bit sample value */
	"RedrawBuffer",             REXXID_RedrawBuffer,        2,  /** # BufferId # Refreshes the sample waveform graphics */
	"RefrProWin",               REXXID_RefrProWin,          3,  /** Aborted # ProWinId NewPosition # Sets the new progress status and check if the user has aborted */
	"RemoveBuffer",             REXXID_RemoveBuffer,        2,  /** # BufferId # Closes the specified sample-buffer */
	"RemoveProWin",             REXXID_RemoveProWin,        2,  /** # ProWinId # Closes the progress window */
	"RenameBuffer",             REXXID_RenameBuffer,        3,  /** # BufferId NewName # Gives the specified sample buffer a new name */
	"SaveSample",               REXXID_SaveSample,          2,  /** # FileName # Saves the currently selected sample under the specified filename with the currently selected saver */
	"SearchBuffer",             REXXID_SearchBuffer,        2,  /** BufferId # Name # Looks up a sample buffer by its name */
	"SelLoader",                REXXID_SelLoader,           2,  /** # LoaderName # Activates the loader with the supplied name */
	"SelOperator",              REXXID_SelOperator,         2,  /** # LoaderName # Activates the operator with the supplied name */
	"SelPlayer",                REXXID_SelPlayer,           2,  /** # LoaderName # Activates the player with the supplied name */
	"SelSaver",                 REXXID_SelSaver,            2,  /** # LoaderName # Activates the saver with the supplied name */
	"SetActiveBuffer",          REXXID_SetActiveBuffer,     2,  /** # BufferId # Makes the supplied sample-buffer the active one */
	"SetLength",                REXXID_SetLength,           3,  /** # BufferId NewLength # Changes the length of the specified buffer */
	"SetLoaderParam",			REXXID_SetLoaderParam,		4,  /** # LoaderName ParamName Value # Sets the value of the given parameter of the given loader */
	"SetLoaderPreset",			REXXID_SetLoaderPreset,		3,  /** # LoaderName PresetName # Selects a preset for the given loader */
	"SetLoopEnd",               REXXID_SetLoopEnd,          3,  /** # BufferId NewLoopEnd # Set the end position of the loop */
	"SetLoopLength",            REXXID_SetLoopLength,       3,  /** # BufferId NewLoopLength # Set the length of the loop */
	"SetLoopMode",              REXXID_SetLoopMode,         3,  /** # BufferId LoopMode[0=Off,1=Forward] # Set the respective loop mode for the specified buffer */
	"SetLoopStart",             REXXID_SetLoopStart,        3,  /** # BufferId NewLoopStart # Set the start position of the loop */
	"SetMarkXEnd",              REXXID_SetMarkXEnd,         3,  /** # BufferId NewMarkXEnd # Set the x-end position of the mark */
	"SetMarkXLength",           REXXID_SetMarkXLength,      3,  /** # BufferId NewMarkXLength # Set the x-length of the mark */
	"SetMarkXStart",            REXXID_SetMarkXStart,       3,  /** # BufferId NewMarkXStart # Set the x-start position of the mark */
	"SetMarkYEnd",              REXXID_SetMarkYEnd,         3,  /** # BufferId NewMarkYEnd # Set the y-end position of the mark */
	"SetMarkYLength",           REXXID_SetMarkYLength,      3,  /** # BufferId NewMarkYLength # Set the y-length of the mark */
	"SetMarkYStart",            REXXID_SetMarkYStart,       3,  /** # BufferId NewMarkYStart # Set the y-start position of the mark */
	"SetOperatorParam",         REXXID_SetOperatorParam,    4,  /** # OperatorName ParamName Value # Sets the value of the given parameter of the given operator */
	"SetOperatorPreset",        REXXID_SetOperatorPreset,   3,  /** # OperatorName PresetName # Selects a preset for the given operator */
	"SetQuietMode",             REXXID_SetQuietMode,        2,  /** OldMode # NewMode[0,1] # (De)activates the quite mode for arexx processing */
	"SetRate",                  REXXID_SetRate,             3,  /** # BufferId NewSampleRate # Changes the sampling rate of the specified buffer */
	"SetSaverParam",            REXXID_SetSaverParam,       4,  /** # SaverName ParamName Value # Sets the value of the given parameter of the given saver */
	"SetSaverPreset",           REXXID_SetSaverPreset,      3,  /** # SaverName PresetName # Selects a preset for the given saver */
	"SetZoomXEnd",              REXXID_SetZoomXEnd,         3,  /** # BufferId NewZoomXEnd # Set the x-end position of the zoom */
	"SetZoomXLength",           REXXID_SetZoomXLength,      3,  /** # BufferId NewZoomXLength # Set the x-length of the zoom */
	"SetZoomXStart",            REXXID_SetZoomXStart,       3,  /** # BufferId NewZoomXStart # Set the x-start position of the zoom */
	"SetZoomYEnd",              REXXID_SetZoomYEnd,         3,  /** # BufferId NewZoomYEnd # Set the y-end position of the zoom */
	"SetZoomYLength",           REXXID_SetZoomYLength,      3,  /** # BufferId NewZoomYLength # Set the y-length of the zoom */
	"SetZoomYStart",            REXXID_SetZoomYStart,       3,  /** # BufferId NewZoomYStart # Set the y-start position of the zoom */
	"ShowBuffer",               REXXID_ShowBuffer,          2,  /** # BufferId # Makes a hidden sample visible again */
	"ToBack",                   REXXID_ToBack,              1,  /** # # Sends __SFX screen to back */
	"ToFront",                  REXXID_ToFront,             1,  /** # # Brings __SFX screen to front */
	NULL
};

//-- definitions --------------------------------------------------------------

void DoIO_Rexx(void)
{
	register ULONG i;
	register UWORD j;
	struct RexxMsg *rmsg;
	char rxstr[MaxCmdLen],params[MaxParLen*MaxParAnz];
	char ret[4096],*retptr;                         // it has to hold a complete List
	UBYTE redraw,res;
	BYTE *addr;
	LONG id;
	WORD erg,numParams,val;
	UBYTE cha,found;
	ULONG len,pos;
	UWORD rat/*,type*/;
	UBYTE rnglock;
	BPTR lock;
	SInfo *si;
	//-- for prowin
	PWData *pwd;
	static char pwtitle[256];
	static ULONG pwcurlen;
	struct Node *node;
	
	RunTime.laktsamples=RunTime.aktsamples;
	while(rmsg=(struct RexxMsg *)GetMsg(arexxport)) {
		len=strlen(rmsg->rm_Args[0]);
		redraw=FALSE;
		if(len>0 && len<MaxCmdLen) {
			strcpy(rxstr,rmsg->rm_Args[0]);
			numParams=GetRexxStr(rxstr,params);
			rmsg->rm_Result1=rmsg->rm_Result2=RC_OK;
			if(rmsg->rm_Action&RXCOMM) {
				res=((rmsg->rm_Action&RXFF_RESULT)!=0);
				id=(LONG)GetCmdId(RexxPar0);
				ret[0]='\n';
				if(id>-1) {
					if(RexxCmds[id].numParams<=numParams) {
						// DEBUG
						//printf("REXX-call : ");
						//for(j=0;j<RexxCmds[id].numParams;j++) printf("%s ",&params[MaxParLen*j]);
						//printf("\n");
						// DEBUG
#ifdef CHECK_REG
						if(IsKeyStatusOkay) {
#endif
							//MSG1("Command : <%s>",RexxCmds[id].strCmdName);
							switch(id) {
								case REXXID_Exit:           RunTime.tryquit=1;break;
								case REXXID_Activate:       ScreenToFront(RunTime.sfxpubscr);break;
								case REXXID_ToFront:        ScreenToFront(RunTime.sfxpubscr);break;
								case REXXID_ToBack:         ScreenToBack(RunTime.sfxpubscr);break;
								case REXXID_Message:        Message(strNoID,RexxPar1,NULL,0);break;
								case REXXID_CleanUp:
									switch(atoi(RexxPar1)) {
										case 0: CleanUpCurr();break;
										case 1: CleanUpAll();break;
										case 2: CleanUpAllNorm();break;
										case 3: CleanUpAllZoom();break;
										default: RexxError(rmsg,REXXERR_InvalidArg);break;
									}
									break;

								case REXXID_GetList:
									retptr=ret;sprintf(ret,"-1");
									if(!stricmp(RexxPar1,"Buffers")) {
										foreach(node,&BufferList) {
											sprintf(retptr,"%3ld,%s\n",((SInfo *)node)->id,node->ln_Name);
											retptr+=(strlen(node->ln_Name)+5);
										}
									}
									if(!stricmp(RexxPar1,"Loaders")) {
										foreach(node,&LoaderList) {
											sprintf(retptr,"%s\n",node->ln_Name);
											retptr+=(strlen(node->ln_Name)+1);
										}
									}
									if(!stricmp(RexxPar1,"Operators")) {
										foreach(node,&EffektList) {
											sprintf(retptr,"%s\n",node->ln_Name);
											retptr+=(strlen(node->ln_Name)+1);
										}
									}
									if(!stricmp(RexxPar1,"Players")) {
										for(i=0;i<playeranz;i++) {
											sprintf(retptr,"%s\n",pllist[i].name);
											retptr+=(strlen(pllist[i].name)+1);
										}
									}
									if(!stricmp(RexxPar1,"Savers")) {
										foreach(node,&SaverList) {
											sprintf(retptr,"%s\n",node->ln_Name);
											retptr+=(strlen(node->ln_Name)+1);
										}
									}
									break;
								case REXXID_SelLoader:
									if((erg=GetIndexByName(&LoaderList,RexxPar1))>-1) {
										strcpy(ret,GetNameByIndex(&LoaderList,RunTime.aloader));
										RunTime.aloader=(UBYTE)erg;Refr_ToolbarLoader();
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_SelSaver:
									if((erg=GetIndexByName(&SaverList ,RexxPar1))>-1) {
										strcpy(ret,GetNameByIndex(&SaverList,RunTime.asaver));
										RunTime.asaver =(UBYTE)erg;Refr_ToolbarSaver();
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_SelPlayer:
									for(j=0;j<playeranz;j++) if(!stricmp(RexxPar1,pllist[j].name)) {
										//-- TODO: return old playname
										//strcpy(ret,pllist[RunTime.aplayer].name);
										RunTime.aplayer=pllist[j].nr;Refr_ToolbarPlayer();j=0xFFF0;
									}
									if(j==playeranz) RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_SelOperator:
									if((erg=GetIndexByName(&EffektList,RexxPar1))>-1) {
										strcpy(ret,GetNameByIndex(&EffektList,RunTime.aktfx));
										RunTime.aktfx=(UBYTE)erg;Refr_ToolbarOperator();
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;

								case REXXID_LoadSample:
									if(LoadSampleRexx(RexxPar1)) sprintf(ret,"%d",RunTime.aktbuf->id);
									else sprintf(ret,"-1");
									break;
								case REXXID_SaveSample:
									sprintf(ret,"%d",SaveSampleRexx(RexxPar1));
									break;
								case REXXID_ProcessSample:
									/** @todo:  how do we return multiple results e.g. for ChannelSplit, SampleSplit - separate by ',' ? */
									if(ProcessSampleRexx(params)) sprintf(ret,"%d",RunTime.aktbuf->id);
									else sprintf(ret,"-1");
									break;
								case REXXID_GetLoaderParam:
								case REXXID_SetLoaderParam:
								case REXXID_SetLoaderPreset:
									HandleLoaderCmd(params,ret);
									break;
								case REXXID_GetOperatorParam:
								case REXXID_SetOperatorParam:
								case REXXID_SetOperatorPreset:
									HandleOperatorCmd(params,ret);
									break;
								case REXXID_GetSaverParam:
								case REXXID_SetSaverParam:
								case REXXID_SetSaverPreset:
									HandleSaverCmd(params,ret);
									break;

								case REXXID_GetProgDir:
									if(lock=Lock("progdir:",ACCESS_READ)) { NameFromLock(lock,ret,255);ret[255]='\0';UnLock(lock); }
									else sprintf(ret,"-1");
									break;
								case REXXID_GetVersion:
									found=FALSE;
									if(!stricmp(RexxPar1,"SoundFX")) { found=TRUE;strcpy(ret,NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)); }
									if(!found) sprintf(ret,"-1");
									break;
								case REXXID_GetUserInfo:
									if(RunTime.status!=STAT_DEMO) sprintf(ret,"%s, %s",Key.line0,RunTime.username);                // keynumber
									else sprintf(ret,"DEMO");
									break;

								case REXXID_SetActiveBuffer:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										RunTime.aktbuf=si;Refr_ToolbarBuffer();
										if(SFX_SOUND_IS_SHOWN(si)) { WindowToFront(si->win);ActivateWindow(si->win); }
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetActiveBuffer:
									if(RunTime.aktbuf) sprintf(ret,"%d",RunTime.aktbuf->id);
									else sprintf(ret,"-1");
									break;
								case REXXID_SearchBuffer:
									if((si=(SInfo *)FindName(&BufferList,RexxPar1))) sprintf(ret,"%d",si->id);
									else sprintf(ret,"-1");
									break;
								case REXXID_RenameBuffer:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { SetName(si,RexxPar2);SetTitle(si);RefreshGads(WIN_BUF); }
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetBufferName:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%s",((struct Node *)si)->ln_Name);
									else {sprintf(ret,"");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_RemoveBuffer:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { RemSample(si);RefreshGads(WIN_BUF); }
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_RedrawBuffer:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) DrawSample(si,0);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_NewBuffer:
									len=atoi(RexxPar1);
									rat=atoi(RexxPar2);
									cha=atoi(RexxPar3)%5;
									if(si=NewSample()) {
										si->zoomxl=si->slen=len;si->srat=rat;
										si->channels=cha-1;si->aktch=cha;si->channelmask=(1L<<cha)-1;
										if(AllocSampleMem(si,len,cha)) {
											AddSample(si,NULL);RunTime.aktbuf=si;redraw=TRUE;
										}
										else { RemSample(si);si=NULL; }
									}
									if(si) sprintf(ret,"%d",si->id);
									else sprintf(ret,"-1");
									break;
								case REXXID_HideBuffer:
									si=RunTime.aktbuf;
									if(RunTime.aktbuf=GetBufferPtr(atoi(RexxPar1),-1)) {
										if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) HideSample();
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									RunTime.aktbuf=si;
									break;
								case REXXID_ShowBuffer:
									si=RunTime.aktbuf;
									if(RunTime.aktbuf=GetBufferPtr(atoi(RexxPar1),-1)) {
										if(SFX_SOUND_IS_HIDDEN(RunTime.aktbuf)) ShowSample();
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									RunTime.aktbuf=si;
									break;

								case REXXID_GetQuietMode:
									sprintf(ret,"%d",RunTime.quiet);
									break;
								case REXXID_SetQuietMode:
									sprintf(ret,"%d",RunTime.quiet);
									RunTime.quiet=atoi(RexxPar1);
									if(!RunTime.quiet) RefreshGads(REF_ALL);
									break;

								/** @todo currently cupports just one open prowin */
								case REXXID_InitProWin:
									strcpy(pwtitle,RexxPar2);
									if(pwd=NewProWin()) StartProWin(pwd,&pwcurlen,pwtitle,atoi(RexxPar1));  // den Statusbalken starten
									sprintf(ret,"%ld",pwd);
									break;
								case REXXID_RefrProWin:
									if(ProWinRunning(pwcurlen)) {
										sprintf(ret,"0");
										pwcurlen=atol(RexxPar2);
									}
									else sprintf(ret,"1");
									break;
								case REXXID_RemoveProWin:
									DelProWin((PWData *)atoi(RexxPar1));
									break;

								case REXXID_GetChannels:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->channels+1);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_IsChannelActive:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->channelmask&(1<<atoi(RexxPar2)));
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_EnableChannel:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										cha=atoi(RexxPar2);
										if(cha<(si->channels+1)) {
											si->channelmask|=(1<<cha);
											j=0;RunTime.aktbuf->aktch=0;
											if(RunTime.aktbuf->channelmask&1) { RunTime.aktbuf->chnr[j++]=0;RunTime.aktbuf->aktch++; }
											if(RunTime.aktbuf->channelmask&2) { RunTime.aktbuf->chnr[j++]=1;RunTime.aktbuf->aktch++; }
											if(RunTime.aktbuf->channelmask&4) { RunTime.aktbuf->chnr[j++]=2;RunTime.aktbuf->aktch++; }
											if(RunTime.aktbuf->channelmask&8) { RunTime.aktbuf->chnr[j  ]=3;RunTime.aktbuf->aktch++; }
											DrawSample(si,0);
										}
										else RexxError(rmsg,REXXERR_InvalidArg);
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_DisableChannel:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										cha=atoi(RexxPar2);
										if(cha<(si->channels+1)) {
											si->channelmask&=~(1<<cha);
											j=0;RunTime.aktbuf->aktch=0;
											if(RunTime.aktbuf->channelmask&1) { RunTime.aktbuf->chnr[j++]=0;RunTime.aktbuf->aktch++; }
											if(RunTime.aktbuf->channelmask&2) { RunTime.aktbuf->chnr[j++]=1;RunTime.aktbuf->aktch++; }
											if(RunTime.aktbuf->channelmask&4) { RunTime.aktbuf->chnr[j++]=2;RunTime.aktbuf->aktch++; }
											if(RunTime.aktbuf->channelmask&8) { RunTime.aktbuf->chnr[j  ]=3;RunTime.aktbuf->aktch++; }
											DrawSample(si,0);
										}
										else RexxError(rmsg,REXXERR_InvalidArg);
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->slen);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { SetSampleLength(si,atoi(RexxPar2));DrawSample(si,0);SetSlider(si,SLIDER_X); }
									else RexxError(rmsg,REXXERR_InvalidArg);
								case REXXID_GetRate:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->srat);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetRate:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { si->srat=atoi(RexxPar2);SetTitle(si); }
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetStorageType:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->storage);   // 1->mem,2->dev
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_GetLoopMode:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->loop);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetLoopMode:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { si->loop=atoi(RexxPar2)%2;DrawSample(si,0); }
									else RexxError(rmsg,REXXERR_InvalidArg);
								case REXXID_GetLoopStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->loops);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetLoopStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { NewRangeStart (atoi(RexxPar2),si->slen,&si->loops,&si->loopl);DrawSample(si,0);RefreshGads(WIN_RNG); }
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetLoopEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->loops+si->loopl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetLoopEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { NewRangeEnd   (atoi(RexxPar2),si->slen,&si->loops,&si->loopl);DrawSample(si,0);RefreshGads(WIN_RNG); }
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetLoopLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->loopl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetLoopLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { NewRangeLength(atoi(RexxPar2),si->slen,&si->loops,&si->loopl);DrawSample(si,0);RefreshGads(WIN_RNG); }
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetMarkXStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->markxs);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetMarkXStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										rnglock=RunTime.rnglock;RunTime.rnglock=TRUE;
										NewRangeStart (atoi(RexxPar2),si->slen,&si->markxs,&si->markxl);
										DrawSample(si,0);RefreshGads(WIN_RNG);
										RunTime.rnglock=rnglock;
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetMarkXEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->markxs+si->markxl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetMarkXEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										rnglock=RunTime.rnglock;RunTime.rnglock=TRUE;
										NewRangeEnd   (atoi(RexxPar2),si->slen,&si->markxs,&si->markxl);
										DrawSample(si,0);RefreshGads(WIN_RNG);
										RunTime.rnglock=rnglock;
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetMarkXLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->markxl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetMarkXLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										rnglock=RunTime.rnglock;RunTime.rnglock=TRUE;
										NewRangeLength(atoi(RexxPar2),si->slen,&si->markxs,&si->markxl);
										DrawSample(si,0);RefreshGads(WIN_RNG);
										RunTime.rnglock=rnglock;
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetMarkYStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->markys);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetMarkYStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { NewRangeStart (atoi(RexxPar2),SMP_MAX,&si->markys,&si->markyl);DrawSample(si,0);RefreshGads(WIN_RNG); }
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetMarkYEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->markys+si->markyl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetMarkYEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { NewRangeEnd   (atoi(RexxPar2),SMP_MAX,&si->markys,&si->markyl);DrawSample(si,0);RefreshGads(WIN_RNG); }
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_GetMarkYLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->markyl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetMarkYLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) { NewRangeLength(atoi(RexxPar2),SMP_MAX,&si->markys,&si->markyl);DrawSample(si,0);RefreshGads(WIN_RNG); }
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetZoomXStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->zoomxs);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetZoomXStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										rnglock=RunTime.rnglock;RunTime.rnglock=TRUE;
										NewRangeStart (atoi(RexxPar2),si->slen,&si->zoomxs,&si->zoomxl);
										DrawSample(si,0);SetSlider(si,SLIDER_X);RefreshGads(WIN_RNG);
										RunTime.rnglock=rnglock;
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetZoomXEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->zoomxs+si->zoomxl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetZoomXEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										rnglock=RunTime.rnglock;RunTime.rnglock=TRUE;
										NewRangeEnd   (atoi(RexxPar2),si->slen,&si->zoomxs,&si->zoomxl);
										DrawSample(si,0);SetSlider(si,SLIDER_X);RefreshGads(WIN_RNG);
										RunTime.rnglock=rnglock;
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetZoomXLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->zoomxl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetZoomXLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										rnglock=RunTime.rnglock;RunTime.rnglock=TRUE;
										NewRangeLength(atoi(RexxPar2),si->slen,&si->zoomxs,&si->zoomxl);
										DrawSample(si,0);SetSlider(si,SLIDER_X);RefreshGads(WIN_RNG);
										RunTime.rnglock=rnglock;
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetZoomYStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->zoomys);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetZoomYStart:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										NewRangeStart (atoi(RexxPar2),SMP_MAX,&si->zoomys,&si->zoomyl);
										DrawSample(si,0);SetSlider(si,SLIDER_Y);RefreshGads(WIN_RNG);
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_GetZoomYEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->zoomys+si->zoomyl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetZoomYEnd:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										NewRangeEnd   (atoi(RexxPar2),SMP_MAX,&si->zoomys,&si->zoomyl);
										DrawSample(si,0);SetSlider(si,SLIDER_Y);RefreshGads(WIN_RNG);
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_GetZoomYLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) sprintf(ret,"%d",si->zoomyl);
									else { sprintf(ret,"-1");RexxError(rmsg,REXXERR_InvalidArg); }
									break;
								case REXXID_SetZoomYLength:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										NewRangeLength(atoi(RexxPar2),SMP_MAX,&si->zoomys,&si->zoomyl);
										DrawSample(si,0);SetSlider(si,SLIDER_Y);RefreshGads(WIN_RNG);
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;

								case REXXID_EditCut:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Cut(si,EDIT_RANGE);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditCutB:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Cut(si,EDIT_2BEGIN);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditCutE:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Cut(si,EDIT_2END);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case  REXXID_EditCopy:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Copy(si,EDIT_RANGE);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditCopyB:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Copy(si,EDIT_2BEGIN);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditCopyE:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Copy(si,EDIT_2END);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditGrab:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Grab(si,EDIT_RANGE);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditGrabB:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Grab(si,EDIT_2BEGIN);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditGrabE:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Grab(si,EDIT_2END);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditPaste:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Paste(si,EDIT_RANGE);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditPasteB:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Paste(si,EDIT_2BEGIN);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditPasteE:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Paste(si,EDIT_2END);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditErase:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Erase(si,EDIT_RANGE);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditEraseB:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Erase(si,EDIT_2BEGIN);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditEraseE:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Erase(si,EDIT_2END);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditZero:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Zero(si,EDIT_RANGE);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditZeroB:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Zero(si,EDIT_2BEGIN);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditZeroE:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Zero(si,EDIT_2END);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_EditOverwrite:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Overwrite(si,EDIT_RANGE);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_EditOverwriteB:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) Edit_Overwrite(si,EDIT_2BEGIN);
									else RexxError(rmsg,REXXERR_InvalidArg);
									break; 
								case REXXID_PutSample:
									/** @todo allow to specify format and channels */
									addr=(BYTE *)atoi(RexxPar1);
									len =atoi(RexxPar2);
									//type=atoi(RexxPar4);
									if(si=NewSample()) {
										si->zoomxl=si->slen=len;
										if(AllocSampleMem(si,si->slen,1)) {
											switch(si->storage) {
												case VM_MEMORY:
													for(i=0;i<len;i++) si->buf[0][i]=((WORD)(*(addr++)))<<8;
													break;
											}
											AddSample(si,RexxPar3);RunTime.aktbuf=si;redraw=TRUE;
										}
										else { RemSample(si);si=NULL; }
									}
									if(si) sprintf(ret,"%d",si->id);
									else sprintf(ret,"-1");
									break;
								case REXXID_GetSample:
									/** @todo allow to specify format and channels */
									addr=(BYTE *)atoi(RexxPar1);
									//type=atoi(RexxPar2);
									switch(RunTime.aktbuf->storage) {
										case VM_MEMORY:
											for(i=0;i<RunTime.aktbuf->slen;i++) *(addr++)=(BYTE)(RunTime.aktbuf->buf[0][i]>>8);
											break;
									}
									break;
								case REXXID_GetSampleValue:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										cha=atoi(RexxPar2);
										pos=atoi(RexxPar3);
										switch(si->storage) {
											case VM_MEMORY:
												if(si->buf[cha] && pos<si->slen) sprintf(ret,"%d",si->buf[cha][pos]);
												else sprintf(ret,"0");
												break;
										}
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
									break;
								case REXXID_PutSampleValue:
									if(si=GetBufferPtr(atoi(RexxPar1),-1)) {
										cha=atoi(RexxPar2);
										pos=atoi(RexxPar3);
										val=atoi(RexxPar4);
										switch(si->storage) {
											case VM_MEMORY:
												if(si->buf[cha] && pos<si->slen) si->buf[cha][pos]=val;
												break;
										}                       
									}
									else RexxError(rmsg,REXXERR_InvalidArg);
							}
							if(res && ret[0]) rmsg->rm_Result2=(ULONG)CreateArgstring(ret,strlen(ret));
#ifdef CHECK_REG
						}
#endif
					}
					else { MSG("Rexx : wrong nr of args");RexxError(rmsg,REXXERR_WrongNrOfArgs); }
				}
				else { MSG1("Rexx : cmd not found : %s",RexxPar0);RexxError(rmsg,REXXERR_CmdNotFound); }
			}
		}
		else { MSG("Rexx : error in cmd string");RexxError(rmsg,REXXERR_ErrorInCmdString); }
		ReplyMsg(rmsg);
		if(redraw) {
			if(RunTime.aktsamples) {
				if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) {
					WindowToFront(RunTime.aktbuf->win);
					DrawSample(RunTime.aktbuf,0);
				}
				RefreshGads(WIN_OP|WIN_RXOP|WIN_SA|WIN_PL|WIN_ED|WIN_ZO);
			}
			else RefreshGads(REF_FLSAMPLE);
		}
	}
}

//-- helper

/******
** 
**
**
** NOTE
**   Wenn man Parameter mit Leerzeichen hat, dann diese wie folgt benutzen : '"125.00 ms"'
*/

WORD GetRexxStr(char *rstr,char *params)            /* nimmt Rexx-parameter auseinander, Trennzeichen = Space */
{
	ULONG pos1=0,pos2=0,sl=strlen(rstr);
	UBYTE parrdy,str;
	WORD numParams=0;

	while(pos1<sl && numParams<MaxParAnz) {             /* solange noch nicht durch und max params nicht erreicht */
		parrdy=0;str=0;
		while(!parrdy) {                                    /* Parameter suchen */
			if((!str && rstr[pos1]==' ') ||                      /* Trennzeichen außerhalb Hochkommas gefunden */
				!rstr[pos1] ||                                  /* Stringende */
				pos2>=((numParams+1)*MaxParLen-1)) parrdy=1;    /* Stringende */
			else {
				if(rstr[pos1]=='\"' || rstr[pos1]=='\'') { pos1++;str=1-str; }  /* " und ' */
				else params[pos2++]=rstr[pos1++];                           /* Zeichen kopieren */
			}
		}
		params[pos2]='\0';                                  /* Parameter terminieren */
		numParams++;pos1++;pos2=numParams*MaxParLen;
	}
	for(pos1=pos2;pos1<MaxParAnz*MaxParLen;pos1++) params[pos1]=0;
	return(numParams);
}

void GetSrcFromRexx(SInfo **src,char *params)
{
	register UBYTE i=0,j=0;

	while(params[i] && j<(MaxParAnz-1)) {
		src[j]=GetBufferPtr(atoi(&params[i]),-1);
		i+=MaxParLen;j++;
	}
}

enum RexxCmdId GetCmdId(const char * const strCmdName)
{
	BOOL boolFound=FALSE;
	LONG uix=REXXCmdCount-1,lix=0,ix;
	enum RexxCmdId idCmd=-1;
	int res;

//  MSG1("GetCmdIx beg %s",strCmdName);
	if(!strnicmp(strCmdName,"SFX_",4)) {
		while((!boolFound) && (uix>=lix)) {         // binary search
			ix=lix+((uix-lix)>>1);
//          MSG6("[%3d]=<%15s> [%3d]=<%15s> [%3d]=<%15s>",lix,RexxCmds[lix].strCmdName,ix,RexxCmds[ix].strCmdName,uix,RexxCmds[uix].strCmdName);
			res=stricmp(&strCmdName[4],RexxCmds[ix].strCmdName);
			if(!res) { boolFound=TRUE;idCmd=RexxCmds[ix].idCmd; }
			else if(res>0) lix=ix+1;
			else uix=ix-1;
		}
	}
//  MSG("GetCmdIx end");
	return(idCmd);
}

void __inline RexxError(struct RexxMsg *rmsg,ULONG errnum)
{
	rmsg->rm_Result1=RC_ERROR;rmsg->rm_Result2=errnum;
}


//-- eof ----------------------------------------------------------------------
