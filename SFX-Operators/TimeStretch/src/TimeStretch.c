/****h* TimeStretch/TimeStretch.c [4.3] *
*
*  NAME
*    TimeStretch.c
*  COPYRIGHT
*    $VER: TimeStretch.c 4.3 (17.02.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    TimeStretch core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    17.Feb.2004	V 4.3	most recent version
*    22.May.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/TimeStretch *
*  Function.eng
*    Changes the length of a sample without making its pitch higher or lower.
*  Funktion.ger
*    Ändert die Länge eines Samples, ohne das seine Tonhöhe höher oder tiefer wird.
*  Parameter.eng
*    TimeStretch Factor (<a href="../node03.02.01.html">P1</a>)
*      factor for change of the length
*    Window
*      windowrange; use values in the range of 5 to 100 ms for good results
*    Smooth
*      how much percentage of the windowrange should used for crossfade;
*      usually between 25 % and 50 %
*  Parameter.ger
*    TimeStretch Factor (<a href="../node03.02.01.html">P1</a>)
*      Faktor für die Längenänderung
*    Window
*      Fensterbereich; gute Ergebnisse erhält man mit Werten von 5 bis 100 ms
*    Smooth
*      Über wieviel Prozent des Fensterbereiches übergeblendet werden soll;
*      üblicherweise zweischen 25 % und 50 %
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
*    should enter the period in Winsize. This will produce clean pitchshifts.<br>
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

#define TIMESTRETCH_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "TimeStretch.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- globals ------------------------------------------------------------------

// DEBUG
//BPTR __dbg_fh1,__dbg_fh2,__dbg_fh3;
//DEBUG

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	instance->winsize2=instance->winsize+instance->winsize;		// make the seek-window twice as big
	instance->winsize4=(instance->winsize>>1);					// we take the best point between 25 % and 75 % of the window to continue
	instance->winsize3=instance->winsize+instance->winsize4;
	//MSG2("winsize  %ld  winsize2 %ld",instance->winsize ,instance->winsize2);
	//MSG2("winsize3 %ld  winsize4 %ld",instance->winsize3,instance->winsize4);

	if(!(instance->rbuf=NewRingBuf(instance->winsize2,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	if(!(instance->renv=NewRingBuf(instance->winsize2,&instance->renvbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	// get the REAL result lenght
	if(instance->mod1.bshape==BSHP_NONE) {
		instance->rlen=instance->slen*instance->timefs;
	}
	else {
		unsigned long i;
		double rptr=0.0;
		double timefs,timefd;
		BShapeFuncPtr modfunc1;
		void *moddata1;
		ULONG tlen;

		// Modulator
		moddata1=(void *)(instance->mod1.bshpdata);
		modfunc1=instance->mod1.modfunc;
		timefs=instance->timefs;
		timefd=instance->timefd;

		AssignBShape(&instance->mod1);
		tlen=instance->slen*min(timefs,instance->timefe);	  // longest possible sample
		for(i=0;i<instance->slen;i++) {
			rptr+=(timefs+timefd*modfunc1(i,instance->slen,moddata1));
		}
		instance->rlen=(ULONG)rptr;
	}
	return(TRUE);	
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	instance->smct=0;												// for cross-fades
	instance->rptr1=instance->rptr2=0.0;							// read pointer
	instance->wptr1=instance->wptr2=0.0;							// write pointer
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	memset(instance->renv,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	instance->renvw=instance->renvbase;
	instance->spos=instance->dpos=0;
	instance->ampenv=0.0;

	// DEBUG
	//__dbg_fh1=Open("t:sfx_dbg1.sraw16",MODE_NEWFILE);
	//__dbg_fh2=Open("t:sfx_dbg2.sraw16",MODE_NEWFILE);
	//__dbg_fh3=Open("t:sfx_dbg3.sraw16",MODE_NEWFILE);
	// DEBUG
}

void process_seek(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double timefs,timefd,timef;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu;
	double rptr1,rptr2;						// read pointer
	ULONG wptr1,wptr2;						// write pointer
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw,*rbufr;
	ULONG rbufmask,rbufsize;
	ULONG spos,dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	timefs=instance->timefs;
	timefd=instance->timefd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	spos=instance->spos;
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	wptr1=instance->wptr1;
	wptr2=instance->wptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;

	/** @todo: use real dbuf.len instead of samples */
	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=*sbuf;										// fill the src ringbuffer
		if(curlen>=winsize) {
			timef=timefs+timefd*modfunc1((curlen-winsize),instance->slen,moddata1);	// current time stretch factor

			val1=(LONG)rptr1;
			bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)
			if(val1>bndu) {									// gap is too wide, skip forward
				//MSG5("seek > : curlen=%6ld/%6ld  rptr1=%9.3lf > bndu=%6ld  wptr1=%6ld",curlen-winsize,curlen,rptr1,bndu,wptr1);
				rptr2=rptr1;							    	// remember old position
				wptr2=(ULONG)rptr2;
				rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,1);
				wptr1-=(ULONG)(rptr2-rptr1);
				//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
				smct=smrng;										// activate cross-fade
			}
			else if(val1<bndl) {							// gap is too wide, skip back
				//MSG5("seek < : curlen=%6ld/%6ld  rptr1=%9.3lf < bndl=%6ld wptr1=%6ld",curlen-winsize,curlen,rptr1,bndl,wptr1);
				rptr2=rptr1;							    	// remember old position
				wptr2=(ULONG)rptr2;
				rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,-1);
				wptr1+=(ULONG)(rptr1-rptr2);
				//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
				smct=smrng;										// activate cross-fade
			}

			rptr1+=timef;
			if(!smct) {										// do not cross-fade
				while((wptr1<(ULONG)rptr1) && (dpos<samples)) {
					ERRC((wptr1>curlen),"wptr1>curlen");
					rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr1++)))&rbufmask));
					val1=*rbufr;
					CLIPINT(val1);
					*(dbuf++)=(SAMPLE)val1;
					dpos++;
				}
			}
			else {											// cross-fade
				while((wptr1<(ULONG)rptr1) && (dpos<samples) && smct) {
					ERRC((wptr1>curlen),"wptr1>curlen");
					ERRC2((wptr2>curlen),"wptr2>curlen, smct=%ld/%ld",smct,smrng);
					smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
					rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr1++)))&rbufmask));
					val2=smfc2*(*rbufr);
					rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr2++)))&rbufmask));
					val2+=smfc1*(*rbufr);
					CLIPFLT(val2);
					*(dbuf++)=(SAMPLE)val2;
					dpos++;
					smct--;
				}
			}
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
	instance->wptr1=wptr1;
	instance->wptr2=wptr2;
	instance->smct=smct;
}

void process_seek_dyncor(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double timefs,timefd,timef;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu,rwdif;
	double rptr1,rptr2;						// read pointer
	ULONG wptr1,wptr2;						// write pointer
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw,*rbufr;
	SAMPLE *renvbase,*renvw,*renvr1,*renvr2;
	ULONG rbufmask,rbufsize;
	ULONG spos,dpos;
	double amp,ampenv,efc;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	timefs=instance->timefs;
	timefd=instance->timefd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	renvbase=instance->renvbase;
	renvw=instance->renvw;
	//
	spos=instance->spos;
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	wptr1=instance->wptr1;
	wptr2=instance->wptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;
	ampenv=instance->ampenv;

	/** @todo: use real dbuf.len instead of samples */
	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=*sbuf;										// fill the src ringbuffer

		amp=fabs((double)*sbuf);
		if(amp>ampenv) ampenv=amp;							// envelope tracker
		else ampenv=ENVFC1*ampenv+ENVFC2*amp;
		//ERRC2(((SAMPLE)ampenv==0),"ampenv dropped to 0 at %8ld/%8ld",curlen,instance->slen);
		if(ampenv<1.0) ampenv=1.0;
		*renvw=(SAMPLE)ampenv;								// fill the env ringbuffer

		// DEBUG
		//Write(__dbg_fh1,renvw,2);
		// DEBUG

		if(curlen>=winsize) {
			timef=timefs+timefd*modfunc1((curlen-winsize),instance->slen,moddata1);	// current time stretch factor

			val1=(LONG)rptr1;
			bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)
			if(val1>bndu) {									// gap is too wide, skip forward
				//MSG5("seek > : curlen=%6ld/%6ld  rptr1=%9.3lf > bndu=%6ld  wptr1=%6ld",curlen-winsize,curlen,rptr1,bndu,wptr1);
				rptr2=rptr1;							    	// remember old position
				wptr2=(ULONG)rptr2;
				rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,1);
				wptr1-=(ULONG)(rptr2-rptr1);
				//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
				smct=smrng;										// activate cross-fade
			}
			else if(val1<bndl) {							// gap is too wide, skip back
				//MSG5("seek < : curlen=%6ld/%6ld  rptr1=%9.3lf < bndl=%6ld wptr1=%6ld",curlen-winsize,curlen,rptr1,bndl,wptr1);
				rptr2=rptr1;							    	// remember old position
				wptr2=(ULONG)rptr2;
				rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,-1);
				wptr1+=(ULONG)(rptr1-rptr2);
				//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
				smct=smrng;										// activate cross-fade
			}

			rptr1+=timef;
			// this needs to be the position, we compare the volume with and adjust accordingly !!
			renvr1=(SAMPLE *)((ULONG)renvbase|((ULONG)((renvw+rbufsize)-winsize)&rbufmask));

			//if((curlen-winsize)<40) {
			//	  MSG2("%3ld : %5ld ",(curlen-winsize),*renvr1);
			//}
			/* diff=(curpos-dst.writepos);  // always positive !
			   read from src.writepos-diff;
			 */
			if(!smct) {										// do not cross-fade
				while((wptr1<(ULONG)rptr1) && (dpos<samples)) {
					rwdif=curlen-(wptr1++);
					//ERRC((rwdif<0),"wptr1>curlen");
					renvr2=(SAMPLE *)((ULONG)renvbase|((ULONG)((renvw+rbufsize)-rwdif)&rbufmask));
					rbufr =(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-rwdif)&rbufmask));

					efc=sqrt(sqrt((double)*renvr2/(double)*renvr1));
					ERRC4((efc>50.0),"0  : %8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,efc,*renvr2,*renvr1);
					//ERRC5((*renvr1==0),"0  : %8ld/%8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,curlen,efc,*renvr2,*renvr1);
					// DEBUG
					//Write(__dbg_fh2,renvr1,2);
					//Write(__dbg_fh3,renvr2,2);
					// DEBUG

					val1=(LONG)((double)*rbufr*((double)*renvr2/(double)*renvr1));
					CLIPINT(val1);
					*(dbuf++)=(SAMPLE)val1;
					dpos++;
					//if((curlen-winsize)<40) {
					//	  MSG4("    : %5ld %5ld/%5ld=%6.4lf",*rbufr,*renvr2,*renvr1,((double)*renvr2/(double)*renvr1));
					//}
				}
			}
			else {											// cross-fade
				while((wptr1<(ULONG)rptr1) && (dpos<samples) && smct) {
					rwdif=curlen-(wptr1++);
					//ERRC((rwdif<0),"wptr1>curlen");
					smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
					renvr2=(SAMPLE *)((ULONG)renvbase|((ULONG)((renvw+rbufsize)-rwdif)&rbufmask));
					rbufr= (SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-rwdif)&rbufmask));

					efc=sqrt(sqrt((double)*renvr2/(double)*renvr1));
					ERRC4((efc>50.0),"1a : %8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,efc,*renvr2,*renvr1);
					//ERRC5((*renvr1==0),"1a : %8ld/%8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,curlen,efc,*renvr2,*renvr1);
					// DEBUG
					//Write(__dbg_fh2,renvr1,2);
					//Write(__dbg_fh3,renvr2,2);
					// DEBUG

					val2=smfc2*(double)*rbufr*((double)*renvr2/(double)*renvr1);

					rwdif=curlen-(wptr2++);
					//ERRC((wptr2>curlen),"wptr2>curlen");
					renvr2=(SAMPLE *)((ULONG)renvbase|((ULONG)((renvw+rbufsize)-rwdif)&rbufmask));
					rbufr= (SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-rwdif)&rbufmask));

					efc=sqrt(sqrt((double)*renvr2/(double)*renvr1));
					ERRC4((efc>50.0),"1b : %8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,efc,*renvr2,*renvr1);
					//ERRC5((*renvr1==0),"1b : %8ld/8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,curlen,efc,*renvr2,*renvr1);

					val2+=smfc1*(double)*rbufr*((double)*renvr2/(double)*renvr1);
					CLIPFLT(val2);
					*(dbuf++)=(SAMPLE)val2;
					dpos++;
					smct--;
				}
			}
		}
		sbuf++;spos--;curlen++;instance->curlen++;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		renvw=(SAMPLE *)((ULONG)renvbase|((ULONG)(renvw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->renvw=renvw;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->spos=spos;
	instance->dpos=dpos;
	instance->rptr1=rptr1;
	instance->wptr1=wptr1;
	instance->wptr2=wptr2;
	instance->smct=smct;
	instance->ampenv=ampenv;
}

void process_seek_rest(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double timefs,timefd,timef;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu;
	double rptr1,rptr2;						// read pointer
	ULONG wptr1,wptr2;						// write pointer
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw,*rbufr;
	ULONG rbufmask,rbufsize;
	ULONG dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	timefs=instance->timefs;
	timefd=instance->timefd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	wptr1=instance->wptr1;
	wptr2=instance->wptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;

	/** @todo: use real dbuf.len instead of samples */
	while((dpos<samples) && (instance->rest>0)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=0;										// fill the ringbuffer
		timef=timefs+timefd*modfunc1((curlen-winsize),instance->slen,moddata1);	// current time stretch factor

		val1=(LONG)rptr1;
		bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)
		if(val1>bndu) {									// gap is too wide, skip forward
			//MSG5("seek > : curlen=%6ld/%6ld  rptr1=%9.3lf > bndu=%6ld  wptr1=%6ld",curlen-winsize,curlen,rptr1,bndu,wptr1);
			rptr2=rptr1;							    	// remember old position
			wptr2=(ULONG)rptr2;
			rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,1);
			wptr1-=(ULONG)(rptr2-rptr1);
			//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
			smct=smrng;										// activate cross-fade
		}
		else if(val1<bndl) {							// gap is too wide, skip back
			//MSG5("seek < : curlen=%6ld/%6ld  rptr1=%9.3lf < bndl=%6ld wptr1=%6ld",curlen-winsize,curlen,rptr1,bndl,wptr1);
			rptr2=rptr1;									// remember old position
			wptr2=(ULONG)rptr2;
			rptr1=SeekContinue_0(curlen,rptr1,rbufw,rbufbase,rbufsize,rbufmask,winsize,winsize3,winsize4,-1);
			wptr1+=(ULONG)(rptr1-rptr2);
			//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
			smct=smrng;										// activate cross-fade
		}

		rptr1+=timef;
		if(!smct) {										// do not cross-fade
			while((wptr1<(ULONG)rptr1) && (dpos<samples)) {
				rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr1++)))&rbufmask));
				val1=*rbufr;
				CLIPINT(val1);
				*(dbuf++)=(SAMPLE)val1;
				dpos++;
			}
		}
		else {											// cross-fade
			while((wptr1<(ULONG)rptr1) && (dpos<samples) && smct) {
				smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
				rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr1++)))&rbufmask));
				val2=smfc2*(*rbufr);
				rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr2++)))&rbufmask));
				val2+=smfc1*(*rbufr);
				CLIPFLT(val2);
				*(dbuf++)=(SAMPLE)val2;
				dpos++;
				smct--;
			}
		}
		curlen++;instance->rest--;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->dbuf=dbuf;
	instance->dpos=dpos;
	instance->rptr1=rptr1;
	instance->wptr1=wptr1;
	instance->wptr2=wptr2;
	instance->smct=smct;
}

void process_noseek(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double timefs,timefd,timef;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu;
	double rptr1,rptr2;						// read pointer
	ULONG wptr1,wptr2;						// write pointer
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw,*rbufr;
	ULONG rbufmask,rbufsize;
	ULONG spos,dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	timefs=instance->timefs;
	timefd=instance->timefd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	spos=instance->spos;
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	wptr1=instance->wptr1;
	wptr2=instance->wptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;

	/** @todo: use real dbuf.len instead of samples */
	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=*sbuf;										// fill the src ringbuffer
		if(curlen>=winsize) {
			timef=timefs+timefd*modfunc1((curlen-winsize),instance->slen,moddata1);	// current time stretch factor

			val1=(LONG)rptr1;
			bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)
			if(val1>bndu) {									// gap is too wide, skip forward
				//MSG5("seek > : curlen=%6ld/%6ld  rptr1=%9.3lf > bndu=%6ld  wptr1=%6ld",curlen-winsize,curlen,rptr1,bndu,wptr1);
				rptr2=rptr1;							    	// remember old position
				wptr2=(ULONG)rptr2;
				rptr1=(double)(curlen-winsize3);				// restart 'rptr1' at 25% of winsize2
				wptr1-=(ULONG)(rptr2-rptr1);
				//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
				smct=smrng;										// activate cross-fade
			}
			else if(val1<bndl) {							// gap is too wide, skip back
				//MSG5("seek < : curlen=%6ld/%6ld  rptr1=%9.3lf < bndl=%6ld wptr1=%6ld",curlen-winsize,curlen,rptr1,bndl,wptr1);
				rptr2=rptr1;							    	// remember old position
				wptr2=(ULONG)rptr2;
				rptr1=(double)(curlen-winsize4);				// start 'rptr1' at 75% of winsize2
				wptr1+=(ULONG)(rptr1-rptr2);
				//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
				smct=smrng;										// activate cross-fade
			}

			rptr1+=timef;
			if(!smct) {										// do not cross-fade
				while((wptr1<(ULONG)rptr1) && (dpos<samples)) {
					ERRC((wptr1>curlen),"wptr1>curlen");
					rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr1++)))&rbufmask));
					val1=*rbufr;
					CLIPINT(val1);
					*(dbuf++)=(SAMPLE)val1;
					dpos++;
				}
			}
			else {											// cross-fade
				while((wptr1<(ULONG)rptr1) && (dpos<samples) && smct) {
					ERRC((wptr1>curlen),"wptr1>curlen");
					ERRC2((wptr2>curlen),"wptr2>curlen, smct=%ld/%ld",smct,smrng);
					smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
					rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr1++)))&rbufmask));
					val2=smfc2*(*rbufr);
					rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr2++)))&rbufmask));
					val2+=smfc1*(*rbufr);
					CLIPFLT(val2);
					*(dbuf++)=(SAMPLE)val2;
					dpos++;
					smct--;
				}
			}
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
	instance->wptr1=wptr1;
	instance->wptr2=wptr2;
	instance->smct=smct;
}

void process_noseek_dyncor(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double timefs,timefd,timef;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu,rwdif;
	double rptr1,rptr2;						// read pointer
	ULONG wptr1,wptr2;						// write pointer
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw,*rbufr;
	SAMPLE *renvbase,*renvw,*renvr1,*renvr2;
	ULONG rbufmask,rbufsize;
	ULONG spos,dpos;
	double amp,ampenv,efc;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	timefs=instance->timefs;
	timefd=instance->timefd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	renvbase=instance->renvbase;
	renvw=instance->renvw;
	//
	spos=instance->spos;
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	wptr1=instance->wptr1;
	wptr2=instance->wptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;
	ampenv=instance->ampenv;

	/** @todo: use real dbuf.len instead of samples */
	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=*sbuf;										// fill the src ringbuffer

		amp=fabs((double)*sbuf);
		if(amp>ampenv) ampenv=amp;							// envelope tracker
		else ampenv=ENVFC1*ampenv+ENVFC2*amp;
		//ERRC2(((SAMPLE)ampenv==0),"ampenv dropped to 0 at %8ld/%8ld",curlen,instance->slen);
		if(ampenv<1.0) ampenv=1.0;
		*renvw=(SAMPLE)ampenv;								// fill the env ringbuffer

		// DEBUG
		//Write(__dbg_fh1,renvw,2);
		// DEBUG

		if(curlen>=winsize) {
			timef=timefs+timefd*modfunc1((curlen-winsize),instance->slen,moddata1);	// current time stretch factor

			val1=(LONG)rptr1;
			bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)
			if(val1>bndu) {									// gap is too wide, skip forward
				//MSG5("seek > : curlen=%6ld/%6ld  rptr1=%9.3lf > bndu=%6ld  wptr1=%6ld",curlen-winsize,curlen,rptr1,bndu,wptr1);
				rptr2=rptr1;							    	// remember old position
				wptr2=(ULONG)rptr2;
				rptr1=(double)(curlen-winsize3);				// restart 'rptr1' at 25% of winsize2
				wptr1-=(ULONG)(rptr2-rptr1);
				//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
				smct=smrng;										// activate cross-fade
			}
			else if(val1<bndl) {							// gap is too wide, skip back
				//MSG5("seek < : curlen=%6ld/%6ld  rptr1=%9.3lf < bndl=%6ld wptr1=%6ld",curlen-winsize,curlen,rptr1,bndl,wptr1);
				rptr2=rptr1;							    	// remember old position
				wptr2=(ULONG)rptr2;
				rptr1=(double)(curlen-winsize4);				// start 'rptr1' at 75% of winsize2
				wptr1+=(ULONG)(rptr1-rptr2);
				//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
				smct=smrng;										// activate cross-fade
			}

			rptr1+=timef;
			// this needs to be the position, we compare the volume with and adjust accordingly !!
			renvr1=(SAMPLE *)((ULONG)renvbase|((ULONG)((renvw+rbufsize)-winsize)&rbufmask));

			//if((curlen-winsize)<40) {
			//	  MSG2("%3ld : %5ld ",(curlen-winsize),*renvr1);
			//}
			/* diff=(curpos-dst.writepos);  // always positive !
			   read from src.writepos-diff;
			 */
			if(!smct) {										// do not cross-fade
				while((wptr1<(ULONG)rptr1) && (dpos<samples)) {
					rwdif=curlen-(wptr1++);
					//ERRC((rwdif<0),"wptr1>curlen");
					renvr2=(SAMPLE *)((ULONG)renvbase|((ULONG)((renvw+rbufsize)-rwdif)&rbufmask));
					rbufr =(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-rwdif)&rbufmask));

					efc=sqrt(sqrt((double)*renvr2/(double)*renvr1));
					ERRC4((efc>50.0),"0  : %8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,efc,*renvr2,*renvr1);
					//ERRC5((*renvr1==0),"0  : %8ld/%8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,curlen,efc,*renvr2,*renvr1);
					// DEBUG
					//Write(__dbg_fh2,renvr1,2);
					//Write(__dbg_fh3,renvr2,2);
					// DEBUG

					val1=(LONG)((double)*rbufr*((double)*renvr2/(double)*renvr1));
					CLIPINT(val1);
					*(dbuf++)=(SAMPLE)val1;
					dpos++;
					//if((curlen-winsize)<40) {
					//	  MSG4("    : %5ld %5ld/%5ld=%6.4lf",*rbufr,*renvr2,*renvr1,((double)*renvr2/(double)*renvr1));
					//}
				}
			}
			else {											// cross-fade
				while((wptr1<(ULONG)rptr1) && (dpos<samples) && smct) {
					rwdif=curlen-(wptr1++);
					//ERRC((rwdif<0),"wptr1>curlen");
					smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
					renvr2=(SAMPLE *)((ULONG)renvbase|((ULONG)((renvw+rbufsize)-rwdif)&rbufmask));
					rbufr= (SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-rwdif)&rbufmask));

					efc=sqrt(sqrt((double)*renvr2/(double)*renvr1));
					ERRC4((efc>50.0),"1a : %8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,efc,*renvr2,*renvr1);
					//ERRC5((*renvr1==0),"1a : %8ld/%8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,curlen,efc,*renvr2,*renvr1);
					// DEBUG
					//Write(__dbg_fh2,renvr1,2);
					//Write(__dbg_fh3,renvr2,2);
					// DEBUG

					val2=smfc2*(double)*rbufr*((double)*renvr2/(double)*renvr1);

					rwdif=curlen-(wptr2++);
					//ERRC((wptr2>curlen),"wptr2>curlen");
					renvr2=(SAMPLE *)((ULONG)renvbase|((ULONG)((renvw+rbufsize)-rwdif)&rbufmask));
					rbufr= (SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-rwdif)&rbufmask));

					efc=sqrt(sqrt((double)*renvr2/(double)*renvr1));
					ERRC4((efc>50.0),"1b : %8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,efc,*renvr2,*renvr1);
					//ERRC5((*renvr1==0),"1b : %8ld/8ld : efc=%lf  envr2=%ld / envr1=%ld",rwdif,curlen,efc,*renvr2,*renvr1);

					val2+=smfc1*(double)*rbufr*((double)*renvr2/(double)*renvr1);
					CLIPFLT(val2);
					*(dbuf++)=(SAMPLE)val2;
					dpos++;
					smct--;
				}
			}
		}
		sbuf++;spos--;curlen++;instance->curlen++;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
		renvw=(SAMPLE *)((ULONG)renvbase|((ULONG)(renvw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->renvw=renvw;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->spos=spos;
	instance->dpos=dpos;
	instance->rptr1=rptr1;
	instance->wptr1=wptr1;
	instance->wptr2=wptr2;
	instance->smct=smct;
	instance->ampenv=ampenv;
}

void process_noseek_rest(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long curlen;
	double timefs,timefd,timef;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf;
	LONG val1,bndl,bndu;
	double rptr1,rptr2;						// read pointer
	ULONG wptr1,wptr2;						// write pointer
	double smfc1,smfc2,val2;				// cross-fade factors
	ULONG smrng,smct,winsize,winsize3,winsize4;
	SAMPLE *rbufbase,*rbufw,*rbufr;
	ULONG rbufmask,rbufsize;
	ULONG dpos;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	timefs=instance->timefs;
	timefd=instance->timefd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufsize=instance->rbufsize;
	rbufw=instance->rbufw;
	//
	dpos=instance->dpos;
	rptr1=instance->rptr1;
	wptr1=instance->wptr1;
	wptr2=instance->wptr2;
	winsize=instance->winsize;
	winsize3=instance->winsize3;
	winsize4=instance->winsize4;
	smrng=instance->smrng;
	smct=instance->smct;

	/** @todo: use real dbuf.len instead of samples */
	while((dpos<samples) && (instance->rest>0)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*rbufw=0;										// fill the ringbuffer
		timef=timefs+timefd*modfunc1((curlen-winsize),instance->slen,moddata1);	// current time stretch factor

		val1=(LONG)rptr1;
		bndl=curlen-winsize3;bndu=curlen-winsize4;	// for smoothing (leaving space for further reads)
		if(val1>bndu) {									// gap is too wide, skip forward
			//MSG5("seek > : curlen=%6ld/%6ld  rptr1=%9.3lf > bndu=%6ld  wptr1=%6ld",curlen-winsize,curlen,rptr1,bndu,wptr1);
			rptr2=rptr1;							    	// remember old position
			wptr2=(ULONG)rptr2;
			rptr1=(double)(curlen-winsize3);				// restart 'rptr1' at 25% of winsize2
			wptr1-=(ULONG)(rptr2-rptr1);
			//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
			smct=smrng;										// activate cross-fade
		}
		else if(val1<bndl) {							// gap is too wide, skip back
			//MSG5("seek < : curlen=%6ld/%6ld  rptr1=%9.3lf < bndl=%6ld wptr1=%6ld",curlen-winsize,curlen,rptr1,bndl,wptr1);
			rptr2=rptr1;									// remember old position
			wptr2=(ULONG)rptr2;
			rptr1=(double)(curlen-winsize4);				// start 'rptr1' at 75% of winsize2
			wptr1+=(ULONG)(rptr1-rptr2);
			//MSG2("       :                       rptr1=%9.3lf              wptr1=%6ld",rptr1,wptr1);
			smct=smrng;										// activate cross-fade
		}

		rptr1+=timef;
		if(!smct) {										// do not cross-fade
			while((wptr1<(ULONG)rptr1) && (dpos<samples)) {
				rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr1++)))&rbufmask));
				val1=*rbufr;
				CLIPINT(val1);
				*(dbuf++)=(SAMPLE)val1;
				dpos++;
			}
		}
		else {											// cross-fade
			while((wptr1<(ULONG)rptr1) && (dpos<samples) && smct) {
				smfc1=(double)smct/(double)smrng;smfc2=1.0-smfc1;
				rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr1++)))&rbufmask));
				val2=smfc2*(*rbufr);
				rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)((rbufw+rbufsize)-(curlen-(wptr2++)))&rbufmask));
				val2+=smfc1*(*rbufr);
				CLIPFLT(val2);
				*(dbuf++)=(SAMPLE)val2;
				dpos++;
				smct--;
			}
		}
		curlen++;instance->rest--;
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->dbuf=dbuf;
	instance->dpos=dpos;
	instance->rptr1=rptr1;
	instance->wptr1=wptr1;
	instance->wptr2=wptr2;
	instance->smct=smct;
}



void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->rbuf)	{ DelRingBuf(instance->rbuf);instance->rbuf=NULL; }
	if(instance->renv)	{ DelRingBuf(instance->renv);instance->renv=NULL; }

	// DEBUG
	//Close(__dbg_fh1);
	//Close(__dbg_fh2);
	//Close(__dbg_fh3);
	// DEBUG
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}

double __inline SeekContinue_0(ULONG curlen,double rptr1,SAMPLE *rbufw,SAMPLE *rbufbase,ULONG rbufsize,ULONG rbufmask,ULONG winsize,ULONG winsize3,ULONG winsize4,BYTE dir) {
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
	//MSG2("  cur-last=%6ld-%6ld",*rbufr2,*rbufr3);
	//MSG1("  delta : %1ld",delta2);

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
double __inline SeekContinue_1(ULONG curlen,double rptr1,SAMPLE *rbufw,SAMPLE *rbufbase,ULONG rbufsize,ULONG rbufmask,ULONG winsize,ULONG winsize3,ULONG winsize4,BYTE dir) {
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

//-- eof ----------------------------------------------------------------------
