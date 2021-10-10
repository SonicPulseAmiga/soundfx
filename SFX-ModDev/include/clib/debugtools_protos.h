#ifndef  CLIB_DEBUGTOOLS_PROTOS_H
#define  CLIB_DEBUGTOOLS_PROTOS_H

/*
**	$VER: debugtools_protos.h 1.0 (24.02.98)
**
**	(C) Copyright 1998-1998 Stefan Kost
**	    All Rights Reserved
*/

#include <exec/types.h>
#include <libraries/debugtools.h>

/*****************************************************************************/

/* Attribute Managment */

void	DT_SetAttrsA(struct TagItem *tags);
void	DT_GetAttrsA(struct TagItem *tags);
void	DT_SetAttr  (ULONG tag,ULONG data);
ULONG	DT_GetAttr  (ULONG tag);

/* Debugausgaben */

void	DT_Output(STRPTR str,STRPTR file,ULONG line,UBYTE mode);
void	DT_ResetOutput(void);

/* Datatype verification */

UBYTE 	DT_IsValidDouble   (double *x,STRPTR funcname,STRPTR parname,STRPTR file,ULONG line);
UBYTE 	DT_IsValidFloat    (float  *x,STRPTR funcname,STRPTR parname,STRPTR file,ULONG line);
UBYTE	DT_IsValidPointer  (APTR x   ,STRPTR funcname,STRPTR parname,STRPTR file,ULONG line,ULONG align);
UBYTE	DT_IsValidPointerN0(APTR x   ,STRPTR funcname,STRPTR parname,STRPTR file,ULONG line,ULONG align);
UBYTE	DT_IsValidString   (STRPTR x ,STRPTR funcname,STRPTR parname,STRPTR file,ULONG line,ULONG maxlength);

/* Version checking */

UBYTE	DT_CheckVersion(ULONG version,ULONG verreq,ULONG libbase,ULONG line,ULONG *verused,char *func,char *file);

/* Resource management */

void	DT_AddResEntry(APTR res,APTR res2,ULONG size,ULONG type,STRPTR file,ULONG line);
UBYTE	DT_RemResEntry(APTR res,APTR res2,ULONG size,ULONG type,STRPTR file,ULONG line);
UBYTE	DT_ExistsResEntry(APTR res,APTR re2,ULONG type);

/* Controlflow tracking */

void	DT_AddCtrlFlowTask(void);
void	DT_RemCtrlFlowTask(void);
void	DT_EnterLevel(STRPTR where);
void	DT_LeaveLevel(STRPTR where);
STRPTR	DT_CurFile(void);
STRPTR	DT_CurFunc(void);
ULONG	DT_CurLine(void);
STRPTR	DT_LastFile(void);
STRPTR	DT_LastFunc(void);
ULONG	DT_LastLine(void);

/* Stack Checking & Scrambling */

void	DT_StackCheck(struct __dtCtrlFlowList *sublist,char *file,ULONG line);

/* Ressource Fail Requests */

UBYTE	DT_FailReq(STRPTR str,STRPTR file,ULONG line);

/* BreakPoint Requests */

void	DT_BreakPoint(STRPTR str,STRPTR file,ULONG line);

/* Memory protection */

UBYTE	DT_BoundChkIX(APTR base,LONG index,ULONG length,UBYTE amode,UBYTE size,STRPTR file,ULONG line);
UBYTE	DT_BoundChk  (APTR base,APTR index,ULONG length,UBYTE amode,UBYTE size,STRPTR file,ULONG line);

/* Make data accessible for DT-GUI */

void	DT_ConnectGui(struct __dtruntime **rt);

/* configio routines */

void	DT_LoadConfig(STRPTR fname);
void	DT_SaveConfig(STRPTR fname);

/* Send Rexx Commands */

void	DT_SendRexxCmd(STRPTR port,STRPTR cmd);

#endif	 /* CLIB_DEBUGTOOLS_PROTOS_H */
