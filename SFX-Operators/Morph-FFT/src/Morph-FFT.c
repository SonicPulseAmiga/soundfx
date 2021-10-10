/****h* Morph-FFT/Morph-FFT.c [4.2] *
*
*  NAME
*    Morph-FFT.c
*  COPYRIGHT
*    $VER: Morph-FFT.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Morph-FFT core file
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
/****u* _operators/Morph-FFT *
*  Function.eng
*    Changes the frequencyspectrum of source 1 in that of source 2.
*  Funktion.ger
*    Wandelt das Frequenzspektrum von Source 1 in das von Source 2 um.
*  Parameter.eng
*    Morph (<a href="../node03.02.01.html">P1</a>)
*      Controls the transition from source 1 to source 2.
*    Bands
*      Just how many bands should SFX use. Less Bands means less math, but you lose out on accuracy.
*    Steps
*      SFX does a transformation every <steps> sample-values. Lower values mean better quallity but
*      longer calculation. Steps can no be bigger than the half of the number of bands.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*  Parameter.ger
*    Morph (<a href="../node03.02.01.html">P1</a>)
*      Kontrolliert den Übergang von Source1 nach Source2
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
*    Try to slowly morph e.g. two long sinewaves with different pitch.<br>
*    This operator uses the <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
*    for its calculations.
*  Hinweise.ger
*    Versuchen Sie z.B. mal zwei lange Sinustöne mit verschieder Tonhöhe langsam
*    zu morphen.<br>
*    Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a> verwendet.
*******
*/

#define MORPH_FFT_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Morph-FFT.h"

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

	instance->m=1L<<(instance->bands+2);
	instance->mm=instance->m<<1;
	instance->m2=1L<<instance->steps;
	if((instance->m2<<1)>=instance->m) instance->m2=instance->m>>1;		// steps must be at least m/2
	size=(instance->mm+2)*sizeof(double);

	if(!(instance->flbuf1_r	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->flbuf1_i	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->flbuf2_r	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->flbuf2_i	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->flbuf3_r	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->flbuf3_i	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->win_fkt1	=NewWindowFkt(instance->mm,instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
	if(!(instance->win_fkt2	=NewWindowFkt(instance->m,WIN_BARTLETT,NULL))) return(FALSE);
	if(!					 NewFFTTabs(&instance->bitrevtab,&instance->sintab1,&instance->sintab2,&instance->costab,instance->m)) return(FALSE);
	if(!(instance->irbuf1	=NewRingBuf(instance->mm,&instance->irbufbase1,&instance->irbufmask,&instance->irbufsize))) return(FALSE);
	if(!(instance->irbuf2	=NewRingBuf(instance->mm,&instance->irbufbase2,&instance->irbufmask,&instance->irbufsize))) return(FALSE);
	if(!(instance->orbuf	=NewRingBuf(instance->mm,&instance->orbufbase ,&instance->orbufmask,&instance->orbufsize))) return(FALSE);

	fact=instance->ampf*(4.0*32767.0)/((double)instance->m/(double)instance->m2);		// make triangle quieter depending on steps
	for(i=0;i<instance->m;i++) instance->win_fkt2[i]*=fact;

	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod);
	memset(instance->irbuf1,0,(instance->irbufsize<<1));
	instance->irbufw1=instance->irbufbase1;
	memset(instance->irbuf2,0,(instance->irbufsize<<1));
	instance->irbufw2=instance->irbufbase2;
	memset(instance->orbuf,0,(instance->orbufsize<<1));
	instance->orbufw=instance->orbufbase;
	instance->pos=(instance->m2-1);			// weil curlen erst später erhöht wird
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned short o,k;
	double mors,mord,mor;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf1,*sbuf2;
	SAMPLE *irbufr1,*irbufw1,*irbufr2,*irbufw2,*orbufr,*orbufw;
	UWORD m,mm,m2;
	double maxv,mv,ant1,ant2;
	UWORD maxb,mb1,mb2;
	double *flbuf1_r,*flbuf1_i,*flbuf2_r,*flbuf2_i,*flbuf3_r,*flbuf3_i;
	double *flp1_r,*flp1_i,*flp2_r,*flp2_i;
	double *win_fkt1,*win_fkt2,*winfp;
	SAMPLE *irbufbase1;
	SAMPLE *irbufbase2;
	ULONG irbufmask;
	SAMPLE *orbufbase;
	ULONG orbufmask;
	double *mb1ptr,*mb2ptr;
	ULONG pos;
	double val1;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	mors=instance->mors;
	mord=instance->mord;
	//
	irbufmask=instance->irbufmask;
	irbufbase1=instance->irbufbase1;
	irbufw1=instance->irbufw1;
	irbufbase2=instance->irbufbase2;
	irbufw2=instance->irbufw2;
	orbufbase=instance->orbufbase;
	orbufmask=instance->orbufmask;
	orbufw=instance->orbufw;
	//
	pos=instance->pos;
	m2=instance->m2;
	mm=instance->mm;
	m=instance->m;
	flbuf1_r=instance->flbuf1_r;
	flbuf1_i=instance->flbuf1_i;
	flbuf2_r=instance->flbuf2_r;
	flbuf2_i=instance->flbuf2_i;
	flbuf3_r=instance->flbuf3_r;
	flbuf3_i=instance->flbuf3_i;
	win_fkt1=instance->win_fkt1;
	win_fkt2=instance->win_fkt2;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		mor=mors+mord*modfunc(curlen,instance->slen,moddata);

		*irbufw1=*(sbuf1++);irbufw1=(SAMPLE *)((ULONG)irbufbase1|((ULONG)(irbufw1+1)&irbufmask));
		*irbufw2=*(sbuf2++);irbufw2=(SAMPLE *)((ULONG)irbufbase2|((ULONG)(irbufw2+1)&irbufmask));
		if(curlen==pos) {													/* we've read the next m2 samples */
			pos+=m2;
			flp1_r=flbuf1_r;flp1_i=flbuf1_i;
			flp2_r=flbuf2_r;flp2_i=flbuf2_i;
			winfp=win_fkt1;
			irbufr1=(SAMPLE *)((ULONG)irbufbase1|((ULONG)(irbufw1-mm)&irbufmask));	/* take out the last mm values */
			irbufr2=(SAMPLE *)((ULONG)irbufbase2|((ULONG)(irbufw2-mm)&irbufmask));	/* take out the last mm values */
			for(o=0;o<mm;o++) {		/* Werte einlesen */
				*(flp1_r++)=(* winfp   )*((double)(*irbufr1)/32768.0);*(flp1_i++)=0.0;
				*(flp2_r++)=(*(winfp++))*((double)(*irbufr2)/32768.0);*(flp2_i++)=0.0;
				irbufr1=(SAMPLE *)((ULONG)irbufbase1|((ULONG)(irbufr1+1)&irbufmask));
				irbufr2=(SAMPLE *)((ULONG)irbufbase2|((ULONG)(irbufr2+1)&irbufmask));
			}

			Transform(flbuf1_r,flbuf1_i,instance->bitrevtab,instance->sintab1,instance->costab,-1,m);	/* in Frequenzdarstellung transformieren */
			Transform(flbuf2_r,flbuf2_i,instance->bitrevtab,instance->sintab1,instance->costab,-1,m);	/* in Frequenzdarstellung transformieren */

			flp1_r=flbuf3_r;flp1_i=flbuf3_i;
			for(o=0;o<=m;o++) *(flp1_r++)=*(flp1_i++)=0.0;		/* löschen */
			for(o=0;o<=m;o++) {
				mb1ptr=flp1_r=flbuf1_r;mb1=0;
				mb2ptr=flp2_r=flbuf2_r;mb2=0;
				for(k=0;k<=m;k++) {
					if(*flp1_r>*mb1ptr) { mb1ptr=flp1_r;mb1=k; }
					flp1_r++;
					if(*flp2_r>*mb2ptr) { mb2ptr=flp2_r;mb2=k; }
					flp2_r++;
				}

				maxb=(UWORD)(maxv=(double)mb1+mor*(double)(mb2-mb1));
				ant1=1.0-(ant2=maxv-(double)maxb);
				mv=(*mb1ptr)+mor*((*mb2ptr)-(*mb1ptr));

				flbuf3_r[maxb]+=(ant1*mv);						/* mit aufteilen */
				if((maxb+1)<=m) flbuf3_r[maxb+1]+=(ant2*mv);
				else flbuf3_r[maxb]+=(ant2*mv);

				*mb1ptr=*mb2ptr=-999999.0;

				mb1ptr=flp1_i=flbuf1_i;mb1=0;
				mb2ptr=flp2_i=flbuf2_i;mb2=0;
				for(k=0;k<=m;k++) {
					if(*flp1_i>*mb1ptr) { mb1ptr=flp1_i;mb1=k; }
					flp1_i++;
					if(*flp2_i>*mb2ptr) { mb2ptr=flp2_i;mb2=k; }
					flp2_i++;
				}

				maxb=(UWORD)(maxv=(double)mb1+mor*(double)(mb2-mb1));
				ant1=1.0-(ant2=maxv-(double)maxb);
				mv=(*mb1ptr)+mor*((*mb2ptr)-(*mb1ptr));

				flbuf3_i[maxb]+=(ant1*mv);						/* mit aufteilen */
				if((maxb+1)<=m) flbuf3_i[maxb+1]+=(ant2*mv);
				else flbuf3_i[maxb]+=(ant2*mv);

				*mb1ptr=*mb2ptr=-999999.0;
			}

			flp1_r=&flbuf3_r[   1];flp1_i=&flbuf3_i[   1];		/* zweite Hälfte spiegeln */
			flp2_r=&flbuf3_r[mm-1];flp2_i=&flbuf3_i[mm-1];
			for(o=1;o<=m;o++) {
				*(flp2_r--)=*(flp1_r++);
				*(flp2_i--)=*(flp1_i++);
			}

			Transform(flbuf3_r,flbuf3_i,instance->bitrevtab,instance->sintab2,instance->costab,1,m);	/* zurücktransformieren */

			flp1_r=flbuf3_r;winfp=win_fkt2;
			orbufr=orbufw;			/* after the loop we will have m2 values ready from here */
			for(o=0;o<m;o++) {		/* Werte zurückschreiben */
				val1=(double)*orbufw+((*(winfp++))*(*(flp1_r++)));
				CLIPFLT(val1);
				*orbufw=(SAMPLE)val1;
				orbufw=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufw+1)&orbufmask));
			}
			orbufw=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufr+m2)&orbufmask));	/* advance writebuffer from start+m2 */
			for(o=0;o<m2;o++) {													/* copy to outputblock and clear rbuf */
				*(dbuf++)=*orbufr;
				*orbufr=0;
				orbufr=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufr+1)&orbufmask));
			}
		}

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->irbufw1=irbufw1;
	instance->irbufw2=irbufw2;
	instance->orbufw=orbufw;
	instance->pos=pos;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->flbuf1_r) 	{ FreeVec((ULONG *)instance->flbuf1_r);instance->flbuf1_r=NULL; }
	if(instance->flbuf1_i)	{ FreeVec((ULONG *)instance->flbuf1_i);instance->flbuf1_i=NULL; }
	if(instance->flbuf2_r) 	{ FreeVec((ULONG *)instance->flbuf2_r);instance->flbuf2_r=NULL; }
	if(instance->flbuf2_i)	{ FreeVec((ULONG *)instance->flbuf2_i);instance->flbuf2_i=NULL; }
	if(instance->flbuf3_r) 	{ FreeVec((ULONG *)instance->flbuf3_r);instance->flbuf3_r=NULL; }
	if(instance->flbuf3_i)	{ FreeVec((ULONG *)instance->flbuf3_i);instance->flbuf3_i=NULL; }
							  DelFFTTabs(instance->bitrevtab,instance->sintab1,instance->sintab2,instance->costab);
							  instance->bitrevtab=NULL;instance->sintab1=instance->sintab2=instance->costab=NULL;
	if(instance->win_fkt1)	{ DelWindowFkt(instance->win_fkt1);instance->win_fkt1=NULL; }
	if(instance->win_fkt2)	{ DelWindowFkt(instance->win_fkt2);instance->win_fkt2=NULL; }
	if(instance->irbuf1)	{ DelRingBuf(instance->irbuf1);instance->irbuf1=NULL; }
	if(instance->irbuf2)	{ DelRingBuf(instance->irbuf2);instance->irbuf2=NULL; }
	if(instance->orbuf)		{ DelRingBuf(instance->orbuf);instance->orbuf=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
