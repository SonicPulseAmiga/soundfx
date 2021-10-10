/****h* SoundFX/SFX-PlayAhi.c [4.2] *
*
*  NAME
*    SFX-PlayAhi.c
*  COPYRIGHT
*    $VER: SFX-PlayAhi.c 4.2 (30.10.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    sample playroutines for ahi - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    06.Jul.1998
*  MODIFICATION HISTORY
*    30.Oct.2002	V 4.2	most recent version
*    28.Nov.2000	V 4.1	most recent version
*    26.Jun.2000	V 4.0	most recent version
*    25.Jan.2000	V 3.8	most recent version
*    18.Aug.1998	V 3.71	most recent version
*    06.Jul.1998	V 3.7	initial version
*  NOTES
*
*******
*/

/****u* _players/Ahi *
*  Function.eng
*    Plays the active sample with the AHI audio system by Martin Blom. This can be
*    downloaded from the following sources :<br>
*    Aminet:<br>
*    <a href="ftp://ftp.germany.aminet.org/pub/aminet/dev/misc/ahidev.lha">ahidev.lha</a><br>
*    <a href="ftp://ftp.germany.aminet.org/pub/aminet/mus/misc/ahiusr.lha">ahiusr.lha</a><br>
*    <a href="ftp://ftp.germany.aminet.org/pub/aminet/docs/misc/ahiman.lha">ahiman.lha</a><br>
*    <br>
*    WWW:<br>
*    <a href="http://www.lysator.liu.se/~lcs/ahi.html">http://www.lysator.liu.se/~lcs/ahi.html</a><br>
*  Funktion.ger
*    Spielt das aktive Sample über das AHI Audio System von Martin Blom ab.
*    Dieses könen sie von folgenden Quellen herunterladen :<br>
*    Aminet:<br>
*    <a href="ftp://ftp.germany.aminet.org/pub/aminet/dev/misc/ahidev.lha">ahidev.lha</a><br>
*    <a href="ftp://ftp.germany.aminet.org/pub/aminet/mus/misc/ahiusr.lha">ahiusr.lha</a><br>
*    <a href="ftp://ftp.germany.aminet.org/pub/aminet/docs/misc/ahiman.lha">ahiman.lha</a><br>
*    <br>
*    WWW:<br>
*    <a href="http://www.lysator.liu.se/~lcs/ahi.html">http://www.lysator.liu.se/~lcs/ahi.html</a><br>
*  Parameter.eng
*    Audiomode
*      Here you can choose the auidomode (which audiohardware, how many channels,...)
*      and what mixing frequency (sampling rate for playback) should be used.
*  Parameter.ger
*    Audiomode
*      Hier kann man den Audiomodus (also welche Audiohardware, wieviele Kan&auml;le,...)
*      und die zu verwendende Mischfrequenz (Samplingrate f&uuml; das Abspielen) festlegen.
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define SFX_PlayAhi_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void PlayAhiASample_All(void);
void PlayAhiASample_Rng(void);
void StopAhiASample(void);

UBYTE InitAhiAPlayer(void);
void FreeAhiAPlayer(void);
void ClearAHIBuffers(void);

__asm __interrupt __saveds ULONG PlayAhiMemLOffInt(register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHISoundMessage *chan);
__asm __interrupt __saveds ULONG PlayAhiMemLOnInt (register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHISoundMessage *chan);
__asm __interrupt __saveds ULONG PlayAhiDevLOffInt(register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHISoundMessage *chan);
__asm __interrupt __saveds ULONG PlayAhiDevLOnInt (register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHISoundMessage *chan);

//-- globals ------------------------------------------------------------------

struct Library    		*AHIBase;
struct MsgPort    		*AHImp=NULL;
struct AHIRequest 		*AHIio=NULL;
BYTE   					AHIDevice=-1;
struct AHIAudioCtrl 	*audioctrl=NULL;
struct AHISampleInfo	ch0[4],ch1[4];
struct Hook SoundHook = {0};
WORD *sbuf0[4]={NULL},*sbuf1[4]={NULL};
LONG off,choffs=0;

//-- definitions --------------------------------------------------------------

void PlayAhiASample_All(void) {
	ULONG plsmpl;
	ULONG err=AHIE_OK;
	UBYTE fail=FALSE;
	struct TagItem audiotags[]={
		AHIA_AudioID,	0,
		AHIA_MixFreq,	0,
		AHIA_Channels,	0,
		AHIA_Sounds,	0,
		AHIA_SoundFunc,	NULL,
		TAG_DONE
	};

	if(AHIBase && optp_Ahi.playmode!=AHI_INVALID_ID) {
		//MSG3("playing : mode 0x%08x  rate %5d  chan %2d",optp_Ahi.playmode,optp_Ahi.playrate,RunTime.plsi->channels+1);
		RunTime.plptr=RunTime.oldptr=0;RunTime.play=PLAY_ALL;RunTime.plstop=0;
		DrawPPos(RunTime.plsi,RunTime.oldptr);	 /* Draw Pos */
		plct=0;off=-1;
		audiotags[0].ti_Data=optp_Ahi.playmode;
		audiotags[1].ti_Data=(ULONG)optp_Ahi.playrate;
		audiotags[2].ti_Data=(ULONG)(RunTime.plsi->channels+1);		 /* {0,1,3} +1 -> {1,2,4} */
		if(RunTime.plsi->storage==VM_MEMORY) audiotags[3].ti_Data=(ULONG)(RunTime.plsi->channels+1);
		else audiotags[3].ti_Data=(ULONG)((RunTime.plsi->channels+1)<<1);
		if(RunTime.plsi->loop && RunTime.plsi->loopl) {
			if(RunTime.plsi->storage==VM_MEMORY) SoundHook.h_Entry=(ULONG(*)())PlayAhiMemLOnInt;
			else SoundHook.h_Entry=(ULONG(*)())PlayAhiDevLOnInt;
			RecalcLoopPlayData();
		}
		else {
			if(RunTime.plsi->storage==VM_MEMORY) SoundHook.h_Entry=(ULONG(*)())PlayAhiMemLOffInt;
			else SoundHook.h_Entry=(ULONG(*)())PlayAhiDevLOffInt;
			RunTime.plslen=RunTime.plsi->slen;
		}
		audiotags[4].ti_Data=(ULONG)(&SoundHook);
		// DEBUG
		//MSG1("AHIA_AudioID  : 0x%08lx",audiotags[0].ti_Data);
		//MSG1("AHIA_MixFreq  : %5d",audiotags[1].ti_Data);
		//MSG1("AHIA_Channels : %4d",audiotags[2].ti_Data);
		//MSG1("AHIA_Sounds   : %4d",audiotags[3].ti_Data);
		// DEBUG
		if(audioctrl=AHI_AllocAudioA(audiotags)) {
			RunTime.audioctrl=audioctrl;
			AHI_GetAudioAttrs(optp_Ahi.playmode,audioctrl,AHIDB_MaxPlaySamples,&plsmpl,TAG_DONE);
			//plbuflen=(ULONG)((double)plsmpl*(double)optp_Ahi.playrate/(double)RunTime.plsi->srat);
			plbuflen=(ULONG)((double)plsmpl*((double)RunTime.plsi->srat/(double)optp_Ahi.playrate));
			// DEBUG
			MSG1("plbufsmp : %4d",MIN_PLAYBUF_SAMPLES);
			MSG1("plsmpl   : %4d",plsmpl);
			MSG1("plbuflen : %4d",plbuflen);
			// DEBUG
			if(plbuflen<MIN_PLAYBUF_SAMPLES) plbuflen=MIN_PLAYBUF_SAMPLES;
			if(plbuflen<=(MIN_PLAYBUF_SAMPLES<<AHI_SHIFT)) {
				switch(RunTime.plsi->channels) {
					case CH_MONO:
						if(RunTime.plsi->storage==VM_MEMORY) {
							ch0[0].ahisi_Length=RunTime.plsi->slen;
							ch0[0].ahisi_Address=RunTime.plsi->buf[0];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
						}
						else {
							ClearAHIBuffers();
							plsa[0]=SAInit(RunTime.plsi,0,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							SARead=plsa[0]->safunc;
							ch0[0].ahisi_Length=ch1[0].ahisi_Length=plbuflen;
							ch0[0].ahisi_Address=sbuf0[0];ch1[0].ahisi_Address=sbuf1[0];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch1[0],audioctrl);	/* 4 */
							choffs=1;
							//MSG1("blen     : %4d",plsa[0]->blen);
						}
						break;
					case CH_STEREO:
						if(RunTime.plsi->storage==VM_MEMORY) {
							ch0[0].ahisi_Length=ch0[1].ahisi_Length=RunTime.plsi->slen;
							ch0[0].ahisi_Address=RunTime.plsi->buf[0];
							ch0[1].ahisi_Address=RunTime.plsi->buf[1];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch0[1],audioctrl);
						}
						else {
							ClearAHIBuffers();
							plsa[0]=SAInit(RunTime.plsi,0,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							plsa[1]=SAInit(RunTime.plsi,1,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							SARead=plsa[0]->safunc;
							ch0[0].ahisi_Length=ch0[1].ahisi_Length=ch1[0].ahisi_Length=ch1[1].ahisi_Length=plbuflen;
							ch0[0].ahisi_Address=sbuf0[0];ch1[0].ahisi_Address=sbuf1[0];
							ch0[1].ahisi_Address=sbuf0[1];ch1[1].ahisi_Address=sbuf1[1];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch0[1],audioctrl);
							err|=AHI_LoadSound(2,AHIST_DYNAMICSAMPLE,&ch1[0],audioctrl);	/* 4 */
							err|=AHI_LoadSound(3,AHIST_DYNAMICSAMPLE,&ch1[1],audioctrl);	/* 5 */
							choffs=2;
						}
						break;
					case CH_QUADRO:
						if(RunTime.plsi->storage==VM_MEMORY) {
							ch0[0].ahisi_Length=ch0[1].ahisi_Length=ch0[2].ahisi_Length=ch0[3].ahisi_Length=RunTime.plsi->slen;
							ch0[0].ahisi_Address=RunTime.plsi->buf[0];
							ch0[1].ahisi_Address=RunTime.plsi->buf[1];
							ch0[2].ahisi_Address=RunTime.plsi->buf[2];
							ch0[3].ahisi_Address=RunTime.plsi->buf[3];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch0[1],audioctrl);
							err|=AHI_LoadSound(2,AHIST_DYNAMICSAMPLE,&ch0[2],audioctrl);
							err|=AHI_LoadSound(3,AHIST_DYNAMICSAMPLE,&ch0[3],audioctrl);
						}
						else {
							ClearAHIBuffers();
							plsa[0]=SAInit(RunTime.plsi,0,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							plsa[1]=SAInit(RunTime.plsi,1,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							plsa[2]=SAInit(RunTime.plsi,2,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							plsa[3]=SAInit(RunTime.plsi,3,0,RunTime.plsi->slen,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							SARead=plsa[0]->safunc;
							ch0[0].ahisi_Length=ch0[1].ahisi_Length=ch1[0].ahisi_Length=ch1[1].ahisi_Length=
							ch0[2].ahisi_Length=ch0[3].ahisi_Length=ch1[2].ahisi_Length=ch1[3].ahisi_Length=plbuflen;
							ch0[0].ahisi_Address=sbuf0[0];ch1[0].ahisi_Address=sbuf1[0];
							ch0[1].ahisi_Address=sbuf0[1];ch1[1].ahisi_Address=sbuf1[1];
							ch0[2].ahisi_Address=sbuf0[2];ch1[2].ahisi_Address=sbuf1[2];
							ch0[3].ahisi_Address=sbuf0[3];ch1[3].ahisi_Address=sbuf1[3];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch0[1],audioctrl);
							err|=AHI_LoadSound(2,AHIST_DYNAMICSAMPLE,&ch0[2],audioctrl);
							err|=AHI_LoadSound(3,AHIST_DYNAMICSAMPLE,&ch0[3],audioctrl);
							err|=AHI_LoadSound(4,AHIST_DYNAMICSAMPLE,&ch1[0],audioctrl);
							err|=AHI_LoadSound(5,AHIST_DYNAMICSAMPLE,&ch1[1],audioctrl);
							err|=AHI_LoadSound(6,AHIST_DYNAMICSAMPLE,&ch1[2],audioctrl);
							err|=AHI_LoadSound(7,AHIST_DYNAMICSAMPLE,&ch1[3],audioctrl);
							choffs=4;
						}
						break;
				}
				if(err==AHIE_OK) {
					if(AHI_ControlAudio(audioctrl,AHIC_Play,TRUE,TAG_DONE)==AHIE_OK) {
						switch(RunTime.plsi->channels) {
							case CH_MONO:
								AHI_SetVol(0,0x10000L,0x8000L,audioctrl,AHISF_IMM);
								AHI_SetFreq(0,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetSound(0,0,0,plbuflen,audioctrl,AHISF_IMM);
								break;
							case CH_STEREO:
								AHI_SetVol(0,0x10000L,0x00000L,audioctrl,AHISF_IMM);
								AHI_SetVol(1,0x10000L,0x10000L,audioctrl,AHISF_IMM);
								AHI_SetFreq(0,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetFreq(1,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetSound(0,0,0,plbuflen,audioctrl,AHISF_IMM);
								AHI_SetSound(1,1,0,plbuflen,audioctrl,AHISF_IMM);
								break;
							case CH_QUADRO:
								AHI_SetVol(0,0x10000L,   0x00000L,audioctrl,AHISF_IMM);		// left			realtime surround, yeah
								AHI_SetVol(1,0x10000L,   0x10000L,audioctrl,AHISF_IMM);		// right
								AHI_SetVol(2,0x10000L,   0x08000L,audioctrl,AHISF_IMM);		// center
								AHI_SetVol(3,0x10000L,0xFFFF8000L,audioctrl,AHISF_IMM);		// rear (just invert)
								AHI_SetFreq(0,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetFreq(1,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetFreq(2,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetFreq(3,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetSound(0,0,0,plbuflen,audioctrl,AHISF_IMM);
								AHI_SetSound(1,1,0,plbuflen,audioctrl,AHISF_IMM);
								AHI_SetSound(2,2,0,plbuflen,audioctrl,AHISF_IMM);
								AHI_SetSound(3,3,0,plbuflen,audioctrl,AHISF_IMM);
								break;
						}
					}
					else { fail=TRUE;sprintf(sfxmsg,GetString(errFail),"AHI/ControlAudio");Message(errFail,sfxmsg,__FILE__,__LINE__); }
				}
				else { fail=TRUE;sprintf(sfxmsg,GetString(errFail),"AHI/LoadSound");Message(errFail,sfxmsg,__FILE__,__LINE__); }
			}
			else {
				char infomsg[200];
				fail=TRUE;
				sprintf(infomsg,"AHI/GetAudioAttrs (playbuffer too large %ld/%ld)",plbuflen,(MIN_PLAYBUF_SAMPLES<<AHI_SHIFT));
				sprintf(sfxmsg,GetString(errFail),infomsg);Message(errFail,sfxmsg,__FILE__,__LINE__);
			}
		}
		else Message(errAllocateAudio,NULL,__FILE__,__LINE__);
	}
	else Message(errNoAHI,NULL,__FILE__,__LINE__);
	if(fail) StopAhiASample();
}

void PlayAhiASample_Rng(void) {
	ULONG plsmpl;
	ULONG err=AHIE_OK;
	UBYTE fail=FALSE;
	struct TagItem audiotags[]={
		AHIA_AudioID,	0,
		AHIA_MixFreq,	0,
		AHIA_Channels,	0,
		AHIA_Sounds,	0,
		AHIA_SoundFunc,	NULL,
		TAG_DONE
	};

	if(AHIBase && optp_Ahi.playmode!=AHI_INVALID_ID) {
		SetPlayRange();
		plct=0;off=-1;
		RunTime.oldptr=0;RunTime.play=PLAY_RNG;RunTime.plstop=0;
		DrawPPos(RunTime.plsi,RunTime.oldptr);	 /* Draw Pos */

		audiotags[0].ti_Data=optp_Ahi.playmode;
		audiotags[1].ti_Data=(ULONG)optp_Ahi.playrate;
		audiotags[2].ti_Data=(ULONG)(RunTime.plsi->channels+1);
		if(RunTime.plsi->storage==VM_MEMORY) {
			audiotags[3].ti_Data=(ULONG)(RunTime.plsi->channels+1);
			SoundHook.h_Entry=(ULONG(*)())PlayAhiMemLOffInt;
		}
		else {
			audiotags[3].ti_Data=(ULONG)((RunTime.plsi->channels+1)<<1);
			SoundHook.h_Entry=(ULONG(*)())PlayAhiDevLOffInt;
		}
		audiotags[4].ti_Data=(ULONG)(&SoundHook);
		if(audioctrl=AHI_AllocAudioA(audiotags)) {
			RunTime.audioctrl=audioctrl;
			AHI_GetAudioAttrs(optp_Ahi.playmode,audioctrl,AHIDB_MaxPlaySamples,&plsmpl,TAG_DONE);
			//plbuflen=(ULONG)((double)plsmpl*(double)optp_Ahi.playrate/(double)RunTime.plsi->srat);
			plbuflen=(ULONG)((double)plsmpl*((double)RunTime.plsi->srat/(double)optp_Ahi.playrate));
			if(plbuflen<MIN_PLAYBUF_SAMPLES) plbuflen=MIN_PLAYBUF_SAMPLES;
			if(plbuflen<=(MIN_PLAYBUF_SAMPLES<<AHI_SHIFT)) {
				switch(RunTime.plsi->channels) {
					case CH_MONO:
						if(RunTime.plsi->storage==VM_MEMORY) {
							ch0[0].ahisi_Length=RunTime.plsi->slen;
							ch0[0].ahisi_Address=RunTime.plsi->buf[0];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
						}
						else {
							ClearAHIBuffers();
							plsa[0]=SAInit(RunTime.plsi,0,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							SARead=plsa[0]->safunc;
							ch0[0].ahisi_Length=ch1[0].ahisi_Length=plbuflen;
							ch0[0].ahisi_Address=sbuf0[0];ch1[0].ahisi_Address=sbuf1[0];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch1[0],audioctrl);
							choffs=1;
						}
						break;
					case CH_STEREO:
						if(RunTime.plsi->storage==VM_MEMORY) {
							ch0[0].ahisi_Length=ch0[1].ahisi_Length=RunTime.plsi->slen;
							ch0[0].ahisi_Address=RunTime.plsi->buf[0];
							ch0[1].ahisi_Address=RunTime.plsi->buf[1];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch0[1],audioctrl);
						}
						else {
							ClearAHIBuffers();
							plsa[0]=SAInit(RunTime.plsi,0,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							plsa[1]=SAInit(RunTime.plsi,1,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							SARead=plsa[0]->safunc;
							ch0[0].ahisi_Length=ch0[1].ahisi_Length=ch1[0].ahisi_Length=ch1[1].ahisi_Length=plbuflen;
							ch0[0].ahisi_Address=sbuf0[0];ch1[0].ahisi_Address=sbuf1[0];
							ch0[1].ahisi_Address=sbuf0[1];ch1[1].ahisi_Address=sbuf1[1];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch0[1],audioctrl);
							err|=AHI_LoadSound(2,AHIST_DYNAMICSAMPLE,&ch1[0],audioctrl);
							err|=AHI_LoadSound(3,AHIST_DYNAMICSAMPLE,&ch1[1],audioctrl);
							choffs=2;
						}
						break;
					case CH_QUADRO:
						if(RunTime.plsi->storage==VM_MEMORY) {
							ch0[0].ahisi_Length=ch0[1].ahisi_Length=ch0[2].ahisi_Length=ch0[3].ahisi_Length=RunTime.plsi->slen;
							ch0[0].ahisi_Address=RunTime.plsi->buf[0];
							ch0[1].ahisi_Address=RunTime.plsi->buf[1];
							ch0[2].ahisi_Address=RunTime.plsi->buf[2];
							ch0[3].ahisi_Address=RunTime.plsi->buf[3];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch0[1],audioctrl);
							err|=AHI_LoadSound(2,AHIST_DYNAMICSAMPLE,&ch0[2],audioctrl);
							err|=AHI_LoadSound(3,AHIST_DYNAMICSAMPLE,&ch0[3],audioctrl);
						}
						else {
							ClearAHIBuffers();
							plsa[0]=SAInit(RunTime.plsi,0,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							plsa[1]=SAInit(RunTime.plsi,1,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							plsa[2]=SAInit(RunTime.plsi,2,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							plsa[3]=SAInit(RunTime.plsi,3,RunTime.plptr,(RunTime.plslen-RunTime.plptr),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK|VM_MODE_IRQ,Samples2Bytes(plbuflen));	  // sequentiell Blöcke konstanter Größe lesen
							SARead=plsa[0]->safunc;
							ch0[0].ahisi_Length=ch0[1].ahisi_Length=ch1[0].ahisi_Length=ch1[1].ahisi_Length=
							ch0[2].ahisi_Length=ch0[3].ahisi_Length=ch1[2].ahisi_Length=ch1[3].ahisi_Length=plbuflen;
							ch0[0].ahisi_Address=sbuf0[0];ch1[0].ahisi_Address=sbuf1[0];
							ch0[1].ahisi_Address=sbuf0[1];ch1[1].ahisi_Address=sbuf1[1];
							ch0[2].ahisi_Address=sbuf0[2];ch1[2].ahisi_Address=sbuf1[2];
							ch0[3].ahisi_Address=sbuf0[3];ch1[3].ahisi_Address=sbuf1[3];
							err|=AHI_LoadSound(0,AHIST_DYNAMICSAMPLE,&ch0[0],audioctrl);
							err|=AHI_LoadSound(1,AHIST_DYNAMICSAMPLE,&ch0[1],audioctrl);
							err|=AHI_LoadSound(2,AHIST_DYNAMICSAMPLE,&ch0[2],audioctrl);
							err|=AHI_LoadSound(3,AHIST_DYNAMICSAMPLE,&ch0[3],audioctrl);
							err|=AHI_LoadSound(4,AHIST_DYNAMICSAMPLE,&ch1[0],audioctrl);
							err|=AHI_LoadSound(5,AHIST_DYNAMICSAMPLE,&ch1[1],audioctrl);
							err|=AHI_LoadSound(6,AHIST_DYNAMICSAMPLE,&ch1[2],audioctrl);
							err|=AHI_LoadSound(7,AHIST_DYNAMICSAMPLE,&ch1[3],audioctrl);
							choffs=4;
						}
						break;
				}
				if(err==AHIE_OK) {
					if(AHI_ControlAudio(audioctrl,AHIC_Play,TRUE,TAG_DONE)==AHIE_OK) {
						switch(RunTime.plsi->channels) {
							case CH_MONO:
								AHI_SetVol(0,0x10000L,0x8000L,audioctrl,AHISF_IMM);
								AHI_SetFreq(0,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								if(RunTime.plsi->storage==VM_MEMORY) {
									AHI_SetSound(0,0,RunTime.plptr,plbuflen,audioctrl,AHISF_IMM);
								}
								else {
									AHI_SetSound(0,0,0,plbuflen,audioctrl,AHISF_IMM);
								}
								break;
							case CH_STEREO:
								AHI_SetVol(0,0x10000L,0x00000L,audioctrl,AHISF_IMM);
								AHI_SetVol(1,0x10000L,0x10000L,audioctrl,AHISF_IMM);
								AHI_SetFreq(0,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetFreq(1,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								if(RunTime.plsi->storage==VM_MEMORY) {
									AHI_SetSound(0,0,RunTime.plptr,plbuflen,audioctrl,AHISF_IMM);
									AHI_SetSound(1,1,RunTime.plptr,plbuflen,audioctrl,AHISF_IMM);
								}
								else {
									AHI_SetSound(0,0,0,plbuflen,audioctrl,AHISF_IMM);
									AHI_SetSound(1,1,0,plbuflen,audioctrl,AHISF_IMM);
								}
								break;
							case CH_QUADRO:
								AHI_SetVol(0,0x10000L,0x00000L,audioctrl,AHISF_IMM);
								AHI_SetVol(1,0x10000L,0x10000L,audioctrl,AHISF_IMM);
								AHI_SetVol(2,0x10000L,0x00000L,audioctrl,AHISF_IMM);
								AHI_SetVol(3,0x10000L,0x10000L,audioctrl,AHISF_IMM);
								AHI_SetFreq(0,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetFreq(1,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetFreq(2,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								AHI_SetFreq(3,RunTime.plsi->srat,audioctrl,AHISF_IMM);
								if(RunTime.plsi->storage==VM_MEMORY) {
									AHI_SetSound(0,0,RunTime.plptr,plbuflen,audioctrl,AHISF_IMM);
									AHI_SetSound(1,1,RunTime.plptr,plbuflen,audioctrl,AHISF_IMM);
									AHI_SetSound(2,2,RunTime.plptr,plbuflen,audioctrl,AHISF_IMM);
									AHI_SetSound(3,3,RunTime.plptr,plbuflen,audioctrl,AHISF_IMM);
								}
								else {
									AHI_SetSound(0,0,0,plbuflen,audioctrl,AHISF_IMM);
									AHI_SetSound(1,1,0,plbuflen,audioctrl,AHISF_IMM);
									AHI_SetSound(2,2,0,plbuflen,audioctrl,AHISF_IMM);
									AHI_SetSound(3,3,0,plbuflen,audioctrl,AHISF_IMM);
								}
								break;
						}
					}
					else { fail=TRUE;sprintf(sfxmsg,GetString(errFail),"AHI/ControlAudio");Message(errFail,sfxmsg,__FILE__,__LINE__); }
				}
				else {fail=TRUE;sprintf(sfxmsg,GetString(errFail),"AHI/LoadSound");Message(errFail,sfxmsg,__FILE__,__LINE__); }
			}
			else {
				char infomsg[200];
				fail=TRUE;
				sprintf(infomsg,"AHI/GetAudioAttrs (playbuffer too large %ld/%ld)",plbuflen,(MIN_PLAYBUF_SAMPLES<<AHI_SHIFT));
				sprintf(sfxmsg,GetString(errFail),infomsg);Message(errFail,sfxmsg,__FILE__,__LINE__);
			}
		}
		else Message(errAllocateAudio,NULL,__FILE__,__LINE__);
	}
	else Message(errNoAHI,NULL,__FILE__,__LINE__);
	if(fail) StopAhiASample();
}

void StopAhiASample(void) {
	RunTime.play=PLAY_OFF;
	//MSG("StopAhiSample begin");
	if(audioctrl) {
		DrawPPos(RunTime.plsi,RunTime.oldptr);					 /* Delete Pos */
		//MSG("after deleting PlayPosBar");
		if(RunTime.plsi->storage==VM_MEMORY) {
			switch(RunTime.plsi->channels) {
				case CH_MONO:
					AHI_SetSound(0,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_ControlAudio(audioctrl,AHIC_Play,FALSE,TAG_DONE);
					AHI_UnloadSound(0,audioctrl);
					break;
				case CH_STEREO:
					AHI_SetSound(0,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(1,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_ControlAudio(audioctrl,AHIC_Play,FALSE,TAG_DONE);
					AHI_UnloadSound(0,audioctrl);AHI_UnloadSound(1,audioctrl);
					break;
				case CH_QUADRO:
					AHI_SetSound(0,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(1,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(2,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(3,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_ControlAudio(audioctrl,AHIC_Play,FALSE,TAG_DONE);
					AHI_UnloadSound(0,audioctrl);AHI_UnloadSound(1,audioctrl);
					AHI_UnloadSound(2,audioctrl);AHI_UnloadSound(3,audioctrl);
					break;
			}
		}
		else {
			switch(RunTime.plsi->channels) {
				case CH_MONO:
					AHI_SetSound(0,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(1,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);	/* 4 */
					AHI_ControlAudio(audioctrl,AHIC_Play,FALSE,TAG_DONE);
					AHI_UnloadSound(0,audioctrl);
					AHI_UnloadSound(1,audioctrl);								/* 4 */
					SADone(plsa[0],RunTime.plsi);plsa[0]=NULL;
					break;
				case CH_STEREO:
					AHI_SetSound(0,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(1,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(2,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);	/* 4 */
					AHI_SetSound(3,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);	/* 5 */
					AHI_ControlAudio(audioctrl,AHIC_Play,FALSE,TAG_DONE);
					AHI_UnloadSound(0,audioctrl);AHI_UnloadSound(1,audioctrl);
					AHI_UnloadSound(2,audioctrl);AHI_UnloadSound(3,audioctrl);	/* 4,5 */
					SADone(plsa[0],RunTime.plsi);plsa[0]=NULL;
					SADone(plsa[1],RunTime.plsi);plsa[1]=NULL;
					break;
				case CH_QUADRO:
					AHI_SetSound(0,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(1,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(2,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(3,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(4,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(5,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(6,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_SetSound(7,(UWORD)AHI_NOSOUND,0,0,audioctrl,AHISF_IMM);
					AHI_ControlAudio(audioctrl,AHIC_Play,FALSE,TAG_DONE);
					AHI_UnloadSound(0,audioctrl);AHI_UnloadSound(1,audioctrl);
					AHI_UnloadSound(2,audioctrl);AHI_UnloadSound(3,audioctrl);
					AHI_UnloadSound(4,audioctrl);AHI_UnloadSound(5,audioctrl);
					AHI_UnloadSound(6,audioctrl);AHI_UnloadSound(7,audioctrl);
					SADone(plsa[0],RunTime.plsi);plsa[0]=NULL;
					SADone(plsa[1],RunTime.plsi);plsa[1]=NULL;
					SADone(plsa[2],RunTime.plsi);plsa[2]=NULL;
					SADone(plsa[3],RunTime.plsi);plsa[3]=NULL;
					break;
			}
		}
		//MSG("after switch block");
		AHI_FreeAudio(audioctrl);RunTime.audioctrl=audioctrl=NULL;
		//MSG("after FreeAudio");
		ModifyIDCMP(RunTime.winBackDrop,IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_MENUPICK|IDCMP_MENUVERIFY);
		if(SFX_SOUND_IS_SHOWN(RunTime.plsi)) SetWindowTitles(RunTime.plsi->win,RunTime.plsi->title,(UBYTE *)-1);
	}
	//MSG("StopAhiSample end");
}

UBYTE InitAhiAPlayer(void) {
	if((AHImp=CreateMsgPort())
	&& (AHIio=(struct AHIRequest *)CreateIORequest(AHImp,sizeof(struct AHIRequest)))) { 
		AHIio->ahir_Version=2;
		if(!(AHIDevice=OpenDevice(AHINAME,AHI_NO_UNIT,(struct IORequest *)AHIio,NULL))
		&& (sbuf0[0]=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES<<AHI_SHIFT,MEMF_PUBLIC|MEMF_CLEAR))
		&& (sbuf0[1]=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES<<AHI_SHIFT,MEMF_PUBLIC|MEMF_CLEAR))
		&& (sbuf0[2]=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES<<AHI_SHIFT,MEMF_PUBLIC|MEMF_CLEAR))
		&& (sbuf0[3]=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES<<AHI_SHIFT,MEMF_PUBLIC|MEMF_CLEAR))
		&& (sbuf1[0]=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES<<AHI_SHIFT,MEMF_PUBLIC|MEMF_CLEAR))
		&& (sbuf1[1]=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES<<AHI_SHIFT,MEMF_PUBLIC|MEMF_CLEAR))
		&& (sbuf1[2]=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES<<AHI_SHIFT,MEMF_PUBLIC|MEMF_CLEAR))
		&& (sbuf1[3]=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES<<AHI_SHIFT,MEMF_PUBLIC|MEMF_CLEAR))) {
			AHIBase=(struct Library *)AHIio->ahir_Std.io_Device;
			RunTime.AHIBase=AHIBase;
			ch0[0].ahisi_Type=ch0[1].ahisi_Type=ch0[2].ahisi_Type=ch0[3].ahisi_Type=
			ch1[0].ahisi_Type=ch1[1].ahisi_Type=ch1[2].ahisi_Type=ch1[3].ahisi_Type=AHIST_M16S;
			return(TRUE);
		}
	}
	// disable options gui
	((ModNode *)GetNodeByIndex(&PlayerList,RunTime.aplayer))->info.gui=FALSE;
	return(FALSE);
}

void FreeAhiAPlayer(void) {
	if(!AHIDevice)	{ CloseDevice((struct IORequest *)AHIio);AHIDevice=-1; }
	if(AHIio)		{ DeleteIORequest((struct IORequest *)AHIio);AHIio=NULL; }
	if(AHImp)		{ DeleteMsgPort(AHImp);AHImp=NULL; }
	if(sbuf0[0])	{ FreeVec(sbuf0[0]);sbuf0[0]=NULL; }
	if(sbuf0[1])	{ FreeVec(sbuf0[1]);sbuf0[1]=NULL; }
	if(sbuf0[2])	{ FreeVec(sbuf0[2]);sbuf0[2]=NULL; }
	if(sbuf0[3])	{ FreeVec(sbuf0[3]);sbuf0[3]=NULL; }
	if(sbuf1[0])	{ FreeVec(sbuf1[0]);sbuf1[0]=NULL; }
	if(sbuf1[1])	{ FreeVec(sbuf1[1]);sbuf1[1]=NULL; }
	if(sbuf1[2])	{ FreeVec(sbuf1[2]);sbuf1[2]=NULL; }
	if(sbuf1[3])	{ FreeVec(sbuf1[3]);sbuf1[3]=NULL; }
}

void ClearAHIBuffers(void) {
	memset(sbuf0[0],0,MIN_PLAYBUF_BYTES<<AHI_SHIFT);
	memset(sbuf0[1],0,MIN_PLAYBUF_BYTES<<AHI_SHIFT);
	memset(sbuf0[2],0,MIN_PLAYBUF_BYTES<<AHI_SHIFT);
	memset(sbuf0[3],0,MIN_PLAYBUF_BYTES<<AHI_SHIFT);
	memset(sbuf1[0],0,MIN_PLAYBUF_BYTES<<AHI_SHIFT);
	memset(sbuf1[1],0,MIN_PLAYBUF_BYTES<<AHI_SHIFT);
	memset(sbuf1[2],0,MIN_PLAYBUF_BYTES<<AHI_SHIFT);
	memset(sbuf1[3],0,MIN_PLAYBUF_BYTES<<AHI_SHIFT);
}

__asm __interrupt __saveds ULONG PlayAhiMemLOffInt(register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHISoundMessage *chan) {
	if(!chan->ahism_Channel) RunTime.plptr+=plbuflen;		// do this just once
	if(RunTime.plptr<RunTime.plslen) {
		if(RunTime.plptr+plbuflen<RunTime.plslen)
			AHI_SetSound(chan->ahism_Channel,chan->ahism_Channel,RunTime.plptr,plbuflen,actrl,NULL);
		else
			AHI_SetSound(chan->ahism_Channel,chan->ahism_Channel,RunTime.plptr,(RunTime.plslen-RunTime.plptr),actrl,NULL);
	}
	else {
		AHI_SetSound(chan->ahism_Channel,AHI_NOSOUND,0,0,actrl,NULL);
		RunTime.plstop++;
	}
	return(NULL);
}

__asm __interrupt __saveds ULONG PlayAhiMemLOnInt(register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHISoundMessage *chan) {
	if(!chan->ahism_Channel) {
		RunTime.plptr+=plbuflen;
		if(RunTime.plptr>=RunTime.plslen) RunTime.plptr=RunTime.pllstart;
	}
	if(RunTime.plptr+plbuflen<RunTime.plslen)
		AHI_SetSound(chan->ahism_Channel,chan->ahism_Channel,RunTime.plptr,plbuflen,actrl,NULL);
	else
		AHI_SetSound(chan->ahism_Channel,chan->ahism_Channel,RunTime.plptr,(RunTime.plslen-RunTime.plptr),actrl,NULL);
	return(NULL);
}

__asm __interrupt __saveds ULONG PlayAhiDevLOffInt(register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHISoundMessage *chan) {
	if(!chan->ahism_Channel) RunTime.plptr+=plbuflen;		// do this just once
	if(RunTime.plptr<RunTime.plslen) {
		plsa0=plsa[chan->ahism_Channel];
		if(plsa0->state!=STATE_WORKING) {
			plbuf0=(UWORD *)plsa0->buffer1;SARead(plsa0,plsa0->blen);
			if(plct) {
				memcpy(sbuf0[chan->ahism_Channel],plbuf0,plsa0->blen);
				AHI_SetSound(chan->ahism_Channel,chan->ahism_Channel,0,0,actrl,NULL);
			}
			else {
				memcpy(sbuf1[chan->ahism_Channel],plbuf0,plsa0->blen);
				AHI_SetSound(chan->ahism_Channel,choffs+chan->ahism_Channel,0,0,actrl,NULL);
			}
			plct^=1;
		}
	}
	else {
		AHI_SetSound(chan->ahism_Channel,AHI_NOSOUND,0,0,actrl,NULL);
		RunTime.plstop++;
	}
	return(NULL);
}

__asm __interrupt __saveds ULONG PlayAhiDevLOnInt(register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHISoundMessage *chan) {
	if(!chan->ahism_Channel) {
		RunTime.plptr+=plbuflen;
		off=(LONG)RunTime.plptr-(LONG)RunTime.plslen;
		if(off>=0) RunTime.plptr=RunTime.pllstart;
	}
	plsa0=plsa[chan->ahism_Channel];
	if(plsa0->state!=STATE_WORKING) {
		if(off>=0) plsa0->seek=Samples2Bytes(RunTime.plsi->loops+off);
		plbuf0=(UWORD *)plsa0->buffer1;SARead(plsa0,plsa0->blen);
		if(plct) {
			if(off<0) memcpy(sbuf0[chan->ahism_Channel],plbuf0,plsa0->blen);
			else {
				memcpy( sbuf0[chan->ahism_Channel],plbuf0,Samples2Bytes(off));
				memcpy(&sbuf0[chan->ahism_Channel][off],plsa0->buffer3,(plsa0->blen-Samples2Bytes(off)));
			}
			AHI_SetSound(chan->ahism_Channel,chan->ahism_Channel,0,0,actrl,NULL);
		}
		else {
			if(off<0) memcpy(sbuf1[chan->ahism_Channel],plbuf0,plsa0->blen);
			else {
				memcpy( sbuf1[chan->ahism_Channel],plbuf0,off);
				memcpy(&sbuf1[chan->ahism_Channel][off],plsa0->buffer3,(plsa0->blen-off));
			}
			AHI_SetSound(chan->ahism_Channel,choffs+chan->ahism_Channel,0,0,actrl,NULL);
		}
		plct^=1;
	}
	return(NULL);
}

//-- eof ----------------------------------------------------------------------
