/****h*SoundFX/SFX-Pointers.h [4.00] *
*
*  NAME
*    SFX-Pointers.h
*  COPYRIGHT
*    $VER: SFX-Pointers.h 4.00 (15.02.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    additional Mousepointers for SoundFX - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    13.Sep.1998
*  MODIFICATION HISTORY
*    15.Feb.2000	V 4.00	most recent version
*    13.Sep.1998	V 3.71	most recent version
*    13.Sep.1998	V 3.71	initial version
*  NOTES
*
*******
*/

#ifndef SFX_Pointers_H
#define SFX_Pointers_H

//-- defines ------------------------------------------------------------------

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_Pointer_C
	extern MPointer *ReadPointerImage(const STRPTR ptrname);
	extern void		 FreePointerImage(const MPointer *mptr);
#endif

#endif /* SFX_Pointers_H */

//-- eof ----------------------------------------------------------------------
