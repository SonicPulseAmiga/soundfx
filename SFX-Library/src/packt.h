/****h* sfxsupport.library/packt.h [1.30] *
*
*  NAME
*    packt.h
*  COPYRIGHT
*    $VER: packt.h 1.30 (18.06.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    packertools for loader & savers - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    18.Jun.2000	V 1.30	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef PACKT_H
#define PACKT_H

//-- prototypes ---------------------------------------------------------------

#ifndef PACKT_C
	extern void ASM LIB_EXPORT(GenIOTables(void));
#endif

//-- globals ------------------------------------------------------------------

#ifndef PACKT_C
	extern BYTE fibtab4[16];
	extern LONG fibtab6[64];
	extern BYTE exptab4[16];
	extern LONG exptab5[32];
	extern WORD ulawtab[8];
	extern WORD gaiP4tab[];
#endif

#endif /* PACKT_H */

//-- eof ----------------------------------------------------------------------
