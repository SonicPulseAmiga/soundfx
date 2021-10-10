/****h* ChannelJoin/ChannelJoin.c [4.2] *
*
*  NAME
*    ChannelJoin.c
*  COPYRIGHT
*    $VER: ChannelJoin.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    ChannelJoin core file
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
/****u* _operators/ChannelJoin *
*  Function.eng
*    Joins two separate sample-channels
*  Funktion.ger
*    Verbindet zwei einzelne Samplekanäle
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    The sourcesamples must have the same length and number of <a href="../node06.03.00.Channels.html">channels</a>.
*    Of course only mono and stereo samples are supported.
*  Hinweise.ger
*    Die Ausgangssamples müssen die gleiche Anzahl von <a href="../node06.03.00.Channels.html">Kanälen</a> und die
*    gleiche Länge haben. Es werden natürlich nur Mono- und Stereosamples unterstützt.
*******
*/

#define CHANNELJOIN_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "ChannelJoin.h"

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

void process_m2s(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf0,*dbuf1;
	SAMPLE *sbuf0,*sbuf1;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_s2q(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf0,*dbuf1,*dbuf2,*dbuf3;
	SAMPLE *sbuf0,*sbuf1,*sbuf2,*sbuf3;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	sbuf3=instance->sbuf3;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;
	dbuf2=instance->dbuf2;
	dbuf3=instance->dbuf3;

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
