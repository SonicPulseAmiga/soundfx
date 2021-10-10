/****h* Echo/Echo.c [4.2] *
*
*  NAME
*    Echo.c
*  COPYRIGHT
*    $VER: Echo.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Echo core file
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
/****u* _operators/Echo *
*  Function.eng
*    Adds echos to the sample
*  Funktion.ger
*    Addiert Echos zu dem Sample
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Delay (<a href="../node03.02.01.html">P3</a>)
*      delaytime for the echos
*    Amplitude (<a href="../node03.02.01.html">P2</a>)
*      the volume of the echos
*    Number
*      the number of echos
*    Ampf
*      final amplification
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      how to calculate (smooth) inbetween data
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Delay (<a href="../node03.02.01.html">P3</a>)
*      modulierbare Verzögerung
*    Amplitude (<a href="../node03.02.01.html">P2</a>)
*      die Lautstärke der Echos
*    Number
*      die Anzahl der Echos
*    Ampf
*      abschließende Lautstärkeanpassung
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      wie sollen (sanfte) Zwischenwerte berechnet werden
*  Notes.eng
*    As SFX mixes the echos to the sample and not only copy them,
*    it's possible that the sample <a href="../node06.03.00.Overdrive.html">overdrives</a>.
*    Choose an amplification-factor smaller than 100 % to avoid the overdrive.<br>
*    With the Echo-Operator you could also simulate hall-rooms. Choose short
*    delay-values for this.<br>
*    And remember; higher number of echos yields longer calculation-times.
*  Hinweise.ger
*    Da SFX die Echosignale einmischt und nicht einfach einsetzt
*    kann es zu <a href="../node06.03.00.Overdrive.html">Übersteuerungen</a> kommen.
*    Der Amplification-Faktor dient dem Abschwächen der eingemischten Werte,
*    so das eine Übersteuerung vermieden wird.<br>
*    Mit dem Echooperator kann auch ein Hallraum simuliert werden. Dazu sollten
*    die Delayzeiten sehr kurz sein.<br>
*    Man sollte bedenken, das höhere Echoanzahl-werte auch längere
*    Berechnungszeiten zur Folge haben.
*******
*/

#define ECHO_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Echo.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	unsigned char k;
	double dels,deld;
	double amps,ampd;
	BShapeFuncPtr modfunc2,modfunc3;
	void *moddata2,*moddata3;

	// Modulator 2
	AssignBShape(&instance->mod2);
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	dels=instance->dels;
	deld=instance->deld;
	// Modulator 3
	AssignBShape(&instance->mod3);
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	amps=instance->amps;
	ampd=instance->ampd;
	
	AssignInterpolFunc(instance->int1.type,&(instance->interpolfunc));	
	if(!(instance->rbuf=NewRingBuf((ULONG)(instance->delay),&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	if(!(instance->amp=(double *)AllocVec((instance->anz*sizeof(double)),MEMF_ANY|MEMF_CLEAR))) return(FALSE);	// amp for each delay
	if(!(instance->del=(double *)AllocVec((instance->anz*sizeof(double)),MEMF_ANY|MEMF_CLEAR))) return(FALSE);	// delaytime for each delay
	for(k=0;k<instance->anz;k++) {					// doing some precalculations
		instance->amp[k]=amps+ampd*modfunc3(k,instance->anz,moddata3);
		instance->del[k]=dels+deld*modfunc2(k,instance->anz,moddata2);					// Version 3
	}
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned char k;
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf,*rbufw,*rbufbase;
	double ampf,sum;
	double *ampptr,*delptr;
	ULONG anz,irange,rbufmask,rbufsize;
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
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	anz=instance->anz;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;

		*rbufw=*sbuf;

		sum=0.0;delptr=instance->del;ampptr=instance->amp;
		for(k=0;k<anz;k++) {
			rbufr=((double)Bytes2Samples((ULONG)rbufw&rbufmask))-(*(delptr++));	/* only Offset to baseaddr */
			if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
			sum+=(interpolfunc(rbufbase,&rbufr,rbufmask,irange)*(*ampptr++));	/* get Value */
		}
		sum*=ampf;
		CLIPFLT(sum);
		*(dbuf++)=(SAMPLE)((double)(*(sbuf++))*dry+sum*eff);
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->rbuf)	{ DelRingBuf(instance->rbuf);instance->rbuf=NULL; }
	if(instance->amp)	{ FreeVec((ULONG *)instance->amp);instance->amp=NULL; }
	if(instance->del)	{ FreeVec((ULONG *)instance->del);instance->del=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
