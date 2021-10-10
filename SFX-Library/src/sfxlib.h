/****h* sfxsupport.library/sfxlib.h [4.3] *
*
*  NAME
*    sfxlib.h
*  COPYRIGHT
*    $VER: sfxlib.h 4.3 (31.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    global include for sfxsupport library
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    15.Jul.1998
*  MODIFICATION HISTORY
*    31.Oct.2003	V 4.3	most recent version
*							A: image handling
*    07.Jul.2003	V 4.2	most recent version
*    22.Aug.2000	V 4.1	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    15.Jul.1998	V 1.00	initial version
*  NOTES
*
*******
*/

//-- includes -----------------------------------------------------------------

#define SFXLibrary

#include <libraries/sfxsupport.h>

//-- Local

#include "project.h"

//-- DebugTools

#define DEBUG_SRCPATH "project:sfx-library/"
#include <debug/dt_decl.h>

//-- definitions --------------------------------------------------------------

#ifdef AMIGA
	//#define LIB(a) LIB##a
	#define LIB_EXPORT(f)	LIB##f
	#define LIB_INTCALL(f)	LIB##f
#else
	//#define LIB ##a
	#define LIB_EXPORT(f)	f
	#define LIB_INTCALL(f)	f
#endif

#ifdef AMIGA
	#include "lib-amiga.h"
	#include "asyncio.h"
#endif
#include "bshape.h"
#include "bshape-curve.h"
#include "bshape-cycle.h"
#include "bshape-none.h"
#include "bshape-user.h"
#include "bshape-vector.h"
#ifdef AMIGA
	#include "bshapegui-curve.h"
	#include "bshapegui-cycle.h"
	#include "bshapegui-none.h"
	#include "bshapegui-user.h"
	#include "bshapegui-vector.h"
	#include "errormsg.h"
	#include "fft.h"
	#include "fxobj-cfgsel.h"
	#include "fxobj-inter.h"
	#include "fxobj-mod.h"
	#include "fxobj-src.h"
	#include "fxobj-wfkt.h"
	#include "fxtools.h"
	#include "fxtools-cfg.h"
	#include "fxtools-gui.h"
	#include "help.h"
	#include "image.h"
	#include "interpol.h"
	#include "loat.h"
	#include "locale.h"
	#include "misc.h"
	#include "mem.h"
	#include "msg.h"
	#include "mwin.h"
	#include "packt.h"
	#include "prowin.h"
	#include "rbuf.h"
	#include "req-interpol.h"
	#include "req-lstentry.h"
	#include "req-period.h"
	#include "req-wfkt.h"
	#include "savt.h"
	#include "sound.h"
	#include "sound-mem.h"
	#include "stringt.h"
	#include "vm.h"
#endif
//-- eof ----------------------------------------------------------------------
