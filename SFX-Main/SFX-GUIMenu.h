/****h* SoundFX/SFX-GUIMenu.h [4.3] *
*
*  NAME
*    SFX-GUIMenu.h
*  COPYRIGHT
*    $VER: SFX-GUIMenu.h 4.3 (13.10.03) by Stefan Kost 1998-2003
*  FUNCTION
*    gui menu routines - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    21.Jan.2002
*  MODIFICATION HISTORY
*    13.Oct.2003	V 4.3	most recent version
*    10.Sep.2002    V 4.2   most recent version
*    21.Jan.2002    V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef SFX_GUIMenu_H
#define SFX_GUIMenu_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_GUIMenu_C

	//-- global Menu handling

	extern void Test_Menu(ULONG mitem,UWORD iqual);
	extern void Refr_GlobalMenu(void);
	extern void Refr_GlobalMRUMenu(void);

	//-- helper

	extern void Uncheck_MenuItem(ULONG itemnr);
	extern void Check_MenuItem(ULONG itemnr);

	extern void Disable_MenuItem(ULONG itemnr);
	extern void Enable_MenuItem(ULONG itemnr);

#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_GUIMenu_C
#endif

#endif /* SFX_GUIMenu_H */

//-- eof ----------------------------------------------------------------------

