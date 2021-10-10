/****h* Analyse-Stereo/Analyse-Stereo.c [4.2] *
*
*  NAME
*    Analyse-Stereo.c
*  COPYRIGHT
*    $VER: Analyse-Stereo.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Analyse-Stereo core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*  IDEAS
*    parameter
*      gamma: enlarge small diffs
*      drawmode: lines (from center to point), dots, connected-dots
*
*******
*/
/****u* _operators/Analyse-Stereo *
*  Function.eng
*    Produces a graph which shows the spatial distribution of a sample
*  Funktion.ger
*    Erzeugt eine Darstellung der Raumverteilung eines Samples
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    This is know as well as a phase-plot.<br>
*    When calculations are complete a new window is opened on which the graph is drawn.
*    A signal where both channels are exactly the same, will appear as a line from the
*    middle to the top (center). When you listen to it with headphones, you will hear the
*    signal inside your head. The Phase of such a signal is absolut synchonous.
*    A complete anti-phase signal (one channel is the inverted copy of the other),
*    will appear as a line towards the bottom (wide). If listening to this on headphones,
*    the sound appears to come from outside. Such a signal is mono-incompatible,
*    which means, if one listens to this on a mono kitch radio he/she will hear absolutely nothing.
*    When analysing real-stereo files, the graph further shows with peaks towards left or right
*    how much "stereo" the signal is. Ideally the graph is a peaked ball around the center
*    with a needle towards the top.
*    <br>
*    If you want to store the generated graphs as images I recommend using a image grabber like SGrab,
*    which can be found on Aminet.
*  Hinweise.ger
*    Dies ist auch als Phasen-Diagramm bekannt.<br>
*    Wenn das Berechnungen fertig sind, wird ein neues Fenster geöffnet und der
*    Graph gezeichnet.
*    Ein Signal, dessen beide Kanäle exakt gleich sind, wird als Linie von der Mitte
*    nach oben (Center) erscheinen. Wenn Sie es sich mit Kopfhörern anhören, werden sie
*    den Sound in ihrem Kopf warnehmen. Die Phase eines solchen Signales ist absolut synchron.
*    Ein komplett gegenphasiges Signal (ein Kanal ist die invertierte kopie des Anderen),
*    wird als Linie nach unten (Wide) erscheinen. Wenn man sich dies mit Kopfhörern anhört,
*    klingt es als ob der Sound von außen kommt. Ein solches Signal ist nicht mono-kompatibel, d.h.
*    wenn dies jemand auf seinem Mono-Küchenradio anhört, wird er/sie absolut garnichts hören.
*    Wenn man ein echtes Stereosignal analysiert, zeigt der Graph mit Nadeln nach Links und Rechts
*    wie "stereo" das Signal ist. Idealerweise zeigt der Graph eine stachelige Kugel in der
*    Mitte mit einer Spitze nach oben.
*    <br>
*    Wenn sie die erzeugten Graphen als Bilder speichern wollen, dann empfehle ich ihnen dafür einen
*    image-grabber wie SGrab zu verwenden, welchen sie aus dem Aminet beziehen können.
*******
*/

#define ANALYSE_STEREO_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Analyse-Stereo.h"

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

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *sbuf0,*sbuf1;
	double vol,dif;
	LONG voll,volr;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf0=instance->sbuf0;
	sbuf1=instance->sbuf1;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		dif=(double)(((LONG)*sbuf1)-((LONG)*sbuf0));
		voll=abs((LONG)*sbuf0);volr=abs((LONG)*sbuf1);
		vol=(double)max(voll,volr);
		if(vol!=0.0) instance->sum[RES+(WORD)(RES_FC*(dif/vol))]++;		/* sum[r-l]++ */
		else instance->sum[RES]++;
		sbuf0++;sbuf1++;
		
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
