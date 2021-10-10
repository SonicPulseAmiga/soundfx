/****h* DeNoise-FIR/DeNoise-FIR.c [4.2] *
*
*  NAME
*    DeNoise-FIR.c
*  COPYRIGHT
*    $VER: DeNoise-FIR.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    DeNoise-FIR core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*  IDEAS
*    modify attack time by change of volume
*      gentle change -> long attack
*      abrupt change -> short attack
*
*******
*/
/****u* _operators/DeNoise-FIR *
*  Function.eng
*    Removes noise from a sample (multifrequency Noisegate)
*  Funktion.ger
*    Entrauscht ein Sample (Multifrequenz Noisegate)
*  Parameter.eng
*	 Attack
*      When the operator detects noise, the noise smoothly will be faded out and back in.
*      This tells how fast this should happen.
*    Shape
*      Type of fade
*    Threshold
*      A signal-component quieter than this level, is considered as noise.
*    Bands
*      Just how many bands should SFX use. Less Bands means less math, but you lose out on accuracy.
*    Steps
*      SFX does a transformation every <steps> sample-values. Lower values mean better quallity but
*      longer calculation. Steps can no be bigger than the half of the number of bands.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*    Nr.
*      How much coefficients will be used. The more, the better is the band-diversion (max. 1024).
*  Parameter.ger
*    Attack
*      Ansprechdauer. Wenn der Operator Rauschen erkennt, wird dies sanft aus- und wieder eingeblendet.
*      Hiermit läßt sich einstellen wie schnell das geschieht.
*    Shape
*      Hüllform für das Aus- und Einblenden.
*    Threshold
*      Eine Signalkomponente die leiser als dieser Pegel ist, wird als Rauschen gewertet.
*    Bands
*      Wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
*      die Frequenzauflösung ist dann aber auch nicht so fein.
*    Steps
*      Aller wieviel Samples eine Transformation erstellt werden soll. Je öfters diese berechnet
*      werden, desto genauer das Ergebnis und desto 	höher auch die Rechenzeit. Steps darf max. halb
*      so groß wie die Nummer der Bänder sein.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      welche Fensterfunktion verwendet wird.
*    Nr.
*      Wieviele Koeffizienten für die Filter verwendet werden. Je mehr, desto beeser ist die
*      Bandtrennung (max. 1024).
*  Notes.eng
*    If threshold is set too high, too much of the sample will be suppressed.
*    The result might sound damp in this case<br>
*    The attack-value should be relative small. If it is too small,
*    the result might sound chopped.<br>
*    A good way to control the effect is to use the <a href="Analyse-Spect2D.html">Analyse-Spect2D</a>
*    operator with a low gamma-value (e.g. 0.2).
*    You should clearly see the noise in quiet sections.
*    After applying the DeNoise operator, check again with the Analyser. You should be able to see if the
*    noise-levels have dropped.<br>
*    __SFX divides the sample into several bands and denoises these.
*    Afterwards the signal will rebuild out of these. The diversion
*    is done by using <a href="../node06.03.00.Filter.html">FIR-Filter</a>.<br>
*    Before using this operator, it is recommended to apply the
*    <a href="Middle.html">Middle</a> operator, to prepare the sample.
*  Hinweise.ger
*    Wenn der Threshold zu hoch ist wird zuviel vom Sample unterdrückt.
*    Das Ergebnis kann in diesem Falle dumpf klingen.<br>
*    Der Attackwert sollte normalerweise recht klein sein. Wenn er aber zu klein ist,
*    kann sich das Ergebnis zerhackt anhören.<br>
*    Der Effekt kann gut mit Hilfe des <a href="Analyse-Spect2D.html">Analyse-Spect2D</a> Operators
*    und eines niedrigen Gamma-Wertes (z.B. 0.2) kontrolliert werden.
*    Nach dem Ausführen des DeNoise Operators prüfen sie erneut mit dem Analyser. Es sollte erkennbar sein
*    ob das Rauschen leiser geworden ist.<br>
*    Man sollte das Rauschen in leisen Abschnitten klar erkennen können.
*    __SFX trennt das Sample in mehrere Bänder auf und entrauscht
*    diese. Danach wird das Signal wieder zusammengesetzt. Die Bändertrennung
*    erfolgt mittels <a href="../node06.03.00.Filter.html">FIR-Filtern</a>.<br>
*    Bevor man diesen Operator nutzt empfiehlt es sich, erst den
*    <a href="Middle.html">Middle</a> Operator anzuwenden, um das Sample vorzubereiten.
*******
*/

#define DENOISE_FIR_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "DeNoise-FIR.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	LONG o;
	ULONG size=(ULONG)instance->anz*sizeof(double);
	double frqs,fg,frq,ff,fgs,fgc,ffs,ffc;
	double si0,si1,fcs;
	double co0,co1,fcc;
	double *mat;
	double *wptr;

	switch(instance->shape) {
		case CURVE_LINEAR:		instance->expon=1.0;break;
		case CURVE_SPEEDUP:		instance->expon=2.0;break;
		case CURVE_SLOWDOWN:	instance->expon=0.5;break;
	}

	if(!(instance->rbuf=NewRingBuf((ULONG)(instance->anz),&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	for(o=0;o<instance->bands;o++) if(!(instance->matrix[o]=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->win_fkt=NewWindowFkt((instance->anz<<1),instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
	if(!(instance->fadearr=(double *)AllocVec((instance->attack+1)*sizeof(double),MEMF_ANY))) return(FALSE);
	if(!(instance->fo		=(UBYTE  *)AllocVec(instance->bands*sizeof(UBYTE),MEMF_ANY))) return(FALSE);
	if(!(instance->fop	=(ULONG  *)AllocVec(instance->bands*sizeof(ULONG),MEMF_ANY))) return(FALSE);
	if(!(instance->ampenv	=(double *)AllocVec(instance->bands*sizeof(double),MEMF_ANY))) return(FALSE);
	if(!(instance->thresh	=(double *)AllocVec(instance->bands*sizeof(double),MEMF_ANY))) return(FALSE);

	instance->fadearr[0]=0.0;
	for(o=1;o<instance->attack;o++) instance->fadearr[o]=mypow(((double)o/(double)instance->attack),instance->expon);
	for(o=0;o<instance->bands;o++) instance->thresh[o]=((instance->threshs+((o*(instance->threshe-instance->threshs))/(double)instance->bands))*32768.0)/(double)instance->bands;
	//instance->thresh=(instance->thresh*32768.0)/(double)instance->bands;
	//instance->thresh=instance->thresh*32768.0;

	/* Filter entwerfen */

	frqs=1.0/(double)instance->bands;								/* Frequencysteps */

	fg=frqs;frq=frqs*PI;ff=0.00001;						/* 1. Band -> LowPass */
	si0=sin(-(frq+frq)+ff);
	si1=sin(-frq+ff);
	fcs=cos(frq);fcs+=fcs;
	mat=&instance->matrix[0][instance->anz-1];
	wptr=&instance->win_fkt[instance->anz-1];
	while(mat>instance->matrix[0]) {
		*(mat--)=fg*((si0=fcs*si1-si0)/ff)*(*(wptr--));ff+=frq;
		*(mat--)=fg*((si1=fcs*si0-si1)/ff)*(*(wptr--));ff+=frq;
	}
	//for(j=0;j<instance->anz;) {
	//	matrix[0][j]=fg*((si0=fcs*si1-si0)/ff)*win_fkt[instance->anz+j];ff+=frq;j++;
	//	matrix[0][j]=fg*((si1=fcs*si0-si1)/ff)*win_fkt[instance->anz+j];ff+=frq;j++;
	//}

	for(o=1;o<(instance->bands-1);o++) {							/* 2. bis vorletztes Band -> BandPässe */
		fgs=PI*fg*0.5;fgc=PI*((double)(o+o+1)*frqs)*0.5;ffs=ffc=0.00001;
		si0=sin(-(fgs+fgs)+ffs);
		si1=sin(-fgs+ffs);
		fcs=cos(fgs);fcs+=fcs;
		co0=cos(-(fgc+fgc)+ffc);
		co1=cos(-fgc+ffc);
		fcc=cos(fgc);fcc+=fcc;
		mat=&instance->matrix[o][instance->anz-1];
		wptr=&instance->win_fkt[instance->anz-1];
		while(mat>instance->matrix[o]) {
			*(mat--)=fg*(co0=fcc*co1-co0)*((si0=fcs*si1-si0)/ffs)*(*(wptr--));ffs+=fgs;
			*(mat--)=fg*(co1=fcc*co0-co1)*((si1=fcs*si0-si1)/ffs)*(*(wptr--));ffs+=fgs;
		}
		//for(j=0;j<instance->anz;) {
		//	matrix[o][j]=fg*(co0=fcc*co1-co0)*((si0=fcs*si1-si0)/ffs)*win_fkt[instance->anz+j];ffs+=fgs;j++;
		//	matrix[o][j]=fg*(co1=fcc*co0-co1)*((si1=fcs*si0-si1)/ffs)*win_fkt[instance->anz+j];ffs+=fgs;j++;
		//}
	}

	fg=frqs;frq=fg*PI;ff=0.00001;				/* letztes Band -> HiPass */
	si0=sin(-(frq+frq)+ff);
	si1=sin(-frq+ff);
	fcs=cos(frq);fcs+=fcs;
	mat=&instance->matrix[o][instance->anz-1];
	wptr=&instance->win_fkt[instance->anz-1];
	while(mat>instance->matrix[o]) {
		*(mat--)= fg*((si0=fcs*si1-si0)/ff)*(*(wptr--));ff+=frq;
		*(mat--)=-fg*((si1=fcs*si0-si1)/ff)*(*(wptr--));ff+=frq;
	}
	//for(j=0;j<instance->anz;) {
	//	matrix[o][j]=-fg*((si0=fcs*si1-si0)/ff)*win_fkt[instance->anz+j];ff+=frq;j++;
	//	matrix[o][j]= fg*((si1=fcs*si0-si1)/ff)*win_fkt[instance->anz+j];ff+=frq;j++;
	//}

	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	unsigned short o;
	ULONG att2;
	ULONG *pfop;

	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	memset(instance->ampenv,0,((instance->bands)*sizeof(double)));	// clear ampenv in every pass
	memset(instance->fo,0,((instance->bands)*sizeof(UBYTE)));	// clear fo in every pass
	att2=instance->attack-1;
	pfop=instance->fop;
	for(o=0;o<instance->bands;o++) *(pfop++)=att2;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned short o;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *rbufbase;
	ULONG rbufmask;
	SAMPLE *rbufr,*rbufw;
	double ssample,amp;
    double sample[MAX_BANDS]={NULL},*psample;
	double *fadearr;
	double *thresh,*pthresh;
	double *ampenv,*pampenv;
	ULONG *fop,*pfop;
	UBYTE *fo,*pfo;
	UWORD anz,anz2;
	LONG matix;
	ULONG att2;
	UBYTE bands;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	ampenv=instance->ampenv;
	fadearr=instance->fadearr;
	fo=instance->fo;
	fop=instance->fop;
	thresh=instance->thresh;
	anz=instance->anz;
	bands=instance->bands;
	att2=instance->attack-1;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=*(sbuf++);										/* get current value */

		anz2=anz;matix=0;										/* apply filters */
		memset(sample,0,(bands*sizeof(double)));
		rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-anz)&rbufmask));
		while(anz2--) {
			for(o=0;o<bands;o++) sample[o]+=(instance->matrix[o][matix]*(*rbufr));
			rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
			matix++;
		}

		ssample=0.0;
		pfo=fo;pfop=fop;pampenv=ampenv;psample=sample;pthresh=thresh;
		for(o=0;o<bands;o++) {									/* Fading */
			amp=fabs(*psample);
			if(amp>*pampenv) *pampenv=amp;								/* Envelopefollower */
			else *pampenv=ENVFC1*(*pampenv)+ENVFC2*amp;
			if(*pampenv<*pthresh) {										/* >= Schwellwert */
				if(!*pfo) { *pfop=att2;*pfo=TRUE; }							/* Ausfaden initialisieren */
				ssample+=(*psample*fadearr[*pfop]);
				if(*pfop) (*pfop)--;
			}
			else {														/* >= Schwellwert */
				ssample+=*psample;											/* Wert normal addieren */
				*pfo=FALSE;														/* Ausfaden aus */
			}
			pfo++;pfop++;pampenv++;psample++;pthresh++;
		}
		CLIPFLT(ssample);
		*(dbuf++)=(SAMPLE)ssample;

		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	LONG o;

	if(instance->rbuf)		{ DelRingBuf(instance->rbuf);instance->rbuf=NULL; }
	for(o=0;o<instance->bands;o++) if(instance->matrix[o]) { FreeVec((ULONG *)instance->matrix[o]);instance->matrix[o]=NULL; }
	if(instance->thresh)	{ FreeVec((ULONG *)instance->thresh);instance->thresh=NULL; }
	if(instance->ampenv)	{ FreeVec((ULONG *)instance->ampenv);instance->ampenv=NULL; }
	if(instance->win_fkt)	{ DelWindowFkt(instance->win_fkt);instance->win_fkt=NULL; }
	if(instance->fadearr)	{ FreeVec((ULONG *)instance->fadearr);instance->fadearr=NULL; }
	if(instance->fo)		{ FreeVec((ULONG *)instance->fo);instance->fo=NULL; }
	if(instance->fop)		{ FreeVec((ULONG *)instance->fop);instance->fop=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
