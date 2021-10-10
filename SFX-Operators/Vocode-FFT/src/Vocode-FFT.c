/****h* Vocode-FFT/Vocode-FFT.c [4.2] *
*
*  NAME
*    Vocode-FFT.c
*  COPYRIGHT
*    $VER: Vocode-FFT.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Vocode-FFT core file
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
/****u* _operators/Vocode-FFT *
*  Function.eng
*    Forces the sources2 (modulator) to "sing" with the sound of source1 (carrier).
*  Funktion.ger
*    Zwingt Source2 (modulator) mit dem Klang von Source1 (carrier) zu "singen".
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Bands
*      Just how many bands should SFX use. Less Bands means less math, but you lose out on accuracy.
*    Steps
*      SFX does a transformation every <steps> sample-values. Lower values mean better quallity but
*      longer calculation. Steps can no be bigger than the half of the number of bands.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*    Ampf
*      final amplification
*    EAmpf
*      amplification for the envelopefollower
*    EFCoef
*      factor for the inertia of the envelope follower. Meaningful values are ranging
*      from 0.8 to 1.0.
*    Src2Inv
*      Should I flip the envelope for the modulator (src2) (loud becomes quiet and reversed).
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Bands
*      Wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
*      die Frequenzauflösung ist dann aber auch nicht so fein.
*    Steps
*      Aller wieviel Samples eine Transformation erstellt werden soll. Je öfters diese berechnet
*      werden, desto genauer das Ergebnis und desto 	höher auch die Rechenzeit. Steps darf max. halb
*      so groß wie die Nummer der Bänder sein.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      welche Fensterfunktion verwendet wird.
*    Ampf
*      abschließende Lautstärkeanpassung
*    EFCoef
*      Faktor für die Trägkeit des Hüllkurvenverfolgers. Sinnvolle Werte reichen
*      von 0.8 bis 1.0.
*    Src2Inv
*      Soll ich die Hüllkurve des Modulators (src2) umdrehen (aus laut wird leise und umgekehrt).
*  Notes.eng
*    Sources should be of high quality. They should be rich with high tones, as
*    the result might otherwise sound too "thin".<br>
*    In some cases the result seems to be empty. Use Amplify with MaxVol to bring
*    the sample full volume or recalculate the sample with higher Ampf- and EAmpf-values.<br>
*    Speech samples as Source2 and synth-sounds as Source1 produce good results.<br>
*    This operator uses the <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
*    for its calculations.
*  Hinweise.ger
*    Für die Sources sollte man hochquallitative Samples nehmen. Die Klänge
*    sollten weiterhin reich an Obertönen sein, da das Ergebnis sonst zu "dünn"
*    klingt.<br>
*    In einigen Fällen presentiert sich das Ergebnis als scheinbar leeres Sample.
*    Benutzen Sie Amplify mit MaxVol um es auf volle Lautstärke zu bringen oder
*    berechen Sie es nochmal und erhöhen dabei die Ampf- und EAmpf-werte.<br>
*    Gute Resultate erhält man mit Sprachsamples als Source2 und synthetischen
*    Klänge als Source1 (Reihenfolge ist wichtig).<br>
*    Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a> verwendet.
*******
*/

#define VOCODE_FFT_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Vocode-FFT.h"

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
	if(!(instance->win_fkt1	=NewWindowFkt(instance->mm,instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
	if(!(instance->win_fkt2	=NewWindowFkt(instance->m,WIN_BARTLETT,NULL))) return(FALSE);
	if(!					 NewFFTTabs(&instance->bitrevtab,&instance->sintab1,&instance->sintab2,&instance->costab,instance->m)) return(FALSE);
	if(!(instance->irbuf1	=NewRingBuf(instance->mm,&instance->irbufbase1,&instance->irbufmask,&instance->irbufsize))) return(FALSE);
	if(!(instance->irbuf2	=NewRingBuf(instance->mm,&instance->irbufbase2,&instance->irbufmask,&instance->irbufsize))) return(FALSE);
	if(!(instance->orbuf	=NewRingBuf(instance->mm,&instance->orbufbase ,&instance->orbufmask,&instance->orbufsize))) return(FALSE);
	if(!(instance->ampenv	=(double *)AllocVec((instance->m+1)*sizeof(double),MEMF_ANY))) return(FALSE);

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
	memset(instance->ampenv,0,((instance->m+1)*sizeof(double)));	// clear maxval in every pass
	instance->pos=(instance->m2-1);			// weil curlen erst später erhöht wird
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned short o;
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf1,*sbuf2;
	double eampf;
	UWORD m,mm,m2;
	double aktval,efc1,efc2;
	double *flbuf1_r,*flbuf1_i,*flbuf2_r,*flbuf2_i;
	double *flp1_r,*flp1_i,*flp2_r,*flp2_i;
	double *win_fkt1,*win_fkt2,*winfp;
	double *ampenv;
	SAMPLE *irbufbase1;
	SAMPLE *irbufbase2;
	ULONG irbufmask;
	SAMPLE *orbufbase;
	ULONG orbufmask;
	SAMPLE *irbufr1,*irbufw1,*irbufr2,*irbufw2,*orbufr,*orbufw;
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
	effs=instance->effs;
	effd=instance->effd;
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
	win_fkt1=instance->win_fkt1;
	win_fkt2=instance->win_fkt2;
	ampenv=instance->ampenv;
	eampf=instance->eampf;
	efc1=instance->efc1;
	efc2=instance->efc2;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc(curlen,instance->slen,moddata);dry=1.0-eff;

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
				/**(flp1_i++)=*(flp2_i++)=0.0; */
				irbufr1=(SAMPLE *)((ULONG)irbufbase1|((ULONG)(irbufr1+1)&irbufmask));
				irbufr2=(SAMPLE *)((ULONG)irbufbase2|((ULONG)(irbufr2+1)&irbufmask));
			}

			Transform(flbuf1_r,flbuf1_i,instance->bitrevtab,instance->sintab1,instance->costab,-1,m);	/* in Frequenzdarstellung transformieren */
			Transform(flbuf2_r,flbuf2_i,instance->bitrevtab,instance->sintab1,instance->costab,-1,m);	/* in Frequenzdarstellung transformieren */
																					/* Envelope Follower */	
			if(instance->src2inv) aktval=(1.0-sqrt(flbuf2_r[0]*flbuf2_r[0]+flbuf2_i[0]*flbuf2_i[0]))*eampf;
			else				 aktval=     sqrt(flbuf2_r[0]*flbuf2_r[0]+flbuf2_i[0]*flbuf2_i[0]) *eampf;
			if(aktval>ampenv[0]) ampenv[0]=aktval;
			else ampenv[0]=efc1*ampenv[0]+efc2*aktval;
			flbuf1_r[0]*=ampenv[0];													/* Vocoding */
			flbuf1_i[0]*=ampenv[0];

			for(o=1;o<=m;o++) {														/* Offset */
				aktval=sqrt(flbuf2_r[o]*flbuf2_r[o]+flbuf2_i[o]*flbuf2_i[o])*eampf;	/* Envelope Follower */
				if(aktval>ampenv[o]) ampenv[o]=aktval;
				else ampenv[o]=efc1*ampenv[o]+efc2*aktval;
				flbuf1_r[mm-o]=(flbuf1_r[o]*=ampenv[o]);							/* Vocoding */
				flbuf1_i[mm-o]=(flbuf1_i[o]*=ampenv[o]);
			}

			Transform(flbuf1_r,flbuf1_i,instance->bitrevtab,instance->sintab2,instance->costab,1,m);		/* zurücktransformieren */

			flp1_r=flbuf1_r;winfp=win_fkt2;
			orbufr=orbufw;			/* after the loop we will have m2 values ready from here */
			irbufr2=(SAMPLE *)((ULONG)irbufbase2|((ULONG)(irbufw2-m2)&irbufmask));	/* take out the last m2 values */
			for(o=0;o<m;o++) {		/* Werte zurückschreiben */
				val1=(double)*orbufw+((*(winfp++))*(*(flp1_r++)));
				CLIPFLT(val1);
				*orbufw=(SAMPLE)((double)(*irbufr2)*dry+val1*eff);
				orbufw =(SAMPLE *)((ULONG)orbufbase |((ULONG)(orbufw +1)&orbufmask));
				irbufr2=(SAMPLE *)((ULONG)irbufbase2|((ULONG)(irbufr2+1)&irbufmask));
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
							  DelFFTTabs(instance->bitrevtab,instance->sintab1,instance->sintab2,instance->costab);
							  instance->bitrevtab=NULL;instance->sintab1=instance->sintab2=instance->costab=NULL;
	if(instance->win_fkt1)	{ DelWindowFkt(instance->win_fkt1);instance->win_fkt1=NULL; }
	if(instance->win_fkt2)	{ DelWindowFkt(instance->win_fkt2);instance->win_fkt2=NULL; }
	if(instance->irbuf1)	{ DelRingBuf(instance->irbuf1);instance->irbuf1=NULL; }
	if(instance->irbuf2)	{ DelRingBuf(instance->irbuf2);instance->irbuf2=NULL; }
	if(instance->orbuf)		{ DelRingBuf(instance->orbuf);instance->orbuf=NULL; }
	if(instance->ampenv)	{ FreeVec((ULONG *)instance->ampenv);instance->ampenv=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
