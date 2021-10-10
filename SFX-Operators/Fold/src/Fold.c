/****h* Fold/Fold.c [4.2] *
*
*  NAME
*    Fold.c
*  COPYRIGHT
*    $VER: Fold.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Fold core file
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
/****u* _operators/Fold *
*  Function.eng
*    Folds the sample data.
*  Funktion.ger
*    Faltet die Samplewerte um
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Amp
*      This parameter tells the operator to hold a constant volume.If deactivated,
*      the amplitude drops for effect-values of 50 % .
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Amp
*      Mit diesem Parameter kann man einstellen, ob die Lautstärke konstant
*      gehalten werden soll. Falls dies ausgeschalten ist, sinkt die Amplitude
*      bei Effektanteilwerten um 50.
*  Notes.eng
*    Please be cautious with the Effect parameter as this operator can seriously
*    change (damage ;-) ) your sample. (but why bother, you can't bust up your
*    source so play away :)
*  Hinweise.ger
*    Hier sollte man vorsichtig mit dem Effektanteil umgehen, da der Operator
*    das Sample gewaltig verändert (zerstört ;-) ) (aber keine Sorge, das
*    Originalsample ist ja noch da).
*******
*/

#define FOLD_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Fold.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod);
}

void process_amp(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;
	LONG val1;
	double ampf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	effs=instance->effs;
	effd=instance->effd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc(curlen,instance->slen,moddata);dry=1.0-eff;

		val1=(LONG)*sbuf;
		if(val1>0) val1=SMP_MAXP-val1;
		else	   val1=SMP_MAXN-val1;
		if(eff>0.5) ampf=1.0+((1.0-eff)*2.0);
		else ampf=1.0+(eff*2.0);
		val1=((double)*(sbuf++)*dry+(double)val1*eff)*ampf;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_noamp(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;
	LONG val1;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	effs=instance->effs;
	effd=instance->effd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc(curlen,instance->slen,moddata);dry=1.0-eff;

		val1=(LONG)*sbuf;
		if(val1>0) val1=SMP_MAXP-val1;
		else	   val1=SMP_MAXN-val1;
		*(dbuf++)=(SAMPLE)((double)*(sbuf++)*dry+(double)val1*eff);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
