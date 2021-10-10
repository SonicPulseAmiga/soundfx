/****h* SurroundDecoder/SurroundDecoder.c [4.3] *
*
*  NAME
*    SurroundDecoder.c
*  COPYRIGHT
*    $VER: SurroundDecoder.c 4.3 (22.08.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SurroundDecoder core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    22.Aug.2003
*  MODIFICATION HISTORY
*    22.Aug.2003	V 4.3	most recent version
*    22.Aug.2003	V 4.3	initial version
*  NOTES
*
*******
*/

#define SOUROUNDDECODER_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "SurroundDecoder.h"

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
}

void process_passive(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	LONG l,r,c,s;
	SAMPLE *dbuf0,*dbuf1,*dbuf2,*dbuf3,*sbuf0,*sbuf1;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;
	dbuf2=instance->dbuf2;
	dbuf3=instance->dbuf3;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		l=*(sbuf0++);
		r=*(sbuf1++);
		s=l-r;
		c=l+r;

		//correction
		////c-=s;
		//c-=abs(s);
		//l-=c;
		//r-=c;


		*(dbuf0++)=l;
		*(dbuf1++)=r;
		*(dbuf2++)=c;
		*(dbuf3++)=s;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_active(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	LONG l,r,c,s;
	double vl,vr,vc,vs,dlr,dcs;
	SAMPLE *dbuf0,*dbuf1,*dbuf2,*dbuf3,*sbuf0,*sbuf1;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;
	dbuf0=instance->dbuf0;
	dbuf1=instance->dbuf1;
	dbuf2=instance->dbuf2;
	dbuf3=instance->dbuf3;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		l=*(sbuf0++);
		r=*(sbuf1++);
		s=l-r;
		c=l+r;

		// full wave rectification
		vl=fabs((double)l/32768.0);vr=fabs((double)r/32768.0);
		vc=fabs((double)c/32768.0);vs=fabs((double)s/32768.0);

		// servo logic
		dlr=log(vl)/log(vr);
		dcs=log(vc)/log(vs);

		*(dbuf0++)=l;
		*(dbuf1++)=r;
		*(dbuf2++)=c>>1;
		*(dbuf3++)=s>>1;

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
