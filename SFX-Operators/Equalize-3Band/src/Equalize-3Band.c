/****h* Equalize-3Band/Equalize-3Band.c [4.2] *
*
*  NAME
*    Equalize-3Band.c
*  COPYRIGHT
*    $VER: Equalize-3Band.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Equalize-3Band core file
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
/****u* _operators/Equalize-3Band *
*  Function.eng
*    Raises or lowers high, mid and low frequencies. Works like the tone control of you hifi.
*  Funktion.ger
*    Hohe, mittlere und tiefe Frequenzen können angehoben oder abgesenkt werden. Funktioniert wie die
*    Klangkontrolle an der Stereoanlage.
*  Parameter.eng
*    Lower Cut-Off (<a href="../node03.02.01.html">P1</a>)
*      frequency which divides the lower from the middle band, relative frequency ranging from 0 Hz to half of sampling-rate
*    Higher Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      frequency which divides the middle from the upper band, relative frequency ranging from 0 Hz to half of sampling-rate
*    Lower gain (<a href="../node03.02.01.html">P3</a>)
*      amplification for the lower band
*    Middle gain (<a href="../node03.02.01.html">P4</a>)
*      amplification for the middle band
*    Higher gain (<a href="../node03.02.01.html">P5</a>)
*      amplification for the higher band
*    Ampf
*      final amplification
*  Parameter.ger
*    Lower Cut-Off (<a href="../node03.02.01.html">P1</a>)
*      Frequenz die das tiefe vom mittleren Band trennt, relative Frequenz von 0 Hz bis zur halben Samplingrate
*    Higher Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      Frequenz die das mittlere vom hohen Band trennt, relative Frequenz von 0 Hz bis zur halben Samplingrate
*    Lower gain (<a href="../node03.02.01.html">P3</a>)
*      Lautstärkeanpassung des tiefen Bandes
*    Middle gain (<a href="../node03.02.01.html">P4</a>)
*      Lautstärkeanpassung des mittleren Bandes
*    Higher gain (<a href="../node03.02.01.html">P5</a>)
*      Lautstärkeanpassung des höheren Bandes
*    Ampf
*      abschließende Lautstärkeanpassung
*  Notes.eng
*    On your hifi you normaly can not change the filter cut-offs. If in doubt, just leave them as they are.
*  Hinweise.ger
*    An Ihrere Stereoanlage werden sie die Filter Cut-Offs warscheinlich nicht ändern können. Im Zweifelsfalle
*    lassen sie die Werte einfach wie sie sind.
*
*******
*/

#define EQUALIZE_3BAND_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Equalize-3Band.h"

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
	AssignBShape(&instance->mod3);
	AssignBShape(&instance->mod4);
	AssignBShape(&instance->mod5);
	instance->llow=instance->lmid=instance->lhigh=instance->hlow=instance->hmid=instance->hhigh=0.0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double frqls,frqld,frql;
	double frqhs,frqhd,frqh;
	double llow,lmid,lhigh;
	double hlow,hmid,hhigh;
	double ampls,ampld,ampl;
	double ampms,ampmd,ampm;
	double amphs,amphd,amph;
	double ampf,mid;
	LONG val1;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3,modfunc4,modfunc5;
	void *moddata1,*moddata2,*moddata3,*moddata4,*moddata5;
	SAMPLE *dbuf,*sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	frqls=instance->frqls;
	frqld=instance->frqld;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	frqhs=instance->frqhs;
	frqhd=instance->frqhd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	ampls=instance->ampls;
	ampld=instance->ampld;
	// Modulator 4
	moddata4=(void *)(instance->mod4.bshpdata);
	modfunc4=instance->mod4.modfunc;
	ampms=instance->ampms;
	ampmd=instance->ampmd;
	// Modulator 5
	moddata5=(void *)(instance->mod5.bshpdata);
	modfunc5=instance->mod5.modfunc;
	amphs=instance->amphs;
	amphd=instance->amphd;
	//
	ampf=instance->ampf;
	llow=instance->llow;
	lmid=instance->lmid;
	lhigh=instance->lhigh;
	hlow=instance->hlow;
	hmid=instance->hmid;
	hhigh=instance->hhigh;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		frql=frqls+frqld*modfunc1(curlen,instance->slen,moddata1);
		frqh=frqhs+frqhd*modfunc2(curlen,instance->slen,moddata2);
		ampl=ampls+ampld*modfunc3(curlen,instance->slen,moddata3);
		ampm=ampms+ampmd*modfunc4(curlen,instance->slen,moddata4);
		amph=amphs+amphd*modfunc5(curlen,instance->slen,moddata5);

		lhigh=(double)*sbuf-lmid-llow;
		lmid+=(lhigh*frql);llow+=(lmid*frql);

		hhigh=(double)*sbuf-hmid-hlow;
		hmid+=(hhigh*frqh);hlow+=(hmid*frqh);

		//mid=(double)*(sbuf++)-(llow+hhigh);
		//mid=(hlow-llow);
		//mid=(lhigh-hhigh);
		mid=0.5*((hlow-llow)+(lhigh-hhigh));sbuf++;
		val1=(LONG)(((llow*ampl)+(mid*ampm)+(hhigh*amph))*ampf);
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->llow=llow;
	instance->lmid=lmid;
	instance->lhigh=lhigh;
	instance->hlow=hlow;
	instance->hmid=hmid;
	instance->hhigh=hhigh;
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
