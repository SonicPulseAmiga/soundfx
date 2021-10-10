/****h* Synthesize-Add/Synthesize-Add.c [4.2] *
*
*  NAME
*    Synthesize-Add.c
*  COPYRIGHT
*    $VER: Synthesize-Add.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Synthesize-Add core file
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
/****u* _operators/Synthesize-Add *
*  Function.eng
*    Waveform generation through additive and sound- synthesis, including
*    frequency-and amplitude modulation.
*  Funktion.ger
*    Generierung von Wellenformen mittels additativer Klangsynthese,
*    inclusive Frequenz- & Amplitudenmodulation.
*  Parameter.eng
*    Wave (Oszillator)
*      What waveform will be used for the oscillator is determined here :
*        Sin : Sine
*        Tri : Triangle
*        Saw : Sawtooth
*        Sqr : Square
*    Wave/Pha. (Phase)
*      Phaseshift (0-360 Degrees)
*    Curve Editing/Range
*      Tool to let you create a smooth linear line between two sliders. Click the
*      first then range and then the second.
*    Curve Editing/Mode
*      Here you can choose how to move or flip the sliders with vertical arrows :
*        Cur : current slider
*        All : all sliders
*        Pos : all positive sliders
*        Neg : all negative sliders
*    Curve Editing/Nr
*      Number of the high tone.
*    Curve Editing/Val
*      Amplitude for the high tone
*    Miscellaneous/SLen
*      Length of the sound
*    Miscellaneous/OnePer
*      Calculates the length of one period using the current rate and puts the result into SLen.
*    Miscellaneous/SRat
*      Playbackrate of the sample. Can be entered as rate, note or choosen from the <a href="../node01.05.04.html">period-window</a>
*    Miscellaneous/Volume (Scale)
*      Volume of the waveform
*    Miscellaneous/MaxVol
*      Calculates the volume for optimum dynamics.
*    Miscellaneous/Frq (Pitch)
*      Basispitch of the sound to be generated. This can happen directly or through
*      the	<a href="../node01.05.04.html">period-window</a>.
*      It's advisable (read:really good) to choose a "C" as a note so you can use
*      the result in any music program easily.
*    Harmonics (SVal)
*      This area has 64 sliders for all the obertone-parts. If the slider's in the
*      the middle (value=0), then this high tone won't be incorporated into the
*      resulting waveform.
*    Harmonics/horiz. arrows
*      Horizontal moving of the lists in steps of one or five.
*    Harmonics/vert. arrows
*      Vertical moving of the list or the current slider in steps on one or five.
*    Harmonics/F-Gadget	Flip.
*      Vertically mirrors the list or the current slider.
*    Frequency (<a href="../node03.02.01.html">P1</a>)
*      Factors for frequency-modulation
*    Amplitude (<a href="../node03.02.01.html">P2</a>)
*      Factors for amplitude-modulation
*  Parameter.ger
*    Wave (Oszillator)
*      Welche Wellenform für den Oszilator verwendet werden soll. Zur Auswahl
*      stehen folgenden Wellenformen :
*        Sin : Sinus
*        Tri : Dreieck
*        Saw : Sägezahn
*        Sqr : Rechteck
*    Wave/Pha. (Phase)
*      Phasenverschiebung (0-360 Grad)
*    Curve Editing/Range
*      Hiermit kann ein linearer Verlauf zwischen 2 Reglern erzeugt werden. Dazu
*      klickt man den 1. Regler an, dann auf Range und jetzt wählt man den 2. Regler aus.
*    Curve Editing/Mode
*      Hier kann man auswählen, wie die Regler mit den vertikalen Pfeilen
*      verschoben oder geflipt werden sollen.
*        Cur : der aktuelle Regler
*        All : alle Regler
*        Pos : alle positiven Regler
*        Neg : alle negativen Regler
*    Curve Editing/Nr
*      Nummer des Obertones
*    Curve Editing/Val
*      Lautstärke für den Oberton
*    Miscellaneous/SLen
*      Länge des Sounds
*    Miscellaneous/OnePer
*      Berechnet die Länge für eine Periode bei aktueller Rate und trägt das Ergebnis bei SLen ein.
*    Miscellaneous/SRat
*      Abspielrate des Samples. Diese kann als Rate direkt oder als Note eingegeben werden bzw. im
*      <a href="../node01.05.04.html">Periodenauswahlfenster</a> ausgewählt werden.
*    Miscellaneous/Volume (Scale)
*      Lautstärke für die Wellenform
*    Miscellaneous/MaxVol
*      Berechnet den Lautstärkewert für eine optimale Dynamikausnutzung.
*    Miscellaneous/Frq (Pitch)
*      Dient der Einstellung der Basistonhöhe. Diese kann direkt oder im
*      <a href="../node01.05.04.html">Periodenauswahlfenster</a> ausgewählt werden.
*      Es empfiehlt sich die Frequenz eines "C" zu nehmen, um die generierten Klänge
*      in einem Musikprogramm verwenden zu können.
*    Harmonics (SVal)
*      In diesem Feld sind 64-Regler für den Lautstärken der Obertöne. Wenn der
*      Regler in der Mitte ist (Wert=0), dann geht der Ton nicht mit in die
*      Berechnung ein.
*    Harmonics/horiz. Pfeile
*      Verschieben die Liste horizontal in Einer- oder Fünferschritten.
*    Harmonics/vert. Pfeile
*      Verschieben die Liste oder den aktuellen Regler vertikal in Einer- oder Fünferschritten.
*    Harmonics/F-Gadget
*      Flip, spiegelt die Liste oder den aktuellen Regler vertikal.
*    Frequency (<a href="../node03.02.01.html">P1</a>)
*      Faktoren zur Frequenzmodulation
*    Amplitude (<a href="../node03.02.01.html">P2</a>)
*      Faktoren zur Amplitudenmodulation
*  Notes.eng
*    Every sound consists of one basic tone and many "high" tones. Frequencies
*    of these "high" or upper tones are a multiples of that of the basic tone.
*    With the help of this operator you can build very complex waveforms by
*    entering the different <a href="../node06.03.00.Harmonics.html">high tones</a>.
*    It might be a good idea to load the example files and take a look or listen
*    in to the resulting waveform. Every high tone has it's own volume setting.
*    The "val" will show it to you in numbers. This value should decline with
*    a rising number of high tones (chance for overdrive:). Positive values are
*    added and negative subtracted.<br>
*    You can produce intersting results for example by taking a basic sample made
*    at "C-2" and another at [C-2] + ( ([C#2]-[C-2]) / 4 ) Some examples :<table>
*    <tr><td>C-0</td><td>65.4063913</td><td>67.35102453</td></tr>
*    <tr><td>C-1</td><td>130.8127827</td><td>132.7574159</td></tr>
*    <tr><td>C-2</td><td>261.6255653</td><td>265.5148317</td></tr>
*    <tr><td>C-3</td><td>523.2511306</td><td>531.0296635</td></tr></table>
*    You now mix these two samples with <a href="Mix.html">Mix</a> with a 50/50 setting.
*    This gives you a sample that sounds a bit etheral, alive and fatter.
*  Hinweise.ger
*    Ein jeder Ton besteht aus einer Grundschwingung und mehreren Obertönen, deren
*    Frequenzen ein Vielfaches der Grundfrequenz betragen. Mit diesem Operator
*    können Sie die komplexesten Wellenformen entwerfen, indem Sie die einzelnen
*    <a href="../node06.03.00.Harmonics.html">Obertöne</a> eingeben. Am Bestem
*    laden Sie eine der abgespeicherten Dateien und schauen bzw. hören sich das
*    Ergebnis an. Ein jeder Oberton wird durch seine Wertigkeit (=Lautstärke)
*    definiert. Diese wird von dem Wert "Val" representiert. Dieser Wert sollte
*    mit zunemenden Obertönen kleiner werden. Positive Werte werden aufaddiert und
*    Negative werden abgezogen.<br>
*    Sehr interessante Ergebnisse erhält man, wenn man ein Grundsample für z.B.
*    C-2 erzeugt und ein weiteres mit folgender Frequenz : [C-2]+(([C#2]-[C-2])/4)
*    Folgend ein paar Beispiele :<table>
*    <tr><td>C-0</td><td>65.4063913</td><td>67.35102453</td></tr>
*    <tr><td>C-1</td><td>130.8127827</td><td>132.7574159</td></tr>
*    <tr><td>C-2</td><td>261.6255653</td><td>265.5148317</td></tr>
*    <tr><td>C-3</td><td>523.2511306</td><td>531.0296635</td></tr></table>
*    Diese zwei Samples mischt man jetzt mit <a href="Mix.html">Mix</a> zu gleichen
*    Teilen zusammen. Dadurch haben wir eine leichte Schwebung in das Sample
*    gebracht ; es klingt nun wesentlich lebendiger und fetter.
*******
*/

#define SYNTHESIZE_ADD_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Synthesize-Add.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	unsigned char k;
	
	instance->oneperh=PIM2/((double)instance->srat/instance->pitch);
	for(k=0;k<64;k++) instance->svals[k]*=instance->amp;
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	instance->angle=instance->phase;
}

void process_sin(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned char k;
	double frqs,frqd,frq;
	double amps,ampd,amp;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf;
	double angle,anglediff,oneperh,dval;
	double *sv;
	double si0,si1,fc;

	curlen=instance->curlen;

	// Source, Dest.
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	amps=instance->amps;
	ampd=instance->ampd;
	//
	oneperh=instance->oneperh;
	angle=instance->angle;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		frq=frqs+frqd*modfunc1(curlen,instance->slen,moddata1);
		amp=amps+ampd*modfunc2(curlen,instance->slen,moddata2);
		anglediff=oneperh*frq;

		dval=0.0;sv=instance->svals;
		si0=sin(-angle);
		si1=0.0;
		fc=cos(angle);fc+=fc;
		for(k=0;k<64;) {
			si0=fc*si1-si0;
			if(*sv!=0.0) dval+=(si0*(*sv));
			k++;sv++;
			si1=fc*si0-si1;
			if(*sv!=0.0) dval+=(si1*(*sv));
			k++;sv++;
		}
		dval*=amp;
		CLIPFLT(dval);
		*(dbuf++)=(SAMPLE)dval;
		angle+=anglediff;
		if(angle>=PIM2) angle-=PIM2;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->angle=angle;
}

void process_tri(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned char k;
	double frqs,frqd,frq;
	double amps,ampd,amp;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf;
	double angle,anglediff,oneperh,an,dval;
	double *sv;

	curlen=instance->curlen;

	// Source, Dest.
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	amps=instance->amps;
	ampd=instance->ampd;
	//
	oneperh=instance->oneperh;
	angle=instance->angle;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		frq=frqs+frqd*modfunc1(curlen,instance->slen,moddata1);
		amp=amps+ampd*modfunc2(curlen,instance->slen,moddata2);
		anglediff=oneperh*frq;

		dval=0.0;an=angle;sv=instance->svals;
		for(k=0;k<64;k++,sv++) {
			if(*sv!=0.0) dval+=(sfxtri(an)*(*sv));
			an+=angle;
		}
		dval*=amp;
		CLIPFLT(dval);
		*(dbuf++)=(SAMPLE)dval;
		angle+=anglediff;
		if(angle>=PIM2) angle-=PIM2;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->angle=angle;
}

void process_saw(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned char k;
	double frqs,frqd,frq;
	double amps,ampd,amp;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf;
	double angle,anglediff,oneperh,an,dval;
	double *sv;

	curlen=instance->curlen;

	// Source, Dest.
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	amps=instance->amps;
	ampd=instance->ampd;
	//
	oneperh=instance->oneperh;
	angle=instance->angle;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		frq=frqs+frqd*modfunc1(curlen,instance->slen,moddata1);
		amp=amps+ampd*modfunc2(curlen,instance->slen,moddata2);
		anglediff=oneperh*frq;

		dval=0.0;an=angle;sv=instance->svals;
		for(k=0;k<64;k++,sv++) {
			if(*sv!=0.0) dval+=(sfxsaw(an)*(*sv));
			an+=angle;
		}
		dval*=amp;
		CLIPFLT(dval);
		*(dbuf++)=(SAMPLE)dval;
		angle+=anglediff;
		if(angle>=PIM2) angle-=PIM2;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->angle=angle;
}

void process_sqr(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned char k;
	double frqs,frqd,frq;
	double amps,ampd,amp;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf;
	double angle,anglediff,oneperh,an,dval;
	double *sv;

	curlen=instance->curlen;

	// Source, Dest.
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	frqs=instance->frqs;
	frqd=instance->frqd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	amps=instance->amps;
	ampd=instance->ampd;
	//
	oneperh=instance->oneperh;
	angle=instance->angle;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		frq=frqs+frqd*modfunc1(curlen,instance->slen,moddata1);
		amp=amps+ampd*modfunc2(curlen,instance->slen,moddata2);
		anglediff=oneperh*frq;

		dval=0.0;an=angle;sv=instance->svals;
		for(k=0;k<64;k++,sv++) {
			if(*sv!=0.0) dval+=(sfxsqr(an)*(*sv));
			an+=angle;
		}
		dval*=amp;
		CLIPFLT(dval);
		*(dbuf++)=(SAMPLE)dval;
		angle+=anglediff;
		if(angle>=PIM2) angle-=PIM2;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->angle=angle;
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
