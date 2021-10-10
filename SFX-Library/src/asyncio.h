/****h* sfxsupport.library/asyncio.h [4.2] *
*
*  NAME
*    asyncio.h
*  COPYRIGHT
*    $VER: asyncio.h 4.2 (27.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    asynchronous disk IO - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    27.Apr.2002	V 4.2	most recent version
*    18.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef ASYNCIO_H
#define ASYNCIO_H

//-- prototypes ---------------------------------------------------------------

#ifndef ASYNCIO_C
	//-- asynchronous disk io

	extern UBYTE	ASM LIB_EXPORT(ASOpen (REG(a0,SmpAccess *sa),REG(a1,STRPTR fn),REG(d0,int mode)));
	extern void		ASM LIB_EXPORT(ASClose(REG(a0,SmpAccess *sa)));

	extern UBYTE	ASM LIB_EXPORT(ASRead	(REG(a0,SmpAccess *sa),REG(d0,ULONG length)));
	extern UBYTE	ASM LIB_EXPORT(ASReadIRQ(REG(a0,SmpAccess *sa),REG(d0,ULONG length)));
	extern UBYTE	ASM LIB_EXPORT(ASWrite	(REG(a0,SmpAccess *sa),REG(d0,ULONG length)));
#endif

#endif /* ASYNCIO_H */

//-- eof ----------------------------------------------------------------------
