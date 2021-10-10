/****h* sfxsupport.library/loat.h [1.20] *
*
*  NAME
*    loat.h
*  COPYRIGHT
*    $VER: loat.h 1.20 (19.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    Helper for Loaders - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    19.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef LOAT_H
#define LOAT_H

//-- prototypes ---------------------------------------------------------------

#ifndef LOAT_C
	//-- loader tools

	ULONG ASM LIB_EXPORT(GetDecrunchedLength(REG(d0,UBYTE crunch),REG(d1,ULONG src_bytes)));
	ULONG ASM LIB_EXPORT(ReadData(REG(a0,SInfo *si),REG(a1,LoadData *ldata),REG(d0,ULONG srclen),REG(d1,UBYTE crunch),REG(d2,UBYTE conv),REG(d3,UBYTE channel)));

	ULONG ASM LIB_EXPORT(GetFileLength(REG(a0,STRPTR fn)));
#endif

#endif /* LOAT_H */

//-- eof ----------------------------------------------------------------------
