/****h* Duplicate/Duplicate.c [4.2] *
*
*  NAME
*    Duplicate.c
*  COPYRIGHT
*    $VER: Duplicate.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Duplicate core file
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
/****u* _operators/Duplicate *
*  Function.eng
*    Doubles a sample multiple times
*  Funktion.ger
*    Doppelt ein Sample mehrfach
*  Parameter.eng
*    Rep.
*      Repetitions. How many copies of the sound do you want to have.
*  Parameter.ger
*    Rep.
*      Wiederholungen. Wie viele Kopien des Sounds sie haben möchten.
*  Notes.eng
*    If you got only one period of a waveform (such as most chipsounds) or only
*    cycle of a drum-loop, you can make it longer, by duplicating it several
*    times. This could be neccessary if you want to generate an effect with this sample.
*  Hinweise.ger
*    Wenn sie von einer Wellenform nur eine Periode haben (z.B. ein Chipsound) oder
*    nur einen Durchlauf eines Drumloops, so können Sie	dieses Sample verlängern,
*    indem Sie es mehrfach duplizieren. Dies ist z.B. notwendig, wenn Sie auf das
*    Sample einen Effekt berechnen möchten.
*******
*/

#define DUPLICATE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Duplicate.h"

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
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf,*sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

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
