/****h* Mix/Mix.c [4.2] *
*
*  NAME
*    Mix.c
*  COPYRIGHT
*    $VER: Mix.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Mix core file
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
/****u* _operators/Mix *
*  Function.eng
*    Mixes two samples.
*  Funktion.ger
*    Mischt zwei Samples
*  Parameter.eng
*    Mixratio Source 1 (<a href="../node03.02.01.html">P1</a>)
*      how much of source 1 goes into the result; controls the propotion of
*      source 2 as well, which is 100 % minus this value.
*    Delay Source 2
*      delays the source 2
*  Parameter.ger
*    Mixratio Source 1 (<a href="../node03.02.01.html">P1</a>)
*      Anteil von Source 1; steuert ebenfalls den Anteil von Source 2, welcher
*      100 % minus dieser Wert ist
*    Delay Source 2
*      verzögert Source 2
*  Notes.eng
*    Smooth change in the mixratio can be used to create blending from one sample
*    to another.<br>
*    Due to the fact that the mixing is done in 80-bit resolution too, there is
*    no need to have a 'clipping' mixmode. Try it and you'll see.
*  Hinweise.ger
*    Durch sanftes Verändern der Mixratio, kann man nahezu stufenlos von einem
*    Sample zu einem anderen überblenden.<br>
*    Da dieser Operator ebenfalls mit 80-bit Auflösung rechnet, benötigt man kein
*    'clipping'-Modus. Einfach ausprobieren, übersteuern kann da nix.
*******
*/

#define MIX_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Mix.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	if(!(instance->rbuf=NewRingBuf(instance->delay2,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod);
	memset(instance->rbuf,0,instance->rbufsize);
	instance->rbufw=instance->rbufbase;
	instance->rbufr=(SAMPLE *)((ULONG)instance->rbufbase|(instance->rbufsize-Samples2Bytes(instance->delay2)));
}

void process_0(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double mix1s,mix1d,mix1,mix2;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf1,*sbuf2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	mix1s=instance->mix1s;
	mix1d=instance->mix1d;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		mix1=mix1s+mix1d*modfunc(curlen,instance->slen,moddata);mix2=1.0-mix1;

		*(dbuf++)=(SAMPLE)((double)*(sbuf1++)*mix1+(double)*(sbuf2++)*mix2);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_1(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double mix1s,mix1d,mix1,mix2;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf1,*sbuf2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	mix1s=instance->mix1s;
	mix1d=instance->mix1d;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		mix1=mix1s+mix1d*modfunc(curlen,instance->slen,moddata);mix2=1.0-mix1;

		*(dbuf++)=(SAMPLE)((double)*(sbuf1++)*mix1);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_2(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double mix1s,mix1d,mix1,mix2;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf1,*sbuf2;
	SAMPLE *rbufr,*rbufw,*rbufbase;
	ULONG rbufmask;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf1=instance->sbuf1;
	sbuf2=instance->sbuf2;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	mix1s=instance->mix1s;
	mix1d=instance->mix1d;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufr=instance->rbufr;
	rbufw=instance->rbufw;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		mix1=mix1s+mix1d*modfunc(curlen,instance->slen,moddata);mix2=1.0-mix1;

		*(dbuf++)=(SAMPLE)((double)*(sbuf1++)*mix1+(double)*rbufr*mix2);
		*rbufw=*(sbuf2++);
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufr=rbufr;
	instance->rbufw=rbufw;
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
