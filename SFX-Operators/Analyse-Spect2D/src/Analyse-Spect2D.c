/****h* Analyse-Spect2D/Analyse-Spect2D.c [4.2] *
*
*  NAME
*    Analyse-Spect2D.c
*  COPYRIGHT
*    $VER: Analyse-Spect2D.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Analyse-Spect2D core file
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
/****u* _operators/Analyse-Spect2D *
*  Function.eng
*    Produces a 2-dimensional frequency-spectrum plot of a sample.
*    This tells you which frequencies are part of the sound over the time.
*    Additionally this helps to spot anomalies and defects, such as klicks and cracks.
*  Funktion.ger
*    Erzeugt eine 2-dimesionale Darstellung des Frequenzspectrums eines Samples.
*    Dies zeigt ihnen, aus welchen Frequenzen ein Klang über die Zeit hinweg aufgebaut ist.
*    Weiterhin können sie damit Anomalien und Störungen, wie Klick und Knackgeräusche aufspüren.
*  Parameter.eng
*    Palette
*        gray : the display uses a grey scale palette
*        color : the display utilises a high contrast colour palette
*    Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*    Lines
*      how many timesslices should SFX render.
*    MaxLin.
*      how many timeslices will fit on this screen.
*    Bands
*      Just how many bands should SFX use. Less Bands means less math, but you lose out on accuracy.
*    Gamma
*      Nonlinear amplification. Values from 100 % towards 0 % means enhancing quiet details.
*      Values above 100 means hiding them. The default value of 75 % is a good choice to
*      make quiet signals visible too.
*    Mode
*        high 2 : four results are merged into one
*        high 1 : two results are merged into one
*        normal : every value in the input will be used to form one result
*        low 1 : every second value in the input will be used, interpolating inbetween data.
*        low 2 : every fourth value in the input will be used, interpolating inbetween data.
*  Parameter.ger
*    Palette
*        gray : zur Darstellung wird eine Graustufenpalette verwendet.
*        color : zur Darstellung wird eine Farbpalette mit hohem Kontrast verwendet.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      welche Fensterfunktion verwendet wird.
*    Lines
*      wieviele Zeitscheiben SFX berechnen soll.
*    MaxLin.
*      wieviele Zeitscheiben auf den Bildschirm passen.
*    Bands
*      wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
*      die Frequenzauflösung ist dann aber auch nicht so fein.
*    Gamma
*      nichtlineare Verstärkung. Werte von 100 % nach 0 % heben leise Details hervor.
*      Werte größer 100 % verbergen diese. Der Standartwert von 75 % ist eine gute Wahl
*      um leise Signalanteile sichtbar zu machen.
*    Mode
*        high 2 : vier Ergebnisse werden zu Einem gemittelt
*        high 1 : zwei Ergebnisse werden zu Einem gemittelt
*        normal : jeder Wert wird genutzt um ein Ergebnis zu erzeugen
*        smooth1 : jeder 2. Wert wird genutzt, die Zwischenwerte werden gemittelt
*        smooth2 : jeder 4. Wert wird genutzt, die Zwischenwerte werden gemittelt
*  Notes.eng
*    When calculations are complete a new window is opened on which the graph is drawn.
*    When the window is active and source-sample is playing, the playposition will be
*    drawn into spectrogram too.<br>
*    Furthermore you can use the key "C" select on of the following modes : no cross hair,
*    single cross hair, harmonic cross hair. The last causes several horizontal lines to move
*    around when moving the mouse. Each doubles the frequency of the one located below.
*    This alows you to find signal harmonics.<br>
*    For the calculations the <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
*    is used.<br>
*    If you want to store the generated graphs as images I recommend using a image grabber like SGrab,
*    which can be found on Aminet.
*  Hinweise.ger
*    Wenn das Spektrum fertig berechnet wurde, wird ein neues Fenster geöffnet und der
*    Graph gezeichnet. Wenn das Fenster aktiv ist und das Sample abgespielt wird, zeichnet
*    __SFX die Abspielposition auch in Spectrogram ein.<br>
*    Weiterhin können sie mit der Taste "C" einen der folgenden Modi auswählen :
*    kein Fadenkreuz, einfaches Fadenkreuz, harmonisches Fadenkreuz. Im letzteren Modus
*    folgen mehrere horizontale Linien dem Mauszeiger. Jede verdoppelt die Frequenz der Tieferliegenden.
*    Damit kann man Signalharmonien finden.<br>
*    Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
*    verwendet.<br>
*    Wenn sie die erzeugten Graphen als Bilder speichern wollen, dann empfehle ich ihnen dafür einen
*    image-grabber wie SGrab zu verwenden, welchen sie aus dem Aminet beziehen können.
*******
*/

#define ANALYSE_SPECT2D_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Analyse-Spect2D.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	if(!(instance->raster=(float *)AllocVec((instance->m+instance->m+2)*(instance->ml+4)*sizeof(float),MEMF_ANY|MEMF_CLEAR))) return(FALSE);
	if(!(instance->rmaxa =(float *)AllocVec((instance->m+2)*sizeof(float),MEMF_ANY|MEMF_CLEAR))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	SAMPLE *sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->raster)	{ FreeVec((ULONG *)instance->raster);instance->raster=NULL; }
	if(instance->rmaxa)		{ FreeVec((ULONG *)instance->rmaxa);instance->rmaxa=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
