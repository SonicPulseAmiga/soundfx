/****h* Noise/Noise.c [4.2] *
*
*  NAME
*    Noise.c
*  COPYRIGHT
*    $VER: Noise.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Noise core file
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
/****u* _operators/Noise *
*  Function.eng
*    Generates coloured noise
*  Funktion.ger
*    Generiert gefärbtes Rauschen
*  Parameter.eng
*    Minimum Change (<a href="../node03.02.01.html">P1</a>)
*      minimum level change from one sample to the next one
*    Maximum Change (<a href="../node03.02.01.html">P2</a>)
*      maximum level change from one sample to the next one
*    SLen
*      length of noise
*    SRat
*      sampling rate of the sample. Can be entered as rate, note or with the <a href="../node01.05.04.html">period-window</a>.
*  Parameter.ger
*    Minimum Change (<a href="../node03.02.01.html">P1</a>)
*      minimale Pegeländerung von einem zum nächsten Samplewert
*    Maximum Change (<a href="../node03.02.01.html">P2</a>)
*      maximale Pegeländerung von einem zum nächsten Samplewert
*    SLen
*      Länge des Rauschens
*    SRat
*      Samplingrate des Samples. Diese kann als Rate direkt oder als Note eingegeben
*      werden bzw. im <a href="../node01.05.04.html">Periodenauswahlfenster</a> angewählt werden.
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define NOISE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Noise.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	srand(time(NULL));			/* init the random-number-generator */
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	instance->val1=0;
	instance->w=(DYN_RNG>>1)-1;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double mis,mid,mi;
	double mas,mad,ma;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf,*sbuf;
	BYTE w;
	LONG val1,diff;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	mis=instance->mis;
	mid=instance->mid;
	// Modulator 1
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	mas=instance->mas;
	mad=instance->mad;
	//
	val1=instance->val1;
	w=instance->w;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		mi=mis+mid*modfunc1(curlen,instance->slen,moddata1);
		ma=mas+mad*modfunc2(curlen,instance->slen,moddata2);
		diff=(LONG)(ma-mi);if(diff<1) diff=1;

		if((rand()%DYN_RNG)>w) {
			val1=val1+(mi+rand()%diff);
			if(val1>SMP_MAXP) {			/* positive clipping */
				val1=SMP_MAXP;
				if(w<(DYN_RNG-DYN_RNGS)) w+=DYN_RNGS;		/* Warscheinlichkeit, das naechster Wert negativ wird hoch */
				else w=DYN_RNG-1;
			}
			if(w<(DYN_RNG-1)) w++;
		}
		else {
			val1=val1-(mi+rand()%diff);
			if(val1<SMP_MAXN) {			/* negative clipping */
				val1=SMP_MAXN;
				if(w>(DYN_RNGS-1)) w-=DYN_RNGS;				/* Warscheinlichkeit, das naechster Wert positiv wird hoch */
				else w=0;
			}
			if(w>0) w--;
		}
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->val1=val1;
	instance->w=w;
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
