/****h* SoundFX/SFX-GUI.c [4.3] *
*
*  NAME
*    SFX-GUI.c
*  COPYRIGHT
*    $VER: SFX-GUI.c 4.3 S13.10.03st 1998-2003
*  FUNCTION
*    gui init and done routines - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    13.Oct.2003	V 4.3	most recent version
*                           * DriPens for OS4
*    21.Feb.2003	V 4.2	most recent version
*                           A: main menu enable/disable
*    11.Jan.2001    V 4.1   most recent version
*    17.Jul.2000    V 4.0   most recent version
*                           A: new menu entries
*    05.Feb.2000    V 3.82  most recent version
*    08.Aug.1999    V 3.81  most recent version
*    02.Mar.1999    V 3.80  most recent version
*    23.Sep.1998    V 3.71  most recent version
*                           * closing subwindow with new members
*    08.Aug.1998    V 3.70  release
*    19.Jul.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#define SFX_GUI_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

#include "toolbar/imgNew.h"
#include "toolbar/imgLoad.h"
#include "toolbar/imgSave.h"
#include "toolbar/imgRunFX.h"
#include "toolbar/imgRunRexx.h"

#include "toolbar/imgDropDown.h"
#include "toolbar/imgPrefs.h"

#include "toolbar/imgCutMen.h"
#include "toolbar/imgCut.h"
#include "toolbar/imgCutL.h"
#include "toolbar/imgCutR.h"
#include "toolbar/imgCopyMen.h"
#include "toolbar/imgCopy.h"
#include "toolbar/imgCopyL.h"
#include "toolbar/imgCopyR.h"
#include "toolbar/imgPasteMen.h"
#include "toolbar/imgPaste.h"
#include "toolbar/imgPasteL.h"
#include "toolbar/imgPasteR.h"
#include "toolbar/imgEraseMen.h"
#include "toolbar/imgErase.h"
#include "toolbar/imgEraseL.h"
#include "toolbar/imgEraseR.h"
#include "toolbar/imgGrabMen.h"
#include "toolbar/imgGrab.h"
#include "toolbar/imgGrabL.h"
#include "toolbar/imgGrabR.h"
#include "toolbar/imgZeroMen.h"
#include "toolbar/imgZero.h"
#include "toolbar/imgZeroL.h"
#include "toolbar/imgZeroR.h"
#include "toolbar/imgOverwrMen.h"
#include "toolbar/imgOverwr.h"
#include "toolbar/imgOverwrL.h"
#include "toolbar/imgOverwrR.h"

#include "toolbar/imgPlayAll.h"
#include "toolbar/imgPlayRng.h"
#include "toolbar/imgStop.h"
#include "toolbar/imgRecord.h"

#include "toolbar/imgHidden.h"
#include "toolbar/imgVisible.h"

#include "toolbar/imgZoomBoth.h"
#include "toolbar/imgZoomX.h"
#include "toolbar/imgZoomY.h"
#include "toolbar/imgZoomIn.h"
#include "toolbar/imgZoomOut.h"
#include "toolbar/imgZoomPixel.h"
#include "toolbar/imgZoomFull.h"

#include "toolbar/imgMoveBeg.h"
#include "toolbar/imgMoveLeftF.h"
#include "toolbar/imgMoveLeftS.h"
#include "toolbar/imgMoveLeftN.h"
#include "toolbar/imgMoveRightN.h"
#include "toolbar/imgMoveRightS.h"
#include "toolbar/imgMoveRightF.h"
#include "toolbar/imgMoveEnd.h"

#include "toolbar/imgMoveTop.h"
#include "toolbar/imgMoveUpF.h"
#include "toolbar/imgMoveUpS.h"
#include "toolbar/imgMoveUpP.h"
#include "toolbar/imgMoveDownP.h"
#include "toolbar/imgMoveDownS.h"
#include "toolbar/imgMoveDownF.h"
#include "toolbar/imgMoveBottom.h"

#include "toolbar/imgZoom.h"
#include "toolbar/imgMark.h"
#include "toolbar/imgLoop.h"
#include "toolbar/imgTrace.h"

#include "toolbar/imgLockOp.h"
#include "toolbar/imgLockCl.h"
#include "toolbar/imgChgBeg.h"
#include "toolbar/imgChgEnd.h"

#include "toolbar/imgWinM.h"
#include "toolbar/imgWinO.h"

//-- prototypes ---------------------------------------------------------------

//-- GUI Init, Done & ReOpen

void GUI_Init(void);
void GUI_Done(void);
void GUI_ReOpen(void);

//-- globals ------------------------------------------------------------------

struct Window *oldWinPtr=NULL;
UBYTE writeprefs=FALSE;		/* indicates that we have auto-adjusted prefs-settings, the users should be notified so that he can save the prefs */

//-- asl requests

struct FileRequester        *File_Request=NULL;
struct FontRequester        *Font_Request=NULL;
struct ScreenModeRequester  *ScrMd_Request=NULL;

//-- window input blocker

struct Requester            NullReq;

//-- Fonts

struct TextAttr ScrFont ={NULL,                    0,FS_NORMAL,FPF_DISKFONT};
struct TextAttr ScrFontB={NULL,                    0,FSF_BOLD, FPF_DISKFONT};
//struct TextAttr SigFont ={"TrinomicSign.font",   6,FS_NORMAL,FPF_DISKFONT};
struct TextAttr SigFont ={"ButtonGlyphMedium.font",6,FS_NORMAL,FPF_DISKFONT};
struct TextAttr ScaFont ={NULL,                    0,FS_NORMAL,FPF_DISKFONT};
struct TextAttr StatFont={NULL,                    0,FS_NORMAL,FPF_DISKFONT};

//-- Menus

struct NewMenu SFXMenu[]={
  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       "N",    0,                  0,  (APTR)MAIN_MENID_PRJ_NEW},
  /*  2 */  {  NM_ITEM, NULL,       "L",    0,                  0,  (APTR)MAIN_MENID_PRJ_LOAD},
  /*  3 */  {  NM_ITEM, NULL,       "S",    0,                  0,  (APTR)MAIN_MENID_PRJ_SAVE},
  /*  4 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  5 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)0},
  /*    */  {   NM_SUB, NULL,       "Del",  NM_COMMANDSTRING,   0,  (APTR)MAIN_MENID_PRJ_FLUSHALL},
  /*    */  {   NM_SUB, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PRJ_FLUSHHIDDEN},
  /*    */  {   NM_SUB, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PRJ_FLUSHSHOWN},
  /*  6 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  7 */  {  NM_ITEM, NULL,       "r",    0,                  0,  (APTR)MAIN_MENID_PRJ_RUNOP},
  /*  8 */  {  NM_ITEM, NULL,       "R",    0,                  0,  (APTR)MAIN_MENID_PRJ_RUNRX},
  /*  9 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /* 10 */  {  NM_ITEM, NULL,       "p",    0,                  0,  (APTR)MAIN_MENID_PRJ_PLAYALL},
  /* 11 */  {  NM_ITEM, NULL,       "P",    0,                  0,  (APTR)MAIN_MENID_PRJ_PLAYRNG},
  /* 12 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PRJ_PLAYSTOP},
  /* 13 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PRJ_RECORD},
  /* 14 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /* 15 */  {  NM_ITEM, NULL,       "B",    0,                  0,  (APTR)MAIN_MENID_PRJ_BATCHPROC},
  /* 16 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /* 17 */  {  NM_ITEM, NULL,       "I",    0,                  0,  (APTR)MAIN_MENID_PRJ_INFO},
  /* 18 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /* 19 */  {  NM_ITEM, MRU_MenuStr[0],"1", 0,                  0,  (APTR)MAIN_MENID_PRJ_MRU1},
  /* 20 */  {  NM_ITEM, MRU_MenuStr[1],"2", 0,                  0,  (APTR)MAIN_MENID_PRJ_MRU2},
  /* 21 */  {  NM_ITEM, MRU_MenuStr[2],"3", 0,                  0,  (APTR)MAIN_MENID_PRJ_MRU3},
  /* 22 */  {  NM_ITEM, MRU_MenuStr[3],"4", 0,                  0,  (APTR)MAIN_MENID_PRJ_MRU4},
  /* 23 */  {  NM_ITEM, MRU_MenuStr[4],"5", 0,                  0,  (APTR)MAIN_MENID_PRJ_MRU5},
  /* 24 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /* 25 */  {  NM_ITEM, NULL,       "Q",    0,                  0,  (APTR)MAIN_MENID_PRJ_QUIT},

  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       "X",    0,                  0,  (APTR)MAIN_MENID_EDIT_CUT},
  /*  2 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_CUTB},
  /*  3 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_CUTE},
  /*  4 */  {  NM_ITEM, NULL,       "C",    0,                  0,  (APTR)MAIN_MENID_EDIT_COPY},
  /*  5 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_COPYB},
  /*  6 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_COPYE},
  /*  7 */  {  NM_ITEM, NULL,       "G",    0,                  0,  (APTR)MAIN_MENID_EDIT_GRAB},
  /*  8 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_GRABB},
  /*  9 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_GRABE},
  /* 10 */  {  NM_ITEM, NULL,       "V",    0,                  0,  (APTR)MAIN_MENID_EDIT_PASTE},
  /* 11 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_PASTEB},
  /* 12 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_PASTEE},
  /* 13 */  {  NM_ITEM, NULL,       "E",    0,                  0,  (APTR)MAIN_MENID_EDIT_ERASE},
  /* 14 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_ERASEB},
  /* 15 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_ERASEE},
  /* 16 */  {  NM_ITEM, NULL,       "Z",    0,                  0,  (APTR)MAIN_MENID_EDIT_ZERO},
  /* 17 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_ZEROB},
  /* 18 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_ZEROE},
  /* 19 */  {  NM_ITEM, NULL,       "O",    0,                  0,  (APTR)MAIN_MENID_EDIT_OVERWRITE},
  /* 20 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_OVERWRITEB},
  /* 21 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /* 22 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_EXCHANGE},
  /* 23 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_EDIT_REPLACE},

  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_RNG_MARK2LOOP},
  /*  2 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_RNG_LOOP2MARK},
  /*  3 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_RNG_ZOOM2LOOP},
  /*  4 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_RNG_ZOOM2MARK},
  /*  5 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  6 */  {  NM_ITEM, NULL,       "H",    0,                  0,  (APTR)MAIN_MENID_RNG_MARKOFF},
  /*  7 */  {  NM_ITEM, NULL,       "A",    0,                  0,  (APTR)MAIN_MENID_RNG_MARKALL},

  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       "<",    0,                  0,  (APTR)MAIN_MENID_ZOOM_IN},
  /*  2 */  {  NM_ITEM, NULL,       ">",    0,                  0,  (APTR)MAIN_MENID_ZOOM_OUT},
  /*  3 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_ZOOM_PIXEL},
  /*  4 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_ZOOM_FULL},

  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_CL_CURR},
  /*  2 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_CL_ALL},
  /*  3 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_CL_ALLNORM},
  /*  4 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_CL_ALLZOOM},

  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_UT_SWAPEND},
  /*  2 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_UT_SWAPSIGN},
  /*  3 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  4 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_UT_INTERLEAVE},
  /*  5 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_UT_DEINTERLEAVE},

  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PREFS_GUI},
  /*  2 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PREFS_SAMP},
  /*  3 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PREFS_VMEM},
  /*  4 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PREFS_MISC},
  /*  5 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  6 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PREFS_USE},
  /*  7 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PREFS_SAVE},
  /*  8 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  9 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PREFS_RESTOREUSED},
  /* 10 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PREFS_RESTORESAVED},
  /* 11 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_PREFS_DEFAULT},

  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       "Help", NM_COMMANDSTRING,   0,  (APTR)MAIN_MENID_HELP_CT},
  /*  2 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_HELP_IX},
  /*  3 */  {  NM_ITEM, NULL,       NULL,   0,                  0,  (APTR)MAIN_MENID_HELP_WS},
  /*  4 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  5 */  {  NM_ITEM, NULL,       NULL,	0,					0,  (APTR)MAIN_MENID_HELP_ONLINE},
  /*  6 */  {  NM_ITEM, NULL,       NULL,	0,					0,  (APTR)MAIN_MENID_HELP_MAIL},
  /*  7 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  8 */  {  NM_ITEM, NULL,       NULL,	CHECKIT|MENUTOGGLE,	0,  (APTR)MAIN_MENID_HELP_SHOWTIPS},
  /*  9 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /* 10 */  {  NM_ITEM, NULL,       "?",    0,				    0,  (APTR)MAIN_MENID_HELP_ABOUT},

  /*  0 */  {   NM_END, NULL,       NULL,   0,                  0,  (APTR)0}
};

struct NewMenu FXMenu[]={
  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       "L",    0,                  0,  (APTR)MENID_LOAD},
  /*  2 */  {  NM_ITEM, NULL,       "S",    0,                  0,  (APTR)MENID_SAVE},
  /*  3 */  {  NM_ITEM, NULL,       "R",    0,                  0,  (APTR)MENID_RESET},
  /*  4 */  {  NM_ITEM, NULL,       "D",    0,                  0,  (APTR)MENID_DEFAULT},
  /*  5 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  6 */  {  NM_ITEM, NULL,       "Enter",NM_COMMANDSTRING,   0,  (APTR)MENID_GO},
  /*  7 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  8 */  {  NM_ITEM, NULL,       "ESC",  NM_COMMANDSTRING,   0,  (APTR)MENID_QUIT},

  /*  0 */  { NM_TITLE, NULL,       NULL,   0,                  0,  (APTR)0},
  /*  1 */  {  NM_ITEM, NULL,       "Help", NM_COMMANDSTRING,   0,  (APTR)MENID_HELP},
  /*  2 */  {  NM_ITEM, NM_BARLABEL,NULL,   0,                  0,  (APTR)0},
  /*  3 */  {  NM_ITEM, NULL,       "?",    NM_COMMANDSTRING,   0,  (APTR)MENID_ABOUT},

  /*  0 */  {   NM_END, NULL,       NULL,   0,                  0,  (APTR)0}
};

//-- Toolbar(s)

struct NewToolbar SFXToolbar[]={
  /* 00 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 01 */  {   NT_ITEM,    NULL,           NULL,           (APTR)nt_Pack(100,0),   0L, NTB_SUNKEN, 0L,                 NULL },
  /* 02 */  {   NT_ITEM,    &imgDropDown0,  &imgDropDown1,  &imgDropDown2,          0L, 0L,         GADID_TB_CHLO,      NULL },
  /* 03 */  {   NT_ITEM,    &imgPrefs0,     &imgPrefs1,     &imgPrefs2,             0L, 0L,         GADID_TB_PREFLO,    NULL },
  /* 04 */  {   NT_ITEM,    &imgLoad0,      &imgLoad1,      &imgLoad2,              0L, 0L,         GADID_TB_LOAD,      NULL },

  /* 05 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 06 */  {   NT_ITEM,    NULL,           NULL,           (APTR)nt_Pack(100,0),   0L, NTB_SUNKEN, 0L,                 NULL },
  /* 07 */  {   NT_ITEM,    &imgDropDown0,  &imgDropDown1,  &imgDropDown2,          0L, 0L,         GADID_TB_CHSA,      NULL },
  /* 08 */  {   NT_ITEM,    &imgPrefs0,     &imgPrefs1,     &imgPrefs2,             0L, 0L,         GADID_TB_PREFSA,    NULL },
  /* 09 */  {   NT_ITEM,    &imgSave0,      &imgSave1,      &imgSave2,              0L, 0L,         GADID_TB_SAVE,      NULL },

  /* 10 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 11 */  {   NT_ITEM,    NULL,           NULL,           (APTR)nt_Pack(120,0),   0L, NTB_SUNKEN, 0L,                 NULL },
  /* 12 */  {   NT_ITEM,    &imgDropDown0,  &imgDropDown1,  &imgDropDown2,          0L, 0L,         GADID_TB_CHFX,      NULL },
  /* 13 */  {   NT_ITEM,    &imgRunFX0,     &imgRunFX1,     &imgRunFX2,             0L, 0L,         GADID_TB_RUNFX,     NULL },

  /* 14 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 15 */  {   NT_ITEM,    NULL,           NULL,           (APTR)nt_Pack(120,0),   0L, NTB_SUNKEN, 0L,                 NULL },
  /* 16 */  {   NT_ITEM,    &imgDropDown0,  &imgDropDown1,  &imgDropDown2,          0L, 0L,         GADID_TB_CHREXX,    NULL },
  /* 17 */  {   NT_ITEM,    &imgRunRexx0,   &imgRunRexx1,   &imgRunRexx2,           0L, 0L,         GADID_TB_RUNREXX,   NULL },

  /* 18 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 19 */  {   NT_ITEM,    NULL,           NULL,           (APTR)nt_Pack(100,0),   0L, NTB_SUNKEN, 0L,                 NULL },
  /* 20 */  {   NT_ITEM,    &imgDropDown0,  &imgDropDown1,  &imgDropDown2,          0L, 0L,         GADID_TB_CHPL,      NULL },
  /* 21 */  {   NT_ITEM,    &imgPrefs0,     &imgPrefs1,     &imgPrefs2,             0L, 0L,         GADID_TB_PREFPL,    NULL },
  /* 22 */  {   NT_ITEM,    &imgPlayAll0,   &imgPlayAll1,   &imgPlayAll2,           0L, 0L,         GADID_TB_PLAYALL,   NULL },
  /* 23 */  {   NT_ITEM,    &imgPlayRng0,   &imgPlayRng1,   &imgPlayRng2,           0L, 0L,         GADID_TB_PLAYRNG,   NULL },
  /* 24 */  {   NT_ITEM,    &imgStop0,      &imgStop1,      &imgStop2,              0L, 0L,         GADID_TB_STOP,      NULL },
  /* 25 */  {   NT_ITEM,    &imgRecord0,    &imgRecord1,    &imgRecord2,            0L, 0L,         GADID_TB_RECORD,    NULL },

  /* 26 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 27 */  {   NT_ITEM,    NULL,           NULL,           (APTR)nt_Pack(150,0),   0L, NTB_SUNKEN, 0L,                 NULL },
  /* 28 */  {   NT_ITEM,    &imgDropDown0,  &imgDropDown1,  &imgDropDown2,          0L, 0L,         GADID_TB_CHBUF,     NULL },
  /* 29 */  {   NT_ITEM,    &imgPrefs0,     &imgPrefs1,     &imgPrefs2,             0L, 0L,         GADID_TB_PREFBUF,   NULL },
  /* 30 */  {   NT_ITEM,    &imgVisible0,   &imgVisible1,   &imgVisible2,           0L, 0L,         GADID_TB_HIDESHOW,  NULL },

  /* 31 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 32 */  {   NT_ITEM,    &imgCutMen0,    &imgCutMen1,    &imgCutMen2,            0L, 0L,         GADID_TB_CUTMEN,    NULL },
  /* 33 */  {   NT_ITEM,    &imgCopyMen0,   &imgCopyMen1,   &imgCopyMen2,           0L, 0L,         GADID_TB_COPYMEN,   NULL },
  /* 34 */  {   NT_ITEM,    &imgPasteMen0,  &imgPasteMen1,  &imgPasteMen2,          0L, 0L,         GADID_TB_PASTEMEN,  NULL },
  /* 35 */  {   NT_ITEM,    &imgEraseMen0,  &imgEraseMen1,  &imgEraseMen2,          0L, 0L,         GADID_TB_ERASEMEN,  NULL },
  /* 36 */  {   NT_ITEM,    &imgGrabMen0,   &imgGrabMen1,   &imgGrabMen2,           0L, 0L,         GADID_TB_GRABMEN,   NULL },
  /* 37 */  {   NT_ITEM,    &imgZeroMen0,   &imgZeroMen1,   &imgZeroMen2,           0L, 0L,         GADID_TB_ZEROMEN,   NULL },
  /* 38 */  {   NT_ITEM,    &imgOverwrMen0, &imgOverwrMen1, &imgOverwrMen2,         0L, 0L,         GADID_TB_OVERWRMEN, NULL },

  /* 39 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 40 */  {   NT_ITEM,    &imgZoomX0,     &imgZoomX1,     &imgZoomX2,             0L, 0L,         GADID_TB_ZOOMMODE,  NULL },
  /* 41 */  {   NT_ITEM,    &imgZoomIn0,    &imgZoomIn1,    &imgZoomIn2,            0L, 0L,         GADID_TB_ZOOMIN,    NULL },
  /* 42 */  {   NT_ITEM,    &imgZoomOut0,   &imgZoomOut1,   &imgZoomOut2,           0L, 0L,         GADID_TB_ZOOMOUT,   NULL },
  /* 43 */  {   NT_ITEM,    &imgZoomPixel0, &imgZoomPixel1, &imgZoomPixel2,         0L, 0L,         GADID_TB_ZOOMPIXEL, NULL },
  /* 44 */  {   NT_ITEM,    &imgZoomFull0,  &imgZoomFull1,  &imgZoomFull2,          0L, 0L,         GADID_TB_ZOOMFULL,  NULL },

  /* 45 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 46 */  {   NT_ITEM,    &imgLoop0,      &imgLoop1,      &imgLoop2,              0L, 0L,         GADID_TB_RNGMODE,   NULL },
  /* 47 */  {   NT_ITEM,    &imgLockOp0,    &imgLockOp1,    &imgLockOp2,            0L, 0L,         GADID_TB_LOCKLEN,   NULL },
  /* 48 */  {   NT_ITEM,    &imgChgBeg0,    &imgChgBeg1,    &imgChgBeg2,            0L, 0L,         GADID_TB_MOVEMODE,  NULL },
  /* 49 */  {   NT_ITEM,    &imgMoveBeg0,   &imgMoveBeg1,   &imgMoveBeg2,           0L, 0L,         GADID_TB_MOVEBEG,   NULL },
  /* 50 */  {   NT_ITEM,    &imgMoveLeftF0, &imgMoveLeftF1, &imgMoveLeftF2,         0L, 0L,         GADID_TB_MOVELEFTF, NULL },
  /* 51 */  {   NT_ITEM,    &imgMoveLeftS0, &imgMoveLeftS1, &imgMoveLeftS2,         0L, 0L,         GADID_TB_MOVELEFTS, NULL },
  /* 52 */  {   NT_ITEM,    &imgMoveLeftN0, &imgMoveLeftN1, &imgMoveLeftN2,         0L, 0L,         GADID_TB_MOVELEFTN, NULL },
  /* 53 */  {   NT_ITEM,    &imgMoveRightN0,&imgMoveRightN1,&imgMoveRightN2,        0L, 0L,         GADID_TB_MOVERIGHTN,NULL },
  /* 54 */  {   NT_ITEM,    &imgMoveRightS0,&imgMoveRightS1,&imgMoveRightS2,        0L, 0L,         GADID_TB_MOVERIGHTS,NULL },
  /* 55 */  {   NT_ITEM,    &imgMoveRightF0,&imgMoveRightF1,&imgMoveRightF2,        0L, 0L,         GADID_TB_MOVERIGHTF,NULL },
  /* 56 */  {   NT_ITEM,    &imgMoveEnd0,   &imgMoveEnd1,   &imgMoveEnd2,           0L, 0L,         GADID_TB_MOVEEND,   NULL },
  /* 57 */  {   NT_ITEM,    &imgMoveTop0,   &imgMoveTop1,   &imgMoveTop2,           0L, 0L,         GADID_TB_MOVETOP,   NULL },
  /* 58 */  {   NT_ITEM,    &imgMoveUpF0,   &imgMoveUpF1,   &imgMoveUpF2,           0L, 0L,         GADID_TB_MOVEUPF,   NULL },
  /* 59 */  {   NT_ITEM,    &imgMoveUpS0,   &imgMoveUpS1,   &imgMoveUpS2,           0L, 0L,         GADID_TB_MOVEUPS,   NULL },
  /* 60 */  {   NT_ITEM,    &imgMoveUpP0,   &imgMoveUpP1,   &imgMoveUpP2,           0L, 0L,         GADID_TB_MOVEUPP,   NULL },
  /* 61 */  {   NT_ITEM,    &imgMoveDownP0, &imgMoveDownP1, &imgMoveDownP2,         0L, 0L,         GADID_TB_MOVEDOWNP, NULL },
  /* 62 */  {   NT_ITEM,    &imgMoveDownS0, &imgMoveDownS1, &imgMoveDownS2,         0L, 0L,         GADID_TB_MOVEDOWNS, NULL },
  /* 63 */  {   NT_ITEM,    &imgMoveDownF0, &imgMoveDownF1, &imgMoveDownF2,         0L, 0L,         GADID_TB_MOVEDOWNF, NULL },
  /* 64 */  {   NT_ITEM,    &imgMoveBottom0,&imgMoveBottom1,&imgMoveBottom2,        0L, 0L,         GADID_TB_MOVEBOTTOM,NULL },

  /* 65 */  { NT_TOOLBAR,   NULL,           NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar pos & size in here */
  /* 66 */  {   NT_ITEM,    &imgWinM0,      &imgWinM1,      &imgWinM2,              0L, 0L,         GADID_TB_WINMODE,   NULL },

  /* 67 */  { NT_END,      NULL,            NULL,           NULL,                   0L, 0L,         0L,                 NULL }, /* we store toolbar-window size in here */
};

// change SFXTB_END in SFX-Includes.h

struct NewToolbar SFXToolbarCut[]={
  /* 00 */  {   NT_ITEM,    &imgCut0,       &imgCut1,       &imgCut2,       0L, 0L,         GADID_DD_CUT,   NULL },
  /* 01 */  {   NT_ITEM,    &imgCutL0,      &imgCutL1,      &imgCutL2,      0L, 0L,         GADID_DD_CUTB,  NULL },
  /* 02 */  {   NT_ITEM,    &imgCutR0,      &imgCutR1,      &imgCutR2,      0L, 0L,         GADID_DD_CUTE,  NULL },
};

struct Gadget tbCutR    ={NULL,     2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCutR0,     &imgCutR2   ,NULL,0,NULL,GADID_DD_CUTE  ,&SFXToolbarCut[2]  };
struct Gadget tbCutL    ={&tbCutR,  2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCutL0,     &imgCutL2   ,NULL,0,NULL,GADID_DD_CUTB  ,&SFXToolbarCut[1]  };
struct Gadget tbCut     ={&tbCutL,  2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCut0,      &imgCut2    ,NULL,0,NULL,GADID_DD_CUT   ,&SFXToolbarCut[0]  };

struct NewToolbar SFXToolbarCopy[]={
  /* 00 */  {   NT_ITEM,    &imgCopy0,      &imgCopy1,      &imgCopy2,      0L, 0L,         GADID_DD_COPY,  NULL },
  /* 01 */  {   NT_ITEM,    &imgCopyL0,     &imgCopyL1,     &imgCopyL2,     0L, 0L,         GADID_DD_COPYB, NULL },
  /* 02 */  {   NT_ITEM,    &imgCopyR0,     &imgCopyR1,     &imgCopyR2,     0L, 0L,         GADID_DD_COPYE, NULL },
};

struct Gadget tbCopyR   ={NULL,     2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCopyR0,    &imgCopyR2  ,NULL,0,NULL,GADID_DD_COPYE ,&SFXToolbarCopy[2] };
struct Gadget tbCopyL   ={&tbCopyR, 2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCopyL0,    &imgCopyL2  ,NULL,0,NULL,GADID_DD_COPYB ,&SFXToolbarCopy[1] };
struct Gadget tbCopy    ={&tbCopyL, 2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCopy0,     &imgCopy2   ,NULL,0,NULL,GADID_DD_COPY  ,&SFXToolbarCopy[0] };

struct NewToolbar SFXToolbarPaste[]={
  /* 00 */  {   NT_ITEM,    &imgPaste0,     &imgPaste1,     &imgPaste2,     0L, 0L,         GADID_DD_PASTE, NULL },
  /* 01 */  {   NT_ITEM,    &imgPasteL0,    &imgPasteL1,    &imgPasteL2,    0L, 0L,         GADID_DD_PASTEB,NULL },
  /* 02 */  {   NT_ITEM,    &imgPasteR0,    &imgPasteR1,    &imgPasteR2,    0L, 0L,         GADID_DD_PASTEE,NULL },
};

struct Gadget tbPasteR  ={NULL,     2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgPasteR0,   &imgPasteR2 ,NULL,0,NULL,GADID_DD_PASTEE,&SFXToolbarPaste[2]    };
struct Gadget tbPasteL  ={&tbPasteR,2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgPasteL0,   &imgPasteL2 ,NULL,0,NULL,GADID_DD_PASTEB,&SFXToolbarPaste[1]    };
struct Gadget tbPaste   ={&tbPasteL,2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgPaste0,    &imgPaste2  ,NULL,0,NULL,GADID_DD_PASTE ,&SFXToolbarPaste[0]    };

struct NewToolbar SFXToolbarErase[]={
  /* 00 */  {   NT_ITEM,    &imgErase0,     &imgErase1,     &imgErase2,     0L, 0L,         GADID_DD_ERASE,     NULL },
  /* 01 */  {   NT_ITEM,    &imgEraseL0,    &imgEraseL1,    &imgEraseL2,    0L, 0L,         GADID_DD_ERASEB,    NULL },
  /* 02 */  {   NT_ITEM,    &imgEraseR0,    &imgEraseR1,    &imgEraseR2,    0L, 0L,         GADID_DD_ERASEE,NULL },
};

struct Gadget tbEraseR  ={NULL,     2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgEraseR0,   &imgEraseR2 ,NULL,0,NULL,GADID_DD_ERASEE,&SFXToolbarErase[2]    };
struct Gadget tbEraseL  ={&tbEraseR,2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgEraseL0,   &imgEraseL2 ,NULL,0,NULL,GADID_DD_ERASEB,&SFXToolbarErase[1]    };
struct Gadget tbErase   ={&tbEraseL,2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgErase0,    &imgErase2  ,NULL,0,NULL,GADID_DD_ERASE ,&SFXToolbarErase[0]    };

struct NewToolbar SFXToolbarGrab[]={
  /* 00 */  {   NT_ITEM,    &imgGrab0,      &imgGrab1,      &imgGrab2,      0L, 0L,         GADID_DD_GRAB,      NULL },
  /* 01 */  {   NT_ITEM,    &imgGrabL0,     &imgGrabL1,     &imgGrabL2,     0L, 0L,         GADID_DD_GRABB, NULL },
  /* 02 */  {   NT_ITEM,    &imgGrabR0,     &imgGrabR1,     &imgGrabR2,     0L, 0L,         GADID_DD_GRABE, NULL },
};

struct Gadget tbGrabR   ={NULL,     2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgGrabR0,    &imgGrabR2  ,NULL,0,NULL,GADID_DD_GRABE ,&SFXToolbarGrab[2] };
struct Gadget tbGrabL   ={&tbGrabR, 2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgGrabL0,    &imgGrabL2  ,NULL,0,NULL,GADID_DD_GRABB ,&SFXToolbarGrab[1] };
struct Gadget tbGrab    ={&tbGrabL, 2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgGrab0,     &imgGrab2   ,NULL,0,NULL,GADID_DD_GRAB  ,&SFXToolbarGrab[0] };

struct NewToolbar SFXToolbarZero[]={
  /* 00 */  {   NT_ITEM,    &imgZero0,      &imgZero1,      &imgZero2,      0L, 0L,         GADID_DD_ZERO,  NULL },
  /* 01 */  {   NT_ITEM,    &imgZeroL0,     &imgZeroL1,     &imgZeroL2,     0L, 0L,         GADID_DD_ZEROB, NULL },
  /* 02 */  {   NT_ITEM,    &imgZeroR0,     &imgZeroR1,     &imgZeroR2,     0L, 0L,         GADID_DD_ZEROE, NULL },
};

struct Gadget tbZeroR   ={NULL,     2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgZeroR0,    &imgZeroR2  ,NULL,0,NULL,GADID_DD_ZEROE ,&SFXToolbarZero[2] };
struct Gadget tbZeroL   ={&tbZeroR, 2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgZeroL0,    &imgZeroL2  ,NULL,0,NULL,GADID_DD_ZEROB ,&SFXToolbarZero[1] };
struct Gadget tbZero    ={&tbZeroL, 2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgZero0,     &imgZero2   ,NULL,0,NULL,GADID_DD_ZERO  ,&SFXToolbarZero[0] };

struct NewToolbar SFXToolbarOverwr[]={
  /* 00 */  {   NT_ITEM,    &imgOverwr0,    &imgOverwr1,    &imgOverwr2,    0L, 0L,         GADID_DD_OVERWR, NULL },
  /* 01 */  {   NT_ITEM,    &imgOverwrL0,   &imgOverwrL1,   &imgOverwrL2,   0L, 0L,         GADID_DD_OVERWRB,NULL },
};

struct Gadget tbOverwrL ={NULL,      2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgOverwrL0, &imgOverwrL2,NULL,0,NULL,GADID_DD_OVERWRB,&SFXToolbarOverwr[1]  };
struct Gadget tbOverwr  ={&tbOverwrL,2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgOverwr0,  &imgOverwr2 ,NULL,0,NULL,GADID_DD_OVERWR ,&SFXToolbarOverwr[0]  };

//-- Popup-Menus

struct PopupMenu *pmenSampleWin=NULL;

//-- asl request tags

UBYTE asl_prefs=FALSE;

struct TagItem File_Reqtags[]={					// edit constants in SFX-Main.h
  /*  0 */  ASLFR_TitleText,        (ULONG)NULL,
  /*  1 */  ASLFR_PubScreenName,    (ULONG)SFX_PUBSCRNAME,
  /*  2 */  ASLFR_InitialLeftEdge,  180,
  /*  3 */  ASLFR_InitialTopEdge,   15,
  /*  4 */  ASLFR_InitialWidth,     300,
  /*  5 */  ASLFR_InitialHeight,    184,
  /*  6 */  ASLFR_InitialDrawer,    (ULONG)"progdir:",		/* NULL seems to cause hits under OS3.9 */
  /*  7 */  ASLFR_InitialFile,      (ULONG)"",				/* NULL seems to cause hits under OS3.9 */
  /*  8 */  ASLFR_RejectIcons,      TRUE,
  /*  9 */  ASLFR_DoSaveMode,       FALSE,
  /* 10 */  ASLFR_DoMultiSelect,    TRUE,
  /* 11 */  ASLFR_DrawersOnly,      FALSE,
  /* 12 */  TAG_DONE
};

struct TagItem Font_Reqtags[]={					// edit constants in SFX-Main.h
  /*  0 */  ASLFO_TitleText,        (ULONG)NULL,
  /*  1 */  ASLFR_PubScreenName,    (ULONG)SFX_PUBSCRNAME,
  /*  2 */  ASLFO_InitialLeftEdge,  180,
  /*  3 */  ASLFO_InitialTopEdge,   15,
  /*  4 */  ASLFO_InitialWidth,     300,
  /*  5 */  ASLFO_InitialHeight,    184,
  /*  6 */  ASLFO_InitialName,      NULL,
  /*  7 */  ASLFO_InitialSize,      NULL,
  /*  8 */  ASLFO_FixedWidthOnly,   FALSE,
  /*  9 */  ASLFO_MinHeight,        6,
  /* 10 */  ASLFO_MaxHeight,        28,
  /* 11 */  ASLFO_MaxFrontPen,      1,
  /* 12 */  ASLFO_MaxBackPen,       1,
  /* 13 */  TAG_DONE
};

struct TagItem ScrMd_Reqtags[]={					// edit constants in SFX-Main.h
  /*  0 */  ASLSM_TitleText,            (ULONG)NULL,
  /*  1 */  ASLSM_PubScreenName,        (ULONG)SFX_PUBSCRNAME,
  /*  2 */  ASLSM_InitialLeftEdge,      180,
  /*  3 */  ASLSM_InitialTopEdge,       15,
  /*  4 */  ASLSM_InitialWidth,         300,
  /*  5 */  ASLSM_InitialHeight,        184,
  /*  6 */  ASLSM_InitialDisplayID,     NULL,
  /*  7 */  ASLSM_InitialDisplayWidth,  0,
  /*  8 */  ASLSM_InitialDisplayHeight, 0,
  /*  9 */  ASLSM_InitialDisplayDepth,  0,
  /* 10 */  ASLSM_InitialOverscanType,  0,
  /* 11 */  ASLSM_InitialAutoScroll,    0,
  /* 12 */  ASLSM_DoWidth,              TRUE,
  /* 13 */  ASLSM_DoHeight,             TRUE,
  /* 14 */  ASLSM_DoDepth,              TRUE,
  /* 15 */  ASLSM_DoOverscanType,       TRUE,
  /* 16 */  ASLSM_DoAutoScroll,         TRUE,
  /* 17 */  ASLSM_MinWidth,             640,
  /* 18 */  ASLSM_MinHeight,            400,
  /* 19 */  ASLSM_MinDepth,             4,
  /* 20 */  TAG_DONE
};

//-- gui pen definitions

#define COL_GRAY1 0     // middle
#define COL_BLACK 1
#define COL_WHITE 2
#define COL_COLOR 3
//                4     // green (sample wave)
#define COL_GRAY2 5     // bright
#define COL_GRAY3 6     // dark
//                7     // yellow (mark)

UWORD DriPens2[]={		/* OS 3.X */
  /*  0 */  COL_WHITE,  /* DETAILPEN        */
  /*  1 */  COL_BLACK,  /* BLOCKPEN         */
  /*  2 */  COL_BLACK,  /* TEXTPEN          */
  /*  3 */  COL_WHITE,  /* SHINEPEN         */
  /*  4 */  COL_BLACK,  /* SHADOWPEN        */
  /*  5 */  COL_COLOR,  /* FILLPEN          */
  /*  6 */  COL_BLACK,  /* FILLTEXTPEN      */
  /*  7 */  COL_GRAY2,  /* BACKGROUNDPEN    */ /* should be 0, I am using 5 */
  /*  8 */  COL_WHITE,  /* HIGHLIGHTTEXTPEN */
  /*  9 */  COL_BLACK,  /* BARDETAILPEN     */
  /* 10 */  COL_WHITE,  /* BARBLOCKPEN      */ /* maybe make it configurable */
  /* 10     COL_GRAY2,     BARBLOCKPEN      */
  /* 11 */  COL_BLACK,  /* BARTRIMPEN       */
  /* 12 */  (unsigned short)~0          /* END OF ARRAY     */
};

UWORD DriPens3[]={		/* OS4.X */
  /*  0 */  COL_WHITE,  /* DETAILPEN             */
  /*  1 */  COL_BLACK,  /* BLOCKPEN              */
  /*  2 */  COL_BLACK,  /* TEXTPEN               */
  /*  3 */  COL_WHITE,  /* SHINEPEN              */
  /*  4 */  COL_BLACK,  /* SHADOWPEN             */
  /*  5 */  COL_COLOR,  /* FILLPEN               */
  /*  6 */  COL_BLACK,  /* FILLTEXTPEN           */
  /*  7 */  COL_GRAY1,  /* BACKGROUNDPEN         */
  /*  8 */  COL_WHITE,  /* HIGHLIGHTTEXTPEN      */
  /*  9 */  COL_BLACK,  /* BARDETAILPEN          */
  /* 10     COL_WHITE,     BARBLOCKPEN           */ /* maybe make it configurable */
  /* 10 */  COL_GRAY1,  /* BARBLOCKPEN           */
  /* 11 */  COL_BLACK,  /* BARTRIMPEN            */
  /* 12 */  COL_WHITE,  /* BARCONTOURPEN         */
  /* 13 */  COL_GRAY2,  /* FOREGROUNDPEN         */
  /* 14 */  COL_WHITE,  /* FORESHINEPEN          */
  /* 15 */  COL_GRAY3,  /* FORESHADOWPEN         */
  /* 16 */  COL_GRAY2,  /* FILLSHINEPEN          */
  /* 17 */  COL_GRAY3,  /* FILLSHADOWPEN         */
  /* 18 */  COL_GRAY1,  /* INACTIVEFILLPEN       */
  /* 19 */  COL_BLACK,  /* INACTIVEFILLTEXTPEN   */
  /* 20 */  COL_GRAY2,  /* INACTIVEFILLSHINEPEN  */
  /* 20 */  COL_GRAY3,  /* INACTIVEFILLSHADOWPEN */
  /* 21 */  COL_GRAY2,  /* DISABLEDPEN           */
  /* 22 */  COL_GRAY1,  /* DISABLEDTEXTPEN       */
  /* 23 */  COL_WHITE,  /* DISABLEDSHINEPEN      */
  /* 24 */  COL_GRAY3,  /* DISABLEDSHADOWPEN     */
  /* 25 */  COL_GRAY1,  /* MENUBACKGROUNDPEN     */
  /* 26 */  COL_BLACK,  /* MENUTEXTPEN           */
  /* 27 */  COL_WHITE,  /* MENUSHINEPEN          */
  /* 28 */  COL_BLACK,  /* MENUSHADOWPEN         */
  /* 29 */  COL_COLOR,  /* SELECTPEN             */
  /* 30 */  COL_WHITE,  /* SELECTTEXTPEN         */
  /* 31 */  COL_WHITE,  /* SELECTSHINEPEN        */
  /* 32 */  COL_BLACK,  /* SELECTSHADOWPEN       */
  /* 33 */  COL_BLACK,  /* GLYPHPEN              */
  /* 34 */  COL_GRAY1,  /* GLYPHFILLPEN          */
  /* 35 */  COL_BLACK,  /* INACTIVEGLYPHPEN      */
  /* 36 */  0        ,  /* RESERVEDPEN           */
  /* 37 */  COL_BLACK,  /* GADGETPEN             */
  /* 38 */  COL_WHITE,  /* TITLEPEN              */
  /* 39 */  COL_GRAY2,  /* HALFSHINEPEN          */
  /* 40 */  COL_GRAY1,  /* HALFSHADOWPEN         */
  /* 41 */  COL_BLACK,  /* FLATBORDERPEN         */
  /* 42 */  COL_BLACK,  /* FILLFLATPEN           */
  /* 43 */  COL_COLOR,  /* INACTIVEFILLFLATPEN
  /* 44 */  (unsigned short)~0          /* END OF ARRAY     */
};

//-- screen & window tags

struct TagItem Main_Scrtags[]={				   // edit constants in SFX-Main.h
  /* 00 */	SA_Left,        0,
  /* 01 */	SA_Top,         0,
  /* 02 */	SA_Width,       640,
  /* 03 */	SA_Height,      265,
  /* 04 */	SA_Depth,       4,
  /* 05 */	SA_Title,       (ULONG)VERS,
  /* 06 */	SA_Colors32,    NULL,
  /* 07 */	SA_Font,        (ULONG)(&ScrFont),
  /* 08 */	SA_Type,        PUBLICSCREEN,
  /* 09 */	SA_DisplayID,   PAL_MONITOR_ID|HIRES_KEY,
  /* 10 */	SA_Pens,        NULL,
  /* 11 */	SA_AutoScroll,  FALSE,
  /* 12 */	SA_Overscan,    0,
  /* 13 */	SA_Interleaved, TRUE,
  /* 14 */	SA_MinimizeISG, TRUE,
  /* 15 */	SA_PubName,     (ULONG)SFX_PUBSCRNAME,
  /* 16 */	SA_SharePens,   TRUE,
  /* 17 */	SA_Behind,      TRUE,
  /* 18 */    TAG_DONE
};

struct TagItem BackDrop_Wintags[]={
  /*  0 */  WA_Left,        0,
  /*  1 */  WA_Top,         0,
  /*  2 */  WA_Width,       0,
  /*  3 */  WA_Height,      0,
  /*  4 */  WA_IDCMP,       gwin_idcmps,
  /*  5 */  WA_ScreenTitle, NULL,
  /*  6 */  WA_CustomScreen,NULL,
  /*  7 */  WA_Flags,       WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_BACKDROP|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
  /*  8 */  TAG_DONE
};

struct TagItem StatusBar_Wintags[]={
  /*  0 */  WA_Left,        0,
  /*  1 */  WA_Top,         0,
  /*  2 */  WA_Width,       0,
  /*  3 */  WA_Height,      0,
  /*  4 */  WA_IDCMP,       0,
  /*  5 */  WA_ScreenTitle, NULL,
  /*  6 */  WA_CustomScreen,NULL,
  /*  7 */  WA_Gadgets,     NULL,
  /*  8 */  WA_Flags,       WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_BACKDROP|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
  /*  9 */  TAG_DONE
};

struct TagItem ToolBar_Wintags[]={
  /*  0 */  WA_Left,        0,
  /*  1 */  WA_Top,         0,
  /*  2 */  WA_Width,       0,
  /*  3 */  WA_Height,      0,
  /*  4 */  WA_IDCMP,       0,
  /*  5 */  WA_ScreenTitle, NULL,
  /*  6 */  WA_CustomScreen,NULL,
  /*  7 */  WA_Gadgets,     NULL,
  /*  8 */  WA_Flags,       WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_BACKDROP|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
  /*  9 */  TAG_DONE
};

struct TagItem DropDown_Wintags[]={
  /*  0 */  WA_Left,        0,
  /*  1 */  WA_Top,         0,
  /*  2 */  WA_Width,       0,
  /*  3 */  WA_Height,      0,
  /*  4 */  WA_IDCMP,       0,
  /*  5 */  WA_ScreenTitle, NULL,
  /*  6 */  WA_CustomScreen,NULL,
  /*  7 */  WA_Gadgets,     NULL,
  /*  8 */  WA_Flags,       WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE|WFLG_ACTIVATE,
  /*  9 */  TAG_DONE
};

struct TagItem Dialog_Wintags[]={
  /*  0 */  WA_Left,        0,
  /*  1 */  WA_Top,         0,
  /*  2 */  WA_InnerWidth,  0,
  /*  3 */  WA_InnerHeight, 0,
  /*  4 */  WA_IDCMP,       0,
  /*  5 */  WA_Gadgets,     NULL,
  /*  6 */  WA_Title,       NULL,
  /*  7 */  WA_ScreenTitle, NULL,
  /*  8 */  WA_CustomScreen,NULL,
  /*  9 */  WA_BlockPen,    1,
  /* 10 */  WA_Flags,       0,
  /* 11 */  TAG_DONE
};

//-- definitions --------------------------------------------------------------

//-- GUI Init, Done & ReOpen

/****** SoundFX/GUI_Init [3.70] *
*
*   NAME
*     GUI_Init --- Create all GUI related instances
*   SYNOPSIS
*     GUI_Init()
*
*     void GUI_Init(void);
*   FUNCTION
*     Opens fonts,screen and windows. Sets colors etc.
*   NOTES
*   creates the following instances
*     struct TextFont
*       *RunTime.scrfont
*       *RunTime.scrfontb
*       *RunTime.sigfont
*       *RunTime.scafont
*     struct screen
*       *RunTime.sfxpubscr
*       struct VisualInfo
*         *RunTime.VisualInfo
*     struct menu
*       *RunTIme.Menu
*       *RunTime.FXMenu
*   SEE ALSO
*     GUI_Done(), GUI_ReOpen()
*******
*/

void GUI_Init(void) {
	register UWORD j;
	WORD ret;
	UWORD *guiscl,cols;
	UBYTE cnr;
	struct Screen *scr;
	struct Pal32 {
		UWORD nr;
		UWORD first;
		ULONG col[16][3];
		ULONG term;
	} pal32;
	struct ViewPort *vp;
	struct ColorMap *cm;
	BPTR lock;

	INTRO;

	RunTime.sig_in=0;       // there should be no open window this time
	writeprefs=FALSE;		// we have not changed the prefs yet

	//-- Set default loader/saver/player
	if(!RunTime.tryreopen) {
		if((ret=GetIndexByName(&LoaderList,sfxprefs_win.lowin.entryname))>-1) RunTime.aloader=ret;
		else RunTime.aloader=0;
		if((ret=GetIndexByName(&SaverList,sfxprefs_win.sawin.entryname))>-1) RunTime.asaver=ret;
		else RunTime.asaver=0;
		RunTime.aplayer =match_name2nr(RunTime.anzpl,sfxprefs_win.plwin.entryname,pllist);
		if((ret=GetIndexByName(&EffektList,sfxprefs_win.opwin.entryname))>-1) RunTime.aktfx=ret;
		else RunTime.aktfx=0;
		if((ret=GetIndexByName(&RexxList  ,sfxprefs_win.rxopwin.entryname))>-1) RunTime.arxop=ret;
		else RunTime.arxop=0;
	}

	//strcpy(RunTime.fontname,sfxprefs_gui.scrfont_name);

	//-- Fonts öffnen
	//MSG(" before opening fonts");
	ScrFont.ta_Name=sfxprefs_gui.scrfont_name;
	ScrFont.ta_YSize=sfxprefs_gui.scrfont_size;
	RunTime.ScrFont=&ScrFont;
	if(!(RunTime.scrfont=OpenDiskFont(RunTime.ScrFont))) {
		ScrFont.ta_YSize=6;
		if(strnicmp(ScrFont.ta_Name,"Trinomic.font",13)) strcpy(ScrFont.ta_Name,"Trinomic.font");
		if(!(RunTime.scrfont=OpenDiskFont(RunTime.ScrFont))) { ADosError(ScrFont.ta_Name,errAccessFile,IoErr(),__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
		sfxprefs_gui.scrfont_size=ScrFont.ta_YSize;
		writeprefs=TRUE;
	}

	ScrFontB.ta_Name=sfxprefs_gui.scrfont_name;
	ScrFontB.ta_YSize=sfxprefs_gui.scrfont_size;
	RunTime.ScrFontB=&ScrFontB;
	if(!(RunTime.scrfontb=OpenDiskFont(RunTime.ScrFontB))) { ADosError(ScrFontB.ta_Name,errAccessFile,IoErr(),__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }

	/* we now use a vector font
	SigFont.ta_YSize=16;
	if(sfxprefs_gui.fontsize<16)    SigFont.ta_YSize=12;
	if(sfxprefs_gui.fontsize<12)    SigFont.ta_YSize=8;
	if(sfxprefs_gui.fontsize<8)     SigFont.ta_YSize=6;
	*/
	SigFont.ta_YSize=sfxprefs_gui.scrfont_size;
	RunTime.SigFont=&SigFont;
	if(!(RunTime.sigfont=OpenDiskFont(RunTime.SigFont))) { ADosError(SigFont.ta_Name,errAccessFile,IoErr(),__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }

	ScaFont.ta_Name =sfxprefs_sbuf.scafont_name;
	ScaFont.ta_YSize=sfxprefs_sbuf.scafont_size;
	RunTime.ScaFont=&ScaFont;
	if(!(RunTime.scafont=OpenDiskFont(RunTime.ScaFont))) {
		ScaFont.ta_YSize=6;
		if(strnicmp(ScaFont.ta_Name,"Trinomic.font",13)) strcpy(ScaFont.ta_Name,"Trinomic.font");
		if(!(RunTime.scafont=OpenDiskFont(RunTime.ScaFont))) { ADosError(ScaFont.ta_Name,errAccessFile,IoErr(),__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
		sfxprefs_sbuf.scafont_size=ScaFont.ta_YSize;
		writeprefs=TRUE;
	}

	StatFont.ta_Name=sfxprefs_gui.statfont_name;
	StatFont.ta_YSize=sfxprefs_gui.statfont_size;
	RunTime.StatFont=&StatFont;
	if(!(RunTime.statfont=OpenDiskFont(RunTime.StatFont))) {
		StatFont.ta_YSize=6;
		if(strnicmp(StatFont.ta_Name,"Trinomic.font",13)) strcpy(StatFont.ta_Name,"Trinomic.font");
		if(!(RunTime.statfont=OpenDiskFont(RunTime.StatFont))) { ADosError(StatFont.ta_Name,errAccessFile,IoErr(),__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
		sfxprefs_gui.statfont_size=ScaFont.ta_YSize;
		writeprefs=TRUE;
	}

	MSG(" do some precalculations for the GUI");

	RunTime.scalex=(double)(RunTime.scrfont->tf_XSize)/6.0;         /* Fontsensitivitylookup's */
	RunTime.scaley=(double)(RunTime.scrfont->tf_YSize)/6.0;
	guiscl=RunTime.scx;for(j=0;j<750;j++) *(guiscl++)=(UWORD)((double)j*RunTime.scalex);
	guiscl=RunTime.scy;for(j=0;j<300;j++) *(guiscl++)=(UWORD)((double)j*RunTime.scaley);

	stringextend.Font=RunTime.scrfont;
	stringextend2.Font=RunTime.statfont;

	MemUsage(sfxtitle);

	//-- use DriPens for the respective OSVersion
	if(RunTime.os_version<50) { /* OS 3.X */
		DriPens2[BACKGROUNDPEN  ]=sfxprefs_gui.Pens[PEN_GUI_BACK     ];
		DriPens2[SHINEPEN       ]=sfxprefs_gui.Pens[PEN_GUI_SHINE    ];
		DriPens2[SHADOWPEN      ]=sfxprefs_gui.Pens[PEN_GUI_SHADOW   ];
		DriPens2[FILLPEN        ]=sfxprefs_gui.Pens[PEN_GUI_HIGHLIGHT];
		Main_Scrtags_Pens		=(ULONG)(&DriPens2);
	}
	else { /* OS 4.X */
		DriPens3[SHINEPEN       ]=sfxprefs_gui.Pens[PEN_GUI_SHINE    ];
		DriPens3[SHADOWPEN      ]=sfxprefs_gui.Pens[PEN_GUI_SHADOW   ];
		DriPens3[FILLPEN        ]=sfxprefs_gui.Pens[PEN_GUI_HIGHLIGHT];
		DriPens3[FOREGROUNDPEN  ]=sfxprefs_gui.Pens[PEN_GUI_BACK     ];
		Main_Scrtags_Pens		=(ULONG)(&DriPens3);
	}

	Main_Scrtags_Colors32	=(ULONG)&pal32;
	Main_Scrtags_AutoScroll	=sfxprefs_gui.autoscroll;
	Main_Scrtags_Overscan	=sfxprefs_gui.oscan;

	if(sfxprefs_gui.scrmode==INVALID_ID) {  // try to get a valid displayid
		MSG("  no prefs -> no default screenmode");
		if((scr=LockPubScreen("Workbench"))) {              // get wb screen mode
			//MSG("  LockPubScreen okay");
			sfxprefs_gui.scrmode=       GetVPModeID(&(scr->ViewPort));
			sfxprefs_gui.xres=          scr->Width;
			sfxprefs_gui.yres=          scr->Height;
			sfxprefs_gui.depth=         scr->BitMap.Depth;
			UnlockPubScreen(NULL,scr);
		}
		if(sfxprefs_gui.scrmode==INVALID_ID) {  // set default scr mode using BestModeID )
			struct DimensionInfo DimsInfo;
			MSG("  LockPubScreen failed");
			sfxprefs_gui.scrmode=       BestModeID(BIDTAG_NominalWidth,640,BIDTAG_NominalHeight,480,BIDTAG_Depth,4,TAG_DONE);
			if(GetDisplayInfoData(0,(UBYTE *)&DimsInfo,sizeof(DimsInfo),DTAG_DIMS,sfxprefs_gui.scrmode)) {
				sfxprefs_gui.xres=          DimsInfo.Nominal.MaxX+1;
				sfxprefs_gui.yres=          DimsInfo.Nominal.MaxY+1;
				sfxprefs_gui.depth=			4;
			}
		}
		writeprefs=TRUE;
	}
	if(sfxprefs_gui.scrmode!=INVALID_ID) {  // do we have one now
		MSG("  set values");
		Main_Scrtags_Width		=sfxprefs_gui.xres;
		Main_Scrtags_Height		=sfxprefs_gui.yres;
		Main_Scrtags_DisplayID  =sfxprefs_gui.scrmode;
		if(sfxprefs_gui.depth<4)        Main_Scrtags_Depth=sfxprefs_gui.depth=4;
		else if(sfxprefs_gui.depth>8)   Main_Scrtags_Depth=sfxprefs_gui.depth=8;
		else                            Main_Scrtags_Depth=sfxprefs_gui.depth;

		pal32.nr=8;
		if(sfxprefs_gui.depth>3) pal32.nr=16;
		pal32.first=0;pal32.term=0L;
		for(j=0;j<pal32.nr;j++) {
			pal32.col[j][0]=MakeCol32(sfxprefs_gui.ScreenColors[j][0]);
			pal32.col[j][1]=MakeCol32(sfxprefs_gui.ScreenColors[j][1]);
			pal32.col[j][2]=MakeCol32(sfxprefs_gui.ScreenColors[j][2]);
		}
		for(j=pal32.nr;j<SFXPrefs_Gui_MaxColors;j++) {
			pal32.col[j][0]=pal32.col[j][1]=pal32.col[j][2]=0;
		}

		MSG4(" before opening screen : 0x%08lx : %3d x %3d x %2d",Main_Scrtags_DisplayID,Main_Scrtags_Width,Main_Scrtags_Height,Main_Scrtags_Depth);
		if(!(RunTime.sfxpubscr=OpenScreenTagList(NULL,Main_Scrtags))) { Message(errOpenScreen,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
		//MSG(" after opening screen");
	}
	else { Message(errOpenScreen,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }

	MSG("\tbefore making it public");
	if(!(PubScreenStatus(RunTime.sfxpubscr,0)&(UWORD)0x1)) { Message(errMakeScreenPublic,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }   /* Make it really public */

	RunTime.sfxpubscr->WBorBottom=4;                    // small hack to have proportinal borders

	//-- get screen dimensions *after* screen has been opened
	RunTime.xres=sfxprefs_gui.xres;
	RunTime.yres=sfxprefs_gui.yres;


	BackDrop_Wintags_CustomScreen=StatusBar_Wintags_CustomScreen=ToolBar_Wintags_CustomScreen=DropDown_Wintags_CustomScreen=Dialog_Wintags_CustomScreen=(ULONG)RunTime.sfxpubscr;
	vp=&RunTime.sfxpubscr->ViewPort;

	//-- this area is available for windows, everytime we add an window to the border, we have to adapt the values
	RunTime.clientrect.LeftEdge     =0;
	RunTime.clientrect.RightEdge    =RunTime.sfxpubscr->Width-1;
	RunTime.clientrect.Width        =RunTime.sfxpubscr->Width;
	RunTime.clientrect.TopEdge      =RunTime.sfxpubscr->BarHeight+1;
	RunTime.clientrect.BottomEdge   =RunTime.sfxpubscr->Height-1;
	RunTime.clientrect.Height       =RunTime.sfxpubscr->Height-(RunTime.sfxpubscr->BarHeight+1);
	//-- adapt sample window sizes
	if(sfxprefs_sbuf.xs1>RunTime.clientrect.Width) sfxprefs_sbuf.xs1=RunTime.clientrect.Width;
	if(sfxprefs_sbuf.ys1>RunTime.clientrect.Height) sfxprefs_sbuf.ys1=RunTime.clientrect.Height;
	if(sfxprefs_sbuf.xs2>RunTime.clientrect.Width) sfxprefs_sbuf.xs2=RunTime.clientrect.Width;
	if(sfxprefs_sbuf.ys2>RunTime.clientrect.Height) sfxprefs_sbuf.ys2=RunTime.clientrect.Height;

	MSG("\tlock pens");
	cm=vp->ColorMap;
	for(j=0;j<pal32.nr;j++) ObtainPen(cm,j,pal32.col[j][0],pal32.col[j][1],pal32.col[j][2],0);
	if(Main_Scrtags_Depth>4) {              // inverse colors to make highlight looking good
		MSG("\tset some colors");
		cols=(1<<Main_Scrtags_Depth)-1;

		cnr=sfxprefs_gui.Pens[PEN_SMP_MARK];
		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_BACK     ],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_SMP_BACK     ]);
		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_MAXL     ],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_SMP_MAXL     ]);
		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_RMSL     ],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_SMP_RMSL     ]);
		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_AVGL     ],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_SMP_AVGL     ]);
//		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_GUI_HIGHLIGHT],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_GUI_HIGHLIGHT]);

//		cnr=sfxprefs_gui.Pens[PEN_SMP_GRID];
//		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_GRID     ],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_SMP_GRID     ]);

		cnr=sfxprefs_gui.Pens[PEN_SMP_LOOP];
		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_GRID     ],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_SMP_GRID     ]);

//		cnr=sfxprefs_gui.Pens[PEN_SMP_BACK];
//		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_LINE     ],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_SMP_LINE     ]);
//		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_HIGHLIGHT],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_SMP_HIGHLIGHT]);
//		if(ObtainPen(cm,cols-sfxprefs_gui.Pens[PEN_GUI_BACK     ],pal32.col[cnr][0],pal32.col[cnr][1],pal32.col[cnr][2],PEN_EXCLUSIVE)==-1) ERR1("ObtainPen failed for ix=%d",cols-sfxprefs_gui.Pens[PEN_GUI_BACK     ]);

		MSG("\tafter setting pens");
		RunTime.grad_nr=8;
	}
	else {
		RunTime.grad_nr=0;
	}
	ObtainGradientColors();
	MSG("\tafter preparing gradients");

	InitScrollerWinStuff();
	MSG("\tafter scrollerwin");

	if(!(RunTime.VisualInfo=GetVisualInfo(RunTime.sfxpubscr,TAG_DONE))) { Message(errGetVisualInfo,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	MSG("\tafter getting visual info");

	if(!(RunTime.Menu=CreateMenus(SFXMenu,GTMN_FrontPen,1,GTMN_NewLookMenus,TRUE,GTMN_FullMenu,TRUE,TAG_DONE))) { Message(errCreateMenus,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	if(!LayoutMenus(RunTime.Menu,RunTime.VisualInfo,GTMN_TextAttr,RunTime.ScrFont,TAG_DONE)) { Message(errLayoutMenus,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	if(sfxprefs_misc.showtips) Check_MenuItem(FULLMENUNUM(7, 7,0));	  /* Show Tips */
	if(!RunTime.boolAHIOkay) Disable_MenuItem(FULLMENUNUM(0,12,0));	  /* Record is not available */
	MSG("\tafter creating main pulldown-menu");

	if(!(RunTime.FXMenu=CreateMenus(FXMenu,GTMN_FrontPen,1,GTMN_NewLookMenus,TRUE,GTMN_FullMenu,TRUE,TAG_DONE))) { Message(errCreateMenus,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	if(!LayoutMenus(RunTime.FXMenu,RunTime.VisualInfo,GTMN_TextAttr,RunTime.ScrFont,TAG_DONE)) { Message(errLayoutMenus,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	MSG("\tafter creating fx pulldown-menu");

	/* !!LOCALIZE !! */

	if(PopupMenuBase) {
		if(!(pmenSampleWin=PMMenu(GetString(strSmpOptions)), /* !!! LOCALIZE !!! */
			PMItem(GetString(menCtxDrawMode)),PMSimpleSub,
				PMCheckItem("Lines"     ,PMENID_DRMD_LINES),    PM_Exclude,PM_ExLst(                  PMENID_DRMD_DOTS,PMENID_DRMD_DOTSABS,PMENID_DRMD_DOTSHQ,PMENID_DRMD_FILLED,PMENID_DRMD_FILLEDABS,PMENID_DRMD_FILLEDHQ,0L),PM_UserData,PMENID_DRMD_LINES,     PMEnd,
				PMCheckItem("Dots"      ,PMENID_DRMD_DOTS),     PM_Exclude,PM_ExLst(PMENID_DRMD_LINES,                 PMENID_DRMD_DOTSABS,PMENID_DRMD_DOTSHQ,PMENID_DRMD_FILLED,PMENID_DRMD_FILLEDABS,PMENID_DRMD_FILLEDHQ,0L),PM_UserData,PMENID_DRMD_DOTS,      PMEnd,
				PMCheckItem("DotsAbs"   ,PMENID_DRMD_DOTSABS),  PM_Exclude,PM_ExLst(PMENID_DRMD_LINES,PMENID_DRMD_DOTS,                    PMENID_DRMD_DOTSHQ,PMENID_DRMD_FILLED,PMENID_DRMD_FILLEDABS,PMENID_DRMD_FILLEDHQ,0L),PM_UserData,PMENID_DRMD_DOTSABS,   PMEnd,
				PMCheckItem("DotsHQ"    ,PMENID_DRMD_DOTSHQ),   PM_Exclude,PM_ExLst(PMENID_DRMD_LINES,PMENID_DRMD_DOTS,PMENID_DRMD_DOTSABS,                   PMENID_DRMD_FILLED,PMENID_DRMD_FILLEDABS,PMENID_DRMD_FILLEDHQ,0L),PM_UserData,PMENID_DRMD_DOTSHQ,    PMEnd,
				PMCheckItem("Filled"    ,PMENID_DRMD_FILLED),   PM_Exclude,PM_ExLst(PMENID_DRMD_LINES,PMENID_DRMD_DOTS,PMENID_DRMD_DOTSABS,PMENID_DRMD_DOTSHQ,                   PMENID_DRMD_FILLEDABS,PMENID_DRMD_FILLEDHQ,0L),PM_UserData,PMENID_DRMD_FILLED,    PMEnd,
				PMCheckItem("FilledAbs" ,PMENID_DRMD_FILLEDABS),PM_Exclude,PM_ExLst(PMENID_DRMD_LINES,PMENID_DRMD_DOTS,PMENID_DRMD_DOTSABS,PMENID_DRMD_DOTSHQ,PMENID_DRMD_FILLED,                      PMENID_DRMD_FILLEDHQ,0L),PM_UserData,PMENID_DRMD_FILLEDABS, PMEnd,
				PMCheckItem("FilledHQ"  ,PMENID_DRMD_FILLEDHQ), PM_Exclude,PM_ExLst(PMENID_DRMD_LINES,PMENID_DRMD_DOTS,PMENID_DRMD_DOTSABS,PMENID_DRMD_DOTSHQ,PMENID_DRMD_FILLED,PMENID_DRMD_FILLEDABS,                     0L),PM_UserData,PMENID_DRMD_FILLEDHQ,  PMEnd,
				PMEnd,
			PMEnd,
			PMCheckItem(GetString(menCtxLoop),PMENID_LOOP),PM_UserData,PMENID_LOOP,PMEnd,
			PMItem(GetString(menCtxChannels)),PMSimpleSub,
				PMCheckItem("0",PMENID_CH_0),PM_UserData,PMENID_CH_0,PMEnd,
				PMCheckItem("1",PMENID_CH_1),PM_UserData,PMENID_CH_1,PMEnd,
				PMCheckItem("2",PMENID_CH_2),PM_UserData,PMENID_CH_2,PMEnd,
				PMCheckItem("3",PMENID_CH_3),PM_UserData,PMENID_CH_3,PMEnd,
				PMEnd,
			PMEnd,
			PMBar,PMEnd,
			PMItem(GetString(menCtxHide)),PM_UserData,PMENID_HIDE,PMEnd,
		PMEnd)) { Message(NULL,"Can't create popupmenu",__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	}
	MSG("\tafter ceating popup-menu");

	//MSG("ClientRect :");
	//MSG1("  Width      %6d",RunTime.clientrect.Width);
	//MSG1("  Height     %6d",RunTime.clientrect.Height);
	//MSG1("  LeftEdge   %6d",RunTime.clientrect.LeftEdge);
	//MSG1("  RightEdge  %6d",RunTime.clientrect.RightEdge);
	//MSG1("  TopEdge    %6d",RunTime.clientrect.TopEdge);
	//MSG1("  BottomEdge %6d",RunTime.clientrect.BottomEdge);

	//-- now add a toolbar area at the top
	MSG("\tbefore CreateToolbars");
	if(!(gadToolbar=CreateToolbars(SFXToolbar,RunTime.clientrect.Width,200))) { Message(errOpenWindow,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	MSG("\tbefore OpenToolbar");
	if(Open_ToolbarWin())   { Message(errOpenWindow,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }

	//-- and a status bar at the bottom
	MSG("\tbefore OpenStatusbar");
	if(Open_StatusWin())    { Message(errOpenWindow,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }

	//-- now we open a backdropwindow as big as our client rect
	if(Open_BDWin())        { Message(errOpenWindow,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	oldWinPtr=RunTime.sfxproc->pr_WindowPtr;
	RunTime.sfxproc->pr_WindowPtr=(APTR)RunTime.winBackDrop;

	//-- test for asl.prefs
	if(lock=Lock("env:asl.prefs",ACCESS_READ)) {
		asl_prefs=TRUE;
		UnLock(lock);
	}

	Font_Reqtags_InitialName			=(ULONG)sfxprefs_gui.scrfont_name;
	Font_Reqtags_InitialSize			=sfxprefs_gui.scrfont_size;
	ScrMd_Reqtags_InitialDisplayID		=sfxprefs_gui.scrmode;
	ScrMd_Reqtags_InitialDisplayWidth	=sfxprefs_gui.xres;
	ScrMd_Reqtags_InitialDisplayHeight	=sfxprefs_gui.yres;
	ScrMd_Reqtags_InitialDisplayDepth	=sfxprefs_gui.depth;
	ScrMd_Reqtags_InitialOverscanType	=sfxprefs_gui.oscan;
	ScrMd_Reqtags_InitialAutoScroll		=sfxprefs_gui.autoscroll;
	if(!asl_prefs) {
		MSG("  no env:asl.prefs found");
		File_Reqtags_InitialLeftEdge=Font_Reqtags_InitialLeftEdge=ScrMd_Reqtags_InitialLeftEdge=(sfxprefs_gui.xres-300)>>1;
		File_Reqtags_InitialHeight	=Font_Reqtags_InitialHeight	 =ScrMd_Reqtags_InitialHeight  = sfxprefs_gui.yres-30;
	}
	else {
		MSG("  env:asl.prefs found");
		File_Reqtags[2].ti_Tag=File_Reqtags[3].ti_Tag=File_Reqtags[4].ti_Tag=File_Reqtags[5].ti_Tag=TAG_IGNORE;
		Font_Reqtags[2].ti_Tag=Font_Reqtags[3].ti_Tag=Font_Reqtags[4].ti_Tag=Font_Reqtags[5].ti_Tag=TAG_IGNORE;
		ScrMd_Reqtags[2].ti_Tag=ScrMd_Reqtags[3].ti_Tag=ScrMd_Reqtags[4].ti_Tag=ScrMd_Reqtags[5].ti_Tag=TAG_IGNORE;
	}

	if(!(File_Request   =AllocAslRequest(ASL_FileRequest,File_Reqtags)))        { Message(errAllocateFileRequest,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	if(!(Font_Request   =AllocAslRequest(ASL_FontRequest,Font_Reqtags)))        { Message(errAllocateFontRequest,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	if(!(ScrMd_Request  =AllocAslRequest(ASL_ScreenModeRequest,ScrMd_Reqtags))) { Message(errAllocateScrmdRequest,NULL,__FILE__,__LINE__);CloseAll(CLLEV_GUI,RETURN_FAIL); }
	MSG("\tafter alloc requests");

	Refr_GlobalMenu();
	MSG("\tafter resetting menu");

	apsig   =1L<<appiconport->mp_SigBit;                            // generate shifted signals
	bwsig   =1L<<RunTime.winBackDrop->UserPort->mp_SigBit;
	gwsig   =1L<<gwin_up->mp_SigBit;
	swsig   =1L<<swin_up->mp_SigBit;
	rxsig   =1L<<arexxport->mp_SigBit;
	RunTime.sig_in|=(apsig|bwsig|rxsig);            // build mask, signals for swin are maintained by MWin-Routines

	/* if we have auto-adapted prefs, we tell the user about it<, so that he can save setting */
	//if(writeprefs) {
	//	WritePrefs("ENVARC:sfx.cfg");
	//	WritePrefs("ENV:sfx.cfg");
	//}

	SetStatus(GetString(strReady));

	OUTRO;
}

void GUI_Done(void) {
	struct ViewPort *vp;
	struct ColorMap *cm;
	UWORD cols;

	INTRO;

	if(RunTime.play) StopSample();
	MSG("    after stopping sound-system");

	//-- close all public windows
	if(RunTime.winDropDown) {
		CloseMWindow(RunTime.winDropDown,&RunTime.gwin_upct);RunTime.winDropDown=NULL;
		FreeGadgets(gadDropDown);gadDropDown=NULL;
		//if(owin==RunTime.winDropDown) { owin=NULL;ogad=NULL; }
	}
	MSG("    after cleaning up dropdowns");

	FreeScrollerWinStuff();
	MSG("    after freeing scrolllerwinstuff");

	if(ScrMd_Request)       FreeAslRequest(ScrMd_Request);
	if(Font_Request)        FreeAslRequest(Font_Request);
	if(File_Request)        FreeAslRequest(File_Request);
	MSG("    after freeing requesters");

	if(RunTime.sfxproc)                 RunTime.sfxproc->pr_WindowPtr=oldWinPtr;
	if(RunTime.winBackDrop)             { ClearMenuStrip(RunTime.winBackDrop);CloseWindow(RunTime.winBackDrop);RunTime.winBackDrop=NULL; }
	if(RunTime.winStatus)               { ClearMenuStrip(RunTime.winStatus  );CloseMWindow(RunTime.winStatus ,&RunTime.gwin_upct);RunTime.winStatus =NULL; }
	if(RunTime.winToolbar)              { ClearMenuStrip(RunTime.winToolbar );CloseMWindow(RunTime.winToolbar,&RunTime.gwin_upct);RunTime.winToolbar=NULL; }
	if(gadToolbar)                      FreeToolbars(SFXToolbar);
	if(RunTime.Menu)                    FreeMenus(RunTime.Menu);
	if(RunTime.FXMenu)                  FreeMenus(RunTime.FXMenu);
	if(PopupMenuBase && pmenSampleWin)  PM_FreePopupMenu(pmenSampleWin);
	if(RunTime.VisualInfo)              FreeVisualInfo(RunTime.VisualInfo);

	MSG("    before releasing pens & closing screen");
	if(RunTime.sfxpubscr) {
		if(Main_Scrtags_Depth>4) {
			vp=&RunTime.sfxpubscr->ViewPort;
			cm=vp->ColorMap;
			cols=(1<<Main_Scrtags_Depth)-1;
			ReleasePen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_BACK     ]);
			ReleasePen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_GRID     ]);
			ReleasePen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_LINE     ]);
			ReleasePen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_MAXL     ]);
			ReleasePen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_RMSL     ]);
			ReleasePen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_AVGL     ]);
			ReleasePen(cm,cols-sfxprefs_gui.Pens[PEN_SMP_HIGHLIGHT]);
			ReleasePen(cm,cols-sfxprefs_gui.Pens[PEN_GUI_BACK     ]);
			ReleasePen(cm,cols-sfxprefs_gui.Pens[PEN_GUI_HIGHLIGHT]);
		}
		FreeGradientColors();
		CloseScreen(RunTime.sfxpubscr);
	}
	MSG("    before closing fonts");
	if(RunTime.scafont)     CloseFont(RunTime.scafont);
	if(RunTime.sigfont)     CloseFont(RunTime.sigfont);
	if(RunTime.scrfontb)    CloseFont(RunTime.scrfontb);
	if(RunTime.scrfont)     CloseFont(RunTime.scrfont);
	OUTRO;
}

void GUI_ReOpen(void) {
	SInfo *oldaktbuf=RunTime.aktbuf;

	INTRO;
	Forbid();

	TemporaryHideAllSounds();
	//MSG("  after hiding all Samples");

	if(!(PubScreenStatus(RunTime.sfxpubscr,PSNF_PRIVATE)&(UWORD)0x1)) Message(errVisitorWindows,NULL,__FILE__,__LINE__);
	else {
		GUI_Done();
		//MSG("  after GUI_Done");
		FlushMsgPort(swin_up);
		FlushMsgPort(gwin_up);
		//MSG("  after flushing msgports");
		GUI_Init();
		//MSG("  after GUI_Init");
		if(writeprefs) Message(msgPrefsHaveChanged,NULL,NULL,0);
	}

	ShowAllTemporaryHiddenSounds();
	//MSG("  after reopening samples");

	RunTime.aktbuf=oldaktbuf;
	Refr_ToolbarBuffer();

	ScreenToFront(RunTime.sfxpubscr);

	Permit();
	OUTRO;
}

//-- eof ----------------------------------------------------------------------

