/****h* Filter-FIRHiPass/Filter-FIRHiPass.c [4.2] *
*
*  NAME
*    Filter-FIRHiPass.c
*  COPYRIGHT
*    $VER: Filter-FIRHiPass.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-FIRHiPass core file
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
/****u* _operators/Filter-FIRHiPass *
*  Function.eng
*    Works on low frequencies, means supresses or boosts them and let high frequencies pass through unaltered.
*  Funktion.ger
*    Bearbeitet tiefe Frequenzen des Samples, d.h. unterdrückt oder verstärkt sie und läßt Hohe durch.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome. Negative values produce the
*      opposite	effect - they boost frequencys.
*    Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      all frequencies below are getting processed, relative frequency ranging from 0 Hz to half of sampling-rate
*    Nr. (Length)
*      How many coefficients should be used. The more they are, the better results will be get.(max 1024, but 64 is usually enough)
*    Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt. Negative Effektanteile bewirken keine Dämpfung,
*      sondern eine Verstärkung der zu bearbeitenden Frequenzen.
*    Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      alle darunterliegenden Frequenzen werden bearbeitet, relative Frequenz von 0 Hz bis zur halben Samplingrate
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

#define FILTER_FIRHIPASS_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Filter-FIRHiPass.h"

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
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	double frqs,frqd,frq;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *rbufbase,*rbufw,*rbufr;
	ULONG rbufmask;
	double sample;
	LONG val1;
	double *matrix,*mat;
	double fg,ff;
	double si0,si1,fc;
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
	frqs=instance->frqs;
	frqd=instance->frqd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	anz=instance->anz;
	win_fkt=instance->win_fkt;
	matrix=instance->matrix;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		frq=frqs+frqd*modfunc2(curlen,instance->slen,moddata2);

		// design filter
		fg=frq;frq*=PI;ff=0.00001;
		si0=sin(-(frq+frq)+ff);
		si1=sin(-frq+ff);
		fc=cos(frq);fc+=fc;
		mat=&matrix[anz-1];wptr=&win_fkt[anz-1];
		while(mat>matrix) {
			*(mat--)= fg*((si0=fc*si1-si0)/ff)*(*(wptr--));ff+=frq;
			*(mat--)=-fg*((si1=fc*si0-si1)/ff)*(*(wptr--));ff+=frq;
		}
		//matrix[anz-1]=1.0-matrix[anz-1];
		
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
