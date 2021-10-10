/****h* Subtract/Subtract.c [4.2] *
*
*  NAME
*    Subtract.c
*  COPYRIGHT
*    $VER: Subtract.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Subtract core file
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
/****u* _operators/Subtract *
*  Function.eng
*    Subtracts the sample data of 2.sample from 1.sample
*  Funktion.ger
*    Subtrahiert die Sampledaten des 2. Samples vom 1. Sample
*  Parameter.eng
*    Delay Source 2
*      delays the source 2
*  Parameter.ger
*    Delay Source 2
*      verzögert Source 2
*  Notes.eng
*    If both samples are identical and the delay time is 0 the result is an empty sample.<br>
*    You can use this effect to determine the change a previous actio has made. Apply an
*    effect then subtract the original from the effect sample. The result is the pure
*    effect signal. An interesting applicatiohn for this is to see what gets lost when using
*    compression 8such as mp3) when saving sounds. Just reload the sample after saving and
*    subtract the compressed from the original.
*  Hinweise.ger
*    Bei gleichen Puffern und einem Delay von 0 kommt es zur totalen Auslöschung.<br>
*    Diesen Effekt kann man auch verwenden um herauszufinden welche Änderungen eine
*    vorherige Aktion auf das Sample hatte. Wenden sie einen Effekt auf ein Sample an
*    und subtrahieren sie dann das Original von dem Effekt-Sample. Das Ergebnis
*    ist das reine Effekt-Signal. Eine interessante Anwendung dafür ist herauszubekommen
*    was beim Speichern mit kompression (wie mp3) verloren geht. Laden sie das Sample
*    nach dem Speichern einfach wieder ein und subtrahieren sie das komprimierte vom
*    originalen Sample.  
*******
*/

#define SUBTRACT_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Subtract.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	if(!(instance->rbuf=NewRingBuf(instance->delay2,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	memset(instance->rbufbase,0,instance->rbufsize);
	instance->rbufw=instance->rbufbase;
	instance->rbufr=(SAMPLE *)((ULONG)instance->rbufbase|(instance->rbufsize-Samples2Bytes(instance->delay2)));
}

void process_0(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf,*sbuf1,*sbuf2;
	
	curlen=instance->curlen;

	// Source, Dest.
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	dbuf=instance->dbuf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*(dbuf++)=(SAMPLE)((double)*(sbuf1++)*0.5-(double)*(sbuf2++)*0.5);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_1(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf,*sbuf1,*sbuf2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	dbuf=instance->dbuf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*(dbuf++)=(SAMPLE)((double)*(sbuf1++)*0.5);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_2(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf,*sbuf1,*sbuf2;
	SAMPLE *rbufw,*rbufr;
	SAMPLE *rbufbase;
	ULONG rbufmask;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	dbuf=instance->dbuf;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufr=instance->rbufr;
	rbufw=instance->rbufw;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*(dbuf++)=(SAMPLE)((double)*(sbuf1++)*0.5+(double)*rbufr*0.5);
		*rbufw=*(sbuf2++);
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufr=rbufr;
	instance->rbufw=rbufw;
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
