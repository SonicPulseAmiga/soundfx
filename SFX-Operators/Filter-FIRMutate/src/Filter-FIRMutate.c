/****h* Filter-FIRMutate/Filter-FIRMutate.c [4.2] *
*
*  NAME
*    Filter-FIRMutate.c
*  COPYRIGHT
*    $VER: Filter-FIRMutate.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-FIRMutate core file
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
/****u* _operators/Filter-FIRMutate *
*  Function.eng
*    Dampens/boosts the signal. The filter coefficients are taken from src 2.
*    Therefore src 2 controls all the parameters like filter type (lowpass,
*    highpass, ...), the cut-off frequencies, the filter slope and so on. 
*  Funktion.ger
*    Dämpft/Boostet das Signal. Die Filterkoeffizienten werden src 2 entnommen.
*    Damit kontrolliert src 2 alle Parameter wie Filtertyp (Tiefpass, Hochpass,
*     ...), die Cut-Off Frenquenzen, die Flankensteilheit und so weiter.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Filter-Offset (<a href="../node03.02.01.html">P2</a>)
*      Modulates the point in the scr 2 sample, where the operator starts
*      retrieving the filter coefficients.
*    Filter-Stretch (<a href="../node03.02.01.html">P3</a>)
*      Modifies the mapping from samplevalues to coefficients.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*    Nr. (Length)
*      How many coefficients should be used. The more they are, the better results will be get.(max 1024, but 64 is usually enough)
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      how to calculate (smooth) inbetween data
*    Ampf
*      final amplification
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Filter-Offset (<a href="../node03.02.01.html">P2</a>)
*      Moduliert den Punkt im src 2 sample, wo der Operator beginnt die
*      Filterkoeffizienten zu entnehmen.
*    Filter-Stretch (<a href="../node03.02.01.html">P3</a>)
*      Ändert die Abbildung von den Samplewerten zu den Filterkoeffizienten.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      welche Fensterfunktion verwendet wird.
*    Nr. (Length)
*      Wieviele Koeffizienten benutzt werden sollen. Je mehr Koeffizienten benutzt werden,
*      desto besser (max. 1024, 64 ist in der Regel ausreichend)
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      wie sollen (sanfte) Zwischenwerte berechnet werden
*    Ampf
*      abschließende Lautstärkeanpassung
*  Notes.eng
*    A <a href="../node06.03.00.Filter.html">filter</a> such as this, is more an experimental thing.
*    There is nearly no way to know the result before. Good results are achived
*    by e.g. changing the filter-offset very little (e.g. linear from 0.0 to 0.1)
*    or using a relativ short sample for scr 2. Furthermore it sounds interessting
*    to blend the filter-stretch from e.g. 0.125 to 8.0.<br>
*    Curved-interpolation is useful, when using very short sample for src 2 or
*    small filter-offset changes.
*  Hinweise.ger
*    Ein <a href="../node06.03.00.Filter.html">Filter</a> wie dieser, ist sehr experimentell.
*    Es ist nahezu unmöglich vorher zu wissen wie das Ergebnis klingen wird.
*    Gute Resultate erhält man, wenn man z.B. den Filter-Offset sehr wenig ändert
*    (z.B. linear von 0.0 zu 0.1) oder ein relativ kurzes Sample für src 2
*    verwendet. Weiterhin ist es interessant den filter-stretch Wert von z.B.
*    0.125 nach 8.0 überzublenden.<br>
*    Kurven-Interpolation ist nützlich, wenn man sehr kurze Samples für src 2
*    verwendet oder mit kleinen filter-offset Werten arbeitet.
*******
*/

#define FILTER_FIRMUTATE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Filter-FIRMutate.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	unsigned long i;
	
	AssignInterpolFunc(instance->int1.type,&(instance->interpolfunc));
	if(!(instance->win_fkt=NewWindowFkt((instance->anz<<1),instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
	if(!(instance->rbuf1=NewRingBuf(instance->anz,&instance->rbufbase1,&instance->rbufmask1,&instance->rbufsize1))) return(FALSE);
	if(!(instance->rbuf2=NewRingBuf(((ULONG)((double)instance->anz*instance->mstr)+(ULONG)(instance->int1.range)),&instance->rbufbase2,&instance->rbufmask2,&instance->rbufsize2))) return(FALSE);

	for(i=0;i<(instance->anz<<1);i++) instance->win_fkt[i]/=32768.0;
	return(TRUE);	
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	AssignBShape(&instance->mod3);
	memset(instance->rbuf1,0,(instance->rbufsize1<<1));
	instance->rbufw1=instance->rbufbase1;
	memset(instance->rbuf2,0,(instance->rbufsize2<<1));
	instance->rbufw2=instance->rbufbase2;
	instance->sbuf2=instance->src2.srcbuf->vbuf[ch];		// has to be in memory !!!
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	double offs,offd,off;
	double strs,strd,str;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	InterpolFuncPtr	interpolfunc;
	SAMPLE *dbuf,*sbuf1,*sbuf2;
	SAMPLE *rbufw1,*rbufr1,*rbufw2;
	SAMPLE *rbufbase1;
	ULONG rbufmask1;
	SAMPLE *rbufbase2;
	ULONG rbufmask2;
	ULONG irange;
	double rbufr2;
	double sample,offset,ival;
	ULONG mstr;
	LONG val1;
	UWORD anz,anz2;
	double ampf,moffs;
	double *wptr;
	ULONG rm_start2,rm_len2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf1=instance->sbuf1;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	offs=instance->offs;
	offd=instance->offd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	strs=instance->strs;
	strd=instance->strd;
	//
	irange=(ULONG)(instance->int1.range);
	interpolfunc=instance->interpolfunc;
	rbufbase1=instance->rbufbase1;
	rbufmask1=instance->rbufmask1;
	rbufw1=instance->rbufw1;	
	rbufbase2=instance->rbufbase2;
	rbufmask2=instance->rbufmask2;
	anz=instance->anz;anz2=(anz<<1)-1;
	rm_start2=instance->rm_start2;
	rm_len2=instance->rm_len2;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;eff*=ampf;
		off=offs+offd*modfunc2(curlen,instance->slen,moddata2);
		str=strs+strd*modfunc3(curlen,instance->slen,moddata3);if(str<0.0) str=-str;

		offset=(double)rm_start2+(off*(double)rm_len2);
		moffs=((double)anz*str);
		sbuf2=&instance->sbuf2[(ULONG)offset];
		rbufw2=rbufbase2;
		for(mstr=moffs+irange;mstr>0;mstr--) {
			*rbufw2=*(sbuf2++);											// get current value
			rbufw2=(SAMPLE *)((ULONG)rbufbase2|((ULONG)(rbufw2+1)&rbufmask2));
		}

		*rbufw1=*sbuf1;											/* get current value */

		rbufr1=(SAMPLE *)((ULONG)rbufbase1|((ULONG)(rbufw1-anz)&rbufmask1));
		wptr=&instance->win_fkt[anz-1];
		rbufr2=(double)Bytes2Samples((ULONG)rbufw2&rbufmask2);	/* only Offset to baseaddr */
		sample=0.0;anz2=anz;									/* apply filter */
		while(anz2--) {
			ival   =interpolfunc(rbufbase2,&rbufr2,rbufmask2,irange);
			sample+=ival*(*(wptr--))*((double)*rbufr1);
			/** @todo: another compiler bug in SAS-C */
			//sample+=(interpolfunc(rbufbase2,&rbufr2,rbufmask2,irange)*(*(wptr--))*((double)*rbufr1));
			rbufr1=(SAMPLE *)((ULONG)rbufbase1|((ULONG)(rbufr1+1)&rbufmask1));
			rbufr2-=str;
		}
		val1=(LONG)((double)*(sbuf1++)*dry+(sample*eff));
		CLIPINT(val1);
		*(dbuf++)=(WORD)val1;

		rbufw1=(SAMPLE *)((ULONG)rbufbase1|((ULONG)(rbufw1+1)&rbufmask1));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw1=rbufw1;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->win_fkt)	{ DelWindowFkt(instance->win_fkt);instance->win_fkt=NULL; }
	if(instance->rbuf1)		{ DelRingBuf(instance->rbuf1);instance->rbuf1=NULL; }
	if(instance->rbuf2)		{ DelRingBuf(instance->rbuf2);instance->rbuf2=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
