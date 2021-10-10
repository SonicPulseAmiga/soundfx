/****h* Delay/Delay.c [4.3] *
*
*  NAME
*    Delay.c
*  COPYRIGHT
*    $VER: Delay.c 4.3 (29.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Delay core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    29.Mar.2004    V 4.3   most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/Delay *
*  Function.eng
*    Generates Delays, Echos, Flanger and much more
*  Funktion.ger
*    Erzeugt Verzögerungen, Echos, Flanger und vieles mehr
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Feedback (<a href="../node03.02.01.html">P2</a>)
*      how much of the result is feeback into the operator. This may be negative producing an inverted feedback.
*    Delay (<a href="../node03.02.01.html">P3</a>)
*      modulatable delaytime.
*    Ampf
*      final amplification
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
*    Ampf
*      abschließende Lautstärkeanpassung
*    Dry
*      gibt an, wie das Verhältnis des trockenen Signals aus dem Effect-Parameter berechnet wird.
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      wie sollen (sanfte) Zwischenwerte berechnet werden
*  Notes.eng
*    Short delay values (about 10 ms) are known to put a metallic character to
*    the sample.<br>
*    When a sampled sound ends too abruptley, you can let it fade out with a
*    long delay effect. For this control the feedback by e.g. vector-envelope,
*    which raises the feedback towards the end.
*    In __SFX's Delay you can even modulate the delaytime and you can enter
*    the delaytime as notes. I know it sounds strange, but it makes sense.
*    If you choose a high feedbackpropotion (> 90 %) and set effektpropotion
*    to 100 %, the sample will resonate on the frequency which corresponds to
*    the delaytime. If you enter a 'C-3', __SFX will calculate the right
*    delaytime so that it resonates on that note.<br>
*    And there is another useful application of this operator. If you have a
*    sample containing hum and you know it's frequency,	then choose Dry='Dry=-Eff',
*    Eff=-100 %, Fb=97 % and Delay=<frq>. This will wipeout the frequency and all
*    it's high harmonics. Unfortunately it may take some cycles before the
*    humming fades away. Therefore try to have a bit humming in the begin,
*    which you can just cut later.
*  Hinweise.ger
*    Kurze Delayzeiten (ca. 10 ms) mit hohen Feedbackanteil bewirken einen
*    metallischen Klangcharakter bei dem Sample.<br>
*    Wenn man einen Klang hat, der ziemlich abruppt abbricht, kann man ihn mit
*    einem langen Delay ausklingen lassen. Dafür sollte man z.B. mit einer
*    Vektor-Hüllkurve den Feedbackanteil gegen Ende hochdrehen.
*    In __SFX's Delay können Sie auch die Delayzeit modulieren und diese sogar
*    als Note eingeben. Ich weiß das das erstmal seltsam klingt, aber es macht
*    durchaus Sinn. Wenn Sie einen hohen Feedbackanteil (> 89 %) und einen
*    Effektanteil von 100 % wählen, resoniert das Sample auf der Frequenz, die
*    der Delayzeit entspricht. Wenn Sie dafür nun 'C-3' eingeben, berechnet
*    __SFX die richtige Delayzeit.<br>
*    Und es gibt noch eine nützliche Anwendung für diesen Operator. Wenn Sie ein
*    Sample haben welches brummt und Sie die Frequenz der Störung kennen, wählen
*    Sie Dry='Dry=-Eff', Eff=-100 %, Fb=97 % und Delay=>frq>. Dies wird das
*    Brummen und alle seine	oberen Harmonien auslöschen. Leider dauert es
*    einige Zyclen bis das Brummen leiser wird. Deshalb versuchen Sie bitte
*    ein bischen Brummen am Beginn der Aufnahme zu haben, welches Sie später
*    einfach wegscheiden können.
*******
*/

#define DELAY_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Delay.h"

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

	instance->delay=max((ULONG)instance->dele,(ULONG)instance->dels)+(ULONG)(instance->int1.range);

	if(!(instance->rbuf=NewRingBuf((ULONG)(instance->delay),&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	AssignBShape(&instance->mod3);
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
}

void process_0(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;				// for Modulation parameters
	double fbs,fbd,fb,dr;
	double dels,deld,del;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *rbufbase,*rbufw;
	ULONG irange,rbufmask,rbufsize;
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
	//
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
		fb =fbs +fbd *modfunc2(curlen,instance->slen,moddata2);dr =(fb>=0.0) ? (1.0-fb) : (1.0+fb);
		del=dels+deld*modfunc3(curlen,instance->slen,moddata3);

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
		val2=interpolfunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */

		val1=(val2*eff+(double)*sbuf*dry)*ampf;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;
		*rbufw=(SAMPLE)(val2*fb+(double)*(sbuf++)*dr);
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;

	MSG4("### 0: eff=%6.4f dry=%6.4f  fb=%6.4f dr=6.4f",eff,dry,fb,dr);
}

void process_1(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;				// for Modulation parameters
	double fbs,fbd,fb,dr;
	double dels,deld,del;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *rbufbase,*rbufw;
	ULONG irange,rbufmask,rbufsize;
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
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase=instance->rbufbase;
	rbufsize=instance->rbufsize;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=-eff;
		fb =fbs +fbd *modfunc2(curlen,instance->slen,moddata2);dr =(fb>=0.0) ? (1.0-fb) : (1.0+fb);
		del=dels+deld*modfunc3(curlen,instance->slen,moddata3);

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
		val2=interpolfunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */

		val1=(val2*eff+(double)*sbuf*dry)*ampf;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;
		*rbufw=(SAMPLE)(val2*fb+(double)*(sbuf++)*dr);
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	MSG4("### 1: eff=%6.4f dry=%6.4f  fb=%6.4f dr=6.4f",eff,dry,fb,dr);
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
