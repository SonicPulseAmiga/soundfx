/****h* SoundFX/SFX-MRU.h [4.1] *
*
*  NAME
*    SFX-MRU.h
*  COPYRIGHT
*    $VER: SFX-MRU.h 4.1 (11.09.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    MostRecentUsed files  - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    11.Sep.2000
*  MODIFICATION HISTORY
*    11.Sep.2000	V 4.1	most recent version
*    11.Sep.2000	V 4.1	initial version
*  NOTES
*
*******
*/

#ifndef SFX_MRU_H
#define SFX_MRU_H

//-- globals ------------------------------------------------------------------

#ifndef SFX_MRU_C
	extern char MRU_MenuStr[5][FILENAME_MAX];
#endif

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_MRU_C
	extern BOOL		MRU_Init(void);
	extern void		MRU_Done(void);
	extern void		MRU_Add(STRPTR fn);
	extern STRPTR	MRU_GetFileNode(UBYTE nr);
	extern STRPTR	MRU_GetFileName(UBYTE nr);
#endif

#endif /* SFX_MRU_H */

//-- eof ----------------------------------------------------------------------
