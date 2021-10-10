/****h* ChorusPhaser/ChorusPhaser.c [4.2] *
*
*  NAME
*    ChorusPhaser.c
*  COPYRIGHT
*    $VER: ChorusPhaser.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    ChorusPhaser core file
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
/****u* _operators/ConvertChannels *
*  Function.eng
*    Mixes the sample with several slightly detuned and delayed variations of itself, to produce fat sounds
*  Funktion.ger
*    Mischt das Sample mit mehreren leicht verstimmten und verzögerten Variationen von sich selbst, um breite Klänge zu erzeugen.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Voice1...4 (<a href="../node03.02.01.html">P2...P5</a>)
*      modulated delaytime.
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      how to calculate (smooth) inbetween data
*    Ampf
*      final amplification
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Voice1...4 (<a href="../node03.02.01.html">P2...P5</a>)
*      modulierte Verzögerung
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      wie sollen (sanfte) Zwischenwerte berechnet werden
*    Ampf
*      abschließende Lautstärkeanpassung
*  Notes.eng
*    Drumloops can give interesting results as they continuously
*    get treblier :) and darker.<br>
*    Futhermore it is effective to apply the fx to long sustained pad-sounds or vocals.
*    These getting more depth by that treatment.
*  Hinweise.ger
*    Sehr interessante Ergebnisse erhält man bei Drumloops. Diese
*    werden kontinuierlich verstimmt - werden dumpfer und heller.<br>
*    Weiterhin ist die Anwendung auf langanhaltende Synthesizerklänge oder Vokalsamples
*    zu empfehlen. Diese bekommen dadurch mehr Tiefe.
*******
*/

#define CHORUSPHASER_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "ChorusPhaser.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	double tdelay;

	AssignInterpolFunc(instance->int1.type,&(instance->interpolfunc));

	instance->maxdelay=max(instance->del1s,instance->del1e);
	tdelay=max(instance->del2s,instance->del2e);instance->maxdelay=max(instance->maxdelay,tdelay);
	tdelay=max(instance->del3s,instance->del3e);instance->maxdelay=max(instance->maxdelay,tdelay);
	tdelay=max(instance->del4s,instance->del4e);instance->maxdelay=max(instance->maxdelay,tdelay);
	instance->maxdelay+=(ULONG)(instance->int1.range);

	if(!(instance->rbuf=NewRingBuf((ULONG)(instance->maxdelay),&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
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
	instance->rblen=(double)Bytes2Samples(instance->rbufsize);
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	double del1s,del1d,del1;
	double del2s,del2d,del2;
	double del3s,del3d,del3;
	double del4s,del4d,del4;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3,modfunc4,modfunc5;
	void *moddata1,*moddata2,*moddata3,*moddata4,*moddata5;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf,*rbufw,*rbufbase;
	double rbufr,rblen;
	LONG val1;
	double val2,ampf;
	ULONG irange,rbufmask;

	curlen=instance->curlen;

	// Source, Dest., RingBuf
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	rbufw=instance->rbufw;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	del1s=instance->del1s;
	del1d=instance->del1d;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	del2s=instance->del2s;
	del2d=instance->del2d;
	// Modulator 4
	moddata4=(void *)(instance->mod4.bshpdata);
	modfunc4=instance->mod4.modfunc;
	del3s=instance->del3s;
	del3d=instance->del3d;
	// Modulator 5
	moddata5=(void *)(instance->mod5.bshpdata);
	modfunc5=instance->mod5.modfunc;
	del4s=instance->del4s;
	del4d=instance->del4d;
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	rblen=instance->rblen;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff =effs +effd *modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		del1=del1s+del1d*modfunc2(curlen,instance->slen,moddata2);
		del2=del2s+del2d*modfunc3(curlen,instance->slen,moddata3);
		del3=del3s+del3d*modfunc4(curlen,instance->slen,moddata4);
		del4=del4s+del4d*modfunc5(curlen,instance->slen,moddata5);

		*rbufw=*sbuf;

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del1;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=rblen;
		val2=interpolfunc(rbufbase,&rbufr,rbufmask,irange);				/* get Value */

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del2;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=rblen;
		val2+=interpolfunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del3;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=rblen;
		val2+=interpolfunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del4;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=rblen;
		val2+=interpolfunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */

		val1=(val2*0.25*eff+(double)*(sbuf++)*dry)*ampf;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
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
