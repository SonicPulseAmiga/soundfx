/****h* Gamma/Gamma.c [4.2] *
*
*  NAME
*    Gamma.c
*  COPYRIGHT
*    $VER: Gamma.c 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Gamma core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    13.Feb.2003
*  MODIFICATION HISTORY
*    06.Mar.2003	V 4.2	most recent version
*    13.Feb.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/Gamma *
*  Function.eng
*    Gamma-correction for sampledata
*  Funktion.ger
*    Gammakorrektur für Samples
*  Parameter.eng
*    Gamma (<a href="../node03.02.01.html">P1</a>)
*      Factor for non-linear amplification/dampening.<br>
*      A value of 1.0 has no effect. A larger value dampens the data (makes quiet
*      signals even quieter). A smaller value amplifys (makes quiete values louder).
*  Parameter.ger
*    Gamma (<a href="../node03.02.01.html">P1</a>)
*      Wert für nichtlineare Verstärkung/Abschwächung.<br>
*      Bei einem Wert von 1.0 passiert nichts. Bei größeren Werten wird abgeschwächt
*      (leise Signale werden noch leiser). Bei kleineren Werten wird verstärkt
*      (leise Signale werden lauter).
*  Notes.eng
*    You may need this operator in the folowing case:
*    You've got a sample which uses the full amplituderange, but is still too
*    quiet because of it's <a href="../node06.03.00.Dynamic.html">Dynamic</a>. To make it louder you must amplify it
*    without to change the volume of the maxima and minima - only amplify
*    the values in the middle. This is exact that, what this operator does.
*    (It is basically the same as a gamma-operator for image-processing.)
*  Hinweise.ger
*    Diesen Operator benötigt man z.B. für folgenden Fall:
*    Man hat ein Sample welches voll ausgesteuert ist, aber wegen seiner <a href="../node06.03.00.Dynamic.html">Dynamik</a>
*    recht leise ist. Jetzt würde man das Ganze gern so verstärken, daß die
*    lauten und die leisen Werte so bleiben wie sie sind und die mittleren Werte
*    angehoben werden. Genau dies macht dieser Operator. (Im Prinzip das Gleiche
*    wie der Gamma-Operator bei Bildbearbeitungsprogrammen.)
*******
*/

#define GAMMA_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Gamma.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	//DEBUG
	printf(__FILE__"::init(\n");
	//DEBUG
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	//DEBUG
	printf(__FILE__"::instantiate(0x%08lx)\n",(unsigned long)instance);
	//DEBUG
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	//DEBUG
	printf(__FILE__"::activate(0x%08lx)\n",(unsigned long)instance);
	//DEBUG
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	//DEBUG
	printf(__FILE__"::activate_ch(0x%08lx,%1ld)\n",(unsigned long)instance,ch);
	//DEBUG

	AssignBShape(&instance->mod);
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double gammas,gammad,gamma,res;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;

	//DEBUG
	printf(__FILE__"::process(0x%08lx,%ld)\n",(unsigned long)instance,samples);
	//DEBUG
	
	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	gammas=instance->gammas;
	gammad=instance->gammad;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		gamma=gammas+gammad*modfunc(curlen,instance->slen,moddata);

		if     (*sbuf<0) res=(SMP_MAXN*mypow(*(sbuf++)/(double)SMP_MAXN,gamma));
		else if(*sbuf>0) res=(SMP_MAXP*mypow(*(sbuf++)/(double)SMP_MAXP,gamma));
		else           { res=0;sbuf++; }
		*(dbuf++)=res;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
	//DEBUG
	printf(__FILE__"::deactivate_ch(0x%08lx,%1ld)\n",(unsigned long)instance,ch);
	//DEBUG
}

void deactivate(SFXCoreInstance *instance) {
	//DEBUG
	printf(__FILE__"::deactivate(0x%08lx)\n",(unsigned long)instance);
	//DEBUG
}

void destroy(SFXCoreInstance *instance) {
	//DEBUG
	printf(__FILE__"::destroy(0x%08lx)\n",(unsigned long)instance);
	//DEBUG
}

void done(void) {
	//DEBUG
	printf(__FILE__"::done(\n");
	//DEBUG
}
//-- eof ----------------------------------------------------------------------
