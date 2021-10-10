/****h* SurroundEncoder/SurroundEncoder.c [4.2] *
*
*  NAME
*    SurroundEncoder.c
*  COPYRIGHT
*    $VER: SurroundEncoder.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SurroundEncoder core file
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
/****u* _operators/SurroundEncoder *
*  Function.eng
*    Encodes the audio data of a quadro sample into a stereo sample, which when
*    replayed via a Surround Decoder which regain all its depth.
*  Funktion.ger
*    Kodiert die Sounddaten eines Quadrosamples in ein Stereosample, das über
*    einen Surround-Decoder mit allen Tiefeninformationen abgespielt werden kann.
*  Parameter.eng
*    Surround/Mode
*      Invert is faster, but causes cacelation of signals on some room positions.
*      Phaseshift do not has those problems, but is slower. Windowfunction and number
*      of coefficients is only needed for Mode=Phaseshift.
*    Surround/Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*    Surround/Nr. (PhaseNr)
*      How many coefficients should be used. The more they are, the better results will be get.(max 1024, but 64 is usually enough)
*    Rearfilter
*      Normally the sound which goes to the rear channel gets filtered. Here
*      you can decide if you want this to happen.
*    Rearfilter/Window (<a href="../node03.02.03.html">W2</a>)
*      what windowfunction to use
*    Rearfilter/Nr. (RearNr)
*      How many coefficients should be used. The more they are, the better results will be get.(max 1024, but 64 is usually enough)
*  Parameter.ger
*    Surround/Mode
*      Invert ist schneller, führt aber oft zu Auslöschungen an einigen Raumpositionen.
*      Phaseshift hat diese Probleme nicht, ist aber langsamer. Geben Sie für
*      Phaseshift, die Fensterfunktion und die Anzahl der Filterkoeffizienten an.
*    Surround/Window (<a href="../node03.02.03.html">W1</a>)
*      welche Fensterfunktion verwendet wird.
*    Surround/Nr. (PhaseNr)
*      Wieviele Koeffizienten benutzt werden sollen. Je mehr Koeffizienten benutzt werden,
*      desto besser (max. 1024, 64 ist in der Regel ausreichend)
*    Rearfilter
*      Normalerweise wird der Klang der auf die hinteren Kanäle geht gefiltert.
*      Hier könenen Sie entscheiden, ob sie das wollen.
*    Rearfilter/Window (<a href="../node03.02.03.html">W2</a>)
*      welche Fensterfunktion verwendet wird.
*    Rearfilter/Nr. (RearNr)
*      Wieviele Koeffizienten benutzt werden sollen. Je mehr Koeffizienten benutzt werden,
*      desto besser (max. 1024, 64 ist in der Regel ausreichend)
*  Notes.eng
*    Use e.g. the <a href="Panorama-4Ch.html">Panorama-4Ch</a> operator to generate
*    quadrosamples.
*  Hinweise.ger
*    Benutzen Sie z.B. den <a href="Panorama-4Ch.html">Panorama-4Ch</a> Operator
*    um ein Quadrosample zu erzeugen.
*******
*/

#define SURROUNDENCODER_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "SurroundEncoder.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	LONG j;
	UWORD anz2;
	double frq1,frq2,fg,fgs,fgc,ffs,ffc;
	double si0,si1,fcs;
	double co0,co1,fcc;

	if(instance->mode) {
		if(!(instance->phwin_fkt=NewWindowFkt((instance->phnr<<1),instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
		if(!(instance->phmatrix1=(double *)AllocVec(instance->phnr*sizeof(double),MEMF_ANY))) return(FALSE);
		if(!(instance->phmatrix2=(double *)AllocVec(instance->phnr*sizeof(double),MEMF_ANY))) return(FALSE);
		if(!(instance->r1buf=NewRingBuf(instance->phnr,&instance->r1bufbase,&instance->r1bufmask,&instance->r1bufsize))) return(FALSE);
		// design phaseshifter
		//-----------------------------------------------------------------------------
		//-- Var 1 --------------------------------------------------------------------
		//NewWindowFkt((phnr<<1),instance->wfkt1.win,&instance->wfkt1.par);
		//fgs=PI/2.0;
		//si0=sin(-(fgs+fgs));
		//si1=sin(-fgs);
		//fcs=cos(fgs);fcs+=fcs;
		//for(j=0;j<phnr;) {
			//phmatrix1[j]=fgs*(si0=fcs*si1-si0)*si0*phwin_fkt[phnr+j];j++;
			//phmatrix1[j]=fgs*(si1=fcs*si0-si1)*si1*phwin_fkt[phnr+j];j++;
		//}
		//fgs=PI/-2.0;
		//si0=sin(-(fgs+fgs));
		//si1=sin(-fgs);
		//fcs=cos(fgs);fcs+=fcs;
		//for(j=0;j<phnr;) {
			//phmatrix2[j]=fgs*(si0=fcs*si1-si0)*si0*phwin_fkt[phnr+j];j++;
			//phmatrix2[j]=fgs*(si1=fcs*si0-si1)*si1*phwin_fkt[phnr+j];j++;
		//}
		//-----------------------------------------------------------------------------
		//-- Var 2 --------------------------------------------------------------------
		//m = (1 - n) >> 1;			/* e.g. for n=16 : m=-7...8 */
		//for(i = 0; i < n; i++) {
			//if(m == 0.0) hilb[i] = 0.0;
			//else {
				//f = m * pi;
				//hilb[i] = (1.0 - cos(f)) / f;
			//}
			//m++;
		//}
		//-----------------------------------------------------------------------------
		//-- Var 3 --------------------------------------------------------------------
		anz2=(instance->phnr>>1);
		for(j=-anz2;j<=anz2;j++) {
			if(j) {
				fgs=j*PI;
				instance->phmatrix1[j+anz2]=((1.0-cos(fgs))/fgs)*instance->phwin_fkt[j+anz2];
				fgs=-fgs;
				instance->phmatrix2[j+anz2]=((1.0-cos(fgs))/fgs)*instance->phwin_fkt[j+anz2];
			}
			else instance->phmatrix1[j+anz2]=instance->phmatrix2[j+anz2]=0.0;
		}
		//-----------------------------------------------------------------------------
	}
	if(instance->rearfilt) {
		if(!(instance->rewin_fkt=NewWindowFkt((instance->renr<<1),instance->wfkt2.win,&instance->wfkt2.par))) return(FALSE);
		if(!(instance->rematrix =(double *)AllocVec(instance->renr*sizeof(double),MEMF_ANY))) return(FALSE);
		if(!(instance->r2buf=NewRingBuf(instance->renr,&instance->r2bufbase,&instance->r2bufmask,&instance->r2bufsize))) return(FALSE);
		frq1=  200.0/(double)instance->srat;if(frq1>1.0) frq1=1.0;		/*  100 hz bis */
		frq2=14000.0/(double)instance->srat;if(frq2>1.0) frq2=1.0;		/* 7000 hz */
		fg=(frq2-frq1);fgs=PI*fg*0.5;fgc=PI*(frq2+frq1)*0.5;ffs=ffc=0.001;
		si0=sin(-(fgs+fgs)+ffs);
		si1=sin(-fgs+ffs);
		fcs=cos(fgs);fcs+=fcs;
		co0=cos(-(fgc+fgc)+ffc);
		co1=cos(-fgc+ffc);
		fcc=cos(fgc);fcc+=fcc;
		for(j=0;j<instance->renr;) {
			instance->rematrix[j]=-fg*(co0=fcc*co1-co0)*((si0=fcs*si1-si0)/ffs)*instance->rewin_fkt[instance->renr+j];ffs+=fgs;j++;
			instance->rematrix[j]=-fg*(co1=fcc*co0-co1)*((si1=fcs*si0-si1)/ffs)*instance->rewin_fkt[instance->renr+j];ffs+=fgs;j++;
		}
		instance->rematrix[0]=1.0+instance->rematrix[0];
	}
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	if(instance->mode) {
		memset(instance->r1buf,0,(instance->r1bufsize<<1));
		instance->r1bufw=instance->r1bufbase;		
	}
	if(instance->rearfilt) {
		memset(instance->r2buf,0,(instance->r2bufsize<<1));
		instance->r2bufw=instance->r2bufbase;
	}
}

void process_0(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf0,*dbuf1,*sbuf0,*sbuf1,*sbuf2,*sbuf3;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	sbuf3=instance->sbuf3;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		*(dbuf0++)=(SAMPLE)((double)*(sbuf0++)*0.44+(double)*sbuf2*0.28+(double)*sbuf3*  0.28 );
		*(dbuf1++)=(SAMPLE)((double)*(sbuf1++)*0.44+(double)*sbuf2*0.28+(double)*sbuf3*(-0.28));
		sbuf2++;sbuf3++;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_1(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf0,*dbuf1,*sbuf0,*sbuf1,*sbuf2,*sbuf3;
	UWORD renr;
	SAMPLE *r2bufw,*r2bufr;
	SAMPLE *r2bufbase;
	ULONG r2bufmask;
	double *mat1;
	double sample1;
	UWORD anz2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	sbuf3=instance->sbuf3;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;
	//
	renr=instance->renr;
	r2bufbase=instance->r2bufbase;
	r2bufmask=instance->r2bufmask;
	r2bufw=instance->r2bufw;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		*r2bufw=(SAMPLE)*(sbuf3++);
		r2bufr=(SAMPLE *)((ULONG)r2bufbase|((ULONG)(r2bufw-renr)&r2bufmask));
		sample1=0.0;anz2=renr;										/* apply bandpass filter */
		mat1=instance->rematrix;
		while(anz2--) {
			sample1+=(*(mat1++)*(double)*r2bufr);
			r2bufr=(SAMPLE *)((ULONG)r2bufbase|((ULONG)(r2bufr+1)&r2bufmask));
		}
											/* Center and Rear Channels are coded with -3db levels */
		*(dbuf0++)=(SAMPLE)((double)*(sbuf0++)*0.44+(double)*sbuf2*0.28+sample1*  0.28 );
		*(dbuf1++)=(SAMPLE)((double)*(sbuf1++)*0.44+(double)*sbuf2*0.28+sample1*(-0.28));
		sbuf2++;

		r2bufw=(SAMPLE *)((ULONG)r2bufbase|((ULONG)(r2bufw+1)&r2bufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->r2bufw=r2bufw;
}

void process_2(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf0,*dbuf1,*sbuf0,*sbuf1,*sbuf2,*sbuf3;
	UWORD phnr;
	SAMPLE *r1bufw,*r1bufr;
	SAMPLE *r1bufbase;
	ULONG r1bufmask;
	double *mat1,*mat2;
	double sample1,sample2,fc2;
	UWORD anz2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	sbuf3=instance->sbuf3;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;
	//
	phnr=instance->phnr;
	r1bufbase=instance->r1bufbase;
	r1bufmask=instance->r1bufmask;
	r1bufw=instance->r1bufw;
	fc2=instance->fc2;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		*r1bufw=(SAMPLE)*(sbuf3++);
		r1bufr=(SAMPLE *)((ULONG)r1bufbase|((ULONG)(r1bufw-phnr)&r1bufmask));
		sample1=sample2=0.0;anz2=phnr;								/* apply phaseshifter */
		mat1=instance->phmatrix1;mat2=instance->phmatrix2;
		while(anz2--) {
			sample1+=(*(mat1++)*(double)*r1bufr);
			sample2+=(*(mat2++)*(double)*r1bufr);
			r1bufr=(SAMPLE *)((ULONG)r1bufbase|((ULONG)(r1bufr+1)&r1bufmask));
		}
											/* Center and Rear Channels are coded with -3db levels */
		*(dbuf0++)=(SAMPLE)((double)*(sbuf0++)*0.44+(double)*sbuf2*0.28+sample1*fc2);
		*(dbuf1++)=(SAMPLE)((double)*(sbuf1++)*0.44+(double)*sbuf2*0.28+sample2*fc2);
		sbuf2++;

		r1bufw=(SAMPLE *)((ULONG)r1bufbase|((ULONG)(r1bufw+1)&r1bufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->r1bufw=r1bufw;
}

void process_3(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf0,*dbuf1,*sbuf0,*sbuf1,*sbuf2,*sbuf3;
	UWORD phnr,renr;
	SAMPLE *r1bufw,*r1bufr,*r2bufw,*r2bufr;
	SAMPLE *r1bufbase;
	ULONG r1bufmask;
	SAMPLE *r2bufbase;
	ULONG r2bufmask;
	double *mat1,*mat2;
	double sample1,sample2,fc2;
	UWORD anz2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	sbuf3=instance->sbuf3;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;
	//
	phnr=instance->phnr;
	renr=instance->renr;
	r1bufbase=instance->r1bufbase;
	r1bufmask=instance->r1bufmask;
	r1bufw=instance->r1bufw;
	r2bufbase=instance->r2bufbase;
	r2bufmask=instance->r2bufmask;
	r2bufw=instance->r2bufw;
	fc2=instance->fc2;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		*r2bufw=(SAMPLE)*(sbuf3++);
		r2bufr=(SAMPLE *)((ULONG)r2bufbase|((ULONG)(r2bufw-renr)&r2bufmask));
		sample1=0.0;anz2=renr;										/* apply bandpass filter */
		mat1=instance->rematrix;
		while(anz2--) {
			sample1+=(*(mat1++)*(double)*r2bufr);
			r2bufr=(SAMPLE *)((ULONG)r2bufbase|((ULONG)(r2bufr+1)&r2bufmask));
		}
		*r1bufw=(SAMPLE)sample1;
		r1bufr=(SAMPLE *)((ULONG)r1bufbase|((ULONG)(r1bufw-phnr)&r1bufmask));
		sample1=sample2=0.0;anz2=phnr;								/* apply phaseshifter */
		mat1=instance->phmatrix1;mat2=instance->phmatrix2;
		while(anz2--) {
			sample1+=(*(mat1++)*(double)*r1bufr);
			sample2+=(*(mat2++)*(double)*r1bufr);
			r1bufr=(SAMPLE *)((ULONG)r1bufbase|((ULONG)(r1bufr+1)&r1bufmask));
		}
											/* Center and Rear Channels are coded with -3db levels */
		*(dbuf0++)=(SAMPLE)((double)*(sbuf0++)*0.44+(double)*sbuf2*0.28+sample1*fc2);
		*(dbuf1++)=(SAMPLE)((double)*(sbuf1++)*0.44+(double)*sbuf2*0.28+sample2*fc2);
		sbuf2++;

		r1bufw=(SAMPLE *)((ULONG)r1bufbase|((ULONG)(r1bufw+1)&r1bufmask));
		r2bufw=(SAMPLE *)((ULONG)r2bufbase|((ULONG)(r2bufw+1)&r2bufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->r1bufw=r1bufw;
	instance->r2bufw=r2bufw;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->phwin_fkt)	{ DelWindowFkt(instance->phwin_fkt);instance->phwin_fkt=NULL; }
	if(instance->phmatrix1)	{ FreeVec((ULONG *)instance->phmatrix1);instance->phmatrix1=NULL; }
	if(instance->phmatrix2)	{ FreeVec((ULONG *)instance->phmatrix2);instance->phmatrix2=NULL; }
	if(instance->r1buf)		{ DelRingBuf(instance->r1buf);instance->r1buf=NULL; }
	if(instance->rewin_fkt)	{ DelWindowFkt(instance->rewin_fkt);instance->rewin_fkt=NULL; }
	if(instance->rematrix)	{ FreeVec((ULONG *)instance->rematrix);instance->rematrix=NULL; }
	if(instance->r2buf)		{ DelRingBuf(instance->r2buf);instance->r2buf=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
