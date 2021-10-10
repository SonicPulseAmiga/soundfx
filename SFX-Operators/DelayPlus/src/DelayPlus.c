/****h* DelayPlus/DelayPlus.c [4.2] *
*
*  NAME
*    DelayPlus.c
*  COPYRIGHT
*    $VER: DelayPlus.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    DelayPlus core file
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
/****u* _operators/DelayPlus *
*  Function.eng
*    Generates Delays, Echos, Flanger plus some really wiered fx and much more
*  Funktion.ger
*    Erzeugt Verzögerungen, Echos, Flanger plus einige ziemlich abgefahrene fx und vieles mehr
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Feedback (<a href="../node03.02.01.html">P2</a>)
*      how much of the result is feeback into the operator. This may be negative producing an inverted feedback.
*    Delay (<a href="../node03.02.01.html">P3</a>)
*      modulatable delaytime.
*    Cut-Off (<a href="../node03.02.01.html">P4</a>)
*      The filter cut-off is the frequency where the filter becomes active.
*    Resonance (<a href="../node03.02.01.html">P5</a>)
*      Resonance attenuates the sound around the cut-off frequency. A value of
*      1.0 means no attenuation and higher values will lead to stronger attentuations.
*      If you turn up this too far, that the filter will begin to oszillate (quwiek).
*    Ampf
*      final amplification
*    Type
*      what kind of <a href="../node06.03.00.Filter.html">Filter</a> do you want it to be
*    Dry
*      determines how the propotion of the dry signal from the effect-parameter is calculated
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      how to calculate (smooth) inbetween data
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Feedback (<a href="../node03.02.01.html">P2</a>)
*      wie viel vom Ergebnis in den Operator zurückgeführt wird. Dies kann auch
*      negativ sein um ein inverses Feedback zu erzeugen.
*    Delay (<a href="../node03.02.01.html">P3</a>)
*      modulierbare Verzögerung
*    Cut-Off (<a href="../node03.02.01.html">P4</a>)
*      Die Filter-Eckfrequenz ist die Frequenz an der der Filter aktiv wird.
*    Resonance (<a href="../node03.02.01.html">P5</a>)
*      Resonanz betont den Sound um die Filter-Eckfrequenz herum. 
*      Ein Wert von 1.0 bedeutet keine Betonung und höhere Werte führen zu immer
*      stärkeren Betonung. Wenn Sie das zu weit aufdrehen, wird der Filter
*       beginnen zu oszillieren (quwiek).
*    Ampf
*      abschließende Lautstärkeanpassung
*    Type
*      was für ein <a href="../node06.03.00.Filter.html">Filter</a> soll es den sein
*    Dry
*      gibt an, wie das Verhältnis des trockenen Signals aus dem Effect-Parameter berechnet wird.
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      wie sollen (sanfte) Zwischenwerte berechnet werden
*  Notes.eng
*    see <a href="Delay.html">Delay</a> and <a href="Filter-StateVariable.html">Filter-StateVariable</a>
*  Hinweise.ger
*    siehe <a href="Delay.html">Delay</a> und <a href="Filter-StateVariable.html">Filter-StateVariable</a>
*******
*/

#define DELAYPLUS_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "DelayPlus.h"

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
	if(!(instance->rbuf=NewRingBuf((ULONG)(instance->delay),&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	AssignBShape(&instance->mod3);
	AssignBShape(&instance->mod4);
	AssignBShape(&instance->mod5);
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	instance->low=instance->mid=instance->high=0.0;
}

void process_lowpass(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;				// for Modulation parameters
	double fbs,fbd,fb,dr;
	double dels,deld,del;
	double frqs,frqd,frq;
	double ress,resd,res;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3,modfunc4,modfunc5;
	void *moddata1,*moddata2,*moddata3,*moddata4,*moddata5;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf,*rbufw,*rbufbase;
	ULONG irange,rbufmask,rbufsize;
	double low,mid,high;
	double ampf;
	LONG val1;
	double val2;
	double rbufr;

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
	fbs=instance->fbs;
	fbd=instance->fbd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	dels=instance->dels;
	deld=instance->deld;
	// Modulator 4
	moddata4=(void *)(instance->mod4.bshpdata);
	modfunc4=instance->mod4.modfunc;
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 5
	moddata5=(void *)(instance->mod5.bshpdata);
	modfunc5=instance->mod5.modfunc;
	ress=instance->ress;
	resd=instance->resd;
	//
	low=instance->low;
	mid=instance->mid;
	high=instance->high;
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase=instance->rbufbase;
	rbufsize=instance->rbufsize;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=(1.0-eff);
		fb =fbs +fbd *modfunc2(curlen,instance->slen,moddata2);dr =(fb>=0.0)    ? (1.0-fb) : (1.0+fb);
		del=dels+deld*modfunc3(curlen,instance->slen,moddata3);
		frq=frqs+frqd*modfunc4(curlen,instance->slen,moddata4);
		res=ress+resd*modfunc5(curlen,instance->slen,moddata5);

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
		val2=interpolfunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */

		val1=(val2*eff+(double)*sbuf*dry)*ampf;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		val2=val2*fb+(double)*(sbuf++)*dr;							/* feedback */
		high=val2-(mid*res)-low;									/* filter value */
		mid+=(high*frq);low+=(mid*frq);
		val1=(low*sqrt(sqrt(res)));
		CLIPINT(val1);
		*rbufw=(SAMPLE)val1;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
	instance->rbufw=rbufw;
}

void process_highpass(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;				// for Modulation parameters
	double fbs,fbd,fb,dr;
	double dels,deld,del;
	double frqs,frqd,frq;
	double ress,resd,res;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3,modfunc4,modfunc5;
	void *moddata1,*moddata2,*moddata3,*moddata4,*moddata5;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf,*rbufw,*rbufbase;
	ULONG irange,rbufmask,rbufsize;
	double low,mid,high;
	double ampf;
	LONG val1;
	double val2;
	double rbufr;

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
	fbs=instance->fbs;
	fbd=instance->fbd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	dels=instance->dels;
	deld=instance->deld;
	// Modulator 4
	moddata4=(void *)(instance->mod4.bshpdata);
	modfunc4=instance->mod4.modfunc;
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 5
	moddata5=(void *)(instance->mod5.bshpdata);
	modfunc5=instance->mod5.modfunc;
	ress=instance->ress;
	resd=instance->resd;
	//
	low=instance->low;
	mid=instance->mid;
	high=instance->high;
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase=instance->rbufbase;
	rbufsize=instance->rbufsize;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=(1.0-eff);
		fb =fbs +fbd *modfunc2(curlen,instance->slen,moddata2);dr =(fb>=0.0)    ? (1.0-fb) : (1.0+fb);
		del=dels+deld*modfunc3(curlen,instance->slen,moddata3);
		frq=frqs+frqd*modfunc4(curlen,instance->slen,moddata4);
		res=ress+resd*modfunc5(curlen,instance->slen,moddata5);

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
		val2=interpolfunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */

		val1=(val2*eff+(double)*sbuf*dry)*ampf;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		val2=val2*fb+(double)*(sbuf++)*dr;							/* feedback */
		high=val2-(mid*res)-low;									/* filter value */
		mid+=(high*frq);low+=(mid*frq);
		val1=(high*sqrt(sqrt(res)));
		CLIPINT(val1);
		*rbufw=(SAMPLE)val1;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
	instance->rbufw=rbufw;
}

void process_bandpass(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;				// for Modulation parameters
	double fbs,fbd,fb,dr;
	double dels,deld,del;
	double frqs,frqd,frq;
	double ress,resd,res;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3,modfunc4,modfunc5;
	void *moddata1,*moddata2,*moddata3,*moddata4,*moddata5;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf,*rbufw,*rbufbase;
	ULONG irange,rbufmask,rbufsize;
	double low,mid,high;
	double ampf;
	LONG val1;
	double val2;
	double rbufr;

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
	fbs=instance->fbs;
	fbd=instance->fbd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	dels=instance->dels;
	deld=instance->deld;
	// Modulator 4
	moddata4=(void *)(instance->mod4.bshpdata);
	modfunc4=instance->mod4.modfunc;
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 5
	moddata5=(void *)(instance->mod5.bshpdata);
	modfunc5=instance->mod5.modfunc;
	ress=instance->ress;
	resd=instance->resd;
	//
	//
	low=instance->low;
	mid=instance->mid;
	high=instance->high;
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase=instance->rbufbase;
	rbufsize=instance->rbufsize;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=(1.0-eff);
		fb =fbs +fbd *modfunc2(curlen,instance->slen,moddata2);dr =(fb>=0.0)    ? (1.0-fb) : (1.0+fb);
		del=dels+deld*modfunc3(curlen,instance->slen,moddata3);
		frq=frqs+frqd*modfunc4(curlen,instance->slen,moddata4);
		res=ress+resd*modfunc5(curlen,instance->slen,moddata5);

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
		val2=interpolfunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */

		val1=(val2*eff+(double)*sbuf*dry)*ampf;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		val2=val2*fb+(double)*(sbuf++)*dr;							/* feedback */
		high=val2-(mid*res)-low;									/* filter value */
		mid+=(high*frq);low+=(mid*frq);
		val1=mid*sqrt(sqrt(res));
		CLIPINT(val1);
		*rbufw=(SAMPLE)val1;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
	instance->rbufw=rbufw;
}

void process_bandstop(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;				// for Modulation parameters
	double fbs,fbd,fb,dr;
	double dels,deld,del;
	double frqs,frqd,frq;
	double ress,resd,res;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3,modfunc4,modfunc5;
	void *moddata1,*moddata2,*moddata3,*moddata4,*moddata5;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf,*rbufw,*rbufbase;
	ULONG irange,rbufmask,rbufsize;
	double low,mid,high;
	double ampf;
	LONG val1;
	double val2;
	double rbufr;

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
	fbs=instance->fbs;
	fbd=instance->fbd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	dels=instance->dels;
	deld=instance->deld;
	// Modulator 4
	moddata4=(void *)(instance->mod4.bshpdata);
	modfunc4=instance->mod4.modfunc;
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 5
	moddata5=(void *)(instance->mod5.bshpdata);
	modfunc5=instance->mod5.modfunc;
	ress=instance->ress;
	resd=instance->resd;
	//
	low=instance->low;
	mid=instance->mid;
	high=instance->high;
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase=instance->rbufbase;
	rbufsize=instance->rbufsize;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=(1.0-eff);
		fb =fbs +fbd *modfunc2(curlen,instance->slen,moddata2);dr =(fb>=0.0)    ? (1.0-fb) : (1.0+fb);
		del=dels+deld*modfunc3(curlen,instance->slen,moddata3);
		frq=frqs+frqd*modfunc4(curlen,instance->slen,moddata4);
		res=ress+resd*modfunc5(curlen,instance->slen,moddata5);

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
		val2=interpolfunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */

		val1=(val2*eff+(double)*sbuf*dry)*ampf;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		val2=val2*fb+(double)*(sbuf++)*dr;							/* feedback */
		high=val2-(mid*res)-low;									/* filter value */
		mid+=(high*frq);low+=(mid*frq);
		val1=(low+high)*sqrt(sqrt(res));
		CLIPINT(val1);
		*rbufw=(SAMPLE)val1;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
	instance->rbufw=rbufw;
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
