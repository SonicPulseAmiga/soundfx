/****h* Crackle/Crackle.c [4.2] *
*
*  NAME
*    Crackle.c
*  COPYRIGHT
*    $VER: Crackle.c 4.2 (06.08.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Crackle core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    06.Aug.2003
*  MODIFICATION HISTORY
*    06.Aug.2003	V 4.2	most recent version
*    06.Aug.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/Crackle *
*  Function.eng
*    add crackle to a sample
*  Funktion.ger
*    fügt einem Sample Knackser hinzu
*  Parameter.eng
*    Crackle Density
*      How many crackles should be added
*  Parameter.ger
*    Crackle Density
*      wie viele Knackser hinzugefügt werden sollen
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define CRACKLE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Crackle.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	srand(time(NULL));
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod);
	instance->density_counter=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;	
	double crds,crdd,crd;
	ULONG density_counter;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;
	LONG val;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	crds=instance->crds;
	crdd=instance->crdd;
	// Parameters
	density_counter=instance->density_counter;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++,density_counter++) {
		val=*(sbuf++);
		//-- add crackles (currently only spikes of width=1)
		/* @TODO better would be two crackle loops (many quiet + a few loud) */
		crd=crds+crdd*modfunc(curlen,instance->slen,moddata);
		if(density_counter>(ULONG)crd) {
			// set current sample to 0 or +/- MAX
			if(rand()&1) val=0;
			else val=(val>0)?SMP_MAXP:SMP_MAXN;
			density_counter=0;
		}
		CLIPINT(val);
		*(dbuf++)=(SAMPLE)val;
	}
	instance->density_counter=density_counter;
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