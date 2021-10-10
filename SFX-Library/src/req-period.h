/****h* sfxsupport.library/req-period.h [1.20] *
*
*  NAME
*    req-period.h
*  COPYRIGHT
*    $VER: req-period.h 1.20 (19.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    get period requester - declarations
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

#ifndef REQ_PERIOD_H
#define REQ_PERIOD_H

//-- prototypes ---------------------------------------------------------------

#ifndef REQ_PERIOD_C
	extern ULONG		ASM LIB_EXPORT(GetPeriode(REG(d0,ULONG srat),REG(a0,SInfo *si)));
	extern UBYTE		ASM LIB_EXPORT(Rate2KeyFrq(REG(d0,ULONG srat),REG(a0,STRPTR  key ),REG(a1,double *frq)));
	extern UBYTE		ASM LIB_EXPORT(Key2RateFrq(REG(a0,STRPTR  key),REG(a1,ULONG *srat),REG(a2,double *frq)));
	extern UBYTE		ASM LIB_EXPORT(Frq2RateKey(REG(a0,double *frq),REG(a1,ULONG *srat),REG(a2,STRPTR  key)));
#endif

#endif /* REQ_PERIOD_H */

//-- eof ----------------------------------------------------------------------
