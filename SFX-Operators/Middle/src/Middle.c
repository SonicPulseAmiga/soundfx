/****h* Middle/Middle.c [4.2] *
*
*  NAME
*    Middle.c
*  COPYRIGHT
*    $VER: Middle.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Middle core file
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
/****u* _operators/Middle *
*  Function.eng
*    Searches for the middle of sample data and centers the sample on the x-axis.
*  Funktion.ger
*    Sucht den Mittelpunkt der Sampledaten und zentriert dieses dann an der x-Achse.
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    Whenever you digitize sound, it can happen that the sample data lies a bit
*    off the x-axis. The middle of the sample just ain't where it should be. On
*    the x-axis. This means that your sample contains an overall offset in its
*    data and should you apply effects to it, can drift away from the middle and
*    at some point become <a href="../node06.03.00.Overdrive.html">overdriven</a> onesidedly.
*    (One half runs up (upper) or down (lower)). This operator prevents this from
*    ever happening again. (Though some of you will overdrive samples a lot at
*    some time, this ain't the cause anymore. It's your fault:)<br>
*    Apart from overdriving this is important for restauration (<a href="DeCrackle.html">DeCrackle</a>, <a href="NoiseGate.html">NoiseGate</a>, ...)
*    so that these operation can correctly analyse the signals.
*  Hinweise.ger
*    Wenn man Töne selbst digitalisiert hat, liegen die Sampledaten oft einwenig
*    neben der x-Achse. Dies bedeutet, daß ein konstanter Gleichspannungsanteil
*    (Offset) auf dem Signal liegt. Hier sollte man diesen Operator anwenden, da
*    sonst das Signal bei einer Weiterverarbeitung immer weiter von der Mitte
*    weggleitet und es dadurch zu einseitigen <a href="../node06.03.00.Overdrive.html">Übersteuerungen</a> kommt.
*    Dieser Operator vermeidet das es dazu kommen kann (Auch wenn manche ihre Samples
*    ordentlich übersteueren, daran liegt es dann nicht mehr. Selber schuld:)<br>
*    Neben dem Übersteuern ist dies auch für Restauration (<a href="DeCrackle.html">DeCrackle</a>, <a href="NoiseGate.html">NoiseGate</a>, ...)
*    sehr wichtig, damit diese Operatoren die Signale richtig analysieren können.
*******
*/

#define MIDDLE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Middle.h"

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
	instance->middle=0.0;
	instance->div1=instance->div2=1.0;
}

void process_pre(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf,*sbuf;
	double middle,div1,div2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	middle=instance->middle;
	div1=instance->div1;
	div2=instance->div2;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		middle=(middle+((double)*(sbuf++))/div1)*(div1/div2);		/* offset (without overflow) */
		div1=div2;div2+=1.0;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->middle=middle;
	instance->div1=div1;
	instance->div2=div2;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf,*sbuf;
	LONG val1,off;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	off=(LONG)instance->middle;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		val1=((LONG)*(sbuf++))-off;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

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
