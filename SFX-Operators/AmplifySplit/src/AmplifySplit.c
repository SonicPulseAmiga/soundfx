/****h* AmplifySplit/AmplifySplit.c [4.2] *
*
*  NAME
*    AmplifySplit.c
*  COPYRIGHT
*    $VER: AmplifySplit.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    AmplifySplit core file
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
/****u* _operators/AmplifySplit *
*  Function.eng
*    Allows independet adjustment of volume for upper and lower parts of a sample.
*    Replaces Clap and Clear operators from older versions of __SFX.
*  Funktion.ger
*    Ermöglicht das unabhängige Ändern der Lautstärke der oberen und
*    unteren Samplehälfte. Ersetzt die Clap und Clear Operatoren von älteren __SFX Versionen.
*  Parameter.eng
*    Upper Amplification (<a href="../node03.02.01.html">P1</a>)
*      This value controls the amount of amplification for the upper sample-half.
*    Lower Amplification (<a href="../node03.02.01.html">P2</a>)
*      This value controls the amount of amplification for the lower sample-half.
*    MaxVolUpper
*      After a clicking this button, the current source will be scanned and the maximum
*      amplification without clipping the upper values will be calculated.
*    MaxVolLower
*      After a clicking this button, the current source will be scanned and the maximum
*      amplification without clipping the lower values will be calculated.
*  Parameter.ger
*    Upper Amplification (<a href="../node03.02.01.html">P1</a>)
*      Dieser Wert gibt die Lautstärkeänderung der oberen Samplehälfte an.
*    Lower Amplification (<a href="../node03.02.01.html">P2</a>)
*      Dieser Wert gibt die Lautstärkeänderung der unteren Samplehälfte an.
*    MaxVol Upper
*      Durch einen Klick auf diesen Knopf, wird das Sample gescannt und die maximale
*      Verstärkung errechnet die möglich ist, ohne das Signal nach oben zu übersteuern.
*    MaxVol Upper
*      Durch einen Klick auf diesen Knopf, wird das Sample gescannt und die maximale
*      Verstärkung errechnet die möglich ist, ohne das Signal nach unten zu übersteuern.
*  Notes.eng
*    see <a href="Amplify.html">Amplify</a> operator
*  Hinweise.ger
*    siehe <a href="Amplify.html">Amplify</a> Operator
*******
*/

#define AMPLIFYSPLIT_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "AmplifySplit.h"

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
	double ampus,ampud,ampu;
	double ampls,ampld,ampl;
	LONG res;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf,*sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	ampus=instance->ampus;
	ampud=instance->ampud;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	ampls=instance->ampls;
	ampld=instance->ampld;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		ampu=ampus+ampud*modfunc1(curlen,instance->slen,moddata1);
		ampl=ampls+ampld*modfunc2(curlen,instance->slen,moddata2);

		res=*(sbuf++);
		if(res<0) res*=ampl;
		else res*=ampu;
		CLIPINT(res);
		*(dbuf++)=(SAMPLE)res;

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
