/****h* SoundFX/SFX-WinStatus.h [4.00] *
*
*  NAME
*    SFX-WinStatus.h
*  COPYRIGHT
*    $VER: SFX-WinStatus.h 4.00 (15.02.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    Statusleiste for SFX Pubscreen - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    26.Jan.2000
*  MODIFICATION HISTORY
*    15.Feb.2000	V 4.00	most recent version
*    26.Jan.2000	V 3.82	most recent version
*    26.Jan.2000	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_WinStatus_H
#define SFX_WinStatus_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_WinStatus_C
	extern UBYTE Open_StatusWin(void);
	extern void Test_StatusWin (ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr);

	extern void Refr_StatusRange(void);
#endif

#endif /* SFX_WinStatus_H */

//-- eof ----------------------------------------------------------------------
