/****h* QuantizeHoriz/QuantizeHoriz.c [4.2] *
*
*  NAME
*    QuantizeHoriz.c
*  COPYRIGHT
*    $VER: QuantizeHoriz.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    QuantizeHoriz core file
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
/****u* _operators/QuantizeHoriz *
*  Function.eng
*    "Holds" the sample values for a given time.
*  Funktion.ger
*    Hält die Samplewerte für eine angegebene Zeit
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Quantisation Range (<a href="../node03.02.01.html">P2</a>)
*      how long a sample value is supposed to be held
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Quantisation Range (<a href="../node03.02.01.html">P2</a>)
*      wie lange ein Samplewert gehalten werden soll
*  Notes.eng
*    This effect gives a sample a "Nintendo" sound. It's also well known as Sample&amp;Hold.
*  Hinweise.ger
*    Dieser Effekt verleiht dem Sample einen typischen "Nintendo"-Klang. Er ist auch
*    unter dem Namen Sample&amp;Hold bekannt.
*******
*/

#define QUANTIZEHORIZ_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "QuantizeHoriz.h"

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
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	instance->len=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	ULONG  lens,lend,len;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf,*sbuf;
	double val1;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	lens=instance->lens;
	lend=instance->lend;
	//
	len=instance->len;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;

		if(!len) {
			val1=(double)*sbuf;
			len=lens+lend*modfunc2(curlen,instance->slen,moddata2);
			/** @idea support fratcional length */
		}
		*(dbuf++)=(SAMPLE)((double)*(sbuf++)*dry+val1*eff);
		len--;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->len=len;
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
