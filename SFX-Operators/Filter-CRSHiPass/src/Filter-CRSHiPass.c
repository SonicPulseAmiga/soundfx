/****h* Filter-CRSHiPass/Filter-CRSHiPass.c [4.2] *
*
*  NAME
*    Filter-CRSHiPass.c
*  COPYRIGHT
*    $VER: Filter-CRSHiPass.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-CRSHiPass core file
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
/****u* _operators/Filter-CRSHiPass *
*  Function.eng
*    Works on low frequencies, means supresses or boosts them while leting high frequencies pass through unaltered.
*  Funktion.ger
*    Bearbeitet tiefe Frequenzen des Samples, d.h. unterdrückt oder verstärkt sie und läßt Hohe durch.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome. Negative values produce the
*      opposite	effect - they boost frequencys.
*    Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      Area for averaging-calculations. The wider that range, the higher the
*      cut-off frequency gets.
*    Resonance (<a href="../node03.02.01.html">P3</a>)
*      Strength of resonance (also Peak or Q-Factor).
*      As a strong resonance thins out the signal, there is an amplification-factor
*      that runs parallel with the resonance, thus gets modulated too. A resonance
*      of 0.0 should have an Amp=100 %. Higher resonances should get higher
*      amplify values. You'll have to experiment to find the perfect values (try resonance+100 %).
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt. Negative Effektanteile bewirken keine Dämpfung,
*      sondern eine Verstärkung der zu bearbeitenden Frequenzen.
*    Cut-Off (<a href="../node03.02.01.html">P2</a>)
*      Bereich für die Durchschnittsberechnung. Je breiter dieser ist, desto höher
*      ist die Cut-Off-Frequenz.
*    Resonance (<a href="../node03.02.01.html">P3</a>)
*      Stärke der Resonanz (auch Peak oder Q-Faktor).
*      Da eine starke Resonanz das Signal ausdünnt, gibt es einen Amplifyparameter
*      der parallel zur Resonanz mitläuft,also auch moduliert wird.
*      Bei einer Resonanz von 0 sollte Amp=100 % sein. Bei einer höheren Resonanz
*      sollten größere Werte verwendet werden. Diese lassen sich allerdings nur
*      durch Probieren herausfinden (versuchen sie mal Resonance+100%).
*  Notes.eng
*    These <a href="../node06.03.00.Filter.html">filters</a> are based on a very
*    simple model and are therefore not very precise, but quite fast to calculate.<br>
*    And be careful. If you just hear a loud metallic noise, then you've turned resonance
*    up too far.
*  Hinweise.ger
*    Diese <a href="../node06.03.00.Filter.html">Filter</a> basieren auf einem
*    recht einfachen Modell und sind daher nicht sonderlich genau, dafür aber
*    recht schnell zu berechnen.<br>
*    Und seien sie vorsichtig. Wenn sie nur noch ein lautes metallisches Geräusch
*    hören, haben sie die Resonanz zu weit aufgedreht.
*******
*/

#define FILTER_CRSHIPASS_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Filter-CRSHiPass.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	if(!(instance->rbuf=NewRingBuf((ULONG)(instance->delay),&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	if(!(instance->fbbuf=(SAMPLE *)AllocVec((instance->mrng<<1),MEMF_ANY))) return(FALSE);
	if(!(instance->matrix=(double *)AllocVec((instance->mrng*sizeof(double)),MEMF_ANY))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	AssignBShape(&instance->mod3);
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	memset(instance->fbbuf,0,(instance->mrng<<1));
	instance->fbptr=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,k,curlen;
	double effs,effd,eff,dry;
	double rngs,rngd,rng;
	double fbs,fbd,fb;
	double amps,ampd,amp;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *rbufr,*rbufw,*rbufbase;
	ULONG rbufmask;
	ULONG mrng,hrng;
	LONG flen,val1;
	double sample,mod;
	SAMPLE *fbbuf;
	double *mat;
	LONG tfbptr;
	ULONG fbptr;

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
	rngs=instance->rngs;
	rngd=instance->rngd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	fbs=instance->fbs;
	fbd=instance->fbd;
	amps=instance->amps;
	ampd=instance->ampd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	fbptr=instance->fbptr;
	fbbuf=instance->fbbuf;
	mrng=instance->mrng;
	
	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=  effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		rng=  rngs+rngd*modfunc2(curlen,instance->slen,moddata2);
		mod=			modfunc3(curlen,instance->slen,moddata3);fb =-(fbs +fbd *mod);amp=  amps+ampd*mod;
		eff*=amp;

		// design filter
		hrng=(ULONG)rng;mat=instance->matrix;flen=hrng;			// build matrix
		for(k=0;k<hrng;k++) *(mat++)= (k&1) ? 1.0 : -1.0;
		if(rng!=(double)hrng) {									// if rng is not integer
			*mat=-(rng-(double)hrng);
			if(hrng&1) *mat=-*mat;
			flen++;
		}
		
		// apply filter
		*rbufw=*sbuf;											/* get current value */

		sample=0.0;mat=&instance->matrix[flen];k=flen;					/* apply filter */
		tfbptr=fbptr-flen;if(tfbptr<0) tfbptr+=mrng;
		rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-flen)&rbufmask));
		while(k--) {											/* convolution with source- */
			sample+=(*(mat--)*(double)(*rbufr+fbbuf[tfbptr]));	/* and fb-signal */
			rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
			tfbptr=(tfbptr+1)%mrng;
		}
		sample/=rng;
		val1=(LONG)(((double)*(sbuf++)*dry)+(sample*eff));		/* result */
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		val1=(LONG)(sample*fb);									/* feedback resonancy */
		CLIPINT(val1);
		fbbuf[fbptr]=(WORD)val1;
		fbptr=(fbptr+1)%mrng;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->fbptr=fbptr;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->rbuf)		{ DelRingBuf(instance->rbuf);instance->rbuf=NULL; }
	if(instance->fbbuf)		{ FreeVec((ULONG *)instance->fbbuf);instance->fbbuf=NULL; }
	if(instance->matrix)	{ FreeVec((ULONG *)instance->matrix);instance->matrix=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
