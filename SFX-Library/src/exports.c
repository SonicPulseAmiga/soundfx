#include <sys/types.h>
#include <stdio.h>
#include <exec/types.h>
#include <libraries/sfxsupport.h>

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" { /* this is for C++ only */
#endif

/* public functions */

extern	void	AssignBShape(APTR mod);

extern void		AssignInterpolFunc(UBYTE typ,APTR InterpolFunc);

extern void		Transform(double *fftbuf_r,double *fftbuf_i,UWORD *BitRevTab,double *SinTab,double *CosTab,BYTE dir,UWORD m);
extern UBYTE	NewFFTTabs(UWORD **bitrevtab,double **sintab1,double **sintab2,double **costab,UWORD m);
extern void		DelFFTTabs(UWORD *bitrevtab,double *sintab1,double *sintab2,double *costab);
extern double *	NewWindowFkt(UWORD len,UBYTE typ,double *par);
extern void		DelWindowFkt(double *buf);

extern SAMPLE	*NewRingBuf(ULONG minlen,SAMPLE **baseptr,ULONG *addrmask,ULONG *size);
extern void		DelRingBuf(APTR rbuf);

/* public function table */

typedef void (*F)();
static F texporttab[] = { 
    (F)AssignBShape,
	
    (F)AssignInterpolFunc,
	
	(F)Transform,
	(F)NewFFTTabs,
	(F)DelFFTTabs,
	(F)NewWindowFkt,
	(F)DelWindowFkt,
	
	(F)NewRingBuf,
	(F)DelRingBuf,
    NULL
};

#if defined(__cplusplus) || defined(c_plusplus)
} /* close scope of extern "C" declaration  */
#endif

