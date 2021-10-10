/****h* SoundFX/SFX-GUIKeys.h [4.2] *
*
*  NAME
*    SFX-GUIKeys.h
*  COPYRIGHT
*    $VER: SFX-GUIKeys.h 4.2 (21.01.02) by Stefan Kost 1998-2001
*  FUNCTION
*    gui keyboard routines - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    21.Jan.2002
*  MODIFICATION HISTORY
*    21.Jan.2002    V 4.2   most recent version
*    21.Jan.2002    V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef SFX_GUIKeys_H
#define SFX_GUIKeys_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_GUIKeys_C

	//-- global shortcut tests

	extern void Test_VKeys(UWORD icode,SInfo *entry);
	extern void Test_RKeys(UWORD icode,UWORD iqual,SInfo *entry);

#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_GUIKeys_C
#endif

#endif /* SFX_GUIKeys_H */

//-- eof ----------------------------------------------------------------------

