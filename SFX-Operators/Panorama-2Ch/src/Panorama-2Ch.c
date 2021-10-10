/****h* Panorama-2Ch/Panorama-2Ch.c [4.2] *
*
*  NAME
*    Panorama-2Ch.c
*  COPYRIGHT
*    $VER: Panorama-2Ch.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Panorama-2Ch core file
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
/****u* _operators/Panaorama-2Ch *
*  Function.eng
*    Distribute a mono-signal between left and right <a href="../node06.03.00.Channel.html">channel</a>.
*  Funktion.ger
*    Verteilt ein Monosample auf den linken und rechten <a href="../node06.03.00.Channel.html">Kanal</a>.
*  Parameter.eng
*    Left-Right Position (<a href="../node03.02.01.html">P1</a>)
*      Propotion for left and right. 0 % (or 0.0) means left and 100 % (or 1.0) right.
*  Parameter.ger
*    Left-Right Position (<a href="../node03.02.01.html">P1</a>)
*      Verhältnis für links und rechts. 0 % (oder 0.0) bedeutet links, 100 % (oder 1.0) rechts.
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define PANORAMA_2CH_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Panorama-2Ch.h"

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

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double lrs,lrd,lr,rl;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf0,*dbuf1,*sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	lrs=instance->lrs;
	lrd=instance->lrd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		lr=lrs+lrd*modfunc(curlen,instance->slen,moddata);rl=1.0-lr;

		*(dbuf0++)=(SAMPLE)((double)* sbuf   *lr);
		*(dbuf1++)=(SAMPLE)((double)*(sbuf++)*rl);

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
