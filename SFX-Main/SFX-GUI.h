/****h* SoundFX/SFX-GUI.h [4.2] *
*
*  NAME
*    SFX-GUI.h
*  COPYRIGHT
*    $VER: SFX-GUI.h 4.2 (18.02.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    gui init & done routines - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    18.Feb.2002  V 4.2	most recent version
*    15.Feb.2000  V 4.00  most recent version
*    05.Feb.2000  V 3.82  most recent version
*    18.Aug.1998  V 3.70  most recent version
*    19.Jul.1998  V 3.70  initial version
*  NOTES
*
*******
*/

#ifndef SFX_GUI_H
#define SFX_GUI_H

//-- defines ------------------------------------------------------------------

#define PMENID_DRMD_LINES		1
#define PMENID_DRMD_DOTS		(PMENID_DRMD_LINES+1)
#define PMENID_DRMD_DOTSABS		(PMENID_DRMD_DOTS+1)
#define PMENID_DRMD_DOTSHQ		(PMENID_DRMD_DOTSABS+1)
#define PMENID_DRMD_FILLED		(PMENID_DRMD_DOTSHQ+1)
#define PMENID_DRMD_FILLEDABS	(PMENID_DRMD_FILLED+1)
#define PMENID_DRMD_FILLEDHQ	(PMENID_DRMD_FILLEDABS+1)

#define PMENID_LOOP				(PMENID_DRMD_FILLEDHQ+1)

#define PMENID_CH_0				(PMENID_LOOP+1)
#define PMENID_CH_1				(PMENID_CH_0+1)
#define PMENID_CH_2				(PMENID_CH_1+1)
#define PMENID_CH_3				(PMENID_CH_2+1)

#define PMENID_HIDE				(PMENID_CH_3+1)

#define File_Reqtags_TitleText				File_Reqtags[ 0].ti_Data
#define File_Reqtags_InitialLeftEdge		File_Reqtags[ 2].ti_Data
#define File_Reqtags_InitialHeight			File_Reqtags[ 5].ti_Data
#define File_Reqtags_InitialDrawer			File_Reqtags[ 6].ti_Data
#define File_Reqtags_InitialFile			File_Reqtags[ 7].ti_Data
#define File_Reqtags_RejectIcons			File_Reqtags[ 8].ti_Data
#define File_Reqtags_DoSaveMode				File_Reqtags[ 9].ti_Data
#define File_Reqtags_DoMultiSelect			File_Reqtags[10].ti_Data
#define File_Reqtags_DrawersOnly			File_Reqtags[11].ti_Data

#define Font_Reqtags_TitleText				Font_Reqtags[ 0].ti_Data
#define Font_Reqtags_InitialLeftEdge		Font_Reqtags[ 2].ti_Data
#define Font_Reqtags_InitialHeight			Font_Reqtags[ 5].ti_Data
#define Font_Reqtags_InitialName			Font_Reqtags[ 6].ti_Data
#define Font_Reqtags_InitialSize			Font_Reqtags[ 7].ti_Data

#define ScrMd_Reqtags_TitleText				ScrMd_Reqtags[ 0].ti_Data
#define ScrMd_Reqtags_InitialLeftEdge		ScrMd_Reqtags[ 2].ti_Data
#define ScrMd_Reqtags_InitialHeight			ScrMd_Reqtags[ 5].ti_Data
#define ScrMd_Reqtags_InitialDisplayID		ScrMd_Reqtags[ 6].ti_Data
#define ScrMd_Reqtags_InitialDisplayWidth	ScrMd_Reqtags[ 7].ti_Data
#define ScrMd_Reqtags_InitialDisplayHeight	ScrMd_Reqtags[ 8].ti_Data
#define ScrMd_Reqtags_InitialDisplayDepth	ScrMd_Reqtags[ 9].ti_Data
#define ScrMd_Reqtags_InitialOverscanType	ScrMd_Reqtags[10].ti_Data
#define ScrMd_Reqtags_InitialAutoScroll		ScrMd_Reqtags[11].ti_Data

#define Main_Scrtags_Width					Main_Scrtags[ 2].ti_Data
#define Main_Scrtags_Height					Main_Scrtags[ 3].ti_Data
#define Main_Scrtags_Depth					Main_Scrtags[ 4].ti_Data
#define Main_Scrtags_Colors32				Main_Scrtags[ 6].ti_Data
#define Main_Scrtags_DisplayID				Main_Scrtags[ 9].ti_Data
#define Main_Scrtags_Pens					Main_Scrtags[10].ti_Data
#define Main_Scrtags_AutoScroll				Main_Scrtags[11].ti_Data
#define Main_Scrtags_Overscan				Main_Scrtags[12].ti_Data

#define BackDrop_Wintags_Left				BackDrop_Wintags[0].ti_Data
#define BackDrop_Wintags_Top				BackDrop_Wintags[1].ti_Data
#define BackDrop_Wintags_Width				BackDrop_Wintags[2].ti_Data
#define BackDrop_Wintags_Height				BackDrop_Wintags[3].ti_Data
#define BackDrop_Wintags_ScreenTitle		BackDrop_Wintags[5].ti_Data
#define BackDrop_Wintags_CustomScreen		BackDrop_Wintags[6].ti_Data

#define StatusBar_Wintags_Top				StatusBar_Wintags[1].ti_Data
#define StatusBar_Wintags_Width				StatusBar_Wintags[2].ti_Data
#define StatusBar_Wintags_Height			StatusBar_Wintags[3].ti_Data
#define StatusBar_Wintags_ScreenTitle		StatusBar_Wintags[5].ti_Data
#define StatusBar_Wintags_CustomScreen		StatusBar_Wintags[6].ti_Data
#define StatusBar_Wintags_Gadgets			StatusBar_Wintags[7].ti_Data

#define ToolBar_Wintags_Top					ToolBar_Wintags[ 1].ti_Data
#define ToolBar_Wintags_Width				ToolBar_Wintags[ 2].ti_Data
#define ToolBar_Wintags_Height				ToolBar_Wintags[ 3].ti_Data
#define ToolBar_Wintags_ScreenTitle			ToolBar_Wintags[ 5].ti_Data
#define ToolBar_Wintags_CustomScreen		ToolBar_Wintags[ 6].ti_Data
#define ToolBar_Wintags_Gadgets				ToolBar_Wintags[ 7].ti_Data

#define DropDown_Wintags_Left				DropDown_Wintags[ 0].ti_Data
#define DropDown_Wintags_Top				DropDown_Wintags[ 1].ti_Data
#define DropDown_Wintags_Width				DropDown_Wintags[ 2].ti_Data
#define DropDown_Wintags_Height				DropDown_Wintags[ 3].ti_Data
#define DropDown_Wintags_ScreenTitle		DropDown_Wintags[ 5].ti_Data
#define DropDown_Wintags_CustomScreen		DropDown_Wintags[ 6].ti_Data
#define DropDown_Wintags_Gadgets			DropDown_Wintags[ 7].ti_Data

#define Dialog_Wintags_Left					Dialog_Wintags[ 0].ti_Data
#define Dialog_Wintags_Top					Dialog_Wintags[ 1].ti_Data
#define Dialog_Wintags_InnerWidth			Dialog_Wintags[ 2].ti_Data
#define Dialog_Wintags_InnerHeight			Dialog_Wintags[ 3].ti_Data
#define Dialog_Wintags_IDCMP				Dialog_Wintags[ 4].ti_Data
#define Dialog_Wintags_Gadgets				Dialog_Wintags[ 5].ti_Data
#define Dialog_Wintags_Title				Dialog_Wintags[ 6].ti_Data
#define Dialog_Wintags_ScreenTitle			Dialog_Wintags[ 7].ti_Data
#define Dialog_Wintags_CustomScreen			Dialog_Wintags[ 8].ti_Data
#define Dialog_Wintags_Flags				Dialog_Wintags[10].ti_Data

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_GUI_C

	//-- GUI Init, Done & ReOpen

	extern void GUI_Init(void);
	extern void GUI_Done(void);
	extern void GUI_ReOpen(void);

#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_GUI_C

	extern UBYTE writeprefs;		/* indicates that we have auto-adjusted prefs-settings, the users should be notified so that he can save the prefs */

	//-- asl requests

	extern UBYTE asl_prefs;
	extern struct FileRequester			*File_Request;
	extern struct FontRequester			*Font_Request;
	extern struct ScreenModeRequester	*ScrMd_Request;

	//-- window input blocker

	extern struct Requester				NullReq;

	//-- Fonts

	extern struct TextAttr				ScrFont,ScrFontB;
	extern struct TextAttr				SigFont;
	extern struct TextAttr				ScaFont;
	extern struct TextAttr				StatFont;

	//-- Menus

	extern struct NewMenu				SFXMenu[],FXMenu[];

	//-- Toolbars

	extern struct NewToolbar			SFXToolbar[];
	extern struct NewToolbar			SFXToolbarCut[],SFXToolbarCopy[],SFXToolbarPaste[],SFXToolbarErase[],SFXToolbarGrab[],SFXToolbarZero[],SFXToolbarOverwr[];

	extern struct Gadget				tbCut,tbCopy,tbPaste,tbErase,tbGrab,tbZero,tbOverwr;

	//-- Popup-Menus

	extern struct PopupMenu				*pmenSampleWin;

	//-- asl request tags

	extern struct TagItem				File_Reqtags[];
	extern struct TagItem				Font_Reqtags[];
	extern struct TagItem				ScrMd_Reqtags[];

	//-- screen & window tags

	extern struct TagItem				BackDrop_Wintags[];
	extern struct TagItem				StatusBar_Wintags[];
	extern struct TagItem				ToolBar_Wintags[],DropDown_Wintags[];
	extern struct TagItem				Dialog_Wintags[];
#endif

#endif /* SFX_GUI_H */

//-- eof ----------------------------------------------------------------------

