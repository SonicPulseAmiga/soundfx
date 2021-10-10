/****h* SampleJoin/SampleJoin.c [4.2] *
*
*  NAME
*    SampleJoin.c
*  COPYRIGHT
*    $VER: SampleJoin.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SampleJoin core file
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
/****u* _operators/SampleJoin *
*  Function.eng
*    Appends one sample to the end of the other
*  Funktion.ger
*    Hängt ein Sample an ein Anderes.
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    The sourcesamples should have the same number of channels.
*  Hinweise.ger
*    Die Ausgangssamples sollten die gleiche Kanalanzahl haben.
*******
*/

#define SAMPLEJOIN_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "SampleJoin.h"

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
	instance->dpos=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf,*sbuf;
	ULONG dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	dpos=instance->dpos;

	for(i=0;((i<samples) && (dpos<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		*(dbuf++)=*(sbuf++);dpos++;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->dpos=dpos;
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
