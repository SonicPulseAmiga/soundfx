/****h* sfxsupport.library/savt.c [4.3] *
*
*  NAME
*    savt.c
*  COPYRIGHT
*    $VER: savt.c 4.3 (05.02.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Helper for Savers - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    15.Jul.1998
*  MODIFICATION HISTORY
*    05.Feb.2004	V 4.3	most recent version
*                           C: endian converion is done after decrunching (to allow IEEE formats)
*    12.Sep.2002	V 4.2	most recent version
*                           C: refactorized WriteData()
*                           A: error checking in WriteData()
*    26.Jan.2001	V 4.1	most recent version
*							C: now uses asynchronous ProWin
*    18.Jun.2000	V 1.3	most recent version
*    19.Sep.1998	V 1.2	most recent version
*    15.Jul.1998	V 1.1	initial version
*  NOTES
*
*******
*/

#define SAVT_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"
#include "project:sfx-main/sfx-filedefs.h"

typedef struct {
	WORD *dbuf1,*sbuf;
	BYTE *dbuf,*dbuf2[CH_MAX];
	ULONG curlen;					// blocs
	ULONG src_hdrbytes ,dst_hdrbytes ,hdrsamples ;
	ULONG src_blkbytes ,dst_blkbytes ,blksamples ,dst_blkbytes_ch;
	ULONG ch_bytes,blocs;
	ULONG samples;
	UBYTE ch_blkbytes;
	UBYTE ch;
	UBYTE rest;
	BYTE tempb[CH_MAX];
	WORD tempw[CH_MAX];
	SAFuncPtr SARead;
	SmpAccess saw,*sar[CH_MAX];
} WorkData;

//-- prototypes ---------------------------------------------------------------

//-- helper

#ifndef RESTRICT_8BITONLY
UBYTE PackFD8_4(BYTE *fibtab,BYTE *lastb,BYTE aktb);
UBYTE PackFD16_6(LONG *fibtab,WORD *lastw,WORD aktw);
UBYTE PackED8_4(BYTE *fibtab,BYTE *lastb,BYTE aktb);
UBYTE PackED16_5(LONG *fibtab,WORD *lastw,WORD aktw);
UBYTE PackAD8(UBYTE *lastb,UBYTE aktb,UBYTE maxv);
#endif

void BitPack(BYTE *dstbuf,WORD *srcbuf,UBYTE bits,UBYTE anz);

void PrepCrunch(WORD *sbuf,BYTE *dbuf,UBYTE crunch,BYTE *tempb,WORD *tempw);
void Crunch(WORD *sbuf,BYTE *dbuf,UBYTE crunch,BYTE *tempb,WORD *tempw,ULONG datalen);
void INLINE ConvWEndian16(BYTE *buf,ULONG datalen);
void INLINE ConvWEndian32(BYTE *buf,ULONG datalen);
void INLINE ConvWEndian64(BYTE *buf,ULONG datalen);
void INLINE ConvWSignum(WORD *buf,ULONG datalen);
void MergeChannels(BYTE *workbuf,BYTE *workbuf2[CH_MAX],ULONG len,UBYTE ch,UBYTE bytes);
void CalcCrunchedLengths(UBYTE crunch,ULONG *src_blkbytes,ULONG *dst_blkbytes,ULONG *src_hdrbytes,ULONG *dst_hdrbytes,UBYTE *ch_blkbytes);

WorkData *WriteDataInit(SInfo *si,SaveData *sdata,UBYTE crunch,UBYTE channel);
void WriteDataDone(WorkData *wd);

UBYTE INLINE WriteDataHdr_NI(WorkData *wd,UBYTE crunch,UBYTE conv);
UBYTE INLINE WriteDataSub_NI(WorkData *wd,UBYTE crunch,UBYTE conv);
UBYTE INLINE WriteDataFtr_NI(WorkData *wd,UBYTE crunch,UBYTE conv);

UBYTE INLINE WriteDataHdr_I(WorkData *wd,UBYTE crunch,UBYTE conv);
UBYTE INLINE WriteDataSub_I(WorkData *wd,UBYTE crunch,UBYTE conv);
UBYTE INLINE WriteDataFtr_I(WorkData *wd,UBYTE crunch,UBYTE conv);

//-- saver tools

ULONG  ASM LIB_EXPORT(GetCrunchedLength(REG(d0,UBYTE crunch),REG(d1,ULONG src_bytes)));
UBYTE   ASM LIB_EXPORT(WriteData(REG(a0,SInfo *si),REG(a1,SaveData *sdata),REG(d0,UBYTE crunch),REG(d1,UBYTE conv),REG(d2,UBYTE channel)));
//UBYTE ASM LIB_EXPORT(WriteChunkHeader(REG(d0,ULONG type),REG(d1,ULONG size),REG(a0,FILE *outfile)));

//-- globals ------------------------------------------------------------------

WORD alawtab[8]={0xFF,0x1FF,0x3FF,0x7FF,0xFFF,0x1FFF,0x3FFF,0x7FFF};

// DEBUG
//ULONG __dbg_buflen;
//BYTE *__dbg_dbuf,*__dbg_dbufe;
//UBYTE __dbg_loop;
// DEBUG

//-- definitions --------------------------------------------------------------

#ifndef RESTRICT_8BITONLY

UBYTE PackFD8_4(BYTE *fibtab,BYTE *lastb,BYTE aktb) {
	register UBYTE k;
	UBYTE fibentr,fibdif,fibdifm;
	WORD dif=(WORD)aktb-(WORD)(*lastb);

	if(dif>=0) {
		fibdifm=labs(dif-fibtab[15]);fibentr=15;
		for(k=14;k>7;k--) if((fibdif=labs(dif-fibtab[k]))<fibdifm) { fibdifm=fibdif;fibentr=k; }
		if((*lastb)+fibtab[fibentr]>127) fibentr--;
	}
	else {
		fibdifm=labs(dif-fibtab[0]);fibentr=0;
		for(k=1;k<8;k++) if((fibdif=labs(dif-fibtab[k]))<fibdifm) { fibdifm=fibdif;fibentr=k; }
		if((*lastb)+fibtab[fibentr]<-128) fibentr++;
	}
	(*lastb)+=fibtab[fibentr];
	return(fibentr);
}

UBYTE PackFD16_6(LONG *fibtab,WORD *lastw,WORD aktw) {
	register UBYTE k;
	UBYTE fibentr;
	UWORD fibdif,fibdifm;
	LONG dif=(WORD)aktw-(WORD)(*lastw);

	if(dif>=0) {
		fibdifm=labs(dif-fibtab[63]);fibentr=63;
		for(k=62;k>31;k--) if((fibdif=labs(dif-fibtab[k]))<fibdifm) { fibdifm=fibdif;fibentr=k; }
		if((*lastw)+fibtab[fibentr]>32767) fibentr--;
	}
	else {
		fibdifm=labs(dif-fibtab[0]);fibentr=0;
		for(k=1;k<32;k++) if((fibdif=labs(dif-fibtab[k]))<fibdifm) { fibdifm=fibdif;fibentr=k; }
		if((*lastw)+fibtab[fibentr]<-32768) fibentr++;
	}
	(*lastw)+=fibtab[fibentr];
	return(fibentr);
}

UBYTE PackED8_4(BYTE *exptab,BYTE *lastb,BYTE aktb) {
	register UBYTE k;
	UBYTE expentr,expdif,expdifm;
	WORD dif=(WORD)aktb-(WORD)(*lastb);

	if(dif>=0) {
		expdifm=labs(dif-exptab[15]);expentr=15;
		for(k=14;k>7;k--) if((expdif=labs(dif-exptab[k]))<expdifm) { expdifm=expdif;expentr=k; }
		if((*lastb)+exptab[expentr]>127) expentr--;
	}
	else {
		expdifm=labs(dif-exptab[0]);expentr=0;
		for(k=1;k<8;k++) if((expdif=labs(dif-exptab[k]))<expdifm) { expdifm=expdif;expentr=k; }
		if((*lastb)+exptab[expentr]<-128) expentr++;
	}
	(*lastb)+=exptab[expentr];
	return(expentr);
}

UBYTE PackED16_5(LONG *exptab,WORD *lastw,WORD aktw) {
	register UBYTE k;
	UBYTE expentr;
	UWORD expdif,expdifm;
	LONG dif=(WORD)aktw-(WORD)(*lastw);

	if(dif>=0) {
		expdifm=labs(dif-exptab[31]);expentr=31;
		for(k=30;k>15;k--) if((expdif=labs(dif-exptab[k]))<expdifm) { expdifm=expdif;expentr=k; }
		if((*lastw)+exptab[expentr]>32767) expentr--;
	}
	else {
		expdifm=labs(dif-exptab[0]);expentr=0;
		for(k=1;k<16;k++) if((expdif=labs(dif-exptab[k]))<expdifm) { expdifm=expdif;expentr=k; }
		if((*lastw)+exptab[expentr]<-32768) expentr++;
	}
	(*lastw)+=exptab[expentr];
	return(expentr);
}

UBYTE PackAD8(UBYTE *lastb,UBYTE aktb,UBYTE maxv) {
	WORD dif;
	UBYTE adpcm;

	dif=(WORD)aktb-(WORD)(*lastb);
	if(dif>=0) {
		if(dif>maxv) dif=maxv;
		adpcm=(UBYTE)dif;
	}
	else {
		if(dif<(-maxv)) dif=-maxv;
		adpcm=(UBYTE)((maxv+1)|(-dif));		/* dif positiv und erstes Bit setzen */
	}
	(*lastb)+=(BYTE)dif;
	return(adpcm);
}

#endif

// dstbuf memory has to be cleared (set to 0) before

void BitPack(BYTE *dstbuf,WORD *srcbuf,UBYTE bits,UBYTE anz) {
	register BYTE i,j;
	register BYTE by=0,bi=7;

	bits--;
	for(i=0;i<anz;i++) {
		for(j=bits;j>=0;j--) {
			dstbuf[by]|=((srcbuf[i]&(1L<<j))>>j)<<bi;
			bi--;
			if(bi==-1) { bi=7;by++; }
		}
	}
}

void PrepCrunch(WORD *sbuf,BYTE *dbuf,UBYTE crunch,BYTE *tempb,WORD *tempw) {
	switch(crunch) {
		case CRUNCH_PCM32:
			break;
		case CRUNCH_PCM24:
			break;
		case CRUNCH_PCM16:
			break;
		case CRUNCH_PCM12:
			break;
		case CRUNCH_PCM8:
			break;
		case CRUNCH_ULAW:
			break;
		case CRUNCH_ULAW_INV:
			break;
		case CRUNCH_ALAW:
			break;
		case CRUNCH_ALAW_INV:
			break;
		case CRUNCH_IEEE64:
			break;
		case CRUNCH_IEEE32:
			break;
		case CRUNCH_FDPCM8_4:
			dbuf[0]=(BYTE)(sbuf[0]>>8);
			dbuf[1]=(BYTE)(sbuf[1]>>8);
			*tempb=dbuf[1];						// 8bit Deltabasis
			break;
		case CRUNCH_FDPCM16_6:
			dbuf[0]=(BYTE)((sbuf[0]&0xFF00)>>8);
			dbuf[1]=(BYTE)(sbuf[0]&0xFF);
			*tempw=sbuf[0];						// 16bit Deltabasis
			break;
		case CRUNCH_EDPCM8_4:
			dbuf[0]=(BYTE)(sbuf[0]>>8);
			dbuf[1]=(BYTE)(sbuf[1]>>8);
			*tempb=dbuf[1];						// 8bit Deltabasis
			break;
		case CRUNCH_EDPCM16_5:
			dbuf[0]=(BYTE)((sbuf[0]&0xFF00)>>8);
			dbuf[1]=(BYTE)(sbuf[0]&0xFF);
			*tempw=sbuf[0];						// 16bit Deltabasis
			break;
		case CRUNCH_VOC_ADPCM8_4:
			dbuf[0]=(UBYTE)(sbuf[0]>>8);
			dbuf[1]=(UBYTE)(sbuf[1]>>8);
			*tempb=dbuf[1];
			break;
		case CRUNCH_VOC_ADPCM8_3:
			dbuf[0]=(UBYTE)(sbuf[0]>>8);
			dbuf[1]=(UBYTE)(sbuf[1]>>8);
			*tempb=dbuf[1];
			break;
		case CRUNCH_VOC_ADPCM8_2:
			dbuf[0]=(UBYTE)(sbuf[0]>>8);
			dbuf[1]=(UBYTE)(sbuf[1]>>8);
			*tempb=dbuf[1];
			break;
		case CRUNCH_VOC_ADPCM16_4:
			break;
	}
}

void Crunch(WORD *sbuf,BYTE *dbuf,UBYTE crunch,BYTE *tempb,WORD *tempw,ULONG datalen) {
	//register ULONG i;
	register ULONG j;
	register BYTE k;
	WORD sign,exponent=0,mantissa,sval,mask;
	WORD tmpbuf[8];
	UBYTE tub;
	UBYTE *tempub=tempb;

	switch(crunch) {
		case CRUNCH_PCM32:
			for(j=0;j<datalen;j++) { *(dbuf++)=(BYTE)(((*sbuf)&0xFF00)>>8);*(dbuf++)=(BYTE)((*(sbuf++))&0xFF);*(dbuf++)=0;*(dbuf++)=0; }
			break;
		case CRUNCH_PCM24:
			for(j=0;j<datalen;j++) { *(dbuf++)=(BYTE)(((*sbuf)&0xFF00)>>8);*(dbuf++)=(BYTE)((*(sbuf++))&0xFF);*(dbuf++)=0; }
			break;
		case CRUNCH_PCM16:
			//for(j=0;j<datalen;j++) { *(dbuf++)=(BYTE)(((*sbuf)&0xFF00)>>8);*(dbuf++)=(BYTE)((*(sbuf++))&0xFF); }
			for(j=0;j<datalen;j++) { *(WORD *)dbuf=*(sbuf++);dbuf+=2; }
			break;
		case CRUNCH_PCM12:
			for(j=0;j<datalen;j+=2) {
				*(dbuf++)=(BYTE)((*sbuf)>>8);
				*(dbuf++)=(BYTE)((*(sbuf++))&0xF0)|(((*sbuf)&0xF0)>>4);
				*(dbuf++)=(BYTE)((*(sbuf++))>>8);
			}
			break;
		case CRUNCH_PCM8:
			for(j=0;j<datalen;j++) *(dbuf++)=(BYTE)(*(sbuf++)>>8);
			break;
		case CRUNCH_ULAW:
			for(j=0;j<datalen;j++) {
				sval=*(sbuf++);
				sign=0;
				if(sval<0) { sign=128;sval=-sval; }
				for(k=7;k>=0;k--) if(sval>=ulawtab[k]) { exponent=k;k=-1; }
				mantissa=(sval-ulawtab[exponent])>>(exponent+3);
				mantissa=min(mantissa,15);
				*(dbuf++)=(BYTE)(~((UBYTE)(sign|(exponent<<4)|mantissa)));
			}
			break;
		case CRUNCH_ULAW_INV:
			for(j=0;j<datalen;j++) {
				sval=*(sbuf++);
				sign=0;
				if(sval<0) { sign=128;sval=-sval; }
				for(k=7;k>=0;k--) if(sval>=ulawtab[k]) { exponent=k;k=-1; }
				mantissa=(sval-ulawtab[exponent])>>(exponent+3);
				mantissa=min(mantissa,15);
				tub=~((UBYTE)(sign|(exponent<<4)|mantissa));
				tub=(tub<<4 & 0xf0)|(tub>>4 & 0x0f);				// invert bits
				tub=(tub<<2 & 0xcc)|(tub>>2 & 0x33);
				tub=(tub<<1 & 0xaa)|(tub>>1 & 0x55);
				*(dbuf++)=(BYTE)tub;
			}
			break;
		case CRUNCH_ALAW:
			for(j=0;j<datalen;j++) {
				sval=*(sbuf++);
				mask=0x55;sign=0;
				if(sval<0) {sign=128;sval=-sval-8; }
				exponent=8;
				for(k=0;k<8;k++) if(sval<=alawtab[k]) { exponent=k;k=8; }
				if(exponent>=8) *(dbuf++)=(BYTE)(sign|(0x7F^mask));
				else {
					tub=exponent<<4;
					if(exponent<2) tub|=(sval>>4)&0x0F;
					else tub|=(sval>>(exponent+3))&0x0F;
					*(dbuf++)=(BYTE)(sign|(tub^mask));
				}
			}
			break;
		case CRUNCH_ALAW_INV:
			for(j=0;j<datalen;j++) {
				sval=*(sbuf++);
				mask=0x55;sign=0;
				if(sval<0) {sign=128;sval=-sval-8; }
				exponent=8;
				for(k=0;k<8;k++) if(sval<=alawtab[k]) { exponent=k;k=8; }
				if(exponent>=8) tub=sign|(0x7F^mask);
				else {
					tub=exponent<<4;
					if(exponent<2) tub|=(sval>>4)&0x0F;
					else tub|=(sval>>(exponent+3))&0x0F;
					tub=sign|(tub^mask);
				}
				tub=(tub<<4 & 0xf0)|(tub>>4 & 0x0f);				// invert bits
				tub=(tub<<2 & 0xcc)|(tub>>2 & 0x33);
				tub=(tub<<1 & 0xaa)|(tub>>1 & 0x55);
				*(dbuf++)=(BYTE)tub;
			}
			break;
		case CRUNCH_IEEE64:
			for(j=0;j<datalen;j++) { *(double *)dbuf=(((double)*(sbuf++))/32768.0);dbuf+=8; }
			break;
		case CRUNCH_IEEE32:
			for(j=0;j<datalen;j++) { *(float *)dbuf=(((float)*(sbuf++))/32768.0);dbuf+=4; }
			break;
		case CRUNCH_FDPCM8_4:
			memset(dbuf,0,(datalen>>1));
			for(j=0;j<datalen;j+=2) {
				tmpbuf[0]=PackFD8_4(fibtab4,tempb,((*(sbuf++))>>8));		/* 1. */
				tmpbuf[1]=PackFD8_4(fibtab4,tempb,((*(sbuf++))>>8));		/* 2. */
				BitPack(dbuf++,tmpbuf,4,2);
			}
			break;
		case CRUNCH_FDPCM16_6:
			memset(dbuf,0,((datalen>>2)*3));
			for(j=0;j<datalen;j+=4) {
				tmpbuf[0]=PackFD16_6(fibtab6,tempw,*(sbuf++));			/* 1. */
				tmpbuf[1]=PackFD16_6(fibtab6,tempw,*(sbuf++));			/* 2. */
				tmpbuf[2]=PackFD16_6(fibtab6,tempw,*(sbuf++));			/* 3. */
				tmpbuf[3]=PackFD16_6(fibtab6,tempw,*(sbuf++));			/* 4. */
				BitPack(dbuf,tmpbuf,6,4);dbuf+=3;
			}
			break;
		case CRUNCH_EDPCM8_4:
			memset(dbuf,0,(datalen>>1));
			for(j=0;j<datalen;j+=2) {
				tmpbuf[0]=PackED8_4(exptab4,tempb,((*(sbuf++))>>8));		/* 1. */
				tmpbuf[1]=PackED8_4(exptab4,tempb,((*(sbuf++))>>8));		/* 2. */
				BitPack(dbuf++,tmpbuf,4,2);
			}
			break;
		case CRUNCH_EDPCM16_5:
			memset(dbuf,0,((datalen>>3)*5));
			for(j=0;j<datalen;j+=8) {
				tmpbuf[0]=PackED16_5(exptab5,tempw,*(sbuf++));			/* 1. */
				tmpbuf[1]=PackED16_5(exptab5,tempw,*(sbuf++));			/* 2. */
				tmpbuf[2]=PackED16_5(exptab5,tempw,*(sbuf++));			/* 3. */
				tmpbuf[3]=PackED16_5(exptab5,tempw,*(sbuf++));			/* 4. */
				tmpbuf[4]=PackED16_5(exptab5,tempw,*(sbuf++));			/* 4. */
				tmpbuf[5]=PackED16_5(exptab5,tempw,*(sbuf++));			/* 4. */
				tmpbuf[6]=PackED16_5(exptab5,tempw,*(sbuf++));			/* 4. */
				tmpbuf[7]=PackED16_5(exptab5,tempw,*(sbuf++));			/* 4. */
				BitPack(dbuf,tmpbuf,5,8);dbuf+=5;
			}
			break;
		case CRUNCH_VOC_ADPCM8_4:
			memset(dbuf,0,(datalen>>1));
			for(j=0;j<datalen;j+=2) {
				tmpbuf[0]=PackAD8(tempub,(UBYTE)((*(sbuf++))>>8),7);		// normally tempub (unsigned)
				tmpbuf[1]=PackAD8(tempub,(UBYTE)((*(sbuf++))>>8),7);
				BitPack(dbuf++,tmpbuf,4,2);
			}
			break;
		case CRUNCH_VOC_ADPCM8_3:
			memset(dbuf,0,(datalen/3));
			for(j=0;j<datalen;j+=3) {
				tmpbuf[0]= PackAD8(tempub,(UBYTE)((*(sbuf++))>>8),3);
				tmpbuf[1]= PackAD8(tempub,(UBYTE)((*(sbuf++))>>8),3);
				tmpbuf[2]=(PackAD8(tempub,(UBYTE)((*(sbuf++))>>8),1)<<1);
				BitPack(dbuf++,tmpbuf,3,3);
			}
			break;
		case CRUNCH_VOC_ADPCM8_2:
			memset(dbuf,0,(datalen>>2));
			for(j=0;j<datalen;j+=4) {
				tmpbuf[0]=PackAD8(tempub,(UBYTE)((*(sbuf++))>>8),1);
				tmpbuf[1]=PackAD8(tempub,(UBYTE)((*(sbuf++))>>8),1);
				tmpbuf[2]=PackAD8(tempub,(UBYTE)((*(sbuf++))>>8),1);
				tmpbuf[3]=PackAD8(tempub,(UBYTE)((*(sbuf++))>>8),1);
				BitPack(dbuf++,tmpbuf,2,4);
			}
			break;
		case CRUNCH_VOC_ADPCM16_4:
			break;
	}
}

void INLINE ConvWEndian16(BYTE *buf,ULONG datalen) {
	register ULONG i;

	for(i=0;i<datalen;i++) { Swap(buf[0],buf[1]);buf+=2; }
}

void INLINE ConvWEndian32(BYTE *buf,ULONG datalen) {
	register ULONG i;

	for(i=0;i<datalen;i++) { Swap(buf[0],buf[3]);Swap(buf[1],buf[2]);buf+=4; }
}

void INLINE ConvWEndian64(BYTE *buf,ULONG datalen) {
	register ULONG i;

	for(i=0;i<datalen;i++) { Swap(buf[0],buf[7]);Swap(buf[1],buf[6]);Swap(buf[2],buf[5]);Swap(buf[2],buf[4]);buf+=8; }
}

void INLINE ConvWSignum(WORD *buf,ULONG datalen) {
	register ULONG i;

	for(i=0;i<datalen;i++) {
		*buf+=32768;
		buf++;
	}
}

void MergeChannels(BYTE *workbuf,BYTE *workbuf2[CH_MAX],ULONG len,UBYTE ch,UBYTE bytes) {
	register ULONG h,i,j;

	for(h=0;h<len;h+=bytes)
		for(i=0;i<ch;i++)
			for(j=0;j<bytes;j++)
				*(workbuf++)=workbuf2[i][h+j];
}

void CalcCrunchedLengths(UBYTE crunch,ULONG *src_blkbytes,ULONG *dst_blkbytes,ULONG *src_hdrbytes,ULONG *dst_hdrbytes,UBYTE *ch_blkbytes) {
	*src_blkbytes=GetRunTime(sfxprefs_vmem)->blksize;		//  Blocklänge vor Packen
	switch(crunch) {			//  Blocklänge nach Entpacken												Kopflänge vor Pac.	~ nach Packen		Kanalblockgruppe
		case CRUNCH_PCM32:			*dst_blkbytes=MultipleOf( 4,(ULONG)(((double)*src_blkbytes)*2.00  ));	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=4;	break;
		case CRUNCH_PCM24:			*dst_blkbytes=MultipleOf( 3,(ULONG)(((double)*src_blkbytes)*1.50  ));	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=3;	break;
		case CRUNCH_PCM16:			*dst_blkbytes=MultipleOf( 2,				 *src_blkbytes		   );	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=2;	break;
		case CRUNCH_PCM12:			*dst_blkbytes=MultipleOf( 3,(ULONG)(((double)*src_blkbytes)*0.75  ));	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=3;	break;
		case CRUNCH_PCM8:
		case CRUNCH_ULAW:
		case CRUNCH_ULAW_INV:
		case CRUNCH_ALAW:
		case CRUNCH_ALAW_INV:		*dst_blkbytes=MultipleOf( 4,(ULONG)(((double)*src_blkbytes)*0.5   ));	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=1;	break;
		case CRUNCH_IEEE64:			*dst_blkbytes=MultipleOf( 8,(ULONG)(((double)*src_blkbytes)*4.00  ));	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=8;	break;
		case CRUNCH_IEEE32:			*dst_blkbytes=MultipleOf( 4,(ULONG)(((double)*src_blkbytes)*2.00  ));	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=4;	break;
		case CRUNCH_FDPCM8_4:		*dst_blkbytes=MultipleOf( 4,(ULONG)(((double)*src_blkbytes)*0.25  ));	*src_hdrbytes=4;	*dst_hdrbytes=2;	*ch_blkbytes=1;	break;
		case CRUNCH_FDPCM16_6:		*dst_blkbytes=MultipleOf(24,(ULONG)(((double)*src_blkbytes)*0.375 ));	*src_hdrbytes=2;	*dst_hdrbytes=2;	*ch_blkbytes=3;	break;
		case CRUNCH_EDPCM8_4:		*dst_blkbytes=MultipleOf( 4,(ULONG)(((double)*src_blkbytes)*0.25  ));	*src_hdrbytes=4;	*dst_hdrbytes=2;	*ch_blkbytes=1;	break;
		case CRUNCH_EDPCM16_5:		*dst_blkbytes=MultipleOf(40,(ULONG)(((double)*src_blkbytes)*0.3125));	*src_hdrbytes=2;	*dst_hdrbytes=2;	*ch_blkbytes=5;	break;
		case CRUNCH_VOC_ADPCM8_4:	*dst_blkbytes=MultipleOf( 4,(ULONG)(((double)*src_blkbytes)*0.25  ));	*src_hdrbytes=4;	*dst_hdrbytes=2;	*ch_blkbytes=1;	break;
		case CRUNCH_VOC_ADPCM8_3:	*dst_blkbytes=MultipleOf( 4,(ULONG)(((double)*src_blkbytes)*0.1875));	*src_hdrbytes=4;	*dst_hdrbytes=2;	*ch_blkbytes=1;	break;
		case CRUNCH_VOC_ADPCM8_2:	*dst_blkbytes=MultipleOf( 4,(ULONG)(((double)*src_blkbytes)*0.125 ));	*src_hdrbytes=4;	*dst_hdrbytes=2;	*ch_blkbytes=1;	break;
	}
}

WorkData *WriteDataInit(SInfo *si,SaveData *sdata,UBYTE crunch,UBYTE channel) {
	register UBYTE i;
	ULONG memsize;
	WorkData *wd;

	if(wd=AllocVec(sizeof(WorkData),MEMF_ANY|MEMF_CLEAR)) {
		wd->saw.a4=__builtin_getreg(REG_A4);				// file sampleacess
		wd->saw.a6=__builtin_getreg(REG_A6);
		wd->saw.start=sdata->offs;
		wd->saw.vm_mode=VM_MODE_BLK|VM_MODE_WRITE;
		wd->saw.cblen=0;
		wd->saw.seganz=0;									// Anzahl der Segmente = 0, damit kein PreRead

		CalcCrunchedLengths(crunch,&wd->src_blkbytes,&wd->dst_blkbytes,&wd->src_hdrbytes,&wd->dst_hdrbytes,&wd->ch_blkbytes);
		wd->blksamples=Bytes2Samples(wd->src_blkbytes);
		wd->hdrsamples=Bytes2Samples(wd->src_hdrbytes);
		wd->ch_bytes=Samples2Bytes(sdata->savelength);							// Bytes per Channel
		wd->ch_bytes-=wd->src_hdrbytes;											// - Bytes per Header
		wd->blocs=wd->ch_bytes/wd->src_blkbytes;								// how many blocs
		wd->rest=(wd->blocs*wd->src_blkbytes<wd->ch_bytes);
		wd->samples=sdata->savelength;

		// DEBUG
		MSG1("  in channel %ld",channel);
		// DEBUG
		switch(channel&~CHANNEL_INTERLEAVED) {
			case CHANNEL_STEREO: wd->ch=2;break;
			case CHANNEL_QUADRO: wd->ch=4;break;
			default: wd->ch=1;break;
		}

		// DEBUG
		MSG3("  src_blkbytes %6ld  dst_blkbytes  %6ld  blksamples %6ld",wd->src_blkbytes,wd->dst_blkbytes,wd->blksamples);
		MSG3("  src_hdrbytes %6ld  dst_hdrbytes  %6ld  hdrsamples %6ld",wd->src_hdrbytes,wd->dst_hdrbytes,wd->hdrsamples);
		MSG2("  ch_blkbytes  %6ld  ch_bytes %6ld",wd->ch_blkbytes,wd->ch_bytes);
		MSG3("  blocs  %6ld  samples %6ld  channels %1d",wd->blocs,wd->samples,wd->ch);
		// DEBUG

		if(channel&CHANNEL_INTERLEAVED) {
			wd->dst_blkbytes_ch=wd->dst_blkbytes*wd->ch;
			wd->saw.blen=wd->dst_blkbytes_ch;						// zu schreibende Bytes
		}
		else {
			wd->saw.blen=wd->dst_blkbytes;							// zu schreibende Bytes
		}

		memsize=max(wd->src_blkbytes,wd->dst_blkbytes);
		if(!(wd->dbuf1=(WORD *)AllocVec(memsize,MEMF_ANY))) goto Error;
		for(i=0;i<CH_MAX;i++) {
			if(!(wd->dbuf2[i]=(BYTE *)AllocVec(memsize,MEMF_ANY))) goto Error;
		}

		if(!(wd->dbuf=wd->saw.buffer1=AllocVec(wd->saw.blen,MEMF_ANY|MEMF_CLEAR))) goto Error;
		if(!(		  wd->saw.buffer2=AllocVec(wd->saw.blen,MEMF_ANY|MEMF_CLEAR))) goto Error;

	}
	return(wd);
Error:
	WriteDataDone(wd);
	return(NULL);

}

void WriteDataDone(WorkData *wd) {
	register UBYTE i;

	if(wd) {
		FreeVec(wd->saw.buffer1);
		FreeVec(wd->saw.buffer2);

		for(i=0;i<CH_MAX;i++) FreeVec((APTR)wd->dbuf2[i]);
		FreeVec(wd->dbuf1);

		FreeVec(wd);
	}
}

UBYTE INLINE WriteDataHdr_NI(WorkData *wd,UBYTE crunch,UBYTE conv) {
	if(wd->src_hdrbytes) {
		if(!wd->SARead(wd->sar[0],wd->src_hdrbytes)) return(FALSE);					// PreRead
		wd->sbuf=(WORD *)wd->sar[0]->buffer1;
		if(!wd->SARead(wd->sar[0],wd->src_blkbytes)) return(FALSE);
		memcpy(wd->dbuf1,wd->sbuf,wd->src_hdrbytes);
		if(conv&CONV_SIGNUM_F) ConvWSignum(wd->dbuf1,wd->hdrsamples);
		PrepCrunch(wd->dbuf1,wd->dbuf,crunch,&wd->tempb[0],&wd->tempw[0]);		// care about offset later on
		if(conv&CONV_ENDIAN_F) {
			switch(wd->ch_blkbytes) {
				case 2: ConvWEndian16(wd->dbuf,wd->hdrsamples);break;	// 16 bit
				case 4: ConvWEndian32(wd->dbuf,wd->hdrsamples);break;	// 32 bit
				case 8: ConvWEndian64(wd->dbuf,wd->hdrsamples);break;	// 64 bit
			}
		}
		if(!LIB_INTCALL(ASWrite(&wd->saw,wd->dst_hdrbytes))) return(FALSE);
	}
	//-- (caused an offset in saved files, when files are in vmem)
	else if(!wd->SARead(wd->sar[0],wd->src_blkbytes)) return(FALSE);				// PreRead
	return(TRUE);
}

UBYTE INLINE WriteDataSub_NI(WorkData *wd,UBYTE crunch,UBYTE conv) {
	while(wd->curlen<wd->blocs) {
		wd->sbuf=(WORD *)wd->sar[0]->buffer1;
		if(!wd->SARead(wd->sar[0],wd->src_blkbytes)) return(FALSE);
		memcpy(wd->dbuf1,wd->sbuf,wd->src_blkbytes);
		if(conv&CONV_SIGNUM_F) ConvWSignum(wd->dbuf1,wd->blksamples);
		Crunch(wd->dbuf1,wd->dbuf,crunch,&wd->tempb[0],&wd->tempw[0],wd->blksamples);
		if(conv&CONV_ENDIAN_F) {
			switch(wd->ch_blkbytes) {
				case 2: ConvWEndian16(wd->dbuf,wd->blksamples);break;	// 16 bit
				case 4: ConvWEndian32(wd->dbuf,wd->blksamples);break;	// 32 bit
				case 8: ConvWEndian64(wd->dbuf,wd->blksamples);break;	// 64 bit
			}
		}
		if(!LIB_INTCALL(ASWrite(&wd->saw,wd->dst_blkbytes))) return(FALSE);
		wd->curlen++;
	}
	return(TRUE);
}

UBYTE INLINE WriteDataFtr_NI(WorkData *wd,UBYTE crunch,UBYTE conv) {
	ULONG src_tblkbytes,dst_tblkbytes,tblksamples;

	src_tblkbytes=wd->ch_bytes-(wd->blocs*wd->src_blkbytes);				// remaining bytes in stream
	dst_tblkbytes=(ULONG)(src_tblkbytes*((double)wd->dst_blkbytes/(double)wd->src_blkbytes));		// how many bytes will come out of this
	tblksamples=Bytes2Samples(src_tblkbytes);								// and how many samples
	MSG3("  src_tblkbytes %6ld  dst_tblkbytes  %6ld  tblksamples %6ld",src_tblkbytes,dst_tblkbytes,tblksamples);
	MSG("  bloc rest");
	wd->sbuf=(WORD *)wd->sar[0]->buffer1;
	if(!wd->SARead(wd->sar[0],0L)) return(FALSE);								// don't care about the length, we're just fetching the last bloc
	memcpy(wd->dbuf1,wd->sbuf,src_tblkbytes);
	if(conv&CONV_SIGNUM_F) ConvWSignum(wd->dbuf1,tblksamples);
	Crunch(wd->dbuf1,wd->dbuf,crunch,&wd->tempb[0],&wd->tempw[0],tblksamples);
	if(conv&CONV_ENDIAN_F) {
		switch(wd->ch_blkbytes) {
			case 2: ConvWEndian16(wd->dbuf,tblksamples);break;	// 16 bit
			case 4: ConvWEndian32(wd->dbuf,tblksamples);break;	// 32 bit
			case 8: ConvWEndian64(wd->dbuf,tblksamples);break;	// 64 bit
		}
	}
	if(!LIB_INTCALL(ASWrite(&wd->saw,dst_tblkbytes))) return(FALSE);
	wd->curlen++;
	return(TRUE);
}

UBYTE INLINE WriteDataHdr_I(WorkData *wd,UBYTE crunch,UBYTE conv) {
	register UBYTE i;

	if(wd->src_hdrbytes) {
		for(i=0;i<wd->ch;i++) {
			if(!wd->SARead(wd->sar[i],wd->src_hdrbytes)) return(FALSE);						// PreRead
			wd->sbuf=(WORD *)wd->sar[i]->buffer1;
			if(!wd->SARead(wd->sar[i],wd->src_blkbytes)) return(FALSE);
			memcpy(wd->dbuf1,wd->sbuf,wd->src_hdrbytes);
			if(conv&CONV_SIGNUM_F) ConvWSignum(wd->dbuf1,wd->hdrsamples);
			PrepCrunch(wd->dbuf1,wd->dbuf2[i],crunch,&wd->tempb[i],&wd->tempw[i]);		// care about offset later on
			if(conv&CONV_ENDIAN_F) {
				switch(wd->ch_blkbytes) {
					case 2: ConvWEndian16(wd->dbuf2[i],wd->hdrsamples);break;	// 16 bit
					case 4: ConvWEndian32(wd->dbuf2[i],wd->hdrsamples);break;	// 32 bit
					case 8: ConvWEndian64(wd->dbuf2[i],wd->hdrsamples);break;	// 64 bit
				}
			}
		}
		MergeChannels(wd->dbuf,wd->dbuf2,wd->dst_hdrbytes,wd->ch,wd->ch_blkbytes);
		if(!LIB_INTCALL(ASWrite(&wd->saw,wd->dst_hdrbytes*wd->ch))) return(FALSE);
	}
	//-- (caused an offset in saved files, when files are in vmem)
	else for(i=0;i<wd->ch;i++) if(!wd->SARead(wd->sar[i],wd->src_blkbytes)) return(FALSE);	// PreRead
	return(TRUE);
}

UBYTE INLINE WriteDataSub_I(WorkData *wd,UBYTE crunch,UBYTE conv) {
	register UBYTE i;

	while(wd->curlen<wd->blocs) {
		for(i=0;i<wd->ch;i++) {														// for each channel
			wd->sbuf=(WORD *)wd->sar[i]->buffer1;
			if(!wd->SARead(wd->sar[i],wd->src_blkbytes)) return(FALSE);
			memcpy(wd->dbuf1,wd->sbuf,wd->src_blkbytes);
			if(conv&CONV_SIGNUM_F) ConvWSignum(wd->dbuf1,wd->blksamples);
			Crunch(wd->dbuf1,wd->dbuf2[i],crunch,&wd->tempb[i],&wd->tempw[i],wd->blksamples);	 // care about offset later on
			if(conv&CONV_ENDIAN_F) {
				switch(wd->ch_blkbytes) {
					case 2: ConvWEndian16(wd->dbuf2[i],wd->blksamples);break;	// 16 bit
					case 4: ConvWEndian32(wd->dbuf2[i],wd->blksamples);break;	// 32 bit
					case 8: ConvWEndian64(wd->dbuf2[i],wd->blksamples);break;	// 64 bit
				}
			}
		}
		MergeChannels(wd->dbuf,wd->dbuf2,wd->dst_blkbytes,wd->ch,wd->ch_blkbytes);
		if(!LIB_INTCALL(ASWrite(&wd->saw,wd->dst_blkbytes_ch))) return(FALSE);
		wd->curlen++;
	}
	return(TRUE);
}

UBYTE INLINE WriteDataFtr_I(WorkData *wd,UBYTE crunch,UBYTE conv) {
	register UBYTE i;
	ULONG src_tblkbytes,dst_tblkbytes,tblksamples;

	src_tblkbytes=wd->ch_bytes-(wd->blocs*wd->src_blkbytes);				// remaining bytes in stream

	dst_tblkbytes=(ULONG)(src_tblkbytes*((double)wd->dst_blkbytes/(double)wd->src_blkbytes));		// how many bytes will come out of this
	tblksamples=Bytes2Samples(src_tblkbytes);								// and how many samples
	MSG3("  src_tblkbytes %6ld  dst_tblkbytes  %6ld  tblksamples %6ld",src_tblkbytes,dst_tblkbytes,tblksamples);
	MSG("  bloc rest");
	for(i=0;i<wd->ch;i++) {													// for each channel
		wd->sbuf=(WORD *)wd->sar[i]->buffer1;
		if(!wd->SARead(wd->sar[i],0L)) return(FALSE);						// don't care about the length, we're just fetching the last bloc
		memcpy(wd->dbuf1,wd->sbuf,src_tblkbytes);
		if(conv&CONV_SIGNUM_F) ConvWSignum(wd->dbuf1,tblksamples);
		Crunch(wd->dbuf1,wd->dbuf2[i],crunch,&wd->tempb[i],&wd->tempw[i],tblksamples);	// care about offset later on
		if(conv&CONV_ENDIAN_F) {
			switch(wd->ch_blkbytes) {
				case 2: ConvWEndian16(wd->dbuf2[i],tblksamples);break;	// 16 bit
				case 4: ConvWEndian32(wd->dbuf2[i],tblksamples);break;	// 32 bit
				case 8: ConvWEndian64(wd->dbuf2[i],tblksamples);break;	// 64 bit
			}
		}
	}
	MergeChannels(wd->dbuf,wd->dbuf2,dst_tblkbytes,wd->ch,wd->ch_blkbytes);
	if(!LIB_INTCALL(ASWrite(&wd->saw,dst_tblkbytes*wd->ch))) return(FALSE);
	wd->curlen++;
	return(TRUE);
}

//-- saver tools

ULONG SAVEDS ASM LIB_EXPORT(GetCrunchedLength(REG(d0,UBYTE crunch),REG(d1,ULONG src_bytes))) {
	ULONG dst_bytes=0;

	switch(crunch) {
		case CRUNCH_PCM32:			dst_bytes=   src_bytes   *2.00;		break;
		case CRUNCH_PCM24:			dst_bytes=   src_bytes   *1.50;		break;
		case CRUNCH_PCM16:			dst_bytes=   src_bytes;				break;
		case CRUNCH_PCM12:			dst_bytes=   src_bytes   *0.75;		break;
		case CRUNCH_PCM8:
		case CRUNCH_ULAW:
		case CRUNCH_ULAW_INV:
		case CRUNCH_ALAW:
		case CRUNCH_ALAW_INV:		dst_bytes=   src_bytes   *0.5;		break;
		case CRUNCH_IEEE64:			dst_bytes=   src_bytes   *4.00;		break;
		case CRUNCH_IEEE32:			dst_bytes=   src_bytes   *2.00;		break;
		case CRUNCH_FDPCM8_4:		dst_bytes=2+(src_bytes-4)*0.25;		break;
		case CRUNCH_FDPCM16_6:		dst_bytes=2+(src_bytes-2)*0.375;	break;
		case CRUNCH_EDPCM8_4:		dst_bytes=2+(src_bytes-4)*0.25;		break;
		case CRUNCH_EDPCM16_5:		dst_bytes=2+(src_bytes-2)*0.3125;	break;
		case CRUNCH_VOC_ADPCM8_4:	dst_bytes=2+(src_bytes-4)*0.25;		break;
		case CRUNCH_VOC_ADPCM8_3:	dst_bytes=2+(src_bytes-4)*0.1875;	break;
		case CRUNCH_VOC_ADPCM8_2:	dst_bytes=2+(src_bytes-4)*0.125;	break;
	}
	return(dst_bytes);
}

UBYTE SAVEDS ASM LIB_EXPORT(WriteData(REG(a0,SInfo *si),REG(a1,SaveData *sdata),REG(d0,UBYTE crunch),REG(d1,UBYTE conv),REG(d2,UBYTE channel))) {
	register UBYTE i;				// channels
	UBYTE fail=FALSE;
	WorkData *wd;
	ProWinTitle	title;
	PWData *pwd;

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(sdata,__FUNC__,"2.par",4));

	MSG2("  saving range %ld ... %ld",sdata->savestart,sdata->savelength);
	//srclen=sdata->savelength;						// in samples

	if(wd=WriteDataInit(si,sdata,crunch,channel)) {
		if(pwd=LIB_INTCALL(NewProWin())) {

			if(!(channel&CHANNEL_INTERLEAVED)) {
				MSG("  ## non interleaved");
				if(!LIB_INTCALL(ASOpen(&wd->saw,sdata->fn,O_RDWR|O_CREAT|O_APPEND))) {
					MSG("  SAOpen() okay");
					for(i=0;(i<wd->ch && !fail);i++) {													   // for each channel
						if((1<<i)&sdata->chmask) {												// which is active
							if((wd->sar[0]=LIB_INTCALL(SAInit(si,i,sdata->savestart,sdata->savelength,VM_MODE_READ|VM_MODE_BLK|VM_MODE_VBLK,VM_DEFAULT_BLKLEN)))) {
								wd->SARead=wd->sar[0]->safunc;
								MSG1("    after SAInit[%d]",i);
								fail=!WriteDataHdr_NI(wd,crunch,conv);
								MSG1("    after header[%d]",i);
								if(!fail) {
									sprintf(title,LIB_INTCALL(GetString(msgProgressSave)),i+1,wd->ch);
									LIB_INTCALL(StartProWin(pwd,&wd->curlen,title,wd->blocs+wd->rest));
									fail=!WriteDataSub_NI(wd,crunch,conv);
								}
								MSG1("    after main[%d]",i);
								if(wd->rest && !fail) {
									fail=!WriteDataFtr_NI(wd,crunch,conv);
								}
								MSG1("    after rest[%d]",i);
								LIB_INTCALL(SADone(wd->sar[0],si));
								//MSG("  after SADone()");
							}
						}
					}
					LIB_INTCALL(ASClose(&wd->saw));
					//MSG("  after ASClose()");
				}
				else LIB_INTCALL(StdCError(wd->saw.fn,errOpenFile,wd->saw.last_error_no,__FILE__,__LINE__));
			}
			else {
				MSG("  ## interleaved");
				if(!LIB_INTCALL(ASOpen(&wd->saw,sdata->fn,O_RDWR|O_CREAT|O_APPEND))) {
					MSG("  SAOpen() okay");
					for(i=0;i<wd->ch;i++) {
						if(!(wd->sar[i]=LIB_INTCALL(SAInit(si,i,sdata->savestart,sdata->savelength,VM_MODE_READ|VM_MODE_BLK|VM_MODE_VBLK,VM_DEFAULT_BLKLEN)))) fail=TRUE;
					}
					if(!fail) {
						wd->SARead=wd->sar[0]->safunc;
						MSG("    after SAInit");
						fail=!WriteDataHdr_I(wd,crunch,conv);
						MSG("    after header");
						if(!fail) {
							sprintf(title,LIB_INTCALL(GetString(msgProgressSave)),1,1);
							LIB_INTCALL(StartProWin(pwd,&wd->curlen,title,wd->blocs+wd->rest));
							fail=!WriteDataSub_I(wd,crunch,conv);
						}
						MSG("    after main");
						if(wd->rest && !fail) {
							fail=!WriteDataFtr_I(wd,crunch,conv);
						}
						MSG("    after rest");
					}
					for(i=0;i<wd->ch;i++) { if(wd->sar[i]) LIB_INTCALL(SADone(wd->sar[i],si)); }
					LIB_INTCALL(ASClose(&wd->saw));
				}
				else LIB_INTCALL(StdCError(wd->saw.fn,errOpenFile,wd->saw.last_error_no,__FILE__,__LINE__));
			}
			MSG("  now freeing resources");
			LIB_INTCALL(DelProWin(pwd));
		}

		WriteDataDone(wd);
	}
	else { LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));fail=TRUE; }
	ERRC(fail,"WriteData failed");
	OUTRO;
	return(fail);
}

//UBYTE SAVEDS ASM LIB_INTCALL(WriteChunkHeader(REG(d0,ULONG type),REG(d1,ULONG size),REG(a0,FILE *outfile))) {
//	struct ChunkHeader ch;
//
//	ch.TYPE=type;
//	ch.chunksize=size;
//	if(!fwrite(&ch,sizeof(struct ChunkHeader),1,outfile)) { LIB_INTCALL(Message(errWriteData,NULL,__FILE__,__LINE__));return(FALSE); }
//	return(TRUE);
//}

void SAVEDS ASM LIB_EXPORT(SetSampleComment(REG(a0,SInfo *si),REG(a1,STRPTR fn),REG(a2,STRPTR module),REG(a3,STRPTR fmt))) {
	BPTR lock;
	struct InfoData id;
	struct FileInfoBlock fib;

	//INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(fn,__FUNC__,"2.par",4));

	//-- can we write to the object ?
	if(lock=Lock(fn,ACCESS_READ)) {
		//- we can lock the disk, it is writable and we can lock the object and it has not comment 
		if(Info(lock,&id) && (id.id_DiskState==ID_VALIDATED) && Examine(lock,&fib)) {
			BOOL change=FALSE;

			if(!fib.fib_Comment[0]) change=TRUE;
			else {
				//-- check if it is a filenote previously written by SFX
				UBYTE sl=strlen(fib.fib_Comment);
				if((!strncmp(&fib.fib_Comment[sl-4],"s:ms",4))	// ends on 's:ms'
				&& ( strstr(fib.fib_Comment,"Hz"))			    // contains HZ
				// check that number of ',' is 4
					) change=TRUE;
				/** @todo should that really be updated (on every load and save)? */
			}

			if(change) {
				char comment[80];				// see dos/dos.h
				char length1[50],length2[50],modname[50];
				UBYTE sl=strlen(module);
				STRPTR chan[]={"mono","stereo","-","quadro"};
	
				//MSG("file exists, is writable and comment needs to be set");
				UnLock(lock);
	
				//-- create formatted string
				if((sl>2) && (module[sl-2]=='_')) sl-=2;
				strncpy(modname,module,sl);modname[sl]='\0';
				sprintf(length2,"%lf ms",(double)si->slen/((double)si->srat/1000.0));
				UC_ConvertParameter(length1,length2,UGRP_ABSTIME,ATME_H_M_S_MS);
				sprintf(comment,"%-11s, %-14s, %6d Hz, %-6s, %s",modname,fmt,si->srat,chan[si->channels],length1);
	
				SetComment(fn,comment);
			}
			else UnLock(lock);
		}
		else UnLock(lock);
	}
	//else ERR("not setting sample comment");
	//OUTRO;
}

//-- eof ----------------------------------------------------------------------
