/****h* Filter-FIRMatrix/Filter-FIRMatrix.c [4.2] *
*
*  NAME
*    Filter-FIRMatrix.c
*  COPYRIGHT
*    $VER: Filter-FIRMatrix.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-FIRMatrix core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/Filter-FIRMatrix *
*  Function.eng
*    Filters or boosts the signal via a convolution-matrix.
*  Funktion.ger
*    Filtert oder Boostet das Signal über eine Convolutionsmatrix.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome. Negative values produce the
*      opposite	effect - they boost frequencys.
*    Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      Area for averaging-calculations. The wider that range, the higher the
*      cut-off frequency gets (you've got to look at this relatively,as the
*      Matrix permits many different characteristics to be set).
*    Resonance (<a href="../node03.02.01.html">P3</a>)
*      Strength of resonance (also Peak or Q-Factor).
*      As a strong resonance thins out the signal, there is an amplification-factor
*      that runs parallel with the resonance, thus gets modulated too. A resonance
*      of 0.0 should have an Amp=100 %. Higher resonances should get higher
*      amplify values. You'll have to experiment to find the perfect values (try resonance+100 %).
*    Matrix
*      List of factors for use with the multiplications in the cross section.
*      Values shold not exceed 15.0.
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt. Negative Effektanteile bewirken keine Dämpfung,
*      sondern eine Verstärkung der zu bearbeitenden Frequenzen.
*    Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      Bereich für die Durchschnittsberechnung. Je breiter dieser ist, desto höher
*      ist die Cut-Off-Frequenz (ist relativ zu sehen, da sich über die Matrix
*      völlig unterschiedliche Charakteristika einstellen lassen).
*    Resonance (<a href="../node03.02.01.html">P3</a>)
*      Stärke der Resonanz (auch Peak oder Q-Faktor).
*      Da eine starke Resonanz das Signal ausdünnt, gibt es einen Amplifyparameter
*      der parallel zur Resonanz mitläuft,also auch moduliert wird.
*      Bei einer Resonanz von 0 sollte Amp=100 % sein. Bei einer höheren Resonanz
*      sollten größere Werte verwendet werden. Diese lassen sich allerdings nur
*      durch Probieren herausfinden (versuchen sie mal Resonance+100%).
*    Matrix
*      Eine Liste der Faktoren die für die Multiplikationen im Querschnittsbereich
*      benutzt werden. Die einzelnen Werte sollten nicht größer als 15.0 gewählt werden.
*  Notes.eng
*    A Matrix-<a href="../node06.03.00.Filter.html">filter</a>, such as this,
*    is a FIR-Filter where you can enter the coefficients yourself, e.g. if
*    you have designed them with an other program.<br>
*    With the matrix you can simulate various Filtercharacteristica. If you
*    want to get e.g. a highpass-filter, just set the first value to e.g. 5
*    and the next ones till cut-off to -1 (e.g. if cut-off=7, then the next
*    six values).
*  Hinweise.ger
*    Ein Matrix-<a href="../node06.03.00.Filter.html">Filter</a>, wie dieser,
*    ist ein FIR-Filter bei dem Sie die Koeffizienten selber eintragen können,
*    falls Sie diese z.B. mit einem andern Programm entworfen haben.<br>
*    Über die Matrix lassen sich die unterschiedlichsten Filtercharakteristika
*    simulieren. Wenn Sie z.B. einen Hochpassfilter simulieren möchten, setzen
*    Sie den ersten Wert auf z.B. 5 und die anderen Werte bis zur Cut-Off Grenze
*    auf -1 (also bei Cut-Off-Range=7, die nächsten 6 Werte).
*******
*/

#define FILTER_FIRMATRIX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Filter-FIRMatrix.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	if(!(instance->fbbuf=(WORD *)AllocVec((instance->mrng<<1),MEMF_ANY))) return(FALSE);
	if(!(instance->matrix=(double *)AllocVec((instance->mrng*sizeof(double)),MEMF_ANY))) return(FALSE);
	if(!(instance->rbuf=NewRingBuf(instance->delay,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	AssignBShape(&instance->mod3);
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	memset(instance->fbbuf,0,(instance->mrng<<1));
	instance->fbptr=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,k,curlen;
	double effs,effd,eff,dry;
	double rngs,rngd,rng;
	double fbs,fbd,fb;
	double amps,ampd,amp;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *rbufr,*rbufw,*rbufbase;
	ULONG rbufmask;
	ULONG mrng,hrng;
	LONG flen,val1;
	double sample,mod;
	SAMPLE *fbbuf;
	double *mat;
	LONG tfbptr;
	ULONG fbptr;
	
	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	rngs=instance->rngs;
	rngd=instance->rngd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	fbs=instance->fbs;
	fbd=instance->fbd;
	amps=instance->amps;
	ampd=instance->ampd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	fbptr=instance->fbptr;
	fbbuf=instance->fbbuf;
	mrng=instance->mrng;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=  effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		rng=  rngs+rngd*modfunc2(curlen,instance->slen,moddata2);
		mod=			modfunc3(curlen,instance->slen,moddata3);fb =-(fbs +fbd *mod);amp=  amps+ampd*mod;
		eff*=amp;

		// design filter
		hrng=(ULONG)rng;mat=instance->matrix;flen=hrng;					// build matrix
		for(k=0;k<hrng;k++) *(mat++)=instance->p_matrix[k];
		if(rng!=(double)hrng) {									// if rng is not integer
			*mat=(rng-(double)hrng)*instance->p_matrix[hrng];
			flen++;
		}
		// apply filter
		*rbufw=*sbuf;											/* get current value */

		sample=0.0;mat=&instance->matrix[flen];k=flen;					/* apply filter */
		tfbptr=fbptr-flen;if(tfbptr<0) tfbptr+=mrng;
		rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-flen)&rbufmask));
		while(k--) {
			sample+=(*(mat--)*(double)(*rbufr+fbbuf[tfbptr]));
			rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
			tfbptr=(tfbptr+1)%mrng;
		}
		sample/=rng;
		val1=(LONG)(((double)*(sbuf++)*dry)+(sample*eff));		/* result */
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		val1=(LONG)(sample*fb);									/* feedback resonancy */
		CLIPINT(val1);
		fbbuf[fbptr]=(WORD)val1;
		fbptr=(fbptr+1)%mrng;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->fbbuf)		{ FreeVec((ULONG *)instance->fbbuf);instance->fbbuf=NULL; }
	if(instance->matrix)	{ FreeVec((ULONG *)instance->matrix);instance->matrix=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
