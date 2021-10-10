/****h* sfxsupport.library/sfxsupport.h [4.3] *
*
*  NAME
*    sfxsupport.h
*  COPYRIGHT
*    $VER: sfxsupport.h 4.3 (14.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    shared library for SoundFX
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    25.Aug.1998
*  MODIFICATION HISTORY
*    14.Oct.2003	V 4.3	most recent version
*    19.Feb.2003	V 4.2	most recent version
*    29.May.2002    V 4.1   most recent version
*    25.Aug.1998    V 1.2   initial version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXSUPPORT_H
#define  LIBRARIES_SFXSUPPORT_H

//-- include --------------------------------------------------------------------------------------

//-- Ansi C/C++

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#ifndef PI
#define PI      3.14159265358979323846
#endif
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
#define mypow(x,y) exp(y*log(x))                                // Dank an Kleiner
#define mypow_nn(x,y) ((x!=0.0) ? exp(y*log(x)) : 0.0)          // dummerweise kann x gleich Null sein
#define __mypow(x,y) __exp(y*__log(x))                          // ooops, this took me bloody ages to locate
#define __mypow_nn(x,y) ((x!=0.0) ? __exp(y*__log(x)) : 0.0)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//-- Amiga OS

#ifdef AMIGA
	#include <dos.h>
	#include <m68881.h>         // include math-extensions depending on type of math beeing used
	#include <mffp.h>
	#include <mieeedoub.h>
#else
	//-- mini dummy AmigaOS emulation
	typedef void * Object;
	//-- we do not yet care about MEMF_CLEAR -> calloc(size,1);
	#define AllocVec(size,typ) (((typ&MEMF_CLEAR)!=0)?calloc(size,1):malloc(size))
	#define FreeVec(mem) free(mem);
#endif

#define __USE_SYSBASE
#include <exec/types.h>
#include <cmacros.h>
#include <exec/exec.h>
#include <utility/tagitem.h>

#ifdef AMIGA
	#include <devices/ahi.h>
	#include <dos/dos.h>
	#include <dos/dosextens.h>
	#include <dos/dostags.h>
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
	#include <intuition/screens.h>
	#include <intuition/sghooks.h>
	#include <libraries/listtools.h>
	#include <libraries/openurl.h>
	#include <libraries/unitconv.h>
	#include <rexx/rxslib.h>
	#include <utility/date.h>
	#include <utility/hooks.h>

	#include <clib/alib_protos.h>           /* Prototypes für Supportfunctions */

	#include <proto/ahi.h>                  /* Prototypes für Libraryfunctions */
	#include <proto/configio.h>
	#include <proto/dos.h>
	#include <proto/exec.h>
	#include <proto/gadtools.h>
	#include <proto/graphics.h>
	#include <proto/intuition.h>
	#include <proto/listtools.h>
	#include <proto/locale.h>
	#include <proto/openurl.h>
	#include <proto/unitconv.h>
#else
	#include <libraries/unitconv.h>
	#include <clib/unitconv_protos.h>
#endif

//-- own includes ---------------------------------------------------------------------------------

#ifdef AMIGA
	// defines for chars-codes in Image-Font
	#include <SFX-Images.h>
	// locale message-id defines
	#include <SFX-LocaleID.h>
	// online help-id defines
	#include <SFX-HelpID.h>
#endif

//-- defines --------------------------------------------------------------------------------------

//-- globals

#define SFXSupName  "sfxsupport.library"

#define MAIN_VERSION    4
#define MAIN_REVISION   3

#define SFX_PREFIX		"SFX_"

#define SFX_PUBSCRNAME	SFX_PREFIX"PubScreen"
#define SFX_AREXXNAME	SFX_PREFIX"REXX"

//#define SFX_8BIT
#define SFX_16BIT
//#define SFX_24BIT
//#define SFX_32BIT

//#define SFX_FLOAT
#define SFX_DOUBLE

#ifdef __RUNTIMELOCAL
#define GetRunTime(name) RunTime.name
#else
#define GetRunTime(name) RunTime->name
#endif

//-- configuration

#define CONF_STANDART
//#define CONF_CASSABLANCA

#ifdef CONF_STANDART
#define CONF_NAME ""
#define PROGDIR "progdir:"
#define ENVDIR  "env:sfx/"
#define ENVROOT "env:sfx"
#endif
#ifdef CONF_CASSABLANCA
#define CONF_NAME "(Cassablanca)"
#define PROGDIR "boot:soundfx/"
#define ENVDIR  "boot:soundfx/env/"
#define ENVROOT "boot:soundfx/env"
#endif

//-- GUI Macros

#define FRMTITLE_HEIGHT         8
#define FRMLINE_HEIGHT          (11+1)
#define FRMCONTENT_YOFFSET      (FRMTITLE_HEIGHT+2)
#define FRMTEXT_YPOS(line)      ((FRMTITLE_HEIGHT-3)+(line*FRMLINE_HEIGHT))

#define FRAME_HEIGHT(lines)     ( 2+lines*FRMLINE_HEIGHT)
#define TEXT_Y_POS(lines)       (-3+lines*FRMLINE_HEIGHT)

#define STDFRM_WIDTH            (GetRunTime(scx[308]))

#define SRCSEL_HEIGHT           (GetRunTime(scy[2+FRMLINE_HEIGHT+FRMTITLE_HEIGHT]))
#define SRCSEL_WIDTH            (GetRunTime(scx[308]))

#define MODSEL_HEIGHT           (GetRunTime(scy[2+2*FRMLINE_HEIGHT+FRMTITLE_HEIGHT]))
#define MODSEL_WIDTH            (GetRunTime(scx[308]))

#define CFGSEL_WIDTH            (GetRunTime(scx[130]))
#define CFGSEL_HEIGHT           (winh-4)

#define CMDFRM_HEIGHT           (GetRunTime(scy[2+FRMLINE_HEIGHT]))

#define ANYFRM_HEIGHT(lines)    (GetRunTime(scy[2+lines*FRMLINE_HEIGHT+FRMTITLE_HEIGHT]))

//-- storage dependent defines --------------------------------------------------------------------

//-- the 8-bit version

#ifdef SFX_8BIT
	#define SMP_MAXP            127             // Skalierung & Clipping
	#define SMP_MAXN            (-128)
	#define SMP_MAX             0x100
	#define SMP_MAXMASK         0x0FF
												// Version
	#define VERS_BITS           "8bit"

	typedef BYTE                SAMPLE_STOR;    // Sampletype

	#define SampleAddr(ptr)     (*ptr)          // Access
	#define SampleAddrInc(ptr)  (*(ptr++))
	#define SampleAddrDec(ptr)  (*(ptr--))

	#define Samples2Bytes(x)    (x)             // Convertierung
	#define Bytes2Samples(x)    (x)
#endif

//-- the 16-bit version

#ifdef SFX_16BIT
	#define SMP_MAXP            32767           // Skalierung & Clipping
	#define SMP_MAXN            (-32768)
	#define SMP_MAX             0x10000
	#define SMP_MAXMASK         0x0FFFF
												// Version
	#define VERS_BITS           "16bit"

	typedef WORD                SAMPLE_STOR;    // Sampletype

	#define SampleAddr(ptr)     (*ptr)          // Access
	#define SampleAddrInc(ptr)  (*(ptr++))
	#define SampleAddrDec(ptr)  (*(ptr--))

	#define Samples2Bytes(x)    ((x)<<1)        // Convertierung
	#define Bytes2Samples(x)    ((x)>>1)
#endif

//-- the 24-bit version

#ifdef SFX_24BIT
	#define SMP_MAXP            8388607         // Skalierung & Clipping
	#define SMP_MAXN            (-8388608)
	#define SMP_MAX             0x1000000
	#define SMP_MAXMASK         0x0FFFFFF
												// Version
	#define VERS_BITS           "24bit"

	typedef LONG                SAMPLE_STOR;    // Sampletype

	#define SampleAddr(ptr)     (*ptr)          // Access           Problem
	#define SampleAddrInc(ptr)  (*(ptr++))
	#define SampleAddrDec(ptr)  (*(ptr--))

	#define Samples2Bytes(x)    ((x)*3)         // Convertierung
	#define Bytes2Samples(x)    ((x)/3)
#endif

//-- the 32-bit version

#ifdef SFX_32BIT
	#define SMP_MAXP            2147483647      // Skalierung & Clipping
	#define SMP_MAXN            (-2147483648)
	#define SMP_MAX             0x100000000
	#define SMP_MAXMASK         0x0FFFFFFFF
												// Version
	#define VERS_BITS           "32bit"

	typedef LONG                SAMPLE_STOR;    // Sampletype

	#define SampleAddr(ptr)     (*ptr)          // Access
	#define SampleAddrInc(ptr)  (*(ptr++))
	#define SampleAddrDec(ptr)  (*(ptr--))

	#define Samples2Bytes(x)    ((x)<<2)        // Convertierung
	#define Bytes2Samples(x)    ((x)>>2)
#endif

// temporarily
typedef SAMPLE_STOR SAMPLE;

//-- storage dependent defines --------------------------------------------------------------------

//-- the float version

#ifdef SFX_FLOAT
	#define VERS_PREC			"32bit"
	typedef	float				SAMPLE_CALC;
#endif
//
//-- the double version

#ifdef SFX_DOUBLE
	#define VERS_PREC			"64bit"
	typedef	double				SAMPLE_CALC;
#endif


//-- the version string for windowtitles ----------------------------------------------------------

#define VERS "SoundFX "CONF_NAME" "NUM2STR(MAIN_VERSION)"."NUM2STR(MAIN_REVISION)" "VERS_BITS"/"VERS_PREC" for "_CPUNAME"/"_FPUNAME" © 1993-2004 by Stefan Kost"

#define MAKE_VERSION_STAMP(ver,rev) (((ver)<<16)|(rev))

//-- some handy macros ----------------------------------------------------------------------------

#define CLIPINT(x) \
	if     (x>SMP_MAXP) x=SMP_MAXP; \
	else if(x<SMP_MAXN) x=SMP_MAXN
//enddef

#define CLIPFLT(x) \
	if     (x>(double)SMP_MAXP) x=(double)SMP_MAXP; \
	else if(x<(double)SMP_MAXN) x=(double)SMP_MAXN
//enddef

#define SIGNINT(x) \
	(x>0) ? 1 : (x<0) ? -1 : 0
//enddef

#define SIGNFLT(x) \
	(x>0.0) ? 1 : (x<0.0) ? -1 : 0
//enddef

//-- parameter strings ----------------------------------------------------------------------------

#define PARBUFLEN 25
typedef char Param[PARBUFLEN+1];

//-------------------------------------------------------------------------------------------------

//-- loop variations
#define SFXLOOP_NONE        0
#define SFXLOOP_FORWARD     1
#define SFXLOOP_PINGPONG    2

//-- channel defines
#define CH_MAX      4

#define CH_MONO     0
#define CH_STEREO   1
#define CH_QUADRO   3

#define CH_CENTER   1
#define CH_LEFT     1
#define CH_RIGHT    2
#define CH_FRONT    4
#define CH_BACK     8
#define CH_ALL		0xFF

#define CH_DOUBLE(c)	((((c)+1)<<1)-1)
#define CH_HALF(c)		((((c)+1)>>1)-1)
#define CH_COUNT(c)		((c)+1)
#define CH_MASK(c)		((1<<(c+1))-1)

#define SET_CHANNELS(si,ch) do { si->channels=ch;si->aktch=CH_COUNT(ch);si->channelmask=CH_MASK(ch); } while(0);

#define IS_CHANNEL_ACTIVE(ch,mask) (mask&(1<<ch))

//-- visibillity (SFXSound.visibillity)
#define SFX_SOUND_VISIBILLITY_HIDDEN	 0
#define SFX_SOUND_VISIBILLITY_SHOWN		 1
#define SFX_SOUND_VISIBILLITY_TEMPHIDDEN 2

#define SFX_SOUND_IS_HIDDEN(snd) ((snd)->visibillity==SFX_SOUND_VISIBILLITY_HIDDEN)
#define SFX_SOUND_IS_SHOWN(snd) ((snd)->visibillity==SFX_SOUND_VISIBILLITY_SHOWN)
#define SFX_SOUND_IS_TEMPHIDDEN(snd) ((snd)->visibillity==SFX_SOUND_VISIBILLITY_TEMPHIDDEN)

//-- sample window scroller
#define SLIDER_X        1
#define SLIDER_Y        2
#define SLIDER_BOTH     (SLIDER_X|SLIDER_Y)

//-- one soundbuffer view -------------------------------------------------------------------------

typedef struct {
	struct Node node;                   /* for linking */
	ULONG id;                           /* for management in list */
	ULONG winxs,winxe;                  /* Position inside the window */
	ULONG zoomxs,zoomxl;                /* X zoom start & length */
	ULONG zoomys,zoomyl;                /* Y Zoom start & length */
	UBYTE rasterx,rastery;              /* X/Y-Raster zeichnen ? */
	UBYTE maxlines,rmslines,avglines;   /* Max/RMS/Avg-lines enabled ? */
	UBYTE drawmode;                     /* 0 - 2 (Lines,Dots,DotsAbs,Filled,FilledAbs,FilledHQ..) */
	UBYTE quickdraw;                    /* 0 => off, 1 => on */
} SView;

//-- one soundbuffer range (loop,range,marker) is this useful ??

#define RNG_LOOP    0
#define RNG_RANGE   1
#define RNG_MARKER  2

typedef struct {
	struct Node node;                   /* for linking */
	ULONG id;                           /* for management in list */
	ULONG rngxl,rngxe;                  /* X range start & length */
	ULONG rngys,rngyl;                  /* Y range start & length */
	UBYTE type;                         /* loop, range, maker */
	//...
} SRange;

//-- the soundbuffer info -------------------------------------------------------------------------

/** @TODO split into
 * SFX_SoundMeta : slen, srat, channels, title, anno, ...
 * SFX_SoundRange : list of ranges
 *
 */
typedef struct {
	//-- internal
	struct  Node node;                  /* for linking */
	ULONG   id;                         /* for management in list and referencing */
	SAMPLE *memory[CH_MAX];             /* pointers to memory allocation */
	ULONG   memlen;                     /* how many samples per channel are allocated */
	ULONG   memoffset;                  /* how many samples in the beginning are unused */
	SAMPLE *buf[CH_MAX];                /* real databuffer (0 mono,1.2 stereo, 1.2.3.4 quadro), pointers to inside of memory */
	SAMPLE *vbuf[CH_MAX];               /* virtual databuffer always all four are assigned */
	UWORD   lockct;                     /* how often locked (referenced)?, close only if 0 */

	//-- sound meta data
	ULONG   slen;                       /* SampleLänge (real) */
	ULONG   srat;                       /* SamplePeriode */
	UBYTE   channels;                   /* 0=mono, 1=stereo, 3=quadro */
	UBYTE   loop;                       /* 0=aus,  1=forward */
	//STRPTR  name;                       /* Samplename !!! OBSOLETE !!! */
	STRPTR  cpyr;                       /* Copyright */
	STRPTR  auth;                       /* Author */
	STRPTR  anno;                       /* Kommentar */
	STRPTR  title;                      /* Titelzeile */

	//-- graphical data and resources
	struct  Window *win;                /* SampleWindow */
	struct  RastPort *rp;               /* RastPort des Windows */
	//struct  Gadget *GList;      		  /* GadgetListe !!! OBSOLETE !!! */
	struct  Gadget *Scroller;           /* Scrollergadget */
	Object *scrobj[10];                 /* Objects in windowframe */
	UWORD   xres,yres;                  /* Fenstergröße */
	UWORD   xpos,ypos;                  /* Fensterpos. */
	UWORD   width,height;               /* äußere Fenstergr. */

	//-- SRange -> should be a list instead ?
	ULONG   loopl,loops;                /* LoopData */
	ULONG   markxl,markxs;              /* XMarkData */
	ULONG   markyl,markys;              /* YMarkData */

	//-- SView -> should be a list instead ?
	ULONG   zoomxl,zoomxs;              /* XZoomData */
	ULONG   zoomyl,zoomys;              /* YZoomData */
	UBYTE   rasterx,rastery;            /* draw x/y-grid ? */
	UBYTE   maxlines,rmslines,avglines; /* draw max-lines ? */
	UBYTE   drawmode;                   /* Lines,Dots,DotsAbs,Filled,FilledAbs,FilledHQ,... */
	UBYTE   quickdraw;                  /* 0=off, 1=on */

	//-- more internal stuff
	UBYTE   saved,safemode;             /* already saved ?, Sicherheitsabfrage */
	UBYTE   aktch,channelmask;          /* how many and which channels are active */
	UBYTE   chnr[CH_MAX];               /* mapping of visible to real channels; usually 0,1,2,3,... */
	UBYTE   visibillity;                /* visible: 0=no, 1=yes, 2=temporaryly hidden */
	UBYTE   mark;                       /* is something marked ? */
	UBYTE   storage;                    /* how to access sample data, see APTR storageinfo */
	ULONG   xo,yo;                      /* border offsets (+ruler offsets) (left,top) */
	UWORD   scaxs,scays;                /* size of the rulers */
	UBYTE   scalax,scalay;              /* rulers for x or y-axis */
	UBYTE   unitx,unity;                /* axis units */
	WORD    smin[CH_MAX],smax[CH_MAX];  /* Min-, Max-values */
	WORD    srms[CH_MAX],savg[CH_MAX];  /* RMS-, AVG-values */
	UBYTE   lines;                      /* have #?lines already been calculated */
	UBYTE   crs;
	UWORD   cx,cy;

	UBYTE   extid[20];                  /* Loadermodules can now attach some data here, which they may reuse when saving */
	ULONG   extlength;                  /* how many bytes is extdata */
	APTR    extdata;

	APTR    storageinfo;                /* addition information about storage, depends on UBYTE storage */
} SInfo;

#define SAMPLENAMELEN 255
typedef char SampleName[SAMPLENAMELEN+1];

typedef struct {
	UBYTE dummy;
} StorageDrive;

typedef struct {
	BYTE            mainsig,worksig,__pad[2];           // for communication
	struct Process  *maintask,*worktask;
	ULONG           blen;
	volatile UBYTE  *state;
} StorageStream;

//-------------------------------------------------------------------------------------------------

#define swin_idcmps IDCMP_CLOSEWINDOW|IDCMP_CHANGEWINDOW|                 IDCMP_RAWKEY|IDCMP_ACTIVEWINDOW|                     IDCMP_MENUPICK|IDCMP_MOUSEMOVE|IDCMP_NEWSIZE|IDCMP_MOUSEBUTTONS|SCROLLERIDCMP|ARROWIDCMP|IDCMP_IDCMPUPDATE
#define lwin_idcmps IDCMP_CLOSEWINDOW|IDCMP_CHANGEWINDOW|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_ACTIVEWINDOW|                     IDCMP_MENUPICK|IDCMP_MOUSEMOVE|IDCMP_NEWSIZE|BUTTONIDCMP|LISTVIEWIDCMP
#define nwin_idcmps IDCMP_CLOSEWINDOW|IDCMP_CHANGEWINDOW|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_ACTIVEWINDOW|                     IDCMP_MENUPICK|IDCMP_MOUSEMOVE|IDCMP_GADGETDOWN|IDCMP_GADGETUP|IDCMP_INTUITICKS
#define gwin_idcmps                                      IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_ACTIVEWINDOW|                     IDCMP_MENUPICK|IDCMP_MOUSEMOVE|IDCMP_GADGETDOWN|IDCMP_GADGETUP|IDCMP_INTUITICKS|LISTVIEWIDCMP
#define dwin_idcmps                                      IDCMP_VANILLAKEY|IDCMP_RAWKEY|                   IDCMP_INACTIVEWINDOW|IDCMP_MENUPICK|IDCMP_MOUSEMOVE|IDCMP_GADGETDOWN|IDCMP_GADGETUP|IDCMP_INTUITICKS|LISTVIEWIDCMP

//-- Windowtypes ----------------------------------------------------------------------------------

// can be found in win->UserData

#define WT_Basic    ((BYTE *)0)
#define WT_ProWin   ((BYTE *)1)
#define WT_BackDrop ((BYTE *)2)

//-- helpnode basenames for modules ---------------------------------------------------------------

// now defined in cfg.h files too
#define HELPBASE_OP  "node02.01.06"
#define HELPBASE_LO  "node02.02.01"
#define HELPBASE_PL  "node02.03.01"
#define HELPBASE_RX  "node02.04.01"
#define HELPBASE_SA  "node02.05.01"

//-- Samplewindow Border Gadget ID's --------------------------------------------------------------

#define GADID_SWB_HORIZ		0
#define GADID_SWB_VERT		1
#define GADID_SWB_LEFT		2
#define GADID_SWB_RIGHT		3
#define GADID_SWB_UP		4
#define GADID_SWB_DOWN		5
#define GADID_SWB_ICONIFY	6
#define GADID_SWB_POPUP		7
#define SWB_GADANZ			10

//-- Pennumbers -----------------------------------------------------------------------------------

#define PEN_GUI_BACK            0
#define PEN_GUI_SHINE           (1+PEN_GUI_BACK)
#define PEN_GUI_SHADOW          (1+PEN_GUI_SHINE)
#define PEN_GUI_HIGHLIGHT       (1+PEN_GUI_SHADOW)
#define PEN_GUI_COLORTEXT       (1+PEN_GUI_HIGHLIGHT)
#define PEN_GUI_COLORTEXTGLOW   (1+PEN_GUI_COLORTEXT)
#define PEN_GUI_GRADIENTSTART   (1+PEN_GUI_COLORTEXTGLOW)
#define PEN_GUI_GRADIENTEND     (1+PEN_GUI_GRADIENTSTART)

#define PEN_SMP_BACK            (1+PEN_GUI_GRADIENTEND)
#define PEN_SMP_TEXT            (1+PEN_SMP_BACK)
#define PEN_SMP_LINE            (1+PEN_SMP_TEXT)
#define PEN_SMP_GRID            (1+PEN_SMP_LINE)
#define PEN_SMP_MAXL            (1+PEN_SMP_GRID)
#define PEN_SMP_RMSL            (1+PEN_SMP_MAXL)
#define PEN_SMP_AVGL            (1+PEN_SMP_RMSL)
#define PEN_SMP_HIGHLIGHT       (1+PEN_SMP_AVGL)
#define PEN_SMP_LOOP            (1+PEN_SMP_HIGHLIGHT)
#define PEN_SMP_MARK            (1+PEN_SMP_LOOP)

/** @TODO: add PEN_SMP_LINE_DARK,PEN_SMP_LINE_BRIGHT */

//-- Arexx Parsing --------------------------------------------------------------------------------

#define MaxParAnz   10

#define MaxParLen    200
#define MaxParLen2   400
#define MaxParLen3   600
#define MaxParLen4   800
#define MaxParLen5  1000
#define MaxParLen6  1200

#define MaxCmdLen   2200

#define RexxPar0    &params[0]
#define RexxPar1    &params[MaxParLen]
#define RexxPar2    &params[MaxParLen2]
#define RexxPar3    &params[MaxParLen3]
#define RexxPar4    &params[MaxParLen4]
#define RexxPar5    &params[MaxParLen5]
#define RexxPar6    &params[MaxParLen6]

//-- Prefs ----------------------------------------------------------------------------------------

typedef struct {                                /* GUI */
	ULONG   scrmode;                                /* ScreenMode */
	UWORD   xres,yres;                              /* Auflösung */
	UBYTE   oscan;                                  /* Overscan */
	UBYTE   autoscroll;                             /* Autoscroll */
	#define SFXPrefs_Gui_MaxColors      16
	UBYTE   ScreenColors[SFXPrefs_Gui_MaxColors][3];/* Bildschirmfarben (48 Bytes) */
	UBYTE   Pens[20];                               /* Pennumbers */
	#define SFXPrefs_Gui_FontNameLen    80
	char    scrfont_name[SFXPrefs_Gui_FontNameLen]; /* Fontname */
	UBYTE   scrfont_size;                           /* Fontgröße */
	UBYTE   depth;                                  /* Screendepth */
	char    statfont_name[SFXPrefs_Gui_FontNameLen];/* Statusfield Fontname */
	UBYTE   statfont_size;                          /* Statusfield Fontgröße */
} SFXPrefs_Gui;

typedef struct {                            /* neue Struktur */
	UWORD x1,y1,x2,y2;                     	/* Pos and Size */
	UBYTE state;                            /* Zustand : 0->closed/1->opened */
	//UBYTE pad;
	UBYTE entryname[50];                    /* angewählter Eintrag */
} SFXPrefs_LWin;

typedef struct {
	UWORD x1,y1;                            /* Pos */
	UBYTE state;                            /* Zustand : 0->closed/1->opened */
	//UBYTE pad;
} SFXPrefs_NWin;

typedef struct {
	SFXPrefs_LWin   opwin;
	SFXPrefs_LWin   rxopwin;
	SFXPrefs_LWin   lowin;
	SFXPrefs_LWin   sawin;
	SFXPrefs_LWin   plwin;
	SFXPrefs_LWin   bufwin;
	SFXPrefs_NWin   zowin;
	SFXPrefs_NWin   edwin;
	SFXPrefs_NWin   rngwin;
} SFXPrefs_Win;

typedef struct {                                /* MISC */
	UBYTE sig_a;                                    /* Wenn mit Berechnung fertig DisplayBeep() */
	UBYTE sig_s;                                    /* Wenn mit Berechnung fertig ScreenToFont() */
	UBYTE backlogo;                                 /* Backlogo on/off */
	UBYTE showtips;									/* show tips on startup */
	ULONG keynr;                                    /* Keynr, wenn die sich ändert ist etwas suspect */
	UBYTE realmem,virtmem;                          /* soll freier realer und virtueller Speicher angezeigt werden */
	UBYTE srcselect;                                /* source select mode (list,cycle) */
	UBYTE contextbutton;                            /* context-menu mouse-button (middle,right) */
	UBYTE logmode;                                  /* 0=never,1=in_quiet_mode,2=always */
	#define SFXPrefs_Misc_LogPathLen        256
	char  log_path[SFXPrefs_Misc_LogPathLen];       /* Pfad für Logdatei */
	UBYTE askexit;									/* 0=never,1=loaded-samples,2=unsaved-sample,3=always */
	UBYTE onewindow;								/* 0=multi-window,1=one-window */
	UBYTE saveicons;								/* save icons with samples ? */
	UBYTE deftool;									/* modify default tool : 0=never,1=if-no-def-tool,2=always */
} SFXPrefs_Misc;

#define LOGMODE_NEVER       0
#define LOGMODE_QUIETMODE   1
#define LOGMODE_ALWAYS      2

#define ASKEXIT_NEVER		0
#define ASKEXIT_LOADED		1
#define ASKEXIT_UNSAVED		2
#define ASKEXIT_ALWAYS		3

typedef struct {                                /* SBUF */
	UBYTE drawmode;                                 /* 0 - 2 (Lines,Dots,Filled) */
	UBYTE maxlines;                                 /* Maxlines zeichnen ? */
	UBYTE rasterx,rastery;                          /* X/Y-Raster zeichnen ? */
	UBYTE safemode;                                 /* Sicherheitsabfrage */
	UBYTE quickdraw;                                /* schnelles Zeichnen beim Scrollen ? */
	UWORD xs1,ys1,xs2,ys2;                          /* Normal and Zoomed Size */
	UBYTE mode;                                     /* Sizemode */
	UBYTE storage;                                  /* Speicherungsform */
	UBYTE rmslines,avglines;                        /* RMS-,AVG Linien zeichnen */
	UWORD scaxs,scays;                              /* Scalengröße */
	UBYTE scalax,scalay;                            /* Scala auf der X bzw. Y-Achse */
	#define SFXPrefs_SBuf_SamplePathLen     256
	char  sample_lo_path[SFXPrefs_SBuf_SamplePathLen];  /* Defaultpfad für Load-Filerequester */
	char  sample_sa_path[SFXPrefs_SBuf_SamplePathLen];  /* Defaultpfad für Save-Filerequester */
	#define SFXPrefs_SBuf_FontNameLen       80
	char  scafont_name[SFXPrefs_SBuf_FontNameLen];  /* Scalenfont */
	UBYTE scafont_size;
	//UBYTE pad2[1];
	UBYTE unitx;                                    /* Samples,Time,SMPTE-MF,SMPTE-PF,SMPTE-NF */
	UBYTE unity;                                    /* Factor,Prozent,Promille,Level,Dezibel */
	UBYTE nolines;                                  /* should sfx disable #?line-calculation for big samples */
	ULONG thrsize;                                  /* and by the way, what is *big*? */
	#define SFXPrefs_SBuf_InfoStrLen        256
	char  copystr[SFXPrefs_SBuf_InfoStrLen];        /* default info strings : copyright */
	char  authstr[SFXPrefs_SBuf_InfoStrLen];        /* default info strings : author */
} SFXPrefs_SBuf;

#define SIZEMODE_ABSOLUTE   0
#define SIZEMODE_RELATIVE   1

typedef struct {                            /* VMEM */
	#define SFXPrefs_VMem_SwapPathLen   256
	UBYTE swap_path[SFXPrefs_VMem_SwapPathLen];     /* wohin sollen die Samples ausgelagert werden */
	ULONG blksize;
	UBYTE enabled;
} SFXPrefs_VMem;

//-- Notification ---------------------------------------------------------------------------------

typedef struct {
	struct Node node;           /* für Verkettung */
	struct Task *task;          /* who to notify */
	LONG        sig;            /* and how */
} NotifyTask;

//-- Client Rect : ScreenArea available for views -------------------------------------------------

struct ClientRect {
	UWORD LeftEdge,TopEdge;
	UWORD RightEdge,BottomEdge;
	UWORD Width,Height;
};

//-- include module specific parts ----------------------------------------------------------------

#if defined(SFXLibrary) || defined(SFXMain) || defined(SFXMod_Operator) || defined(SFXMod_Saver)
	#include <libraries/sfxsource.h>
#endif

#if defined(SFXLibrary) || defined(SFXMod_Operator)
	#include <libraries/sfxmodulator.h>
	#include <libraries/sfxinterpolator.h>
	#include <libraries/sfxwinfunction.h>
#endif

// needed everywhere
#include <libraries/sfxcfgsel.h>
#include <libraries/sfxplayeropts.h>
#include <libraries/sfxvm.h>
#include <libraries/sfxprowin.h>

#if defined(SFXLibrary) || defined(SFXMod_Operator) || defined(SFXMod_Saver) || defined(SFXMod_Loader)
	#include <libraries/sfxprefsdata.h>
#endif

#if defined(SFXLibrary) || defined(SFXMod_Saver)
	#include <libraries/sfxsavedata.h>
#endif

#if defined(SFXLibrary) || defined(SFXMod_Loader)
	#include <libraries/sfxloaddata.h>
#endif

//#if defined(SFXLibrary) || defined(SFXMain)
	#include <libraries/sfxguiext.h>
//#endif

//-- Shared memory data structure -----------------------------------------------------------------

/****** sfxsupport.libary/RTime [1.20] *
*
*  NAME
*    RTime -- runtime shared memory structure
*  FUNCTION
*    share data between SoundFX, sfxsupport.libray & SoundFX-modules
*  NOTES
*    try to move more items into local objects
*  BUGS
*
*  SEE ALSO
*
**********
*/

typedef struct {
	/*------------------------------------------------ GUI Globals */
	double              yh;
	float               scalex,scaley;                          /* x,y -scalefactors */
	ULONG               sig_in;                                 /* Signalmaske */
	UWORD               scx[750];                               /* Skalierungstabellen für GUI */
	UWORD               scy[300];
	UWORD               xres,yres;                              /* aktuelle Auflösung */
	UWORD               fxoff,looff,saoff,ploff;                /* Listenoffsets für guides */
	struct Screen       *sfxpubscr;                             /* der eingene PubScreen */
	struct TextFont     *scrfont,*scrfontb,*sigfont,*scafont,*statfont;   /* Fonts */
	struct TextAttr     *ScrFont,*ScrFontB,*SigFont,*ScaFont,*StatFont;
	APTR                VisualInfo;                             /* VisualInfo for GUI */
	struct Menu         *Menu,*FXMenu;                          /* die Menühandles */
	//char                fontname[80];                           /* Fontname (seems 08.Feb.2004 we need it) */
	struct MsgPort      *swin_up;                               /* Samplewindow UserPort */
	/*------------------------------------------------ OnLine Hilfe */
	//AMIGAGUIDECONTEXT aghandle;                               /* AmigaguideHandle */
	//STRPTR            *agcontents;                            /* Hilfenodes */
	//ULONG             agsig;                                  /* Sigbit für AGuide */
	/*------------------------------------------------ verschiedene Globals */
	UBYTE               rexxactive;                             /* Ist eine Funktion von einem REXX-Scripts aus aufgerufen worden ? */
	UBYTE               quitsfx;                                /* Programm beenden ? */
	UBYTE               aloader,asaver,aplayer,aktfx,arxop;     /* aktuelles Modul */
	UBYTE               anzlo,anzsa,anzpl,anzfx,anzrxop;        /* Anzahl der Module */
	UBYTE               loopp;                                  /* aktiver LoopPointer 0=LoopS 1=LoopL */
	UBYTE               hfilter;                                /* Status der PowerLED */
	UBYTE               periplay;                               /* Play im Periodenfenster */
	UBYTE               rngmove;                                /* RangeMove ? */
	UBYTE               play,plstop;                            /* Abspielen ? */
	UBYTE               rngmode,rnglock,movemode;               /* Rangewindow */
	UBYTE               zoommode;                               /* X / Y-Zoom */
	UBYTE               status;                                 /* Status => 0=Demoversion */
	UBYTE               swin_upct,gwin_upct;                    /* Open/CLoseMWindow counter */
	UBYTE               laktsamples,aktsamples;
	UBYTE               status2,status3;                        /* Kopien der Statusflags */
	UBYTE               tryquit;                                /* finishing all the mesageloops, before quitting */
	UBYTE               tryreopen;                              /* finishing all the mesageloops, before reopen GUI */
	UBYTE               changesamples;                          /* number of samples has changed */
	UBYTE               samplestat;                             /* is a sample the first one opened or last one closed or nothing special */
	UBYTE               swallowactivate;                        /* ignore activate window event */
	LONG                rngdiff;                                /* Richtung / Betrag */
	ULONG               mx1,mx2;                                /* Werte unter dem Mauszeiger für akt Sample bzw. Markhelp */
	WORD                my1,my2;
	struct Process      *sfxproc;                               /* unser Hauptprozess */
	struct List         gblist;                                 /* Pointer to Bufferlist */
	ULONG               srclist[10];                            /* SInfo IDs source-buffers */
	ULONG               dstlist[10];                            /* SInfo IDs destination-buffers */
	struct List         *blist;                                 /* Pointer to BufferList */
	UBYTE               **barray;                               /* Pointer to BufferArray, wird aus BufferList generiert */
	SInfo               *aktbuf;                                /* currently selected buffer */
	UBYTE				onewindow;								/* one window mode */
	/*------------------------------------------------ Gui Extensions */
	struct StringExtend *stringextend;                          /* Pointer to global StringExtend */
	struct Hook         *tablelvhook,*colorlvhook;              /* ListViewHooks */
	struct ClientRect   clientrect;                             /* freier Fensterbereich */
	struct Window       *winStatus;                             /* Statusanzeigefenster */
	struct Window       *winToolbar;                            /* Fenster für Toolbars */
	struct Window       *winDropDown;                           /* Fenster für DropDown-Elemente der Toolbars */
	struct Window       *winBackDrop;                           /* backdrop window */
	/*------------------------------------------------ Daten für Playroutinen */
	SInfo               *plsi;                                  /* was wird gespielt */
	UBYTE               *plst,*plst2;                           /* Pointer to Playervariables */
	struct AHIAudioCtrl *audioctrl;                             /* AHI-Stuff */
	struct Library      *AHIBase;
	OPTP_Amiga_8bit     *optp_Amiga_8bit;                       /* Playeroptions */
	OPTP_Amiga_14bit    *optp_Amiga_14bit;
	OPTP_Amiga_14cbit   *optp_Amiga_14cbit;
	OPTP_Ahi            *optp_Ahi;
	UBYTE               boolPaulaOkay,boolCaCaOkay,boolAHIOkay; /* für Player */
	ULONG               plptr,oldptr,plslen;                    /* Abspielpositionen und ~länge */
	ULONG               pllstart,pldiff;                        /* Loopstarts und ~rücksprünge */
	SmpAccess			**plsa;									/* SampleAccess handles */
	/*-----------------------------------------------  Konfigurationsdaten */
	SFXPrefs_SBuf       *sfxprefs_sbuf;                         /* Samplebuffer */
	SFXPrefs_VMem       *sfxprefs_vmem;                         /* virtueller Speicher */
	SFXPrefs_Gui        *sfxprefs_gui;                          /* graphische Nutzerschnittstelle */
	SFXPrefs_Misc       *sfxprefs_misc;                         /* verschiedenes */
	/*------------------------------------------------ Keyfiledaten */
	char                username[85];                           /* the name of user, taken from keyfile */
	ULONG               keynr;                                  /* Keydaten */
	UBYTE               keymi,keyma,keymabits,keyrv;
	UBYTE               *keypd,*keycd;
	UBYTE               *keyrowd,*keycold;
	UBYTE               keyerr;                                 /* Fehlercode */
	/*------------------------------------------------ GUI enhancements (gradients) */
	UBYTE               grad_cmap[32];
	UWORD               grad_nr;
	/*------------------------------------------------ new ones */
	UBYTE               quiet;                                  /* do things quietely (msgs->log,no windows,...) */
	PWData              *pwd;                                   /* for embedding a progressbar */
	UBYTE				os_version;								/* differenciate between OS3.x=39 and OS4.X=50 */
	MPointer			*MPtrCrossHair,*MPtrMove,*MPtrRange;	/* custom mouse pointers */
	MPointer			*MPtrSize64,*MPtrSize73,*MPtrSize82,*MPtrSize91;
} RTime;

#endif   /* LIBRARIES_SFXSUPPORT_H */

//-- eof ----------------------------------------------------------------------
