/****h* debugtools/dt_decl.h [1.20] *
*
*  NAME
*    dt_decl.h
*  COPYRIGHT
*    $VER: dt_decl.h 1.20 (02.07.99) © by Stefan Kost 1998-1999
*  FUNCTION
*    one and only include file for the debugtools system
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    04.Feb.1998
*  MODIFICATION HISTORY
*    02.Jul.1999	V 1.20	first release
*    10.Apr.1998    V 0.80  breakpoints added
*    22.Mar.1998    V 0.20  control-flow macros added
*    04.Feb.1998	V 0.00	initial version
*  NOTES
*
*******
*/

//-- includes -----------------------------------------------------------------

//-- Ansi-C

#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-- AmigaOS

#define USE_SYSBASE
#include <exec/types.h>
#include <cmacros.h>
#include <dos/dos.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <libraries/debugtools.h>

#include <proto/debugtools.h>

//-- definitions --------------------------------------------------------------

//-- debug general

/****** debugtools/--background-- *
*
* PURPOSE
*   A system, which helps you to write better (more solid, less bugs) software.
* USAGE
*   Simply put the following lines into every source or into one global include
*     #define DEBUG_SRCPATH "..."
*     #include <debug/dt-decl.h>
*   Then link with the following libraries.
*     LIB:debug.lib
*     LIB:ddebug.lib
*     LIB:debugtools(|881|ffp|ieee).lib
*   Choose the last one depending on the mathtype you are using.
*   Finally set the symbol DEBUG, either by adding the following line to the
*   global include or every file
*     #define DEBUG
*   or better set the symbol using the compiler option 'DEFINE DEBUG'
*   With this symbol you control, if the system is used or not. 
*
**********
*/
#ifdef DEBUG
	#ifndef DEBUG_DEFS
		extern struct Library *DebugToolsBase;
	#else
		struct Library *DebugToolsBase=0l;
	#endif

	#ifndef DEBUG_SRCPATH
		#define DEBUG_SRCPATH ""
	#endif

	#define	NODEBUG	DebugToolsBase=NULL
#else
	#define NODEBUG
#endif

//-- debug outputs

/****** debugtools/MSG [0.00] *
*
*  NAME
*    MSG -- displays a message
*  SYNOPSIS
*    MSG(str)
*    MSG1(str,arg1)
*    :
*    MSG6(str,arg1,arg2,arg3,arg4,arg5,arg6)
*  FUNCTION
*    Displays a message.
*  INPUTS
*    str - the printf like format string
*    arg<x> - message arguments
*  EXAMPLE
*    MSG1("unexpected error : %ld",errcode);
*  NOTES
*    The system automatically adds a '\n' character to the end of the message,
*    if neccesary,
*  SEE ALSO
*    MSGC(),MSGL(),ERR(),ERRL()
**********
*/
/****** debugtools/MSGC [0.00] *
*
*  NAME
*    MSGC -- displays a message depending on a condition
*  SYNOPSIS
*    MSGC(cond,str)
*    MSGC1(cond,str,arg1)
*    :
*    MSGC6(cond,str,arg1,arg2,arg3,arg4,arg5,arg6)
*  FUNCTION
*    Displays a message if the condition evaluates to true.
*  INPUTS
*    cond - a condition
*    str - the printf like format string
*    arg<x> - message arguments
*  EXAMPLE
*    MSGC1((errcode<0),"unexpected error : %ld",errcode);
*  NOTES
*    The system automatically adds a '\n' character to the end of the message,
*    if neccesary,
*  SEE ALSO
*    MSG(),MSGL(),ERR(),ERRL()
**********
*/
/****i* debugtools/MSGL [0.00] *
*
*  NAME
*    MSGL -- displays a message for a specific file and line
*  SYNOPSIS
*    MSGL(str,file,line)
*    MSGL1(str,arg1,file,line)
*    :
*    MSGL6(str,arg1,arg2,arg3,arg4,arg5,arg6,file,line)
*  FUNCTION
*    Displays a message.
*  INPUTS
*    str - the printf like format string
*    arg<x> - message arguments
*    file - the source file name
*    line - the source file line
*  NOTES
*    The system automatically adds a '\n' character to the end of the message,
*    if neccesary,
*    This macro is used internally by all function wrapper to show the source
*    name and line of the calling procedure
*  SEE ALSO
*    MSG(),MSGC(),ERR(),ERRL()
**********
*/
/****** debugtools/ERR [0.00] *
*
*  NAME
*    ERR -- displays an error message
*  SYNOPSIS
*    ERR(str)
*    ERR1(str,arg1)
*    :
*    ERR6(str,arg1,arg2,arg3,arg4,arg5,arg6)
*  FUNCTION
*    Displays an error message.
*  INPUTS
*    str - the printf like format string
*    arg<x> - message arguments
*  EXAMPLE
*    ERR1("unexpected error : %ld",errcode);
*  NOTES
*    The system automatically adds a '\n' character to the end of the message,
*    if neccesary,
*  SEE ALSO
*    MSG(),MSGC(),ERRL()
**********
*/
/****i* debugtools/ERRL [0.00] *
*
*  NAME
*    ERRL -- displays an error message for a specific file and line
*  SYNOPSIS
*    ERRL(str,file,line)
*    ERRL1(str,arg1,file,line)
*    :
*    ERRL6(str,arg1,arg2,arg3,arg4,arg5,arg6,file,line)
*  FUNCTION
*    Displays an error message for a specific file and line.
*  INPUTS
*    str - the printf like format string
*    arg<x> - message arguments
*    file - the source file name
*    line - the source file line
*  NOTES
*    The system automatically adds a '\n' character to the end of the message,
*    if neccesary,
*    This macro is used internally by all function wrapper to show the source
*    name and line of the calling procedure
*  SEE ALSO
*    MSG(),MSGC(),MSGL(),ERR()
**********
*/
/****** debugtools/IF_... [0.00] *
*
*  NAME
*    IF_... -- conditionl execution
*  SYNOPSIS
*    IF_DEBUG(exp1)
*    IF_DEBUG_ELSE(exp1,exp2)
*  FUNCTION
*    exp1 will be executed in a debug-version and exp2 in a non-debug version.
*  INPUTS
*    exp1,exp2 - any expression
*  EXAMPLE
*    IF_DEBUG_ELSE(SaveInsert(list,node),Insert(list,node));
**********
*/
/****** debugtools/LED_... [0.00] *
*
*  NAME
*    LED_... -- switching the AMIGA-powerled
*  SYNOPSIS
*    LED_ON
*    LED_OFF
*    LED_TOGGLE
*  FUNCTION
*    These macros switching the AMIGA-powerled on, off or toggling the led
*    state.
*  NOTES
*    You must only use this with a REAL AMIGA. Do not use this on clones.
**********
*/
/****** debugtools/SHOW_... [0.00] *
*
*  NAME
*    SHOW_... -- displays contents of variables
*  SYNOPSIS
*    SHOW_BYTE(var)
*    SHOW_WORD(var)
*    SHOW_LONG(var)
*    SHOW_BOOL(var)
*    SHOW_FLT(var)
*    SHOW_PTR(var)
*    SHOW_BPTR(var)
*    SHOW_STR(var)
*    SHOW_BSTR(var)
*  FUNCTION
*    Displays nicely formated the contents of several variable types.
*  INPUTS
*    var - the varable to output
*  EXAMPLE
*    SHOW_BYTE(cntLoop1);
*  NOTES
*    There is no type checking, because those are marcros.
*  IDEAS
*    Use inline functions instead of pure macros, to add type checking.
**********
*/
/****** debugtools/SHOWC_... [0.00] *
*
*  NAME
*    SHOWC_... -- displays contents of variables depending on a condition
*  SYNOPSIS
*    SHOWC_BYTE(cond,var)
*    SHOWC_WORD(cond,var)
*    SHOWC_LONG(cond,var)
*    SHOWC_BOOL(cond,var)
*    SHOWC_FLT(cond,var)
*    SHOWC_PTR(cond,var)
*    SHOWC_BPTR(cond,var)
*    SHOWC_STR(cond,var)
*    SHOWC_BSTR(cond,var)
*  FUNCTION
*    Displays nicely formated the contents of several variable types if the
*    condition evaluates to true.
*  INPUTS
*    cond - a condition
*    var - the varable to output
*  EXAMPLE
*    SHOWC_STR((cntLoop1<10),strList[cntLoop1]);
*  NOTES
*    There is no type checking, because those are marcros.
*  IDEAS
*    Use inline functions instead of pure macros, to add type checking.
**********
*/
/****** debugtools/SHOWC_... [0.00] *
*
*  NAME
*    INTRO,OUTRO -- displays message for entering and leaving a function
*  SYNOPSIS
*    INTRO
*    OUTRO
*  FUNCTION
*    Displays a message indication that the current function has been
*    entered or left.
*  NOTES
*    These function should be the first / last statements in a function.
**********
*/
/****** debugtools/CLEAR_OUTPUT [0.20] *
*
*  NAME
*    CLEAR_OUTPUT -- clears all active output channels
*  SYNOPSIS
*    CLEAR_OUTPUT
*  FUNCTION
*    Clears all active output channels.
**********
*/
#ifdef DEBUG
	#ifndef DEBUG_DEFS
		extern char __dbgfmt[256],__dbgstr[256];
		extern UBYTE *__PRA;
	#else
		char __dbgfmt[256],__dbgstr[256];
		UBYTE *__PRA=(UBYTE *)0xbfe001;		/* u.a. LED/Filter */
	#endif
	#define MSG(str)											do { DT_Output(str,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSG1(str,arg1)										do { sprintf(__dbgfmt,str,arg1);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSG2(str,arg1,arg2)									do { sprintf(__dbgfmt,str,arg1,arg2);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSG3(str,arg1,arg2,arg3)							do { sprintf(__dbgfmt,str,arg1,arg2,arg3);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSG4(str,arg1,arg2,arg3,arg4)						do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSG5(str,arg1,arg2,arg3,arg4,arg5)					do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSG6(str,arg1,arg2,arg3,arg4,arg5,arg6)				do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5,arg6);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSGC(cond,str)										if(cond) do { DT_Output(str,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSGC1(cond,str,arg1)								if(cond) do { sprintf(__dbgfmt,str,arg1);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSGC2(cond,str,arg1,arg2)							if(cond) do { sprintf(__dbgfmt,str,arg1,arg2);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSGC3(cond,str,arg1,arg2,arg3)						if(cond) do { sprintf(__dbgfmt,str,arg1,arg2,arg3);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSGC4(cond,str,arg1,arg2,arg3,arg4)					if(cond) do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSGC5(cond,str,arg1,arg2,arg3,arg4,arg5)			if(cond) do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSGC6(cond,str,arg1,arg2,arg3,arg4,arg5,arg6)		if(cond) do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5,arg6);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define MSGL(str,file,line)									do { DT_Output(str,file,line,DTV_OutputMode_Msg); } while(0)
	#define MSGL1(str,arg1,file,line)							do { sprintf(__dbgfmt,str,arg1);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Msg); } while(0)
	#define MSGL2(str,arg1,arg2,file,line)						do { sprintf(__dbgfmt,str,arg1,arg2);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Msg); } while(0)
	#define MSGL3(str,arg1,arg2,arg3,file,line)					do { sprintf(__dbgfmt,str,arg1,arg2,arg3);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Msg); } while(0)
	#define MSGL4(str,arg1,arg2,arg3,arg4,file,line)			do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Msg); } while(0)
	#define MSGL5(str,arg1,arg2,arg3,arg4,arg5,file,line)		do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Msg); } while(0)
	#define MSGL6(str,arg1,arg2,arg3,arg4,arg5,arg6,file,line)	do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5,arg6);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Msg); } while(0)
	#define ERR(str)											do { DT_Output(str,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Err); } while(0)
	#define ERR1(str,arg1)										do { sprintf(__dbgfmt,str,arg1);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Err); } while(0)
	#define ERR2(str,arg1,arg2)									do { sprintf(__dbgfmt,str,arg1,arg2);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Err); } while(0)
	#define ERR3(str,arg1,arg2,arg3)							do { sprintf(__dbgfmt,str,arg1,arg2,arg3);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Err); } while(0)
	#define ERR4(str,arg1,arg2,arg3,arg4)						do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Err); } while(0)
	#define ERR5(str,arg1,arg2,arg3,arg4,arg5)					do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Err); } while(0)
	#define ERR6(str,arg1,arg2,arg3,arg4,arg5,arg6)				do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5,arg6);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Err); } while(0)
	#define ERRL(str,file,line)									do { DT_Output(str,file,line,DTV_OutputMode_Err); } while(0)
	#define ERRL1(str,arg1,file,line)							do { sprintf(__dbgfmt,str,arg1);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Err); } while(0)
	#define ERRL2(str,arg1,arg2,file,line)						do { sprintf(__dbgfmt,str,arg1,arg2);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Err); } while(0)
	#define ERRL3(str,arg1,arg2,arg3,file,line)					do { sprintf(__dbgfmt,str,arg1,arg2,arg3);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Err); } while(0)
	#define ERRL4(str,arg1,arg2,arg3,arg4,file,line)			do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Err); } while(0)
	#define ERRL5(str,arg1,arg2,arg3,arg4,arg5,file,line)		do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Err); } while(0)
	#define ERRL6(str,arg1,arg2,arg3,arg4,arg5,arg6,file,line)	do { sprintf(__dbgfmt,str,arg1,arg2,arg3,arg4,arg5,arg6);DT_Output(__dbgfmt,file,line,DTV_OutputMode_Err); } while(0)
	#define IF_DEBUG(exp1) exp1
	#define IF_DEBUG_ELSE(exp1,exp2) exp1
	#define LED_ON											*__PRA&=(UBYTE)253
	#define LED_OFF											*__PRA|=(UBYTE)2
	#define LED_TOGGLE										*__PRA^=(UBYTE)2
	#define LEDC_ON(cond)									if(cond) *__PRA&=(UBYTE)253
	#define LEDC_OFF(cond)									if(cond) *__PRA|=(UBYTE)2
	#define LEDC_TOGGLE(cond)								if(cond) *__PRA^=(UBYTE)2
	#define SHOW_BYTE(var)									do { sprintf(__dbgfmt,"(U)BYTE %10s=%4d=0x%02x='%c'",#var,var,var,((var>31) ? var : '.'));DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOW_WORD(var)									do { sprintf(__dbgfmt,"(U)WORD %10s=%6hd=0x%04x",#var,var,var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOW_LONG(var)									do { sprintf(__dbgfmt,"(U)LONG %10s=%11ld=0x%08lx",#var,var,var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOW_BOOL(var)									do { sprintf(__dbgfmt,"(U)LONG %10s=%s",#var,((var) ? "true" : "false"));DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOW_FLT(var)									do { sprintf(__dbgfmt,"FLT/DBL %10s=%lf",#var,(double)var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOW_PTR(var)									do { sprintf(__dbgfmt,"POINTER %10s=0x%08lx",#var,var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOW_BPTR(var)									do { sprintf(__dbgfmt,"POINTER %10s=0x%08lx",#var,BADDR(var));DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOW_STR(var)									do { sprintf(__dbgfmt,"STRING  %10s=%6ld=\"%s\"",#var,strlen(var),var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOW_BSTR(var)									do { sprintf(__dbgfmt,"STRING  %10s=%6ld=\"%.*s\"",#var,*((STRPTR)BADDR(var)),*((STRPTR)BADDR(var)),&((STRPTR)BADDR(var))[1]);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOWC_BYTE(cond,var)							if(cond) do { sprintf(__dbgfmt,"(U)BYTE %10s=%4d=0x%02x='%c'",#var,var,var,((var>31) ? var : '.'));DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOWC_WORD(cond,var)							if(cond) do { sprintf(__dbgfmt,"(U)WORD %10s=%6hd=0x%04x",#var,var,var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOWC_LONG(cond,var)							if(cond) do { sprintf(__dbgfmt,"(U)LONG %10s=%11ld=0x%08lx",#var,var,var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOWC_BOOL(cond,var)							if(cond) do { sprintf(__dbgfmt,"(U)LONG %10s=%s",#var,((var) ? "true" : "false"));DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOWC_FLT(cond,var)								if(cond) do { sprintf(__dbgfmt,"FLT/DBL %10s=%lf",#var,(double)var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOWC_PTR(cond,var)								if(cond) do { sprintf(__dbgfmt,"POINTER %10s=0x%08lx",#var,var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOWC_BPTR(cond,var)							if(cond) do { sprintf(__dbgfmt,"POINTER %10s=0x%08lx",#var,BADDR(var));DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOWC_STR(cond,var)								if(cond) do { sprintf(__dbgfmt,"STRING  %10s=%6ld=\"%s\"",#var,strlen(var),var);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define SHOWC_BSTR(cond,var)							if(cond) do { sprintf(__dbgfmt,"STRING  %10s=%6ld=\"%.*s\"",#var,*((STRPTR)BADDR(var)),*((STRPTR)BADDR(var)),&((STRPTR)BADDR(var))[1]);DT_Output(__dbgfmt,DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define INTRO											do { DT_Output(__FUNC__" beg",DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define OUTRO											do { DT_Output(__FUNC__" end",DEBUG_SRCPATH __FILE__,__LINE__,DTV_OutputMode_Msg); } while(0)
	#define REXX_CMD(port,cmd)								DT_SendRexxCmd(port,cmd);
	#define CLEAR_OUTPUT									DT_ResetOutput(void);
#else
	#define MSG(str)
	#define MSG1(str,arg1)
	#define MSG2(str,arg1,arg2)
	#define MSG3(str,arg1,arg2,arg3)
	#define MSG4(str,arg1,arg2,arg3,arg4)
	#define MSG5(str,arg1,arg2,arg3,arg4,arg5)
	#define MSG6(str,arg1,arg2,arg3,arg4,arg5,arg6)
	#define MSGC(cond,str)
	#define MSGC1(cond,str,arg1)
	#define MSGC2(cond,str,arg1,arg2)
	#define MSGC3(cond,str,arg1,arg2,arg3)
	#define MSGC4(cond,str,arg1,arg2,arg3,arg4)
	#define MSGC5(cond,str,arg1,arg2,arg3,arg4,arg5)
	#define MSGC6(cond,str,arg1,arg2,arg3,arg4,arg5,arg6)
	#define MSGL(str,file,line)
	#define MSGL1(str,file,line,arg1)
	#define MSGL2(str,file,line,arg1,arg2)
	#define MSGL3(str,file,line,arg1,arg2,arg3)
	#define MSGL4(str,file,line,arg1,arg2,arg3,arg4)
	#define MSGL5(str,file,line,arg1,arg2,arg3,arg4,arg5)
	#define MSGL6(str,file,line,arg1,arg2,arg3,arg4,arg5,arg6)
	#define ERR(str)
	#define ERR1(str,arg1)
	#define ERR2(str,arg1,arg2)
	#define ERR3(str,arg1,arg2,arg3)
	#define ERR4(str,arg1,arg2,arg3,arg4)
	#define ERR5(str,arg1,arg2,arg3,arg4,arg5)
	#define ERR6(str,arg1,arg2,arg3,arg4,arg5,arg6)
	#define ERRL(str,file,line)
	#define ERRL1(str,file,line,arg1)
	#define ERRL2(str,file,line,arg1,arg2)
	#define ERRL3(str,file,line,arg1,arg2,arg3)
	#define ERRL4(str,file,line,arg1,arg2,arg3,arg4)
	#define ERRL5(str,file,line,arg1,arg2,arg3,arg4,arg5)
	#define ERRL6(str,file,line,arg1,arg2,arg3,arg4,arg5,arg6)
	#define IF_DEBUG(exp1)
	#define IF_DEBUG_ELSE(exp1,exp2) exp2
	#define LED_ON
	#define LED_OFF
	#define LED_TOGGLE
	#define LEDC_ON(cond)
	#define LEDC_OFF(cond)
	#define LEDC_TOGGLE(cond)
	#define SHOW_BYTE(var)
	#define SHOW_WORD(var)
	#define SHOW_LONG(var)
	#define SHOW_BOOL(var)
	#define SHOW_FLT(var)
	#define SHOW_PTR(var)
	#define SHOW_BPTR(var)
	#define SHOW_STR(var)
	#define SHOW_BSTR(var)
	#define SHOWC_BYTE(cond,var)
	#define SHOWC_WORD(cond,var)
	#define SHOWC_LONG(cond,var)
	#define SHOWC_BOOL(cond,var)
	#define SHOWC_FLT(cond,var)
	#define SHOWC_PTR(cond,var)
	#define SHOWC_BPTR(cond,var)
	#define SHOWC_STR(cond,var)
	#define SHOWC_BSTR(cond,var)
	#define INTRO
	#define OUTRO
	#define REXX_CMD(port,cmd)
	#define CLEAR_OUTPUT
#endif

//-- assertions (condition checking)

/****** debugtools/ASSERT [0.00] *
*
*  NAME
*    ASSERT -- produces an error if an expression evaluates to FALSE
*  SYNOPSIS
*    ASSERT(exp)
*  FUNCTION
*    Evaluates the expression and prints out an error if the result is FALSE.
*    If one of a series of assertions fails the global assert state is false as
*    well.
*  INPUTS
*    exp - expression with a result to test for (condition)
*  EXAMPLE
*    ASSERT(ptrMem);
*    ASSERT(numMaxLoops<conMaxLoops);
*
*    ASSERT_RESET;
*    ASSERT(...)
*    ASSERT(...)
*    :
*    if(ASSERT_OKAY) ...
*  NOTES
*    If the global state is used it must be reseted first
*  SEE ALSO
*    ASSERTL(),ASSERTQ(),ASSERT_RESET,ASSERT_OKAY,VERIFY(),VERIFYL()
**********
*/
/****** debugtools/ASSERTL [0.00] *
*
*  NAME
*    ASSERTL -- prints an error (specific file & line) if the assertion fails
*  SYNOPSIS
*    ASSERTL(exp,file,line,msg)
*  FUNCTION
*    Evaluates the expression and prints out an error if the result is FALSE.
*    If one of a series of assertions fails the global assert state is false as
*    well.
*  INPUTS
*    exp - expression with a result to test for (condition)
*    file - the source file name
*    line - the source file line
*    msg - the error message
*  EXAMPLE
*    ASSERTL(ptrMem,__FILE__,__LINE__,"1. par is null pointer);
*  NOTES
*    If the global state is used it must be reseted first
*    This macro is used internally by all function wrapper to show the source
*    name and line of the calling procedure
*  SEE ALSO
*    ASSERT(),ASSERTQ(),ASSERT_RESET,ASSERT_OKAY,VERIFY(),VERIFYL()
**********
*/
/****** debugtools/ASSERTQ [0.00] *
*
*  NAME
*    ASSERTQ -- checks if an assertion fails
*  SYNOPSIS
*    ASSERTL(exp)
*  FUNCTION
*    Evaluates the expression and sets the global state to FALSE the condition
*    resulted in FALSE.
*  INPUTS
*    exp - expression with a result to test for (condition)
*  EXAMPLE
*    ASSERTQ(IS_VALID_POINTER(ptrMem,"free","1. par"));
*  NOTES
*    If the global state is used it must be reseted first
*    This macro can be used when a part of the expression already prints the
*    error.
*  SEE ALSO
*    ASSERT(),ASSERTL(),ASSERT_RESET,ASSERT_OKAY,VERIFY(),VERIFYL()
**********
*/
/****** debugtools/ASSERT_RESET [0.00] *
*
*  NAME
*    ASSERT_RESET
*  SYNOPSIS
*    ASSERT_RESET
*  FUNCTION
*    Resets the global assertion state.
*  NOTES
*    This is required, before a series of ASSERT's is started, when the result
*    is required.
*  SEE ALSO
*    ASSERT(),ASSERTL(),ASSERT_OKAY,VERIFY(),VERIFYL()
**********
*/
/****** debugtools/VERIFY [0.00] *
*
*  NAME
*    VERIFY -- produces an error if an expression evaluates to FALSE
*  SYNOPSIS
*    VERIFY(exp)
*  FUNCTION
*    Evaluates the expression and prints out an error if the result is FALSE.
*    If one of a series of assertions fails the global assert state is false as
*    well.
*    In contrast to ASSERT the expression will be evaluated in non-debug ver-
*    sions too.
*  INPUTS
*    exp - expression with a result to test for (condition)
*  EXAMPLE
*    VERIFY(ptrMem=malloc(MEMSIZE));
*  NOTES
*    If the global state is used it must be reseted first
*  SEE ALSO
*    ASSERT(),ASSERTL(),ASSERTQ(),ASSERT_RESET,ASSERT_OKAY,VERIFYL()
**********
*/
/****** debugtools/VERIFYL [0.00] *
*
*  NAME
*    VERIFYL -- prints an error (specific file & line) if the assertion fails
*  SYNOPSIS
*    VERIFYL(exp,file,line,msg)
*  FUNCTION
*    Evaluates the expression and prints out an error if the result is FALSE.
*    If one of a series of assertions fails the global assert state is false as
*    well.
*    In contrast to ASSERT the expression will be evaluated in non-debug ver-
*    sions too.
*  INPUTS
*    exp - expression with a result to test for (condition)
*    file - the source file name
*    line - the source file line
*    msg - the error message
*  NOTES
*    If the global state is used it must be reseted first
*    This macro is used internally by all function wrapper to show the source
*    name and line of the calling procedure
*  SEE ALSO
*    ASSERT(),ASSERTL(),ASSERTQ(),ASSERT_RESET,ASSERT_OKAY,VERIFY()
**********
*/
/****** debugtools/IS_VALID_... [0.00] *
*
*  NAME
*    IS_VALID_... -- tests the correctness of some basic types
*  SYNOPSIS
*    res=IS_VALID_DOUBLE(x,funcname,parname)
*    res=IS_VALID_FLOAT(x,funcname,parmum)
*    res=IS_VALID_POINTER(x,funcname,parmum,align)
*    res=IS_VALID_POINTER_N0(x,funcname,parmum,align)
*    res=IS_VALID_STRING(x,funcname,parmum,maxlength)
*  FUNCTION
*    Tests if the supplied parameter is a valid member of the variable type.
*  INPUTS
*    x - variable to test
*        DOUBLE     double *
*        FLOAT      float *
*        POINTER    APTR
*        POINTER_N0 APTR
*        STRING     STRPTR
*    funcname - name of the function, which we are in
*    parname - name of the variable/parameter
*    align - if the pointer needs to be aligned, supply here to how many bytes
*    maxlength - the maximum legnth, for which the string will be checked
*  RESULT
*    res - 0 if variable is not valid, 1 if it is
*  EXAMPLE
*    ASSERTQ(IS_VALID_DOUBLE(&x,"sqrt","1.par"));
*  SEE ALSO
*    ASSERTQ()
**********
*/
/****** debugtools/CHECK_VER [0.00] *
*
*  NAME
*    CHECK_VER -- performs library version checking and tracking
*  SYNOPSIS
*    res=CHECK_VER(lib,ver,func,file,line)
*  FUNCTION
*    Checks if a function is available in a specific library version. Further-
*    more it tracks the used version numbers to find out which version is re-
*    quired at least.
*    Additionally it tests if the library is opened, before it is used.
*  INPUTS
*    lib - the library name
*    ver - the version from which the function exists
*    func - the function name
*    file - the source file name
*    line - the source file line
*  RESULT
*    res - 0 if function is not available, 1 else
*  EXAMPLE
*    ASSERTQ(CHECK_VER(Exec,OSVER_1_2,"exec.library/AddMemList",file,line));
**********
*/
#ifdef DEBUG
	#ifndef DEBUG_DEFS
		extern BOOL __flgAssert;
	#else
		BOOL __flgAssert;
	#endif
	#define FAIL	FALSE
	#define OKAY	TRUE
	#define ASSERT(exp)                do { if(__flgAssert && !(exp)) { ERR1("assertion <%s> failed",#exp);__flgAssert=FAIL; } } while(0)
	#define ASSERTL(exp,file,line,msg) do { if(__flgAssert && !(exp)) { ERRL(msg,file,line);__flgAssert=FAIL; } } while(0)
	#define ASSERTQ(exp)               do { if(__flgAssert && !(exp)) __flgAssert=FAIL; } while(0)
	#define ASSERT_RESET               do { __flgAssert=OKAY; } while(0)
	#define ASSERT_OKAY                __flgAssert
	#define VERIFY(exp)                do { if(!(exp)) { ERR1("assertion <%s> failed",#exp);__flgAssert=FAIL; } } while(0)
	#define VERIFYL(exp,file,line,msg) do { if(!(exp)) { ERRL(msg,file,line);__flgAssert=FAIL; } } while(0)

	#define IS_VALID_DOUBLE(x,funcname,parname) DT_IsValidDouble(x,funcname,parname,DEBUG_SRCPATH __FILE__,__LINE__)
	#define IS_VALID_FLOAT(x,funcname,parname)  DT_IsValidFloat (x,funcname,parname,DEBUG_SRCPATH __FILE__,__LINE__)

	#define IS_VALID_POINTER(x,funcname,parname,align)    DT_IsValidPointer  ((APTR)x,funcname,parname,DEBUG_SRCPATH __FILE__,__LINE__,align)
	#define IS_VALID_POINTER_N0(x,funcname,parname,align) DT_IsValidPointerN0((APTR)x,funcname,parname,DEBUG_SRCPATH __FILE__,__LINE__,align)

	#define IS_VALID_STRING(x,funcname,parname,maxlength) DT_IsValidString(x,funcname,parname,DEBUG_SRCPATH __FILE__,__LINE__,maxlength)

	#define	OSVER_1_0			30
	#define	OSVER_1_1			31
	#define	OSVER_1_2			33
	#define	OSVER_1_3			34
	#define	OSVER_2_0			36
	#define	OSVER_2_04			37
	#define	OSVER_2_1			38
	#define	OSVER_3_0			39
	#define	OSVER_3_1			40
	#define CHECK_VER(lib,ver,func,file,line)	DT_CheckVersion(ver,__LibVerReq_##lib,__LibBase_##lib,line,&__LibVerMaxUsed_##lib,func,file)
#else
	#define ASSERT(exp)
	#define ASSERTL(exp,file,line,msg)
	#define ASSERTQ(exp)
	#define ASSERT_RESET
	#define ASSERT_OKAY 1
	#define VERIFY(exp) exp
	#define VERIFYL(exp,file,line,msg) exp
	#define IS_VALID_DOUBLE(x,funcname,parmum) 1
	#define IS_VALID_FLOAT(x,funcname,parmum) 1
	#define IS_VALID_POINTER(x,funcname,parmum,align) 1
	#define IS_VALID_POINTER_N0(x,funcname,parmum,align) 1
	#define IS_VALID_STRING(x,funcname,parmum,maxlength) 1
	#define CHECK_VER(lib,ver,func,file,line)
#endif

//-- breakpoints

/****** debugtools/BREAK [0.80] *
*
*  NAME
*    BREAK -- halt the process
*  SYNOPSIS
*    BREAK(str)
*  FUNCTION
*    Halts the process, displays the supplied message in a request-window and
*    waits for the "Continue"-button to be pressed.
*  INPUTS
*    str - a string to be displayed, could be NULL as well
*  EXAMPLE
*    BREAK("critical point reached");
*  SEE ALSO
*    BREAKC();AUTOBREAKS_...
**********
*/
/****** debugtools/BREAKC [0.80] *
*
*  NAME
*    BREAKC -- halt the process if a condition is satisfied
*  SYNOPSIS
*    BREAKC(cond,str)
*  FUNCTION
*    Halts the process if a condition is satisfied, displays the supplied
*    message in a request-window and waits for the "Continue"-button to be
*    pressed.
*  INPUTS
*    cond - a condition
*    str - a string to be displayed, could be NULL as well
*  EXAMPLE
*    BREAKC((numLoops>100),"critical point reached");
*  SEE ALSO
*    BREAK();AUTOBREAKS_...
**********
*/
/****** debugtools/AUTOBREAKS_... [0.80] *
*
*  NAME
*    AUTOBREAKS_... -- switches auto break points on or off
*  SYNOPSIS
*    AUTOBREAKS_OFF
*    AUTOBREAKS_ENTER
*    AUTOBREAKS_LEAVE
*    AUTOBREAKS_BOTH
*  FUNCTION
*    Enables or disables automatic break points. If enabled these breakpoints
*    are inserted at each function entry, exit or both locations.
*  EXAMPLE
*    AUTOBREAKS_BOTH;
*    : // some critical code
*    AUTOBREAKS_OFF;
*  NOTES
*    Requires that the code has been compiler with the PROFILE option.
**********
*/
#ifdef DEBUG
	#define BREAK(str)			DT_BreakPoint(str,DEBUG_SRCPATH __FILE__,__LINE__)
	#define BREAKC(cond,str)	if(cond) DT_BreakPoint(str,DEBUG_SRCPATH __FILE__,__LINE__)
	#define AUTOBREAKS_OFF		DT_SetAttr(DTA_AutoBreaks,DTV_AutoBreaks_Off)
	#define AUTOBREAKS_ENTER	DT_SetAttr(DTA_AutoBreaks,DTV_AutoBreaks_Enter)
	#define AUTOBREAKS_LEAVE	DT_SetAttr(DTA_AutoBreaks,DTV_AutoBreaks_Leave)
	#define AUTOBREAKS_BOTH		DT_SetAttr(DTA_AutoBreaks,DTV_AutoBreaks_Both)
#else
	#define BREAK(str)
	#define BREAKC(cond,str)
	#define AUTOBREAKS_OFF
	#define AUTOBREAKS_ENTER
	#define AUTOBREAKS_LEAVE
	#define AUTOBREAKS_BOTH
#endif

//-- control flow tracking

/****** debugtools/CUR... [0.20] *
*
*  NAME
*    CUR... -- get current location
*  SYNOPSIS
*    res=CURFILE
*    res=CURFUNC
*    res=CURLINE
*  FUNCTION
*    Gets the location the current code comes from.
*  RESULT
*    res - CURFILE : the file name the current function is defined in
*          CURFUNC : the current functions name
*          CURLINE : the line at which the definition of the current function
*                    starts
*  NOTES
*    Requires that the code has been compiler with the PROFILE option.
*  SEE ALSO
*    LAST...
**********
*/
/****** debugtools/LAST... [0.20] *
*
*  NAME
*    LAST... -- get previous location
*  SYNOPSIS
*    res=LASTFILE
*    res=LASTFUNC
*    res=LASTLINE
*  FUNCTION
*    Gets the location of the function, from which the current one has been
*    called.
*  RESULT
*    res - CURFILE : the file name the previous function is defined in
*          CURFUNC : the previous functions name
*          CURLINE : the line at which the definition of the previous function
*                    starts
*  NOTES
*    Requires that the code has been compiler with the PROFILE option.
*  SEE ALSO
*    CUR...
**********
*/
#ifdef DEBUG
	#define CURFILE  DT_CurFile()
	#define CURFUNC  DT_CurFunc()
	#define CURLINE  DT_CurLine()
	#define LASTFILE DT_LastFile()
	#define LASTFUNC DT_LastFunc()
	#define LASTLINE DT_LastLine()
#else
	#define CURFILE  "unknown"
	#define CURFUNC  "unknown"
	#define CURLINE  0
	#define LASTFILE "unknown"
	#define LASTFUNC "unknown"
	#define LASTLINE 0
#endif

//-- resource tracking & os wrapping

/****** debugtools/SCRAMBLE_PTR [0.00] *
*
*  NAME
*    SCRAMBLE_PTR -- invalidates a pointer
*  SYNOPSIS
*    SCRAMBLE_PTR(ptr);
*  FUNCTION
*    Marks a pointer as invalid. This is useful to let the dt.system find out,
*    if you are going to use a ressource after you have already freed it.
*  INPUTS
*    ptr - a pointer (lvalue required - no expressions, no casts)
*  EXAMPLE
*    Free(ptrMem);SCRAMBLE_PTR(ptrMem);
**********
*/
/****** debugtools/REPORT_... [0.00] *
*
*  NAME
*    REPORT_... -- switches auto reports on or off
*  SYNOPSIS
*    REPORT_OFF
*    REPORT_CALLS
*    REPORT_FAILS
*  FUNCTION
*    REPORT_OFF - switches reports off
*    REPORT_CALLS - generates report entries for each OS-call
*    REPORT_FAILS - generate report entries for each OS-call which failed
*  EXAMPLE
*    REPORT_CALLS;
*    :             // critical section
*    REPORT_OFF;
*  SEE ALSO
*    REPORT
**********
*/
/****** debugtools/REPORT [0.00] *
*
*  NAME
*    REPORT -- returns  which kind of reports is desired
*  SYNOPSIS
*    res=REPORT
*  FUNCTION
*    Return which kind of reports has been choosen.
*  RESULT
*    res - type of report
*          DTV_Report_Off
*          DTV_Report_Calls
*          DTV_Report_Fails
*  SEE ALSO
*    REPORT_...
**********
*/
/****** debugtools/FAILREQ_... [0.00] *
*
*  NAME
*    FAILREQ_... -- switches ressource fail requester on or off
*  SYNOPSIS
*    FAILREQ_ON
*    FAILREQ_OFF
*  FUNCTION
*    Switches ressource fail requesters on or off. If switched on, the user can
*    interactive decide that a ressource allocation should fail. This is useful
*    to test program paths which are not executed normally.
*  SEE ALSO
*    FAILREQ
**********
*/
/****** debugtools/FAILREQ [0.00] *
*
*  NAME
*    FAILREQ -- returns if fail requesters are switched on or off
*  SYNOPSIS
*    res=FAILREQ
*  FUNCTION
*    Returns if fail requesters are switched on or off
*  RESULT
*    res - 1 if they are switched on, 0 if off
*  SEE ALSO
*    FAILREQ_...
**********
*/
#ifdef DEBUG
	#ifndef DEBUG_DEFS
//		extern APTR __sosptr;
		extern ULONG __LibVerReq_Dos	  ,__LibVerOpened_Dos	   ,__LibVerMaxUsed_Dos		 ,__LibBase_Dos;
		extern ULONG __LibVerReq_Exec	  ,__LibVerOpened_Exec	   ,__LibVerMaxUsed_Exec	 ,__LibBase_Exec;
		extern ULONG __LibVerReq_Graphics ,__LibVerOpened_Graphics ,__LibVerMaxUsed_Graphics ,__LibBase_Graphics;
		extern ULONG __LibVerReq_Intuition,__LibVerOpened_Intuition,__LibVerMaxUsed_Intuition,__LibBase_Intuition;
	#else
//		APTR __sosptr;
		ULONG __LibVerReq_Dos	   =0,__LibVerOpened_Dos	  =0,__LibVerMaxUsed_Dos	  =0,__LibBase_Dos		=0;
		ULONG __LibVerReq_Exec	   =0,__LibVerOpened_Exec	  =0,__LibVerMaxUsed_Exec	  =0,__LibBase_Exec		=0;
		ULONG __LibVerReq_Graphics =0,__LibVerOpened_Graphics =0,__LibVerMaxUsed_Graphics =0,__LibBase_Graphics	=0;
		ULONG __LibVerReq_Intuition=0,__LibVerOpened_Intuition=0,__LibVerMaxUsed_Intuition=0,__LibBase_Intuition=0;
	#endif
	#define SCRAMBLE_PTR(ptr)	ptr=DT_InvalidPointer
	#define REPORT_OFF			DT_SetAttr(DTA_Report,DTV_Report_Off)
	#define REPORT_CALLS		DT_SetAttr(DTA_Report,DTV_Report_Calls)
	#define REPORT_FAILS		DT_SetAttr(DTA_Report,DTV_Report_Fails)
	#define REPORT				DT_GetAttr(DTA_Report)
	#define FAILREQ_OFF			DT_SetAttr(DTA_FailReq,DTV_FailReq_Off)
	#define FAILREQ_ON			DT_SetAttr(DTA_FailReq,DTV_FailReq_On)
	#define FAILREQ				DT_GetAttr(DTA_FailReq)
#else
	#define SCRAMBLE_PTR(ptr)
	#define REPORT_OFF
	#define REPORT_CALLS
	#define REPORT_FAILS
	#define REPORT 0
	#define FAILREQ_OFF
	#define FAILREQ_ON
	#define FAILREQ	0
#endif

// !!! leave it this way !!!

#include <debug/sos_exec_decl.h>		// exec-functions
//#include <debug/sos_listtools_decl.h>	// listtools-functions

#include <debug/sos_stdc_decl.h>		// stdc-functions : sc#?.lib
#include <debug/sos_math_decl.h>		// math-functions : scm#?.lib

//-- memory protection (memory bound checking)

/****** debugtools/MemReadIX [0.00] *
*
*  NAME
*    MemReadIX -- reads one value from an array
*  SYNOPSIS
*    val=MemReadIX(base,index,size)
*  FUNCTION
*    Reads one value from an array. Performs bound checks to detect illegal
*    memory accesses.
*  INPUTS
*    base - basis address of the array
*    index - index to read value from
*    size - length of the array
*  RESULT
*    val - value form array or 0 if it is an illegal access
*  EXAMPLE
*    numSum+=MemReadIX(ptrVector,i,numVectorSize);
*  TODO
*    Add variatons for multidimensional arrays.
*  SEE ALSO
*    MemWriteIX(),MemRead(),MemWrite()
**********
*/
/****** debugtools/MemWriteIX [0.00] *
*
*  NAME
*    MemWriteIX -- writes one value into an array
*  SYNOPSIS
*    MemWriteIX(base,index,size,val)
*  FUNCTION
*    Writes one value into an array. Performs bound checks to detect illegal
*    memory accesses.
*  INPUTS
*    base - basis address of the array
*    index - index to write value into
*    size - length of the array
*    val  - value to write
*  EXAMPLE
*    MemWriteIX(ptrVector,i,numVectorSize,i);
*  TODO
*    Add variatons for multidimensional arrays.
*  SEE ALSO
*    MemReadIX(),MemRead(),MemWrite()
**********
*/
/****** debugtools/MemRead [0.00] *
*
*  NAME
*    MemRead -- reads one value from memory
*  SYNOPSIS
*    val=MemRead(base,index,size)
*  FUNCTION
*    Reads one value from memory. Performs bound checks to detect illegal
*    memory accesses.
*  INPUTS
*    base - basis address of the memory block
*    index - index (pointer) to read value from
*    size - length of the memory block
*  RESULT
*    val - value form array or 0 if it is an illegal access
*  EXAMPLE
*    numSum+=MemReadIX(ptrVector,ptrElement,numVectorSize);
*  SEE ALSO
*    MemReadIX(),MemWriteIX(),MemWrite()
**********
*/
/****** debugtools/MemWrite [0.00] *
*
*  NAME
*    MemWrite -- writes one value into memory
*  SYNOPSIS
*    MemWrite(base,index,size,val)
*  FUNCTION
*    Writes one value into memory. Performs bound checks to detect illegal
*    memory accesses.
*  INPUTS
*    base - basis address of the memory block
*    index - index (pointer) to write value into
*    size - length of the memory block
*    val  - value to write
*  EXAMPLE
*    MemWrite(ptrVector,ptrElement,numVectorSize,i);
*  SEE ALSO
*    MemReadIX(),MemWriteIX(),MemRead()
**********
*/
#ifdef DEBUG
	#define MemReadIX(base,index,size)		DT_BoundChkIX(base,index,size,DT_BoundChk_Read,sizeof(*base),DEBUG_SRCPATH __FILE__,__LINE__) ? base[index] : 0
	#define MemWriteIX(base,index,size,val)	if(DT_BoundChkIX(base,index,size,DT_BoundChk_Write,sizeof(*base),DEBUG_SRCPATH __FILE__,__LINE__)) base[index]=val
	#define MemRead(base,index,size)		DT_BoundChk(base,index,size,DT_BoundChk_Read,sizeof(*base),DEBUG_SRCPATH __FILE__,__LINE__) ? *index : 0
	#define MemWrite(base,index,size,val)	if(DT_BoundChk(base,index,size,DT_BoundChk_Write,sizeof(*base),DEBUG_SRCPATH __FILE__,__LINE__)) *index=val
#else
	#define MemReadIX(base,index,size)		base[index]
	#define MemWriteIX(base,index,size,val) base[index]=val
	#define MemRead(base,index,size)		*index
	#define MemWrite(base,index,size,val)	*index=val
#endif

//-- stack checking & scambling

/****** debugtools/STACKCHECK [1.00] *
*
*  NAME
*    STACKCHECK -- checks for stack-overflows and invalidates unused stack area
*  SYNOPSIS
*    STACKCHECK
*  FUNCTION
*    Checks for stack overflows, keeps track of maximum stack-usage. Further-
*    more it invalidates unused stack space to force that the use of uninitia-
*    lized variables will cause Enforcer/CyberGuard hits or other effects.
*  NOTES
*    When you compile with the PROFILE option, this function is called auto-
*    matically each time a function is called and left.
**********
*/
#ifdef DEBUG
	#define STACKCHECK						DT_StackCheck(0l,DEBUG_SRCPATH __FILE__,__LINE__);
#else
	#define STACKCHECK
#endif

//-- overload error traps

#ifdef DEBUG
	extern int _FPERR;
	#ifndef DEBUG_DEFS
		extern void __InitTraps(void);
		extern void __DoneTraps(void);
	#else
		void __InitTraps(void);
		void __DoneTraps(void);
	#endif
	#define INITTRAPS __InitTraps()
	#define DONETRAPS __DoneTraps()
#else
	#define INITTRAPS
	#define DONETRAPS
#endif

//-- safe casts

#ifdef DEBUG
	#ifndef DEBUG_DEFS
		extern UBYTE SC_2UBYTE(ULONG var,char *file,unsigned long line);
		extern UWORD SC_2UWORD(ULONG var,char *file,unsigned long line);
	#else
		UBYTE SC_2UBYTE(ULONG var,char *file,unsigned long line);
		UWORD SC_2UWORD(ULONG var,char *file,unsigned long line);
	#endif
	#define UBYTE(var)	SC_2UBYTE((ULONG)var,__FILE__,__LINE__)
	#define UWORD(var)	SC_2UWORD((ULONG)var,__FILE__,__LINE__)
#endif

//-- general

/****** debugtools/INITDEBUG [0.00] *
*
*  NAME
*    INITDEBUG -- calles the dt-system constructor
*  SYNOPSIS
*    INITDEBUG
*  FUNCTION
*    Calles the dt-system constructor, which initializes the dt-system for use.
*    This is normally done automatically by the startup-code. It must be done for
*    own tasks / processes and shared libraries / devices / ressources etc.
*  SEE ALSO
*    DONEDEBUG
**********
*/
/****** debugtools/DONEDEBUG [0.00] *
*
*  NAME
*    DONEDEBUG -- calles the dt-system destructor
*  SYNOPSIS
*    DONEDEBUG
*  FUNCTION
*    Calles the dt-system destructor, which cleans up the dt-system after use.
*    This is normally done automatically by the startup-code. It must be done for
*    own tasks / processes and shared libraries / devices / ressources etc.
*  SEE ALSO
*    INITDEBUG
**********
*/
#ifdef DEBUG
	#ifndef DEBUG_DEFS
		extern int  _STI_1000_DebugTools(void);
		extern void _STD_1000_DebugTools(void);
	#else
		int  _STI_1000_DebugTools(void);
		void _STD_1000_DebugTools(void);
	#endif
	#define INITDEBUG _STI_1000_DebugTools()
	#define DONEDEBUG _STD_1000_DebugTools()
#else
	#define INITDEBUG
	#define DONEDEBUG
#endif

//-- eof ----------------------------------------------------------------------
