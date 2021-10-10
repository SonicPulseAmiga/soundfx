/****h* SoundFX/SFX-WinBackdrop.h [4.2] *
*
*  NAME
*    SFX-WinBackdrop.h
*  COPYRIGHT
*    $VER: SFX-WinBackdrop.h 4.2 (15.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Backdropwindow for SFX Pubscreen - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    11.Aug.1998
*  MODIFICATION HISTORY
*    15.May.2002	V 4.2	most recent version
*    18.Aug.1998	V 3.70	most recent version
*    11.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_WinBackdrop_H
#define SFX_WinBackdrop_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_WinBackdrop_C
	extern UBYTE Open_BDWin(void);
	extern void Test_BDWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr);
#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_WinBackdrop_C
	extern struct Image backlogo;
#endif

#endif /* SFX_WinBackdrop_H */

//-- eof ----------------------------------------------------------------------
