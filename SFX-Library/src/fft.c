/****h* sfxsupport.library/fft.c [4.2] *
*
*  NAME
*    fft.c
*  COPYRIGHT
*    $VER: fft.c 4.2 (04.06.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    FastFourierTransformation - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    26.Nov.1995
*  MODIFICATION HISTORY
*    04.Jun.2002	V 4.2	most recent version
*							A: NewFFTTabs, DelFFTTabs instead of InitFFT
*							A: NewWindowFkt, DelWindowFkt instead of GenWindow
*    11.Mar.2000	V 1.30	most recent version
*							C:unrolled two loops in Transform(...)
*    18.Sep.1998	V 1.20	most recent version
*    26.Nov.1995	V 1.00	initial version
*  NOTES
*
*******
*/

#define FFT_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- FFT-Routines

void		ASM LIB_EXPORT(Transform(REG(a0,double *fftbuf_r),REG(a1,double *fftbuf_i),REG(a2,UWORD *BitRevTab),REG(a3,double *SinTab),REG(a4,double *CosTab),REG(d0,BYTE dir),REG(d1,UWORD m)));
UBYTE		ASM LIB_EXPORT(NewFFTTabs(REG(a0,UWORD **bitrevtab),REG(a1,double **sintab1),REG(a2,double **sintab2),REG(a3,double **costab),REG(d0,UWORD m)));
void		ASM LIB_EXPORT(DelFFTTabs(REG(a0,UWORD *bitrevtab),REG(a1,double *sintab1),REG(a2,double *sintab2),REG(a3,double *costab)));

//-- Windows for FFT and Filters

double *	ASM LIB_EXPORT(NewWindowFkt(REG(d0,UWORD len),REG(d1,UBYTE typ),REG(a0,double *par)));
void		ASM LIB_EXPORT(DelWindowFkt(REG(a0,double *buf)));

//-- private Stuff

UWORD		INLINE BitRev(UWORD in,UWORD nu);
double		INLINE KaiserI(double x);

//-- definitions --------------------------------------------------------------

//-- FFT-Routines

void SAVEDS ASM LIB_EXPORT(Transform(REG(a0,double *fftbuf_r),REG(a1,double *fftbuf_i),REG(a2,UWORD *BitRevTab),REG(a3,double *SinTab),REG(a4,double *CosTab),REG(d0,BYTE dir),REG(d1,UWORD m))) {
	register UBYTE  l;
	register UWORD j,k;
	UWORD *brtptr;
	double *stptr,*ctptr;
	UWORD i1,i2,i3,i4;
	UWORD mm=m<<1;
	double mmd=(double)mm;
	register WORD p,nu=0,nu1=m;
	double tmp_r,tmp_i;
	double *ibuf_r,*ibuf_i,*kbuf_r,*kbuf_i;

	while(nu1) { nu1>>=1;nu++; }		/* nu=log2(m)+1; */

	i1=m;
	i2=1;
	for(l=0;l<nu;l++) {	// split levels 0..log2(m)
		i3=0;i4=i1;
		stptr=SinTab;
		ctptr=CosTab;
		for(j=0;j<i2;j++) {	// 0..1,0..2,0..4,0..8,....
			ibuf_r=&fftbuf_r[i3+i1];ibuf_i=&fftbuf_i[i3+i1];
			kbuf_r=&fftbuf_r[i3];kbuf_i=&fftbuf_i[i3];
			for(k=i3;k<i4;k++) {
				tmp_r=(*ctptr)*(*ibuf_r)-(*stptr)*(*ibuf_i);
				tmp_i=(*ctptr)*(*ibuf_i)+(*stptr)*(*ibuf_r);
				*(ibuf_r++)=(*kbuf_r)-tmp_r;
				*(ibuf_i++)=(*kbuf_i)-tmp_i;
				*(kbuf_r++)+=tmp_r;
				*(kbuf_i++)+=tmp_i;
			}
			i3+=(i1<<1);
			i4+=(i1<<1);
			ctptr++;stptr++;
		}
		i1>>=1;
		i2<<=1;
	}

	kbuf_r=fftbuf_r;kbuf_i=fftbuf_i;brtptr=BitRevTab;
	k=0;
	while(k<mm) {
		if((p=*(brtptr++))>k) {
			tmp_r=*kbuf_r;*kbuf_r=fftbuf_r[p];fftbuf_r[p]=tmp_r;			/* swap real */
			tmp_i=*kbuf_i;*kbuf_i=fftbuf_i[p];fftbuf_i[p]=tmp_i;			/* swap imag */
		}
		kbuf_r++;kbuf_i++;k++;
		if((p=*(brtptr++))>k) {
			tmp_r=*kbuf_r;*kbuf_r=fftbuf_r[p];fftbuf_r[p]=tmp_r;			/* swap real */
			tmp_i=*kbuf_i;*kbuf_i=fftbuf_i[p];fftbuf_i[p]=tmp_i;			/* swap imag */
		}
		kbuf_r++;kbuf_i++;k++;
	}

	if(dir==-1) {							// Zeitbereich -> Frequenzbereich
		kbuf_r=fftbuf_r;kbuf_i=fftbuf_i;
		for(k=0;k<m;k++) {
			*(kbuf_r++)/=mmd;*(kbuf_i++)/=mmd;
			*(kbuf_r++)/=mmd;*(kbuf_i++)/=mmd;
		}
	}
}

UBYTE SAVEDS ASM LIB_EXPORT(NewFFTTabs(REG(a0,UWORD **bitrevtab),REG(a1,double **sintab1),REG(a2,double **sintab2),REG(a3,double **costab),REG(d0,UWORD m))) {
	register UWORD i;
	UWORD mm=m<<1;
	double mmd=(double)mm;
	UWORD nu=0,nu1=m;
	double pi2f=6.283185307/mmd,arc;
	UWORD *brtptr;
	double *s1tptr,*s2tptr,*ctptr;

	if(	(*bitrevtab=(UWORD  *)AllocVec(mm*sizeof(UWORD),MEMF_ANY))
	&&	(*sintab1  =(double *)AllocVec(m*sizeof(double),MEMF_ANY))
	&&	((sintab2
		&& (*sintab2  =(double *)AllocVec(m*sizeof(double),MEMF_ANY)))
		|| !sintab2)
	&&	(*costab   =(double *)AllocVec(m*sizeof(double),MEMF_ANY))) {

		while(nu1) { nu1>>=1;nu++; }		/* nu=log2(mm); */

		brtptr=*bitrevtab;
		for(i=0;i<mm;i++) *(brtptr++)=BitRev(i,nu);				// bit-reversal table
		brtptr=*bitrevtab;

		ctptr=*costab;
		s1tptr=*sintab1;
		if(sintab2) {
			s2tptr=*sintab2;
			for(i=0;i<m;i++) {
				arc=pi2f*(double)brtptr[i<<1];
				*(ctptr++) =cos(arc);
				*s2tptr    =sin(arc);
				*(s1tptr++)=-(*(s2tptr++));
			}
		}
		else {
			for(i=0;i<m;i++) {
				arc=pi2f*(double)brtptr[i<<1];
				*(ctptr++) = cos(arc);
				*(s1tptr++)=-sin(arc);
			}
		}
		return(TRUE);
	}
	return(FALSE);
}

void SAVEDS ASM LIB_EXPORT(DelFFTTabs(REG(a0,UWORD *bitrevtab),REG(a1,double *sintab1),REG(a2,double *sintab2),REG(a3,double *costab))) {
	if(bitrevtab)	FreeVec((ULONG *)bitrevtab);
	if(sintab1)		FreeVec((ULONG *)sintab1);
	if(sintab2)		FreeVec((ULONG *)sintab2);
	if(costab)		FreeVec((ULONG *)costab);
}

//-- Windows for FFT and Filters

double * SAVEDS ASM LIB_EXPORT(NewWindowFkt(REG(d0,UWORD len),REG(d1,UBYTE typ),REG(a0,double *par))) {
	register UWORD i,len2=len>>1,len3=len-1;
	double l=(double)len;
	double pih=3.141592654/l,pih2=2.0*pih,pih4=4.0*pih;
	double ak,t,p;
	double *buf;

	if((buf=(double *)AllocVec(len*sizeof(double),MEMF_ANY))) {
		switch(len) {
			case 1:
				buf[0]=1.0;
				break;
			case 2:
				buf[0]=0.5;
				buf[1]=0.5;
				break;
			default:
				switch(typ) {
					case WIN_RECTANGLE:
						for(i=0;i<len;i++) buf[i]=1.0;
						break;
					case WIN_BARTLETT:
						for(i=0;i<len2;i++) buf[len3-i]=buf[i]=((double)(i<<1))/l;
						break;
					case WIN_FEJER:
						for(i=0;i<len2;i++) buf[len3-i]=buf[i]=0.5+(double)i/l;
						break;
					case WIN_WELCH:
						t=len2*len2;
						for(i=0;i<len2;i++) buf[len2+i]=buf[len2-i]=1.0-((double)(i*i)/t);
						break;
					case WIN_HANNING:
						for(i=0;i<len2;i++) buf[len3-i]=buf[i]=0.5-0.5*cos(pih2*(double)i);
						break;
					case WIN_HAMMING:
						for(i=0;i<len2;i++) buf[len3-i]=buf[i]=0.54-0.46*cos(pih2*(double)i);
						break;
					case WIN_BLACKMAN:
						for(i=0;i<len2;i++) buf[len3-i]=buf[i]=0.42-0.5*cos(pih2*(double)i)+0.08*cos(pih4*(double)i);
						break;
					case WIN_KAISER:
						ak=KaiserI(*par);p=(*par)+(*par);
						for(i=0;i<len2;i++) {
							t=(double)i/l;
							buf[len3-i]=buf[i]=KaiserI(p*sqrt(t-(t*t)))/ak;
						}
						break;
					case WIN_HALFSINE:
						for(i=0;i<len2;i++) buf[len3-i]=buf[i]=sin(pih*(double)i);
						break;
					case WIN_HALFSINEQ:
						for(i=0;i<len2;i++) buf[len3-i]=buf[i]=(t=sin(pih*(double)i))*t;
						break;
					case WIN_HALFSINES:
						for(i=0;i<len2;i++) buf[len3-i]=buf[i]=sqrt(sin(pih*(double)i));
						break;
				}
				break;
		}
	}
	return(buf);
}

void SAVEDS ASM LIB_EXPORT(DelWindowFkt(REG(a0,double *buf))) {
	FreeVec((ULONG *)buf);
}


//-- private Stuff

UWORD INLINE BitRev(UWORD in,UWORD nu) {			// mirros words bitewise
	register UBYTE h,h2;
	register UWORD out=0;

	for(h=0,h2=(nu-1);h<nu;h++,h2--) {
		out|=((in&(1<<h))>>h)<<h2;
	}
	return(out);
}

double INLINE KaiserI(double x) {					// Besselfunction for Kaiserwindows
	register UBYTE v;
	double vf[11]={1.0,2.0,6.0,24.0,120.0,720.0,5040.0,40320.0,362880.0,3628800.0,39916800.0 };
	double sum=0.0,x2=x/2.0,x2i=x2;

	for(v=0;v<11;v++) {		/* normalerweise v=1...inf. */
		sum+=(x2/vf[v]);
		x2*=x2i;
	}
	return(sum);
}

//-- eof ----------------------------------------------------------------------
