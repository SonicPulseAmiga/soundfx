/****h* PitchShift/PitchShift.c [0.0] *
*
*  NAME
*    PitchShift.c
*  COPYRIGHT
*    $VER: PitchShift.c 0.0 (06.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    PitchShift core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    06.Apr.2004    V 0.0   most recent version
*    05.Apr.2004	V 4.3	most recent version
*    22.May.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/PitchShift *
*  Function.eng
*    Changes the pitch of a sample without making it shorter or longer.
*  Funktion.ger
*    Ändert die Tonhöhe eines Samples, ohne das es kürzer oder länger wird.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    PitchShift Factor (<a href="../node03.02.01.html">P2</a>)
*      factor for change in pitch
*    Window
*      windowrange; use values in the range of 5 to 100 ms for good results
*    Smooth
*      how much percentage of the windowrange should used for crossfade;
*      usually between 25 % and 50 %
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      how to calculate (smooth) inbetween data
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    PitchShift Factor (<a href="../node03.02.01.html">P2</a>)
*      Faktor für die Tonhöhenänderung
*    Window
*      Fensterbereich; gute Ergebnisse erhält man mit Werten von 5 bis 100 ms
*    Smooth
*      Über wieviel Prozent des Fensterbereiches übergeblendet werden soll;
*      üblicherweise zweischen 25 % und 50 %
*    Interpolation (<a href="../node03.02.02.html">I1</a>)
*      wie sollen (sanfte) Zwischenwerte berechnet werden
*  Notes.eng
*    Before I give some more detailed tips, I will generally describe how this all works.
*    If you want to pitch up a sound, you can achive this by playing the sound faster
*    and thus compressing the wave (on the time axis). Unfortunately this makes the
*    sound shorter as well. To compensate this, __SFX will repeat small chunks of
*    sound to stretch the sample. While doing this __SFX has to take care that those
*    chunks fit relative seamingless together to avoid crackles. The winsize parameter
*    determines how far SFX searches maximaly for a good transition. The size depends
*    on the material to pitch-shift. I recomend smaller values (30-50 ms) for percussive
*    samples (this avoids that attacks are repeated audible) and longer values (100-200
*    ms) for synth/pad/string sounds (to avoid loops).<br>
*    If the <a href="../noed06.03.00.Modulation.html">modulator</a> is a sine wave and
*    a small pitchfactor is used (+/- 10 ct), you'll get a vibrato effect.<br>
*    If you'd like to manipulate synthetic waveforms that have a constant period you
*    should enter the period in Winsize. This'll produce clean pitchshifts.<br>
*    Factors shouldn't exceed 4.0 with sampled sounds as such high factors result in
*    bad pitchshifts (this is due to the way the pitchshifter works). Synthetic waveforms
*    can be pitchshifted however the far you like.<br>
*    If the result contains crackles try to slightly change the window-size and/or raise
*    the smooth value.<br>
*  Hinweise.ger
*    Bevor ich etwas speziellere Tips gebe, möchte ich allgemein beschreiben, wie dies
*    alles funktioniert. Wenn sie einen Sound höher stimmen möchten, können Sie dies
*    erreichen indem sie den Klang schneller abspielen und damit die Wellenform stauchen
*    (auf der Zeitachse). Leider wird der Sound dadurch kürzer. Um dies zu kompensieren,
*    wird __SFX kleine Stückchen des Sounds wiederholen um das Sample zu strecken.
*    Dabei muß __SFX aufpassen, daß die Stückchen möglichst nahtlos zusammenpassen.
*    Der WinSize Parameter gibt an wie weit __SFX maximal nach einem guten Übergang
*    sucht. Die Größe hängt jedoch vom zu beabeitenden Material ab. Ich empfehle kleinere
*    Werte (30-50 ms) für perkussive Samples (dies verhindert, daß man die Anschläge
*    mehrfach hört) und längere Werte (100-200 ms) für Synth/Pad/String-Sounds
*    (um ein mögliches Leiern zu vermeiden).<br>
*    Wenn man eine Sinuswelle als <a href="../noed06.03.00.Modulation.html">Modulator</a>
*    nimmt eine geringe Verstimmung (+/- 10 ct) einstellt, erhält man einen Vibratoeffekt.<br>
*    Falls man synthetische Wellenformen mit konstanter Periode bearbeiten möchte, sollte
*    man die Periode bei WinSize eintragen. Dadurch erhält man sehr saubere Pitchshifts.<br>
*    Der Faktor sollte nicht größer als 4.0 genommen werden. Bei solch hohen Faktoren wird
*    das Sample schnell unsauber (liegt in der Funktionsweise des Pitchshifters begründet).
*    Synthetische Wellenformen lassen sich alledings nahezu beliebig "pitchshiften".<br>
*    Wenn das resultierende Sample Knackser aufweist, ändern sie den WinSize-Parameter
*    etwas und/oder erhöhen sie smooth.<br>
*******
*/
#define PITCHSHIFT_C

//{{{ includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "PitchShift.h"
//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>
//}}}

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	instance->winsize2=instance->winsize+instance->winsize;				// make the seek-window twice as big
	instance->winsize4=(instance->winsize>>1);					// we take the best point between 25 % and 75 % of the window to continue
	instance->winsize3=instance->winsize+instance->winsize4;
	//MSG2("winsize  %ld  winsize2 %ld",instance->winsize ,instance->winsize2);
	//MSG2("winsize3 %ld  winsize4 %ld",instance->winsize3,instance->winsize4);

	AssignInterpolFunc(instance->int1.type,&(instance->interpolfunc));
	if(!(instance->rbuf=NewRingBuf(instance->winsize2,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	instance->smct=0;									// for cross-fades
	instance->rptr1=instance->rptr2=0.0;				// read pointer
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	instance->spos=instance->dpos=0;
}

void process_seek(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double effs,effd,eff,dry;
	double pitchs,pitchd,pitch;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	//ULONG __t1=0xFFFFFFFF,__t2=0xFFFFFFFF;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu;
	double rptr1,rptr2;						// read position
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw;
	ULONG rbufmask,rbufsize;
	double rbufr;
	ULONG irange,spos,dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	pitchs=instance->pitchs;
	pitchd=instance->pitchd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	irange=(ULONG)(instance->int1.range);
	spos=instance->spos;
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	rptr2=instance->rptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;

	/** @todo: use real dbuf.len instead of samples */
	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=*sbuf;										// fill the ringbuffer
		if(curlen>=winsize) {
			eff  =effs  +effd  *modfunc1((curlen-winsize),instance->slen,moddata1);dry=1.0-eff;
			pitch=pitchs+pitchd*modfunc2((curlen-winsize),instance->slen,moddata2); // current pitch shift factor

			val1=(LONG)rptr1;
			bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)
			if(val1>bndu) {									// gap is too wide, skip forward
				rptr2=rptr1;									// remember old position
				rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,1);
				smct=smrng;										// activate cross-fade
			}
			else if(val1<bndl) {							// gap is too wide, skip back
				rptr2=rptr1;									// remember old position
				rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,-1);
				smct=smrng;										// activate cross-fade
			}

			if(!smct) {										// do not cross-fade
				rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr1));
				val1=(LONG)(instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange)*eff+((double)*sbuf)*dry);
			}
			else {											// cross-fade
				/** @note calling interpolfunc() directly twice causes faulty code, calling via instance-> */
				smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
				rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr1));
				val2=smfc2*instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange);
				rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr2));
				val2+=smfc1*instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange);
				val1=(LONG)(val2*eff+((double)*sbuf)*dry);
				rptr2+=pitch;smct--;
			}
			CLIPINT(val1);
			*(dbuf++)=(SAMPLE)val1;
			dpos++;
			rptr1+=pitch;
		}
		sbuf++;spos--;curlen++;instance->curlen++;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->spos=spos;
	instance->dpos=dpos;
	instance->rptr1=rptr1;
	instance->rptr2=rptr2;
	instance->smct=smct;
}

void process_seek_rest(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double effs,effd,eff,dry;
	double pitchs,pitchd,pitch;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu;
	double rptr1,rptr2;						// read position
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw;
	ULONG rbufmask,rbufsize;
	double rbufr;
	ULONG irange,dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	pitchs=instance->pitchs;
	pitchd=instance->pitchd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	irange=(ULONG)(instance->int1.range);
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	rptr2=instance->rptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;


	//MSG2("curlen=%ld/slen=%ld",curlen,instance->slen);

	/** @todo: use real dbuf.len instead of samples */
	while((dpos<samples) && (instance->rest>0)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=0;										// fill the ringbuffer
		eff  =effs  +effd  *modfunc1((curlen-winsize),instance->slen,moddata1);dry=1.0-eff;
		pitch=pitchs+pitchd*modfunc2((curlen-winsize),instance->slen,moddata2); // current pitch shift factor

		val1=(LONG)rptr1;
		bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)
		if(val1>bndu) {									// gap is too wide, skip forward
			rptr2=rptr1;									// remember old position
			rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,1);
			smct=smrng;										// activate cross-fade
		}
		else if(val1<bndl) {							// gap is too wide, skip back
			rptr2=rptr1;									// remember old position
			rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,-1);
			smct=smrng;										// activate cross-fade
		}

		if(!smct) {										// do not cross-fade
			rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr1));
			val1=(instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange)*eff+(double)*sbuf*dry);
		}
		else {
			/** @note calling interpolfunc() directly twice causes faulty code, calling via instance-> */
			smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
			rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr1));
			val2=smfc2*instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange);
			rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr2));
			val2+=smfc1*instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange);
			val1=(val2*eff+(double)*sbuf*dry);
			rptr2+=pitch;smct--;
		}
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;
		dpos++;
		rptr1+=pitch;curlen++;instance->rest--;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		
		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->dbuf=dbuf;
	instance->dpos=dpos;
	instance->rptr1=rptr1;
	instance->rptr2=rptr2;
	instance->smct=smct;
}

void process_noseek(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double effs,effd,eff,dry;
	double pitchs,pitchd,pitch;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	//ULONG __t1=0xFFFFFFFF,__t2=0xFFFFFFFF;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu;
	double rptr1,rptr2;						// read position
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw;
	ULONG rbufmask,rbufsize;
	double rbufr;
	ULONG irange,spos,dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	pitchs=instance->pitchs;
	pitchd=instance->pitchd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	irange=(ULONG)(instance->int1.range);
	spos=instance->spos;
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	rptr2=instance->rptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;

	/** @todo: use real dbuf.len instead of samples */
	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=*sbuf;										// fill the ringbuffer
		if(curlen>=winsize) {
			eff  =effs  +effd  *modfunc1((curlen-winsize),instance->slen,moddata1);dry=1.0-eff;
			pitch=pitchs+pitchd*modfunc2((curlen-winsize),instance->slen,moddata2); // current pitch shift factor

			val1=(LONG)rptr1;
			bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)

			//DEBUG
			//if(curlen==winsize) {
			//	MSG3("bndl=%ld < rptr1=%ld < bndu=%ld",bndl,val1,bndu);
			//}
			//DEBUG

			if(val1>bndu) {									// gap is too wide, skip forward
				rptr2=rptr1;									// remember old position
				rptr1=(double)bndl;								// restart 'rptr1' at 25% of winsize2
				smct=smrng;										// activate cross-fade
			}
			else if(val1<bndl) {							// gap is too wide, skip back
				rptr2=rptr1;									// remember old position
				rptr1=(double)bndu;								// start 'rptr1' at 75% of winsize2
				smct=smrng;										// activate cross-fade
			}

			if(!smct) {										// do not cross-fade
				rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr1));
				val1=(LONG)(instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange)*eff+((double)*sbuf)*dry);
			}
			else {											// cross-fade
				/** @note calling interpolfunc() directly twice causes faulty code, calling via instance-> */
				smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
				rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr1));
				val2=smfc2*instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange);
				rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr2));
				val2+=smfc1*instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange);
				val1=(LONG)(val2*eff+((double)*sbuf)*dry);
				rptr2+=pitch;smct--;
			}
			CLIPINT(val1);
			*(dbuf++)=(SAMPLE)val1;
			dpos++;
			rptr1+=pitch;
		}
		sbuf++;spos--;curlen++;instance->curlen++;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->spos=spos;
	instance->dpos=dpos;
	instance->rptr1=rptr1;
	instance->rptr2=rptr2;
	instance->smct=smct;
}

void process_noseek_rest(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double effs,effd,eff,dry;
	double pitchs,pitchd,pitch;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu;
	double rptr1,rptr2;						// read position
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw;
	ULONG rbufmask,rbufsize;
	double rbufr;
	ULONG irange,dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	pitchs=instance->pitchs;
	pitchd=instance->pitchd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	irange=(ULONG)(instance->int1.range);
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	rptr2=instance->rptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;


	//MSG2("curlen=%ld/slen=%ld",curlen,instance->slen);

	/** @todo: use real dbuf.len instead of samples */
	while((dpos<samples) && (instance->rest>0)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=0;										// fill the ringbuffer
		eff  =effs  +effd  *modfunc1((curlen-winsize),instance->slen,moddata1);dry=1.0-eff;
		pitch=pitchs+pitchd*modfunc2((curlen-winsize),instance->slen,moddata2);	// current pitch shift factor

		val1=(LONG)rptr1;
		bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)
		if(val1>bndu) {									// gap is too wide, skip forward
			rptr2=rptr1;									// remember old position
			rptr1=(double)bndl;								// restart 'rptr1' at 25% of winsize2
			smct=smrng;										// activate cross-fade
		}
		else if(val1<bndl) {							// gap is too wide, skip back
			rptr2=rptr1;									// remember old position
			rptr1=(double)bndu;								// start 'rptr1' at 75% of winsize2
			smct=smrng;										// activate cross-fade
		}

		if(!smct) {										// do not cross-fade
			rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr1));
			val1=(instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange)*eff+(double)*sbuf*dry);
		}
		else {											// cross-fade
			/** @note calling interpolfunc() directly twice causes faulty code, calling via instance-> */
			smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
			rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr1));
			val2=smfc2*instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange);
			rbufr=getRPtr(rbufw,rbufmask,rbufsize,((double)curlen-rptr2));
			val2+=smfc1*instance->interpolfunc(rbufbase,&rbufr,rbufmask,irange);
			val1=(val2*eff+(double)*sbuf*dry);
			rptr2+=pitch;smct--;
		}
		CLIPINT(val1);
		*(dbuf++)=(SAMPLE)val1;
		dpos++;
		rptr1+=pitch;curlen++;instance->rest--;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->dbuf=dbuf;
	instance->dpos=dpos;
	instance->rptr1=rptr1;
	instance->rptr2=rptr2;
	instance->smct=smct;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->rbuf)	{ DelRingBuf(instance->rbuf);instance->rbuf=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}

static double INLINE SeekContinue_0(ULONG curlen,double rptr1,SAMPLE *rbufw,SAMPLE *rbufbase,ULONG rbufsize,ULONG rbufmask,ULONG winsize,ULONG winsize3,ULONG winsize4,BYTE dir) {
	LONG val1;
	UBYTE seek;
	LONG delta1,delta2,lv;
	LONG dv,bdv;
	double rptr2,rptr3;
	SAMPLE *rbufr1,*rbufr2,*rbufr3;

	//MSG5("curlen=%6ld bndu=%6ld bndl=%6ld rptr1=%6ld,dir=%+2d",curlen,curlen-winsize4,curlen-winsize3,(ULONG)rptr1,dir);

	//-- get the current delta ----------------------------------------------------
	val1=((LONG)rbufw)-Samples2Bytes(curlen-((LONG)rptr1-1));
	if(val1<0) val1+=rbufsize;
	rbufr3=(SAMPLE *)((ULONG)rbufbase|((ULONG) val1     &rbufmask));	// last value
	rbufr2=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr3+1)&rbufmask));	// current value
	delta2=((LONG)*rbufr2)-((LONG)*rbufr3);								// sign (curent value - last value)
	delta2=SIGNINT(delta2);
	delta2*=dir;
	//MSG2(" cur-last=%6ld-%6ld",*rbufr2,*rbufr3);
	//MSG2(" delta=%+2d  dir=%+2d",delta2,dir);

	//-- initialise search parameters ---------------------------------------------
	if(dir==1) {
		rptr2=(double)(curlen-winsize3);				// start 'rptr2' at 25% of winsize2 and go to 75% (50%)
		val1=(LONG)rbufw-Samples2Bytes(winsize3);
		//MSG2(" seek: %6ld->%6ld",(curlen-winsize3),(curlen-winsize4));
	}
	else {
		rptr2=(double)(curlen-winsize4);				// start 'rptr2' at 75% of winsize2 and go to 25% (50%)
		val1=(LONG)rbufw-Samples2Bytes(winsize4);
		//MSG2(" seek:%6ld->%6ld",(curlen-winsize3),(curlen-winsize4));
	}
	//if(rptr2<0) rptr2+=Bytes2Samples(rbufsize);
	if(val1<0) val1+=rbufsize;
	//-- read first value
	rbufr1=(SAMPLE *)((ULONG)rbufbase|((ULONG) val1     &rbufmask));
	lv=((LONG)*rbufr1);
	bdv=SMP_MAX;  // best difference (smallest)
	seek=TRUE;
	if(dir==1) {
		rptr2+=1.0;
		rbufr1=(SAMPLE *)((ULONG)rbufbase|(((ULONG)(rbufr1+1))&rbufmask));
	}
	else {
		rptr2-=1.0;
		rbufr1=(SAMPLE *)((ULONG)rbufbase|(((ULONG)(rbufr1+(Bytes2Samples(rbufsize)-1)))&rbufmask));
	}
	rptr3=rptr2;
	//-- start search -------------------------------------------------------------
	while(seek) {							// until similiar position has been found
		delta1=((LONG)*rbufr1)-lv;			// current value - last value
		delta1=SIGNINT(delta1);				// sign (direction)
		if(delta1==delta2) {					// is current sign equal to sign at source position ?
			dv=((LONG)*rbufr1)-((LONG)*rbufr2);	// current value - value at source position
			if(dv<0) dv=-dv;
			if(dv<bdv) {						// found better position
				rptr3=rptr2;						// remember
				bdv=dv;
				//MSG2("  continue found, with dv=%5ld and diff=%5ld",bdv,(ULONG)fabs(rptr1-rptr3));
				if(!dv) seek=FALSE;					// there can't be a better one
			}
		}
		lv=*rbufr1;							// remember last value
		if(dir==1) {
			rptr2+=1.0;
			rbufr1=(SAMPLE *)((ULONG)rbufbase|(((ULONG)(rbufr1+1))&rbufmask));
			if(rptr2>=(double)(curlen-winsize4)) seek=FALSE; // stop 'rptr2' at 75% (50%) of winsize2
		}
		else {
			rptr2-=1.0;
			rbufr1=(SAMPLE *)((ULONG)rbufbase|(((ULONG)(rbufr1+(Bytes2Samples(rbufsize)-1)))&rbufmask));
			if(rptr2<=(double)(curlen-winsize3)) seek=FALSE; // stop 'rptr2' at 25% (50%) of winsize2
		}
	}
	if(bdv<SMP_MAX) {
		//MSG2("  continue found, with dv=%5ld and diff=%5ld",bdv,(ULONG)fabs(rptr1-rptr3));
		return(rptr3);						// jump to best position
	}
	else {
		//ERR("  no continue found");
		return((double)(curlen-winsize));	// jump into the middle of the ringbuffer
	}
}

/*
static double INLINE SeekContinue_1(ULONG curlen,double rptr1,SAMPLE *rbufw,SAMPLE *rbufbase,ULONG rbufsize,ULONG rbufmask,ULONG winsize,ULONG winsize3,ULONG winsize4,BYTE dir) {
	LONG val1;
	UBYTE seek,better;
	LONG lv,bdv,dv;
	double rptr2,rptr3;
	SAMPLE *rbufr1,*rbufr2,*rbufr3;
	LONG rng=16,rng2=rng>>1;
	
	//-- initialise search parameters ---------------------------------------------
	rptr1=(double)(curlen-winsize3);			// now rptr1 is at 25 % of winsize2
	if(rptr1<0) rptr1+=Bytes2Samples(rbufsize);
	val1=(LONG)rbufw-Samples2Bytes(winsize3);
	if(val1<0) val1+=rbufsize;
	rbufr1=(SAMPLE *)((ULONG)rbufbase|((ULONG) val1     &rbufmask));

	lv=((LONG)*rbufr1);bdv=65536;seek=TRUE;better=0;
	rbufr1=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr1+1)&rbufmask));
	while(seek)								// solange vorspulen bis 'ähnliche' Stelle gefunden wurde
	{
		rptr1+=1.0;
		rbufr1=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr1+1)&rbufmask));
		if(rptr1>=(double)(curlen-winsize4)) seek=FALSE;
	}
	if(bdv<65536)
	{
		//MSG("  continue found");
		return(rptr3);						// springe zur besten gefundenen Stelle
	}
	else
	{
		//MSG("  no continue found");
		return((double)(curlen-winsize));	// springe in die Mitte des Ringpuffers
	}
}
*/

static double INLINE getRPtr(SAMPLE *rbufw,ULONG rbufmask,ULONG rbufsize,double offs) {
	double rbufr;

	rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-offs;	/* only Offset to baseaddr */
	if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);
	return(rbufr);
}

//-- eof ----------------------------------------------------------------------
