/****h* Analyse-Data/Analyse-Data.c [4.2] *
*
*  NAME
*    Analyse-Data.c
*  COPYRIGHT
*    $VER: Analyse-Data.c 4.2 (25.04.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Analyse-Data core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    25.Apr.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*  IDEAS
*
*******
*/
/****u* _operators/Analyse-Data *
*  Function.eng
*    Generates histograms of amplitude and amplitude-deltas, as well as a number of statistics of a sample
*  Funktion.ger
*    Erzeugt Histogramme der Amplituden und der Amplituden-Deltas, sowie verschiedenen Statistiken eines Samples
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    Once the calculations are complete, a new window is opened, containing the graphs and numbers.
*    With the "channel" cycle-button you can choose for which channel you would like to see the graphs.
*    Close the window by clicking in it's close-gadget.<br>
*    The data shown helps you in the mastering process to e.g. align the volume of
*    different tracks.
*    If the operator has been invoked from ARexx or from the batchprocessor, the results will be stored
*    in the file "Analyse-Data.log" located in the current saver-path (or the destination path of the batchprocessor).
*  Hinweise.ger
*    Wenn die Berechnungen fertig sind, wird ein neues Fenster geöffnet, welches die Graphen und die Werte enthält.
*    Mit dem "Channel" Cycle-button kann man auswählen, für welchen Kanal man die Graphen sehen möchte.
*    Mit einem Klick in das Close-Gadget schließen Sie das Fenster.<br>
*    Die gezeigten Daten helfen ihnen beim Mastering z.B. die Lautstärken
*    verschiedener Stücke anzugleichen.
*    Wenn der Operator von ARexx oder vom Batchprozessor aufgerufen wurde, werden die Ergebnisse in
*    die Datei "Analyse-Data.log" im aktuellen Saverpfad (oder Zielpfad des Batchprozessors) geschrieben.
*******
*/

#define ANALYSE_DATA_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Analyse-Data.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	//instance->numch=0;
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	instance->chista=instance->hista[ch];
	instance->chistd=instance->histd[ch];
	instance->cmaxa=instance->cmaxd=0;
	instance->cmins=32767;
	instance->cmaxs=-32768;
	instance->cavg=instance->crms=instance->cavgdif=0.0;
	instance->cpcs=0;instance->cdc=0.0;
	instance->lstv=0;
	instance->div1=instance->div2=1.0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE	*sbuf;
	SAMPLE val;
	double fc,cval;
	LONG absv,difv;

	ULONG *chista,*chistd;
	ULONG cpcs;
	LONG cmaxa,cmaxd;
	LONG cmins,cmaxs;
	double cavg,crms,cdc,cavgdif;
	LONG lstv;
	double div1,div2;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	// restore values
	chista=instance->chista;
	chistd=instance->chistd;
	cpcs=instance->cpcs;
	cmaxa=instance->cmaxa;
	cmaxd=instance->cmaxd;
	cmins=instance->cmins;
	cmaxs=instance->cmaxs;
	cavg=instance->cavg;
	crms=instance->crms;
	cdc=instance->cdc;
	cavgdif=instance->cavgdif;
	lstv=instance->lstv;
	div1=instance->div1;
	div2=instance->div2;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		val=*(sbuf++);

		cval=((double)val)/32768.0;fc=(div1/div2);		/* volume analysis */
		cavg=(cavg+( fabs(cval)/div1))*fc;					/* avg (without overflow) */
		crms=(crms+((cval*cval)/div1))*fc;					/* squared avg (without overflow) */
		cdc =(cdc +(      cval /div1))*fc;					/* offset (without overflow) */
														/* value analysis */
		if(val>cmaxs) cmaxs=val;							/* max sample */
		if(val<cmins) cmins=val;							/* min sample */

														/* probably clipped samples */
		if( (val==SMP_MAXP && lstv==SMP_MAXP) || (val==SMP_MAXN && lstv==SMP_MAXN)) cpcs++;

		if(val==SMP_MAXN) val++;						/* histogramm analysis */
		absv=labs(val);			  chista[absv>>8]++;if(absv>cmaxa) cmaxa=absv;	/* amplitude */
		difv=labs((LONG)val-lstv);chistd[difv>>8]++;if(difv>cmaxd) cmaxd=difv;	/* delta */
		cavgdif=(cavgdif+(difv/div1))*fc;					/* delta avg (without overflow) */

		div1=div2;div2+=1.0;
		lstv=(LONG)val;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	// store values
	instance->chista=chista;
	instance->chistd=chistd;
	instance->cpcs=cpcs;
	instance->cmaxa=cmaxa;
	instance->cmaxd=cmaxd;
	instance->cmins=cmins;
	instance->cmaxs=cmaxs;
	instance->cavg=cavg;
	instance->crms=crms;
	instance->cdc=cdc;
	instance->cavgdif=cavgdif;
	instance->lstv=lstv;
	instance->div1=div1;
	instance->div2=div2;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
	register UWORD j;
	ULONG mv;
	ULONG *ptr;
	double fc;
	
	instance->maxa[ch]=instance->cmaxa;instance->maxd[ch]=instance->cmaxd;
	instance->maxs[ch]=instance->cmaxs;instance->mins[ch]=instance->cmins;
	instance->pcs[ch]=instance->cpcs;instance->dc[ch]=instance->cdc;
	instance->avg[ch]=instance->cavg;
	instance->rms[ch]=sqrt(instance->crms);					// rms volume
	instance->avgdif[ch]=instance->cavgdif;
	mv=0;													// Normalizing data
	ptr=instance->chista;for(j=0;j<128;j++,ptr++) if(*ptr>mv) mv=*ptr;
	fc=64.0/(double)mv;
	ptr=instance->chista;for(j=0;j<128;j++) *(ptr++)*=fc;
	mv=0;
	ptr=instance->chistd;for(j=0;j<256;j++,ptr++) if(*ptr>mv) mv=*ptr;
	fc=64.0/(double)mv;
	ptr=instance->chistd;for(j=0;j<256;j++) *(ptr++)*=fc;

	instance->numch++;		// remmember how many channels we analyse
}

void deactivate(SFXCoreInstance *instance) {
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
