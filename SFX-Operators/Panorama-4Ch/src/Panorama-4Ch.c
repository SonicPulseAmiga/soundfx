/****h* Panorama-4Ch/Panorama-4Ch.c [4.3] *
*
*  NAME
*    Panorama-4Ch.c
*  COPYRIGHT
*    $VER: Panorama-4Ch.c 4.3 (20.08.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Panorama-4Ch core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    22.Aug.2003	V 4.3	most recent version
*							* audio-field normalisation
*							* two speaker-layouts
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/Panaorama-4Ch *
*  Function.eng
*    Distribute a mono-signal between 4 <a href="../node06.03.00.Channel.html">channels</a>.
*  Funktion.ger
*    Verteilt ein Monosample auf 4 <a href="../node06.03.00.Channel.html">Kanäle</a>.
*  Parameter.eng
*    Left-Right Position (<a href="../node03.02.01.html">P1</a>)
*      Propotion for left and right. 0 % (or 0.0) means left and 100 % (or 1.0) right.
*    Front-Back Position (<a href="../node03.02.01.html">P1</a>)
*      Propotion for front and back. 0 % (or 0.0) means front and 100 % (or 1.0) back.
*  Parameter.ger
*    Left-Right Position (<a href="../node03.02.01.html">P1</a>)
*      Verhältnis für links und rechts. 0 % (oder 0.0) bedeutet links, 100 % (oder 1.0) rechts.
*    Front-Back Position (<a href="../node03.02.01.html">P1</a>)
*      Verhältnis für vorne und hinten. 0 % (oder 0.0) bedeutet vorne, 100 % (oder 1.0) hinten.
*  Notes.eng
*    The result could be transformed back into a stero-sample by using the <a href="SurroundEncoder.html">SurroundEncoder</a>.
*    The resulting sample enfolds its depth by playing it via a surround-decoder.
*  Hinweise.ger
*    Das Ergebnis kann per <a href="SurroundEncoder.html">SurroundEncoder</a> wieder in ein Stereosample gewandelt werden,
*    welches seinen vollen Raumklang entfaltet wenn es über einen SurroundDecoder abgespielt wird.
*******
*/

#define PANORAMA_4CH_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Panorama-4Ch.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

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

void process_audio(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double lrs,lrd,lr,rl;
	double vhs,vhd,vh,hv;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *sbuf,*dbuf0,*dbuf1,*dbuf2,*dbuf3;
	double voll,volr,volv,volh,val;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;
	dbuf2=instance->dbuf2;
	dbuf3=instance->dbuf3;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	lrs=instance->lrs;
	lrd=instance->lrd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	vhs=instance->vhs;
	vhd=instance->vhd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		lr=lrs+lrd*modfunc1(curlen,instance->slen,moddata1);
		vh=vhs+vhd*modfunc2(curlen,instance->slen,moddata2);

#ifdef PAN4_VERSION1
		// most simple (like 2x stereo)
		// disadvantage: not possible, that it comes only from *one* speaker, e.g only rear: lr=0.5, vh=0.0
		voll=lr;
		volr=1.0-lr;
		volv=vh;
		volh=1.0-vh;
#endif

#ifdef PAN4_VERSION2
		// disadvantage: it is quiet in the middle
		if(lr<0.5)	{ voll=1.0-(lr+lr);volr=0.0; }
		else 		{ voll=0.0;volr=1.0-(lr+lr); }
		if(vh<0.5)	{ volv=1.0-(vh+vh);volh=0.0; }
		else		{ volv=0.0;volh=1.0-(vh+vh); }

#endif

#ifdef PAN4_VERSION3
		// distance based distribution (not good for later surround encoding)

		val=0.5-vh;val*=val;								// distance speakers <-> source
		voll=1.0-sqrt(        lr *lr+val);
		volr=1.0-sqrt((v2=1.0-lr)*v2+val);

		val=0.5-lr;val*=val;
		volv=1.0-sqrt(        vh *vh+val);
		volh=1.0-sqrt((v2=1.0-vh)*v2+val);
#endif

//#ifdef PAN$_VERSION4
		hv=1.0-vh;
		rl=1.0-lr;

		voll=(vh<0.5)?((lr-0.5)+vh):((lr-0.5)+hv);
		if(voll<0.0) voll=0.0;

		volr=(vh<0.5)?((0.5-lr)+vh):((0.5-lr)+hv);
		if(volr<0.0) volr=0.0;

		volv=(lr<0.5)?((vh-0.5)+lr):((vh-0.5)+rl);
		if(volv<0.0) volv=0.0;

		volh=(lr<0.5)?((0.5-vh)+lr):((0.5-vh)+rl);
		if(volh<0.0) volh=0.0;
//#endif

		val=voll+volr+volv+volh;							// normalise
		voll/=val;volr/=val;volv/=val;volh/=val;

		val=(double)*(sbuf++);								// distribute
		*(dbuf0++)=(SAMPLE)(val*voll);
		*(dbuf1++)=(SAMPLE)(val*volr);
		*(dbuf2++)=(SAMPLE)(val*volv);
		*(dbuf3++)=(SAMPLE)(val*volh);
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	MSG5("%ld : %8.6lf %8.6lf %8.6lf %8.6lf",curlen,voll,volr,volv,volh);
}

void process_video(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double lrs,lrd,lr,rl;
	double vhs,vhd,vh,hv;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *sbuf,*dbuf0,*dbuf1,*dbuf2,*dbuf3;
	double voll,volr,volv,volh,val;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;
	dbuf2=instance->dbuf2;
	dbuf3=instance->dbuf3;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	lrs=instance->lrs;
	lrd=instance->lrd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	vhs=instance->vhs;
	vhd=instance->vhd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		lr=lrs+lrd*modfunc1(curlen,instance->slen,moddata1);
		vh=vhs+vhd*modfunc2(curlen,instance->slen,moddata2);

		hv=1.0-vh;
		rl=1.0-lr;

		voll=(lr-0.5)+((vh-0.5)-rl);
		if(voll<0.0) voll=0.0;

		volr=(rl-0.5)+((vh-0.5)-lr);
		if(volr<0.0) volr=0.0;

		volv=(lr>0.5)?vh-((lr-0.5)*2):vh-((rl-0.5)*2);
		if(volv<0.0) volv=0.0;

		volh=hv;
		//if(volh<0.0) volh=0.0;

		val=voll+volr+volv+volh;							// normalise
		voll/=val;volr/=val;volv/=val;volh/=val;

		val=(double)*(sbuf++);
		*(dbuf0++)=(SAMPLE)(val*voll);						 // distribute
		*(dbuf1++)=(SAMPLE)(val*volr);
		*(dbuf2++)=(SAMPLE)(val*volv);
		*(dbuf3++)=(SAMPLE)(val*volh);
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	MSG5("%ld : %8.6lf %8.6lf %8.6lf %8.6lf",curlen,voll,volr,volv,volh);
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
