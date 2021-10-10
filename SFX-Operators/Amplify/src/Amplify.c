/****h* Amplify/Amplify.c [4.2] *
*
*  NAME
*    Amplify.c
*  COPYRIGHT
*    $VER: Amplify.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Amplify core file
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
/****u* _operators/Amplify *
*  Function.eng
*    Changes the volume of a sample
*  Funktion.ger
*    Ändert die Lautstärke eines Samples
*  Parameter.eng
*    Amplification (<a href="../node03.02.01.html">P1</a>)
*      This value controls the amount of amplification. The volume can be raised and/or lowered.
*    MaxVol
*      After a clicking this button, the current source will be scanned and the maximum
*      amplification without clipping will be calculated.
*      the result will be entered into Par0 and modulation will be set to "None".
*    Wrap
*      Choose how to handle clipping. The modi below are available :
*        NoClip : don't test for overdriven values; will produce distorted sounds when raising the volume beyond the maximum
*        Clip : overdriven values are clipped
*        Wrap1 : overdriven values are pushed into the opposite side until they don't clip anymore.
*        Wrap2 : overdriven values are overturned (folded) until they don't clip anymore.
*      Just give it a try. Take a long sinewave  and slowly overdrive it.
*  Parameter.ger
*    Amplification (<a href="../node03.02.01.html">P1</a>)
*      Dieser Wert gibt die Lautstärkeänderung an. Die Lautstärke kann angehoben und/oder abgesenkt werden.
*    MaxVol
*      Durch einen Klick auf diesen Knopf, wird das Sample gescannt und die maximale
*      Verstärkung errechnet die möglich ist, ohne das Signal zu übersteuern.
*      Das Ergebnis wird in Par0 eingetragen und die Modulation wird auf "None" gesetzt.
*    Wrap
*      Gibt an, wie eine mögliche Übersteuerung des Signals behandelt werden soll.
*      Hierbei gibt es 4 Modi :
*        NoClip : es wird nicht auf Übersteuerte Werte gestestet; erzeugt verzerrte Klänge wenn die Lautstärke über das Maximum hinaus angehoben wird
*        Clip : die übersteuerten Werte werden auf Maximum bzw. Minimum gesetzt
*        Wrap1 : der übersteuerte Anteil wird an der anderen Seite wieder hereingeschoben, und zwar solange, bis er komplett im Normalbereich ist.
*        Wrap2 : der übersteuerte Anteil wird solange an der Ober- und Unterkante umgeklappt, bis er komplett im Normalbereich ist.
*      Diese Modi sollte man ruhig mal ausprobieren. Dazu nimmt man einen lange Sinus
*      und übersteuert diesen langsam.
*  Notes.eng
*    Percussion sounds (bassdrums,snare,drums,...) can be lifted a bit
*    (ca. 120 %). This'll produce the typical <a href="../node06.03.00.Overdrive.html">overdrive</a> effect,
*    by clipping sample data (vertically).<br>
*    The amount of amplification without hitting the ceiling hard, can be estimated
*    taking a look at the min-and maxlines in the <a href="../node01.05.01.html">samplewindow</a>.<br>
*    This operator can also be used for amplitude and ring-modulation, creating
*    further possiblities for sound synthesis.
*    For example, make one sine wave with normal period and another
*    with double. Let the one sine be the source and
*    the other sine be the modulation waveform with User/
*    Normal set. Set Par0 to 0.0 and Par1 to 1.0. 
*    Generate the new sample and take a close look (with zoom maybe?:) at the result.
*    What you have done is called ring-modulation. When choosing the modulation
*    range to be from -1.0 to 1.0 then you will get amplitude modulation.
*  Hinweise.ger
*    Schlagzeuginstrumente ( besonders Basedrums ) können ruhig mal
*    etwas übersteuert werden (ca. 120 %). Eine solche leichte
*    <a href="../node06.03.00.Overdrive.html">Übersteuerung</a> ergibt den typischen Overdriveeffekt, durch
*    die gekappten Samplewerte.<br>
*    Die Verstärkung die ohne Übersteuerung möglich ist, kann an den
*    Min- und Maxlinien im <a href="../node01.05.01.html">Samplefenster</a> abgeschätzt werden.<br>
*    Dieser Operator läßt sich außerdem noch zur Amplituden- und Ringmodulation
*    verwenden, wodurch sich weitere Synthesemöglichkeiten ergeben.
*    Erzeugen Sie dazu z.B. einen Sinus mit normaler Periode
*    und einen weiteren mit doppelter. Jetzt wählen Sie ein Sample
*    als Source und stellen als Modulation User/Normal ein. Par0
*    setzen Sie auf 0.0 und Par1 auf 1.0. Als Modulator nehmen Sie den
*    anderen Sinus. Lassen Sie das neue Sample erzeugen und schauen
*    Sie es sich an (eventuell vergrößern).
*    Was sie getan haben nennt man Ringmodulation. Wenn sie den Modulationsbereich
*    auf -1.0 bis 1.0 ausdehnen erhalten sie eine Amplitudenmodulation.
*******
*/

#define AMPLIFY_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Amplify.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	instance->maxvalp=0;
	instance->maxvaln=0;
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod);
}

void process_pre(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i;
	SAMPLE *sbuf;
	LONG maxvalp,maxvaln;

	// Source, Dest.
	sbuf=instance->sbuf;
	//
	maxvalp=instance->maxvalp;
	maxvaln=instance->maxvaln;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,instance->curlen++) {
		if(*sbuf>maxvalp) maxvalp=*sbuf;
		else if(*sbuf<maxvaln) maxvaln=*sbuf;
		sbuf++;
	}
	instance->maxvalp=maxvalp;
	instance->maxvaln=maxvaln;
}

void process_pre_adjust(SFXCoreInstance *instance) {
	if(instance->maxvalp || instance->maxvaln) {
		instance->maxvaln=-instance->maxvaln;

		// initialize instance->amps witrh 32766 for maxvol fx
		instance->amps/=(double)(max(instance->maxvalp,instance->maxvaln));
		instance->ampe=instance->amps;
		instance->ampd=0.0;
		instance->mod.bshape=BSHP_NONE;
		AssignBShape(&instance->mod);
	}
}

void process_noclip(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double amps,ampd,amp;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	amps=instance->amps;
	ampd=instance->ampd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		amp=amps+ampd*modfunc(curlen,instance->slen,moddata);

		*(dbuf++)=(SAMPLE)(((double)(*(sbuf++)))*amp);

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_clip(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double amps,ampd,amp;
	LONG res;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	amps=instance->amps;
	ampd=instance->ampd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		amp=amps+ampd*modfunc(curlen,instance->slen,moddata);

		res=(LONG)(((double)(*(sbuf++)))*amp);
		if      (res>SMP_MAXP) res=SMP_MAXP;
		else if (res<SMP_MAXN) res=SMP_MAXN;
		*(dbuf++)=(SAMPLE)res;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_wrap1(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double amps,ampd,amp;
	LONG res;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	amps=instance->amps;
	ampd=instance->ampd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		amp=amps+ampd*modfunc(curlen,instance->slen,moddata);

		res=(LONG)(((double)(*(sbuf++)))*amp);
		if      (res>SMP_MAXP) res=((res-SMP_MAXN)&SMP_MAXMASK)+SMP_MAXN;
		else if (res<SMP_MAXN) res=((res-SMP_MAXP)&SMP_MAXMASK)+SMP_MAXP;
		*(dbuf++)=(SAMPLE)res;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_wrap2(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double amps,ampd,amp;
	LONG res;
	BShapeFuncPtr modfunc;
	void *moddata;
	SAMPLE *dbuf,*sbuf;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator
	moddata=(void *)(instance->mod.bshpdata);
	modfunc=instance->mod.modfunc;
	amps=instance->amps;
	ampd=instance->ampd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		amp=amps+ampd*modfunc(curlen,instance->slen,moddata);

		res=(LONG)(((double)(*(sbuf++)))*amp);
		while(res>SMP_MAXP || res<SMP_MAXN) {
			if      (res>SMP_MAXP) res=SMP_MAXP-(res-SMP_MAXP);
			else if (res<SMP_MAXN) res=SMP_MAXN-(res-SMP_MAXN);
		}
		*(dbuf++)=(SAMPLE)res;

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
