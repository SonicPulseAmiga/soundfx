/****h*convertprefs/cp-includes.h [1.00] *
*
*  NAME
*    cp-includes.h
*  COPYRIGHT
*    $VER: cp-includes.h 1.00 (21.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    main include file
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    21.Aug.1998
*  MODIFICATION HISTORY
*    21.Aug.1998	V 1.00	most recent version
*    21.Aug.1998	V 1.00	initial version
*  NOTES
*
*******
*/

//-- includes -----------------------------------------------------------------

//-- Ansi C/C++

#include <ctype.h>
#include <dos.h>
#include <math.h>
#include <m68881.h>			// include math-extensions depending on type of math beeing used
#include <mffp.h>
#include <mieeedoub.h>
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
#include <libraries/unitconv.h>

#include <clib/alib_protos.h>			/* Prototypes for Supportfunctions */

#include <proto/dos.h>					/* Prototypes for Libraryfunctions */
#include <proto/exec.h>
#include <proto/configio.h>

//-- Local

#include "project.h"

//-- Debugtools

#define DEBUG_SRCPATH "data:source/sfx/convertprefs/"
#include <debug/dt_decl.h>

//-- definitions --------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
