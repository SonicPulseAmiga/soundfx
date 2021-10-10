/****h* NoiseGate/NoiseGate.c [4.2] *
*
*  NAME
*    NoiseGate.c
*  COPYRIGHT
*    $VER: NoiseGate.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    NoiseGate core file
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
/****u* _operators/NoiseGate *
*  Function.eng
*    Fades parts which are quieter than the threshold out.
*  Funktion.ger
*    Blendet Stellen welche leiser als der Schwellwert sind aus..
*  Parameter.eng
*    Threshold (<a href="../node03.02.01.html">P1</a>)
*      Amplitude which serves as a threshold for starting to fade out
*    Attack
*      the sound will not just get muted, it be be faded out and back in.
*    Shape
*      type of the fade
*  Parameter.ger
*    Threshold (<a href="../node03.02.01.html">P1</a>)
*      Amplitude die als Schwellwert für das Ausblenden dient
*    Attack
*      Ansprechdauer; der Sound wird nicht abgehackt sonden aus- und eingeblendet.
*    Shape
*      Hüllform, wie übergeblendet werden soll.
*  Notes.eng
*    Can be used with solo recordings (e.g. speech, guitar, ...) that contain
*    noisy pauses.<br>
*    For percussive material I recommend using shorter attack values (e.g. 0.5 ms),
*    otherwise the attack can be a bit longer (e.g. 1.0 ms).<br>
*    Bevor man diesen Operator nutzt empfiehlt es sich, erst den
*    <a href="Middle.html">Middle</a> Operator anzuwenden, um das Sample vorzubereiten.<br>
*    Remember this operator a lot more effective with real 16-bit samples
*    than with 8-bit samples.
*  Hinweise.ger
*    Der Operator kann bei Soloaufnahmen (z.B. Sprache, Guitarre, ..) angewandt
*    werden, deren Pausen verrauscht sind.<br>
*    Bei percussiven Material empfehle ich kurzere Attackwerte (z.B. 0.5 ms),
*    sonst kann der Attack auch mal etwas länger sein (z.B. 1.0 ms).<br>
*    Bevor man diesen Operator nutzt empfiehlt es sich, erst den
*    <a href="Middle.html">Middle</a> Operator anzuwenden, um das Sample vorzubereiten.<br>
*    Bedenken sie immer, dass dieser Operator bei echten 16 bit Samples
*    besser wirkt, als bei 8 bit Samples.
*******
*/

#define NOISEGATE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "NoiseGate.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	unsigned long j;

	switch(instance->shape) {
		case CURVE_LINEAR:		instance->expon=1.0;break;
		case CURVE_SPEEDUP:		instance->expon=2.0;break;
		case CURVE_SLOWDOWN:	instance->expon=0.5;break;
	}

	if(!(instance->fadearr=(double *)AllocVec((instance->attack+1)*sizeof(double),MEMF_ANY))) return(FALSE);
	instance->fadearr[0]=0.0;
	for(j=1;j<=instance->attack;j++) instance->fadearr[j]=mypow(((double)j/(double)instance->attack),instance->expon);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod);

	instance->fop=instance->attack-1;	// =att2
	instance->fo=0;
	instance->ampenv=0.0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double thrs,thrd,thr;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;
	ULONG att2;
	double sample,amp,ampenv,*fadearr;
	ULONG fop;
	UBYTE fo;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	thrs=instance->thrs;
	thrd=instance->thrd;
	//
	att2=instance->attack-1;
	ampenv=instance->ampenv;
	fop=instance->fop;
	fo=instance->fo;
	fadearr=instance->fadearr;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		thr=thrs+thrd*modfunc(curlen,instance->slen,moddata);

		sample=*(sbuf++);amp=fabs(sample);
		if(amp>ampenv) ampenv=amp;						/* envelope tracker */
		else ampenv=ENVFC1*ampenv+ENVFC2*amp;
		if(ampenv<thr) {								/* < Schwellwert -> */
			if(!fo) { fop=att2;fo=1; }						/* Ausfaden initialisieren */
			sample=(sample*fadearr[fop]);
			if(fop) fop--;
		}
		else fo=0;										/* >= Schwellwert -> Ausfaden aus */
		*(dbuf++)=(SAMPLE)sample;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->ampenv=ampenv;
	instance->fop=fop;
	instance->fo=fo;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->fadearr) { FreeVec((ULONG *)instance->fadearr);instance->fadearr=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
