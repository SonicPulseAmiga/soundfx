/****h* Shorten/Shorten.c [4.2] *
*
*  NAME
*    Shorten.c
*  COPYRIGHT
*    $VER: Shorten.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Shorten core file
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
/****u* _operators/Shorten *
*  Function.eng
*    Optimizes the samplelength.
*  Funktion.ger
*    Optimiert die Samplelänge.
*  Parameter.eng
*    Threshold
*      The operator cuts the sample from begin and end until the amplitude peaks
*      over the theshold. This level can be adjusted separately for start and
*      end of the sample.
*  Parameter.ger
*    Threshold
*      Schwellwert. Der Operator kürzt das Sample von Begin und Ende bis die
*      Amplitude den Schwellwert übersteigt. Diesen können sie separat für Start
*      und Ende des Samples einstellen.
*  Notes.eng
*    With 8-bit samples it will be less successful as with 16-bit samples,
*    because the last ones have a larger amplitude range.
*  Hinweise.ger
*    Bei 8-bit Samples wird der Erfolg nicht so hoch sein, wie bei 16-bit Samples,
*    weil bei letztern der Wertebereich einfach feiner ist.
*******
*/

#define SHORTEN_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Shorten.h"

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
	instance->rngst=0;
	instance->rnget=instance->slen;
	instance->sscan=instance->escan=TRUE;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf,*sbuf;
	ULONG rngst,rnget;
	UBYTE sscan,escan;
	LONG tvals,tvale,aval;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	tvals=instance->tvals;
	tvale=instance->tvale;
	sscan=instance->sscan;
	escan=instance->escan;
	rngst=instance->rngst;
	rnget=instance->rnget;
	
	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		aval=labs(*sbuf);
		if(aval>=tvals) {									/* cur value is larger than treshold */
			if(sscan) { rngst=curlen;sscan=FALSE; }				/* erster Peak */
			rnget=curlen;escan=TRUE;							/* doch nicht letzter Peak gewesen */
		}
		if(aval<=tvale) {									/* cur value is smaller than treshold */
			if(escan) { rnget=curlen;escan=FALSE; }				/* ev. nach letztem peak */
		}
		sbuf++;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->sscan=sscan;
	instance->escan=escan;
	instance->rngst=rngst;
	instance->rnget=rnget;
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
