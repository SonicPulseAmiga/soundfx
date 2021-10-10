/****h* Synthesize-FM/Synthesize-FM.c [4.3] *
*
*  NAME
*    Synthesize-FM.c
*  COPYRIGHT
*    $VER: Synthesize-FM.c 4.3 (04.12.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Synthesize-FM core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    04.Dec.2003	V 4.3	most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/Synthesize-FM *
*  Function.eng
*    Waveform generation by fm-synthesis like on a Yamaha CX-7.
*  Funktion.ger
*    Generierung von Wellenformen mittels FM-Synthese nach dem Vorbild eines Yamaha DX-7.
*  Parameter.eng
*    Miscellaneous/SLen
*      Length of the sound
*    Miscellaneous/SRat
*      Playbackrate of the sample. Can be entered as rate, note or choosen from the <a href="../node01.05.04.html">period-window</a>
*    Miscellaneous/Volume (Scale)
*      Volume of the waveform
*    Miscellaneous/Frq (Pitch)
*      Basispitch of the sound to be generated. This can happen directly or through
*      the	<a href="../node01.05.04.html">period-window</a>.
*      It's advisable (read:really good) to choose a "C" as a note so you can use
*      the result in any music program easily.
*    Miscellaneous/Operator
*      Choose for which operator (wave generator) you want to edit wave, ampitude and frequency.
*    Wave (Oszillator)
*      What waveform will be used for the oscillator is determined here :
*        Sin : Sine
*        Tri : Triangle
*        Saw : Sawtooth
*        Sqr : Square
*    Wave/Pha. (Phase)
*      Phaseshift (0-360 Degrees)
*    Frequency
*      This defines the operators frequency relative to the basis pitch.
*    Amplitude
*      This defines the operators amplitude.
*    Modulation-Matrix
*      A checked box means that the amplitude of the src-operator modulates the
*      frequency of the dest operator. As you can easilly see, there are lots
*      of variations possible.
*  Parameter.ger
*    Miscellaneous/SLen
*      Länge des Sounds
*    Miscellaneous/SRat
*      Abspielrate des Samples. Diese kann als Rate direkt oder als Note eingegeben werden bzw. im
*      <a href="../node01.05.04.html">Periodenauswahlfenster</a> ausgewählt werden.
*    Miscellaneous/Volume (Scale)
*      Lautstärke für die Wellenform
*    Miscellaneous/Frq (Pitch)
*      Dient der Einstellung der Basistonhöhe. Diese kann direkt oder im
*      <a href="../node01.05.04.html">Periodenauswahlfenster</a> ausgewählt werden.
*      Es empfiehlt sich die Frequenz eines "C" zu nehmen, um die generierten Klänge
*      in einem Musikprogramm verwenden zu können.
*    Miscellaneous/Operator
*      Wählen sie für welchen Operator (Wellenform-Generator) sie die Wellenform, die Amplitude und die Frequenz einstellen möchten.
*    Wave (Oszillator)
*      Welche Wellenform für den Oszilator verwendet werden soll. Zur Auswahl
*      stehen folgenden Wellenformen :
*        Sin : Sinus
*        Tri : Dreieck
*        Saw : Sägezahn
*        Sqr : Rechteck
*    Wave/Pha. (Phase)
*      Phasenverschiebung (0-360 Grad)
*    Frequency
*      Hier wird die Frequenz des Operators relativ zur Basistonhöhe definiert.
*    Amplitude
*      Hier wird die Amplitude des Operators definiert.
*    Modulation-Matrix
*      Ein Häkchen bedeutet das die Amplitude des Quelloperators die Frequenz
*      des Zieloperators moduliert. Wie sie leicht sehen können, gibt es hier
*      eine Unmenge von Möglichkeiten.
*  Notes.eng
*    As a speciality of this operator you can import presets saved by FMSynth
*    (fileversion 1.3) as well. I would like to say thank you to the author
*    Christian Stiens for the source at this place.<br>
*    FM-synthesis is a complex matter. Just have a look at the included presets
*    and modify them. If you generated some good one, just send them to me, so
*    I can include them in further versions.
*  Hinweise.ger
*    Als Besonderheit können sie mit diesem Operator auch Presets importieren
*    die mit dem Programm FMSynth (Fileversion 1.3) erzeugt worden. An dieser
*    Stelle recht herzlichen Dank an den Autor Christian Stiens für den
*    Sourcecode.<br>
*    FM-Synthese ist eine recht komplexe Angelegenheit. Schauen sie sich die
*    beiliegenden Presets an und basteln sie an diesen rum. Gute Prests können
*    sie mir gerne zusenden, damit ich sie in weiteren Versionen mit beilegen
*    kann.
*******
*/

#define SYNTHESIZE_FM_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Synthesize-FM.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	UBYTE l,o,p;
	UBYTE oszis=0;

	instance->oneperh=PIM2/((double)instance->srat/instance->pitch);
	for(o=0;o<ANZ_OSZIS;o++) {
		for(l=p=0;p<ANZ_OSZIS;p++) {			// which oszis modulate the current one
			if(instance->matrix[p][o]) instance->modptr[o][l++]=&instance->lastval[p];
		}
		instance->modptr[o][l]=NULL;						// terminate
		/*if(l==0) instance->fc[o]=0.0;						// avoid overdrive ? (or not)
		else instance->fc[o]=1.0/(double)l;*/
		//MSG1("  Number of Modsources : %d",l);

		instance->audible[o]=TRUE;
		for(p=0;p<ANZ_OSZIS;p++) {			// is current oszi audible (it does not modulate an other one)
			instance->audible[o]&=(instance->matrix[o][p]==FALSE);
		}
		if(instance->audible[o]) oszis++;
		//MSG1("  Audible ? : %d",instance->audible[o]);
	}
	instance->ampf/=(double)oszis;
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	UBYTE o;

	for(o=0;o<ANZ_OSZIS;o++) {
		AssignBShape(&instance->mod1[o]);
		AssignBShape(&instance->mod2[o]);
		instance->lastval[o]=0.0;
		instance->angle[o]=instance->phase[o];
		//MSG2("angle[%2d]=%10.6lf",o,angle[o]);
	}
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	UBYTE o,p;
	unsigned long i,curlen;
	double *amps ,*ampd ,amp;
	double *frq1s,*frq1d,frq1;
	double /**frq2s,*frq2d,*/frq2;
	BShapeFuncPtr modfunc1[ANZ_OSZIS],modfunc2[ANZ_OSZIS];
	void *moddata1[ANZ_OSZIS],*moddata2[ANZ_OSZIS];
	SAMPLE *dbuf;
	double *lastval;
	double *angle/*,*fc*/;
	double anglediff,oneperh;
	double val1,sum,ampf;
	UBYTE *audible;

	curlen=instance->curlen;

	// Source, Dest.
	dbuf=instance->dbuf;
	for(o=0;o<ANZ_OSZIS;o++) {
		// Modulator 1
		moddata1[o]=(void *)(instance->mod1[o].bshpdata);
		modfunc1[o]=instance->mod1[o].modfunc;
		// Modulator 2
		moddata2[o]=(void *)(instance->mod2[o].bshpdata);
		modfunc2[o]=instance->mod2[o].modfunc;
	}
	frq1s=instance->frq1s;
	frq1d=instance->frq1d;
	amps=instance->amps;
	ampd=instance->ampd;
	//
	oneperh=instance->oneperh;
	angle=instance->angle;
	lastval=instance->lastval;
	audible=instance->audible;
	ampf=instance->ampf;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		val1=0.0;
		for(o=0;o<ANZ_OSZIS;o++) {
			frq1=frq1s[o]+frq1d[o]*modfunc1[o](curlen,instance->slen,moddata1[o]);	/* frq from FrqEnv */
			//MSGC1((instance->curlen<5),"  frq1=%8.6lf",frq1);

			sum=0.0;p=0;
			while(instance->modptr[o][p]) sum+=(*instance->modptr[o][p++]);			/* pointer to lastvals */
			frq2=sum/**fc[o]*/;
			//MSGC1((instance->curlen<5),"  frq2=%8.6lf",frq2);

			amp =amps [o]+ampd [o]*modfunc2[o](curlen,instance->slen,moddata2[o]);	/* amp from AmpEnv */
			//MSGC1((curlen<5),"  amp=%8.6lf",amp);

			anglediff=(oneperh*frq1)+frq2;
			switch(instance->oszi[o]) {			/* current value (-1.0...1.0) */
				case WAVE_SIN: lastval[o]=amp*   sin(angle[o]);break;
				case WAVE_TRI: lastval[o]=amp*sfxtri(angle[o]);break;
				case WAVE_SAW: lastval[o]=amp*sfxsaw(angle[o]);break;
				case WAVE_SQR: lastval[o]=amp*sfxsqr(angle[o]);break;
			}
			//MSGC1((curlen<5),"  lval=%10.6lf",lastval[o]);
			if(audible[o]) val1+=lastval[o];
			angle[o]+=anglediff;
			if(angle[o]>=PIM2) angle[o]-=PIM2;
		}
		val1*=ampf;
		CLIPFLT(val1);
		*(dbuf++)=(SAMPLE)val1;

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

//-- oszillatoren -------------------------------------------------------------

double INLINE sfxtri(double angle) {
	angle=fmod(angle,PIM2);
	if(angle<PID2) return(angle/PID2);
	else if(angle>=PID2 && angle<(PI+PID2)) return(1.0-((angle-PID2)/PID2));
	else return(((angle-(PI+PID2))/PID2)-1.0);
	//if(angle>=PID2 && angle<(PI+PID2)) return(1.0-((angle-PID2)/PID2));
	//if(angle>=(PI+PID2)) return(((angle-(PI+PID2))/PID2)-1.0);
}

double INLINE sfxsaw(double angle) {
	return(1.0-(fmod(angle,PIM2)/PI));
}

double INLINE sfxsqr(double angle) {
	if(fmod(angle,PIM2)<=PI) return(1.0);
	else return(-1.0);
}

//-- eof ----------------------------------------------------------------------
