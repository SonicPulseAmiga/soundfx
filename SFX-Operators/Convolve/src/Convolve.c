/****h* Convolve/Convolve.c [4.2] *
*
*  NAME
*    Convolve.c
*  COPYRIGHT
*    $VER: Convolve.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Convolve core file
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
/****u* _operators/Convolve *
*  Function.eng
*    Applies the <a href="../node06.03.00.Impulseresponse.html">impulseresponse</a> in src2 to src1. If you e.g. have
*    the sampled impulseresponse of a church-hall then you can apply
*    this reverberation characteristics to any sample in src1.
*  Funktion.ger
*    Prägt die <a href="../node06.03.00.Impulseresponse.html">Impulsantwort</a> in src2 auf src1 auf. Wenn Sie z.B die
*    gesampelte Impulsantwort eines Kirchenraumes haben, dann können
*    Sie diese Raumakustik zu jedem Sample in src1 hinzufügen.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Ampf
*      final amplification
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Ampf
*      abschließende Lautstärkeanpassung
*  Notes.eng
*    You probably have no sample impulsresponses - right, go ahead and try a
*    snaredrum sample (something with a noisy fading trail).
*    The resulting signal gets very loud (depends on the src2-sample) - choose
*    a smaller value of Ampd to compensate.
*  Hinweise.ger
*    Da sie jetzt bestimmt keine gesampelte Impulsantwort haben, können sie es
*    auch mal mit einem Snaredrum-sample probieren (etwas was verrauscht ausklingt).
*    Das Ergebis dürfte sehr laut werden (hängt vom src2-sample ab) - verwenden
*    Sie einen kleineren Ampf-Wert um dies zu kompensieren.
*******
*/

#define CONVOLVE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Convolve.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	if(!(instance->delbufbeg=(SAMPLE *)AllocVec(instance->dellen*sizeof(SAMPLE),MEMF_ANY|MEMF_CLEAR))) return(FALSE);
	if(!(instance->sirbufbeg=(double *)AllocVec(instance->dellen*sizeof(double),MEMF_ANY))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	unsigned long i;
	double *sirbufptr;
	
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);

	instance->delbufend=&instance->delbufbeg[instance->dellen-1];
	instance->delbufptr=instance->delbufbeg;
	sirbufptr=instance->sirbufbeg;
	for(i=0;i<instance->dellen;i++) *(sirbufptr++)=((double)(*(instance->delbufptr++)))/32768.0;
	instance->delbufptr=instance->src2.srcbuf->vbuf[ch];		// has to be in memory !!!
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,k,curlen;
	double effs,effd,eff,dry;
	double fbs,fbd,fb,dr;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf,*sbuf;
	LONG val1;
	double sum,ampf;
	SAMPLE *delbufbeg,*delbufend,*delbufptr;
	double *sirbufptr;
	
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
	fbs=instance->fbs;
	fbd=instance->fbd;

	ampf=instance->ampf;
	delbufbeg=instance->delbufbeg;
	delbufend=instance->delbufend;
	delbufptr=instance->delbufptr;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		fb =fbs +fbd *modfunc2(curlen,instance->slen,moddata2);dr =1.0-fb;
		
		*delbufptr=fb*(*delbufptr)+dr*(*sbuf);
		sum=0.0;sirbufptr=instance->sirbufbeg;
		k=0;
		while(k<instance->dellen) {
			sum+=((double)(*(delbufptr--)))*(*(sirbufptr++));if(delbufptr<delbufbeg) delbufptr=delbufend;
			sum+=((double)(*(delbufptr--)))*(*(sirbufptr++));if(delbufptr<delbufbeg) delbufptr=delbufend;
			sum+=((double)(*(delbufptr--)))*(*(sirbufptr++));if(delbufptr<delbufbeg) delbufptr=delbufend;
			sum+=((double)(*(delbufptr--)))*(*(sirbufptr++));if(delbufptr<delbufbeg) delbufptr=delbufend;
			k+=4;
		}
		sum*=ampf;
		delbufptr++;if(delbufptr>delbufend) delbufptr=delbufbeg;
		val1=(LONG)(sum*eff+(double)*(sbuf++)*dry);
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->delbufptr=delbufptr;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->delbufbeg) { FreeVec((ULONG *)instance->delbufbeg);instance->delbufbeg=NULL; }
	if(instance->sirbufbeg) { FreeVec((ULONG *)instance->sirbufbeg);instance->sirbufbeg=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
