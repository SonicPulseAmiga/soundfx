/****h* sfxsupport.library/rbuf.h [1.20] *
*
*  NAME
*    rbuf.h
*  COPYRIGHT
*    $VER: rbuf.h 1.20 (19.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    RingBuffer routines - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Sep.1998
*  MODIFICATION HISTORY
*    19.Sep.1998	V 1.20	most recent version
*    19.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef RBUF_H
#define RBUF_H

//-- prototypes ---------------------------------------------------------------

#ifndef RBUF_C
	//-- RingBuffer

	extern SAMPLE ASM *LIB_EXPORT(NewRingBuf(REG(d0,ULONG minlen),REG(a0,SAMPLE **baseptr),REG(a1,ULONG *addrmask),REG(a2,ULONG *size)));
	extern void   ASM  LIB_EXPORT(DelRingBuf(REG(a0,SAMPLE *rbuf)));
#endif

#endif /* RBUF_H */

//-- eof ----------------------------------------------------------------------
