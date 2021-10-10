/****h* DeNoise-FFT/DeNoise-FFT.c [4.2] *
*
*  NAME
*    DeNoise-FFT.c
*  COPYRIGHT
*    $VER: DeNoise-FFT.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    DeNoise-FFT core file
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
/****u* _operators/DeNoise-FFT *
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
*    It is often very difficult to find the right settings. Processing samples
*    with this operator leads in most cases to an alienated sound, which
*    sounds sometimes very interesting.<br>
*    This operator uses the <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
*    for its calculations.<br>
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
*    Es ist sehr kompliziert, die richtigen Einstellungen zu finden.
*    Das Bearbeiten mit diesem Operator führt nahezu immer zu diversen
*    Klangverfremdungen, die allerdings teilweise sehr interessant sind.<br>
*    Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a> verwendet.<br>
*    Bevor man diesen Operator nutzt empfiehlt es sich, erst den
*    <a href="Middle.html">Middle</a> Operator anzuwenden, um das Sample vorzubereiten.
*******
*/

#define DENOISE_FFT_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "DeNoise-FFT.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	ULONG i,size;
	double fact;

	instance->mb=1<<(instance->bands+2);								// the number of frequency bands we want to get
	instance->mw=instance->mb<<1;										// the width of the databuffer is twice as large as the number of bands
	instance->mv=instance->mw-(instance->mw>>1);						// put how many values into databuffer, zero the rest
	instance->ms=1<<instance->steps;									// do a analysys every <ms> steps
	if(instance->ms>=(instance->mb>>1)) instance->ms=instance->mb>>1;	// steps must be at least mb/2
	size=(instance->mw+2)*sizeof(double);

	if(!(instance->flbuf_r	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->flbuf_i	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->win_fkt1	=NewWindowFkt(instance->mv,instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
	if(!(instance->win_fkt2	=NewWindowFkt(instance->mb,WIN_BARTLETT,NULL))) return(FALSE);
	if(!					 NewFFTTabs(&instance->bitrevtab,&instance->sintab1,&instance->sintab2,&instance->costab,instance->mb)) return(FALSE);
	if(!(instance->irbuf	=NewRingBuf(instance->mw,&instance->irbufbase,&instance->irbufmask,&instance->irbufsize))) return(FALSE);
	if(!(instance->orbuf	=NewRingBuf(instance->mw,&instance->orbufbase,&instance->orbufmask,&instance->orbufsize))) return(FALSE);
	if(!(instance->ampenv	=(double *)AllocVec((instance->mb+1)*sizeof(double),MEMF_ANY))) return(FALSE);
	if(!(instance->fadearr	=(double *)AllocVec((instance->attack+1)*sizeof(double),MEMF_ANY))) return(FALSE);
	if(!(instance->fo		=(UBYTE  *)AllocVec((instance->mb+1)*sizeof(UBYTE),MEMF_ANY))) return(FALSE);
	if(!(instance->fop		=(ULONG  *)AllocVec((instance->mb+1)*sizeof(ULONG),MEMF_ANY))) return(FALSE);

	fact=(4.0*32767.0)/((double)instance->mb/(double)instance->ms);		// make triangle quieter depending on steps
	for(i=0;i<instance->mb;i++) instance->win_fkt2[i]*=fact;
	instance->fadearr[0]=0.0;
	for(i=1;i<=instance->attack;i++) instance->fadearr[i]=mypow(((double)i/(double)instance->attack),instance->expon);	// values are ranging from 0.0 to 1.0
	
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	memset(instance->irbuf,0,(instance->irbufsize<<1));
	instance->irbufw=instance->irbufbase;
	memset(instance->orbuf,0,(instance->orbufsize<<1));
	instance->orbufw=instance->orbufbase;
	memset(instance->ampenv,0,((instance->mb+1)*sizeof(double)));	// clear ampenv in every pass
	memset(instance->fo,0,((instance->mb+1)*sizeof(UBYTE)));	// clear fo in every pass
	instance->pos=(instance->ms-1);			// weil curlen erst später erhöht wird
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned short o;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *irbufbase,*orbufbase;
	ULONG irbufmask,orbufmask;
	SAMPLE *irbufr,*irbufw,*orbufr,*orbufw;
	UWORD mb,mw,ms,mv;
	double *flbuf_r,*flbuf_i,*flp1_r,*flp1_i,*flp2_r,*flp2_i;
	double *win_fkt1,*win_fkt2,*winfp;
	double thresh,amp;
	ULONG pos,att2;
	double *ampenv,*fadearr,*pampenv;
	UBYTE *fo,*pfo;
	ULONG *fop,*pfop;
	LONG val1;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	irbufbase=instance->irbufbase;
	irbufmask=instance->irbufmask;
	irbufw=instance->irbufw;
	orbufbase=instance->orbufbase;
	orbufmask=instance->orbufmask;
	orbufw=instance->orbufw;
	mb=instance->mb;
	mw=instance->mw;
	ms=instance->ms;
	mv=instance->mv;
	flbuf_r=instance->flbuf_r;
	flbuf_i=instance->flbuf_i;
	win_fkt1=instance->win_fkt1;
	win_fkt2=instance->win_fkt2;
	ampenv=instance->ampenv;
	fadearr=instance->fadearr;
	fo=instance->fo;
	fop=instance->fop;
	thresh=instance->thresh;
	pos=instance->pos;
	att2=instance->attack-1;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		*irbufw=*(sbuf++);
		irbufw=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufw+1)&irbufmask));
		if(curlen==pos) {										/* we've read the next ms samples */
			pos+=ms;											/* ms=stepsize can range from 1 to 2^n */
			flp1_r=flbuf_r;flp1_i=flbuf_i;winfp=win_fkt1;
			irbufr=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufw-mv)&irbufmask));
			for(o=0;o<mv;o++) {			/* read the last mv values */
				*(flp1_r++)=(*(winfp++))*((double)(*irbufr)/32768.0);*(flp1_i++)=0.0;
				irbufr=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufr+1)&irbufmask));
			}
			for(o=mv;o<mw;o++) {/* set rest to zero */
				*(flp1_r++)=*(flp1_i++)=0.0;
			}

			Transform(flbuf_r,flbuf_i,instance->bitrevtab,instance->sintab1,instance->costab,-1,mb);	/* -> to frequency domain */

			pfo=fo;pfop=fop;pampenv=ampenv;								/* 0. Band */
			amp=sqrt(flbuf_r[0]*flbuf_r[0]+flbuf_i[0]*flbuf_i[0]);			/* will always be positive */
			if(amp>*pampenv) *pampenv=amp;									/* Envelopefollower */
			else *pampenv=ENVFC1*(*pampenv)+ENVFC1*amp;
			if(*pampenv<thresh) {
				if(!*pfo) { *pfop=att2;*pfo=1; }
				flbuf_r[0]*=fadearr[*pfop];
				flbuf_i[0]*=fadearr[*pfop];
				if(*pfop) (*pfop)--;
			}
			else *pfo=0;
			pfo++;pfop++;pampenv++;
			flp1_r=&flbuf_r[   1];flp1_i=&flbuf_i[   1];
			flp2_r=&flbuf_r[mw-1];flp2_i=&flbuf_i[mw-1];
			for(o=0;o<mb;o++) {											/* for each band */
				amp=sqrt((*flp1_r)*(*flp1_r)+(*flp1_i)*(*flp1_i));			/* will always be positive */
				if(amp>*pampenv) *pampenv=amp;								/* Envelopefollower */
				else *pampenv=0.95*(*pampenv)+0.05*amp;
				if(*pampenv<thresh) {
					if(!*pfo) { *pfop=att2;*pfo=1; }
					*(flp2_r--)=(*(flp1_r++)*=fadearr[*pfop]);
					*(flp2_i--)=(*(flp1_i++)*=fadearr[*pfop]);
					if(*pfop) (*pfop)--;
				}
				else *pfo=0;
				pfo++;pfop++;pampenv++;
			}

			Transform(flbuf_r,flbuf_i,instance->bitrevtab,instance->sintab2,instance->costab,1,mb);	/* -> time domain */

			flp1_r=flbuf_r;winfp=win_fkt2;
			orbufr=orbufw;			/* after the loop we will have ms values ready from here */
			for(o=0;o<mb;o++) {		/* Werte zurückschreiben */
				val1=(LONG)*orbufw+(LONG)((*(winfp++))*(*(flp1_r++)));
				CLIPINT(val1);
				*orbufw=(SAMPLE)val1;
				orbufw=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufw+1)&orbufmask));
			}
			orbufw=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufr+ms)&orbufmask));	/* advance writebuffer from start+ms */
			for(o=0;o<ms;o++) {		/* copy to outputblock and clear rbuf */
				*(dbuf++)=*orbufr;
				*orbufr=0;
				orbufr=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufr+1)&orbufmask));
			}
		}
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->irbufw=irbufw;
	instance->orbufw=orbufw;
	instance->pos=pos;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->flbuf_r) 	{ FreeVec((ULONG *)instance->flbuf_r);instance->flbuf_r=NULL; }
	if(instance->flbuf_i)	{ FreeVec((ULONG *)instance->flbuf_i);instance->flbuf_i=NULL; }
							  DelFFTTabs(instance->bitrevtab,instance->sintab1,instance->sintab2,instance->costab);
							  instance->bitrevtab=NULL;instance->sintab1=instance->sintab2=instance->costab=NULL;
	if(instance->win_fkt1)	{ DelWindowFkt(instance->win_fkt1);instance->win_fkt1=NULL; }
	if(instance->win_fkt2)	{ DelWindowFkt(instance->win_fkt2);instance->win_fkt2=NULL; }
	if(instance->irbuf)		{ DelRingBuf(instance->irbuf);instance->irbuf=NULL; }
	if(instance->orbuf)		{ DelRingBuf(instance->orbuf);instance->orbuf=NULL; }
	if(instance->ampenv)	{ FreeVec((ULONG *)instance->ampenv);instance->ampenv=NULL; }
	if(instance->fadearr)	{ FreeVec((ULONG *)instance->fadearr);instance->fadearr=NULL; }
	if(instance->fo)		{ FreeVec((ULONG *)instance->fo);instance->fo=NULL; }
	if(instance->fop)		{ FreeVec((ULONG *)instance->fop);instance->fop=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
