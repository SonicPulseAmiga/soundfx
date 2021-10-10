/****h* SoundFX/SFX-Utilities.c [4.2] *
*
*  NAME
*    SFX-Utilities.c
*  COPYRIGHT
*    $VER: SFX-Utilities.c 4.2 (30.10.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    utility functions - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    04.Feb.2002
*  MODIFICATION HISTORY
*    30.Oct.2002	V 4.2	most recent version
*    04.Feb.2002	V 4.2   initial version
*  NOTES
*
*******
*/

#define SFX_Utilities_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void UtilitiesSwapEndian(SInfo *entry);
void UtilitiesSwapSign(SInfo *entry);
void UtilitiesInterleave(SInfo *entry);
void UtilitiesDeInterleave(SInfo *entry);

//-- definitions --------------------------------------------------------------

void UtilitiesSwapEndian(SInfo *entry)
{
	UBYTE l;
	register ULONG j;
	SmpAccess *srcsa,*dstsa;
	SAFuncPtr SARead,SAWrite;
	register SAMPLE *sbuf,*dbuf;
	ULONG curlen,blen;
	char titlestr[256];

	if(RunTime.play && entry==RunTime.plsi) StopSample();
	if(AllocSampleMem(&temp,entry->slen,CH_COUNT(entry->channels))) {
		temp.channels=entry->channels;

		if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: swapping endian ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
		for(l=0;l<CH_COUNT(entry->channels);l++) {
			srcsa=SAInit(entry,l,0,entry->slen,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	 // sequentiell Blöcke konstanter Größe lesen
			dstsa=SAInit(&temp,l,0,entry->slen,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	 // sequentiell Blöcke konstanter Größe schreiben
			if(srcsa && dstsa) {
				SARead =srcsa->safunc;
				SAWrite=dstsa->safunc;
				blen=Bytes2Samples(srcsa->blen);
				curlen=0;
				for(srcsa->seg=dstsa->seg=0;curlen<entry->slen;srcsa->seg++,dstsa->seg++) {
					sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
					dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
					j=0;
					while((j<blen) && (curlen<entry->slen)) {		 // die Blocklänge durchgehen
						*(dbuf++)=(WORD)(((((WORD)(*sbuf))&0x00FF)<<8)|((((WORD)(*sbuf))&0xFF00)>>8));
						sbuf++;
						curlen++;j++;
					}
					SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
				}
			}
			if(srcsa) SADone(srcsa,entry);
			if(dstsa) SADone(dstsa,&temp);
		}

		ExchangeData(&temp,entry);
		//-- recalc lines
		entry->lines=FALSE;RecalcSampleLines(entry);
		DrawSample(entry,0);
		if(SFX_SOUND_IS_SHOWN(entry)) SetWindowTitles(entry->win,entry->title,(UBYTE *)-1);
	}
	else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
}

void UtilitiesSwapSign(SInfo *entry)
{
	UBYTE l;
	register ULONG j;
	SmpAccess *srcsa,*dstsa;
	SAFuncPtr SARead,SAWrite;
	register SAMPLE *sbuf,*dbuf;
	ULONG curlen,blen;
	char titlestr[256];

	if(RunTime.play && entry==RunTime.plsi) StopSample();
	if(AllocSampleMem(&temp,entry->slen,CH_COUNT(entry->channels))) {
		temp.channels=entry->channels;

		if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: swapping sign ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
		for(l=0;l<CH_COUNT(entry->channels);l++) {
			srcsa=SAInit(entry,l,0,entry->slen,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	 // sequentiell Blöcke konstanter Größe lesen
			dstsa=SAInit(&temp,l,0,entry->slen,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	 // sequentiell Blöcke konstanter Größe schreiben
			if(srcsa && dstsa) {
				SARead =srcsa->safunc;
				SAWrite=dstsa->safunc;
				blen=Bytes2Samples(srcsa->blen);
				curlen=0;
				for(srcsa->seg=dstsa->seg=0;curlen<entry->slen;srcsa->seg++,dstsa->seg++) {
					sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
					dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
					j=0;
					while((j<blen) && (curlen<entry->slen)) {		 // die Blocklänge durchgehen
						*(dbuf++)=(WORD)((*sbuf)+SMP_MAXN);
						sbuf++;
						curlen++;j++;
					}
					SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
				}
			}
			if(srcsa) SADone(srcsa,entry);
			if(dstsa) SADone(dstsa,&temp);
		}

		ExchangeData(&temp,entry);
		//-- recalc lines
		entry->lines=FALSE;RecalcSampleLines(entry);
		DrawSample(entry,0);
		if(SFX_SOUND_IS_SHOWN(entry)) SetWindowTitles(entry->win,entry->title,(UBYTE *)-1);
	}
	else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
}

void UtilitiesInterleave(SInfo *entry)
{
	UBYTE l;
	register ULONG j;
	SmpAccess *srcsa1,*srcsa2,*dstsa;
	SAFuncPtr SARead,SAWrite;
	register SAMPLE *sbuf1,*sbuf2,*dbuf;
	ULONG curlen,blen;
	char titlestr[256];

	if(entry->channels<CH_STEREO) return;

	if(RunTime.play && entry==RunTime.plsi) StopSample();
	temp.channels=CH_HALF(entry->channels);
	temp.slen=entry->slen<<1;
	if(AllocSampleMem(&temp,temp.slen,CH_COUNT(temp.channels))) {

		if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: interleaving ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
		//-- itereate over dest channels, merge two sources each time
		for(l=0;l<CH_COUNT(temp.channels);l++) {
			srcsa1=SAInit(entry,(l<<1)  ,0,entry->slen,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	 // sequentiell Blöcke konstanter Größe lesen
			srcsa2=SAInit(entry,(l<<1)+1,0,entry->slen,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);    // sequentiell Blöcke konstanter Größe lesen
			dstsa =SAInit(&temp, l      ,0,temp.slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	 // sequentiell Blöcke konstanter Größe schreiben
			if(srcsa1 && srcsa2 && dstsa) {
				SARead =srcsa1->safunc;
				SAWrite=dstsa->safunc;
				blen=Bytes2Samples(dstsa->blen);
				curlen=0;
				for(srcsa1->seg=srcsa2->seg=dstsa->seg=0;curlen<entry->slen;srcsa1->seg++,srcsa2->seg++) {
					sbuf1=(SAMPLE *)srcsa1->buffer1;SARead(srcsa1,srcsa1->blen);	// Pointer auf src-Block holen & nächstes Read starten
					sbuf2=(SAMPLE *)srcsa2->buffer2;SARead(srcsa2,srcsa2->blen);	// Pointer auf src-Block holen & nächstes Read starten
					dbuf=(SAMPLE *)dstsa->buffer1;									// Pointer auf dst-Block
					j=0;
					while((j<blen) && (curlen<entry->slen)) {		 // die Blocklänge durchgehen
						*(dbuf++)=*(sbuf1++);
						*(dbuf++)=*(sbuf2++);
						curlen++;j+=2;
					}
					SAWrite(dstsa,dstsa->blen);										// akt Block schreiben
					dstsa->seg++;
					dbuf=(SAMPLE *)dstsa->buffer1;									// Pointer auf dst-Block
					j=0;
					while((j<blen) && (curlen<entry->slen)) {		 // die Blocklänge durchgehen
						*(dbuf++)=*(sbuf1++);
						*(dbuf++)=*(sbuf2++);
						curlen++;j+=2;
					}
					SAWrite(dstsa,dstsa->blen);										// akt Block schreiben
					dstsa->seg++;
				}
			}
			if(srcsa1) SADone(srcsa1,entry);
			if(srcsa2) SADone(srcsa2,entry);
			if(dstsa) SADone(dstsa,&temp);
		}

		ExchangeData(&temp,entry);
		entry->loops <<=1;entry->loopl <<=1;
		entry->markxs<<=1;entry->markxl<<=1;
		entry->zoomxs<<=1;entry->zoomxl<<=1;
		entry->slen=temp.slen;
		entry->channels=temp.channels;
		entry->channelmask=CH_MASK(entry->channels);
		entry->aktch=CH_COUNT(entry->channels);
		SetTitle(entry);Refr_GlobalMenu();
		//-- recalc lines
		entry->lines=FALSE;RecalcSampleLines(entry);
		DrawSample(entry,0);
	}
	else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
}

void UtilitiesDeInterleave(SInfo *entry)
{
	UBYTE l;
	register ULONG j;
	SmpAccess *srcsa,*dstsa1,*dstsa2;
	SAFuncPtr SARead,SAWrite;
	register SAMPLE *sbuf,*dbuf1,*dbuf2;
	ULONG curlen,blen;
	char titlestr[256];

	if(entry->channels>CH_STEREO) return;

	if(RunTime.play && entry==RunTime.plsi) StopSample();
	temp.channels=CH_DOUBLE(entry->channels);
	temp.slen=entry->slen>>1;
	if(AllocSampleMem(&temp,temp.slen,CH_COUNT(temp.channels))) {

		if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: deinterleaving ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
		//-- itereate over source channels, split into two destinations each time
		for(l=0;l<CH_COUNT(entry->channels);l++) {
			srcsa =SAInit(entry, l      ,0,entry->slen,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	 // sequentiell Blöcke konstanter Größe lesen
			dstsa1=SAInit(&temp,(l<<1)  ,0,temp.slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	 // sequentiell Blöcke konstanter Größe schreiben
			dstsa2=SAInit(&temp,(l<<1)+1,0,temp.slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	 // sequentiell Blöcke konstanter Größe schreiben
			if(srcsa && dstsa1 && dstsa2) {
				SARead =srcsa->safunc;
				SAWrite=dstsa1->safunc;
				blen=Bytes2Samples(srcsa->blen);
				curlen=0;
				for(srcsa->seg=dstsa1->seg=dstsa2->seg=0;curlen<temp.slen;dstsa1->seg++,dstsa2->seg++) {
					dbuf1=(SAMPLE *)dstsa1->buffer1;								// Pointer auf dst-Block
					dbuf2=(SAMPLE *)dstsa2->buffer1;								// Pointer auf dst-Block
					sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);		// Pointer auf src-Block holen & nächstes Read starten
					srcsa->seg++;
					j=0;
					while((j<blen) && (curlen<temp.slen)) {			// die Blocklänge durchgehen
						*(dbuf1++)=*(sbuf++);
						*(dbuf2++)=*(sbuf++);
						curlen++;j+=2;
					}
					sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);		// Pointer auf src-Block holen & nächstes Read starten
					srcsa->seg++;
					j=0;
					while((j<blen) && (curlen<temp.slen)) {			// die Blocklänge durchgehen
						*(dbuf1++)=*(sbuf++);
						*(dbuf2++)=*(sbuf++);
						curlen++;j+=2;
					}
					SAWrite(dstsa1,dstsa1->blen);									// akt Block schreiben
					SAWrite(dstsa2,dstsa2->blen);									// akt Block schreiben
				}
			}
			if(srcsa) SADone(srcsa,entry);
			if(dstsa1) SADone(dstsa1,&temp);
			if(dstsa2) SADone(dstsa2,&temp);
		}
		ExchangeData(&temp,entry);
		entry->loops >>=1;entry->loopl >>=1;
		entry->markxs>>=1;entry->markxl>>=1;
		entry->zoomxs>>=1;entry->zoomxl>>=1;
		entry->slen=temp.slen;
		entry->channels=temp.channels;
		entry->channelmask=CH_MASK(entry->channels);
		entry->aktch=CH_COUNT(entry->channels);
		SetTitle(entry);Refr_GlobalMenu();
		//-- recalc lines
		entry->lines=FALSE;RecalcSampleLines(entry);
		DrawSample(entry,0);
	}
	else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
}

//-- eof ----------------------------------------------------------------------


