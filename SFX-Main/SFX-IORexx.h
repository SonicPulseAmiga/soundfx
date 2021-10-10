/****h* SoundFX/SFX-IORexx.h [4.2] *
*
*  NAME
*    SFX-IORexx.h
*  COPYRIGHT
*    $VER: SFX-IORexx.h 4.2 (25.07.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    ARexx port for SoundFX - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    18.Aug.1998
*  MODIFICATION HISTORY
*    25.Jul.2001	V 4.2	most recent version
*    02.Mar.1999	V 3.80	most recent version
*    18.Aug.1998	V 3.70	most recent version
*    18.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_IORexx_H
#define SFX_IORexx_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_IORexx_C
	extern void DoIO_Rexx(void);

	//-- helper

	extern void GetSrcFromRexx(SInfo **src,char *params);
#endif

#endif /* SFX_IORexx_H */

//-- eof ----------------------------------------------------------------------
