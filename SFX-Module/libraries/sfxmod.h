/****h* sfxmod/sfxmod.h [0.0] *
*
*  NAME
*    sfxmod.h
*  COPYRIGHT
*    $VER: sfxmod.h 0.0 (16.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    plugins for SoundFX
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    25.Aug.1998
*  MODIFICATION HISTORY
*    16.Apr.2004	V 0.0	most recent version
*    17.Apr.2002    V 4.2   most recent version
*    29.May.2001    V 1.2   most recent version
*    25.Aug.1998    V 1.2   initial version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXMOD_H
#define  LIBRARIES_SFXMOD_H

//-- include --------------------------------------------------------------------------------------

//-- Ansi C/C++

#include <ctype.h>
#include <dos.h>
#include <math.h>
#include <m68881.h>			// include math-extensions depending on type of math beeing used
#include <mffp.h>
#include <mieeedoub.h>
#define PIM2	6.28318530717958647692
#define MAXBYTE 255
#define MAXBYTEP 127
#define MAXBYTEN -128
#define MAXWORD 65535
#define MAXWORDP 32767
#define MAXWORDN -32768
#define MAXLONG 4294967295
#define MAXLONGP 2147483647
#define MAXLONGN -2147483648
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Amiga OS

#define __USE_SYSBASE
#include <exec/types.h>
#include <cmacros.h>
#include <devices/inputevent.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <dos/dostags.h>
#include <exec/exec.h>
#include <graphics/gfx.h>
#include <graphics/gfxmacros.h>
#include <graphics/display.h>
#include <graphics/displayinfo.h>
#include <intuition/gadgetclass.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/screens.h>
#include <ladspa.h>
#include <libraries/amigaguide.h>
#include <libraries/listtools.h>
#include <libraries/sfxsupport.h>
#include <libraries/unitconv.h>
#include <rexx/rxslib.h>
#include <utility/date.h>
#include <utility/hooks.h>
#include <utility/tagitem.h>

#include <clib/alib_protos.h>			/* Prototypes für Supportfunctions */

#include <proto/amigaguide.h>			/* Prototypes für Libraryfunctions */
#include <proto/asl.h>
#include <proto/configio.h>
#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/icon.h>
#include <proto/iffparse.h>
#include <proto/intuition.h>
#include <proto/listtools.h>
#include <proto/rexxsyslib.h>
#include <proto/sfxsupport.h>
#include <proto/unitconv.h>
#include <proto/utility.h>
#include <proto/wb.h>

#define __CXM33 __UCXM33				/* für Utillitie.library */
#define __CXD33 __UCXD33
#define __CXM22 __UCXM22
#define __CXD22 __UCXD22

/*---------------------------------------------------------------------------------------------------------------*/

#include <SFX-Images.h>
#include <SFX-LocaleId.h>
#include <SFX-FileDefs.h>

//-- ProcessData ----------------------------------------------------------------------------------

typedef struct {
	UBYTE   callmd;                 // which way a module has been invoked, see below
	SInfo   *src[10];               // source buffers
	SInfo   *dst[10];               // destination buffers
	STRPTR  fn;                     // filename (for Loaders/Savers)
} ProcessData;

//-- which way a module has been invoked

#define CM_GUI      0
#define CM_DIRECT   1
#define CM_STREAM   2
#define CM_PREHEAR  3

/*---------------------------------------------------------------------------------------------------------------*/

//-- duplicate is in "SFX-Main/SFX-Includes.h"

#define STAT_DEMO	0
#define STAT_AUTHOR	1
#define STAT_TESTER	2
#define STAT_USER	3

/*---------------------------------------------------------------------------------------------------------------*/

#define MENID_LOAD		 0
#define MENID_SAVE		 1
#define MENID_RESET		 2
#define MENID_DEFAULT	 3
#define MENID_GO		 4
#define MENID_HELP		 5
#define MENID_ABOUT		 6
#define MENID_QUIT		 7

/*---------------------------------------------------------------------------------------------------------------*/

//UBYTE *BandLabels[]={ "4","8","16","32","64","128","256","512",0l };
//UBYTE *StepLabels[]={ "1","2","4","8","16","32","64","128","256",0l };

/*---------------------------------------------------------------------------------------------------------------*/

//-- configurable plugin building blocks --------------------------------------

#define SFXMod_Prototypes                                                       \
/*-- plugin initialisation and destruction */                                   \
int ASM __UserLibInit    (REG(a6,struct Library *SFXModBase));                \
void ASM __UserLibCleanup(REG(a6,struct Library *SFXModBase));                \
																				\
/*-- instance initialisation and destruction */                                 \
void ASM *LIBSFXMod_ClientDataInit(REG(a0,RTime *RunTime_));                  \
void ASM  LIBSFXMod_ClientDataDone(REG(a0,void *instance));                   \
/*-- public routienes */                                                        \
UBYTE ASM LIBSFXMod_Interface     (REG(a0,void *instance));                   \
UBYTE ASM LIBSFXMod_Process       (REG(a0,void *instance),REG(a1,ProcessData *pdata));               \
void  ASM LIBSFXMod_HandleARexxCmd(REG(a0,void *instance),REG(a1,char *params),REG(a2,char *ret));   \
																				\
/*-- private routines */                                                        \
void LoadSettings		  (void *instance,char *fname);                         \
void SaveSettings		  (void *instance,char *fname);                         \
void SetDefaultSettings	  (void *instance);                                     \
void RecallLastSettings	  (void *instance);                                     \
void RecallDefaultSettings(void *instance);                                     \
void RefreshGUI           (void *instance,struct Gadget *modGadgets[],struct Window *modWin);
//------------------------------------------------------------------------------

#if defined(SFXMod_Operator) || defined(SFXMod_Saver) || defined(SFXMod_Loader)
	#define GADID_GO        0
	#define GADIX_GO        0
	#define GADID_OKAY		(GADID_GO+1)
	
	#define GADID_BASE		(GADID_OKAY)
	#define GADIX_BASE		(GADIX_GO)
#endif

#define SFXMod_Globals                                                          \
/*-- common globals */                                                          \
RTime 					*RunTime=NULL;                                          \
APTR 					cfg=NULL;                                               \
char					errmsg[256];                                            \
																				\
/*-- library base addresses */                                                  \
extern struct ExecBase	*SysBase;                                               \
struct Library	 		*AslBase=NULL;                                          \
struct Library	 		*ConfigIOBase=NULL;                                     \
struct DosLibrary		*DOSBase=NULL;                                          \
struct Library			*GadToolsBase=NULL;                                     \
struct GfxBase	 		*GfxBase=NULL;                                          \
struct IntuitionBase	*IntuitionBase=NULL;                                    \
struct Library			*SFXSupportBase=NULL;                                   \
struct Library			*UnitConvBase=NULL;

//------------------------------------------------------------------------------

#ifdef CONF_CASSABLANCA
#define SFXMod_OpenCommonLibs do {                                              \
	SysBase=(struct ExecBase *)*((ULONG *)4L);                                  \
	if(!(SFXSupportBase	=OpenLibrary(PROGDIR"libs/"SFXSupportName,SFXSupportVersion)))  { DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open "PROGDIR"libs/" SFXSupportName " >=V" NUM2STR(SFXSupportVersion) "\x00\x00",44);goto Error; } \
	if(GetSFXVersion()!=MAKE_VERSION_STAMP(PRJ_VERSION,PRJ_REVISION))					{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" PROGDIR"libs/"SFXSupportName" has not the requested version V"NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)"!" "\x00\x00",44);goto Error; } \
	if(!(DOSBase		=(struct DosLibrary *)OpenLibrary("dos.library",37L)))			{ ADosError("dos.library >=V37.0",errOpenLibrary,0L,__FILE__,__LINE__);goto Error; } \
	if(!(ConfigIOBase	=OpenLibrary(ConfigIOName,ConfigIOVersion)))					{ ADosError(ConfigIOName" >=V"NUM2STR(ConfigIOVersion),errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; } \
	if(!(UnitConvBase	=OpenLibrary(UnitConvName,UnitConvVersion)))					{ ADosError(UnitConvName" >=V"NUM2STR(UnitConvVersion),errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; } \
	init();\
} while(0)
#else
#ifdef SFXMod_Operator
#define SFXMod_OpenCommonLibs do {                                              \
	SysBase=(struct ExecBase *)*((ULONG *)4L);                                  \
	if(!(IntuitionBase	=(struct IntuitionBase *)OpenLibrary("intuition.library",37L))) goto Error; \
	if(!(SFXSupportBase	=OpenLibrary(PROGDIR"libs/"SFXSupportName,SFXSupportVersion)))  { DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open "PROGDIR"libs/" SFXSupportName " >=V" NUM2STR(SFXSupportVersion) "\x00\x00",44);goto Error; } \
	if(GetSFXVersion()!=MAKE_VERSION_STAMP(PRJ_VERSION,PRJ_REVISION))					{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" PROGDIR"libs/"SFXSupportName" has not the requested version V"NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)"!" "\x00\x00",44);goto Error; } \
	if(!(DOSBase		=(struct DosLibrary *)OpenLibrary("dos.library",37L)))			{ ADosError("dos.library >=V37.0",errOpenLibrary,0L,__FILE__,__LINE__);goto Error; } \
	if(!(ConfigIOBase	=OpenLibrary(ConfigIOName,ConfigIOVersion)))					{ ADosError(ConfigIOName" >=V"NUM2STR(ConfigIOVersion),errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; } \
	if(!(UnitConvBase	=OpenLibrary(UnitConvName,UnitConvVersion)))					{ ADosError(UnitConvName" >=V"NUM2STR(UnitConvVersion),errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; } \
	init();\
} while(0)
#else
#define SFXMod_OpenCommonLibs do {                                              \
	SysBase=(struct ExecBase *)*((ULONG *)4L);                                  \
	if(!(IntuitionBase	=(struct IntuitionBase *)OpenLibrary("intuition.library",37L))) goto Error; \
	if(!(SFXSupportBase	=OpenLibrary(PROGDIR"libs/"SFXSupportName,SFXSupportVersion)))  { DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open "PROGDIR"libs/" SFXSupportName " >=V" NUM2STR(SFXSupportVersion) "\x00\x00",44);goto Error; } \
	if(GetSFXVersion()!=MAKE_VERSION_STAMP(PRJ_VERSION,PRJ_REVISION))					{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" PROGDIR"libs/"SFXSupportName" has not the requested version V"NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)"!" "\x00\x00",44);goto Error; } \
	if(!(DOSBase		=(struct DosLibrary *)OpenLibrary("dos.library",37L)))			{ ADosError("dos.library >=V37.0",errOpenLibrary,0L,__FILE__,__LINE__);goto Error; } \
	if(!(ConfigIOBase	=OpenLibrary(ConfigIOName,ConfigIOVersion)))					{ ADosError(ConfigIOName" >=V"NUM2STR(ConfigIOVersion),errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; } \
	if(!(UnitConvBase	=OpenLibrary(UnitConvName,UnitConvVersion)))					{ ADosError(UnitConvName" >=V"NUM2STR(UnitConvVersion),errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; } \
} while(0)
#endif
//------------------------------------------------------------------------------

#define SFXMod_OpenGUILibs do {                                                 \
	if(!AslBase) 		if(!(AslBase		=OpenLibrary("asl.library",37L)))							{ ADosError("asl.library >=V37.0",errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; } \
	if(!GadToolsBase)	if(!(GadToolsBase	=OpenLibrary("gadtools.library",37L)))						{ ADosError("gadtools.library >=V37.0",errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; } \
	if(!GfxBase)		if(!(GfxBase		=(struct GfxBase *)OpenLibrary("graphics.library",37L)))	{ ADosError("graphics.library >=V37.0",errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; } \
} while(0)
//------------------------------------------------------------------------------

#ifdef SFXMod_Operator
#define SFXMod_CloseCommonAndGUILibs do {                                       \
	done();																		\
	if(UnitConvBase)	CloseLibrary(UnitConvBase);                             \
	if(DOSBase)			CloseLibrary((struct Library *)DOSBase);                \
	if(ConfigIOBase)	CloseLibrary(ConfigIOBase);                             \
	if(SFXSupportBase)	CloseLibrary(SFXSupportBase);							\
	if(IntuitionBase)	CloseLibrary((struct Library *)IntuitionBase);          \
	if(GfxBase)			CloseLibrary((struct Library *)GfxBase);                \
	if(GadToolsBase)	CloseLibrary(GadToolsBase);                             \
	if(AslBase)			CloseLibrary(AslBase);                                  \
} while(0)
#else
#define SFXMod_CloseCommonAndGUILibs do {                                       \
	if(UnitConvBase)	CloseLibrary(UnitConvBase);                             \
	if(DOSBase)			CloseLibrary((struct Library *)DOSBase);                \
	if(ConfigIOBase)	CloseLibrary(ConfigIOBase);                             \
	if(SFXSupportBase)	CloseLibrary(SFXSupportBase);							\
	if(IntuitionBase)	CloseLibrary((struct Library *)IntuitionBase);          \
	if(GfxBase)			CloseLibrary((struct Library *)GfxBase);                \
	if(GadToolsBase)	CloseLibrary(GadToolsBase);                             \
	if(AslBase)			CloseLibrary(AslBase);                                  \
} while(0)
#endif
//------------------------------------------------------------------------------

#ifdef SFXMod_Operator
#ifdef SFXMod_HasModDesc
#define SFXMod_ClientDataInitCommon do {                                        \
	instantiate((SFXCoreInstance *)instance);									\
	RunTime=RunTime_;                                                           \
	UnLock(CreateDir(PROGDIR"_"SFXMod_Type"/"PRJ_NAME));                        \
	RecallLastSettings(instance);		/* set default or last-used values */   \
	InitFXParams(instance->moddesc);	/* lock buffers */                      \
} while(0)
#else
#define SFXMod_ClientDataInitCommon do {                                        \
	instantiate((SFXCoreInstance *)instance);									\
	RunTime=RunTime_;                                                           \
	UnLock(CreateDir(PROGDIR"_"SFXMod_Type"/"PRJ_NAME));                        \
	RecallLastSettings(instance);		/* set default or last-used values */   \
} while(0)
#endif
#else
#ifdef SFXMod_HasModDesc
#define SFXMod_ClientDataInitCommon do {                                        \
	RunTime=RunTime_;                                                           \
	UnLock(CreateDir(PROGDIR"_"SFXMod_Type"/"PRJ_NAME));                        \
	RecallLastSettings(instance);		/* set default or last-used values */   \
	InitFXParams(instance->moddesc);	/* lock buffers */                      \
} while(0)
#else
#define SFXMod_ClientDataInitCommon do {                                        \
	RunTime=RunTime_;                                                           \
	UnLock(CreateDir(PROGDIR"_"SFXMod_Type"/"PRJ_NAME));                        \
	RecallLastSettings(instance);		/* set default or last-used values */   \
} while(0)
#endif
#endif
//------------------------------------------------------------------------------

#define SFXMod_DoClientDataInit                                                 \
void __saveds __asm *LIBSFXMod_ClientDataInit(REG(a0,RTime *RunTime_)) {		\
	struct Instance *instance;                                                  \
	if(instance=(struct Instance *)AllocVec(sizeof(struct Instance),MEMF_ANY|MEMF_CLEAR)) { \
		SFXMod_ClientDataInitCommon;											\
	}																			\
	return((void *)instance);                                                   \
}

//------------------------------------------------------------------------------

#ifdef SFXMod_HasModDesc
#define SFXMod_ClientDataDoneCommon do {                                        \
	DoneFXParams(instance->moddesc);	/* unlock buffers */                    \
	FreeFXParams(instance->moddesc);											\
} while(0)
#else
#define SFXMod_ClientDataDoneCommon
#endif

//------------------------------------------------------------------------------

#ifdef SFXMod_Operator
#define SFXMod_DoClientDataDone                                                 \
void __saveds __asm LIBSFXMod_ClientDataDone(REG(a0,struct Instance *instance)) { \
	if(instance) {																\
		SFXMod_ClientDataDoneCommon;											\
		destroy((SFXCoreInstance *)instance);									\
		FreeVec((void *)instance);												\
	}																			\
}
#else
#define SFXMod_DoClientDataDone                                                 \
void __saveds __asm LIBSFXMod_ClientDataDone(REG(a0,struct Instance *instance)) { \
	if(instance) {																\
		SFXMod_ClientDataDoneCommon;											\
		FreeVec((void *)instance);												\
	}																			\
}
#endif
//------------------------------------------------------------------------------

#define SFXMod_ModDescInit(ix,_type,_prefix,_data) do {                         \
	instance->moddesc[ix].type=_type;                                           \
	instance->moddesc[ix].prefix=_prefix;                                       \
	instance->moddesc[ix].data=_data;                                           \
} while(0)

//------------------------------------------------------------------------------

#define SFXMod_Interface_GUIVariables		                                    \
	struct NewGadget ng;                                                        \
	struct Window *modWin;                                                      \
	struct Gadget *g;                                                           \
	struct Gadget *modGList=NULL;                                               \
	struct Gadget *modGadgets[LAST_GADIX];                                      \
	struct FileRequester *modFRequest;                                          \
	struct MenuItem *item;                                                      \
	struct RastPort *modrp;                                                     \
	struct IntuiMessage *imsg;                                                  \
	ULONG iclass,isec,imic;                                                     \
	APTR iadr;                                                                  \
	USHORT icode;                                                               \
	ULONG gnr;                                                                  \
	UBYTE ret=FALSE,runing=TRUE;                                                \
	char fn[FILENAME_MAX],cn[FNSIZE];                                           \
																				\
	struct TagItem wintags[]={                                                  \
	/* 00 */WA_Left,			0,                                              \
	/* 01 */WA_Top,				0,                                              \
	/* 02 */WA_InnerWidth,		0,                                              \
	/* 03 */WA_InnerHeight,		0,                                              \
	/* 04 */WA_IDCMP,			BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MENUPICK, \
	/* 05 */WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS, \
	/* 06 */WA_Gadgets,			NULL,                                           \
	/* 07 */WA_Title,			(ULONG)PRJ_NAME,                                \
	/* 08 */WA_ScreenTitle,		(ULONG)PRJ_VERS,                                \
	/* 09 */WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,                          \
	/* 10 */WA_BlockPen,		1,                                              \
	/* 11 */TAG_DONE                                                            \
	};                                                                          \
	struct TagItem MODFReqtags[]={                                              \
	/* 00 */ASLFR_TitleText,		NULL,                                       \
	/* 01 */ASLFR_PubScreenName,	(ULONG)SFX_PUBSCRNAME,                      \
	/* 02 */ASLFR_InitialLeftEdge,	180,                                        \
	/* 03 */ASLFR_InitialTopEdge,	15,                                         \
	/* 04 */ASLFR_InitialWidth,		300,                                        \
	/* 05 */ASLFR_InitialHeight,	184,                                        \
	/* 06 */ASLFR_InitialDrawer,	(ULONG)PROGDIR"_"SFXMod_Type"/"PRJ_NAME,    \
	/* 07 */ASLFR_InitialFile,		(ULONG)"default.cfg",                       \
	/* 08 */ASLFR_RejectIcons,		TRUE,                                       \
	/* 09 */ASLFR_DoSaveMode,		FALSE,                                      \
	/* 10 */ASLFR_DoMultiSelect,    FALSE,                                      \
	/* 11 */ASLFR_DrawersOnly,      FALSE,                                      \
	/* 12 */TAG_DONE                                                            \
	};

#define MODFReqtags_TitleText			   MODFReqtags[ 0].ti_Data
#define MODFReqtags_InitialLeftEdge		   MODFReqtags[ 2].ti_Data
#define MODFReqtags_InitialHeight		   MODFReqtags[ 5].ti_Data
#define MODFReqtags_InitialDrawer		   MODFReqtags[ 6].ti_Data
#define MODFReqtags_InitialFile			   MODFReqtags[ 7].ti_Data
#define MODFReqtags_RejectIcons			   MODFReqtags[ 8].ti_Data
#define MODFReqtags_DoSaveMode			   MODFReqtags[ 9].ti_Data
#define MODFReqtags_DoMultiSelect		   MODFReqtags[10].ti_Data
#define MODFReqtags_DrawersOnly			   MODFReqtags[11].ti_Data

//------------------------------------------------------------------------------

#ifdef SFXMod_HasCfgSel
#define SFXMod_Interface_CfgSelVariables                                        \
	struct Node *node;                                                          \
	CfgSel cfgsel;                                                              \
	UBYTE nr;
#else
#define SFXMod_Interface_CfgSelVariables
#endif

//------------------------------------------------------------------------------

#ifdef SFXMod_HasSource
#define SFXMod_Interface_NotifyVariables                                        \
	NotifyTask nt={0l};                                                         \
	LONG sigin,sigout;                                                          \
	UBYTE enabled;
#else
#define SFXMod_Interface_NotifyVariables										\
	UBYTE enabled=TRUE;
#endif

//------------------------------------------------------------------------------

#ifdef SFXMod_HasSource
#define SFXMod_Interface_NotifyInit                                             \
	enabled=(RunTime->aktsamples>0);                                            \
	nt.task=FindTask(NULL);                                                     \
	if((nt.sig=AllocSignal(-1))!=-1) {                                          \
		AddTail(&RunTime->gblist,&nt);
#else
#define SFXMod_Interface_NotifyInit
#endif

//------------------------------------------------------------------------------

#ifdef SFXMod_HasSource
#define SFXMod_Interface_NotifyDone                                             \
		if(((struct Node *)&nt)==RunTime->gblist.lh_Head) RemHead(&RunTime->gblist); \
		else if(((struct Node *)&nt)==RunTime->gblist.lh_Tail) RemTail(&RunTime->gblist); \
		else Remove((struct Node *)&nt);                                        \
		FreeSignal(nt.sig);                                                     \
	}                                                                           \
	else Message(errAllocateSignal,NULL,__FILE__,__LINE__);
#else
#define SFXMod_Interface_NotifyDone
#endif
//------------------------------------------------------------------------------

#define SFXMod_Interface_FReqInit                                               \
			MODFReqtags_TitleText      =(ULONG)GetString(strSelectPreset);      \
			MODFReqtags_InitialLeftEdge=(RunTime->xres-300)>>1;                 \
			MODFReqtags_InitialHeight  =RunTime->yres-30;                       \
			if(modFRequest=AllocAslRequest(ASL_FileRequest,MODFReqtags)) {
//------------------------------------------------------------------------------

#define SFXMod_Interface_FReqDone                                               \
				FreeAslRequest(modFRequest);                                    \
			}                                                                   \
			else Message(errAllocateFileRequest,0l,__FILE__,__LINE__);
//------------------------------------------------------------------------------

#define SFXMod_Interface_WinInit                                                \
			wintags[0].ti_Data=(RunTime->xres-winw)>>1;                         \
			wintags[1].ti_Data=(RunTime->yres-winh)>>1;                         \
			wintags[2].ti_Data=winw;                                            \
			wintags[3].ti_Data=winh;                                            \
			wintags[6].ti_Data=(ULONG)modGList;                                 \
			if(modWin=OpenWindowTagList(0l,wintags)) {                          \
				GT_RefreshWindow(modWin,0l);                                    \
				SetMenuStrip(modWin,RunTime->FXMenu);                           \
				modrp=modWin->RPort;SetFont(modrp,RunTime->scrfont);
//------------------------------------------------------------------------------

#define SFXMod_Interface_WinDone                                                \
				ClearMenuStrip(modWin);                                         \
				CloseWindow(modWin);                                            \
			}                                                                   \
			else Message(errOpenWindow,0l,__FILE__,__LINE__);
//------------------------------------------------------------------------------

#define SFXMod_Interface_AnalyseIMsg                                            \
						iclass	=imsg->Class;                                   \
						iadr	=imsg->IAddress;                                \
						icode	=imsg->Code;                                    \
						isec	=imsg->Seconds;                                 \
						imic	=imsg->Micros;                                  \
						GT_ReplyIMsg(imsg);
//------------------------------------------------------------------------------

#ifdef SFXMod_HasCfgSel
#define SFXMod_Interface_GadgetUpEvents                                         \
	case GADID_GO:                                                              \
		SaveSettings(instance,ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg");			\
		ret=TRUE;runing=FALSE;                                                  \
		break;                                                                  \
	case GADID_OKAY:                                                            \
		SaveSettings(instance,ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg");			\
		runing=FALSE;                                                           \
		break;                                                                  \
	case GADID_CFGLIST:                                                         \
	case GADID_CFGNAME:                                                         \
	case GADID_CFGDEL:                                                          \
		switch(cfgsel.action) {                                                 \
			case CFGSEL_ACTION_LOAD:                                            \
				/* free old setting */											\
				DoneFXParams(instance->moddesc);FreeFXParams(instance->moddesc);\
				if(!strnicmp(cfgsel.aktcfg->ln_Name," default",8))				\
					RecallDefaultSettings(instance);							\
				else {                                                          \
					if(!strnicmp(cfgsel.aktcfg->ln_Name," current",8))			\
						RecallLastSettings(instance);							\
					else {														\
						sprintf(fn,"%s/%s.cfg",cfgsel.path,cfgsel.aktcfg->ln_Name); \
						LoadSettings(instance,fn);								\
					}															\
				}                                                               \
				InitFXParams(instance->moddesc);	/* init new settings */		\
				RefreshGUI(instance,modGadgets,modWin);                         \
				break;                                                          \
			case CFGSEL_ACTION_GO:                                              \
				if(enabled) {													\
					SaveSettings(instance,ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg");\
					ret=TRUE;runing=FALSE;										\
				}																\
				break;                                                          \
		}                                                                       \
		break;                                                                  \
	case GADID_CFGADD:                                                          \
		nr=0;sprintf(cn,"New%02d.cfg",nr);                                      \
		while((FindName(&(cfgsel.cfgs),cn)) && (nr<100)) {						\
			nr++;sprintf(cn,"New%02d.cfg",nr);									\
		}																		\
		if(nr<100) {                                                            \
			strmfp(fn,cfgsel.path,cn);SaveSettings(instance,fn);                \
			strsfn(fn,NULL,NULL,cn,NULL);                                       \
			if(node=AddCfgSelConfig(&cfgsel,cn)) {                              \
				cfgsel.aktcfg=node;												\
				SetCfgSel(&cfgsel,modWin,modGadgets,GADIX_CFGSEL);				\
				ActivateGadget(modGadgets[GADIX_CFGNAME],modWin,0l);            \
			}                                                                   \
			else Message(errAllocateMemory,0l,__FILE__,__LINE__);               \
		}                                                                       \
		break;
#else
#define SFXMod_Interface_GadgetUpEvents                                         \
	case GADID_GO:                                                              \
		SaveSettings(instance,ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg");			\
		ret=TRUE;runing=FALSE;                                                  \
		break;                                                                  \
	case GADID_OKAY:                                                            \
		SaveSettings(instance,ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg");			\
		runing=FALSE;                                                           \
		break;
#endif
//------------------------------------------------------------------------------


#ifdef SFXMod_HasSource
#define SFXMod_Interface_VKeyEvents                                             \
	case KEY_ENTER:                                                             \
	case KEY_RETURN:                                                            \
		if(enabled) {															\
			SaveSettings(instance,ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg");		\
			ret=TRUE;runing=FALSE;                                              \
		}                                                                       \
		break;                                                                  \
	case KEY_ESC:                                                               \
		runing=FALSE;                                                           \
		break;                                                                  \
	case '?':                                                                   \
		Message(strNoID,PRJ_VERS,0l,0l);                                        \
		break;
#else
#define SFXMod_Interface_VKeyEvents                                             \
	case KEY_ENTER:                                                             \
	case KEY_RETURN:                                                            \
		SaveSettings(instance,ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg");			\
		ret=TRUE;runing=FALSE;                                                  \
		break;                                                                  \
	case KEY_ESC:                                                               \
		runing=FALSE;                                                           \
		break;                                                                  \
	case '?':                                                                   \
		Message(strNoID,PRJ_VERS,0l,0l);                                        \
		break;
#endif
//------------------------------------------------------------------------------

#ifdef SFXMod_HasCfgSel
#define SFXMod_Interface_Events                                                 \
	case IDCMP_CLOSEWINDOW:                                                     \
		runing=FALSE;                                                           \
		break;                                                                  \
	case IDCMP_RAWKEY:                                                          \
		if(icode==RKEY_HELP) ShowHelp("html/nodes/"SFXMod_HelpBase"/"PRJ_NAME".html"); \
		break;                                                                  \
	case IDCMP_MENUPICK:                                                        \
		while(icode!=MENUNULL) {                                                \
			if(item=ItemAddress(RunTime->FXMenu,icode)) {                       \
				switch((ULONG)MENU_USERDATA(item)) {                            \
					case MENID_LOAD:                                            \
						MODFReqtags_DoSaveMode=FALSE;                           \
						MODFReqtags_DoMultiSelect=FALSE;						\
						if(AslRequest(modFRequest,MODFReqtags)) {               \
							strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File); \
							DoneFXParams(instance->moddesc);FreeFXParams(instance->moddesc); /* free old settings */ \
							LoadSettings(instance,fn);InitFXParams(instance->moddesc); /* init new settings */ \
							RefreshGUI(instance,modGadgets,modWin);             \
						}                                                       \
						break;                                                  \
					case MENID_SAVE:                                            \
						MODFReqtags_DoSaveMode=TRUE;                            \
						MODFReqtags_DoMultiSelect=FALSE;						\
						if(AslRequest(modFRequest,MODFReqtags)) {               \
							strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File); \
							SaveSettings(instance,fn);strsfn(fn,NULL,NULL,cn,NULL); \
							if(!strcmp(modFRequest->fr_Drawer,cfgsel.path)) {   \
								if(!FindName(&(cfgsel.cfgs),cn)) {              \
									if(node=AddCfgSelConfig(&cfgsel,cn)) {      \
										cfgsel.aktcfg=node;SetCfgSel(&cfgsel,modWin,modGadgets,GADIX_CFGSEL); \
									}                                           \
									else Message(errAllocateMemory,0l,__FILE__,__LINE__); \
								}                                               \
							}                                                   \
						}                                                       \
						break;                                                  \
					case MENID_RESET:                                           \
						DoneFXParams(instance->moddesc);FreeFXParams(instance->moddesc); /* free old settings */ \
						RecallLastSettings(instance);InitFXParams(instance->moddesc);	/* init new settings */ \
						RefreshGUI(instance,modGadgets,modWin);                 \
						break;                                                  \
					case MENID_DEFAULT:                                         \
						DoneFXParams(instance->moddesc);FreeFXParams(instance->moddesc); /* free old settings */ \
						RecallDefaultSettings(instance);InitFXParams(instance->moddesc);/* init new settings */ \
						RefreshGUI(instance,modGadgets,modWin);                 \
						break;                                                  \
					case MENID_GO:                                              \
						SaveSettings(instance,ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg"); /* store current settings */ \
						ret=TRUE;runing=FALSE;                                  \
						break;                                                  \
					case MENID_HELP:                                            \
						ShowHelp("html/nodes/"SFXMod_HelpBase"/"PRJ_NAME".html"); \
						break;                                                  \
					case MENID_ABOUT:                                           \
						Message(strNoID,PRJ_VERS,NULL,0l);                      \
						break;                                                  \
					case MENID_QUIT:                                            \
						runing=FALSE;                                           \
						break;                                                  \
				}                                                               \
				icode=item->NextSelect;                                         \
			}                                                                   \
		}                                                                       \
		break;
#else
#define SFXMod_Interface_Events              			                        \
	case IDCMP_CLOSEWINDOW:                                                     \
		runing=FALSE;                                                           \
		break;                                                                  \
	case IDCMP_RAWKEY:                                                          \
		if(icode==RKEY_HELP) ShowHelp("html/nodes/"SFXMod_HelpBase"/"PRJ_NAME".html"); \
		break;                                                                  \
	case IDCMP_MENUPICK:                                                        \
		while(icode!=MENUNULL) {                                                \
			if(item=ItemAddress(RunTime->FXMenu,icode)) {                       \
				switch((ULONG)MENU_USERDATA(item)) {                            \
					case MENID_LOAD:                                            \
						MODFReqtags[9].ti_Data=FALSE;                           \
						if(AslRequest(modFRequest,MODFReqtags)) {               \
							strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File); \
							DoneFXParams(instance->moddesc);FreeFXParams(instance->moddesc); /* free old settings */ \
							LoadSettings(instance,fn);InitFXParams(instance->moddesc); /* init new settings */ \
							RefreshGUI(instance,modGadgets,modWin);             \
						}                                                       \
						break;                                                  \
					case MENID_SAVE:                                            \
						MODFReqtags[9].ti_Data=TRUE;                            \
						if(AslRequest(modFRequest,MODFReqtags)) {               \
							strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File); \
							SaveSettings(instance,fn);strsfn(fn,NULL,NULL,cn,NULL); \
						}                                                       \
						break;                                                  \
					case MENID_RESET:                                           \
						DoneFXParams(instance->moddesc);FreeFXParams(instance->moddesc); /* free old settings */ \
						RecallLastSettings(instance);InitFXParams(instance->moddesc);	/* init new settings */ \
						RefreshGUI(instance,modGadgets,modWin);                 \
						break;                                                  \
					case MENID_DEFAULT:                                         \
						DoneFXParams(instance->moddesc);FreeFXParams(instance->moddesc); /* free old settings */ \
						RecallDefaultSettings(instance);InitFXParams(instance->moddesc);/* init new settings */ \
						RefreshGUI(instance,modGadgets,modWin);                 \
						break;                                                  \
					case MENID_GO:                                              \
						SaveSettings(instance,ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg"); /* store currentsettings */ \
						ret=TRUE;runing=FALSE;                                  \
						break;                                                  \
					case MENID_HELP:                                            \
						ShowHelp("html/nodes/"SFXMod_HelpBase"/"PRJ_NAME".html"); \
						break;                                                  \
					case MENID_ABOUT:                                           \
						Message(strNoID,PRJ_VERS,NULL,0l);                      \
						break;                                                  \
					case MENID_QUIT:                                            \
						runing=FALSE;                                           \
						break;                                                  \
				}                                                               \
				icode=item->NextSelect;                                         \
			}                                                                   \
		}                                                                       \
		break;
#endif
//------------------------------------------------------------------------------

#define SFXMod_HandleARexxCmd_SetOperatorPreset                                 \
	if(!cf &&  !stricmp(RexxPar0,"SFX_SetOperatorPreset")) {                    \
		if(!stricmp(RexxPar2,"default")) {                                      \
			DoneFXParams(instance->moddesc);FreeFXParams(instance->moddesc);	\
			RecallDefaultSettings(instance);InitFXParams(instance->moddesc); }	\
		else {                                                                  \
			char fn[FILENAME_MAX];                                              \
			sprintf(fn,PROGDIR"_operators/"PRJ_NAME"/%s.cfg",RexxPar2);         \
			DoneFXParams(instance->moddesc);FreeFXParams(instance->moddesc);	\
			LoadSettings(instance,fn);InitFXParams(instance->moddesc);			\
		}                                                                       \
		SaveSettings(instance,ENVDIR"operators/"PRJ_NAME".cfg");                \
	}
//------------------------------------------------------------------------------

#define SFXMod_RecallLastSettings                                                                                                \
void RecallLastSettings(struct Instance *instance) {                                                                             \
	if		(!access( ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg",F_OK|R_OK))		    LoadSettings(instance, ENVDIR""SFXMod_Type"/"PRJ_NAME".cfg");           \
	else if	(!access(PROGDIR"_"SFXMod_Type"/"PRJ_NAME"/default.cfg",F_OK|R_OK)) LoadSettings(instance,PROGDIR"_"SFXMod_Type"/"PRJ_NAME"/default.cfg");  \
	else																 		SetDefaultSettings(instance);                    \
}
//-------------------------------------------------------------------------------------------------------------------------------

#define SFXMod_RecallDefaultSettings                                                                                             \
void RecallDefaultSettings(struct Instance *instance) {                                                                          \
	if		(!access(PROGDIR"_"SFXMod_Type"/"PRJ_NAME"/default.cfg",F_OK|R_OK)) LoadSettings(instance,PROGDIR"_"SFXMod_Type"/"PRJ_NAME"/default.cfg");  \
	else																 		SetDefaultSettings(instance);                    \
}
//-------------------------------------------------------------------------------------------------------------------------------


/*---------------------------------------------------------------------------------------------------------------*/

#endif	 /* LIBRARIES_SFXMOD_H */
