/******************************************************************************
** SFX-ALL-INCLUDES.h                                                        **
**---------------------------------------------------------------------------**
** Global Include File                                                       **
** for SFX-Libraries                                                         **
**---------------------------------------------------------------------------**
** Version : V 1.2                                                           **
** Date    : 04.02.1994                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

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
#include <hardware/intbits.h>
#include <intuition/gadgetclass.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/screens.h>
#include <intuition/sghooks.h>
#include <libraries/amigaguide.h>
#include <rexx/rxslib.h>
#include <utility/date.h>
#include <utility/hooks.h>
#include <utility/tagitem.h>

#include <clib/alib_protos.h>			/* Prototypes für Supportfunctions */

#include <proto/amigaguide.h>			/* Prototypes für Libraryfunctions */
#include <proto/asl.h>
#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/icon.h>
#include <proto/iffparse.h>
#include <proto/intuition.h>
#include <proto/rexxsyslib.h>
#include <proto/utility.h>
#include <proto/wb.h>

#define __CXM33 __UCXM33				/* für Utillitie.library */
#define __CXD33 __UCXD33
#define __CXM22 __UCXM22
#define __CXD22 __UCXD22
