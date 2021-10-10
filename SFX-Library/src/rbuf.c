/****h* sfxsupport.library/rbuf.c [1.20] *
*
*  NAME
*    rbuf.c
*  COPYRIGHT
*    $VER: rbuf.c 1.20 (19.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    RingBuffer routines - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    15.Jun.1998
*  MODIFICATION HISTORY
*    19.Sep.1998	V 1.20	most recent version
*    15.Jun.1998	V 1.10	initial version
*  NOTES
*
*******
*/

#define RBUF_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- RingBuffer

SAMPLE ASM *LIB_EXPORT(NewRingBuf(REG(d0,ULONG minlen),REG(a0,SAMPLE **baseptr),REG(a1,ULONG *addrmask),REG(a2,ULONG *size)));
void   ASM  LIB_EXPORT(DelRingBuf(REG(a0,SAMPLE *rbuf)));

//-- definitions --------------------------------------------------------------

//-- RingBuffer

SAMPLE SAVEDS ASM *LIB_EXPORT(NewRingBuf(REG(d0,ULONG minlen),REG(a0,SAMPLE **baseptr),REG(a1,ULONG *addrmask),REG(a2,ULONG *size))) {
	UBYTE bits;
	ULONG mlen;
	SAMPLE *rbuf;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(baseptr,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(addrmask,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(size,__FUNC__,"3.par",4));

	mlen=Samples2Bytes(minlen);bits=0;				// Wieviele Bytes werden für minlen Samples gebraucht ?
	while(mlen) {mlen>>=1;bits++; }					// Wieviele Bits hat minlen ?
	*size=(1L<<bits);								// tatsächliche Länge des Ringbuffers
	*addrmask=*size-1;								// Adressmaske
	//MSG4("minlen %4ld  mlen %4ld bits %2ld  size %6ld",minlen,mlen,bits,*size);

	if((rbuf=(SAMPLE *)AllocVec((*size<<1),MEMF_PUBLIC)))		// muß sowieso vor jedem Durchlauf gelöscht werden
		*baseptr=(SAMPLE *)(((ULONG)rbuf+(*size))&(~(*addrmask)));	// Basisadresse
	return(rbuf);
}

void   SAVEDS ASM  LIB_EXPORT(DelRingBuf(REG(a0,SAMPLE *rbuf))) {
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(rbuf,__FUNC__,"1.par",4));

	FreeVec((APTR)rbuf);
}

//-- eof ----------------------------------------------------------------------
