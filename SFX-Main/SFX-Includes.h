/****h* SoundFX/SFX-Includes.h [4.2] *
*
*  NAME
*    SFX-Includes.h
*  COPYRIGHT
*    $VER: SFX-Includes.h 4.2 115.12002
*  FUNCTION
*    Global Include File
*  AUTHOR
*    Stefan Kost            sk      kost@imn.htwk-leipzig.de
*  CREATION DATE
*    06.Aug.1998
*  MODIFICATION HISTORY
*    15.Nov.2002    V 4.2   most recent version
*    23.Feb.2001    V 4.1   most recent version
*    02.Aug.2000    V 4.1   most recent version
*                           * added sfx-error.h
*    01.Aug.2000    V 4.0   most recent version
*                           * added range-menu constants
*    05.Feb.2000    V 3.82  most recent version
*    02.Mar.1999    V 3.80  most recent version
*    26.Sep.1998    V 3.71  most recent version
*    24.Aug.1998    V 3.70  initial version
*  NOTES
*
*******
*/

//-- includes -----------------------------------------------------------------

#define SFXMain

//-- Ansi C/C++

#include <ctype.h>
#include <dos.h>
#include <math.h>
#include <m68881.h>         // include math-extensions depending on type of math beeing used
#include <mffp.h>
#include <mieeedoub.h>
#define PIM2    6.28318530717958647692
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
#include <devices/ahi.h>
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
#include <intuition/imageclass.h>
#include <intuition/icclass.h>
#include <intuition/pointerclass.h>
#include <intuition/screens.h>
#include <intuition/sghooks.h>
#include <libraries/delfina.h>
#include <libraries/identify.h>
#include <libraries/listtools.h>
#include <libraries/pm.h>
#include <libraries/prelude.h>
#include <libraries/repulse.h>
#include <libraries/sfxmod.h>
#include <libraries/sfxsupport.h>
#include <libraries/unitconv.h>
#include <prefs/pointer.h>
#include <rexx/errors.h>
#include <rexx/rxslib.h>
#include <utility/date.h>
#include <utility/hooks.h>
#include <utility/tagitem.h>

#include <clib/alib_protos.h>           /* Prototypes for Supportfunctions */

#include <proto/ahi.h>                  /* Prototypes for Libraryfunctions */
#include <proto/asl.h>
#include <proto/configio.h>
#include <proto/delfina.h>
#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/expansion.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/icon.h>
#include <proto/identify.h>
#include <proto/intuition.h>
#include <proto/listtools.h>
#include <proto/pm.h>
#include <proto/prelude.h>
#include <proto/repulse.h>
#include <proto/rexxsyslib.h>
#include <proto/sfxmod.h>
#include <proto/sfxsupport.h>
#include <proto/sfxkey.h>
#include <proto/unitconv.h>
#include <proto/utility.h>
#include <proto/wb.h>

#define __CXM33 __UCXM33                /* für Utillitie.library */
#define __CXD33 __UCXD33
#define __CXM22 __UCXM22
#define __CXD22 __UCXD22

//-- Local

#include "project.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-main/"
#include <debug/dt_decl.h>

//-- shutdown levels on initial fail --------------------------------------------------------------

#define CLLEV_LIBS      0
#define CLLEV_SERVER    1
#define CLLEV_GUI       2
#define CLLEV_ALL       100                 /* alles zu machen */

//-- for players ----------------------------------------------------------------------------------

// latency : (2048*1000)/44100=46.44 ms
#define MIN_PLAYBUF_SAMPLES     2048
#define MIN_PLAYBUF_BYTES       Samples2Bytes(MIN_PLAYBUF_SAMPLES)
// how much bigger do we make our AHI buffers
#define AHI_SHIFT 5

#define PLAY_OFF 0
#define PLAY_ALL 1
#define PLAY_RNG 2

//-- Drawmodes ------------------------------------------------------------------------------------

#define DRAWMODES       7
#define DM_LINES        0
#define DM_DOTS         1
#define DM_DOTSABS      2
#define DM_DOTSHQ       3
#define DM_FILLED       4
#define DM_FILLEDABS    5
#define DM_FILLEDHQ     6

//-- what is the keyfile status -------------------------------------------------------------------

//-- duplicate is in "SFX-Module/libraries/sfxmod.h"

#define STAT_DEMO   0
#define STAT_AUTHOR 1
#define STAT_TESTER 2
#define STAT_USER   3
#define STAT_ILLEGAL 255

// CheckKeyFile1()  returns the direct status of the keyfile (STAT_ILLEGAL for errors)
// CheckKeyFile2()  returns 0 on success and 0xFF on failure
// CheckKeyFile3()  returns 0 on success and 0xFF on failure
// CheckKeyFile4()  returns 0xFF on success and 0 on failure

#define SetKeyStatus_0  RunTime.status=CheckKeyFile1()
#define SetKeyStatus_1  RunTime.status=CheckKeyFile1()&CheckKeyFile2()
#define SetKeyStatus_2  RunTime.status=RunTime.status2>>3
#define SetKeyStatus_3  RunTime.status&=CheckKeyFile2()
#define SetKeyStatus_4  RunTime.status=CheckKeyFile1()&CheckKeyFile4()
#define SetKeyStatus_5  RunTime.status|=CheckKeyFile3()

#define SetKeyStatus2_0 RunTime.status2=CheckKeyFile1()<<3
#define SetKeyStatus2_1 RunTime.status2=RunTime.status<<3

#define SetKeyStatus3_0 RunTime.status3=255-(RunTime.status2<<1)


#define IsKeyStatusOkay (RunTime.status!=STAT_DEMO && RunTime.status!=STAT_ILLEGAL && !RunTime.keyerr)

#define IsKeyStatusIllegal_1 (RunTime.status!=(RunTime.status2>>3))
#define IsKeyStatusIllegal_2 (RunTime.status2!=((255-RunTime.status3)>>1))


//-- what kind of keyfile checking ----------------------------------------------------------------

#define CHECK_REG
#define CHECK_KEY
//#define CHECK_DELFINA
//#define CHECK_PRELUDE
//#define CHECK_REPULSE

// comment this out for releases !!!!!!
#define CHECK_NOCRASH
// comment this out for releases !!!!!!

//-- the old key ----------------------------------------------------------------------------------

typedef struct
{
	char    line0[40];
	UWORD   chkh0;
	char    line1[40];
	UWORD   chkh1;
	char    line2[40];
	UWORD   chkh2;
	char    line3[40];
	UWORD   chkh3;
	char    line4[40];
	UWORD   chkh4;
	char    line5[40];
	UWORD   chkh5;
	char    line6[40];
	UWORD   chkh6;
	char    line7[40];
	UWORD   chkh7;
	char    line8[40];
	UWORD   chkh8;
	char    line9[40];
	UWORD   chkh9;
	UWORD   chkv[42];
} KeyFile;

//-- menu id's ------------------------------------------------------------------------------------

#define MAIN_MENID_PROJECT         0
#define MAIN_MENID_PRJ_NEW         (MAIN_MENID_PROJECT+ 0)
#define MAIN_MENID_PRJ_LOAD        (MAIN_MENID_PROJECT+ 1)
#define MAIN_MENID_PRJ_SAVE        (MAIN_MENID_PROJECT+ 2)
#define MAIN_MENID_PRJ_FLUSHALL    (MAIN_MENID_PROJECT+ 3)
#define MAIN_MENID_PRJ_FLUSHHIDDEN (MAIN_MENID_PROJECT+ 4)
#define MAIN_MENID_PRJ_FLUSHSHOWN  (MAIN_MENID_PROJECT+ 5)
#define MAIN_MENID_PRJ_RUNOP       (MAIN_MENID_PROJECT+ 6)
#define MAIN_MENID_PRJ_RUNRX       (MAIN_MENID_PROJECT+ 7)
#define MAIN_MENID_PRJ_PLAYALL     (MAIN_MENID_PROJECT+ 8)
#define MAIN_MENID_PRJ_PLAYRNG     (MAIN_MENID_PROJECT+ 9)
#define MAIN_MENID_PRJ_PLAYSTOP    (MAIN_MENID_PROJECT+10)
#define MAIN_MENID_PRJ_RECORD      (MAIN_MENID_PROJECT+11)
#define MAIN_MENID_PRJ_BATCHPROC   (MAIN_MENID_PROJECT+12)
#define MAIN_MENID_PRJ_INFO        (MAIN_MENID_PROJECT+13)
#define MAIN_MENID_PRJ_MRU1        (MAIN_MENID_PROJECT+14)
#define MAIN_MENID_PRJ_MRU2        (MAIN_MENID_PROJECT+15)
#define MAIN_MENID_PRJ_MRU3        (MAIN_MENID_PROJECT+16)
#define MAIN_MENID_PRJ_MRU4        (MAIN_MENID_PROJECT+17)
#define MAIN_MENID_PRJ_MRU5        (MAIN_MENID_PROJECT+18)
#define MAIN_MENID_PRJ_QUIT        (MAIN_MENID_PROJECT+19)

#define MAIN_MENID_EDIT            (MAIN_MENID_PRJ_QUIT+1)
#define MAIN_MENID_EDIT_CUT        (MAIN_MENID_EDIT+ 0)
#define MAIN_MENID_EDIT_CUTB       (MAIN_MENID_EDIT+ 1)
#define MAIN_MENID_EDIT_CUTE       (MAIN_MENID_EDIT+ 2)
#define MAIN_MENID_EDIT_COPY       (MAIN_MENID_EDIT+ 3)
#define MAIN_MENID_EDIT_COPYB      (MAIN_MENID_EDIT+ 4)
#define MAIN_MENID_EDIT_COPYE      (MAIN_MENID_EDIT+ 5)
#define MAIN_MENID_EDIT_GRAB       (MAIN_MENID_EDIT+ 6)
#define MAIN_MENID_EDIT_GRABB      (MAIN_MENID_EDIT+ 7)
#define MAIN_MENID_EDIT_GRABE      (MAIN_MENID_EDIT+ 8)
#define MAIN_MENID_EDIT_PASTE      (MAIN_MENID_EDIT+ 9)
#define MAIN_MENID_EDIT_PASTEB     (MAIN_MENID_EDIT+10)
#define MAIN_MENID_EDIT_PASTEE     (MAIN_MENID_EDIT+11)
#define MAIN_MENID_EDIT_ERASE      (MAIN_MENID_EDIT+12)
#define MAIN_MENID_EDIT_ERASEB     (MAIN_MENID_EDIT+13)
#define MAIN_MENID_EDIT_ERASEE     (MAIN_MENID_EDIT+14)
#define MAIN_MENID_EDIT_ZERO       (MAIN_MENID_EDIT+15)
#define MAIN_MENID_EDIT_ZEROB      (MAIN_MENID_EDIT+16)
#define MAIN_MENID_EDIT_ZEROE      (MAIN_MENID_EDIT+17)
#define MAIN_MENID_EDIT_OVERWRITE  (MAIN_MENID_EDIT+18)
#define MAIN_MENID_EDIT_OVERWRITEB (MAIN_MENID_EDIT+19)
#define MAIN_MENID_EDIT_EXCHANGE   (MAIN_MENID_EDIT+20)
#define MAIN_MENID_EDIT_REPLACE    (MAIN_MENID_EDIT+21)

#define MAIN_MENID_RANGE           (MAIN_MENID_EDIT_REPLACE+1)
#define MAIN_MENID_RNG_LOOP2MARK   (MAIN_MENID_RANGE+ 0)
#define MAIN_MENID_RNG_MARK2LOOP   (MAIN_MENID_RANGE+ 1)
#define MAIN_MENID_RNG_ZOOM2LOOP   (MAIN_MENID_RANGE+ 2)
#define MAIN_MENID_RNG_ZOOM2MARK   (MAIN_MENID_RANGE+ 3)
#define MAIN_MENID_RNG_MARKOFF     (MAIN_MENID_RANGE+ 4)
#define MAIN_MENID_RNG_MARKALL     (MAIN_MENID_RANGE+ 5)

#define MAIN_MENID_ZOOM            (MAIN_MENID_RNG_MARKALL+1)
#define MAIN_MENID_ZOOM_IN         (MAIN_MENID_ZOOM+ 0)
#define MAIN_MENID_ZOOM_OUT        (MAIN_MENID_ZOOM+ 1)
#define MAIN_MENID_ZOOM_PIXEL      (MAIN_MENID_ZOOM+ 2)
#define MAIN_MENID_ZOOM_FULL       (MAIN_MENID_ZOOM+ 3)

#define MAIN_MENID_CLEANUP         (MAIN_MENID_ZOOM_FULL+1)
#define MAIN_MENID_CL_CURR         (MAIN_MENID_CLEANUP+ 0)
#define MAIN_MENID_CL_ALL          (MAIN_MENID_CLEANUP+ 1)
#define MAIN_MENID_CL_ALLNORM      (MAIN_MENID_CLEANUP+ 2)
#define MAIN_MENID_CL_ALLZOOM      (MAIN_MENID_CLEANUP+ 3)

#define MAIN_MENID_UTILITIES       (MAIN_MENID_CL_ALLZOOM+1)
#define MAIN_MENID_UT_SWAPEND      (MAIN_MENID_UTILITIES+ 0)
#define MAIN_MENID_UT_SWAPSIGN     (MAIN_MENID_UTILITIES+ 1)
#define MAIN_MENID_UT_INTERLEAVE   (MAIN_MENID_UTILITIES+ 2)
#define MAIN_MENID_UT_DEINTERLEAVE (MAIN_MENID_UTILITIES+ 3)

#define MAIN_MENID_PREFS				(MAIN_MENID_UT_DEINTERLEAVE+1)
#define MAIN_MENID_PREFS_GUI			(MAIN_MENID_PREFS+ 0)
#define MAIN_MENID_PREFS_SAMP			(MAIN_MENID_PREFS+ 1)
#define MAIN_MENID_PREFS_VMEM			(MAIN_MENID_PREFS+ 2)
#define MAIN_MENID_PREFS_MISC			(MAIN_MENID_PREFS+ 3)
#define MAIN_MENID_PREFS_USE			(MAIN_MENID_PREFS+ 4)
#define MAIN_MENID_PREFS_SAVE			(MAIN_MENID_PREFS+ 5)
#define MAIN_MENID_PREFS_RESTOREUSED	(MAIN_MENID_PREFS+ 6)
#define MAIN_MENID_PREFS_RESTORESAVED	(MAIN_MENID_PREFS+ 7)
#define MAIN_MENID_PREFS_DEFAULT		(MAIN_MENID_PREFS+ 8)

#define MAIN_MENID_HELP            (MAIN_MENID_PREFS_DEFAULT+1)
#define MAIN_MENID_HELP_CT         (MAIN_MENID_HELP+ 0)
#define MAIN_MENID_HELP_IX         (MAIN_MENID_HELP+ 1)
#define MAIN_MENID_HELP_WS         (MAIN_MENID_HELP+ 2)
#define MAIN_MENID_HELP_ONLINE     (MAIN_MENID_HELP+ 3)
#define MAIN_MENID_HELP_MAIL       (MAIN_MENID_HELP+ 4)
#define MAIN_MENID_HELP_SHOWTIPS   (MAIN_MENID_HELP+ 5)
#define MAIN_MENID_HELP_ABOUT      (MAIN_MENID_HELP+ 6)

#define MAIN_MENIX_PROJECT   0
#define MAIN_MENIX_EDIT      (MAIN_MENIX_PROJECT  +29)
#define MAIN_MENIX_RANGE     (MAIN_MENIX_EDIT     +24)
#define MAIN_MENIX_ZOOM      (MAIN_MENIX_RANGE    + 8)
#define MAIN_MENIX_CLEANUP   (MAIN_MENIX_ZOOM     + 5)
#define MAIN_MENIX_UTILITIES (MAIN_MENIX_CLEANUP  + 5)
#define MAIN_MENIX_PREFS     (MAIN_MENIX_UTILITIES+ 6)
#define MAIN_MENIX_HELP      (MAIN_MENIX_PREFS    +12)
//                           (                    +10)

#define MENID_LOAD       0
#define MENID_SAVE       1
#define MENID_RESET      2
#define MENID_DEFAULT    3
#define MENID_GO         4
#define MENID_HELP       5
#define MENID_ABOUT      6
#define MENID_QUIT       7

#define MOD_MENIX_PROJECT   0
#define MOD_MENIX_HELP      (MOD_MENIX_PROJECT+9)

//-- toolbar id's ---------------------------------------------------------------------------------

#define GADID_TB_LOAD       0
#define GADID_TB_CHLO       (GADID_TB_LOAD+1)
#define GADID_TB_PREFLO     (GADID_TB_CHLO+1)

#define GADID_TB_SAVE       (GADID_TB_PREFLO+1)
#define GADID_TB_CHSA       (GADID_TB_SAVE+1)
#define GADID_TB_PREFSA     (GADID_TB_CHSA+1)

#define GADID_TB_RUNFX      (GADID_TB_PREFSA+1)
#define GADID_TB_CHFX       (GADID_TB_RUNFX+1)

#define GADID_TB_RUNREXX    (GADID_TB_CHFX+1)
#define GADID_TB_CHREXX     (GADID_TB_RUNREXX+1)

#define GADID_TB_PLAYALL    (GADID_TB_CHREXX+1)
#define GADID_TB_PLAYRNG    (GADID_TB_PLAYALL+1)
#define GADID_TB_STOP       (GADID_TB_PLAYRNG+1)
#define GADID_TB_RECORD     (GADID_TB_STOP+1)
#define GADID_TB_CHPL       (GADID_TB_RECORD+1)
#define GADID_TB_PREFPL     (GADID_TB_CHPL+1)

#define GADID_TB_CHBUF      (GADID_TB_PREFPL+1)
#define GADID_TB_PREFBUF    (GADID_TB_CHBUF+1)
#define GADID_TB_HIDESHOW   (GADID_TB_PREFBUF+1)

#define GADID_TB_CUTMEN     (GADID_TB_HIDESHOW+1)
#define GADID_TB_COPYMEN    (GADID_TB_CUTMEN+1)
#define GADID_TB_PASTEMEN   (GADID_TB_COPYMEN+1)
#define GADID_TB_ERASEMEN   (GADID_TB_PASTEMEN+1)
#define GADID_TB_GRABMEN    (GADID_TB_ERASEMEN+1)
#define GADID_TB_ZEROMEN    (GADID_TB_GRABMEN+1)
#define GADID_TB_OVERWRMEN  (GADID_TB_ZEROMEN+1)

#define GADID_TB_ZOOMMODE   (GADID_TB_OVERWRMEN+1)
#define GADID_TB_ZOOMIN     (GADID_TB_ZOOMMODE+1)
#define GADID_TB_ZOOMOUT    (GADID_TB_ZOOMIN+1)
#define GADID_TB_ZOOMPIXEL  (GADID_TB_ZOOMOUT+1)
#define GADID_TB_ZOOMFULL   (GADID_TB_ZOOMPIXEL+1)

#define GADID_TB_RNGMODE    (GADID_TB_ZOOMFULL+1)
#define GADID_TB_LOCKLEN    (GADID_TB_RNGMODE+1)
#define GADID_TB_MOVEMODE   (GADID_TB_LOCKLEN+1)
#define GADID_TB_MOVEBEG    (GADID_TB_MOVEMODE+1)
#define GADID_TB_MOVELEFTF  (GADID_TB_MOVEBEG+1)
#define GADID_TB_MOVELEFTS  (GADID_TB_MOVELEFTF+1)
#define GADID_TB_MOVELEFTN  (GADID_TB_MOVELEFTS+1)
#define GADID_TB_MOVERIGHTN (GADID_TB_MOVELEFTN+1)
#define GADID_TB_MOVERIGHTS (GADID_TB_MOVERIGHTN+1)
#define GADID_TB_MOVERIGHTF (GADID_TB_MOVERIGHTS+1)
#define GADID_TB_MOVEEND    (GADID_TB_MOVERIGHTF+1)
#define GADID_TB_MOVETOP    (GADID_TB_MOVEEND+1)
#define GADID_TB_MOVEUPF    (GADID_TB_MOVETOP+1)
#define GADID_TB_MOVEUPS    (GADID_TB_MOVEUPF+1)
#define GADID_TB_MOVEUPP    (GADID_TB_MOVEUPS+1)
#define GADID_TB_MOVEDOWNP  (GADID_TB_MOVEUPP+1)
#define GADID_TB_MOVEDOWNS  (GADID_TB_MOVEDOWNP+1)
#define GADID_TB_MOVEDOWNF  (GADID_TB_MOVEDOWNS+1)
#define GADID_TB_MOVEBOTTOM (GADID_TB_MOVEDOWNF+1)

#define GADID_TB_WINMODE    (GADID_TB_MOVEBOTTOM+1)


#define GADID_DD_LIST       0

#define GADID_DD_CUT        (GADID_DD_LIST+1)
#define GADID_DD_CUTB       (GADID_DD_CUT+1)
#define GADID_DD_CUTE       (GADID_DD_CUTB+1)

#define GADID_DD_COPY       (GADID_DD_CUTE+1)
#define GADID_DD_COPYB      (GADID_DD_COPY+1)
#define GADID_DD_COPYE      (GADID_DD_COPYB+1)

#define GADID_DD_PASTE      (GADID_DD_COPYE+1)
#define GADID_DD_PASTEB     (GADID_DD_PASTE+1)
#define GADID_DD_PASTEE     (GADID_DD_PASTEB+1)

#define GADID_DD_ERASE      (GADID_DD_PASTEE+1)
#define GADID_DD_ERASEB     (GADID_DD_ERASE+1)
#define GADID_DD_ERASEE     (GADID_DD_ERASEB+1)

#define GADID_DD_GRAB       (GADID_DD_ERASEE+1)
#define GADID_DD_GRABB      (GADID_DD_GRAB+1)
#define GADID_DD_GRABE      (GADID_DD_GRABB+1)

#define GADID_DD_ZERO       (GADID_DD_GRABE+1)
#define GADID_DD_ZEROB      (GADID_DD_ZERO+1)
#define GADID_DD_ZEROE      (GADID_DD_ZEROB+1)

#define GADID_DD_OVERWR     (GADID_DD_ZEROE+1)
#define GADID_DD_OVERWRB    (GADID_DD_OVERWR+1)

#define GADIX_TB_LO         00
#define GADIX_TB_LOADER     (GADIX_TB_LO+1)
#define GADIX_TB_CHLOADER   (GADIX_TB_LO+2)
#define GADIX_TB_PREFLO     (GADIX_TB_LO+3)
#define GADIX_TB_LOAD       (GADIX_TB_LO+4)

#define GADIX_TB_SA         05
#define GADIX_TB_SAVER      (GADIX_TB_SA+1)
#define GADIX_TB_CHSAVER    (GADIX_TB_SA+2)
#define GADIX_TB_PREFSA     (GADIX_TB_SA+3)
#define GADIX_TB_SAVE       (GADIX_TB_SA+4)

#define GADIX_TB_OP         10
#define GADIX_TB_OPERATOR   (GADIX_TB_OP+1)
#define GADIX_TB_CHOP       (GADIX_TB_OP+2)
#define GADIX_TB_RUNOP      (GADIX_TB_OP+3)

#define GADIX_TB_RX         14
#define GADIX_TB_REXX       (GADIX_TB_RX+1)
#define GADIX_TB_CHREXX     (GADIX_TB_RX+2)
#define GADIX_TB_RUNREXX    (GADIX_TB_RX+3)

#define GADIX_TB_PL         18
#define GADIX_TB_PLAYER     (GADIX_TB_PL+1)
#define GADIX_TB_CHPLAYER   (GADIX_TB_PL+2)
#define GADIX_TB_PREFPL     (GADIX_TB_PL+3)
#define GADIX_TB_PLAYALL    (GADIX_TB_PL+4)
#define GADIX_TB_PLAYRNG    (GADIX_TB_PL+5)
#define GADIX_TB_STOP       (GADIX_TB_PL+6)
#define GADIX_TB_RECORD     (GADIX_TB_PL+7)

#define GADIX_TB_BUF        26
#define GADIX_TB_BUFFER     (GADIX_TB_BUF+1)
#define GADIX_TB_CHBUF      (GADIX_TB_BUF+2)
#define GADIX_TB_PREFBUF    (GADIX_TB_BUF+3)
#define GADIX_TB_HIDESHOW   (GADIX_TB_BUF+4)

#define GADIX_TB_EDIT       31
#define GADIX_TB_CUTMEN     (GADIX_TB_EDIT+1)
#define GADIX_TB_COPYMEN    (GADIX_TB_EDIT+2)
#define GADIX_TB_PASTEMEN   (GADIX_TB_EDIT+3)
#define GADIX_TB_ERASEMEN   (GADIX_TB_EDIT+4)
#define GADIX_TB_GRABMEN    (GADIX_TB_EDIT+5)
#define GADIX_TB_ZEROMEN    (GADIX_TB_EDIT+6)
#define GADIX_TB_OVERWRMEN  (GADIX_TB_EDIT+7)

#define GADIX_TB_ZOOM       39
#define GADIX_TB_ZOOMMODE   (GADIX_TB_ZOOM+1)
#define GADIX_TB_ZOOMIN     (GADIX_TB_ZOOM+2)
#define GADIX_TB_ZOOMOUT    (GADIX_TB_ZOOM+3)
#define GADIX_TB_ZOOMPIXEL  (GADIX_TB_ZOOM+4)
#define GADIX_TB_ZOOMFULL   (GADIX_TB_ZOOM+5)

#define GADIX_TB_RNGMODE    46
#define GADIX_TB_LOCKLEN    (GADIX_TB_RNGMODE+1)
#define GADIX_TB_MOVEMODE   (GADIX_TB_LOCKLEN+1)
#define GADIX_TB_MOVEBEG    (GADIX_TB_MOVEMODE+1)
#define GADIX_TB_MOVELEFTF  (GADIX_TB_MOVEBEG+1)
#define GADIX_TB_MOVELEFTS  (GADIX_TB_MOVELEFTF+1)
#define GADIX_TB_MOVELEFTN  (GADIX_TB_MOVELEFTS+1)
#define GADIX_TB_MOVERIGHTN (GADIX_TB_MOVELEFTN+1)
#define GADIX_TB_MOVERIGHTS (GADIX_TB_MOVERIGHTN+1)
#define GADIX_TB_MOVERIGHTF (GADIX_TB_MOVERIGHTS+1)
#define GADIX_TB_MOVEEND    (GADIX_TB_MOVERIGHTF+1)
#define GADIX_TB_MOVETOP    (GADIX_TB_MOVEEND+1)
#define GADIX_TB_MOVEUPF    (GADIX_TB_MOVETOP+1)
#define GADIX_TB_MOVEUPS    (GADIX_TB_MOVEUPF+1)
#define GADIX_TB_MOVEUPP    (GADIX_TB_MOVEUPS+1)
#define GADIX_TB_MOVEDOWNP  (GADIX_TB_MOVEUPP+1)
#define GADIX_TB_MOVEDOWNS  (GADIX_TB_MOVEDOWNP+1)
#define GADIX_TB_MOVEDOWNF  (GADIX_TB_MOVEDOWNS+1)
#define GADIX_TB_MOVEBOTTOM (GADIX_TB_MOVEDOWNF+1)

#define GADIX_TB_WINMODE    66


#define SFXTB_END           67
//#####################

//-- window id's ----------------------------------------------------------------------------------

#define WIN_OP          0x0001
#define WIN_RXOP        0x0002
#define WIN_LO          0x0004
#define WIN_SA          0x0008
#define WIN_PL          0x0010
#define WIN_BUF         0x0020
#define WIN_ED          0x0040
#define WIN_RNG         0x0080
#define WIN_ZO          0x0100

#define REF_ALL         WIN_OP|WIN_RXOP|WIN_SA|WIN_PL|WIN_BUF|WIN_ED|WIN_RNG|WIN_ZO
#define REF_FLSAMPLE    WIN_SA|WIN_PL|WIN_BUF|WIN_ED|WIN_RNG|WIN_ZO

#define SMPSTAT_UPDATE      0
#define SMPSTAT_FIRSTNEW    1
#define SMPSTAT_LASTOLD     2

//-- state of range window ------------------------------------------------------------------------

#define RNGMD_LOOP      0
#define RNGMD_MARK      1
#define RNGMD_ZOOM      2
#define RNGMD_TRACE     3

//-- range move -----------------------------------------------------------------------------------

#define RNGMOVE_NONE	0
#define RNGMOVE_X       1
#define RNGMOVE_Y       2

//-- zoom mode ------------------------------------------------------------------------------------

#define ZOOMMD_X        1
#define ZOOMMD_Y        2
#define ZOOMMD_BOTH     (ZOOMMD_X|ZOOMMD_Y)


//-- Info structure for each module ---------------------------------------------------------------

typedef struct
{
	UBYTE   gui;                    // does the modul got a gui (loader)
	UBYTE   fsel;                   // should I invoke it with an filerequester (loader/saver)
	UBYTE   ext[10][10];            // registered extensions (universal loader)
	ULONG   magic_off[5];           // registered magic cookies (universal loader)
	UBYTE   magic_str[5][5][30];
	UBYTE   description[100];       // modul description
} ModInfo;

#define MODINFO_NAMELEN 40-7        // 40 chars max filenamelen - 7 chars extension (.sfxmod)

typedef struct
{
	struct Node node;
	ModInfo     info;
	char        name[MODINFO_NAMELEN+1];        // node name
} ModNode;

//-- types of edit-operation ----------------------------------------------------------------------

#define EDIT_RANGE  0
#define EDIT_2BEGIN 1
#define EDIT_2END   2

//-- Key check errors -----------------------------------------------------------------------------

#define KEYERR_INVALID_KEY           1
#define KEYERR_KEYLIST_MODIFIED      2
#define KEYERR_ILLEGAL_KEY           4
#define KEYERR_ILLEGAL_OLD_KEY       8
#define KEYERR_NOT_IN_KEYLIST       16
#define KEYERR_KEYNR_HAS_CHANGED    32
#define KEYERR_INVALID_KEY_CONTENTS 64

//-- only used for player list --------------------------------------------------------------------

typedef struct {                        /* Zuweisungsliste */
	char name[50];                      /* unbedingt auf 50 lassen, wegen Bufferliste */
	WORD nr;
} ArrangeList;

//-- definitions --------------------------------------------------------------

#include "SFX.h"

#include "SFX-GUI.h"
#include "SFX-GUIKeys.h"
#include "SFX-GUIMenu.h"

#include "SFX-BootWin.h"

#include "SFX-Error.h"

#include "SFX-Edit.h"
#include "SFX-Range.h"
#include "SFX-Zoom.h"
#include "SFX-CleanUp.h"
#include "SFX-Utilities.h"

#include "SFX-HookListView.h"
#include "SFX-HookString.h"

#include "SFX-IDCMPCtrl.h"
#include "SFX-ModTools.h"

#include "SFX-IORexx.h"
#include "SFX-IOApp.h"

#include "SFX-Play.h"
#include "SFX-PlayAhi.h"
#include "SFX-PlayCaCa.h"
#include "SFX-PlayCasc.h"
#include "SFX-PlayNorm.h"

#include "SFX-PlayOpts.h"

#include "SFX-Pointers.h"

#include "SFX-Prefs.h"
#include "SFX-PrefsGUI.h"
#include "SFX-PrefsMisc.h"
#include "SFX-PrefsSamp.h"
#include "SFX-PrefsVMem.h"

#include "SFX-ModLoader.h"
#include "SFX-ModOperator.h"
#include "SFX-ModRexx.h"
#include "SFX-ModSaver.h"

#include "SFX-SmpGfx.h"
#include "SFX-SmpUnits.h"

#include "SFX-Tools.h"
#include "SFX-ToolsGUI.h"
#include "SFX-ToolsList.h"
#include "SFX-ToolsRange.h"

#include "SFX-WinBackdrop.h"
#include "SFX-WinBatchproc.h"
#include "SFX-WinInfo.h"
#include "SFX-WinNew.h"
#include "SFX-WinRecord.h"
#include "SFX-WinSample.h"
#include "SFX-WinStatus.h"
#include "SFX-WinSmpOpts.h"
#include "SFX-WinToolbar.h"

#include "SFX-CrashMark.h"

#include "SFX-MRU.h"

#include "SFX-CheckOldKey1.h"               // old keyfile routines
#include "SFX-CheckOldKey2.h"
#include "SFX-CheckSum1.h"

#include "SFX-CheckKey1.h"                  // new keyfile routines
#include "SFX-CheckKey2.h"
#include "SFX-CheckKey3.h"
#include "SFX-CheckKey4.h"

#include "SFX-CheckKeyContents.h"           // keyfile routines
#include "SFX-CheckKeyList.h"
#include "SFX-CheckKeyHelper.h"

#include "SFX-Filedefs.h"                   // fileformat constants

//include via libraries/sfxsupport.h
//#include "SFX-Optstructs.h"
//#include "SFX-Images.h"
//#include "SFX-LocaleID.h"

//-- eof ----------------------------------------------------------------------
