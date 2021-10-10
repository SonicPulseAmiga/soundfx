/****h*SoundFX/SFX-ModSaver.h [4.1] *
*
*  NAME
*    SFX-ModSaver.h
*  COPYRIGHT
*    $VER: SFX-ModSaver.h 4.1 (02.09.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    sample saver interface - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    02.Sep.2000    V 4.1   most recent version
*    18.Aug.1998    V 3.70  most recent version
*    14.Aug.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#ifndef SFX_ModSaver_H
#define SFX_ModSaver_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_ModSaver_C
	extern void SaveSampleInterface(void);
	extern void SaveSample(void);
	extern UBYTE SaveSampleRexx(STRPTR fn);

	//-- helper for arexx cmds
	extern void HandleSaverCmd(STRPTR params,STRPTR ret);
#endif

#endif /* SFX_ModSaver_H */

//-- eof ----------------------------------------------------------------------
