/****h* Detune/Detune.c [4.2] *
*
*  NAME
*    Detune.c
*  COPYRIGHT
*    $VER: Detune.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Detune core file
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
/****u* _operators/Detune *
*  Function.eng
*    Detunes a sample (modulated resampling)
*  Funktion.ger
*    Verstimmt ein Sample (Modulierbares Resampling)
*  Parameter.eng
*    Factor (<a href="../node03.02.01.html">P1</a>)
*      Pitchfactor. A value of 2.0 means, your result is one octave higher (twice as high higher).
*      The sample will be shortened by the same factor as well.
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      how to calculate (smooth) inbetween data
*  Parameter.ger
*    Factor (<a href="../node03.02.01.html">P1</a>)
*      Tonhöhenfactor. Ein Wert von 2.0 bedeutet, daß das Sample eine Oktave höher (doppelt so hoch) klingt.
*      Das Sample wird dabei auch um den gleichen Faktor verkürzt.
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      wie sollen (sanfte) Zwischenwerte berechnet werden
*  Notes.eng
*    In this operator pitch and length are coupled. If you want to change only the pitch,
*    have a look at the <a href="PitchShift.html">PitchShift</a> operator and if you want to change the length,
*    try the <a href="TimeStretch">TimeStretch</a> operator.
*  Hinweise.ger
*    In diesem Operator sind Tonhöhe und Länge aneinander gekoppelt. Wenn Sie nur
*    die Tonhöhe ändern wollen, schauen Sie sich den <a href="PitchShift.html">PitchShift</a> Operator
*    an und wenn sie Länge ändern möchten, probieren sie den <a href="TimeStretch">TimeStretch</a> Operator.
*******
*/

#define DETUNE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Detune.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	AssignInterpolFunc(instance->int1.type,&(instance->interpolfunc));	
	if(!(instance->rbuf=NewRingBuf((ULONG)(instance->int1.range)<<2,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	// get the REAL result lenght
	if(instance->mod.bshape==BSHP_NONE) {
		instance->rlen=instance->slen/instance->factors;
	}
	else {
		unsigned long i;
		double rptr=0.0;
		double factors,factord;
		BShapeFuncPtr modfunc;
		void *moddata;
		ULONG tlen;

		// Modulator
		moddata=(void *)(instance->mod.bshpdata);
		modfunc=instance->mod.modfunc;
		factors=instance->factors;
		factord=instance->factord;
		
		AssignBShape(&instance->mod);
		instance->rlen=0;
		tlen=instance->slen/min(factors,instance->factore);		// longest possible sample
		for(i=0;i<tlen;i++)	{
			rptr+=(factors+factord*modfunc((ULONG)rptr,instance->slen,moddata));
			if((ULONG)rptr>instance->slen) { instance->rlen=i;break; }
		}
		if(instance->rlen==0) instance->rlen=tlen;
	}
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod);
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	instance->rptr=0.0;
	instance->spos=instance->dpos=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double factors,factord;
	BShapeFuncPtr modfunc;
	void *moddata;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf,*rbufw,*rbufbase;
	double rbufr,rptr,rptrs;
	ULONG irange,spos,dpos,rbufmask,rbufsize;
	UBYTE inc=FALSE;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	factors=instance->factors;
	factord=instance->factord;
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	rptr=instance->rptr;
	spos=instance->spos;
	dpos=instance->dpos;

	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		if((rptr+irange)>=(double)curlen) {
			*rbufw=*(sbuf++);spos--;
			inc=TRUE;
		}

		while(((rptr+irange)<(double)curlen) && (dpos<samples)) {
			rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-((double)curlen-rptr);	/* only Offset to baseaddr */
			if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
			// DEBUG
			if(rbufr>Bytes2Samples(rbufsize)) ERR2("rbufr>rbufsize %lf>%lf",rbufr,Bytes2Samples(rbufsize));
			// DEBUG
			*(dbuf++)=interpolfunc(rbufbase,&rbufr,rbufmask,irange);dpos++;
			rptrs=(factors+factord*modfunc((ULONG)rptr,instance->slen,moddata));
			if(rptrs<0.0001) rptrs=0.0001;	/* can't be negative */
			rptr+=rptrs;
		}

		if(inc) {
			rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
			curlen++;instance->curlen++;
			inc=FALSE;
		}

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->rptr=rptr;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->spos=spos;
	instance->dpos=dpos;
}

void process_rest(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double factors,factord;
	BShapeFuncPtr modfunc;
	void *moddata;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*rbufw,*rbufbase;
	double rbufr,rptr,rptrs;
	ULONG irange,dpos,rbufmask,rbufsize;
	UBYTE inc=FALSE;

	curlen=instance->curlen;

	// Source, Dest.
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	factors=instance->factors;
	factord=instance->factord;
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	rptr=instance->rptr;
	dpos=instance->dpos;

	while((rptr<(double)curlen) && (dpos<samples) && (instance->curlen<=instance->slen)) {
		// fill interpolrbuf with 0's
		if((rptr+irange)>=(double)curlen) {
			*rbufw=0;
			inc=TRUE;
		}

		while((rptr<(double)curlen) && (dpos<samples)) {
			rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-((double)curlen-rptr);	// only Offset to baseaddr
			if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
			// DEBUG
			if(rbufr>Bytes2Samples(rbufsize)) ERR2("rbufr>rbufsize %lf>%lf",rbufr,Bytes2Samples(rbufsize));
			// DEBUG
			*(dbuf++)=interpolfunc(rbufbase,&rbufr,rbufmask,irange);dpos++;
			rptrs=(factors+factord*modfunc((ULONG)rptr,instance->slen,moddata));
			if(rptrs<0.0001) rptrs=0.0001;	/* can't be negative */
			rptr+=rptrs;
		}

		if(inc) {
			rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
			//curlen++;instance->curlen++;
			inc=FALSE;
		}
	}
	instance->rbufw=rbufw;
	instance->rptr=rptr;
	instance->dbuf=dbuf;
	instance->dpos=dpos;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->rbuf)	{ DelRingBuf(instance->rbuf);instance->rbuf=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
