/****h* sfxsupport.library/savt.h [4.3] *
*
*  NAME
*    savt.h
*  COPYRIGHT
*    $VER: savt.h 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Helper for Savers - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Sep.1998
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    27.Apr.2002	V 4.2	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    19.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef SAVT_H
#define SAVT_H

//-- prototypes ---------------------------------------------------------------

#ifndef SAVT_C
	//-- saver tools

	extern ULONG  ASM LIB_EXPORT(GetCrunchedLength(REG(d0,UBYTE crunch),REG(d1,ULONG src_bytes)));
	extern UBYTE  ASM LIB_EXPORT(WriteData(REG(a0,SInfo *si),REG(a1,SaveData *sdata),REG(d0,UBYTE crunch),REG(d1,UBYTE conv),REG(d2,UBYTE channel)));
	extern void   ASM LIB_EXPORT(SetSampleComment(REG(a0,SInfo *si),REG(a1,STRPTR fn),REG(a2,STRPTR saver),REG(a3,STRPTR fmt)));
#endif

#endif /* LOAT_H */

//-- eof ----------------------------------------------------------------------
