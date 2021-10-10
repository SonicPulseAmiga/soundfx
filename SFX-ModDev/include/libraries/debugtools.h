#ifndef  LIBRARIES_DEBUGTOOLS_H
#define  LIBRARIES_DEBUGTOOLS_H

/*
**	$VER: debugtools.h 1.0 (25.02.98)
**
**	(C) Copyright 1998-1998 Stefan Kost
**	    All Rights Reserved
*/

//-- include --------------------------------------------------------------------------------------

//-- Ansi C/C++

#include <dos.h>
#include <error.h>
#include <fcntl.h>
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

//-- Amiga OS

#define __USE_SYSBASE
#include <exec/types.h>
#include <cmacros.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <dos/dostags.h>
#include <exec/exec.h>
#include <intuition/intuition.h>
#include <rexx/rxslib.h>

#include <clib/alib_protos.h>			/* Prototypes für Supportfunctions */
int kprintf(char *fmt,...);

#include <proto/dos.h>					/* Prototypes für Libraryfunctions */
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/rexxsyslib.h>

//-- defines --------------------------------------------------------------------------------------

#define REG(x) register __## x 

//-- Attributes  ------------------------------------------------------------------------------------
//
// Attribute			// S=Set G=Get
//   Value1
//   Value2

#define TAGBASE_DT			TAG_USER

#define DTA_Report			(TAGBASE_DT+0)			// SG  for Report generation
	#define DTV_Report_Off			 0
	#define DTV_Report_Calls		 1
	#define DTV_Report_Fails		 2

#define DTA_OutputDev		(TAGBASE_DT+1)			// SG  which Output device(s)
	#define DTV_OutputDev_Off		 0
	#define DTV_OutputDev_Ser		 1
	#define DTV_OutputDev_Par		 2
	#define DTV_OutputDev_Con		 4
	#define DTV_OutputDev_File		 8
	#define DTV_OutputDev_Scmsg		16

#define DTA_OutputMode		(TAGBASE_DT+2)			// SG  what Output modes
	#define DTV_OutputMode_Msg		 0					// bit 0
	#define DTV_OutputMode_Err		 1
	#define DTV_OutputMode_Stamped	 0					// bit 1
	#define DTV_OutputMode_Plain	 2

#define DTA_OutputFmt		(TAGBASE_DT+2)			// SG  what Output fmt
	#define DTV_OutputFmt_File		 1
	#define DTV_OutputFmt_Line		 2
	#define DTV_OutputFmt_Proc		 4
	#define DTV_OutputFmt_Date		 8
	#define DTV_OutputFmt_Time		16

#define DTA_FailReq			(TAGBASE_DT+3)			// SG  for Fail-Requesters
	#define DTV_FailReq_Off			 0
	#define DTV_FailReq_On			 1

#define DTA_AutoBreaks		(TAGBASE_DT+4)			// SG  for Auto-BreakPoints
	#define DTV_AutoBreaks_Off		0
	#define DTV_AutoBreaks_Enter	1
	#define DTV_AutoBreaks_Leave	2
	#define DTV_AutoBreaks_Both		3

//-- for IsValidPointer ---------------------------------------------------------------------------

#define DT_NullPointerContents		(0xC0DEDBAD)
#define DT_FreeMemory0				(0xABADCAFE)
#define DT_FreeMemory1				(0xADCAFEAB)
#define DT_FreeMemory2				(0xCAFEABAD)
#define DT_FreeMemory3				(0xFEABADCA)
#define DT_UnclearedMemory			(0xDEADF00D)
#define DT_FreedMemory0				(0xDEADBEEF)
#define DT_FreedMemory1				(0xADBEEFDE)
#define DT_FreedMemory2				(0xBEEFDEAD)
#define DT_FreedMemory3				(0xEFDEADBE)
#define DT_FreeStack0				(0xADEADBED)
#define DT_FreeStack1				(0xEADBEDAD)
#define DT_FreeStack2				(0xDBEDADEA)
#define DT_FreeStack3				(0xEDADEADB)

#define DT_InvalidPointer			((APTR)0xFACED00F)
#define DT_InvalidD1				((APTR)0xD100DEAD)
#define DT_InvalidA0				((APTR)0xA000DEAD)
#define DT_InvalidA1				((APTR)0xA100DEAD)

// WORDS : O=0 I=1 Z=2 S=5 G=6
//
// A, ACE, AFFE
// BAD, BEEF, BED
// CAFE, C0FFE, CODED
// DAD, DEAD, DEAF, D00F
// 
// FAB, FACE, FADE, FEED, F00D

//-- for Resourcetracking -------------------------------------------------------------------------

struct __dtResEntry
{
	struct Node	node;
	APTR		resptr,resptr2;
	ULONG		ressize;
	ULONG		resid;
	ULONG		line;
	STRPTR		file;
};

//-- definitions for MemoryWall feature

#define SOS_WallSize 4
#define SOS_WallFill 0xAF

//-- exec.library

#define RES_EXEC			0
#define RES_EXEC_CT			23

#define RES_INTERRUPTS		(RES_EXEC+ 0)		// Disable/Enable
#define RES_MULTITASKING	(RES_EXEC+ 1)		// Forbid/Permit
#define RES_INTSERVER		(RES_EXEC+ 2)		// AddIntServer/RemIntServer
#define RES_MEMORY			(RES_EXEC+ 3)		// Allocate/Deallocate
#define RES_MEM				(RES_EXEC+ 4)		// AlocAbs,AllocMem/FreeMem
#define RES_VEC				(RES_EXEC+ 5)		// AllocVec/FreeVec
#define RES_ENTRY			(RES_EXEC+ 6)		// AllocEntry/FreeEntry
#define RES_SIGNAL			(RES_EXEC+ 7)		// AllocSignal/FreeSignal
#define RES_TRAP			(RES_EXEC+ 8)		// AllocTrap/FreeTrap
#define RES_PORT			(RES_EXEC+ 9)		// AddPort/RemPort
#define RES_LIBRARY			(RES_EXEC+10)		// AddLibrary/RemLibrary
#define RES_LIBRARYIO		(RES_EXEC+11)		// OldOpenLibrary,OpenLibrary/CloseLibrary
#define RES_DEVICE			(RES_EXEC+12)		// AddDevice/RemDevice
#define RES_DEVICEIO		(RES_EXEC+13)		// OpenDevice/CloseDevice
#define RES_RESOURCE		(RES_EXEC+14)		// AddReource,OpenResource/RemResource
#define RES_SEMAPHORE		(RES_EXEC+15)		// AddSemaphore/RemSemaphore
#define RES_SEMAPHOREIO		(RES_EXEC+16)		// ObtainSemaphore/ReleaseSemaphore
#define RES_SEMAPHORELISTIO	(RES_EXEC+17)		// ObtainSemaphoreList/ReleaseSemaphoreList
#define RES_IOREQUEST		(RES_EXEC+18)		// CreateIORequest/DeleteIORequest
#define RES_MSGPORT			(RES_EXEC+19)		// CreateMsgPort/DeleteMsgPort
#define RES_POOL			(RES_EXEC+20)		// CreatePool/DeletePool
#define RES_POOLIO			(RES_EXEC+21)		// AllocPooled/FreePooled
#define RES_MEMHANDLER		(RES_EXEC+22)		// AddMemHandler/RemMemHandler             

//-- dos.library

#define RES_DOS				(RES_EXEC+RES_EXEC_CT)
#define RES_DOS_CT			2

#define RES_LEVEL1FILE		(RES_DOS+ 0)		// Open/Close
#define RES_LOCK			(RES_DOS+ 1)		// Lock/Unlock

//-- amiga.lib

#define RES_SUP				(RES_DOS+RES_DOS_CT)
#define RES_SUP_CT			0

//-- sc#?.lib

#define RES_STDC			(RES_SUP+RES_SUP_CT)
#define RES_STDC_CT			1

#define RES_CLEVEL2FILE		(RES_STDC+ 0)		// fopen/fclos
#define RES_CMEM			(RES_STDC+ 1)		// malloc,strdup/free

//-- scm#?.lib

//-- for Controlflowtracking ----------------------------------------------------------------------

struct __dtCtrlFlowList
{
	struct Node node;
	struct Task	*task;
	struct List ctrlflowlist;					// for Ctrlflowtracking
	ULONG		level;
	ULONG		curstack,maxstack,stacksize;	// for Stackwatch
};

struct __dtCtrlFlowNode
{
	struct Node	node;
	char file[128];
	char func[128];
	ULONG line;
};

//-- for Memory protection ------------------------------------------------------------------------

#define DT_BoundChk_Read	0
#define DT_BoundChk_Write	1

//-- Dataexchange with DT-GUI ---------------------------------------------------------------------

#define DTNM_None			0			// DT_Notify_Methods
#define DTNM_Rebuild		1
#define DTNM_AddMember		2
#define DTNM_RemMember		3
#define DTNM_UpdMember		4

#define DTNV_MainWin_PnlPrgs	0			// DT_Notify_Value
#define DTNV_MainWin_PnlOpts	1
#define DTNV_ResWin				2
#define DTNV_CtrlflowWin		3
#define DTNV_MsgWin				4
#define DTNV_StackWin			5
#define DTNV_WatchWin			6

//-- shared Memorystruktur (DebugTools.library <-> DTGui) -----------------------------------------

struct __dtruntime
{
	struct Task		*taskDTGui,*taskDebug;	// to be filled by dt-gui (exept taskDTGui)
	BYTE			sigNotifyDTGui;
	ULONG			midNotify,mvaNotify,parNotify;

	struct List		*lstCtrlFlow;			// dt-data
	struct List		*lstResources;
	ULONG			devOutput;					// where to send output to	
	ULONG			fmtOutput;					// how to format output
	char			**strResNames;
	UBYTE			flgReport;					// Reportmode
	UBYTE			flgFailReq;					// FailRequesters ?
	UBYTE			flgAutoBreaks;				// Auto BreakPoints ?
};

//-------------------------------------------------------------------------------------------------

#endif	 /* LIBRARIES_DEBUGTOOLS_H */

//-- eof ------------------------------------------------------------------------------------------
