/****h* SoundFX/SFX-Edit.h [4.2] *
*
*  NAME
*    SFX-Edit.h
*  COPYRIGHT
*    $VER: SFX-Edit.h 4.2 (05.02.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    edit functions - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    20.Jun.1998
*  MODIFICATION HISTORY
*    05.Feb.2002	V 4.2	most recent version
*    23.Sep.1998	V 3.71	most recent version
*    18.Aug.1998	V 3.70	most recent version
*    20.Jun.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_Edit_H
#define SFX_Edit_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_Edit_C

	//-- Helper

	extern void InitCopyBuffer(void);
	extern void FreeCopyBuffer(void);
//	extern void CorrectLoop(SInfo *entry,UBYTE op);

	//-- Editfuntions
		
	extern void Edit_Cut(SInfo *entry,UBYTE mode);
	extern void Edit_Copy(SInfo *entry,UBYTE mode);
	extern void Edit_Grab(SInfo *entry,UBYTE mode);
	extern void Edit_Paste(SInfo *entry,UBYTE mode);
	extern void Edit_Erase(SInfo *entry,UBYTE mode);
	extern void Edit_Zero(SInfo *entry,UBYTE mode);
	extern void Edit_Overwrite(SInfo *entry,UBYTE mode);
	extern void Edit_Exchange(SInfo *entry);
	extern void Edit_Replace(SInfo *entry);
#endif

#endif /* SFX_Edit_H */

//-- eof ----------------------------------------------------------------------

