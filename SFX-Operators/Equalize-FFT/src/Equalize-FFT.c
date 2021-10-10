/****h* Equalize-FFT/Equalize-FFT.c [4.2] *
*
*  NAME
*    Equalize-FFT.c
*  COPYRIGHT
*    $VER: Equalize-FFT.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Equalize-FFT core file
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
/****u* _operators/Equalize-FFT *
*  Function.eng
*    Modify the amplitude of the frequency components of a sample
*  Funktion.ger
*    Ändert die Lautstärke der Frequenzbestandteile des Samples
*  Parameter.eng
*    Frequency-Curve
*      Here you can draw the shape of the frequency spectrum.
*    Arrow-Gadgets
*      for moving the curve
*    F-Gadget
*      Flip, mirrors the curve
*    Band
*      number of the band you're currently working on
*    Val
*      value of current band
*    Frq
*      shows the frequency range for the current band.
*    Range
*      Simple tool to do a straight line between two bands. For those who can't draw these lines
*      (like me) just click on the first band then range and then the second band.
*    Mode
*      Gives you the choice of moving just the current band or all when using the arrow buttons..
*    Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*    Bands
*      Just how many bands should SFX use. Less Bands means less math, but you lose out on accuracy.
*    Steps
*      SFX does a transformation every <steps> sample-values. Lower values mean better quallity but
*      longer calculation. Steps can no be bigger than the half of the number of bands.
*  Parameter.ger
*    Frequency-Curve
*      Hier können sie die Form des Frequenzspektrums verändern.
*    Pfeil-Gadgets
*      dienen dem Verschieben der Kurve
*    F-Gadget
*      Flip, spiegelt die Kurve
*    Band
*      zeigt die Nummer des aktuellen Bandes an
*    Val
*      zeigt den Wert des aktuellen Bandes
*    Frq
*      zeigt den Frequenzbereich für das aktuelle Band
*    Range
*      Hiermit kann ein linearer Verlauf zwischen 2 Bändern erzeugt werden. Dazu klickt man das 1.
*      Band an, dann auf Range und jetzt wählt man das 2. Band aus.
*    Mode
*      Hier kann man auswählen, ob alle Bänder oder nur das aktuelle verschoben werden sollen,
*      wenn man die Pfeil-Buttons benutzt.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      welche Fensterfunktion verwendet wird.
*    Bands
*      Wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
*      die Frequenzauflösung ist dann aber auch nicht so fein.
*    Steps
*      Aller wieviel Samples eine Transformation erstellt werden soll. Je öfters diese berechnet
*      werden, desto genauer das Ergebnis und desto 	höher auch die Rechenzeit. Steps darf max. halb
*      so groß wie die Nummer der Bänder sein.
*  Notes.eng
*    This operator uses the <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
*    for its calculations.
*  Hinweise.ger
*    Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a> verwendet.
*******
*/

#define EQUALIZE_FFT_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Equalize-FFT.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	ULONG i,size,ix;
	double fact,ant;

	instance->mb=1<<(instance->bands+2);								// the number of frequency bands we want to get
	instance->mw=instance->mb<<1;										// the width of the databuffer is twice as large as the number of bands
	instance->mv=instance->mw-(instance->mw>>1);						// put how many values into databuffer, zero the rest
	instance->ms=1<<instance->steps;									// do a analysys every <ms> steps
	if(instance->ms>=(instance->mb>>1)) instance->ms=instance->mb>>1;	// steps must be at least mb/2
	size=(instance->mw+2)*sizeof(double);
	fact=64.0/(double)instance->mb;							// interpolate frq.coeffs
	for(i=0;i<=instance->mb;i++) {
		ix=(UWORD)(i*fact);
		instance->teqf[i]=(ant=(i*fact)-ix)*instance->eqf[ix+1]+(1.0-ant)*instance->eqf[ix];
	}

	if(!(instance->flbuf_r	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->flbuf_i	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->win_fkt1	=NewWindowFkt(instance->mv,instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
	if(!(instance->win_fkt2	=NewWindowFkt(instance->mb,WIN_BARTLETT,NULL))) return(FALSE);
	if(!					 NewFFTTabs(&instance->bitrevtab,&instance->sintab1,&instance->sintab2,&instance->costab,instance->mb)) return(FALSE);
	if(!(instance->irbuf	=NewRingBuf(instance->mw,&instance->irbufbase,&instance->irbufmask,&instance->irbufsize))) return(FALSE);
	if(!(instance->orbuf	=NewRingBuf(instance->mw,&instance->orbufbase,&instance->orbufmask,&instance->orbufsize))) return(FALSE);

	/* @todo does the result of the transformation becomes quieter, when using more zeros ? */
	//fact=(4.0*32767.0)/((1.0-instance->zeros)*(double)instance->mb/(double)instance->ms);		// make triangle quieter depending on steps
	fact=(4.0*32767.0)/((double)instance->mb/(double)instance->ms);		// make triangle quieter depending on steps
	for(i=0;i<instance->mb;i++) instance->win_fkt2[i]*=fact;

	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	memset(instance->irbuf,0,(instance->irbufsize<<1));
	instance->irbufw=instance->irbufbase;
	memset(instance->orbuf,0,(instance->orbufsize<<1));
	instance->orbufw=instance->orbufbase;
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
	ULONG pos;
	LONG val1;
	double *teqf,*teqfp;

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
	pos=instance->pos;
	teqf=instance->teqf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		*irbufw=*(sbuf++);
		irbufw=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufw+1)&irbufmask));
		if(curlen==pos) {										/* we've read the next ms samples */
			pos+=ms;											/* ms=stepsize can range from 1 to m/2 */
			flp1_r=flbuf_r;flp1_i=flbuf_i;winfp=win_fkt1;
			irbufr=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufw-mv)&irbufmask));
			for(o=0;o<mv;o++) {	/* read in values */
				*(flp1_r++)=(*(winfp++))*((double)(*irbufr)/32768.0);*(flp1_i++)=0.0;
				irbufr=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufr+1)&irbufmask));
			}
			for(o=mv;o<mw;o++) {/* set rest to zero */
				*(flp1_r++)=*(flp1_i++)=0.0;
			}

			Transform(flbuf_r,flbuf_i,instance->bitrevtab,instance->sintab1,instance->costab,-1,mb);	/* -> frequency domain */

			flbuf_r[0]*=teqf[0];										/* 0. Band */
			flbuf_i[0]*=teqf[0];
			teqfp=&teqf[1];												/* restliche Bänder */
			flp1_r=&flbuf_r[   1];flp1_i=&flbuf_i[   1];
			flp2_r=&flbuf_r[mw-1];flp2_i=&flbuf_i[mw-1];
			for(o=0;o<mb;o++) {
				*(flp2_r--)=(*(flp1_r++)*=*teqfp);						/* mult by real fc */
				*(flp2_i--)=(*(flp1_i++)*=*(teqfp++));
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
			orbufw=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufr+ms)&orbufmask));		/* advance writebuffer from start+ms */
			for(o=0;o<ms;o++) {	/* copy to outputblock and clear rbuf */
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
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
