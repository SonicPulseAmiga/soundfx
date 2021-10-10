/****h* Slide/Slide.c [4.2] *
*
*  NAME
*    Slide.c
*  COPYRIGHT
*    $VER: Slide.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Slide core file
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
/****u* _operators/Slide *
*  Function.eng
*    Does a vertical slide to the sample data.
*  Funktion.ger
*    Verschiebt die Samplewerte vertikal.
*  Parameter.eng
*    Distance (<a href="../node03.02.01.html">P1</a>)
*      Value by which the data will slide. Negative values slide down , positive up.
*    Wrap
*      Choose how to handle clipping. The modi below are available :
*        NoClip: don't care
*        Clip : obverdriven values are clipped
*        Wrap1 : overdriven values are pushed into the opposite side until they don't clip anymore.
*        Wrap2 : overdriven values are overturned (folded) until they don't clip anymore.
*  Parameter.ger
*    Distance (<a href="../node03.02.01.html">P1</a>)
*      Wert, um den das Sample verschoben werden soll. Negative Werte ergeben eine
*      Verschiebung nach unten, positive Werte eine nach oben.
*    Wrap
*      Gibt an, wie eine mögliche Übersteuerung des Signals behandelt werden soll.
*      Hierbei gibt es 4 Modi : 
*        NoClip: es wird nicht auf Übersteuerte Werte gestestet.
*        Clip : die übersteuerten Werte werden auf Maximum bzw. Minimum gesetzt
*        Wrap1 : der übersteuerte Anteil wird an der anderen Seite wieder hereingeschoben, und zwar solange, bis er komplett im Normalbereich ist.
*        Wrap2 : der übersteuerte Anteil wird solange an der Ober- und Unterkante umgeklappt, bis er komplett im Normalbereich ist.
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define SLIDE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Slide.h"

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

void process_noclip(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double slides,slided;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;
	LONG slide;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	slides=instance->slides;
	slided=instance->slided;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		slide=(LONG)(slides+slided*modfunc(curlen,instance->slen,moddata));

		*(dbuf++)=(SAMPLE)(*(sbuf++)+slide);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_clip(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double slides,slided;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;
	LONG val1;
	LONG slide;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	slides=instance->slides;
	slided=instance->slided;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		slide=(LONG)(slides+slided*modfunc(curlen,instance->slen,moddata));

		val1=((LONG)*(sbuf++))+slide;
		if     (val1>SMP_MAXP) val1=SMP_MAXP;
		else if(val1<SMP_MAXN) val1=SMP_MAXN;
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_wrap1(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double slides,slided;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;
	LONG val1;
	LONG slide;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	slides=instance->slides;
	slided=instance->slided;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		slide=(LONG)(slides+slided*modfunc(curlen,instance->slen,moddata));

		val1=((LONG)*(sbuf++))+slide;
		if     (val1>SMP_MAXP) val1=((val1-SMP_MAXN)&SMP_MAXMASK)+SMP_MAXN;
		else if(val1<SMP_MAXN) val1=((val1-SMP_MAXP)&SMP_MAXMASK)+SMP_MAXP;
		*(dbuf++)=(SAMPLE)val1;
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_wrap2(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double slides,slided;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;
	LONG val1;
	LONG slide;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	slides=instance->slides;
	slided=instance->slided;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		slide=(LONG)(slides+slided*modfunc(curlen,instance->slen,moddata));

		val1=((LONG)*(sbuf++))+slide;
		while(val1>SMP_MAXP || val1<SMP_MAXN) {
			if     (val1>SMP_MAXP) val1=SMP_MAXP-(val1-SMP_MAXP);
			else if(val1<SMP_MAXN) val1=SMP_MAXN-(val1-SMP_MAXN);
		}
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
