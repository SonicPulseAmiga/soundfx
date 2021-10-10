/****h* SoundFX/SFX-WinToolbar.h [4.2] *
*
*  NAME
*    SFX-WinToolbar.h
*  COPYRIGHT
*    $VER: SFX-WinToolbar.h 4.2 (29.10.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Toolbarleiste for SFX Pubscreen - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    26.Jan.2000
*  MODIFICATION HISTORY
*    29.Oct.2002	V 4.2	most recent version
*    13.Feb.2000	V 4.00	most recent version
*    30.Jan.2000	V 3.82	most recent version
*    26.Jan.2000	V 3.82	initial version
*  NOTES
*
*******
*/

#ifndef SFX_WinToolbar_H
#define SFX_WinToolbar_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_WinToolbar_C
	extern UBYTE Open_ToolbarWin(void);
	extern void Test_ToolbarWin (ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr);
	extern void Test_DropDownWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr,BOOL *closedd);

	extern void Refr_ToolbarLoader(void);
	extern void Refr_ToolbarSaver(void);
	extern void Refr_ToolbarOperator(void);
	extern void Refr_ToolbarRexx(void);
	extern void Refr_ToolbarPlayer(void);
	extern void Refr_ToolbarBuffer(void);
	extern void Refr_ToolbarEdit(void);
	extern void Refr_ToolbarZoom(void);
	extern void Refr_ToolbarRange(void);
	extern void Refr_ToolbarWindows(void);

	extern struct Gadget *CreateToolbars(struct NewToolbar *ntb,ULONG maxwidth,ULONG maxheight);
	extern void DrawToolbars(struct NewToolbar *ntb,struct RastPort *rp);
	extern void FreeToolbars(struct NewToolbar *ntb);
#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_WinToolbar_C
	extern struct Gadget *gadToolbar,*gadDropDown;
#endif

#endif /* SFX_WinToolbar_H */

//-- eof ----------------------------------------------------------------------
