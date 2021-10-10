/****h* ZeroPass/ZeroPass.c [4.2] *
*
*  NAME
*    ZeroPass.c
*  COPYRIGHT
*    $VER: ZeroPass.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    ZeroPass core file
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
/****u* _operators/ZeroPass *
*  Function.eng
*    Fades the volume at beginning from 0 in and at end to 0 off
*  Funktion.ger
*    Blendet die Lautstärke am Start von 0 ein und am Ende zu 0 aus
*  Parameter.eng
*    FadeIn/Range (SRange)
*      Range for fading the sound in
*    FadeIn/Shape (SModShape)
*      Type of fade
*    FadeOut/Range (ERange)
*      Range for fading the sound out
*    FadeOut/Shape (EModShape)
*      Type of fade
*  Parameter.ger
*    FadeIn/Range (SRange)
*      Bereich der für die Einblendung genutzt werden soll
*    FadeIn/Shape (SModShape)
*      Hüllform, wie übergeblendet werden soll.
*    FadeOut/Range (ERange)
*      Bereich der für die Ausblendung genutzt werden soll
*    FadeOut/Shape (EModShape)
*      Hüllform, wie übergeblendet werden soll.
*  Notes.eng
*    If a sample doesn't starts or ends with a value of zero, we hear that as
*    cracks during play. This operator force the begin and the end to zero and
*    fades to normal volume.
*    The shape &quot;slowdown&quot; creates a fade that sounds linear to the ear.
*  Hinweise.ger
*    Falls ein Sample über/unter der 0-Linie beginnt/endet, hört man dies als
*    Knacken. Diese Funktion legt den Anfang bzw. das Ende auf 0 und blendet
*    dann innerhalb des Bereichs (RangeS/RangeE) zu den Originalwerten über.
*    Die Kurve &quot;slowdown&quot;  erzeugt eine Überblendung, die das Ohr
*    als linear empfindet.
*******
*/

#define ZEROPASS_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "ZeroPass.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	if((instance->mod1.bshpdata=AllocVec(sizeof(ModCurve),(MEMF_ANY|MEMF_CLEAR)))) {
		instance->mod1.bshape=BSHP_CURVE;
		switch(instance->shapes) {
			case CURVE_LINEAR:		((ModCurve *)(instance->mod1.bshpdata))->exp=1.0;break;
			case CURVE_SPEEDUP:		((ModCurve *)(instance->mod1.bshpdata))->exp=2.0;break;
			case CURVE_SLOWDOWN:	((ModCurve *)(instance->mod1.bshpdata))->exp=0.5;break;
		}
	}
	else instance->mod1.bshape=BSHP_NONE;
	instance->mod1.desc[0]='\0';
	instance->poss=instance->lens;

	if((instance->mod2.bshpdata=AllocVec(sizeof(ModCurve),(MEMF_ANY|MEMF_CLEAR)))) {
		instance->mod2.bshape=BSHP_CURVE;
		switch(instance->shapee) {
			case CURVE_LINEAR:		((ModCurve *)(instance->mod2.bshpdata))->exp=1.0;break;
			case CURVE_SPEEDUP:		((ModCurve *)(instance->mod2.bshpdata))->exp=2.0;break;
			case CURVE_SLOWDOWN:	((ModCurve *)(instance->mod2.bshpdata))->exp=0.5;break;
		}
	}
	else instance->mod2.bshape=BSHP_NONE;
	instance->mod2.desc[0]='\0';
	instance->pose=instance->slen-instance->lene;

	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	ULONG poss,pose,lens,lene;
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
	lens=instance->lens;
	poss=instance->poss;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	lene=instance->lene;
	pose=instance->pose;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		if(curlen<poss)			*(dbuf++)=*(sbuf++)*modfunc1(                curlen ,lens,moddata1);
		else if(curlen>pose)	*(dbuf++)=*(sbuf++)*modfunc2((instance->slen-curlen),lene,moddata2);
		else 					*(dbuf++)=*(sbuf++);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->mod1.bshpdata)	{ FreeVec(instance->mod1.bshpdata);instance->mod1.bshpdata=NULL;instance->mod1.bshape=BSHP_NONE; }
	if(instance->mod2.bshpdata)	{ FreeVec(instance->mod2.bshpdata);instance->mod2.bshpdata=NULL;instance->mod2.bshape=BSHP_NONE; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
