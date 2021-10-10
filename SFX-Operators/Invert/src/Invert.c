/****h* Invert/Invert.c [4.2] *
*
*  NAME
*    Invert.c
*  COPYRIGHT
*    $VER: Invert.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Invert core file
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
/****u* _operators/Invert *
*  Function.eng
*    Swaps upper and lower half of the sample
*  Funktion.ger
*    Vertauscht die obere und untere Samplehälfte
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*  Notes.eng
*    Should the effect parameter be set to 100<->0 or visa versa, then
*    somewhere an area of the sample could be zeroed (oppposite match).<br>
*    Mixing an inverted sample on top of the original with a slight delay
*    can produce effects that sound like resonance stuff.<br>
*    creating a stereo sample from a mono source, when one channel is
*    inverted produces wide-stereo sounds.
*  Hinweise.ger
*    Wenn man den Effektanteil von 100 nach 0 überblendet wird das Samplesignal
*    zwischenzeitlich ausgelöscht (Überlagerung).<br>
*    Wenn man ein invertiertes Sample vesetzt auf das Orginal aufmischt
*    und danach verstärkt, erhält man resonanzähnliche Effekte.<br>
*    Wenn man ein Stereo-Sample aus einem Mono-Sample erzeugt und einen Kanal
*    invertiert, erhält man einen breiten Stereoklang.
*******
*/

#define INVERT_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Invert.h"

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
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	effs=instance->effs;
	effd=instance->effd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc(curlen,instance->slen,moddata);dry=1.0-eff;

		*(dbuf++)=(SAMPLE)((double)*sbuf*dry+(double)(-1-*(sbuf++))*eff);

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
