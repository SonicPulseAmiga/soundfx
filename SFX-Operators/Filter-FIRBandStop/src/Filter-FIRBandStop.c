/****h* Filter-FIRBandStop/Filter-FIRBandStop.c [4.2] *
*
*  NAME
*    Filter-FIRBandStop.c
*  COPYRIGHT
*    $VER: Filter-FIRBandStop.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-FIRBandStop core file
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
/****u* _operators/Filter-FIRBandStop *
*  Function.eng
*    Works on a specific frequency-band, means supresses or boosts them and let the band pass through unaltered.
*  Funktion.ger
*    Bearbeitet ein bestimmtes Frequenzband, welches gedämpft oder geboostet werden und läßt das Band durch.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome. Negative values produce the
*      opposite	effect - they boost frequencys.
*    Low Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      lower bound of the band, relative frequencies ranging from 0 Hz to half of sampling-rate
*    High Cut-Off (<a href="../node03.02.01.html">P3</a>)
*      upper bound of the band
*    Nr. (Length)
*      How many coefficients should be used. The more they are, the better results will be get.(max 1024, but 64 is usually enough)
*    Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt. Negative Effektanteile bewirken keine Dämpfung,
*      sondern eine Verstärkung der zu bearbeitenden Frequenzen.
*    Low Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      untere Schranke des Bandes, relative Frequenz von 0 Hz bis zur halben Samplingrate
*    High Cut-Off (<a href="../node03.02.01.html">P3</a>)
*      obere Schranke des Bandes
*    Nr. (Length)
*      Wieviele Koeffizienten benutzt werden sollen. Je mehr Koeffizienten benutzt werden,
*      desto besser (max. 1024, 64 ist in der Regel ausreichend)
*    Window (<a href="../node03.02.03.html">W1</a>)
*      welche Fensterfunktion verwendet wird.
*  Notes.eng
*    Please don't wonder because of these long calculation-times. If you are using
*    e.g. 64 coefficients, SFX needs to do 128 multiplications and 128 additions
*    for each samplevalue. Since SFX lets you modulate filterspecifications (and
*    not using fixed ones like other programs do), it has to redesign the filter
*    each samplevalue. Therefore again a bunch of calculation steps are neccessary.<br>
*    For <a href="../node06.03.00.Filter.html">FIR-filters</a> a mathematical
*    coprocessor really helps !
*  Hinweise.ger
*    Bitte wundern Sie sich nicht über die teilweise recht langen Rechenzeiten.
*    Wenn Sie z.B. mit 64 Koeffizienten arbeiten, werden 128 Multiplikationen
*    und 128 Additionen pro Samplewert durchgeführt. Da Sie die Filterspezifikationen
*    in SFX modulieren lassen können (und nicht wie in anderen Programmen fest
*    einstellen muessen), muß der Filter bei jedem Rechenschritt neu entworfen
*    werden. Dazu sind nochmals eine ganze Menge von Rechenoperationen notwendig.<br>
*    Bei den <a href="../node06.03.00.Filter.html">FIR-Filtern</a> hift ein
*    mathematischer Co-Prozessor erheblich !
*******
*/

#define FILTER_FIRBANDSTOP_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Filter-FIRBandStop.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	if(!(instance->win_fkt=NewWindowFkt((instance->anz<<1),instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
	if(!(instance->matrix =(double *)AllocVec(instance->anz*sizeof(double),MEMF_ANY))) return(FALSE);
	if(!(instance->rbuf=NewRingBuf(instance->anz,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	AssignBShape(&instance->mod3);
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	double frq1s,frq1d,frq1;
	double frq2s,frq2d,frq2;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *rbufbase,*rbufw,*rbufr;
	ULONG rbufmask;
	double sample;
	LONG val1;
	double *matrix,*mat;
	double fg,fgs,fgc,ffs,ffc;
	double si0,si1,fcs;
	double co0,co1,fcc;
	UWORD anz,anz2;
	double *win_fkt,*wptr;

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
	frq1s=instance->frq1s;
	frq1d=instance->frq1d;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	frq2s=instance->frq2s;
	frq2d=instance->frq2d;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	anz=instance->anz;
	win_fkt=instance->win_fkt;
	matrix=instance->matrix;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff =effs +effd *modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		frq1=frq1s+frq1d*modfunc2(curlen,instance->slen,moddata2);
		frq2=frq2s+frq2d*modfunc3(curlen,instance->slen,moddata3);

		// design filter
		fg=(frq2-frq1);fgs=PI*fg*0.5;fgc=PI*(frq2+frq1)*0.5;ffs=ffc=0.00001;
		si0=sin(-(fgs+fgs)+ffs);
		si1=sin(-fgs+ffs);
		fcs=cos(fgs);fcs+=fcs;
		co0=cos(-(fgc+fgc)+ffc);
		co1=cos(-fgc+ffc);
		fcc=cos(fgc);fcc+=fcc;
		mat=&matrix[anz-1];wptr=&win_fkt[anz-1];
		while(mat>matrix) {
			*(mat--)= fg*(co0=fcc*co1-co0)*((si0=fcs*si1-si0)/ffs)*(*(wptr--));ffs+=fgs;
			*(mat--)=-fg*(co1=fcc*co0-co1)*((si1=fcs*si0-si1)/ffs)*(*(wptr--));ffs+=fgs;
		}
		//this does not filter enough
		//matrix[anz-1]=1.0-matrix[anz-1];
		// *1 this affects the outer bands as well
		// *2 we don't need to do this for the highpass, but if we omit it, it acts like the bandpass
		matrix[anz-1]=-matrix[anz-1];

		// apply filter
		*rbufw=*sbuf;											/* get current value */

		sample=0.0;mat=matrix;anz2=anz;							/* apply filter */
		rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-anz)&rbufmask));
		while(anz2--) {
			sample+=(*(mat++)*(double)*rbufr);
			rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
		}

		val1=(LONG)((double)*(sbuf++)*dry+sample*eff);			/* result */
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->win_fkt)	{ DelWindowFkt(instance->win_fkt);instance->win_fkt=NULL; }
	if(instance->matrix)	{ FreeVec((ULONG *)instance->matrix);instance->matrix=NULL; }
	if(instance->rbuf)		{ DelRingBuf(instance->rbuf);instance->rbuf=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
