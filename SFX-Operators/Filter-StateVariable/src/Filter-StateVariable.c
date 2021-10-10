/****h* Filter-StateVariable/Filter-StateVariable.c [4.2] *
*
*  NAME
*    Filter-StateVariable.c
*  COPYRIGHT
*    $VER: Filter-StateVariable.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-StateVariable core file
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
/****u* _operators/Filter-StateVariable *
*  Function.eng
*    Filters/boosts frequencies according to the filtertype. Can resonanate at the cut-off-frequency.
*  Funktion.ger
*    Filtert/boostet Frequenzen je nach Filtertyp. Kann auch auf der Cut-Off-Frequenz resonieren.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome. Negative values produce the
*      opposite	effect - they boost frequencys.
*    Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      frequency where processing starts, relative frequency ranging from 0 Hz to half of sampling-rate
*    Resonance
*      From 1.0 to infinity. Too high values will make your sample scream (basicly
*      lots of overdrive on the cutoff-frequency)
*    Ampf
*      final amplification
*    Type
*      what kind of filter you want to apply
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt. Negative Effektanteile bewirken keine Dämpfung,
*      sondern eine Verstärkung der zu bearbeitenden Frequenzen.
*    Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      Frequenz an der die Bearbeitung einsetzt, relative Frequenz von 0 Hz bis zur halben Samplingrate
*    Resonance
*       Von 1.0 bis unendlich. Zu hohe Werte lassen das Sample nur noch kreischen
*       (wegen starker Übersteuerung der Cut-Off-Frequenz)
*    Ampf
*      abschließende Lautstärkeanpassung
*    Type
*      Welche Filterart man berechnen möchte
*  Notes.eng
*    This filter is not a accurate as a <a href="../node06.03.00.Filter.html">FIR-filter</a>, but is much
*    faster and can resonate.
*  Hinweise.ger
*    Dieser Filter ist nicht so genau wie ein <a href="../node06.03.00.Filter.html">FIR-Filter</a>, ist aber
*    wesentlich schneller und kann resonieren.
*******
*/

#define FILTER_STATEVARIABLE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Filter-StateVariable.h"

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
	instance->low=instance->mid=instance->high=0.0;
}

void process_lowpass(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	double frqs,frqd,frq;
	double ress,resd,res;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	double low,mid,high;
	double ampf;
	LONG val1;

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
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	ress=instance->ress;
	resd=instance->resd;
	
	low=instance->low;
	mid=instance->mid;
	high=instance->high;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		frq=frqs+frqd*modfunc2(curlen,instance->slen,moddata2);
		res=ress+resd*modfunc3(curlen,instance->slen,moddata3);
		eff*=ampf;dry*=ampf;

		high=((double)*sbuf)-(mid*res)-low;
		mid+=(high*frq);low+=(mid*frq);

		val1=(LONG)((((double)*(sbuf++))*dry)+(low*eff));
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
}

void process_highpass(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	double frqs,frqd,frq;
	double ress,resd,res;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	double low,mid,high;
	double ampf;
	LONG val1;

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
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	ress=instance->ress;
	resd=instance->resd;
	
	low=instance->low;
	mid=instance->mid;
	high=instance->high;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		frq=frqs+frqd*modfunc2(curlen,instance->slen,moddata2);
		res=ress+resd*modfunc3(curlen,instance->slen,moddata3);
		eff*=ampf;dry*=ampf;

		high=((double)*sbuf)-(mid*res)-low;
		mid+=(high*frq);low+=(mid*frq);

		val1=(LONG)((((double)*(sbuf++))*dry)+(high*eff));
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
}

void process_bandpass(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	double frqs,frqd,frq;
	double ress,resd,res;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	double low,mid,high;
	double ampf;
	LONG val1;

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
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	ress=instance->ress;
	resd=instance->resd;
	
	low=instance->low;
	mid=instance->mid;
	high=instance->high;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		frq=frqs+frqd*modfunc2(curlen,instance->slen,moddata2);
		res=ress+resd*modfunc3(curlen,instance->slen,moddata3);
		eff*=ampf;dry*=ampf;

		high=((double)*sbuf)-(mid*res)-low;
		mid+=(high*frq);low+=(mid*frq);

		val1=(LONG)((((double)*(sbuf++))*dry)+(mid*eff));
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
}

void process_bandstop(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	double frqs,frqd,frq;
	double ress,resd,res;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	double low,mid,high;
	double ampf;
	LONG val1;

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
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	ress=instance->ress;
	resd=instance->resd;
	
	low=instance->low;
	mid=instance->mid;
	high=instance->high;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		frq=frqs+frqd*modfunc2(curlen,instance->slen,moddata2);
		res=ress+resd*modfunc3(curlen,instance->slen,moddata3);
		eff*=ampf;dry*=ampf;

		high=((double)*sbuf)-(mid*res)-low;
		mid+=(high*frq);low+=(mid*frq);

		val1=(LONG)((((double)*(sbuf++))*dry)+((low+high)*eff));
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
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
