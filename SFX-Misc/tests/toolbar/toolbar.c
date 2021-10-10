/****h* toolbar/toolbar.c [0.00] *
*
*  NAME
*    toolbar.c
*  COPYRIGHT
*    $VER: toolbar.c 0.00 (29.01.00) © by Stefan Kost 1999-2000
*  FUNCTION
*
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    01.Dec.1999
*  MODIFICATION HISTORY
*    29.Jan.2000	V 0.00	most recent version
*    01.Dec.1999	V 0.00	initial version
*  NOTES
*
*******
*/

//-- includes -----------------------------------------------------------------

#define __USE_SYSBASE

//-- Ansi

#include <cmacros.h>
#include <stdio.h>

//-- AmigaOS

#include <exec/types.h>
#include <exec/memory.h>
#include <intuition/intuition.h>
#include <intuition/screens.h>
#include <libraries/gadtools.h>

#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

//-- Local

//#include "SFX-ImgBacklogo.h"
#include "backlogo26.h"

#include "Style/imgNew.h"
#include "Style/imgLoad.h"
#include "Style/imgSave.h"

#include "Style/imgDropDown.h"
#include "Style/imgRun.h"

#include "Style/imgCutMen.h"
#include "Style/imgCut.h"
#include "Style/imgCutL.h"
#include "Style/imgCutR.h"
#include "Style/imgCopyMen.h"
#include "Style/imgCopy.h"
#include "Style/imgCopyL.h"
#include "Style/imgCopyR.h"
#include "Style/imgPasteMen.h"
#include "Style/imgPaste.h"
#include "Style/imgPasteL.h"
#include "Style/imgPasteR.h"
#include "Style/imgEraseMen.h"
#include "Style/imgErase.h"
#include "Style/imgEraseL.h"
#include "Style/imgEraseR.h"
#include "Style/imgGrabMen.h"
#include "Style/imgGrab.h"
#include "Style/imgGrabL.h"
#include "Style/imgGrabR.h"
#include "Style/imgZeroMen.h"
#include "Style/imgZero.h"
#include "Style/imgZeroL.h"
#include "Style/imgZeroR.h"
#include "Style/imgOverwrMen.h"
#include "Style/imgOverwr.h"
#include "Style/imgOverwrL.h"
#include "Style/imgOverwrR.h"

#include "Style/imgPlayAll.h"
#include "Style/imgPlayRng.h"
#include "Style/imgStop.h"
#include "Style/imgRecord.h"

#include "Style/imgZoomIn.h"
#include "Style/imgZoomOut.h"
#include "Style/imgZoomEq.h"
#include "Style/imgZoomOutAll.h"

#include "Style/imgMoveBeg.h"
#include "Style/imgMoveLeftF.h"
#include "Style/imgMoveLeftS.h"
#include "Style/imgMoveLeftN.h"
#include "Style/imgMoveRightN.h"
#include "Style/imgMoveRightS.h"
#include "Style/imgMoveRightF.h"
#include "Style/imgMoveEnd.h"

#include "Style/imgMoveTop.h"
#include "Style/imgMoveUpF.h"
#include "Style/imgMoveUpS.h"
#include "Style/imgMoveUpP.h"
#include "Style/imgMoveDownP.h"
#include "Style/imgMoveDownS.h"
#include "Style/imgMoveDownF.h"
#include "Style/imgMoveBottom.h"

#include "Style/imgZoom.h"
#include "Style/imgMark.h"
#include "Style/imgLoop.h"
#include "Style/imgTrace.h"

#include "Style/imgLockOp.h"
#include "Style/imgLockCl.h"

#include "project.h"

//-- DebugTools

#define DEBUG_SRCPATH "data:source/sfx/sfx-misc/tests/toolbar/"
#include <debug/dt_decl.h>

//--

#define TB_HEIGHT	31
#define SB_HEIGHT	20
#define DD_HEIGHT	100

//-- prototypes ---------------------------------------------------------------

//-- helper

void Box3DOut(struct RastPort *rp,UWORD x1, UWORD y1, UWORD x2, UWORD y2);
void Box3DIn(struct RastPort *rp,UWORD x1, UWORD y1, UWORD x2, UWORD y2);

void ShowDropDownWin_TextList(UWORD x1,UWORD y1,UWORD x2,UBYTE **choices);
void ShowDropDownWin_Images  (UWORD x1,UWORD y1,UWORD x2,UWORD y2,struct Gadget *gadgets);

void HighLightGadgets(UWORD smx,UWORD smy);

struct Gadget *CreateToolbars(struct NewToolbar *ntb,ULONG maxwidth,ULONG maxheight);
void DrawToolbars(struct NewToolbar *ntb,struct RastPort *rp);
void FreeToolbars(struct NewToolbar *ntb);

void SetStatusbar(STRPTR msg);

void NG_HandleEvents(ULONG iclass,UWORD icode,APTR iadr,struct Window *iwin,WORD imx,WORD imy,BOOL *closedd);

//-- basic framework

void init(void);
void done(void);
void run(void);

//-- defines ------------------------------------------------------------------

//-- globals ------------------------------------------------------------------

//-- Versionsstring

//UBYTE *VersTag="\0$VER: "PRJ_VERS;

//-- Libarybases

struct IntuitionBase	*IntuitionBase=NULL;
struct GfxBase	 		*GfxBase=NULL;
struct Library	 		*GadToolsBase=NULL;

//-- GUI-ressources

struct Screen			*scr=NULL;
struct ViewPort			*vp=NULL;
struct ColorMap			*cm=NULL;
struct Window			*win=NULL,*bdwin=NULL,*tbwin=NULL,*sbwin=NULL,*ddwin=NULL;
struct RastPort			*rp=NULL;
struct Gadget			*tbgad=NULL,*ddglist=NULL;
APTR					VisualInfo=NULL;
struct {
	UWORD LeftEdge,TopEdge;
	UWORD RightEdge,BottomEdge;
	UWORD Width,Height;
} ClientRect;

static WORD ogp=0;
static struct Gadget *ogad=NULL;
static struct Window *owin=NULL;

//-- Gadget IDs

#define GADID_TB_NEW		0
#define GADID_TB_LOAD		(GADID_TB_NEW+1)
#define GADID_TB_SAVE		(GADID_TB_LOAD+1)
#define GADID_TB_FXCHOICE	(GADID_TB_SAVE+1)
#define GADID_TB_FXRUN		(GADID_TB_FXCHOICE+1)
#define GADID_TB_CUTMEN		(GADID_TB_FXRUN+1)
#define GADID_TB_COPYMEN	(GADID_TB_CUTMEN+1)
#define GADID_TB_PASTEMEN	(GADID_TB_COPYMEN+1)
#define GADID_TB_ERASEMEN	(GADID_TB_PASTEMEN+1)
#define GADID_TB_GRABMEN	(GADID_TB_ERASEMEN+1)
#define GADID_TB_ZEROMEN	(GADID_TB_GRABMEN+1)
#define GADID_TB_OVERWRMEN	(GADID_TB_ZEROMEN+1)
#define GADID_TB_PLAYALL	(GADID_TB_OVERWRMEN+1)
#define GADID_TB_PLAYRNG	(GADID_TB_PLAYALL+1)
#define GADID_TB_STOP		(GADID_TB_PLAYRNG+1)
#define GADID_TB_RECORD		(GADID_TB_STOP+1)
#define GADID_TB_ZOOMIN		(GADID_TB_RECORD+1)
#define GADID_TB_ZOOMOUT	(GADID_TB_ZOOMIN+1)
#define GADID_TB_ZOOMEQ		(GADID_TB_ZOOMOUT+1)
#define GADID_TB_ZOOMOUTALL	(GADID_TB_ZOOMEQ+1)
#define GADID_TB_RNGMODE	(GADID_TB_ZOOMOUTALL+1)
#define GADID_TB_LOCKLEN	(GADID_TB_RNGMODE+1)
#define GADID_TB_MOVEBEG	(GADID_TB_LOCKLEN+1)
#define GADID_TB_MOVELEFTF	(GADID_TB_MOVEBEG+1)
#define GADID_TB_MOVELEFTS	(GADID_TB_MOVELEFTF+1)
#define GADID_TB_MOVELEFTN	(GADID_TB_MOVELEFTS+1)
#define GADID_TB_MOVERIGHTN	(GADID_TB_MOVELEFTN+1)
#define GADID_TB_MOVERIGHTS	(GADID_TB_MOVERIGHTN+1)
#define GADID_TB_MOVERIGHTF	(GADID_TB_MOVERIGHTS+1)
#define GADID_TB_MOVEEND	(GADID_TB_MOVERIGHTF+1)
#define GADID_TB_MOVETOP	(GADID_TB_MOVEEND+1)
#define GADID_TB_MOVEUPF	(GADID_TB_MOVETOP+1)
#define GADID_TB_MOVEUPS	(GADID_TB_MOVEUPF+1)
#define GADID_TB_MOVEUPP	(GADID_TB_MOVEUPS+1)
#define GADID_TB_MOVEDOWNP	(GADID_TB_MOVEUPP+1)
#define GADID_TB_MOVEDOWNS	(GADID_TB_MOVEDOWNP+1)
#define GADID_TB_MOVEDOWNF	(GADID_TB_MOVEDOWNS+1)
#define GADID_TB_MOVEBOTTOM	(GADID_TB_MOVEDOWNF+1)

#define GADID_DD_LIST		0

#define GADID_DD_CUT		(GADID_DD_LIST+1)
#define GADID_DD_CUTLEFT	(GADID_DD_CUT+1)
#define GADID_DD_CUTRIGHT	(GADID_DD_CUTLEFT+1)

#define GADID_DD_COPY		(GADID_DD_CUTRIGHT+1)
#define GADID_DD_COPYLEFT	(GADID_DD_COPY+1)
#define GADID_DD_COPYRIGHT	(GADID_DD_COPYLEFT+1)

#define GADID_DD_PASTE		(GADID_DD_COPYRIGHT+1)
#define GADID_DD_PASTELEFT	(GADID_DD_PASTE+1)
#define GADID_DD_PASTERIGHT	(GADID_DD_PASTELEFT+1)

#define GADID_DD_ERASE		(GADID_DD_PASTERIGHT+1)
#define GADID_DD_ERASELEFT	(GADID_DD_ERASE+1)
#define GADID_DD_ERASERIGHT	(GADID_DD_ERASELEFT+1)

#define GADID_DD_GRAB		(GADID_DD_ERASERIGHT+1)
#define GADID_DD_GRABLEFT	(GADID_DD_GRAB+1)
#define GADID_DD_GRABRIGHT	(GADID_DD_GRABLEFT+1)

#define GADID_DD_ZERO		(GADID_DD_GRABRIGHT+1)
#define GADID_DD_ZEROLEFT	(GADID_DD_ZERO+1)
#define GADID_DD_ZERORIGHT	(GADID_DD_ZEROLEFT+1)

//-- Choices

UBYTE *ddChoices[]={ "Choice1","Choice2",0l };

//-- gui pen definitions

ULONG imgPaletteRGB32[50] =
{
	0x00100000,	/* Record Header */
	0xAEEEEEEE,0xA2222222,0xA2222222,
	0x20000000,0x10000000,0x20000000,
	0xFFFFFFFF,0xEFFFFFFF,0xFFFFFFFF,
	0xAEEEEEEE,0x8EEEEEEE,0xCEEEEEEE,
	0x3DDDDDDD,0xFFFFFFFF,0x3DDDDDDD,
	0xCEEEEEEE,0xBEEEEEEE,0xBEEEEEEE,
	0x71111111,0x61111111,0x61111111,
	0xFFFFFFFF,0xFFFFFFFF,0x7DDDDDDD,
	0x65555555,0x00000000,0x20000000,
	0x96666666,0x00000000,0x31111111,
	0xFFFFFFFF,0x00000000,0x41111111,
	0x0CCCCCCC,0x3DDDDDDD,0xFFFFFFFF,
	0xFFFFFFFF,0xFFFFFFFF,0x00000000,
	0xF3333333,0xAEEEEEEE,0x00000000,
	0x00000000,0xBEEEEEEE,0x00000000,
	0x00000000,0x7DDDDDDD,0x00000000,
	0x00000000	/* Terminator */
};

#define COL_GRAY1 0		// middle
#define COL_BLACK 1
#define COL_WHITE 2
#define COL_COLOR 3
#define COL_GRAY2 5		// bright
#define COL_GRAY3 6		// dark

UWORD DriPens3[]=
{
/*  0 */	COL_WHITE,	/* DETAILPEN        */
/*  1 */	COL_BLACK,	/* BLOCKPEN         */
/*  2 */	COL_BLACK,	/* TEXTPEN          */
/*  3 */	COL_WHITE,	/* SHINEPEN         */
/*  4 */	COL_BLACK,	/* SHADOWPEN	    */
/*  5 */	COL_COLOR,	/* FILLPEN		    */
/*  6 */	COL_BLACK,	/* FILLTEXTPEN      */
/*  7 */	COL_GRAY2,	/* BACKGROUNDPEN	*/
/*  8 */	COL_WHITE,	/* HIGHLIGHTTEXTPEN */
/*  9 */	COL_BLACK,	/* BARDETAILPEN	    */
/* 10 */	COL_WHITE,	/* BARBLOCKPEN      */
/* 11 */	COL_BLACK,	/* BARTRIMPEN       */
/* 12 */	(unsigned short)~0			/* END OF ARRAY     */
};

struct TagItem scrtags[]=
{
#define IX_SA_Left			 0
								SA_Left,		0,
#define IX_SA_Top			 1
								SA_Top,			0,
#define IX_SA_Width			 2
								SA_Width,		640,
#define IX_SA_Height		 3
								SA_Height,		265,
#define IX_SA_Depth			 4
								SA_Depth,		8,
#define IX_SA_Title			 5
								SA_Title,		(ULONG)"Toolbar",
#define IX_SA_Colors32		 6
								SA_Colors32,	(ULONG)imgPaletteRGB32,
#define IX_SA_Type			 7
								SA_Type,		CUSTOMSCREEN,
#define IX_SA_DisplayID		 8
								SA_DisplayID,	PAL_MONITOR_ID|HIRES_KEY,
#define IX_SA_Pens			 9
								SA_Pens,		(ULONG)&DriPens3,
#define IX_SA_Interleaved	10
								SA_Interleaved,	TRUE,
#define IX_SA_MinimizeISG	11
								SA_MinimizeISG,	TRUE,
#define IX_SA_SharePens		12
								SA_SharePens,	TRUE,
/* 18 */						TAG_DONE
};

struct TagItem bdwintags[]=
{
/*  0 */	WA_Left,		0,
/*  1 */	WA_Top,			0,
/*  2 */	WA_Width,		640,
/*  3 */	WA_Height,		264,
/*  4 */	WA_IDCMP,		IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_MENUPICK|IDCMP_ACTIVEWINDOW|IDCMP_MOUSEMOVE,
/*  5 */	WA_Flags,		WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_BACKDROP|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
/*  6 */	WA_Gadgets,		0l,
/*  7 */	WA_ScreenTitle,	(ULONG)"Toolbar Test",
/*  8 */	WA_CustomScreen,0l,
/*  9 */	TAG_DONE
};

struct TagItem wintags[]=
{
/*  0 */	WA_Left,		100,
/*  1 */	WA_Top,			150,
/*  2 */	WA_InnerWidth,	250,
/*  3 */	WA_InnerHeight,	100,
/*  4 */	WA_IDCMP,		IDCMP_CLOSEWINDOW|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_MENUPICK|IDCMP_ACTIVEWINDOW|IDCMP_CHANGEWINDOW|IDCMP_MOUSEMOVE,
/*  5 */	WA_Flags,		WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_SIZEGADGET|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
/*  6 */	WA_Gadgets,		0l,
/*  7 */	WA_ScreenTitle,	(ULONG)"Toolbar Test",
/*  8 */	WA_CustomScreen,0l,
/*  9 */	WA_Title,		(ULONG)"Testwindow",
/* 10 */	WA_MinWidth,	100,
/* 11 */	WA_MinHeight,	40,
/* 12 */	WA_MaxWidth,	640,
/* 13 */	WA_MaxHeight,	264,
/* 14 */	TAG_DONE
};

struct TagItem tbwintags[]=
{
/*  0 */	WA_Left,		0,
/*  1 */	WA_Top,			0,
/*  2 */	WA_Width,		640,
/*  3 */	WA_Height,		TB_HEIGHT,
/*  4 */	WA_IDCMP,		IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_ACTIVEWINDOW|IDCMP_GADGETUP|IDCMP_MOUSEMOVE,
/*  5 */	WA_Flags,		WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
/*  6 */	WA_Gadgets,		NULL,
/*  7 */	WA_ScreenTitle,	(ULONG)"Toolbar Test",
/*  8 */	WA_CustomScreen,0l,
/*  9 */	WA_MinWidth,	640,
/* 10 */	WA_MinHeight,	264,
/* 11 */	WA_MaxWidth,	640,
/* 12 */	WA_MaxHeight,	264,
/* 13 */	TAG_DONE
};

struct TagItem sbwintags[]=
{
/*  0 */	WA_Left,		0,
/*  1 */	WA_Top,			0,
/*  2 */	WA_Width,		640,
/*  3 */	WA_Height,		SB_HEIGHT,
/*  4 */	WA_IDCMP,		IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_MENUPICK|IDCMP_ACTIVEWINDOW|IDCMP_MOUSEMOVE,
/*  5 */	WA_Flags,		WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
/*  6 */	WA_Gadgets,		0l,
/*  7 */	WA_ScreenTitle,	(ULONG)"Toolbar Test",
/*  8 */	WA_CustomScreen,0l,
/*  9 */	WA_MinWidth,	640,
/* 10 */	WA_MinHeight,	264,
/* 11 */	WA_MaxWidth,	640,
/* 12 */	WA_MaxHeight,	264,
/* 13 */	TAG_DONE
};

struct TagItem ddwintags[]=
{
/*  0 */	WA_Left,		0,
/*  1 */	WA_Top,			0,
/*  2 */	WA_Width,		640,
/*  3 */	WA_Height,		SB_HEIGHT,
/*  4 */	WA_IDCMP,		IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_MENUPICK|IDCMP_ACTIVEWINDOW|IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS,
/*  5 */	WA_Flags,		WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE|WFLG_ACTIVATE,
/*  6 */	WA_Gadgets,		0l,
/*  7 */	WA_ScreenTitle,	(ULONG)"Toolbar Test",
/*  8 */	WA_CustomScreen,0l,
/*  9 */	WA_MinWidth,	640,
/* 10 */	WA_MinHeight,	264,
/* 11 */	WA_MaxWidth,	640,
/* 12 */	WA_MaxHeight,	264,
/* 13 */	TAG_DONE
};

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

struct NewToolbar SFXToolbar[]=
{
/* 00 */	{ NT_TOOLBAR,	NULL,			0L,				0L,				"Toolbar 1",			0L,			0L,					NULL },	/* we store toolbar pos & size in here */
/* 01 */	{   NT_ITEM,	&imgNew0,		&imgNew1,		&imgNew2,		"Neue Datei",			0L,			GADID_TB_NEW,		NULL },
/* 02 */	{   NT_ITEM,	&imgLoad0,		&imgLoad1,		&imgLoad2,		"Datei laden",			0L,			GADID_TB_LOAD,		NULL },
/* 03 */	{   NT_ITEM,	&imgSave0,		&imgSave1,		&imgSave2,		"Datei speichern",		0L,			GADID_TB_SAVE,		NULL },

/* 04 */	{ NT_TOOLBAR,	NULL,			0L,				0L,				"Toolbar 2",			0L,			0L,					NULL },	/* we store toolbar pos & size in here */
/* 05 */	{   NT_ITEM,	NULL,			(APTR)150,		(APTR)22,		"Operator wählen",		NTB_RIDGE,	0L,					NULL },
/* 06 */	{   NT_ITEM,	&imgDropDown0,	&imgDropDown1,	&imgDropDown2,	"Operator wählen",		0L,			GADID_TB_FXCHOICE,	NULL },
/* 07 */	{   NT_ITEM,	&imgRun0,		&imgRun1,		&imgRun2,		"Operator starten",		0L,			GADID_TB_FXRUN,		NULL },

/* 08 */	{ NT_TOOLBAR,	NULL,			0L,				0L,				"Toolbar 3",			0L,			0L,					NULL },	/* we store toolbar pos & size in here */
/* 09 */	{   NT_ITEM,	&imgCutMen0,	&imgCutMen1,	&imgCutMen2,	"Bereich ausschneiden",	0L,			GADID_TB_CUTMEN,	NULL },
/* 10 */	{   NT_ITEM,	&imgCopyMen0,	&imgCopyMen1,	&imgCopyMen2,	"Bereich kopieren",		0L,			GADID_TB_COPYMEN,	NULL },
/* 11 */	{   NT_ITEM,	&imgPasteMen0,	&imgPasteMen1,	&imgPasteMen2,	"Bereich einfÜgen",		0L,			GADID_TB_PASTEMEN,	NULL },
/* 12 */	{   NT_ITEM,	&imgEraseMen0,	&imgEraseMen1,	&imgEraseMen2,	"Bereich löschen",		0L,			GADID_TB_ERASEMEN,	NULL },
/* 13 */	{   NT_ITEM,	&imgGrabMen0,	&imgGrabMen1,	&imgGrabMen2,	"Bereich herauslösen",	0L,			GADID_TB_GRABMEN,	NULL },
/* 14 */	{   NT_ITEM,	&imgZeroMen0,	&imgZeroMen1,	&imgZeroMen2,	"Bereich stumm schalten",0L,		GADID_TB_ZEROMEN,	NULL },
/* 15 */	{   NT_ITEM,	&imgOverwrMen0,	&imgOverwrMen1,	&imgOverwrMen2,	"Bereich überschreiben",0L,			GADID_TB_OVERWRMEN,	NULL },

/* 16 */	{ NT_TOOLBAR,	NULL,			0L,				0L,				"Toolbar 4",			0L,			0L,					NULL },	/* we store toolbar pos & size in here */
/* 17 */	{   NT_ITEM,	&imgPlayAll0,	&imgPlayAll1,	&imgPlayAll2,	"alles abspielen",		0L,			GADID_TB_PLAYALL,	NULL },
/* 18 */	{   NT_ITEM,	&imgPlayRng0,	&imgPlayRng1,	&imgPlayRng2,	"Bereich abspielen",	0L,			GADID_TB_PLAYRNG,	NULL },
/* 19 */	{   NT_ITEM,	&imgStop0,		&imgStop1,		&imgStop2,		"Abspielen anhalten",	0L,			GADID_TB_STOP,		NULL },
/* 20 */	{   NT_ITEM,	&imgRecord0,	&imgRecord1,	&imgRecord2,	"aufnehmen",			0L,			GADID_TB_RECORD,	NULL },

/* 21 */	{ NT_TOOLBAR,	NULL,			0L,				0L,				"Toolbar 5",			0L,			0L,					NULL },	/* we store toolbar pos & size in here */
/* 22 */	{   NT_ITEM,	&imgZoomIn0,	&imgZoomIn1,	&imgZoomIn2,	"Bereich vergrößern",	0L,			GADID_TB_ZOOMIN,	NULL },
/* 23 */	{   NT_ITEM,	&imgZoomOut0,	&imgZoomOut1,	&imgZoomOut2,	"Bereich verkleinern",	0L,			GADID_TB_ZOOMOUT,	NULL },
/* 24 */	{   NT_ITEM,	&imgZoomEq0,	&imgZoomEq1,	&imgZoomEq2,	"Bereich 1:1",			0L,			GADID_TB_ZOOMEQ,	NULL },
/* 25 */	{   NT_ITEM,	&imgZoomOutAll0,&imgZoomOutAll1,&imgZoomOutAll2,"Sample komplett",		0L,			GADID_TB_ZOOMOUTALL,NULL },

/* 26 */	{ NT_TOOLBAR,	NULL,			0L,				0L,				"Toolbar 6",			0L,			0L,					NULL },	/* we store toolbar pos & size in here */
/* 27 */	{   NT_ITEM,	&imgZoom0,		&imgZoom1,		&imgZoom2,		"Bereichsmodus",		0L,			GADID_TB_RNGMODE,	NULL },
/* 28 */	{   NT_ITEM,	&imgLockOp0,	&imgLockOp1,	&imgLockOp2,	"Länge feststellen",	0L,			GADID_TB_LOCKLEN,	NULL },
/* 29 */	{   NT_ITEM,	NULL,			(APTR)10,		(APTR)27,		"",						NTB_NONE,	0L,					NULL },
/* 30 */	{   NT_ITEM,	&imgMoveBeg0,	&imgMoveBeg1,	&imgMoveBeg2,	"Bereich ...",			0L,			GADID_TB_MOVEBEG,	NULL },
/* 31 */	{   NT_ITEM,	&imgMoveLeftF0,	&imgMoveLeftF1,	&imgMoveLeftF2,	"Bereich ...",			0L,			GADID_TB_MOVELEFTF,	NULL },
/* 32 */	{   NT_ITEM,	&imgMoveLeftS0,	&imgMoveLeftS1,	&imgMoveLeftS2,	"Bereich ...",			0L,			GADID_TB_MOVELEFTS,	NULL },
/* 33 */	{   NT_ITEM,	&imgMoveLeftN0,	&imgMoveLeftN1,	&imgMoveLeftN2,	"Bereich ...",			0L,			GADID_TB_MOVELEFTN,	NULL },
/* 34 */	{   NT_ITEM,	&imgMoveRightN0,&imgMoveRightN1,&imgMoveRightN2,"Bereich ...",			0L,			GADID_TB_MOVERIGHTN,NULL },
/* 35 */	{   NT_ITEM,	&imgMoveRightS0,&imgMoveRightS1,&imgMoveRightS2,"Bereich ...",			0L,			GADID_TB_MOVERIGHTS,NULL },
/* 36 */	{   NT_ITEM,	&imgMoveRightF0,&imgMoveRightF1,&imgMoveRightF2,"Bereich ...",			0L,			GADID_TB_MOVERIGHTF,NULL },
/* 37 */	{   NT_ITEM,	&imgMoveEnd0,	&imgMoveEnd1,	&imgMoveEnd2,	"Bereich ...",			0L,			GADID_TB_MOVEEND,	NULL },
/* 38 */ 	{   NT_ITEM,	NULL,			(APTR)10,		(APTR)27,		"",						NTB_NONE,	0L,					NULL },
/* 39 */	{   NT_ITEM,	&imgMoveTop0,	&imgMoveTop1,	&imgMoveTop2,	"Bereich ...",			0L,			GADID_TB_MOVETOP,	NULL },
/* 40 */	{   NT_ITEM,	&imgMoveUpF0,	&imgMoveUpF1,	&imgMoveUpF2,	"Bereich ...",			0L,			GADID_TB_MOVEUPF,	NULL },
/* 41 */	{   NT_ITEM,	&imgMoveUpS0,	&imgMoveUpS1,	&imgMoveUpS2,	"Bereich ...",			0L,			GADID_TB_MOVEUPS,	NULL },
/* 42 */	{   NT_ITEM,	&imgMoveUpP0,	&imgMoveUpP1,	&imgMoveUpP2,	"Bereich ...",			0L,			GADID_TB_MOVEUPP,	NULL },
/* 43 */	{   NT_ITEM,	&imgMoveDownP0,	&imgMoveDownP1,	&imgMoveDownP2,	"Bereich ...",			0L,			GADID_TB_MOVEDOWNP,	NULL },
/* 44 */	{   NT_ITEM,	&imgMoveDownS0,	&imgMoveDownS1,	&imgMoveDownS2,	"Bereich ...",			0L,			GADID_TB_MOVEDOWNS,	NULL },
/* 45 */	{   NT_ITEM,	&imgMoveDownF0,	&imgMoveDownF1,	&imgMoveDownF2,	"Bereich ...",			0L,			GADID_TB_MOVEDOWNF,	NULL },
/* 46 */	{   NT_ITEM,	&imgMoveBottom0,&imgMoveBottom1,&imgMoveBottom2,"Bereich ...",			0L,			GADID_TB_MOVEBOTTOM,NULL },
/* 47 */	{ NT_END,	   NULL,			0L,				0L,				0L,						0L,			0L,					NULL },	/* we store toolbar-window size in here */
};

//#####################
#define	GADIX_TB_RNGMODE	27
#define	GADIX_TB_LOCKLEN	28
#define SFXTB_END			47
//#####################

struct NewToolbar SFXToolbarCut[]=
{
/* 00 */	{   NT_ITEM,	&imgCut0,		&imgCut1,		&imgCut2,		"Bereich ausschneiden",					0L,			GADID_DD_CUT,		NULL },
/* 01 */	{   NT_ITEM,	&imgCutL0,		&imgCutL1,		&imgCutL2,		"Bereich bis zum Beginn ausschneiden",	0L,			GADID_DD_CUTLEFT,	NULL },
/* 02 */	{   NT_ITEM,	&imgCutR0,		&imgCutR1,		&imgCutR2,		"Bereich bis zum Ende ausschneiden",	0L,			GADID_DD_CUTRIGHT,	NULL },
};

struct Gadget tbCut			={NULL,			2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCut0,		&imgCut2	,NULL,0,NULL,GADID_DD_CUT		,&SFXToolbarCut[0]	};
struct Gadget tbCutLeft 	={&tbCut,		2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCutL0,		&imgCutL2	,NULL,0,NULL,GADID_DD_CUTLEFT	,&SFXToolbarCut[1]	};
struct Gadget tbCutRight	={&tbCutLeft,	2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCutR0,		&imgCutR2	,NULL,0,NULL,GADID_DD_CUTRIGHT	,&SFXToolbarCut[2]	};

struct NewToolbar SFXToolbarCopy[]=
{
/* 00 */	{   NT_ITEM,	&imgCopy0,		&imgCopy1,		&imgCopy2,		"Bereich kopieren",						0L,			GADID_DD_COPY,		NULL },
/* 01 */	{   NT_ITEM,	&imgCopyL0,		&imgCopyL1,		&imgCopyL2,		"Bereich bis zum Beginn kopieren",		0L,			GADID_DD_COPYLEFT,	NULL },
/* 02 */	{   NT_ITEM,	&imgCopyR0,		&imgCopyR1,		&imgCopyR2,		"Bereich bis zum Ende kopieren",		0L,			GADID_DD_COPYRIGHT,	NULL },
};

struct Gadget tbCopy		={NULL,			2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCopy0,		&imgCopy2	,NULL,0,NULL,GADID_DD_COPY		,&SFXToolbarCopy[0]	};
struct Gadget tbCopyLeft	 ={&tbCopy,		2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCopyL0,	&imgCopyL2	,NULL,0,NULL,GADID_DD_COPYLEFT	,&SFXToolbarCopy[1]	};
struct Gadget tbCopyRight	={&tbCopyLeft,	2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgCopyR0,	&imgCopyR2	,NULL,0,NULL,GADID_DD_COPYRIGHT	,&SFXToolbarCopy[2]	};

struct NewToolbar SFXToolbarPaste[]=
{
/* 00 */	{   NT_ITEM,	&imgPaste0,		&imgPaste1,		&imgPaste2,		"Bereich einfügen",						0L,			GADID_DD_PASTE,		NULL },
/* 01 */	{   NT_ITEM,	&imgPasteL0,	&imgPasteL1,	&imgPasteL2,	"Bereich am Beginn einfügen",			0L,			GADID_DD_PASTELEFT,	NULL },
/* 02 */	{   NT_ITEM,	&imgPasteR0,	&imgPasteR1,	&imgPasteR2,	"Bereich am Ende einfügen",				0L,			GADID_DD_PASTERIGHT,NULL },
};

struct Gadget tbPaste		={NULL,			2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgPaste0,	&imgPaste2	,NULL,0,NULL,GADID_DD_PASTE		,&SFXToolbarPaste[0]	};
struct Gadget tbPasteLeft	={&tbPaste,		2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgPasteL0,	&imgPasteL2	,NULL,0,NULL,GADID_DD_PASTELEFT	,&SFXToolbarPaste[1]	};
struct Gadget tbPasteRight	={&tbPasteLeft,	2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgPasteR0,	&imgPasteR2	,NULL,0,NULL,GADID_DD_PASTERIGHT,&SFXToolbarPaste[2]	};

struct NewToolbar SFXToolbarErase[]=
{
/* 00 */	{   NT_ITEM,	&imgErase0,		&imgErase1,		&imgErase2,		"Bereich löschen",						0L,			GADID_DD_ERASE,		NULL },
/* 01 */	{   NT_ITEM,	&imgEraseL0,	&imgEraseL1,	&imgEraseL2,	"Bereich bis zum Beginn löschen",		0L,			GADID_DD_ERASELEFT,	NULL },
/* 02 */	{   NT_ITEM,	&imgEraseR0,	&imgEraseR1,	&imgEraseR2,	"Bereich bis zum Ende löschen",			0L,			GADID_DD_ERASERIGHT,NULL },
};

struct Gadget tbErase		={NULL,			2, 2,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgErase0,	&imgErase2	,NULL,0,NULL,GADID_DD_ERASE		,&SFXToolbarErase[0]	};
struct Gadget tbEraseLeft	={&tbErase,		2,29,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgEraseL0,	&imgEraseL2	,NULL,0,NULL,GADID_DD_ERASELEFT	,&SFXToolbarErase[1]	};
struct Gadget tbEraseRight	={&tbEraseLeft,	2,56,27,27,GFLG_GADGIMAGE|GFLG_GADGHIMAGE,GACT_RELVERIFY,GTYP_BOOLGADGET,&imgEraseR0,	&imgEraseR2	,NULL,0,NULL,GADID_DD_ERASERIGHT,&SFXToolbarErase[2]	};


//-- definitions --------------------------------------------------------------

void Box3DOut(struct RastPort *rp,UWORD x1, UWORD y1, UWORD x2, UWORD y2)
{
	Move(rp,x1,y2);
	SetAPen(rp,2);Draw(rp,x1,y1);Draw(rp,x2,y1);
	SetAPen(rp,1);Draw(rp,x2,y2);Draw(rp,x1,y2);
}

void Box3DIn(struct RastPort *rp,UWORD x1, UWORD y1, UWORD x2, UWORD y2)
{
	Move(rp,x1,y2);
	SetAPen(rp,1);Draw(rp,x1,y1);Draw(rp,x2,y1);
	SetAPen(rp,2);Draw(rp,x2,y2);Draw(rp,x1,y2);
}

void ShowDropDownWin_TextList(UWORD x1,UWORD y1,UWORD x2,UBYTE **choices)
{
	struct NewGadget ng;
	struct Gadget *g;

	if(ddwin)			{ CloseWindow(ddwin);MSG("oops"); }

	if(g=CreateContext(&ddglist))
	{
		ng.ng_LeftEdge		=0;
		ng.ng_TopEdge		=0;
		ng.ng_Width			=x2-x1;
		ng.ng_Height		=DD_HEIGHT;
		ng.ng_GadgetText	="";
		ng.ng_TextAttr		=NULL;
		ng.ng_GadgetID		=GADID_DD_LIST;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=VisualInfo;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,0l,GTLV_Selected,0,GTLV_Top,0,GTLV_ShowSelected,0l,TAG_DONE);

		ddwintags[0].ti_Data=x1;
		ddwintags[1].ti_Data=y1;
		ddwintags[2].ti_Data=x2-x1;
		ddwintags[3].ti_Data=DD_HEIGHT;
		ddwintags[6].ti_Data=(ULONG)ddglist;
		if(!(ddwin=OpenWindowTagList(0l,ddwintags)))							 			{ printf("Can't open dd-textlist window !\n");done(); }
		{
			GT_RefreshWindow(ddwin,0l);
			Box3DOut(ddwin->RPort,0,0,((x2-x1)-1),(DD_HEIGHT-1));
		}
	}
}

void ShowDropDownWin_Images(UWORD x1,UWORD y1,UWORD x2,UWORD y2,struct Gadget *gadgets)
{
	if(ddwin)			{ CloseWindow(ddwin);MSG("oops"); }
	ddwintags[0].ti_Data=x1;
	ddwintags[1].ti_Data=y1;
	ddwintags[2].ti_Data=x2-x1;
	ddwintags[3].ti_Data=y2-y1;
	ddwintags[6].ti_Data=(ULONG)gadgets;
	if(!(ddwin=OpenWindowTagList(0l,ddwintags)))							 			{ printf("Can't open dd-image window !\n");done(); }
	{
		Box3DOut(ddwin->RPort,0,0,((x2-x1)-1),((y2-y1)-1));
	}
}

void HighLightGadgets(UWORD smx,UWORD smy)
{
	BOOL found;
	UWORD wmx,wmy,wix=0;
//	APTR tmp;
	WORD ngp;
	struct Gadget *ngad;
	struct Window *w,*wins[3];

	if(ddwin) { wins[0]=ddwin;wins[1]=tbwin;wins[2]=0l; }
	else	  {	wins[0]=tbwin;wins[1]=0l; }
	if(owin && (owin!=tbwin) && (owin!=ddwin)) { owin=NULL;ogad=NULL;MSG("oops"); }

	found=FALSE;
	w=wins[wix];
	while(w)									/* check all toolbar windows */
	{
		if((smx>w->LeftEdge) && (smy>w->TopEdge) && (smx<w->LeftEdge+w->Width) && (smy<w->TopEdge+w->Height))	/* is mouse inside this window ? */
		{
			wmx=smx-w->LeftEdge;wmy=smy-w->TopEdge;
			ngad=w->FirstGadget;ngp=0;
			while(ngad)
			{
				if(ngad->GadgetType==GTYP_BOOLGADGET)
				{
					if((wmx>ngad->LeftEdge) && (wmy>ngad->TopEdge) && (wmx<ngad->LeftEdge+ngad->Width) && (wmy<ngad->TopEdge+ngad->Height))
					{
						if(ngad!=ogad)
						{
							if(ogad && owin)		// altes Gadget 'de'highlighten
							{
								RemoveGadget(owin,ogad);
								ogad->GadgetRender=((struct NewToolbar *)ogad->UserData)->nt_Image;
//								tmp=ogad->UserData;ogad->UserData=ogad->GadgetRender;ogad->GadgetRender=tmp;
								AddGadget(owin,ogad,ogp);
								RefreshGList(ogad,owin,0l,1);
							}
							RemoveGadget(w,ngad);
							ngad->GadgetRender=((struct NewToolbar *)ngad->UserData)->nt_HighImage;
//							tmp=ngad->UserData;ngad->UserData=ngad->GadgetRender;ngad->GadgetRender=tmp;
							AddGadget(w,ngad,ngp);
							RefreshGList(ngad,w,0l,1);
							SetStatusbar(((struct NewToolbar *)ngad->UserData)->nt_Label);
							ogad=ngad;ogp=ngp;owin=w;
						}
						found=TRUE;
						ngad=NULL;w=NULL;
					}
				}
				if(!found) ngad=ngad->NextGadget;
				ngp++;
			}
		}
		if(!found)
		{
			wix++;
			w=wins[wix];
		}
	}
	if((!found) && ogad && owin)
	{
		RemoveGadget(owin,ogad);
		ogad->GadgetRender=((struct NewToolbar *)ogad->UserData)->nt_Image;
//		tmp=ogad->UserData;ogad->UserData=ogad->GadgetRender;ogad->GadgetRender=tmp;
		AddGadget(owin,ogad,ogp);
		RefreshGList(ogad,owin,0l,1);
		SetStatusbar(NULL);
		ogad=NULL;ogp=0;owin=0;
	}
}

//--

struct Gadget *CreateToolbars(struct NewToolbar *ntb,ULONG maxwidth,ULONG maxheight)
{
	struct NewToolbar *ntb_tbhead=NULL;
	UWORD x=0,y=0,w,h,lh=0;
	UWORD ix,iy,iw,ih;
	struct Gadget *glist=NULL,*ng,*og=NULL;

	og=CreateContext(&glist);

	while(ntb->nt_Type!=NT_END)							/* read all entries */
	{
		if(ntb->nt_Type==NT_TOOLBAR)							/* a new toolbar starts */
		{
//			MSG2("%4d : New Toolbar : %s",x,ntb->nt_Label);
			ntb_tbhead=ntb++;									/* remember head element; we enter layoutinfo there */
			w=h=0;
			while(ntb->nt_Type==NT_ITEM)						/* read all toolbar elements and calculate toolbar size */
			{
				if(ntb->nt_Image) { w+=ntb->nt_Image->Width;if(ntb->nt_Image->Height>h) h=ntb->nt_Image->Height; }
				else			  { w+=ntb->nt_Width;		if(ntb->nt_Height>h) h=ntb->nt_Height; }
				ntb++;
			}
			w+=4;h+=4;											/* add some space */
//			MSG2("  w=%4d  h=%2d",w,h);
			if(w>maxwidth) w=maxwidth;												/* hope this never happens */
			if(x+w>maxwidth) { y+=lh;lh=h;x=0;/*ntb_tbhead->nt_Image=(struct Image *)1;*/ }	/* this toolbar won't fit into this line */
			if(h>lh) lh=h;										/* if this toolbar is higher than the ones before, change line-height */
			ntb=ntb_tbhead;
			ntb->nt_Pos =(ULONG)(x<<16)|y;
			ntb->nt_Size=(ULONG)(w<<16)|h;
			ntb++;
			ix=x+2;iy=y;x+=w;
//			MSG2("  x=%4d  y=%2d",x,y);
			while(ntb->nt_Type==NT_ITEM)						/* read all toolbar elements again and create gadgets */
			{
				if(ntb->nt_Image)
				{
					if(ng=AllocVec(sizeof(struct Gadget),MEMF_ANY))
					{
						og->NextGadget	=ng;
						ng->NextGadget  =NULL;
						ng->LeftEdge	=ix;
						ng->TopEdge		=iy+((h-ntb->nt_Image->Height)>>1);
						ng->Width		=ntb->nt_Image->Width;
						ng->Height		=ntb->nt_Image->Height;
						ng->Flags		=GFLG_GADGIMAGE|GFLG_GADGHIMAGE;
						ng->Activation	=GACT_RELVERIFY;
						ng->GadgetType	=GTYP_BOOLGADGET;
						ng->GadgetRender=ntb->nt_Image;
						ng->SelectRender=ntb->nt_ActiveImage;
						ng->GadgetText	=NULL;
						ng->MutualExclude=0L;
						ng->SpecialInfo	=NULL;
						ng->GadgetID	=ntb->nt_GadgetID;
						ng->UserData	=ntb;
						ntb->nt_Gadget	=ng;
						og=ng;
						ix+=ng->Width;
					}
				}
				else
				{
//					if(ntb->nt_Gadget)		/* there is some gadget attaced */ 
//					{
//						ng=ntb->nt_Gadget;
//						ng->NextGadget  =NULL;
//						og->NextGadget	=ng;
//						og=ng;
//					}
					iw=ntb->nt_Width;
					ih=ntb->nt_Height;
					ntb->nt_Pos =(ULONG)(ix<<16)|(iy+((h-ih)>>1));
					ntb->nt_Size=(ULONG)(iw<<16)|ih;
					ix+=iw;
				}
				ntb++;
			}
		}
		else ntb++;
	}
//	ntb_tbhead->nt_Image=(struct Image *)1;
	ntb->nt_Width=maxwidth;
	ntb->nt_Height=y+lh;
	return(glist);
}

void DrawToolbars(struct NewToolbar *ntb,struct RastPort *rp)
{
	UWORD x1,x2,y1,y2;
	
	while(ntb->nt_Type!=NT_END)							/* read all entries */
	{
		if(ntb->nt_Type==NT_TOOLBAR)							/* a toolbar */
		{
			x1= (ntb->nt_Pos &0xFFFF0000)>>16;				/* get pos & size and draw it */
			y1=  ntb->nt_Pos &0x0000FFFF;
			x2=((ntb->nt_Size&0xFFFF0000)>>16)-1;
			y2=( ntb->nt_Size&0x0000FFFF)     -1;
			Box3DOut(rp,x1,y1,x1+x2,y1+y2);
//			if(ntb->nt_Image==(struct Image *)1)			/* last toolbar in row */
//			{
//				x1+=x2+1;
//				Box3DOut(rp,x1,y1,scr->Width-1,y1+y2);
//			}
		}
		if(ntb->nt_Type==NT_ITEM)							/* a toolbar item */
		{
			if(!ntb->nt_Image)
			{
				x1= (ntb->nt_Pos &0xFFFF0000)>>16;				/* get pos & size and draw it */
				y1=  ntb->nt_Pos &0x0000FFFF;
				x2=((ntb->nt_Size&0xFFFF0000)>>16)-1;
				y2=( ntb->nt_Size&0x0000FFFF)     -1;
				switch(ntb->nt_Border)
				{
					case NTB_RAISED: Box3DOut(rp,x1,y1,x1+x2,y1+y2);break;
					case NTB_SUNKEN: Box3DIn (rp,x1,y1,x1+x2,y1+y2);break;
					case NTB_RIDGE:  Box3DOut(rp,x1,y1,x1+x2,y1+y2);Box3DIn (rp,x1+1,y1+1,x1+x2-1,y1+y2-1);break;
					case NTB_GROOVE: Box3DIn (rp,x1,y1,x1+x2,y1+y2);Box3DOut(rp,x1+1,y1+1,x1+x2-1,y1+y2-1);break;
				}
			}
		}
		ntb++;
	}
}

void FreeToolbars(struct NewToolbar *ntb)
{
	while(ntb->nt_Type!=NT_END)							/* read all entries */
	{
		if(ntb->nt_Type==NT_TOOLBAR)							/* a new toolbar starts */
		{
			ntb++;
			while(ntb->nt_Type==NT_ITEM)						/* read all toolbar elements and free memory if neccesary */
			{
				if(ntb->nt_Image && ntb->nt_Gadget) { FreeVec(ntb->nt_Gadget);ntb->nt_Gadget=NULL; }
				ntb++;
			}
		}
		else ntb++;
	}
}

//--

void SetStatusbar(STRPTR msg)
{
	SetAPen(sbwin->RPort,0);
	RectFill(sbwin->RPort,2,2,(scr->Width-3),(SB_HEIGHT-3));
	if(msg)
	{
		SetAPen(sbwin->RPort,1);
		Move(sbwin->RPort,5,12);
		Text(sbwin->RPort,msg,strlen(msg));
	}
}

void NG_HandleEvents(ULONG iclass,UWORD icode,APTR iadr,struct Window *iwin,WORD imx,WORD imy,BOOL *closedd)
{
	UWORD x1,y1,x2,y2,w,h;
	BOOL change=FALSE;
	
	switch(iclass)
	{
		case IDCMP_MOUSEMOVE:
			HighLightGadgets(imx+iwin->LeftEdge,imy+iwin->TopEdge);
			break;
		case IDCMP_CHANGEWINDOW:
			w=iwin->Width;	x1=iwin->LeftEdge;	x2=x1+(w-1);
			h=iwin->Height;	y1=iwin->TopEdge;	y2=y1+(h-1);
			if(w>ClientRect.Width)		{ MSG("ChWin : w"); change=TRUE;w =ClientRect.Width;x2=x1+(w-1); }
			if(h>ClientRect.Height)		{ MSG("ChWin : h"); change=TRUE;h =ClientRect.Height;y2=y1+(h-1); }
			if(x1<ClientRect.LeftEdge)	{ MSG("ChWin : x1");change=TRUE;x1=ClientRect.LeftEdge; }
			if(y1<ClientRect.TopEdge)	{ MSG("ChWin : y1");change=TRUE;y1=ClientRect.TopEdge; }
			if(x2>ClientRect.RightEdge)	{ MSG("ChWin : x2");change=TRUE;x2=ClientRect.RightEdge;x1=x2-(w-1); }
			if(y2>ClientRect.BottomEdge){ MSG("ChWin : y2");change=TRUE;y2=ClientRect.BottomEdge;y1=y2-(h-1); }
			if(change) ChangeWindowBox(iwin,x1,y1,w,h);
			break;
		case IDCMP_ACTIVEWINDOW:
			if(iwin!=ddwin) *closedd=TRUE;
			break;
	}
}

//--

void init(void)
{
	//-- open some libraries
	if(!(IntuitionBase	=(struct IntuitionBase *)OpenLibrary("intuition.library",39L))) { printf("Can't open intuition.library >=V39 !\n");done(); }
	if(!(GadToolsBase	=OpenLibrary("gadtools.library",39L)))							{ printf("Can't open gadtools.library >=V39 !\n");done(); }
	if(!(GfxBase		=(struct GfxBase *)OpenLibrary("graphics.library",39L)))		{ printf("Can't open graphics.library >=V39 !\n");done(); }

	//-- clone wb-screenmode
	if(!(scr=LockPubScreen(0l)))				{ printf("Can't lock def pub screen !");done(); }
	else
	{
		scrtags[IX_SA_DisplayID	].ti_Data=GetVPModeID(&(scr->ViewPort));
		scrtags[IX_SA_Width		].ti_Data=scr->Width;
		scrtags[IX_SA_Height	].ti_Data=scr->Height;
		UnlockPubScreen(0l,scr);
	}

	//-- open screen
	if(!(scr=OpenScreenTagList(0l,scrtags)))				{ printf("Can't open screen !\n");done(); }
	scr->WBorBottom=4;					// small hack to have proportinal borders
	bdwintags[8].ti_Data=tbwintags[8].ti_Data=sbwintags[8].ti_Data=ddwintags[8].ti_Data=wintags[8].ti_Data=(ULONG)scr;
	vp=&scr->ViewPort;

	if(!(VisualInfo=GetVisualInfo(scr,TAG_DONE)))						{ printf("Can't get visual info !\n");done(); }

	//-- this area is available for windows, everytime we add an window to the border, we have to adapt the values
	ClientRect.LeftEdge	=0;					ClientRect.RightEdge	=scr->Width-1;	ClientRect.Width	=scr->Width;
	ClientRect.TopEdge	=scr->BarHeight+1;	ClientRect.BottomEdge	=scr->Height-1;	ClientRect.Height	=scr->Height-(scr->BarHeight+1);

	//-- now add a toolbar area at the top
	if(!(tbgad=CreateToolbars(SFXToolbar,scr->Width,200)))								{ printf("Can't create toolbars !\n");done(); }
	tbwintags[ 1].ti_Data=scr->BarHeight+1;
	tbwintags[ 2].ti_Data=scr->Width;
	tbwintags[ 3].ti_Data=SFXToolbar[SFXTB_END].nt_Height;
	tbwintags[ 6].ti_Data=(ULONG)tbgad;
	if(!(tbwin=OpenWindowTagList(0l,tbwintags)))							 			{ printf("Can't open tb window !\n");done(); }
	DrawToolbars(SFXToolbar,tbwin->RPort);
	ClientRect.TopEdge	+=SFXToolbar[SFXTB_END].nt_Height;	ClientRect.Height	-=SFXToolbar[SFXTB_END].nt_Height;

	//-- and a status bar at the bottom
	sbwintags[1].ti_Data=scr->Height-(SB_HEIGHT-1);
	sbwintags[2].ti_Data=scr->Width;
	if(!(sbwin=OpenWindowTagList(0l,sbwintags)))							 			{ printf("Can't open sb window !\n");done(); }
	Box3DOut(sbwin->RPort,0,0,(scr->Width-1),(SB_HEIGHT-1));
	Box3DIn (sbwin->RPort,1,1,(scr->Width-2),(SB_HEIGHT-2));
	ClientRect.BottomEdge	-=SB_HEIGHT;	ClientRect.Height	-=SB_HEIGHT;

	//-- now we open a backdropwindow as big as our client rect
	bdwintags[0].ti_Data=ClientRect.LeftEdge;
	bdwintags[1].ti_Data=ClientRect.TopEdge;
	bdwintags[2].ti_Data=ClientRect.Width;
	bdwintags[3].ti_Data=ClientRect.Height;
	if(!(bdwin=OpenWindowTagList(0l,bdwintags)))							 			{ printf("Can't open bd window !\n");done(); }
	{
		struct RastPort *bdrp=bdwin->RPort;

		SetRast(bdrp,6);
		SetABPenDrMd(bdrp,1,5,JAM1);
		DrawImage(bdrp,&backlogo,(scr->Width-277)>>1,(scr->Height-103)>>1);
		Box3DOut(bdrp,0,0,RunTime.clientrect.Width-1,RunTime.clientrect.Height-1);
	}

	//-- and finally theres some application window
	wintags[12].ti_Data=scr->Width;
	wintags[13].ti_Data=scr->Height-(scr->BarHeight+1)-SB_HEIGHT-TB_HEIGHT;
	if(!(win=OpenWindowTagList(0l,wintags)))								 			{ printf("Can't open data window !\n");done(); }
	rp=win->RPort;
}

void done(void)
{
	if(ddwin)			CloseWindow(ddwin);
	if(sbwin)			CloseWindow(sbwin);
	if(tbwin)			CloseWindow(tbwin);
	if(win)				CloseWindow(win);
	if(bdwin)			CloseWindow(bdwin);

	if(VisualInfo)		FreeVisualInfo(VisualInfo);

	if(scr)				CloseScreen(scr);

	if(tbgad)			FreeToolbars(SFXToolbar);
	
#pragma msg 88 ignore
	if(IntuitionBase)	CloseLibrary(IntuitionBase);
	if(GfxBase)			CloseLibrary(GfxBase);
	if(GadToolsBase)	CloseLibrary(GadToolsBase);
#pragma msg 88 warn
}

void run(void)
{
	BOOL running=TRUE;
	struct IntuiMessage *imsg;
	struct Gadget *ddgadget=NULL;
	struct Window *iwin;
	struct NewToolbar *ntb;
	ULONG maskin,maskout,sig_win,sig_bdwin,sig_tbwin,sig_sbwin,sig_ddwin;
	ULONG iclass=0;
	UWORD icode;
	APTR  iadr;
	WORD imx,imy;
	BOOL closedd,swallowdd=FALSE;
	char msg[200];
	UBYTE rngmode=0,locklen=0;

	sig_win		=(1L<<  win->UserPort->mp_SigBit);
	sig_bdwin	=(1L<<bdwin->UserPort->mp_SigBit);
	sig_tbwin	=(1L<<tbwin->UserPort->mp_SigBit);
	sig_sbwin	=(1L<<sbwin->UserPort->mp_SigBit);
	sig_ddwin	=0l;
	maskin=sig_win|sig_bdwin|sig_tbwin|sig_sbwin;
	while(running)
	{
		closedd=FALSE;
		maskout=Wait(maskin);
		if(maskout&sig_win)
		{
			while(imsg=GT_GetIMsg(win->UserPort))
			{
				iclass=imsg->Class;icode=imsg->Code;iadr=imsg->IAddress;
				iwin=imsg->IDCMPWindow;imx=imsg->MouseX;imy=imsg->MouseY;
				GT_ReplyIMsg(imsg);
				NG_HandleEvents(iclass,icode,iadr,iwin,imx,imy,&closedd);
				switch(iclass)
				{
					case IDCMP_CLOSEWINDOW: running=FALSE;break;
				}
			}
		}
		if(maskout&sig_bdwin)
		{
			while(imsg=GT_GetIMsg(bdwin->UserPort))
			{
				iclass=imsg->Class;icode=imsg->Code;iadr=imsg->IAddress;
				iwin=imsg->IDCMPWindow;imx=imsg->MouseX;imy=imsg->MouseY;
				GT_ReplyIMsg(imsg);
				NG_HandleEvents(iclass,icode,iadr,iwin,imx,imy,&closedd);
			}
		}
		if(maskout&sig_sbwin)
		{
			while(imsg=GT_GetIMsg(sbwin->UserPort))
			{
				iclass=imsg->Class;icode=imsg->Code;iadr=imsg->IAddress;
				iwin=imsg->IDCMPWindow;imx=imsg->MouseX;imy=imsg->MouseY;
				GT_ReplyIMsg(imsg);
				NG_HandleEvents(iclass,icode,iadr,iwin,imx,imy,&closedd);
			}
		}
		if(maskout&sig_ddwin)
		{
			while(imsg=GT_GetIMsg(ddwin->UserPort))
			{
				iclass=imsg->Class;icode=imsg->Code;iadr=imsg->IAddress;
				iwin=imsg->IDCMPWindow;imx=imsg->MouseX;imy=imsg->MouseY;
				GT_ReplyIMsg(imsg);
				NG_HandleEvents(iclass,icode,iadr,iwin,imx,imy,&closedd);
				switch(iclass)
				{
					case IDCMP_MOUSEBUTTONS:
						closedd=TRUE;
						break;
					case IDCMP_GADGETUP:
//					case IDCMP_GADGETDOWN:
						closedd=TRUE;
						switch(((struct Gadget *)iadr)->GadgetID)
						{
							case GADID_DD_LIST:		sprintf(msg,"DropDown : list    ");Move(rp,10,30);Text(rp,msg,19);break;
							case GADID_DD_CUT:		sprintf(msg,"DropDown : cut     ");Move(rp,10,30);Text(rp,msg,19);break;
							case GADID_DD_CUTLEFT:	sprintf(msg,"DropDown : cutleft ");Move(rp,10,30);Text(rp,msg,19);break;
							case GADID_DD_CUTRIGHT:	sprintf(msg,"DropDown : cutright");Move(rp,10,30);Text(rp,msg,19);break;
						}
				}
			}
		}
		if(maskout&sig_tbwin)
		{
			while(imsg=GT_GetIMsg(tbwin->UserPort))
			{
				iclass=imsg->Class;icode=imsg->Code;iadr=imsg->IAddress;
				iwin=imsg->IDCMPWindow;imx=imsg->MouseX;imy=imsg->MouseY;
				GT_ReplyIMsg(imsg);
				NG_HandleEvents(iclass,icode,iadr,iwin,imx,imy,&closedd);
				switch(iclass)
				{
					case IDCMP_GADGETUP:
//					case IDCMP_GADGETDOWN:
						switch(((struct Gadget *)iadr)->GadgetID)
						{
							case GADID_TB_NEW:		sprintf(msg,"ToolBar : new      ");Move(rp,10,30);Text(rp,msg,19);break;
							case GADID_TB_LOAD:		sprintf(msg,"ToolBar : load     ");Move(rp,10,30);Text(rp,msg,19);break;
							case GADID_TB_SAVE:		sprintf(msg,"ToolBar : save     ");Move(rp,10,30);Text(rp,msg,19);break;
							case GADID_TB_FXCHOICE:	sprintf(msg,"ToolBar : fxchoice ");Move(rp,10,30);Text(rp,msg,19);
								if(!swallowdd)
								{
									ddgadget=(struct Gadget *)iadr;
									ShowDropDownWin_TextList(ddgadget->LeftEdge-151,iwin->TopEdge+ddgadget->TopEdge+ddgadget->Height+1,ddgadget->LeftEdge-1,ddChoices);
									sig_ddwin=(1L<<ddwin->UserPort->mp_SigBit);maskin|=sig_ddwin;
								}
								else swallowdd=FALSE;
								break;
							case GADID_TB_CUTMEN:	sprintf(msg,"ToolBar : cutmen   ");Move(rp,10,30);Text(rp,msg,19);
								if(!swallowdd)
								{
									ddgadget=(struct Gadget *)iadr;
									ShowDropDownWin_Images(ddgadget->LeftEdge-2,iwin->TopEdge+ddgadget->TopEdge+ddgadget->Height,ddgadget->LeftEdge+ddgadget->Width+2,iwin->TopEdge+ddgadget->TopEdge+ddgadget->Height+85,&tbCutRight);
									sig_ddwin=(1L<<ddwin->UserPort->mp_SigBit);maskin|=sig_ddwin;
								}
								else swallowdd=FALSE;
								break;
							case GADID_TB_COPYMEN:	sprintf(msg,"ToolBar : copymen  ");Move(rp,10,30);Text(rp,msg,19);
								if(!swallowdd)
								{
									ddgadget=(struct Gadget *)iadr;
									ShowDropDownWin_Images(ddgadget->LeftEdge-2,iwin->TopEdge+ddgadget->TopEdge+ddgadget->Height,ddgadget->LeftEdge+ddgadget->Width+2,iwin->TopEdge+ddgadget->TopEdge+ddgadget->Height+85,&tbCopyRight);
									sig_ddwin=(1L<<ddwin->UserPort->mp_SigBit);maskin|=sig_ddwin;
								}
								else swallowdd=FALSE;
								break;
							case GADID_TB_PASTEMEN:	sprintf(msg,"ToolBar : pastemen ");Move(rp,10,30);Text(rp,msg,19);
								if(!swallowdd)
								{
									ddgadget=(struct Gadget *)iadr;
									ShowDropDownWin_Images(ddgadget->LeftEdge-2,iwin->TopEdge+ddgadget->TopEdge+ddgadget->Height,ddgadget->LeftEdge+ddgadget->Width+2,iwin->TopEdge+ddgadget->TopEdge+ddgadget->Height+85,&tbPasteRight);
									sig_ddwin=(1L<<ddwin->UserPort->mp_SigBit);maskin|=sig_ddwin;
								}
								else swallowdd=FALSE;
								break;
							case GADID_TB_ERASEMEN:	sprintf(msg,"ToolBar : erasemen ");Move(rp,10,30);Text(rp,msg,19);
								if(!swallowdd)
								{
									ddgadget=(struct Gadget *)iadr;
									ShowDropDownWin_Images(ddgadget->LeftEdge-2,iwin->TopEdge+ddgadget->TopEdge+ddgadget->Height,ddgadget->LeftEdge+ddgadget->Width+2,iwin->TopEdge+ddgadget->TopEdge+ddgadget->Height+85,&tbEraseRight);
									sig_ddwin=(1L<<ddwin->UserPort->mp_SigBit);maskin|=sig_ddwin;
								}
								else swallowdd=FALSE;
								break;
							case GADID_TB_RNGMODE:	sprintf(msg,"ToolBar : rngmode  ");Move(rp,10,30);Text(rp,msg,19);
								ntb=((struct NewToolbar *)((struct Gadget *)iadr)->UserData);
								switch(rngmode)
								{
									case 0:	ntb->nt_Image=&imgMark0; ntb->nt_HighImage=&imgMark1; ntb->nt_ActiveImage=&imgMark2; rngmode=1;break;
									case 1:	ntb->nt_Image=&imgLoop0; ntb->nt_HighImage=&imgLoop1; ntb->nt_ActiveImage=&imgLoop2; rngmode=2;break;
									case 2:	ntb->nt_Image=&imgTrace0;ntb->nt_HighImage=&imgTrace1;ntb->nt_ActiveImage=&imgTrace2;rngmode=3;break;
									case 3:	ntb->nt_Image=&imgZoom0; ntb->nt_HighImage=&imgZoom1; ntb->nt_ActiveImage=&imgZoom2; rngmode=0;break;
								}
								RemoveGadget(iwin,(struct Gadget *)iadr);
								((struct Gadget *)iadr)->GadgetRender=ntb->nt_HighImage;
								((struct Gadget *)iadr)->SelectRender=ntb->nt_ActiveImage;
								AddGadget(iwin,(struct Gadget *)iadr,0);
								RefreshGList((struct Gadget *)iadr,iwin,0l,1);
								break;
							case GADID_TB_LOCKLEN:	sprintf(msg,"ToolBar : locklen  ");Move(rp,10,30);Text(rp,msg,19);
								ntb=((struct NewToolbar *)((struct Gadget *)iadr)->UserData);
								switch(locklen)
								{
									case 0: ntb->nt_Image=&imgLockCl0;ntb->nt_HighImage=&imgLockCl1;ntb->nt_ActiveImage=&imgLockCl2;locklen=1;break;
									case 1: ntb->nt_Image=&imgLockOp0;ntb->nt_HighImage=&imgLockOp1;ntb->nt_ActiveImage=&imgLockOp2;locklen=0;break;
								}
								RemoveGadget(iwin,(struct Gadget *)iadr);
								((struct Gadget *)iadr)->GadgetRender=ntb->nt_HighImage;
								((struct Gadget *)iadr)->SelectRender=ntb->nt_ActiveImage;
								AddGadget(iwin,(struct Gadget *)iadr,0);
								RefreshGList((struct Gadget *)iadr,iwin,0l,1);
								break;
						}
						break;
					case IDCMP_ACTIVEWINDOW:
						if((ddwin) && (ddgadget) && (imx>ddgadget->LeftEdge) && (imy>ddgadget->TopEdge) && (imx<ddgadget->LeftEdge+ddgadget->Width) && (imy<ddgadget->TopEdge+ddgadget->Height)) swallowdd=TRUE;
						break;
				}
			}
		}
		if(ddwin && closedd)
		{
			maskin&=~sig_ddwin;sig_ddwin=0;
			CloseWindow(ddwin);ddwin=NULL;
			FreeGadgets(ddglist);ddglist=NULL;
			ddgadget=NULL;
			if(owin==ddwin) { owin=NULL;ogad=NULL; }
		}
	}
}

void main(void)
{
	init();
	run();
	done();
}

//-- eof ----------------------------------------------------------------------
