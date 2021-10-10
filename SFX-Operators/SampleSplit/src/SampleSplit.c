/****h* SampleSplit/SampleSplit.c [4.2] *
*
*  NAME
*    SampleSplit.c
*  COPYRIGHT
*    $VER: SampleSplit.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SampleSplit core file
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
/****u* _operators/SampleSplit *
*  Function.eng
*    Splits one sample at certain positions.
*  Funktion.ger
*    Trennt ein Sample an bestimmten Stellen.
*  Parameter.eng
*    Pos
*      where should it be sliced
*    GrabMark
*      get the Splitpos from current range
*    Splits
*      how many slices
*  Parameter.ger
*    Pos
*      wo soll gesplittet werden
*    GrabMark
*      lies die Splitpos vom aktuellen Bereich
*    Splits
*      wie viele Unterteilungen
*  Notes.eng
*    If you want to slice e.g. a drumloop, then use e.g. Pos=25 % and
*    Splits=3. You'll get 4 samples then.
*    The Pos parameter denotes the size of one slice. The last slice will
*    contain all the remaining bits though.
*  Hinweise.ger
*    Wenn man z.B. einen Drumloop zerschneiden möchte, sagt man z.B.
*    Pos=25 % und Splits=3. Man erhält dann 4 Samples.
*    Der Pos Parameter gib die Größe eines Segmentes an, das letzte
*    Segment wird jedoch den verbleibenden Rest haben.
*******
*/

#define SAMPLESPLIT_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "SampleSplit.h"

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
	instance->spos=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *dbuf,*sbuf;
	ULONG spos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	spos=instance->spos;

	for(i=0;((i<samples) && (spos>0) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		*(dbuf++)=*(sbuf++);spos--;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->spos=spos;
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
