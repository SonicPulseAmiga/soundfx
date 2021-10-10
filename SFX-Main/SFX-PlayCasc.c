/****h* SoundFX/SFX-PlayCasc.c [4.2] *
*
*  NAME
*    SFX-PlayCasc.c
*  COPYRIGHT
*    $VER: SFX-PlayCasc.c 4.2 (30.10.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    samples playroutines for Paula 14 bit - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    30.Oct.2002	V 4.2	most recent version
*    28.Nov.2000	V 4.1	most recent version
*    26.Jan.2000	V 3.82	most recent version
*    18.Aug.1998	V 3.71	most recent version
*    13.Aug.1998	V 3.7	initial version
*  NOTES
*
*******
*/

/****u* _players/Amiga-14bit *
*  Function.eng
*    Plays the active sample over cascaded soundchannels in 14-bit, whithout extra hardware.
*    The maximum playbackrate on PAL/NTSC screens is about 29Khz and on Productivity
*    screens about 58kHz.
*  Funktion.ger
*    Spielt das Sample des aktuellen Puffers über Cascadierung der Soundkanäle in 14-bit ab.
*    Es wird keine zusätzliche Hardware benötigt. Die maximale Abspielrate beträgt auf
*    PAL/NTSC-Bildschirmen ca. 29Khz und auf Productivity-Bildschirmen ca. 58kHz.
*  Parameter.eng
*    HFilter
*      Hardware filter on/off (Power LED)
*    RateClip
*      maximum playbackrate, if the samplerate is higher, SFX resamples while
*      playing, so the pitch is right.
*  Parameter.ger
*    HFilter
*      ob der Hardwarefilter an- oder ausgeschaltet werden soll.
*    RateClip
*      max. abspielbare Rate, wenn die Samplingrate des Samples größer ist, wird
*      es während des Abspielens resampled, so das es wie mit der entsprechenden Rate abgespielt klingt.
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define SFX_PlayCasc_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"
#include "SFX-ChipRegDefs.h"

//-- prototypes ---------------------------------------------------------------

void PlayCascSample_All(void);
void PlayCascSample_Rng(void);
void StopCascSample(void);

__interrupt __saveds void PlayCascLOffMInt(void);
__interrupt __saveds void PlayCascLOffSInt(void);
__interrupt __saveds void PlayCascLOffQInt(void);
__interrupt __saveds void PlayCascLOnMInt(void);
__interrupt __saveds void PlayCascLOnSInt(void);
__interrupt __saveds void PlayCascLOnQInt(void);

//-- definitions --------------------------------------------------------------

void PlayCascSample_All(void) {
	UWORD playperi;

	plct=0;plst=1;
	if(optp_Amiga_14bit.rateclip==0 && RunTime.plsi->srat>28867) { plst=2;while((RunTime.plsi->srat/plst)>28867) plst<<=1; }
	if(optp_Amiga_14bit.rateclip==1 && RunTime.plsi->srat>57734) { plst=2;while((RunTime.plsi->srat/plst)>57734) plst<<=1; }
	plst2=plst+plst;
	playperi=(*(struct ExecBase**)(4))->ex_EClockFrequency*5*plst/RunTime.plsi->srat;

	if(RunTime.plsi->loop && RunTime.plsi->loopl) {
		switch(RunTime.plsi->channels) {
			case CH_MONO:
				plsa0=SAInit(RunTime.plsi,0,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				audint->is_Code=PlayCascLOnMInt;
				break;
			case CH_STEREO:
				plsa0=SAInit(RunTime.plsi,0,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				plsa1=SAInit(RunTime.plsi,1,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				audint->is_Code=PlayCascLOnSInt;
				break;
			case CH_QUADRO:
				plsa0=SAInit(RunTime.plsi,0,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				plsa1=SAInit(RunTime.plsi,1,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				plsa2=SAInit(RunTime.plsi,2,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				plsa3=SAInit(RunTime.plsi,3,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				audint->is_Code=PlayCascLOnQInt;
				break;
		}
	}
	else {
		switch(RunTime.plsi->channels) {
			case CH_MONO:
				plsa0=SAInit(RunTime.plsi,0,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				audint->is_Code=PlayCascLOffMInt;
				break;
			case CH_STEREO:
				plsa0=SAInit(RunTime.plsi,0,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				plsa1=SAInit(RunTime.plsi,1,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				audint->is_Code=PlayCascLOffSInt;
				break;
			case CH_QUADRO:
				plsa0=SAInit(RunTime.plsi,0,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				plsa1=SAInit(RunTime.plsi,1,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				plsa2=SAInit(RunTime.plsi,2,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				plsa3=SAInit(RunTime.plsi,3,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	  // sequentiell Blöcke konstanter Größe lesen
				audint->is_Code=PlayCascLOffQInt;
				break;
		}
	}
	plsa[0]=plsa0;plsa[1]=plsa1;plsa[2]=plsa2;plsa[3]=plsa3;
	SARead =plsa0->safunc;
	RunTime.plptr=RunTime.oldptr=0;RunTime.play=PLAY_ALL;RunTime.plstop=0;
	DrawPPos(RunTime.plsi,RunTime.oldptr);	 /* Draw Pos */
	if(RunTime.plsi->loop && RunTime.plsi->loopl) RecalcLoopPlayData();
	else RunTime.plslen=RunTime.plsi->slen;
	audint->is_Node.ln_Type	=NT_INTERRUPT;
	audint->is_Node.ln_Pri	=0;
	audint->is_Node.ln_Name	=intname;
	audint->is_Data			=NULL;

	*dmacon=(UWORD)0xF;							/* alle Audiokanäle aus */
	if(optp_Amiga_14bit.hfilter) *PRA|=(UBYTE)2;	/* LED / PA1 */

	*aud3len=*aud2len=*aud1len=*aud0len=(UWORD)MIN_PLAYBUF_SAMPLES;
	*aud1vol=*aud0vol=(UWORD)64;
	*aud2vol=*aud3vol=(UWORD)1;
	*aud3per=*aud2per=*aud1per=*aud0per=(UWORD)playperi;
	*aud3lc =*aud2lc =*aud1lc =*aud0lc =(ULONG)ebuflr;

	*intena=0x0080;								/* AUD0-IRQ off */
	oldaudint=SetIntVector(INTB_AUD0,audint);

	*intena=0x8080;								/* AUD0-IRQ an */
	*dmacon=(UWORD)0x800F;						/* AUD0-DMA an */
}

void PlayCascSample_Rng(void) {
	UWORD playperi;

	SetPlayRange();
	plct=0;plst=1;
	RunTime.oldptr=0;RunTime.play=PLAY_RNG;RunTime.plstop=0;
	DrawPPos(RunTime.plsi,RunTime.oldptr);	 /* Draw Pos */

	if(optp_Amiga_14bit.rateclip==0 && RunTime.plsi->srat>28867) { plst=2;while((RunTime.plsi->srat/plst)>28867) plst<<=1; }
	if(optp_Amiga_14bit.rateclip==1 && RunTime.plsi->srat>57734) { plst=2;while((RunTime.plsi->srat/plst)>57734) plst<<=1; }
	plst2=plst+plst;
	playperi=(*(struct ExecBase**)(4))->ex_EClockFrequency*5*plst/RunTime.plsi->srat;
	
	switch(RunTime.plsi->channels) {
		case CH_MONO:
			plsa0=SAInit(RunTime.plsi,0,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	 // sequentiell Blöcke konstanter Größe lesen
			audint->is_Code=PlayCascLOffMInt;
			break;
		case CH_STEREO:
			plsa0=SAInit(RunTime.plsi,0,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	 // sequentiell Blöcke konstanter Größe lesen
			plsa1=SAInit(RunTime.plsi,1,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	 // sequentiell Blöcke konstanter Größe lesen
			audint->is_Code=PlayCascLOffSInt;
			break;
		case CH_QUADRO:
			plsa0=SAInit(RunTime.plsi,0,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	 // sequentiell Blöcke konstanter Größe lesen
			plsa1=SAInit(RunTime.plsi,1,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	 // sequentiell Blöcke konstanter Größe lesen
			plsa2=SAInit(RunTime.plsi,2,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	 // sequentiell Blöcke konstanter Größe lesen
			plsa3=SAInit(RunTime.plsi,3,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,(MIN_PLAYBUF_BYTES*plst2));	 // sequentiell Blöcke konstanter Größe lesen
			audint->is_Code=PlayCascLOffQInt;
			break;
	}
	plsa[0]=plsa0;plsa[1]=plsa1;plsa[2]=plsa2;plsa[3]=plsa3;
	SARead =plsa0->safunc;
	audint->is_Node.ln_Type	=NT_INTERRUPT;
	audint->is_Node.ln_Pri	=0;
	audint->is_Node.ln_Name	=intname;
	audint->is_Data			=NULL;

	*dmacon=(UWORD)0xF;							/* alle Audiokanäle aus */
	if(optp_Amiga_14bit.hfilter) *PRA|=(UBYTE)2;	/* LED / PA1 */

	*aud3len=*aud2len=*aud1len=*aud0len=(UWORD)MIN_PLAYBUF_SAMPLES;
	*aud1vol=*aud0vol=(UWORD)64;
	*aud2vol=*aud3vol=(UWORD)1;
	*aud3per=*aud2per=*aud1per=*aud0per=(UWORD)playperi;
	*aud3lc =*aud2lc =*aud1lc =*aud0lc =(ULONG)ebuflr;

	*intena=0x0080;								/* AUD0-IRQ off */
	oldaudint=SetIntVector(INTB_AUD0,audint);

	*intena=0x8080;								/* AUD0-IRQ an */
	*dmacon=(UWORD)0x800F;						/* AUD0-DMA an */
}

void StopCascSample(void) {
	DrawPPos(RunTime.plsi,RunTime.oldptr);					 /* Delete Pos */

	*aud3vol=*aud2vol=*aud1vol=*aud0vol=(UWORD)0;	/* Audiokanal leise schalten */
	*dmacon=(UWORD)0xF;								/* alle Audiokanäle aus */

	*intena=0x0080;									/* AUD0-IRQ off */
	SetIntVector(INTB_AUD0,oldaudint);oldaudint=NULL;

	if(plsa0) { SADone(plsa0,RunTime.plsi);plsa0=NULL; }
	if(plsa1) { SADone(plsa1,RunTime.plsi);plsa1=NULL; }
	if(plsa2) { SADone(plsa2,RunTime.plsi);plsa2=NULL; }
	if(plsa3) { SADone(plsa3,RunTime.plsi);plsa3=NULL; }

	RunTime.play=PLAY_OFF;
	ModifyIDCMP(RunTime.winBackDrop,IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_MENUPICK|IDCMP_MENUVERIFY);
	if(SFX_SOUND_IS_SHOWN(RunTime.plsi)) SetWindowTitles(RunTime.plsi->win,RunTime.plsi->title,(UBYTE *)-1);
}

/* Cascade LoopOff Mono */
__interrupt __saveds void PlayCascLOffMInt(void) {
	register UWORD i;

	*intreq=0x0080;
	if(RunTime.plptr<RunTime.plslen) {
		if(plsa0->state!=STATE_WORKING) {
			plbuf0=(UWORD *)plsa0->buffer1;SARead(plsa0,plsa0->blen);
			if(plct) {
				sptr0=sbuf0l;vptr0=vbuf0l;
				for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
					s2=(*plbuf0&0xFF00);v2=(*plbuf0&0xFC)<<6;plbuf0+=plst;
					s2|=(*plbuf0>>8);v2|=((*plbuf0&0xFC)>>2);plbuf0+=plst;
					*(sptr0++)=s2;
					*(vptr0++)=v2;
				}
				*aud1lc=*aud0lc=(ULONG)sbuf0l;
				*aud2lc=*aud3lc=(ULONG)vbuf0l;
			}
			else {
				sptr0=sbuf1l;vptr0=vbuf1l;
				for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
					s2=(*plbuf0&0xFF00);v2=(*plbuf0&0xFC)<<6;plbuf0+=plst;
					s2|=(*plbuf0>>8);v2|=((*plbuf0&0xFC)>>2);plbuf0+=plst;
					*(sptr0++)=s2;
					*(vptr0++)=v2;
				}
				*aud1lc=*aud0lc=(ULONG)sbuf1l;
				*aud2lc=*aud3lc=(ULONG)vbuf1l;
			}
			plct^=1;
		}
	}
	else {
		RunTime.plstop++;
		*aud1lc=*aud0lc=(ULONG)ebuflr;
	}
}

/* Cascade LoopOff Stereo */
__interrupt __saveds void PlayCascLOffSInt(void) {
	register UWORD i;

	*intreq=0x0080;			// IRQ's off
	if(RunTime.plptr<RunTime.plslen) {
		if(plsa0->state!=STATE_WORKING && plsa1->state!=STATE_WORKING) {
			plbuf0=(UWORD *)plsa0->buffer1;SARead(plsa0,plsa0->blen);
			plbuf1=(UWORD *)plsa1->buffer1;SARead(plsa1,plsa1->blen);
			if(plct) {
				sptr0=sbuf0l;sptr1=sbuf0r;vptr0=vbuf0l;vptr1=vbuf0r;
				for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
					s2=(*plbuf0&0xFF00);v2=(*plbuf0&0xFC)<<6;plbuf0+=plst;
					s2|=(*plbuf0)>>8;v2|=((*plbuf0&0xFC)>>2);plbuf0+=plst;
					*(sptr0++)=s2;
					*(vptr0++)=v2;
					s2=(*plbuf1&0xFF00);v2=(*plbuf1&0xFC)<<6;plbuf1+=plst;
					s2|=(*plbuf1>>8);v2|=((*plbuf1&0xFC)>>2);plbuf1+=plst;
					*(sptr1++)=s2;
					*(vptr1++)=v2;
				}
				*aud0lc=(ULONG)sbuf0l;*aud1lc=(ULONG)sbuf0r;
				*aud3lc=(ULONG)vbuf0l;*aud2lc=(ULONG)vbuf0r;
			}
			else {
				sptr0=sbuf1l;sptr1=sbuf1r;vptr0=vbuf1l;vptr1=vbuf1r;
				for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
					s2=(*plbuf0&0xFF00);v2=(*plbuf0&0xFC)<<6;plbuf0+=plst;
					s2|=(*plbuf0)>>8;v2|=((*plbuf0&0xFC)>>2);plbuf0+=plst;
					*(sptr0++)=s2;
					*(vptr0++)=v2;
					s2=(*plbuf1&0xFF00);v2=(*plbuf1&0xFC)<<6;plbuf1+=plst;
					s2|=(*plbuf1>>8);v2|=((*plbuf1&0xFC)>>2);plbuf1+=plst;
					*(sptr1++)=s2;
					*(vptr1++)=v2;
				}
				*aud0lc=(ULONG)sbuf1l;*aud1lc=(ULONG)sbuf1r;
				*aud3lc=(ULONG)vbuf1l;*aud2lc=(ULONG)vbuf1r;
			}
			plct^=1;
		}
	}
	else {
		RunTime.plstop++;
		*aud1lc=*aud0lc=(ULONG)ebuflr;
	}
}

/* Cascade LoopOff Quadro */
__interrupt __saveds void PlayCascLOffQInt(void) {
	register UWORD i;

	*intreq=0x0080;
	if(RunTime.plptr<RunTime.plslen) {
		if(plsa0->state!=STATE_WORKING && plsa1->state!=STATE_WORKING && plsa2->state!=STATE_WORKING && plsa3->state!=STATE_WORKING) {
			plbuf0=(UWORD *)plsa0->buffer1;SARead(plsa0,plsa0->blen);
			plbuf1=(UWORD *)plsa1->buffer1;SARead(plsa1,plsa1->blen);
			plbuf2=(UWORD *)plsa2->buffer1;SARead(plsa2,plsa2->blen);
			plbuf3=(UWORD *)plsa3->buffer1;SARead(plsa3,plsa3->blen);
			if(plct) {
				sptr0=sbuf0l;sptr1=sbuf0r;vptr0=vbuf0l;vptr1=vbuf0r;
				for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
					s2=((sv=(*plbuf0+*plbuf2)>>1)&0xFF00);v2=(sv&0xFC)<<6;plbuf0+=plst;plbuf2+=plst;
					s2|=((sv=(*plbuf0+*plbuf2)>>1)>>8);v2|=((sv&0xFC)>>2);plbuf0+=plst;plbuf2+=plst;
					*(sptr0++)=s2;
					*(vptr0++)=v2;
					s2=((sv=(*plbuf1+*plbuf3)>>1)&0xFF00);v2=(sv&0xFC)<<6;plbuf1+=plst;plbuf3+=plst;
					s2|=((sv=(*plbuf1+*plbuf3)>>1)>>8);v2|=((sv&0xFC)>>2);plbuf1+=plst;plbuf3+=plst;
					*(sptr1++)=s2;
					*(vptr1++)=v2;
				}
				*aud0lc=(ULONG)sbuf0l;*aud1lc=(ULONG)sbuf0r;
				*aud3lc=(ULONG)vbuf0l;*aud2lc=(ULONG)vbuf0r;
			}
			else {
				sptr0=sbuf1l;sptr1=sbuf1r;vptr0=vbuf1l;vptr1=vbuf1r;
				for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
					s2=((sv=(*plbuf0+*plbuf2)>>1)&0xFF00);v2=(sv&0xFC)<<6;plbuf0+=plst;plbuf2+=plst;
					s2|=((sv=(*plbuf0+*plbuf2)>>1)>>8);v2|=((sv&0xFC)>>2);plbuf0+=plst;plbuf2+=plst;
					*(sptr0++)=s2;
					*(vptr0++)=v2;
					s2=((sv=(*plbuf1+*plbuf3)>>1)&0xFF00);v2=(sv&0xFC)<<6;plbuf1+=plst;plbuf3+=plst;
					s2|=((sv=(*plbuf1+*plbuf3)>>1)>>8);v2|=((sv&0xFC)>>2);plbuf1+=plst;plbuf3+=plst;
					*(sptr1++)=s2;
					*(vptr1++)=v2;
				}
				*aud1lc=(ULONG)sbuf1l;*aud0lc=(ULONG)sbuf1r;
				*aud3lc=(ULONG)vbuf1l;*aud2lc=(ULONG)vbuf1r;
			}
			plct^=1;
		}
	}
	else {
		RunTime.plstop++;
		*aud1lc=*aud0lc=(ULONG)ebuflr;
	}
}

/* Cascade LoopOn Mono */
__interrupt __saveds void PlayCascLOnMInt(void) {
	register UWORD i;
	LONG off;

	*intreq=0x0080;
	if(plsa0->state!=STATE_WORKING) {
		off=(LONG)(RunTime.plptr+(MIN_PLAYBUF_SAMPLES*plst2))-(LONG)RunTime.plslen;
		if(off>=0) plsa0->seek=Samples2Bytes(RunTime.plsi->loops+off);
		plbuf0=(UWORD *)plsa0->buffer1;SARead(plsa0,plsa0->blen);
		if(plct) {
			sptr0=sbuf0l;vptr0=vbuf0l;
			for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
				if(RunTime.plptr>=RunTime.plslen) { plbuf0=(UWORD *)plsa0->buffer3;RunTime.plptr=RunTime.pllstart; }
				s2=(*plbuf0&0xFF00);v2=(*plbuf0&0xFC)<<6;plbuf0+=plst;
				s2|=(*plbuf0>>8);v2|=((*plbuf0&0xFC)>>2);plbuf0+=plst;
				*(sptr0++)=s2;
				*(vptr0++)=v2;
			}
			*aud1lc=*aud0lc=(ULONG)sbuf0l;
			*aud2lc=*aud3lc=(ULONG)vbuf0l;
		}
		else {
			sptr0=sbuf1l;vptr0=vbuf1l;
			for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
				if(RunTime.plptr>=RunTime.plslen) { plbuf0=(UWORD *)plsa0->buffer3;RunTime.plptr=RunTime.pllstart; }
				s2=(*plbuf0&0xFF00);v2=(*plbuf0&0xFC)<<6;plbuf0+=plst;
				s2|=(*plbuf0>>8);v2|=((*plbuf0&0xFC)>>2);plbuf0+=plst;
				*(sptr0++)=s2;
				*(vptr0++)=v2;
			}
			*aud1lc=*aud0lc=(ULONG)sbuf1l;
			*aud2lc=*aud3lc=(ULONG)vbuf1l;
		}
		plct^=1;
	}
}

/* Cascade LoopOn Stereo */
__interrupt __saveds void PlayCascLOnSInt(void) {
	register UWORD i;
	LONG off;

	*intreq=0x0080;
	if(plsa0->state!=STATE_WORKING && plsa1->state!=STATE_WORKING) {
		off=(LONG)(RunTime.plptr+(MIN_PLAYBUF_SAMPLES*plst2))-(LONG)RunTime.plslen;
		if(off>=0) plsa0->seek=plsa1->seek=Samples2Bytes(RunTime.plsi->loops+off);
		plbuf0=(UWORD *)plsa0->buffer1;SARead(plsa0,plsa0->blen);
		plbuf1=(UWORD *)plsa1->buffer1;SARead(plsa1,plsa1->blen);
		if(plct) {
			sptr0=sbuf0l;sptr1=sbuf0r;vptr0=vbuf0l;vptr1=vbuf0r;
			for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
				if(RunTime.plptr>=RunTime.plslen) { plbuf0=(UWORD *)plsa0->buffer3;plbuf1=(UWORD *)plsa1->buffer3;RunTime.plptr=RunTime.pllstart; }
				s2=(*plbuf0&0xFF00);v2=(*plbuf0&0xFC)<<6;plbuf0+=plst;
				s2|=(*plbuf0)>>8;v2|=((*plbuf0&0xFC)>>2);plbuf0+=plst;
				*(sptr0++)=s2;
				*(vptr0++)=v2;
				s2=(*plbuf1&0xFF00);v2=(*plbuf1&0xFC)<<6;plbuf1+=plst;
				s2|=(*plbuf1>>8);v2|=((*plbuf1&0xFC)>>2);plbuf1+=plst;
				*(sptr1++)=s2;
				*(vptr1++)=v2;
			}
			*aud0lc=(ULONG)sbuf0l;*aud1lc=(ULONG)sbuf0r;
			*aud3lc=(ULONG)vbuf0l;*aud2lc=(ULONG)vbuf0r;
		}
		else {
			sptr0=sbuf1l;sptr1=sbuf1r;vptr0=vbuf1l;vptr1=vbuf1r;
			for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
				if(RunTime.plptr>=RunTime.plslen) { plbuf0=(UWORD *)plsa0->buffer3;plbuf1=(UWORD *)plsa1->buffer3;RunTime.plptr=RunTime.pllstart; }
				s2=(*plbuf0&0xFF00);v2=(*plbuf0&0xFC)<<6;plbuf0+=plst;
				s2|=(*plbuf0)>>8;v2|=((*plbuf0&0xFC)>>2);plbuf0+=plst;
				*(sptr0++)=s2;
				*(vptr0++)=v2;
				s2=(*plbuf1&0xFF00);v2=(*plbuf1&0xFC)<<6;plbuf1+=plst;
				s2|=(*plbuf1>>8);v2|=((*plbuf1&0xFC)>>2);plbuf1+=plst;
				*(sptr1++)=s2;
				*(vptr1++)=v2;
			}
			*aud0lc=(ULONG)sbuf1l;*aud1lc=(ULONG)sbuf1r;
			*aud3lc=(ULONG)vbuf1l;*aud2lc=(ULONG)vbuf1r;
		}
		plct^=1;
	}
}

/* Cascade LoopOn Quadro */
__interrupt __saveds void PlayCascLOnQInt(void) {
	register UWORD i;
	LONG off;

	*intreq=0x0080;
	if(plsa0->state!=STATE_WORKING && plsa1->state!=STATE_WORKING && plsa2->state!=STATE_WORKING && plsa3->state!=STATE_WORKING) {
		off=(LONG)(RunTime.plptr+(MIN_PLAYBUF_SAMPLES*plst2))-(LONG)RunTime.plslen;
		if(off>=0) plsa0->seek=plsa1->seek=plsa2->seek=plsa3->seek=Samples2Bytes(RunTime.plsi->loops+off);
		plbuf0=(UWORD *)plsa0->buffer1;SARead(plsa0,plsa0->blen);
		plbuf1=(UWORD *)plsa1->buffer1;SARead(plsa1,plsa1->blen);
		plbuf2=(UWORD *)plsa2->buffer1;SARead(plsa2,plsa2->blen);
		plbuf3=(UWORD *)plsa3->buffer1;SARead(plsa3,plsa3->blen);
		if(plct) {
			sptr0=sbuf0l;sptr1=sbuf0r;vptr0=vbuf0l;vptr1=vbuf0r;
			for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
				if(RunTime.plptr>=RunTime.plslen) { plbuf0=(UWORD *)plsa0->buffer3;plbuf1=(UWORD *)plsa1->buffer3;plbuf2=(UWORD *)plsa2->buffer3;plbuf3=(UWORD *)plsa3->buffer3;RunTime.plptr=RunTime.pllstart; }
				s2=((sv=(*plbuf0+*plbuf2)>>1)&0xFF00);v2=(sv&0xFC)<<6;plbuf0+=plst;plbuf2+=plst;
				s2|=((sv=(*plbuf0+*plbuf2)>>1)>>8);v2|=((sv&0xFC)>>2);plbuf0+=plst;plbuf2+=plst;
				*(sptr0++)=s2;
				*(vptr0++)=v2;
				s2=((sv=(*plbuf1+*plbuf3)>>1)&0xFF00);v2=(sv&0xFC)<<6;plbuf1+=plst;plbuf3+=plst;
				s2|=((sv=(*plbuf1+*plbuf3)>>1)>>8);v2|=((sv&0xFC)>>2);plbuf1+=plst;plbuf3+=plst;
				*(sptr1++)=s2;
				*(vptr1++)=v2;
			}
			*aud0lc=(ULONG)sbuf0l;*aud1lc=(ULONG)sbuf0r;
			*aud3lc=(ULONG)vbuf0l;*aud2lc=(ULONG)vbuf0r;
		}
		else {
			sptr0=sbuf1l;sptr1=sbuf1r;vptr0=vbuf1l;vptr1=vbuf1r;
			for(i=0;i<MIN_PLAYBUF_SAMPLES;i++,RunTime.plptr+=plst2) {
				if(RunTime.plptr>=RunTime.plslen) { plbuf0=(UWORD *)plsa0->buffer3;plbuf1=(UWORD *)plsa1->buffer3;plbuf2=(UWORD *)plsa2->buffer3;plbuf3=(UWORD *)plsa3->buffer3;RunTime.plptr=RunTime.pllstart; }
				s2=((sv=(*plbuf0+*plbuf2)>>1)&0xFF00);v2=(sv&0xFC)<<6;plbuf0+=plst;plbuf2+=plst;
				s2|=((sv=(*plbuf0+*plbuf2)>>1)>>8);v2|=((sv&0xFC)>>2);plbuf0+=plst;plbuf2+=plst;
				*(sptr0++)=s2;
				*(vptr0++)=v2;
				s2=((sv=(*plbuf1+*plbuf3)>>1)&0xFF00);v2=(sv&0xFC)<<6;plbuf1+=plst;plbuf3+=plst;
				s2|=((sv=(*plbuf1+*plbuf3)>>1)>>8);v2|=((sv&0xFC)>>2);plbuf1+=plst;plbuf3+=plst;
				*(sptr1++)=s2;
				*(vptr1++)=v2;
			}
			*aud1lc=(ULONG)sbuf1l;*aud0lc=(ULONG)sbuf1r;
			*aud3lc=(ULONG)vbuf1l;*aud2lc=(ULONG)vbuf1r;
		}
		plct^=1;
	}
}

//-- eof ----------------------------------------------------------------------
