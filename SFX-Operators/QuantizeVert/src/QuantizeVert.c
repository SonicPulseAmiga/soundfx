/****h* QuantizeVert/QuantizeVert.c [4.2] *
*
*  NAME
*    QuantizeVert.c
*  COPYRIGHT
*    $VER: QuantizeVert.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    QuantizeVert core file
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
/****u* _operators/QuantizeVert *
*  Function.eng
*    Brings down the bit resolution of the sample
*  Funktion.ger
*    Kürzt die Bitauflösung des Samples
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Quantisation Range (<a href="../node03.02.01.html">P2</a>)
*      to how many bits is the sample to be scaled
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Quantisation Range (<a href="../node03.02.01.html">P2</a>)
*      Auf wieviel Bit runtergerechnet werden soll
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define QUANTIZEVERT_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "QuantizeVert.h"

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
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	LONG  divs,divd,div;
	BShapeFuncPtr modfunc1,modfunc2;
	APTR moddata1,moddata2;
	SAMPLE *dbuf,*sbuf;

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
	divs=instance->divs;
	divd=instance->divd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		div=divs+divd*modfunc2(curlen,instance->slen,moddata2);

		*(dbuf++)=(SAMPLE)((double)*sbuf*dry+((double)((LONG)((double)*(sbuf++)/div))*div)*eff);

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
