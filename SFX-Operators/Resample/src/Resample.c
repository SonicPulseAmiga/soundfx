/****h* Resample/Resample.c [0.0] *
*
*  NAME
*    Resample.c
*  COPYRIGHT
*    $VER: Resample.c 0.0 (31.08.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Resample core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    31.Aug.2004    V 0.0   most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/Resample *
*  Function.eng
*    Changes the <a href="../node06.03.00.Samplingrate.html">sampling-rate</a> and sample length while preserving the original sound.
*  Funktion.ger
*    Ändert <a href="../node06.03.00.Samplingrate.html">Samplingrate</a> und Samplelänge bei gleichbleibendem Klang.
*  Parameter.eng
*    SLen old
*      old sample length
*    SLen new
*      new sample length. Factor and new rate are calculated and entered
*    SRat old
*      old sampling-rate
*    SRat new
*      new sampling-rate. Factor and new length are	calculated and entered
*    Factor
*      factor of change in length and rate. A factor of 1.0 changes nothing.
*    Lock
*      Determines which parameter should be locked. If you want e.g. resample several
*      samples with different rates all to the same rate, you would choose "SRat" then.
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      how to calculate (smooth) inbetween data
*    Aliasing Filter
*      If enabled, the sound gets filtered before resampling. This is important when
*      lowering the sampling-rate.
*  Parameter.ger
*    SLen old
*      alte Samplelänge
*    SLen new
*      neue Samplelänge. Der Faktor und die neue Rate werden berechnet und eingetragen.
*    SRat old
*      alte Samplingrate
*    SRat new
*      neue Samplingrate. Der Faktor und die neue Länge werden berechnet und eingetragen.
*    Factor
*      Der Änderungsfaktor für Rate und Länge. Ein Faktor gleich 1.0 ändert nix.
*    Lock
*      Bestimmt welcher Parameter festgehalten wird. Wenn man z.B. mehrere Samples mit
*      unterschiedlichen Raten auf ein Rate bringen möchte, wählt man "SRat" aus.
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      wie sollen (sanfte) Zwischenwerte berechnet werden
*    Aliasing Filter
*      Wenn aktiviert, wird das Sample erst gefinltert und dann resampled. Dies ist wichtig
*      wenn die Samplingrate niedriger wird.
*  Notes.eng
*    When you sample a sound and use it in a music program it is sometimes out of tune.
*    This operator can correct this. For this you'll have to enter the playback rate as for
*    example the rate you hear the note "C" on with this sample. Now you enter "Resample"
*    and set the resampling rate to "C" -> 16780  and generate the new sample. The new
*    sample will play a "C" at the correct rate now.<br>
*    With "Resample" it is also possible to change the length of the sample for when you
*    want to modulate something with this sample and need to get to a correct length to
*    do so. Use interpolation with this so that the waveform won't gets too "edgy".
*  Hinweise.ger
*    Wenn man einen Klang digitalisiert hat und dieser im Musikprogramm verstimmt klingt,
*    kann man dies hier korregieren. Dazu stellt man die Rate ein bei der man z.B.
*    ein "C" hört, jetzt ruft man Resample auf und stellt die Rate für das "C" ein
*    (z.B. C-3 -> 16780). Danach hört man bei dieser Rate das "C".<br>
*    Hiermit kann man auch Wellenformen, die zum Modulieren eines Parameters verwendet
*    werden sollen, optimal in ihrer Länge anpassen. Dabei sollte man Interpolation
*    einschalten, damit die Wellenform erhalten bleibt und nicht eckig wird.
*******
*/

#define RESAMPLE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Resample.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	instance->frq=0.95*instance->factor;		// for aliasfilter with some safety
	instance->step=1.0/instance->factor;		// step

	AssignInterpolFunc(instance->int1.type,&(instance->interpolfunc));
	if(instance->aliasflt) {
		unsigned short j;
		double frq=instance->frq;
		double fg,ff,fc;
		double si0,si1,ampf;

		if(instance->anz&1) instance->anz++;		// make it an even number
		if(instance->anz<2) instance->anz=2;		// limmit to range
		if(instance->anz>1024) instance->anz=1024;

		if(!(instance->win_fkt=NewWindowFkt((instance->anz<<1),instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
		if(!(instance->matrix =(double *)AllocVec(instance->anz*sizeof(double),MEMF_ANY))) return(FALSE);
		if(!(instance->r1buf	=NewRingBuf(instance->anz,&instance->r1bufbase,&instance->r1bufmask,&instance->r1bufsize))) return(FALSE);
		fg=frq;frq*=PI;ff=0.001;
		si0=sin(-(frq+frq)+ff);
		si1=sin(-frq+ff);
		fc=cos(frq);fc+=fc;
		ampf=0.0;
		for(j=0;j<instance->anz;) {
			ampf+=(instance->matrix[j]=fg*((si0=fc*si1-si0)/ff)*instance->win_fkt[instance->anz+j]);
			ff+=frq;j++;
			ampf+=(instance->matrix[j]=fg*((si1=fc*si0-si1)/ff)*instance->win_fkt[instance->anz+j]);
			ff+=frq;j++;
		}
		instance->ampf=1.0/ampf;
	}
	if(!(instance->r2buf=NewRingBuf((((ULONG)(instance->int1.range))<<2),&instance->r2bufbase,&instance->r2bufmask,&instance->r2bufsize))) return(FALSE);
	return(TRUE);	
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	memset(instance->r2buf,0,(instance->r2bufsize<<1));
	instance->r2bufw=instance->r2bufbase;
	instance->rpos=0;
	instance->rptr=0.0;
	if(instance->aliasflt) {
		memset(instance->r1buf,0,(instance->r1bufsize<<1));
		instance->r1bufw=instance->r1bufbase;
	}
	instance->spos=instance->dpos=0;
}

void process_flt(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *r1bufw,*r1bufr,*r1bufbase;
	ULONG r1bufmask;
	SAMPLE *r2bufw,*r2bufbase;
	ULONG r2bufmask,r2bufsize;
	double r2bufr;
	ULONG irange,spos,dpos,rpos;
	double *mat;
	double ampf,sample;
	UWORD anz,anz2;
	double rptr;							// readpos
	double step;
	UBYTE inc=FALSE;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	r1bufbase=instance->r1bufbase;
	r2bufbase=instance->r2bufbase;
	r1bufmask=instance->r1bufmask;
	r2bufmask=instance->r2bufmask;
	r2bufsize=instance->r2bufsize;
	r1bufw=instance->r1bufw;
	r2bufw=instance->r2bufw;
	spos=instance->spos;
	dpos=instance->dpos;
	step=instance->step;
	rpos=instance->rpos;
	rptr=instance->rptr;
	ampf=instance->ampf;
	anz=instance->anz;

	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		if((rptr+irange)>=(double)curlen) {
			*r1bufw=*(sbuf++);spos--;
			curlen++;instance->curlen++;
			inc=TRUE;
	
			sample=0.0;anz2=anz;									/* apply filter */
			r1bufr=(SAMPLE *)((ULONG)r1bufbase|((ULONG)(r1bufw-anz)&r1bufmask));
			mat=instance->matrix;
			while(anz2--) {
				sample+=(*(mat++)*(double)*r1bufr);
				r1bufr=(SAMPLE *)((ULONG)r1bufbase|((ULONG)(r1bufr+1)&r1bufmask));
			}
			sample*=ampf;
			CLIPFLT(sample);
			*r2bufw=(SAMPLE)sample;
		}

		while(((rptr+irange)<(double)curlen) && (dpos<samples)) {
			r2bufr=(double)Bytes2Samples((ULONG)r2bufw&r2bufmask)-((double)curlen-rptr);	/* only Offset to baseaddr */
			if(r2bufr<0.0) r2bufr+=(double)Bytes2Samples(r2bufsize);
			*(dbuf++)=interpolfunc(r2bufbase,&r2bufr,r2bufmask,irange);dpos++;rpos++;
			rptr=(double)rpos*step;
		}
		if(inc) {
			r1bufw=(SAMPLE *)((ULONG)r1bufbase|((ULONG)(r1bufw+1)&r1bufmask));
			r2bufw=(SAMPLE *)((ULONG)r2bufbase|((ULONG)(r2bufw+1)&r2bufmask));
		}
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->r1bufw=r1bufw;
	instance->r2bufw=r2bufw;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->spos=spos;
	instance->dpos=dpos;
	instance->rpos=rpos;
	instance->rptr=rptr;
}

void process_noflt(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *r2bufw,*r2bufbase;
	ULONG r2bufmask,r2bufsize;
	double r2bufr;
	ULONG irange,spos,dpos,rpos;
	double rptr;							// readpos
	double step;
	UBYTE inc=FALSE;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	r2bufbase=instance->r2bufbase;
	r2bufmask=instance->r2bufmask;
	r2bufsize=instance->r2bufsize;
	r2bufw=instance->r2bufw;
	spos=instance->spos;
	dpos=instance->dpos;
	step=instance->step;
	rpos=instance->rpos;
	rptr=instance->rptr;

	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		if((rptr+irange)>=(double)curlen) {
			*r2bufw=*(sbuf++);spos--;
			curlen++;instance->curlen++;
			inc=TRUE;
		}

		while(((rptr+irange)<(double)curlen) && (dpos<samples)) {
			r2bufr=(double)Bytes2Samples((ULONG)r2bufw&r2bufmask)-((double)curlen-rptr);	/* only Offset to baseaddr */
			if(r2bufr<0.0) r2bufr+=(double)Bytes2Samples(r2bufsize);
			*(dbuf++)=interpolfunc(r2bufbase,&r2bufr,r2bufmask,irange);dpos++;rpos++;
			rptr=(double)rpos*step;
		}

		if(inc) {
			r2bufw=(SAMPLE *)((ULONG)r2bufbase|((ULONG)(r2bufw+1)&r2bufmask));
		}
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->r2bufw=r2bufw;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->spos=spos;
	instance->dpos=dpos;
	instance->rpos=rpos;
	instance->rptr=rptr;
}

void process_rest(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf;
	SAMPLE *r2bufw,*r2bufbase;
	ULONG r2bufmask,r2bufsize;
	double r2bufr;
	ULONG irange,dpos,rpos;
	double rptr;							// readpos
	double step;
	UBYTE inc=FALSE;

	curlen=instance->curlen;

	// Source, Dest.
	dbuf=instance->dbuf;
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	r2bufbase=instance->r2bufbase;
	r2bufmask=instance->r2bufmask;
	r2bufsize=instance->r2bufsize;
	r2bufw=instance->r2bufw;
	dpos=instance->dpos;
	step=instance->step;
	rpos=instance->rpos;
	rptr=instance->rptr;

	while((dpos<samples) && (instance->curlen<instance->slen)) {
		// fill interpolrbuf with 0's
		if((rptr+irange)>=(double)curlen) {
			*r2bufw=0;
			inc=TRUE;
		}

		while((rptr<(double)curlen) && (dpos<samples)) {
			r2bufr=(double)Bytes2Samples((ULONG)r2bufw&r2bufmask)-((double)curlen-rptr);	// only Offset to baseaddr
			if(r2bufr<0.0) r2bufr+=(double)Bytes2Samples(r2bufsize);
			*(dbuf++)=interpolfunc(r2bufbase,&r2bufr,r2bufmask,irange);dpos++;rpos++;
			rptr=(double)rpos*step;
		}

		if(inc) {
			r2bufw=(SAMPLE *)((ULONG)r2bufbase|((ULONG)(r2bufw+1)&r2bufmask));
		}
	}
	instance->r2bufw=r2bufw;
	instance->dbuf=dbuf;
	instance->dpos=dpos;
	instance->rpos=rpos;
	instance->rptr=rptr;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->win_fkt)	{ DelWindowFkt(instance->win_fkt);instance->win_fkt=NULL; }
	if(instance->matrix)	{ FreeVec((ULONG *)instance->matrix);instance->matrix=NULL; }
	if(instance->r2buf)		{ DelRingBuf(instance->r2buf);instance->r2buf=NULL; }
	if(instance->r1buf)		{ DelRingBuf(instance->r1buf);instance->r1buf=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
