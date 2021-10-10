/****h* Smear/Smear.c [4.3] *
*
*  NAME
*    Smear.c
*  COPYRIGHT
*    $VER: Smear.c 4.3 (23.11.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Smear core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    23.Nov.2003	V 4.3	most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/Smear *
*  Function.eng
*    The readout position of the sample data is <a href="../node06.03.00.Modulation.html">modulated</a>
*    and the resulting values get mixed into the original data.
*  Funktion.ger
*    <a href="../node06.03.00.Modulation.html">Moduliert</a> die Ausleseposition der Sampledaten und mischt die
*    gelesenen Werte auf die Orignalwerte.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Smear Range (<a href="../node03.02.01.html">P2</a>)
*      how far to shift position
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      how to calculate (smooth) inbetween data
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Smear Range (<a href="../node03.02.01.html">P2</a>)
*      wie stark die Position verschoben werden soll
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      wie sollen (sanfte) Zwischenwerte berechnet werden
*  Notes.eng
*    The range shouldn't be too big as that rarely produces nice effects.<br>
*    Normaly you should modulate the Smear Range-parameter.
*  Hinweise.ger
*    Man sollte den Bereich für die Verschiebung nicht zu groß wählen (dürfte nur
*    selten gut klingen).<br>
*    Normalerweise sollte der Smear Range-Parameter Sample moduliert werden.
*******
*/

#define SMEAR_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Smear.h"

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
	if(!(instance->rbuf=NewRingBuf((ULONG)instance->del,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	return(TRUE);	
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	instance->spos=instance->dpos=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double effs,effd,eff,dry;
	double rngs,rngd,rng;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf,*rbufw;
	LONG val1,val2;
	SAMPLE *rbufbase;
	ULONG off,rbufmask,rbufsize;
	double rbufr;
	ULONG irange,spos,dpos;

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
	rngs=instance->rngs;
	rngd=instance->rngd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	spos=instance->spos;
	dpos=instance->dpos;
	off=instance->off;
	rng=instance->rng;

	/** @todo: use real dbuf.len instead of samples */
	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=*(sbuf++);
		if(curlen>=off) {
			eff=effs+effd*modfunc1((ULONG)(curlen-off),instance->slen,moddata1);dry=1.0-eff;
			rng=rngs+rngd*modfunc2((ULONG)(curlen-off),instance->slen,moddata2);
			
			rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-(double)off;		/* only Offset to baseaddr */
			if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
			val1=interpolfunc(rbufbase,&rbufr,rbufmask,irange);					/* get Value */

			rbufr=((double)Bytes2Samples((ULONG)rbufw&rbufmask)-(double)off)+rng;	/* only Offset to baseaddr */
			while(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
			val2=interpolfunc(rbufbase,&rbufr,rbufmask,irange);					/* get Value */

			*(dbuf++)=(SAMPLE)((double)val1*dry+(double)val2*eff);dpos++;
		}
		spos--;curlen++;instance->curlen++;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->spos=spos;
	instance->dpos=dpos;
}

void process_rest(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double effs,effd,eff,dry;
	double rngs,rngd,rng;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*rbufw;
	LONG val1,val2;
	SAMPLE *rbufbase;
	ULONG off,rbufmask,rbufsize;
	double rbufr;
	ULONG irange,dpos;

	curlen=instance->curlen;

	// Source, Dest.
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	rngs=instance->rngs;
	rngd=instance->rngd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	dpos=instance->dpos;
	off=instance->off;
	rng=instance->rng;

	/** @todo: use real dbuf.len instead of samples */
	while((dpos<samples) && (instance->rest>0)) {
		*rbufw=0;
		eff=effs+effd*modfunc1((ULONG)(curlen-off),instance->slen,moddata1);dry=1.0-eff;
		rng=rngs+rngd*modfunc2((ULONG)(curlen-off),instance->slen,moddata2);

		rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-(double)off;		/* only Offset to baseaddr */
		if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
		val1=interpolfunc(rbufbase,&rbufr,rbufmask,irange);					/* get Value */

		rbufr=((double)Bytes2Samples((ULONG)rbufw&rbufmask)-(double)off)+rng;	/* only Offset to baseaddr */
		while(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
		val2=interpolfunc(rbufbase,&rbufr,rbufmask,irange);					/* get Value */

		*(dbuf++)=(SAMPLE)((double)val1*dry+(double)val2*eff);dpos++;

		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		instance->rest--;
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
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
