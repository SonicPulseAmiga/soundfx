/****h*SoundFX/SFX-Prefs.h [4.2] *
*
*  NAME
*    SFX-Prefs.h
*  COPYRIGHT
*    $VER: SFX-Prefs.h 4.2 (24.07.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    preferences io - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    24.Jul.2001	V 4.2	most recent version
*    11.Jan.2001	V 4.1	most recent version
*    24.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_Prefs_H
#define SFX_Prefs_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_Prefs_C

	//-- io routines

	extern void ReadPrefs(char *fn);
	extern void WritePrefs(char *fn);

	//-- defaults

	extern void SetDefPOptions(int nr);
#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_Prefs_C

	//-- Preferences-Strukturen

	extern SFXPrefs_Gui			sfxprefs_gui;
	extern SFXPrefs_Misc		sfxprefs_misc;
	extern SFXPrefs_SBuf		sfxprefs_sbuf;
	extern SFXPrefs_VMem		sfxprefs_vmem;
	extern SFXPrefs_Win			sfxprefs_win;
#endif

#endif /* SFX_Prefs_H */

//-- eof ----------------------------------------------------------------------
