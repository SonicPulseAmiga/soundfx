/****h* sfxsupport.library/fft.h [1.20] *
*
*  NAME
*    fft.h
*  COPYRIGHT
*    $VER: fft.h 1.20 (18.09.98) © by Stefan Kost 1998-11998
*  FUNCTION
*    FastFourierTransformation - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    18.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef FFT_H
#define FFT_H

//-- prototypes ---------------------------------------------------------------

#ifndef FFT_C
	//-- FFT-Routines

	extern void		ASM LIB_EXPORT(Transform(REG(a0,double *fftbuf_r),REG(a1,double *fftbuf_i),REG(a2,UWORD *BitRevTab),REG(a3,double *SinTab),REG(a4,double *CosTab),REG(d0,BYTE dir),REG(d1,UWORD m)));
	extern UBYTE	ASM LIB_EXPORT(NewFFTTabs(REG(a0,UWORD **bitrevtab),REG(a1,double **sintab1),REG(a2,double **sintab2),REG(a3,double **costab),REG(d0,UWORD m)));
	extern void		ASM LIB_EXPORT(DelFFTTabs(REG(a0,UWORD *bitrevtab),REG(a1,double *sintab1),REG(a2,double *sintab2),REG(a3,double *costab)));

	//-- Windows for FFT and Filters

	extern double *	ASM LIB_EXPORT(NewWindowFkt(REG(d0,UWORD len),REG(d1,UBYTE typ),REG(a0,double *par)));
	extern void		ASM LIB_EXPORT(DelWindowFkt(REG(a0,double *buf)));
#endif

#endif /* FFT_H */

//-- eof ----------------------------------------------------------------------
