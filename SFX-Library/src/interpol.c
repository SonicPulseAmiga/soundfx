/****h* sfxsupport.library/interpol.c [1.30] *
*
*  NAME
*    interpol.c
*  COPYRIGHT
*    $VER: interpol.c 1.30 (18.03.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    Interpolation routines - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    20.Jun.1998
*  MODIFICATION HISTORY
*    18.Mar.2000	V 1.30	most recent version
*    06.Nov.1998	V 1.20	most recent version
*    20.Jun.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define INTERPOL_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Interpolator Assigns and Initialisations

void ASM LIB_EXPORT(AssignInterpolFunc(REG(d0,UBYTE typ),REG(a0,InterpolFuncPtr *InterpolFunc)));

//-- Interpolators

double ASM Interpol_None      (REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range));
double ASM Interpol_Lin       (REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range));
double ASM Interpol_Si        (REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range));
double ASM Interpol_Lagrange  (REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range));

//-- definitions --------------------------------------------------------------

//-- Interpolator Assigns and Initialisations

void SAVEDS ASM LIB_EXPORT(AssignInterpolFunc(REG(d0,UBYTE typ),REG(a0,InterpolFuncPtr *InterpolFunc))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(InterpolFunc,__FUNC__,"1.par",4));

	switch(typ) {
		case INT_TYPE_NONE:		*InterpolFunc=Interpol_None;break;
		case INT_TYPE_LIN:		*InterpolFunc=Interpol_Lin;break;
		case INT_TYPE_SI:		*InterpolFunc=Interpol_Si;break;
		case INT_TYPE_LAGRANGE:	*InterpolFunc=Interpol_Lagrange;break;
		default:				*InterpolFunc=Interpol_None;break;
	}
}

//-- Interpolators

double ALIGNED ASM Interpol_None(REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range)) {
	return((double)(*(SAMPLE *)((ULONG)data|((Samples2Bytes((ULONG)*pos  ))&len))));
}

double ALIGNED ASM Interpol_Lin(REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range)) {
	double ant1,ant2;
	register SAMPLE *pos1,*pos2;

	ant1=1.0-(ant2=(*pos)-(double)((ULONG)(*pos)));
//	ant2=*pos;
//	ant2-=floor(ant2);
//	ant1=1.0-ant2;
	pos1=(SAMPLE *)((ULONG)data|((Samples2Bytes((ULONG)*pos  ))&len));
	pos2=(SAMPLE *)((ULONG)data|((Samples2Bytes((ULONG)*pos+1))&len));
	return((((double)*pos1)*ant1+((double)*pos2)*ant2));
}

double ALIGNED ASM Interpol_Si(REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range)) {
	register UBYTE i;
	register double sum,tmp;
	register SAMPLE *pos1;

//	if((*pos)!=(ULONG)(*pos)) {
		pos1=(SAMPLE *)((ULONG)data|((Samples2Bytes((ULONG)*pos+(Bytes2Samples(len+1)-(range>>1))))&len));
		tmp=PI*(*pos-(double)((LONG)*pos-(LONG)(range>>1)));
		for(sum=0.0,i=0;i<range;i++) {
			sum+=(*pos1*__sin(tmp)/tmp);
			pos1=(SAMPLE *)((ULONG)data|((ULONG)(pos1+1)&len));
			tmp-=PI;
			// DEBUG
			if(fabs(tmp)<0.00001) tmp=0.00001;
			// DEBUG
		}
		CLIPFLT(sum);
		return(sum);
//	}
//	else return((double)(*(SAMPLE *)((ULONG)data|Samples2Bytes((ULONG)*pos))));
}

double ALIGNED ASM Interpol_Lagrange(REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range)) {
	register UBYTE i,j;
	register double sum,tmp,val;
	register SAMPLE *pos1;

//	if((*pos)!=(ULONG)(*pos)) {											// if we're not at an integer position
		pos1=(SAMPLE *)((ULONG)data|((Samples2Bytes((ULONG)*pos+(Bytes2Samples(len+1)-(range>>1))))&len));
		tmp=*pos-(double)((LONG)*pos-(LONG)(range>>1));					// Startindex
		for(sum=0.0,i=0;i<range;i++) {									// für <n> Schritte
			val=(double)*pos1;
			for(j=0;j<range;j++) {											// für <n> Schritte
				if(i!=j) val*=(tmp-(double)j)/(double)((BYTE)i-(BYTE)j);
			}
			sum+=val;
			pos1=(SAMPLE *)((ULONG)data|((ULONG)(pos1+1)&len));
 		}
		CLIPFLT(sum);
		return(sum);
//	}
//	else return((double)(*(SAMPLE *)((ULONG)data|Samples2Bytes((ULONG)*pos))));
}

//-- eof ----------------------------------------------------------------------
