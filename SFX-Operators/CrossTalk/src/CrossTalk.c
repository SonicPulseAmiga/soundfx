/****h* CrossTalk/CrossTalk.c [4.2] *
*
*  NAME
*    CrossTalk.c
*  COPYRIGHT
*    $VER: CrossTalk.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    CrossTalk core file
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
/****u* _operators/CrossTalk *
*  Function.eng
*    Removes or adds crosstalk of channels
*  Funktion.ger
*    Vermindert oder erweitert das Übersprechen zwischen einzelnen Kanälen
*  Parameter.eng
*    Width (<a href="../node03.02.01.html">P1</a>)
*      -100 % yields a monosignal and 100 % an extreme expansion
*    Depth (<a href="../node03.02.01.html">P2</a>)
*      same as width, (only avalable when processing quadrosamples)
*    Ampf
*      final amplification
*  Parameter.ger
*    Width (<a href="../node03.02.01.html">P1</a>)
*      -100 % ergibt ein Monosignal und 100 % eine extreme Erweiterung
*    Depth (<a href="../node03.02.01.html">P2</a>)
*      analog zu Width, (nur verfügbar, wenn man Quadrosamples bearbeitet)
*    Ampf
*      abschließende Lautstärkeanpassung
*  Notes.eng
*    Monosamples can't be processed, as they have no room-information. This cannot
*    be fixed by converting them to stereo samples.
*  Hinweise.ger
*    Monosamples können nicht bearbeitet werden, da sie keine Rauminformationen
*    beinhalten. Es hilft auch nicht, diese zu Stereosamples zu konvertieren.
*******
*/

#define CROSSTALK_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "CrossTalk.h"

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

void process_s(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double wids,widd,wid;
	LONG lv,rv,tv;
	double ampf;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf0,*dbuf1;
	SAMPLE *sbuf0,*sbuf1;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;sbuf1=instance->sbuf1;
	dbuf0=instance->dbuf0;dbuf1=instance->dbuf1;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	wids=instance->wids;
	widd=instance->widd;

	ampf=instance->ampf;
	
	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		wid=wids+widd*modfunc1(curlen,instance->slen,moddata1);
		
		tv=lv=(LONG)*(sbuf0++);rv=(LONG)*(sbuf1++);
		lv=(LONG)(lv-((double)rv*wid))*ampf;
		rv=(LONG)(rv-((double)tv*wid))*ampf;
		CLIPINT(lv);
		*(dbuf0++)=(SAMPLE)lv;
		CLIPINT(rv);
		*(dbuf1++)=(SAMPLE)rv;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_q(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double wids,widd,wid;
	double deps,depd,dep;
	LONG lv,rv,vv,hv,tv;
	double ampf;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf0,*dbuf1,*dbuf2,*dbuf3;
	SAMPLE *sbuf0,*sbuf1,*sbuf2,*sbuf3;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;sbuf1=instance->sbuf1;sbuf2=instance->sbuf2;sbuf3=instance->sbuf3;
	dbuf0=instance->dbuf0;dbuf1=instance->dbuf1;dbuf2=instance->dbuf2;dbuf3=instance->dbuf3;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	wids=instance->wids;
	widd=instance->widd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	deps=instance->deps;
	depd=instance->depd;

	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		wid=wids+widd*modfunc1(curlen,instance->slen,moddata1);
		dep=deps+depd*modfunc2(curlen,instance->slen,moddata2);
		
		tv=lv=(LONG)*(sbuf0++);rv=(LONG)*(sbuf1++);
		lv=(LONG)(lv-((double)rv*wid))*ampf;
		rv=(LONG)(rv-((double)tv*wid))*ampf;
		tv=vv=(LONG)*(sbuf2++);hv=(LONG)*(sbuf3++);
		vv=(LONG)(vv-((double)hv*dep))*ampf;
		hv=(LONG)(hv-((double)tv*dep))*ampf;
		CLIPINT(lv);
		*(dbuf0++)=(SAMPLE)lv;
		CLIPINT(rv);
		*(dbuf1++)=(SAMPLE)rv;
		CLIPINT(vv);
		*(dbuf2++)=(SAMPLE)vv;
		CLIPINT(hv);
		*(dbuf3++)=(SAMPLE)hv;

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
