/****h* sfxsupport.library/mem.h [4.2] *
*
*  NAME
*    mem.h
*  COPYRIGHT
*    $VER: mem.h 4.2 (04.06.02) © by Stefan Kost 1998-12002
*  FUNCTION
*    better memory allocation - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    04.Jun.2002	V 4.2	most recent version
*    22.Aug.2000	V 4.1	most recent version
*    18.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef MEM_H
#define MEM_H

//-- prototypes ---------------------------------------------------------------

#ifndef MEM_C
	//-- additional memory allocation
	APTR		ASM LIB_EXPORT(ReAllocVec(REG(a0,APTR addro),REG(d0,ULONG sizeo),REG(d1,ULONG sizen),REG(d2,ULONG typen)));

	//-- monitor memory usage
	void		ASM LIB_EXPORT(MemUsage(REG(a0,STRPTR str)));
	ULONG		AvailVMem(void);

#endif

#endif /* MEM_H */

//-- eof ----------------------------------------------------------------------
