/****h* MultiDelay/MultiDelay.c [4.2] *
*
*  NAME
*    MultiDelay.c
*  COPYRIGHT
*    $VER: MultiDelay.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    MultiDelay core file
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
/****u* _operators/MultiDelay *
*  Function.eng
*    Generates up to 8 Delays at once.
*  Funktion.ger
*    Erzeugt bis zu 8 Echoverzögerungen
*  Parameter.eng
*    Delay (Del1...8)
*      delaytime.
*    Volume (Amp1...8)
*      how loud should this delay be
*    Fb Local (FbL1...8)
*      how much of the result is feeback into the delay. This may be negative producing an inverted feedback.
*    Fb Global (FbG1...8)
*      how much of the result is feeback into the operator. This may be negative producing an inverted feedback.
*    Ampf
*      final amplification
*    Dry
*      how loud should the source be mixed in
*    Num
*      how many delays should be used
*  Parameter.ger
*    Delay (Del1...8)
*      Verzögerung
*    Volume (Amp1...8)
*      wie laut soll dieses Delay sein
*    Fb Local (FbL1...8)
*      wie viel vom Ergebnis in das Delay zurückgeführt wird. Dies kann auch
*      negativ sein um ein inverses Feedback zu erzeugen.
*    Fb Global (FbG1...8)
*      wie viel vom Ergebnis in den Operator zurückgeführt wird. Dies kann auch
*      negativ sein um ein inverses Feedback zu erzeugen.
*    Ampf
*      abschließende Lautstärkeanpassung
*    Dry
*      wie laut soll das Ausgangssample mit eingemischt werden
*    Num
*      wie vile Delays sollen verwendet werden
*  Notes.eng
*    As a novelty since V 3.4 you can enter the delay-time as notes too. Just
*    load the preset "Resonate-CEG". With those settings you let the sample
*    resonate on the c-major chord. This gets more clearer, if you run it twice,
*    but its strongly recommended to process the source sample with <a href="Middle.html">Middle</a> before.
*  Hinweise.ger
*    Als Neuheit seit V 3.4 können Sie auch hier Noten als Delayzeit eigeben.
*    Laden Sie doch mal das Preset "Resonate-CEG". Mit diesen Einstellungen
*    lassen Sie ein Sample im C-Dur Akkord resonieren. Der Effekt wird noch
*    deutlicher, wenn Sie den Operator zweimal ausführen. Das Ausgangssample
*    sollte unbedingt vorher mit <a href="Middle.html">Middle</a> bearbeitet werden.
*******
*/

#define MULTIDELAY_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "MultiDelay.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	unsigned char i;

	for(i=0;i<instance->anz;i++) if(!(instance->rbuf[i]=NewRingBuf(instance->delay[i],&instance->rbufbase[i],&instance->rbufmask[i],&instance->rbufsize[i]))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	unsigned char i;

	for(i=0;i<instance->anz;i++) {
		memset(instance->rbuf[i],0,(instance->rbufsize[i]<<1));
		instance->rbufw[i]=instance->rbufbase[i];
		instance->rbufr[i]=instance->rbufbase[i]+(Bytes2Samples(instance->rbufsize[i])-instance->delay[i]);
	}
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned char k;
	SAMPLE *dbuf,*sbuf;
	SAMPLE **rbufw,**rbufr;
	double in,out,sumfb;
	double ampf,dry;
	LONG val1;
	SAMPLE **rbufbase;
	ULONG *rbufmask;
	double *fbl,*fbg,*dr,*amp;
	double *out2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	ampf=instance->ampf;
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	rbufr=instance->rbufr;
	dry=instance->dry;
	fbl=instance->fbl;
	fbg=instance->fbg;
	dr=instance->dr;
	amp=instance->amp;
	out2=instance->out2;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		in=(double)*(sbuf++);
		out=in*dry;
		sumfb=0.0;

		//-- read from delays
		for(k=0;k<instance->anz;k++) {
			out2[k]=(double)*rbufr[k];			/* read from delay n */
			out  +=(out2[k]*amp[k]);			/* add to output with given intensity */
			sumfb+=out2[k];
			rbufr[k]=(SAMPLE *)((ULONG)rbufbase[k]|((ULONG)(rbufr[k]+1)&rbufmask[k]));
		}
		sumfb/=(double)instance->anz;			/* scale glob. feedback sum */
		//-- write into delays
		for(k=0;k<instance->anz;k++)	{
			val1=(LONG)((in*dr[k]+out2[k]*fbl[k]+sumfb*fbg[k])/(1.0+fbg[k]));
			CLIPINT(val1);
			*rbufw[k]=(SAMPLE)val1;
			/**rbufw[k]=(SAMPLE)((in*dr[k]+out2[k]*fbl[k]+sumfb)/sumampf); */
			rbufw[k]=(SAMPLE *)((ULONG)rbufbase[k]|((ULONG)(rbufw[k]+1)&rbufmask[k]));
		}

		val1=(LONG)out*ampf;
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	unsigned char i;

	for(i=0;i<instance->anz;i++) if(instance->rbuf[i]) { DelRingBuf(instance->rbuf[i]);instance->rbuf[i]=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
