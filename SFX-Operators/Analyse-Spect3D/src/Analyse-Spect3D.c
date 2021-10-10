/****h* Analyse-Spect3D/Analyse-Spect3D.c [4.2] *
*
*  NAME
*    Analyse-Spect3D.c
*  COPYRIGHT
*    $VER: Analyse-Spect3D.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Analyse-Spect3D core file
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
/****u* _operators/Analyse-Spect3D *
*  Function.eng
*    Produces a 3-dimensional frequency-spectrum plot of a sample
*  Funktion.ger
*    Erzeugt eine 3-dimesionale Darstellung des Frequenzspectrums eines Samples
*  Parameter.eng
*    Dir
*        front : put samplestart at the front-side
*        back : put samplestart at the back-side
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
*  Parameter.ger
*    Dir
*        front : legt den Beginn des Samples nach vorn
*        back : legt den Beginn des Samples nach hinten
*    Window (<a href="../node03.02.03.html">W1</a>)
*      welche Fensterfunktion verwendet wird.
*    Lines
*      Wieviele Zeitscheiben SFX berechnen soll.
*    MaxLin.
*      Wieviele Zeitscheiben auf den Bildschirm passen.
*    Bands
*      Wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
*      die Frequenzauflösung ist dann aber auch nicht so fein.
*    Gamma
*      Nichtlineare Verstärkung. Werte von 100 % nach 0 % heben leise Details hervor.
*      Werte größer 100 % verbergen diese. Der Standartwert von 75 % ist eine gute Wahl
*      um leise Signalanteile sichtbar zu machen.
*  Notes.eng
*    When calculations are complete a new window is opened on which the graph is drawn.
*    For the calculations the <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
*    is used.<br>
*    If you want to store the generated graphs as images I recommend using a image grabber like SGrab,
*    which can be found on Aminet.
*  Hinweise.ger
*    Wenn das Spektrum fertig berechnet wurde, wird ein neues Fenster geöffnet und der
*    Graph gezeichnet. 
*    Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
*    verwendet.<br>
*    Wenn sie die erzeugten Graphen als Bilder speichern wollen, dann empfehle ich ihnen dafür einen
*    image-grabber wie SGrab zu verwenden, welchen sie aus dem Aminet beziehen können.
*******
*/

#define ANALYSE_SPECT3D_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Analyse-Spect3D.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	if(!(instance->raster=(float *)AllocVec((instance->m+instance->m+2)*(instance->ml+1)*sizeof(float),MEMF_ANY|MEMF_CLEAR))) return(FALSE);
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
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
