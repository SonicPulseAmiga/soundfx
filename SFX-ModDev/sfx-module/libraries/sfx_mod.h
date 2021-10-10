#ifndef  LIBRARIES_SFX_MOD_H
#define  LIBRARIES_SFX_MOD_H

/*
**	$VER: sfx_mod.h 3.70 (08.09.98)
**
**	(C) Copyright 1993-1998 Stefan Kost
**	    All Rights Reserved
*/

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

#include "project:sfx-main/SFX-Images.h"
#include "project:sfx-main/SFX-LocaleId.h"
#include "project:sfx-main/SFX-FileDefs.h"

/*---------------------------------------------------------------------------------------------------------------*/

#define SFX_PUBSCRNAME "SFX-PubScreen"

/*---------------------------------------------------------------------------------------------------------------*/

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

UBYTE *RModeLabels[]={ "All","Window","Range",0l };

//UBYTE *BandLabels[]={ "4","8","16","32","64","128","256","512",0l };
//UBYTE *StepLabels[]={ "1","2","4","8","16","32","64","128","256",0l };

/*---------------------------------------------------------------------------------------------------------------*/

#endif	 /* LIBRARIES_SFX_MOD_H */
