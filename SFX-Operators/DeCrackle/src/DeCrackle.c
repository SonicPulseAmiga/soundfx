/****h* DeCrackle/DeCrackle.c [4.3] *
*
*  NAME
*    DeCrackle.c                                                        
*  COPYRIGHT
*    $VER: DeCrackle.c 4.3 (05.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    DeCrackle core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    05.Mar.2004	V 4.3	most recent version
*							A: dry-run
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*     problem 1:
*       there should be not crackles right in the beginning as we have not yet
*       a picture of what sounds normal
*     problem 2:
*       if we initially detect cracks
*       -> then we don't get any sensible amp,dif-envelopes
*          (if we don't update envelopes when adjusting)
*     problem 3:
*      	if we adjust by 100%
*       -> then dif becomes 0% and next dif is very like detected as a crack as well
*          (if amp is around 0 at that time we get a flat line
*  IDEAS
*   * Analyser
*     * AmpEnv Threshold
*     * DeltaAmpEnv Threshold
*     * DeltaSpectrogramm Threshold
*       * FFT, avg. delta der Spektralwerte zu einem Zeitpunkt 'klein'
*		* und avg. amp relativ hoch ?
*     * Spectrogramm Ripple
*       * FFT, wenn keine harmonischen Peaks sondern Rauschen
*   * What are crackles
*     1.) short bursts
*         -> crack_len
*     2.) high frequent burst
*         -> high diff value
*     3.) crackles on records reappear with similliar amplitude after the
*         same time
*
*
*******
*/
/****u* _operators/DeCrackle *
*  Function.eng
*    Dampens strong leveljumps (crackles)
*  Funktion.ger
*    Dämpft starke Pegelsprüge (Knackser)
*  Parameter.eng
*    Dif.
*      Leveljump-threshold. If the detected leveljump lays this much above
*      the average leveljumps in current area, it will dampened.
*    Amp.
*      Amplitude-threshold. If the current amplitudes lays this much above
*      the average amplitudes in current area, it will dampened.
*    Adjust
*      How strong should the crack be dampened. 100 % means fully cancelation.
*    Size
*      The maximum length of an anormal signal to be considered as a crackle.
*      Crackles are normaly relative short. This Parameter is use to separate
*      crackles from percussive sounds.
*    Test
*      Starts the operator without modifying the sample and shows the results of the crackle-analysis.
*    Stat.
*      The amount of crackles detected (absolut and relative to the length) for each channel of the sample.
*  Parameter.ger
*    Dif.
*      Pegelsprungschwellwert. Sobald ein Pegelsprung gegenüber den durchschnitlichen Pegelsprungwerten in der aktuellen
*      Umgebung soviel über diesem Wert liegt, wird er gedämpft.
*    Amp.
*      Amplitudenschwellwert. Sobald die aktuelle Amplitude gegenüber der Durchschnittsamplitude der aktuellen
*      Umgebung soviel über diesem Wert liegt, wird er gedämpft.
*    Adjust
*      Wie stark der Knackser gedämpft werden soll. 100 % entspricht der totalen Auslöschung.
*    Size
*      The maximale Länge die ein Störsignal haben darf um als Knackser eingestuft zu werden.
*      Knackser sind normalerweise sehr kurz. Dieser Parameter dient dazu Knackser
*      von percusiven Klängen zu unterscheiden.
*    Test
*      Startet den Operator ohne das Sample zu verändern und zeigt die Ergebnisse der Knackseranalyse an.
*    Stat.
*      Die Menge der gefundenen Knackser (absolut und in Prozent relativ zur Länge) für jeden Kanal des Samples.
*  Notes.eng
*    This operator detects cracks in samples and makes them quieter.
*    Such cracks are contained in samples recorded from a longplayer
*    or can be caused by r/w-errors on a disk.<br>
*    Before using this operator, it is recommended to apply the
*    <a href="Middle.html">Middle</a> operator, followed by the
*    <a href="ZeroPass.html">ZeroPass</a> operator and finally the
*    <a href="Amplify.html">Amplify</a> operator with the MaxVol function,
*    to prepare the sample.<br>
*    If the result obtained by this operator sounds damp and misses attacks,
*    then raise the Dif. and Amp. values, so that fewer signals are
*    interpreted as crackles. If obvious crackles are not supressed zoom into
*    one and look at their length. Then adjust the size parameter accordingly.
*    You can use the Test function to tune the results.
*  Hinweise.ger
*    Dieser Operator entfernt bzw. dämpft Knackstellen in Samples.
*    Diese treten z.B. auf wenn man von einer Schallplatte sampelt
*    oder mal einen R/W-Fehler auf einem Datenträger hatte.<br>
*    Bevor man diesen Operator nutzt empfiehlt es sich, erst den
*    <a href="Middle.html">Middle</a> Operator, gefolgt vom
*    <a href="ZeroPass.html">ZeroPass</a> Operator und abschließend den
*    <a href="Amplify.html">Amplify</a> Operator mit der MaxVol Funktion
*    anzuwenden, um das Sample vorzubereiten.
*    Wenn die Ergebnisse dieses Operators dumpf klingen und die Anschläge fehlen,
*    dann erhöhen sie die Dif. und Amp. Werte, so das weniger Signale als
*    Knackser interpretiert werden. Wenn offensichtliche Knackser nicht entfernt werden,
*    vergrößern sie einen Solchen und betrachten sie die Länge. Stellen sie den
*    Size Parameter neu ein.
*    Sie können die Test Funktion benutzen um die Ergebnisse abzustimmen.
*******
*/

#define DECRACKLE_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "DeCrackle.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- globals ------------------------------------------------------------------

//DEBUG
FILE *__dbg_fh1;
FILE *__dbg_fh2;
FILE *__dbg_fh3;
SAMPLE __dbg_sample;
LONG __dbg_val;
//DEBUG

//-- definitions --------------------------------------------------------------

#define process_common_vars \
	unsigned long curlen; \
	LONG lval,tlval; \
	double amp,camp,propa,tamp,thresha; \
	double dif,cdif,propd,tdif,threshd;
	double nrj,cnrj,propn,tnrj; \
	double adj,d1,d2; \
	double low,mid,high; \
	BOOL crackle_scan; \
	SAMPLE *rbufbase; \
	SAMPLE *rbufr,*rbufw,*rbufcr,*rbufcw; \
	ULONG rbufmask,rbufsize; \
	ULONG cracklesize,crackle_len_ct; \
//


//#define process_one_input(val) \
#define process_one_input_with_highpass(val) \
	/* apply high pass filter in input */		\
	high=((double)(val))-mid-low;				\
	mid+=(high*0.25);low+=(mid*0.25);			\
												\
	/* get amplitude */							\
	camp=fabs(high);							\
	if(amp<0.25) amp=0.25;						\
	propa=camp/amp;								\
	amp=(camp>amp)?camp:(ENVFC_A1*amp+ENVFC_A2*camp);\
												\
	/* get delta */								\
	cdif=(double)labs(lval-(LONG)high);			\
	if(dif<0.25) dif=0.25;						\
	propd=cdif/dif;								\
	dif=(cdif>dif)?cdif:(ENVFC_A1*dif+ENVFC_A2*cdif);\
												\
	lval=(LONG)high;							\
//

#define process_one_input_direct(val) \
	/* get amplitude */							\
	camp=(double)labs((LONG)(val));				\
	if(amp<0.25) amp=0.25;						\
	propa=camp/amp;								\
	amp=(camp>amp)?camp:(ENVFC_A1*amp+ENVFC_A2*camp);\
												\
	/* get delta */								\
	cdif=(double)labs(lval-(LONG)(val));		\
	if(dif<0.25) dif=0.25;						\
	propd=cdif/dif;								\
	dif=(cdif>dif)?cdif:(ENVFC_D1*dif+ENVFC_D2*cdif);\
												\
	/* get energy */							\
	cnrj=(camp/32768.0);cnrj*=cnrj;				\
	if(nrj<0.25) nrj=0.25;						\
	propn=cnrj/nrj;								\
	nrj=(cnrj>nrj)?cnrj:(ENVFC_A1*nrj+ENVFC_A2*cnrj);\
												\
	lval=(LONG)(val);							\
//

#define process_one_input(val) \
	/* get enery amplitude */				    \
	camp=(double)(val)/32768.0;camp*=camp;		\
	if(amp==0.0) amp=0.00001;					\
	propa=camp/amp;								\
	amp=(camp>amp)?camp:(ENVFC_A1*amp+ENVFC_A2*camp);\
												\
	/* get energy delta */						\
	d1=(double)( val)/32768.0;d1=(d1<0)?-(d1*d1):(d1*d1);	\
	d2=(double)(lval)/32768.0;d2=(d2<0)?-(d2*d2):(d2*d2);	\
	cdif=fabs(d2-d1);							\
	if(dif==0.0) dif=0.00001;					\
	propd=cdif/dif;								\
	dif=(cdif>dif)?cdif:(ENVFC_D1*dif+ENVFC_D2*cdif);\
												\
	lval=(LONG)(val);							\
//


#define possible_crackle_start(unused) ((propa>=thresha) && (propd>=threshd))
//#define possible_crackle_start(unused) ((propa>=thresha) || (propd>=threshd))
//#define possible_crackle_end(unused) 	((propa<thresha) || (propd<threshd))
#define possible_crackle_end(unused)   ((amp<=instance->samp) && (dif<=instance->sdif))

#define store_state(unused) \
	instance->samp=tamp;						\
	instance->sdif=tdif;						\
	instance->snrj=tnrj;						\
	instance->sval=tlval;						\
//

#define recall_state(unused) \
	amp=instance->samp;							\
	dif=instance->sdif;							\
	nrj=instance->snrj;							\
	lval=instance->sval;						\
//

#define __DEBUG_WRITE_VALUE(fh,val) __dbg_val=(LONG)(val);CLIPINT(__dbg_val);__dbg_sample=(SAMPLE)__dbg_val;fwrite(&__dbg_sample,sizeof(SAMPLE),1,fh);

#define __DEBUG_PROCESS_VALS \
	__DEBUG_WRITE_VALUE(__dbg_fh1,propa* 4096.0);									\
	__DEBUG_WRITE_VALUE(__dbg_fh1,amp  *32767.0);									\
	/*__DEBUG_WRITE_VALUE(__dbg_fh1,camp *   1.0);*/								\
	__DEBUG_WRITE_VALUE(__dbg_fh1,(propa>=thresha)?32767:0);    					\
	if(crackle_found) { __DEBUG_WRITE_VALUE(__dbg_fh1,-32768);crackle_found=FALSE; }\
	else {				__DEBUG_WRITE_VALUE(__dbg_fh1,crackle_scan?32767:0); }		\
																					\
	__DEBUG_WRITE_VALUE(__dbg_fh2,propd* 1024.0);									\
	__DEBUG_WRITE_VALUE(__dbg_fh2,dif  *32768.0);									\
	/*__DEBUG_WRITE_VALUE(__dbg_fh2,cdif *   1.0);*/								\
	__DEBUG_WRITE_VALUE(__dbg_fh2,(propd>=threshd)?32767:0);						\
	/*__DEBUG_WRITE_VALUE(__dbg_fh2,crackle_len_ct<<6);*/							\
	__DEBUG_WRITE_VALUE(__dbg_fh2,0.0);												\
																					\
	/*__DEBUG_WRITE_VALUE(__dbg_fh3,propn*8192.0);*/									\
	/*__DEBUG_WRITE_VALUE(__dbg_fh3,nrj  *32767.0);*/									\
	/*__DEBUG_WRITE_VALUE(__dbg_fh3,(propn>=thresha)?32767:0);*/    					\
	/*__DEBUG_WRITE_VALUE(__dbg_fh3,0.0);*/												\
//


int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	UBYTE l;
	ULONG rbuflen;

	//-- reset crackle counters
	for(l=0;l<CH_MAX;l++) instance->crackle_cts[l]=0;

	instance->fadesize=instance->srat/1000.0;	// 1 ms
	MSG1("fadesize=%ld",instance->fadesize);
	rbuflen=instance->cracklesize+instance->cracklesize+instance->fadesize;
	if(!(instance->rbuf=NewRingBuf(rbuflen,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	MSG1("rbuflen=%ld",rbuflen);

	//DEBUG
	__dbg_fh1=fopen("t:sfx_dbg1.sraw16","wb");
	__dbg_fh2=fopen("t:sfx_dbg2.sraw16","wb");
	//__dbg_fh3=fopen("t:sfx_dbg3.sraw16","wb");
	//DEBUG

	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	instance->lval=instance->sval=0;
	instance->amp=instance->samp=0.0;
	instance->dif=instance->sdif=0.0;
	instance->nrj=instance->snrj=0.0;

	instance->crackle_ct=0;
	instance->crackle_len_ct=0;
	instance->crackle_scan=FALSE;

	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	instance->rbufr=instance->rbufbase;
	//init with cracklesize delay
	//instance->rbufr=(SAMPLE *)((ULONG)instance->rbufbase+(instance->rbufsize-Samples2Bytes(instance->cracklesize-1)));
	MSG1("rbufsize=%ld",instance->rbufsize);
	MSG2("rbufw,rbufr=%ld,%ld",Bytes2Samples(instance->rbufw-instance->rbufbase),Bytes2Samples(instance->rbufr-instance->rbufbase));

	instance->low=instance->mid=instance->high=0.0;

	instance->spos=instance->dpos=0;
}

void process_dry(SFXCoreInstance *instance,unsigned long samples) {
	SAMPLE *sbuf;
	ULONG spos,dpos;
	BOOL crackle_found=FALSE;
	process_common_vars

	curlen=instance->curlen;

	// Source
	sbuf=instance->sbuf;
	//
	lval=instance->lval;
	amp=instance->amp;thresha=instance->thresha;
	dif=instance->dif;threshd=instance->threshd;
	nrj=instance->nrj;
	//	
	adj=instance->adj;
	crackle_scan=instance->crackle_scan;
	crackle_len_ct=instance->crackle_len_ct;
	cracklesize=instance->cracklesize;
	spos=instance->spos;
	dpos=instance->dpos;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	low=instance->low;
	mid=instance->mid;
	high=instance->high;

	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {

		*rbufw=*sbuf;										/* fill the ringbuffer */

		//-- prepare for eventual store_state
		tamp=amp;
		tdif=dif;
		tnrj=nrj;
		tlval=lval;

		//-- get amplitude and delta
		process_one_input(*sbuf);

		//DEBUG
		__DEBUG_PROCESS_VALS
		//if((curlen>51190) && (curlen<51210)) {
		//	  MSG2("curlen=%6ld curval=%5d",curlen,*sbuf);
		//	  MSG4("amp=%9.4f camp=%9.4f samp=%9.4f  propa=%6.4f",amp,camp,instance->samp,propa);
		//	  MSG4("dif=%9.4f cdif=%9.4f sdif=%9.4f  propd=%6.4f",dif,cdif,instance->sdif,propd);
		//}
		//DEBUG


		if(crackle_scan) {
			crackle_len_ct++;
			// has end of anormality been reached
			if(possible_crackle_end(0)) {
				//-- restore saved state
				//##// recall_state(0);
				//-- count crackles
				instance->crackle_ct++;
				crackle_found=TRUE;
				// scan for new crackles
				crackle_scan=FALSE;
				crackle_len_ct=0;
			}
			else {
				// it is too long for a crackle
				if(crackle_len_ct==cracklesize) {
					//-- rescan ringbuffer if a later possition might be the start of a crackle
					crackle_len_ct--;
					rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-crackle_len_ct))&rbufmask));
					ASSERT((ULONG)rbufcr>=(ULONG)rbufbase);ASSERT((ULONG)rbufcr<((ULONG)rbufbase+rbufsize));
					//-- restore saved state
					recall_state(0);
					while(rbufcr!=rbufw) {
						//-- get amplitude and delta
						process_one_input(*rbufcr);

						//-- we not inside a possible crackle
						if(possible_crackle_start(0)) {
							//-- we assume this might be the start of a crackle, as the signal is 100%+thresh% over agerage
							//-- save current state
							store_state(0);
							//MSG1("restart with crackle_len=%ld",crackle_len_ct);
							break;
						}
						crackle_len_ct--;
						rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufcr+1)&rbufmask));
					}
					if(rbufcr==rbufw) crackle_scan=FALSE;
				}
			}
		}
		else {
			//-- we not inside a possible crackle
			if(possible_crackle_start(0)) {
				//-- we assume this might be the start of a crackle, as the signal is 100%+thresh% over agerage
				crackle_scan=TRUE;
				crackle_len_ct=0;
				//-- save current state
				store_state(0);
			}
		}

		if(curlen>=cracklesize) {
			dpos++;
		}
		sbuf++;spos--;curlen++;instance->curlen++;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
	}
	instance->lval=lval;
	instance->amp=amp;
	instance->dif=dif;
	instance->nrj=nrj;
	instance->crackle_scan=crackle_scan;
	instance->crackle_len_ct=crackle_len_ct;
	instance->rbufw=rbufw;
	instance->spos=spos;
	instance->dpos=dpos;
	instance->sbuf=sbuf;
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
}

void process_dry_rest(SFXCoreInstance *instance,unsigned long samples) {
	SAMPLE *sbuf;
	ULONG dpos;
	BOOL crackle_found=FALSE;
	process_common_vars

	curlen=instance->curlen;

	// Source
	sbuf=instance->sbuf;
	//
	lval=instance->lval;
	amp=instance->amp;thresha=instance->thresha;
	dif=instance->dif;threshd=instance->threshd;
	nrj=instance->nrj;
	//	
	adj=instance->adj;
	crackle_scan=instance->crackle_scan;
	crackle_len_ct=instance->crackle_len_ct;
	cracklesize=instance->cracklesize;
	dpos=instance->dpos;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	low=instance->low;
	mid=instance->mid;
	high=instance->high;

	while((dpos<samples) && (instance->rest>0)) {

		*rbufw=0;										/* fill the ringbuffer */

		//-- prepare for eventual store_state
		tamp=amp;
		tdif=dif;
		tnrj=nrj;
		tlval=lval;

		//-- get amplitude and delta
		process_one_input(0);

		//DEBUG
		__DEBUG_PROCESS_VALS
		//DEBUG

		if(crackle_scan) {
			crackle_len_ct++;
			// has end of anormality been reached
			if(possible_crackle_end(0)) {
				//-- restore saved state
				//##// recall_state(0);
				//-- count crackles
				instance->crackle_ct++;
				crackle_found=TRUE;
				// scan for new crackles
				crackle_scan=FALSE;
				crackle_len_ct=0;
			}
			else {
				// it is too long for a crackle
				if(crackle_len_ct==cracklesize) {
					//-- rescan ringbuffer if a later possition might be the start of a crackle
					crackle_len_ct--;
					rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-crackle_len_ct))&rbufmask));
					ASSERT((ULONG)rbufcr>=(ULONG)rbufbase);ASSERT((ULONG)rbufcr<((ULONG)rbufbase+rbufsize));
					//-- restore saved state
					recall_state(0);
					while(rbufcr!=rbufw) {
						//-- get amplitude and delta
						process_one_input(*rbufcr);

						//-- we not inside a possible crackle
						if(possible_crackle_start(0)) {
							//-- we assume this might be the start of a crackle, as the signal is 100%+thresh% over agerage
							//-- save current state
							store_state(0);
							//MSG1("restart with crackle_len=%ld",crackle_len_ct);
							break;
						}
						crackle_len_ct--;
						rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufcr+1)&rbufmask));
					}
					if(rbufcr==rbufw) crackle_scan=FALSE;
				}
			}
		}
		else {
			//-- we not inside a possible crackle
			if(possible_crackle_start(0)) {
				//-- we assume this might be the start of a crackle, as the signal is 100%+thresh% over agerage
				crackle_scan=TRUE;
				crackle_len_ct=0;
				//-- save current state
				store_state(0);
			}
		}

		if(curlen>=cracklesize) {
			dpos++;
		}
		curlen++;instance->rest--;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
	}
	instance->lval=lval;
	instance->amp=amp;
	instance->dif=dif;
	instance->nrj=nrj;
	instance->crackle_scan=crackle_scan;
	instance->crackle_len_ct=crackle_len_ct;
	instance->rbufw=rbufw;
	instance->dpos=dpos;
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	SAMPLE *dbuf,*sbuf;
	ULONG spos,dpos;
	ULONG fadesize,fadect,fadeend;
	double fader,fadew,fadeis,fadeos;
	process_common_vars

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	lval=instance->lval;
	amp=instance->amp;thresha=instance->thresha;
	dif=instance->dif;threshd=instance->threshd;
	nrj=instance->nrj;
	//
	adj=instance->adj;
	crackle_scan=instance->crackle_scan;
	crackle_len_ct=instance->crackle_len_ct;
	cracklesize=instance->cracklesize;
	fadesize=instance->fadesize;
	spos=instance->spos;
	dpos=instance->dpos;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	rbufr=instance->rbufr;
	//
	low=instance->low;
	mid=instance->mid;
	high=instance->high;

	//MSG3("  > cracklescan=%1d  rbufr=%4ld : %6ld",crackle_scan,Bytes2Samples(rbufr-rbufbase),*rbufr);

	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=*sbuf;										/* fill the ringbuffer */

		//-- prepare for eventual store_state
		tamp=amp;
		tdif=dif;
		tnrj=nrj;
		tlval=lval;

		//-- get amplitude and delta
		process_one_input(*sbuf);

		if(crackle_scan) {
			crackle_len_ct++;
			// has end of anormality been reached
			if(possible_crackle_end(0)) {
				//-- go back crackle_len_ct values in rbuf
				rbufcw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-( crackle_len_ct    +fadesize)))&rbufmask));
				rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-((crackle_len_ct<<1)+fadesize)))&rbufmask));
				//-- prepare cross fade
				fader=0.0;fadew=1.0-fader;fadect=0;
				fadeis=adj/fadesize;
				if(crackle_len_ct>fadesize) { fadeend=fadesize+(crackle_len_ct-fadesize);fadeos=adj/fadesize; }
				else                        { fadeend=fadesize;fadeos=adj/crackle_len_ct; }
				//MSG4("  : curlen=%8ld  rbufw,rbufcr,rbufcw=%4ld,%4ld,%4ld",curlen,Bytes2Samples(rbufw-rbufbase),Samples2Bytes(rbufcr-rbufbase),Samples2Bytes(rbufcw-rbufbase));
				while(rbufcw!=rbufw) {
					ASSERT((ULONG)rbufcr>=(ULONG)rbufbase);ASSERT((ULONG)rbufcr<((ULONG)rbufbase+rbufsize));
					ASSERT((ULONG)rbufcw>=(ULONG)rbufbase);ASSERT((ULONG)rbufcw<((ULONG)rbufbase+rbufsize));
					//-- correct crackle
					*rbufcw=((*rbufcw)*fadew)+((*rbufcr)*fader);
					if(fadect>=fadeend)		  fader-=fadeos;		// fade out
					else if (fadect<fadesize) fader+=fadeis;		// fade in
					fadew=1.0-fader;fadect++;

					rbufcw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufcw+1)&rbufmask));
					rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufcr+1)&rbufmask));
				}
				//-- restore saved state
				//##//recall_state(0);
				//-- count crackles
				//instance->crackle_ct++;	 // only count in process_dry()
				// scan for new crackles
				crackle_scan=FALSE;
			}
			else {
				// it is too long for a crackle
				if(crackle_len_ct==cracklesize) {
					//-- rescan ringbuffer if a later possition might be the start of a crackle
					crackle_len_ct--;
					rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-crackle_len_ct))&rbufmask));
					ASSERT((ULONG)rbufcr>=(ULONG)rbufbase);ASSERT((ULONG)rbufcr<((ULONG)rbufbase+rbufsize));
					//-- restore saved state
					recall_state(0);
					while(rbufcr!=rbufw) {
						//-- get amplitude and delta
						process_one_input(*rbufcr);

						//-- we not inside a possible crackle
						if(possible_crackle_start(0)) {
							//-- we assume this might be the start of a crackle, as the signal is 100%+thresh% over agerage
							//-- save current state
							store_state(0);
							//MSG1("restart with crackle_len=%ld",crackle_len_ct);
							break;
						}
						crackle_len_ct--;
						rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufcr+1)&rbufmask));
					}
					if(rbufcr==rbufw) crackle_scan=FALSE;
				}
			}
		}
		else {
			//-- we not inside a possible crackle
			if(possible_crackle_start(0)) {
				//-- we assume this might be the start of a crackle, as the signal is 100%+thresh% over agerage
				crackle_scan=TRUE;
				crackle_len_ct=0;
				//-- save current state
				store_state(0);
			}
		}

		if(curlen>=cracklesize) {
			// write back delayed value from ring-buffer
			*(dbuf++)=*rbufr;
			dpos++;
			rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
		}
		sbuf++;spos--;curlen++;instance->curlen++;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	//MSG3("  < cracklescan=%1d  rbufr=%4ld : %6ld",crackle_scan,Bytes2Samples(rbufr-rbufbase),*rbufr);

	instance->lval=lval;
	instance->amp=amp;
	instance->dif=dif;
	instance->nrj=nrj;
	instance->crackle_scan=crackle_scan;
	instance->crackle_len_ct=crackle_len_ct;
	instance->rbufw=rbufw;
	instance->rbufr=rbufr;
	instance->spos=spos;
	instance->dpos=dpos;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
}

void process_rest(SFXCoreInstance *instance,unsigned long samples) {
	SAMPLE *dbuf,*sbuf;
	ULONG dpos;
	ULONG fadesize,fadect,fadeend;
	double fader,fadew,fadeis,fadeos;
	process_common_vars
	
	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	//
	lval=instance->lval;
	amp=instance->amp;thresha=instance->thresha;
	dif=instance->dif;threshd=instance->threshd;
	nrj=instance->nrj;
	//
	adj=instance->adj;
	crackle_scan=instance->crackle_scan;
	crackle_len_ct=instance->crackle_len_ct;
	cracklesize=instance->cracklesize;
	fadesize=instance->fadesize;
	dpos=instance->dpos;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	rbufr=instance->rbufr;
	//
	low=instance->low;
	mid=instance->mid;
	high=instance->high;

	while((dpos<samples) && (instance->rest>0)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=0;										/* fill the ringbuffer */

		//-- prepare for eventual store_state
		tamp=amp;
		tdif=dif;
		tnrj=nrj;
		tlval=lval;

		//-- get amplitude and delta
		process_one_input(0);

		if(crackle_scan) {
			crackle_len_ct++;
			// has end of anormality been reached
			if(possible_crackle_end(0)) {
				//-- go back crackle_len_ct values in rbuf
				rbufcw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-( crackle_len_ct    +fadesize)))&rbufmask));
				rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-((crackle_len_ct<<1)+fadesize)))&rbufmask));
				//-- prepare cross fade
				fader=0.0;fadew=1.0-fader;fadect=0;
				fadeis=adj/fadesize;
				if(crackle_len_ct>fadesize) { fadeend=fadesize+(crackle_len_ct-fadesize);fadeos=adj/fadesize; }
				else                        { fadeend=fadesize;fadeos=adj/crackle_len_ct; }
				while(rbufcw!=rbufw) {
					ASSERT((ULONG)rbufcr>=(ULONG)rbufbase);ASSERT((ULONG)rbufcr<((ULONG)rbufbase+rbufsize));
					ASSERT((ULONG)rbufcw>=(ULONG)rbufbase);ASSERT((ULONG)rbufcw<((ULONG)rbufbase+rbufsize));
					//-- correct crackle
					*rbufcw=((*rbufcw)*fadew)+((*rbufcr)*fader);
					if(fadect>=fadeend)		  fader-=fadeos;		// fade out
					else if (fadect<fadesize) fader+=fadeis;		// fade in
					fadew=1.0-fader;fadect++;

					rbufcw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufcw+1)&rbufmask));
					rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufcr+1)&rbufmask));
				}
				//-- restore saved state
				//##//recall_state(0);
				//-- count crackles
				//instance->crackle_ct++;	 // only count in process_dry()
				// scan for new crackles
				crackle_scan=FALSE;
			}
			else {
				// it is too long for a crackle
				if(crackle_len_ct==cracklesize) {
					//-- rescan ringbuffer if a later possition might be the start of a crackle
					crackle_len_ct--;
					rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-crackle_len_ct))&rbufmask));
					ASSERT((ULONG)rbufcr>=(ULONG)rbufbase);ASSERT((ULONG)rbufcr<((ULONG)rbufbase+rbufsize));
					//-- restore saved state
					recall_state(0);
					while(rbufcr!=rbufw) {
						//-- get amplitude and delta
						process_one_input(*rbufcr);

						//-- we not inside a possible crackle
						if(possible_crackle_start(0)) {
							//-- we assume this might be the start of a crackle, as the signal is 100%+thresh% over agerage
							//-- save current state
							store_state(0);
							//MSG1("restart with crackle_len=%ld",crackle_len_ct);
							break;
						}
						crackle_len_ct--;
						rbufcr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufcr+1)&rbufmask));
					}
					if(rbufcr==rbufw) crackle_scan=FALSE;
				}
			}
		}
		else {
			//-- we not inside a possible crackle
			if(possible_crackle_start(0)) {
				//-- we assume this might be the start of a crackle, as the signal is 100%+thresh% over agerage
				crackle_scan=TRUE;
				crackle_len_ct=0;
				//-- save current state
				store_state(0);
			}
		}

		//if(curlen>=cracklesize) {
			// write back delayed value from ring-buffer
			*(dbuf++)=*rbufr;
			dpos++;
			rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
		//}
		curlen++;instance->rest--;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->lval=lval;
	instance->amp=amp;
	instance->dif=dif;
	instance->nrj=nrj;
	instance->crackle_scan=crackle_scan;
	instance->crackle_len_ct=crackle_len_ct;
	instance->rbufw=rbufw;
	instance->rbufr=rbufr;
	instance->dpos=dpos;
	instance->dbuf=dbuf;
	instance->low=low;
	instance->mid=mid;
	instance->high=high;
}


void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
	instance->crackle_cts[ch]=instance->crackle_ct;
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->rbuf)	{ DelRingBuf(instance->rbuf);instance->rbuf=NULL; }
	//DEBUG
	fclose(__dbg_fh1);
	fclose(__dbg_fh2);
	//fclose(__dbg_fh3);
	//DEBUG
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}

//-- eof ----------------------------------------------------------------------
