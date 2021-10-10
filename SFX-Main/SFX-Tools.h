/****h*SoundFX/SFX-Tools.h [4.3] *
*
*  NAME
*    SFX-Tools.h
*  COPYRIGHT
*    $VER: SFX-Tools.h 4.3 (28.01.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    various routines - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    28.Jan.2004	V 4.3	most recent version
*    21.Feb.2003	V 4.2	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_Tools_H
#define SFX_Tools_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_Tools_C

	//-- sample tools

	extern void SetSampleLength(SInfo *si,ULONG newlen);
	extern void TemporaryHideAllSounds(void);
	extern void ShowAllTemporaryHiddenSounds(void);

	//-- key helper ;-)

	extern void KillAllTasks(void);

	//-- arexx helper

	extern void SendRexxCmd(STRPTR port,STRPTR cmd);

	//-- file-requester helper

	extern void SetInitialFileAndDrawer(STRPTR fn);
#endif

#endif /* SFX_Tools_H */

//-- eof ----------------------------------------------------------------------
