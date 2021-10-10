/****h* sfxsupport.libary/sfxsupport.h [1.20] *
*
*  NAME
*    sfxsupport.h
*  COPYRIGHT
*    $VER: sfxsupport.h 1.20 (02.10.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    shared library for SoundFX
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    25.Aug.1998
*  MODIFICATION HISTORY
*    02.Oct.2000	V 1.20	most recent version
*    25.Aug.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXSUPPORT_H
#define  LIBRARIES_SFXSUPPORT_H

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
#define mypow(x,y) exp(y*log(x))								// Dank an Kleiner
#define mypow_nn(x,y) ((x!=0.0) ? exp(y*log(x)) : 0.0)			// dummerweise kann x gleich Null sein
#define __mypow(x,y) __exp(y*__log(x))							// ooops, this took me bloody ages to locate
#define __mypow_nn(x,y) ((x!=0.0) ? __exp(y*__log(x)) : 0.0)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//-- Amiga OS

#define __USE_SYSBASE
#include <exec/types.h>
#include <cmacros.h>
#include <devices/ahi.h>
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
#include <intuition/screens.h>
#include <intuition/sghooks.h>
#include <libraries/amigaguide.h>
#include <libraries/listtools.h>
#include <libraries/openurl.h>
#include <libraries/unitconv.h>
#include <rexx/rxslib.h>
#include <utility/date.h>
#include <utility/hooks.h>
#include <utility/tagitem.h>

#include <clib/alib_protos.h>			/* Prototypes für Supportfunctions */

#include <proto/amigaguide.h>			/* Prototypes für Libraryfunctions */
#include <proto/ahi.h>
#include <proto/asl.h>
#include <proto/configio.h>
#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/listtools.h>
#include <proto/locale.h>
#include <proto/openurl.h>
#include <proto/rexxsyslib.h>
#include <proto/unitconv.h>
#include <proto/utility.h>

//-- own includes ---------------------------------------------------------------------------------

#include "project:sfx-main/SFX-Optstructs.h"
#include "project:sfx-main/SFX-Images.h"
#include "project:sfx-main/SFX-LocaleId.h"

//-- debug ----------------------------------------------------------------------------------------

#ifdef DEBUG
	#define LIBMyAllocMem(size,type) AllocMem(size,type)
	#define LIBMyFreeMem(addr,size) FreeMem(addr,size)
	#define MyAllocMem(size,type) AllocMem(size,type)
	#define MyFreeMem(addr,size) FreeMem(addr,size)
#endif

//-- defines --------------------------------------------------------------------------------------

#define	SFXSupName	"sfxsupport.library"

#define	MAIN_VERSION	4
#define	MAIN_REVISION	1

#define SFX_PUBSCRNAME "SFX-PubScreen"

//#define SFX_8BIT
#define SFX_16BIT
//#define SFX_24BIT
//#define SFX_32BIT

#ifdef __RUNTIMELOCAL
#define GetRunTime(name) RunTime.name
#else
#define GetRunTime(name) RunTime->name
#endif

#define FRMTITLE_HEIGHT			8
#define FRMLINE_HEIGHT			(11+1)
#define FRMCONTENT_YOFFSET		(FRMTITLE_HEIGHT+2)
#define FRMTEXT_YPOS(line)		((FRMTITLE_HEIGHT-3)+(line*FRMLINE_HEIGHT))

#define FRAME_HEIGHT(lines)		( 2+lines*FRMLINE_HEIGHT)
#define TEXT_Y_POS(lines)		(-3+lines*FRMLINE_HEIGHT)

#define STDFRM_WIDTH			(GetRunTime(scx[308]))

#define SRCSEL_HEIGHT			(GetRunTime(scy[2+FRMLINE_HEIGHT+FRMTITLE_HEIGHT]))
#define SRCSEL_WIDTH			(GetRunTime(scx[308]))

#define MODSEL_HEIGHT			(GetRunTime(scy[2+2*FRMLINE_HEIGHT+FRMTITLE_HEIGHT]))
#define MODSEL_WIDTH			(GetRunTime(scx[308]))

#define CFGSEL_WIDTH			(GetRunTime(scx[130]))
#define CFGSEL_HEIGHT			(winh-4)

#define CMDFRM_HEIGHT			(GetRunTime(scy[2+FRMLINE_HEIGHT]))

#define ANYFRM_HEIGHT(lines)	(GetRunTime(scy[2+lines*FRMLINE_HEIGHT+FRMTITLE_HEIGHT]))


#define boolSuccess 1
#define boolFail	0

#define ProWinEnd				0xFFFFFFD0
#define ProWinAborted(pos)		(pos>=ProWinEnd)
#define ProWinRunning(pos)		(pos< ProWinEnd)

//-- storagedependent defines ---------------------------------------------------------------------

//-- the 8-bit version

#ifdef SFX_8BIT
	#define SMP_MAXP			127				// Skalierung & Clipping
	#define SMP_MAXN			-128
	#define SMP_MAX				0x100
	#define SMP_MAXMASK			0x0FF
												// Version
	#define	VERS_BITS			"8bit"

	typedef BYTE				SAMPLE;			// Sampletype

	#define SampleAddr(ptr)		(*ptr)			// Access
	#define SampleAddrInc(ptr)	(*(ptr++))
	#define SampleAddrDec(ptr)	(*(ptr--))

	#define Samples2Bytes(x)	(x)				// Convertierung
	#define Bytes2Samples(x)	(x)
#endif

//-- the 16-bit version

#ifdef SFX_16BIT
	#define SMP_MAXP			32767			// Skalierung & Clipping
	#define SMP_MAXN			-32768
	#define SMP_MAX				0x10000
	#define SMP_MAXMASK			0x0FFFF
												// Version
	#define	VERS_BITS			"16bit"

	typedef WORD				SAMPLE;			// Sampletype

	#define SampleAddr(ptr)		(*ptr)			// Access
	#define SampleAddrInc(ptr)	(*(ptr++))
	#define SampleAddrDec(ptr)	(*(ptr--))

	#define Samples2Bytes(x)	((x)<<1)		// Convertierung
	#define Bytes2Samples(x)	((x)>>1)
#endif

//-- the 24-bit version

#ifdef SFX_24BIT
	#define SMP_MAXP			8388607			// Skalierung & Clipping
	#define SMP_MAXN			-8388608
	#define SMP_MAX				0x1000000
	#define SMP_MAXMASK			0x0FFFFFF
												// Version
	#define	VERS_BITS			"24bit"

	typedef LONG				SAMPLE;			// Sampletype

	#define SampleAddr(ptr)		(*ptr)			// Access			Problem
	#define SampleAddrInc(ptr)	(*(ptr++))
	#define SampleAddrDec(ptr)	(*(ptr--))

	#define Samples2Bytes(x)	((x)*3)			// Convertierung
	#define Bytes2Samples(x)	((x)/3)
#endif

//-- the 32-bit version

#ifdef SFX_32BIT
	#define SMP_MAXP			2147483647		// Skalierung & Clipping
	#define SMP_MAXN			-2147483648
	#define SMP_MAX				0x100000000
	#define SMP_MAXMASK			0x0FFFFFFFF
												// Version
	#define	VERS_BITS			"32bit"

	typedef LONG				SAMPLE;			// Sampletype

	#define SampleAddr(ptr)		(*ptr)			// Access
	#define SampleAddrInc(ptr)	(*(ptr++))
	#define SampleAddrDec(ptr)	(*(ptr--))

	#define Samples2Bytes(x)	((x)<<2)		// Convertierung
	#define Bytes2Samples(x)	((x)>>2)
#endif

//-- the version string for windowtitles ----------------------------------------------------------

#define	VERS "SoundFX "NUM2STR(MAIN_VERSION)"."NUM2STR(MAIN_REVISION)" "VERS_BITS" for "_CPUNAME"/"_FPUNAME" © 1993-2000 by Stefan Kost"

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

//-------------------------------------------------------------------------------------------------

typedef double __asm (*BShapeFuncPtr)(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));

//-------------------------------------------------------------------------------------------------

#define SFXLOOP_NONE		0
#define SFXLOOP_FORWARD		1
#define SFXLOOP_PINGPONG	2

//-------------------------------------------------------------------------------------------------

#define CM_SFX  0
#define CM_REXX 1

//-- the soundbuffer info -------------------------------------------------------------------------

typedef struct
{
	struct Node	node;					/* für Verkettung */
	ULONG id;							/* für Verwaltung in Liste */
	WORD *buf[4];						/* real datatbuffer (0 mono,1.2 stereo, 1.2.3.4 quadro) */
	WORD *vbuf[4];						/* virtual databuffer always all four are assigned */
	ULONG slen;							/* SampleLänge */
	ULONG srat;							/* SamplePeriode */
	ULONG loopl,loops;					/* LoopData */
	ULONG markxl,markxs;				/* XMarkData */
	ULONG markyl,markys;				/* YMarkData */
	ULONG zoomxl,zoomxs;				/* XZoomData */
	ULONG zoomyl,zoomys;				/* YZoomData */
	char *cpyr;							/* Copyright */
	char *auth;							/* Author */
	char *anno;							/* Kommentar */
	char *title;						/* Titelzeile */
	struct Window *win;					/* SampleWindow */
	struct RastPort *rp;				/* RastPort des Windows */
	struct Gadget *GList;				/* GadgetListe */
	struct Gadget *Scroller;			/* Scrollergadget */
	UWORD xres,yres;					/* Fenstergröße */
	UWORD xpos,ypos;					/* Fensterpos. */
	UWORD width,height;					/* aüßere Fenstergr. */
	UBYTE rasterx,rastery;				/* X/Y-Raster zeichnen ? */
	UBYTE maxlines,rmslines,avglines;	/* Maxlines zeichnen ? */
	UBYTE saved,safemode;				/* gespeichert ?, Sicherheitsabfrage */
	UBYTE drawmode;						/* 0 - 2 (Lines,Dots,DotsAbs,Filled,FilledAbs,FilledHQ..) */
	UBYTE channels;						/* 0 = mono, 1=stereo, 3=quadro */
	UBYTE aktch,channelmask;			/* wieviele und welche Kanäle aktiv sind */
	UBYTE chnr[4];						/* mapping des sichtbaren auf den tatsächlichen Kanal; normalerweise 0,1,2,3 */
	UBYTE visual;						/* 1 => ja, 0 => nein */
	UBYTE mark;							/* ist was markiert ? */
	UBYTE loop;							/* 0 => aus, 1 => an */
	UBYTE quickdraw;					/* 0 => aus, 1 => an */
	UBYTE storage;						/* wie auf das Sample zugegriffen wird */
	UWORD lockct;						/* how often locked ?, close only if 0 */
	Object *scrobj[10];					/* Objects in windowframe */
	ULONG xo,yo;						/* Borderoffsets (+Scalaoffsets) (links,oben) */
	UWORD scaxs,scays;					/* Scalengröße */
	UBYTE scalax,scalay;				/* Scala auf der X bzw. Y-Achse */
	UBYTE unitx,unity;					/* Achseneinheiten */
	WORD smin[4],smax[4];				/* Min-, Max-werte */
	WORD srms[4],savg[4];				/* RMS-, AVG-werte */
	UBYTE lines;						/* are they yet calculated */
	UBYTE crs;
	UWORD cx,cy;
	UBYTE	extid[20];					/* Loadermodules can now attach some data here, which they may reuse when saving */
	ULONG	extlength;					/* how many bytes is extdata */
	APTR	extdata;
} SInfo;

#define CH_MONO		0
#define CH_STEREO	1
#define CH_QUADRO	3

//-- Modulator ------------------------------------------------------------------------------------

#define PARBUFLEN 25
typedef char Param[PARBUFLEN+1];

typedef struct
{
	Param			vals,vale;								// Start / Endwert
	UBYTE			bshape;									// Blendshape : None,Curve,Cycle,Vector,User
	BShapeFuncPtr	modfunc;								// BShapeFunction
	char			desc[50];								// Beschreibung
	ULONG			slen,srat;								// für "Cycle"
	APTR			bshpdata;								// Blenshapedata
} Modulator;

#define MOD_IDCT	7
#define MOD_IXCT	6

#define MODINIT_FIRST 1

#define BSHP_NONE		0
#define BSHP_CURVE		1
#define BSHP_CYCLE		2
#define BSHP_VECTOR		3
#define BSHP_USER		4

#define BSHP_ANZ		5

//-- BlendShape typen

typedef struct
{
	double	exp;					// Ausbeulung der Kurve
} ModCurve;

typedef struct
{
	UBYTE	oszi;					// welcher Oszilator, siehe unten
	Param	frq;					// bestimmt die frequenz (ev. indirekt)
	Param	pha;					// startphase
	double	angle,anglest,hold;		// winkel,winkelschritt oder sample & hold;
} ModCycle;

#define MC_OSZI_SAW		0
#define MC_OSZI_SIN		1
#define MC_OSZI_SQR		2
#define MC_OSZI_TRI		3
#define MC_OSZI_RND		4
#define MC_OSZI_ANZ		5

#define MV_MAX_VECTORS	20

typedef struct
{
	UBYTE	anz;					// Anzahl der Vectorpunkte,max=20;
	UBYTE	ix;						// akt. Punkt
	float	dfc;					// Differenzenfactor im aktuellen Bereich
	float	lpos,llev;				// letzte Werte
	float	pos[MV_MAX_VECTORS];	// Positionen von 0.0...1.0
	float	lev[MV_MAX_VECTORS];	// Amplitude von 0.0...1.0
	BOOL	lockfirst;				// is first point fixed
	BOOL	locklast;				// is last point fixed
} ModVector;

typedef struct
{
	UBYTE	type;					// wie die sourcedaten interpretiert werden sollen, siehe unten
	UBYTE	bmode;					// mode : single,repeat,stretch
	ULONG	modid;					// Modulationsid.
	UBYTE	env;					// Hüllkurventyp, siehe unten
	SInfo	*modbuf;				// Modulationspuffer
	APTR	*envdata;				// für bmode=AMPENV oder FRQENV
} ModUser;

#define MU_TYPE_NORMAL	0			// User0 : -1.0 -> 0.0 , 1.0 -> 1.0 
#define MU_TYPE_ABS		1			// User1 :  0.0 -> 0.0 , 1.0 & -1.0 -> 1.0
#define MU_TYPE_AMPENV	2			// User2 : Volumenhüllkurve
#define MU_TYPE_FRQENV	3			// User? : Frequenzhüllkurve
#define MU_TYPE_ANZ		4

#define MU_BMOD_SINGLE	0			// Sample einmal durchlaufen, dann 0.0
#define MU_BMOD_REPEAT	1			// Sample unendlich oft durchlaufen
#define MU_BMOD_STRETCH	2			// Sample auf Sourcelänge strecken
#define MU_BMOD_ANZ		3

#define MU_AENV_MAXDECAY	0

typedef struct
{
	double maxval;
	double fc1,fc2;
} AmpEnvMaxDecay;

#define MU_PENV_PEAKSEARCH	0

typedef struct
{
	ULONG lastpos,curlen;
	WORD lastval;
	UBYTE maxs,mins;
} FrqEnvPeakSearch;

//-- Source ---------------------------------------------------------------------------------------

typedef struct
{
	SInfo			*srcbuf;
	UBYTE			rmode;
} Source;

#define SRC_IDCT	3
#define SRC_IXCT	3

#define RNGMD_ALL		0
#define RNGMD_WINDOW	1
#define RNGMD_RANGE		2

//-- Interpolator ---------------------------------------------------------------------------------

typedef struct
{
	UBYTE	type;
	double  range;
} Interpolator;

#define INT_IDCT	2
#define INT_IXCT	2

#define INT_TYPE_NONE		0
#define INT_TYPE_LIN		1
#define INT_TYPE_SI			2
#define INT_TYPE_LAGRANGE	3

#define INT_TYPE_ANZ		4

typedef double __asm (*InterpolFuncPtr)(REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range));

//-- Windowfunctions ------------------------------------------------------------------------------

typedef struct
{
	ULONG		  	win;			// which kind of function
	double			par;			// it's parameter
	char			desc[50];
} WinFunction;

#define WFKT_IDCT	2
#define WFKT_IXCT	2

#define WIN_BARTLETT	 0
#define WIN_BLACKMAN	 1
#define WIN_FEJER		 2
#define WIN_HALFSINE	 3
#define WIN_HALFSINEQ	 4
#define WIN_HALFSINES	 5
#define WIN_HAMMING		 6
#define WIN_HANNING		 7
#define WIN_KAISER		 8
#define WIN_RECTANGLE	 9
#define WIN_WELCH		10

#define WIN_ANZ			11

//-- CfgSel ---------------------------------------------------------------------------------------

typedef struct
{
	struct List cfgs;
	struct Node *aktcfg;
	ULONG		action;
	char		path[FMSIZE];
} CfgSel;

#define CFGSEL_IDCT	4
#define CFGSEL_IXCT	4

#define CFGSEL_ACTION_NONE	0
#define CFGSEL_ACTION_GO	1
#define CFGSEL_ACTION_LOAD	2

//-- PrefsData ------------------------------------------------------------------------------------

typedef struct
{
	UWORD	type;
	STRPTR	prefix;
	APTR	data;
} PrefsData;

#define PD_END			 0
#define PD_SOURCE		 1
#define PD_MODULATOR	 2
#define PD_INTERPOLATOR	 3
#define PD_WINFUNCTION	 4
#define PD_OTHER		99

#define PDS_SOURCE			sizeof(Source)
#define PDS_MODULATOR		sizeof(Modulator)
#define PDS_INTERPOLATOR	sizeof(Interpolator)
#define PDS_WINFUNCTION		sizeof(WinFunction)

#define PDS_MODULATOR_OLD 114

//-------------------------------------------------------------------------------------------------

#define swin_idcmps IDCMP_CLOSEWINDOW|IDCMP_CHANGEWINDOW|                 IDCMP_RAWKEY|IDCMP_ACTIVEWINDOW|IDCMP_MENUPICK|IDCMP_MOUSEMOVE|IDCMP_NEWSIZE|IDCMP_MOUSEBUTTONS|SCROLLERIDCMP|ARROWIDCMP|IDCMP_IDCMPUPDATE
#define lwin_idcmps IDCMP_CLOSEWINDOW|IDCMP_CHANGEWINDOW|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_ACTIVEWINDOW|IDCMP_MENUPICK|IDCMP_MOUSEMOVE|IDCMP_NEWSIZE|BUTTONIDCMP|LISTVIEWIDCMP
#define nwin_idcmps IDCMP_CLOSEWINDOW|IDCMP_CHANGEWINDOW|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_ACTIVEWINDOW|IDCMP_MENUPICK|IDCMP_MOUSEMOVE|IDCMP_GADGETDOWN|IDCMP_GADGETUP|IDCMP_INTUITICKS
#define gwin_idcmps                                      IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_ACTIVEWINDOW|IDCMP_MENUPICK|IDCMP_MOUSEMOVE|IDCMP_GADGETDOWN|IDCMP_GADGETUP|IDCMP_INTUITICKS|LISTVIEWIDCMP

//-- Windowtypes ----------------------------------------------------------------------------------

// can be found in win->UserData

#define WT_Basic	((BYTE *)0)
#define WT_ProWin	((BYTE *)1)
#define WT_BackDrop	((BYTE *)2)

//-- ProgressWindow -------------------------------------------------------------------------------

typedef struct
{
	struct Window	*pw;			/* ProceedWindow */
	struct Gadget 	*proGList;		/* Gadgetliste */
	double			ptr,ptrs,prozh;	/* Hilfsvar`s für Anzeige */
	UWORD			tx,ty;			/* Text x,y Pos */
	UWORD			gy1,gy2;		/* y Pos for Bar */
	UWORD			optr;
	char			pstr[6];		/* Textpuffer */
} PWInfo;

typedef struct
{
	ULONG			maxlen;
	ULONG			*curlen;
	char			*title;
	ULONG			a4,a6;
	ULONG			mainsig,worksig;			// for communication
	struct Process	*maintask,*worktask;
	LONG			status;
} PWData;

//-- Main Help ID's -------------------------------------------------------------------------------

#define HELPID_MAIN			0
#define HELPID_INDEX		(HELPID_MAIN+1)
#define HELPID_OPWIN		(HELPID_INDEX+1)
#define HELPID_LOWIN		(HELPID_OPWIN+1)
#define HELPID_SAWIN		(HELPID_LOWIN+1)
#define HELPID_PLWIN		(HELPID_SAWIN+1)
#define HELPID_RXOPWIN		(HELPID_PLWIN+1)
#define HELPID_BUFWIN		(HELPID_RXOPWIN+1)
#define HELPID_EDWIN		(HELPID_BUFWIN+1)
#define HELPID_ZOOMWIN		(HELPID_EDWIN+1)
#define HELPID_RNGWIN		(HELPID_ZOOMWIN+1)
#define HELPID_SMPWIN		(HELPID_RNGWIN+1)
#define HELPID_OPTWIN		(HELPID_SMPWIN+1)
#define HELPID_PERIWIN		(HELPID_OPTWIN+1)
#define HELPID_SRCSELWIN	(HELPID_PERIWIN+1)
#define HELPID_PREFSGUIWIN	(HELPID_SRCSELWIN+1)
#define HELPID_PREFSSMPWIN	(HELPID_PREFSGUIWIN+1)
#define HELPID_PREFSVMEMWIN	(HELPID_PREFSSMPWIN+1)
#define HELPID_PREFSMISCWIN	(HELPID_PREFSVMEMWIN+1)
#define HELPID_INFOWIN		(HELPID_PREFSMISCWIN+1)
#define HELPID_STATUSWIN	(HELPID_INFOWIN+1)
#define HELPID_WORKSHOPMAIN	(HELPID_STATUSWIN+1)
#define HELPID_WFKTSELWIN	(HELPID_WORKSHOPMAIN+1)
#define HELPID_MODWINCURVE	(HELPID_WFKTSELWIN+1)
#define HELPID_MODWINCYCLE	(HELPID_MODWINCURVE+1)
#define HELPID_MODWINVECTOR	(HELPID_MODWINCYCLE+1)
#define HELPID_MODWINUSER	(HELPID_MODWINVECTOR+1)

#define HELPID_MSG1			(HELPID_MODWINUSER+1)
#define HELPID_MSG2_1		(HELPID_MSG1+1)
#define HELPID_MSG2_2		(HELPID_MSG2_1+1)
#define HELPID_MSG2_3		(HELPID_MSG2_2+1)
#define HELPID_MSG2_4		(HELPID_MSG2_3+1)
#define HELPID_MSG3			(HELPID_MSG2_4+1)
#define HELPID_MSG3_1		(HELPID_MSG3+1)
#define HELPID_MSG3_2		(HELPID_MSG3_1+1)
#define HELPID_MSG3_3		(HELPID_MSG3_2+1)
#define HELPID_MSG3_4		(HELPID_MSG3_3+1)
#define HELPID_MSG3_5		(HELPID_MSG3_4+1)
#define HELPID_MSG3_6		(HELPID_MSG3_5+1)
#define HELPID_MSG4			(HELPID_MSG3_6+1)
#define HELPID_MSG4_1		(HELPID_MSG4+1)
#define HELPID_MSG4_2		(HELPID_MSG4_1+1)
#define HELPID_MSG4_3		(HELPID_MSG4_2+1)
#define HELPID_MSG5			(HELPID_MSG4_3+1)
#define HELPID_MSG5_1		(HELPID_MSG5+1)
#define HELPID_MSG5_2		(HELPID_MSG5_1+1)
#define HELPID_QUE			(HELPID_MSG5_2+1)
#define HELPID_QUE1			(HELPID_QUE+1)
#define HELPID_QUE1_1		(HELPID_QUE1+1)
#define HELPID_QUE1_2		(HELPID_QUE1_1+1)
#define HELPID_QUE2			(HELPID_QUE1_2+1)
#define HELPID_QUE2_1		(HELPID_QUE2+1)
#define HELPID_QUE2_2		(HELPID_QUE2_1+1)
#define HELPID_QUE2_3		(HELPID_QUE2_2+1)

#define HELPID_ANZ			(HELPID_QUE2_3+1)

//-- helpnode basenames for modules ---------------------------------------------------------------

#define HELPBASENAME_OP		"node02.01.06"
#define HELPBASENAME_LO		"node02.02.01"
#define HELPBASENAME_SA		"node02.03.01"
#define HELPBASENAME_PL		"node02.04.01"
#define HELPBASENAME_RX		"node02.05.01"

//-- Samplewindow Border Gadget ID's --------------------------------------------------------------

#define GADID_SWB_HORIZ	0
#define GADID_SWB_VERT	1
#define GADID_SWB_LEFT	2
#define GADID_SWB_RIGHT	3
#define GADID_SWB_UP	4
#define GADID_SWB_DOWN	5

//-- Pennumbers -----------------------------------------------------------------------------------

#define PEN_GUI_BACK			0
#define PEN_GUI_SHINE			(1+PEN_GUI_BACK)
#define PEN_GUI_SHADOW			(1+PEN_GUI_SHINE)
#define PEN_GUI_HIGHLIGHT		(1+PEN_GUI_SHADOW)
#define PEN_GUI_COLORTEXT	 	(1+PEN_GUI_HIGHLIGHT)
#define PEN_GUI_COLORTEXTGLOW	(1+PEN_GUI_COLORTEXT)
#define PEN_GUI_GRADIENTSTART	(1+PEN_GUI_COLORTEXTGLOW)
#define PEN_GUI_GRADIENTEND		(1+PEN_GUI_GRADIENTSTART)

#define PEN_SMP_BACK			(1+PEN_GUI_GRADIENTEND)
#define PEN_SMP_TEXT			(1+PEN_SMP_BACK)
#define PEN_SMP_LINE		 	(1+PEN_SMP_TEXT)
#define PEN_SMP_GRID			(1+PEN_SMP_LINE)
#define PEN_SMP_MAXL			(1+PEN_SMP_GRID)
#define PEN_SMP_RMSL			(1+PEN_SMP_MAXL)
#define PEN_SMP_AVGL			(1+PEN_SMP_RMSL)
#define PEN_SMP_HIGHLIGHT		(1+PEN_SMP_AVGL)
#define PEN_SMP_LOOP			(1+PEN_SMP_HIGHLIGHT)
#define PEN_SMP_MARK			(1+PEN_SMP_LOOP)

//-- Virtual Memory Stuff -------------------------------------------------------------------------

#define VM_NODATA	0
#define VM_MEMORY	1
#define VM_DRIVE	2

#define VM_MODE_READ_WRITE	 1	// bit 0
#define VM_MODE_READ		 0
#define VM_MODE_WRITE		 1
#define VM_MODE_SEQ_RND		 2	// bit 1
#define VM_MODE_SEQ			 0
#define VM_MODE_RND			 2
#define VM_MODE_DIR_BLK		 4	// bit 2
#define VM_MODE_DIR			 0					// work directly with the file
#define VM_MODE_BLK			 4					// use async blkIO
#define VM_MODE_CBLK_VBLK	 8	// bit 3
#define VM_MODE_CBLK		 0					// constant blocs
#define VM_MODE_VBLK		 8					// variable blocs
#define VM_MODE_IRQ			16	// bit 4		// for device samples, use read & writes without waits

#define VM_DEFAULT_BLKLEN	0				// use default blklen from prefs

typedef struct smpaccess
{
	BYTE			*buffer1,*buffer2;			// für double buffering
	BYTE 			*buffer3,*buffer4;			// für loopstartblock und für seeks in mem
	void __asm		(*safunc)(REG(a0,struct smpaccess *sa),REG(d0,ULONG length));	// sample data access function
	int				fh;
	ULONG			blen,blensh;				// segmentlength (in bytes), nr of shifts (to do the multiplication)
	ULONG			cblen;						// current blocklength to read
	ULONG			start;						// seekoffset (in bytes)
	ULONG			seganz;						// how many segments
	ULONG			seg,aseg;					// segment nr to read/write, segment nr of fileptr
	ULONG			a4,a6;
	BYTE			mainsig,worksig;			// for communication
	struct Process	*maintask,*worktask;
	UBYTE			state,access;
	char			*fn;
	int				mode;
	LONG			seek;						// -1 means no seek required before next read
} SmpAccess;

typedef void __asm (*SAFuncPtr)(REG(a0,SmpAccess *sa),REG(d0,ULONG length));

#define STATE_READY		0
#define STATE_NOTIFY	1
#define STATE_WORKING	2
#define STATE_FINISH	3

#define VM_BUFSIZE 16384

#define CPD_NIL2MEM ((VM_NODATA<<8)|VM_MEMORY)
#define CPD_NIL2DRV ((VM_NODATA<<8)|VM_DRIVE )
#define CPD_MEM2MEM ((VM_MEMORY<<8)|VM_MEMORY)
#define CPD_MEM2DRV ((VM_MEMORY<<8)|VM_DRIVE )
#define CPD_DRV2MEM ((VM_DRIVE <<8)|VM_MEMORY)
#define CPD_DRV2DRV ((VM_DRIVE <<8)|VM_DRIVE )

//-- Arexx Parsing --------------------------------------------------------------------------------

#define	MaxCmdLen	256

#define MaxParLen	 50
#define MaxParLen2	100
#define MaxParLen3	150
#define MaxParLen4	200
#define MaxParLen5	250
#define MaxParLen6	300

#define RexxPar0	&params[0]
#define RexxPar1	&params[MaxParLen]
#define RexxPar2	&params[MaxParLen2]
#define RexxPar3	&params[MaxParLen3]
#define RexxPar4	&params[MaxParLen4]
#define RexxPar5	&params[MaxParLen5]
#define RexxPar6	&params[MaxParLen6]

#define MaxParAnz	20

//-------------------------------------------------------------------------------------------------

typedef struct							/* Zuweisungsliste */
{
	char name[50];						/* unbedingt auf 50 lassen, wegen Bufferliste */
	WORD nr;
} ArrangeList;

//-- Prefs ----------------------------------------------------------------------------------------

#define PREF_FORM		MakeID('F','O','R','M')			// obsolete when all modules are using configio
#define PREF_PREF		MakeID('P','R','E','F')
#define PREF_VERS		MakeID('V','E','R','S')

#define PREF_PARA		MakeID('P','A','R','A')			// for Modules
#define PREF_NAME		MakeID('N','A','M','E')
#define PREF_DATA		MakeID('D','A','T','A')

typedef struct							/* Prefs */
{
	ULONG	id;								/* FORM,VERS,PATH,GUI_,MISC,SBUF, */
	ULONG	len;							/* 4+(8+2)+(8+768)+(8+166)+(8+2)+(8+14)=1002 */
} SFXPrefs_Hdr;

typedef struct								/* VERS */
{
	UBYTE	ver;							/* Version   */
	UBYTE	rev;							/* Reversion */
} SFXPrefs_Ver;

typedef struct								/* PATH */
{
	char sample_path[256];					/* Pfade für Filerequester */
	char swap_path[256];
	char unused_path[256];					/* unused */
} SFXPrefs_Path;

typedef struct								/* GUI_ */
{
	ULONG	scrmode;								/* ScreenMode */
	UWORD	xres,yres;								/* Auflösung */
	UBYTE	oscan;									/* Overscan */
	UBYTE	autoscroll;								/* Autoscroll */
	#define SFXPrefs_Gui_MaxColors		16
	UBYTE	ScreenColors[SFXPrefs_Gui_MaxColors][3];/* Bildschirmfarben (48 Bytes) */
	UBYTE	Pens[20];								/* Pennumbers */
	#define SFXPrefs_Gui_FontNameLen	80
	char	fontname[SFXPrefs_Gui_FontNameLen];		/* Fontname */
	UBYTE	fontsize;								/* Fontgröße */
	UBYTE	depth;									/* Screendepth */
} SFXPrefs_Gui;

typedef struct								/* neue Struktur */
{
	UWORD x1,y1,x2,y2;						/* Pos and Size */
	UBYTE state;							/* Zustand : 0->closed/1->opened */
//	UBYTE pad;
	UBYTE entryname[50];					/* angewählter Eintrag */
} SFXPrefs_LWin;

typedef struct
{
	UWORD x1,y1;							/* Pos */
	UBYTE state;							/* Zustand : 0->closed/1->opened */
//	UBYTE pad;
} SFXPrefs_NWin;

typedef struct
{
	SFXPrefs_LWin	opwin;
	SFXPrefs_LWin	rxopwin;
	SFXPrefs_LWin	lowin;
	SFXPrefs_LWin	sawin;
	SFXPrefs_LWin	plwin;
	SFXPrefs_LWin	bufwin;
	SFXPrefs_NWin	zowin;
	SFXPrefs_NWin	edwin;
	SFXPrefs_NWin	rngwin;
} SFXPrefs_Win;

typedef struct								/* MISC */
{
	UBYTE sig_a;							/* Wenn mit Berechnung fertig DisplayBeep() */
	UBYTE sig_s;							/* Wenn mit Berechnung fertig ScreenToFont() */
	UBYTE backlogo;							/* Backlogo on/off */
	UBYTE pad;
	ULONG keynr;							/* Keynr, wenn die sich ändert ist etwas suspect */
	UBYTE freemem,maxmem;					/* soll freier Speicher, sowie maximaler Speicher angezeigt werden */
	UBYTE srcselect;						/* source select mode (list,cycle) */
	UBYTE contextbutton;					/* context-menu mouse-button (middle,left) */
} SFXPrefs_Misc;

typedef struct								/* SBUF */
{
	UBYTE drawmode;									/* 0 - 2 (Lines,Dots,Filled) */
	UBYTE maxlines;									/* Maxlines zeichnen ? */
	UBYTE rasterx,rastery;							/* X/Y-Raster zeichnen ? */
	UBYTE safemode;									/* Sicherheitsabfrage */
	UBYTE quickdraw;								/* schnelles Zeichnen beim Scrollen ? */
	UWORD xs1,ys1,xs2,ys2;							/* Normal and Zoomed Size */
	UBYTE mode;										/* Sizemode */
	UBYTE storage;									/* Speicherungsform */
	UBYTE rmslines,avglines;						/* RMS-,AVG Linien zeichnen */
	UWORD scaxs,scays;								/* Scalengröße */
	UBYTE scalax,scalay;							/* Scala auf der X bzw. Y-Achse */
	#define SFXPrefs_SBuf_SamplePathLen		256
	char  sample_path[SFXPrefs_SBuf_SamplePathLen];	/* Defaultpfad für Filerequester */
	#define SFXPrefs_SBuf_FontNameLen		80
	char  scafont_name[SFXPrefs_SBuf_FontNameLen];	/* Scalenfont */
	UBYTE scafont_size;
//	UBYTE pad2[1];
	UBYTE unitx;									/* Samples,Time,SMPTE-MF,SMPTE-PF,SMPTE-NF */
	UBYTE unity;									/* Factor,Prozent,Promille,Level,Dezibel */
	UBYTE nolines;									/* should sfx disable #?line-calculation for big samples */
	ULONG thrsize;									/* and by the way, what is *big*? */
	#define SFXPrefs_SBuf_InfoStrLen		256
	char  copystr[SFXPrefs_SBuf_InfoStrLen];		/* default info strings : copyright */
	char  authstr[SFXPrefs_SBuf_InfoStrLen];		/* default info strings : author */
} SFXPrefs_SBuf;

typedef struct								/* VMEM */
{
	#define SFXPrefs_VMem_SwapPathLen 	256
	UBYTE swap_path[SFXPrefs_VMem_SwapPathLen];		/* wohin sollen die Samples ausgelagert werden */
	ULONG blksize;
	UBYTE enabled;
} SFXPrefs_VMem;

//-- Notification ---------------------------------------------------------------------------------

typedef struct
{
	struct Node	node;			/* für Verkettung */
	struct Task *task;			/* who to notify */
	LONG		sig;			/* and how */
} NotifyTask;

//-- Client Rect : Screenarea available for views -------------------------------------------------

struct ClientRect {
	UWORD LeftEdge,TopEdge;
	UWORD RightEdge,BottomEdge;
	UWORD Width,Height;
};

//-- Toolbarextension -----------------------------------------------------------------------------

#define nt_Pack(w,h) ((ULONG)(w<<16)|h)
#define nt_UnpackX(p) ((p&0xFFFF0000)>>16)
#define nt_UnpackY(p) (p&0x0000FFFF)

struct NewToolbar
{
    ULONG nt_Type;					/* see below */
	struct Image *nt_Image;			/* image which is shown normally, when NULL data is interpreted different */
	union
	{
		struct Image *nt_HighImage;		/* image which is shown when mouse pointer is over it */
		ULONG nt_Width;					/* width of space */
		ULONG nt_Pos;					/* pos of toolbar */
	};
	union
	{
		struct Image *nt_ActiveImage;	/* image which is shown when mouse button is down */
		ULONG nt_Height;				/* height of space */
		ULONG nt_Size;					/* size of toolbar */
	};
    STRPTR nt_Label;				/* toolbar's label */
	ULONG nt_Border;				/* see below */
	ULONG nt_GadgetID;				/* the id you'll get via intuimessage*/
    struct Gadget *nt_Gadget;		/* Gadget-pointers are entered here */
};

/* nt_Type determines what each NewToolbar structure corresponds to.
 */
#define NT_TOOLBAR      1       /* Toolbar header */
#define NT_ITEM         2       /* Toolbar item */
#define NT_SUB          3       /* Toolbar sub-item */

/* The NewToolbar array should be terminated with a NewToolbar whose
 * nt_Type equals NT_END.
 */
#define NT_END          0       /* End of NewToolbar array */

#define NTB_NONE		0
#define NTB_RAISED		1
#define NTB_SUNKEN		2
#define NTB_RIDGE		3
#define NTB_GROOVE		4

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

typedef struct
{
	/*------------------------------------------------ GUI Globals */
	double				yh;
	float				scalex,scaley;							/* x,y -scalefactors */
	ULONG				sig_in;									/* Signalmaske */
	UWORD				scx[750];								/* Skalierungstabellen für GUI */
	UWORD				scy[300];
	UWORD				xres,yres;								/* aktuelle Auflösung */
	UWORD				fxoff,looff,saoff,ploff;				/* Listenoffsets für guides */
	struct Screen		*sfxpubscr;								/* der eingene PubScreen */
	struct TextFont		*scrfont,*scrfontb,*sigfont,*scafont;	/* Fonts */
	struct TextAttr 	*ScrFont,*ScrFontB,*SigFont,*ScaFont;
	APTR				VisualInfo;								/* VisualInfo for GUI */
	struct Menu			*Menu,*FXMenu;							/* die Menühandles */
	char				fontname[80];							/* Fontname */
	struct MsgPort		*swin_up;								/* Samplewindow UserPort */
	/*------------------------------------------------ OnLine Hilfe */
	AMIGAGUIDECONTEXT	aghandle;								/* AmigaguideHandle */
	char				**agcontents;							/* Hilfenodes */
	ULONG				agsig;									/* Sigbit für AGuide */
	/*------------------------------------------------ verschiedene Globals */
	UBYTE				rexxactive;								/* Ist eine Funktion von einem REXX-Scripts aus aufgerufen worden ? */
	UBYTE				quitsfx;								/* Programm beenden ? */
	UBYTE				aloader,asaver,aplayer,aktfx,arxop;		/* aktuelles Modul */
	UBYTE				anzlo,anzsa,anzpl,anzfx,anzrxop;		/* Anzahl der Module */
	UBYTE				loopp;									/* aktiver LoopPointer 0=LoopS 1=LoopL */
	UBYTE				hfilter;								/* Status der PowerLED */
	UBYTE				periplay;								/* Play im Periodenfenster */
	UBYTE				rngmove;								/* RangeMove ? */
	UBYTE				play,plstop;							/* Abspielen ? */
	UBYTE				rngmode,rnglook,movemode;				/* Rangewindow */
	UBYTE				zoommode;								/* X / Y-Zoom */
	UBYTE				status;									/* Status => 0=Demoversion */
	UBYTE				swin_upct,gwin_upct;					/* Open/CLoseMWindow counter */
	UBYTE				laktsamples,aktsamples;
	UBYTE				status2,status3;						/* Kopien der Statusflags */
	UBYTE				tryquit;								/* finishing all the mesageloops, before quitting */
	UBYTE				tryreopen;								/* finishing all the mesageloops, before reopen GUI */
	UBYTE				changesamples;							/* number of samples has changed */
	UBYTE				samplestat;								/* is a sample the first one opened or last one closed or nothing special */
	UBYTE				swallowactivate;						/* ignore activate window event */
	ULONG				mem;									/* Speicherverbrauch */
	LONG				rngdiff;								/* Richtung / Betrag */
	ULONG				mx1,mx2;								/* Werte unter dem Mauszeiger für akt Sample bzw. Markhelp */
	WORD				my1,my2;
	struct Process		*sfxproc;								/* unser Hauptprozess */
	struct List 		gblist;									/* Pointer to Bufferlist */
	ULONG				srclist[20];
	struct List 		*blist;									/* Pointer to BufferList */
	UBYTE		 		**barray;								/* Pointer to BufferArray, wird aus BufferList generiert */
	SInfo 				*aktbuf;								/* currently selected buffer */
	/*------------------------------------------------ Gui Extensions */
	struct StringExtend *stringextend;							/* Pointer to global StringExtend */
	struct Hook			*tablelvhook,*colorlvhook;				/* ListViewHooks */
	struct ClientRect	clientrect;								/* freier Fensterbereich */
	struct Window		*winStatus;								/* Statusanzeigefenster */
	struct Window		*winToolbar;							/* Fenster für Toolbars */
	struct Window		*winDropDown;							/* Fenster für DropDown-Elemente der Toolbars */
	struct Window		*winBackDrop;							/* backdrop window */
	/*------------------------------------------------ Daten für Playroutinen */
	SInfo 				*plentry;								/* was wird gespielt */
	UBYTE 				*plst,*plst2;							/* Pointer to Playervariables */
	struct AHIAudioCtrl *audioctrl;								/* AHI-Stuff */
	struct Library 		*AHIBase;
	OPTP_Amiga_8bit		*optp_Amiga_8bit;						/* Playeroptions */
	OPTP_Amiga_14bit	*optp_Amiga_14bit;
	OPTP_Amiga_14cbit	*optp_Amiga_14cbit;
	OPTP_Ahi			*optp_Ahi;
	UBYTE				boolPaulaOkay,boolCaCaOkay,boolAHIOkay;	/* für Player */
	ULONG				plptr,oldptr,plslen;					/* Abspielpositionen und ~länge */
	ULONG				pllstart,pldiff;						/* Loopstarts und ~rücksprünge */
	/*-----------------------------------------------  Konfigurationsdaten */
	SFXPrefs_SBuf		*sfxprefs_sbuf;							/* Samplebuffer */
	SFXPrefs_VMem		*sfxprefs_vmem;							/* virtueller Speicher */
	SFXPrefs_Gui		*sfxprefs_gui;							/* graphische Nutzerschnittstelle */
	SFXPrefs_Misc		*sfxprefs_misc;							/* verschiedenes */
	/*------------------------------------------------ Keyfiledaten */
	char				username[85];							/* the name of user, taken from keyfile */
	ULONG				keynr;									/* Keydaten */
	UBYTE				keymi,keyma,keymabits,keyrv;
	UBYTE				*keypd,*keycd;
	UBYTE				*keyrowd,*keycold;
	UBYTE				keyerr;									/* Fehlercode */
	/*------------------------------------------------ GUI enhancements (gradients) */
	UBYTE				grad_cmap[32];
	UWORD				grad_nr;
} RTime;

//-- Loader and Saversupport ----------------------------------------------------------------------

/* I can't supply all params to the library directly, because running out of data regs. */

typedef struct
{
	char	fn[256];
	ULONG	savestart,savelength;
	ULONG	offs;
	APTR	tab;					// for packing
	UBYTE	chmask;					// for just saving selected channels
} SaveData;

typedef struct
{
	char	fn[256];
	ULONG	offs;
	APTR	tab;					// for de-packing
	UBYTE	chmask;					// for just saving selected channels
	UBYTE	flags;
	APTR	codecdata;				// additional data for the codec
} LoadData;

#define LDF_NONE		0
#define LDF_NOCHOFF	1			// no channel offset

#endif	 /* LIBRARIES_SFXSUPPORT_H */

//-- eof ----------------------------------------------------------------------
