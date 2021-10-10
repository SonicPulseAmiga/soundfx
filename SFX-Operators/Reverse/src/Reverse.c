/****h* Reverse/Reverse.c [4.2] *
*
*  NAME
*    Reverse.c
*  COPYRIGHT
*    $VER: Reverse.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Reverse core file
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
/****u* _operators/Reverse *
*  Function.eng
*    Turns the sample backwards
*  Funktion.ger
*    Dreht das Sample um (rückwärts)
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*  Notes.eng
*    When Effect is set to 50% an X-Fade is performed. This means that the reverse
*    sample is mixed into the original sample. This is a neat way to hide loops.
*    Who've just done in string samples as the beginning will sound just like the end.
*  Hinweise.ger
*    Bei einem Effektanteil von 50 % erhält man ein XFade-Operator. Dann wird das
*    gedrehte Sample auf das Originalsample aufgemischt. Wenn man Strings Loopen
*    will, ist das manchmal eine gute Möglichkeit den Loop unhörbar zu machen,
*    weil das Sample nun am Anfang und am Ende gleich klingt.
*******
*/

#define REVERSE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Reverse.h"

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
	instance->dpos=instance->spos1=instance->spos2=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf0,*sbuf1;
	ULONG spos1,spos2,dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	//
	spos1=instance->spos1;
	spos2=instance->spos2;
	dpos=instance->dpos;

	while((spos1>0) && (spos2>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc(curlen,instance->slen,moddata);dry=1.0-eff;

		*(dbuf++)=(SAMPLE)((double)*(sbuf1++)*dry+(double)*(sbuf0--)*eff);
		//dbuf++;sbuf1++;sbuf0--;
		dpos++;spos2--;spos1--;curlen++;instance->curlen++;
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->sbuf0=sbuf0;
	instance->sbuf1=sbuf1;
	instance->dbuf=dbuf;
	instance->spos1=spos1;
	instance->spos2=spos2;
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
