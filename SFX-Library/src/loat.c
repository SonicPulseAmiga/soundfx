/****h* sfxsupport.library/loat.c [4.3] *
*
*  NAME
*    loat.c
*  COPYRIGHT
*    $VER: loat.c 4.3 (17.09.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Helper for Loaders - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    17.Jun.1998
*  MODIFICATION HISTORY
*    17.Sep.2003	V 4.3	most recent version
*                           C: endian converion is done prior decrunching (to allow IEEE formats)
*    18.Feb.2003	V 4.2	most recent version
*                           C: refactorized ReadData()
*                           A: error checking in ReadData()
*    08.Feb.2001	V 4.1	most recent version
*							C: now uses asynchronous ProWin
*    18.Jun.2000	V 1.3	most recent version
*    03.Apr.1999	V 1.2	most recent version
*    17.Jun.1998	V 1.0	initial version
*  NOTES
*
*******
*/

#define LOAT_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"
#include "project:sfx-main/sfx-filedefs.h"

typedef struct {
	BYTE *sbuf2[CH_MAX];				// for interleaved data
	BYTE *sbuf1;
	WORD *dbuf;
	ULONG curlen;				    	// blocs
	ULONG src_hdrbytes ,dst_hdrbytes ,hdrsamples ;
	ULONG src_blkbytes ,dst_blkbytes ,blksamples ,src_blkbytes_ch;
	ULONG ch_bytes,blocs;
	ULONG samples;
	UBYTE ch_blkbytes;
	UBYTE ch;
	UBYTE rest;
	SAFuncPtr SAWrite[CH_MAX];
	SmpAccess sar,*saw[CH_MAX];
	UBYTE codecvals[CH_MAX][200];		// temp space for codec globals
} WorkData;

//-- prototypes ---------------------------------------------------------------

//-- helper

void BitUnPack(BYTE *srcbuf,WORD *dstbuf,UBYTE bits,UBYTE anz);

void PrepDecrunch(WORD *dbuf,BYTE *sbuf,UBYTE crunch,APTR codecvals,APTR codecdata);
void Decrunch(	  WORD *dbuf,BYTE *sbuf,UBYTE crunch,APTR codecvals,ULONG datalen);
void INLINE ConvREndian16(BYTE *buf,ULONG datalen);
void INLINE ConvREndian32(BYTE *buf,ULONG datalen);
void INLINE ConvREndian64(BYTE *buf,ULONG datalen);
void INLINE ConvRSignum(  WORD *buf,ULONG datalen);
void SplitChannels(BYTE *workbuf,BYTE *workbuf2[CH_MAX],ULONG len,UBYTE ch,UBYTE bytes);
void CalcDecrunchedLengths(UBYTE crunch,ULONG *src_blkbytes,ULONG *dst_blkbytes,ULONG *src_hdrbytes,ULONG *dst_hdrbytes,UBYTE *ch_blkbytes);

WorkData *ReadDataInit(SInfo *si,LoadData *ldata,ULONG src_bytes,UBYTE crunch,UBYTE channel);
void ReadDataDone(WorkData *wd);

UBYTE INLINE ReadDataHdr_NI(WorkData *wd,UBYTE crunch,UBYTE conv,APTR codecdata);
UBYTE INLINE ReadDataSub_NI(WorkData *wd,UBYTE crunch,UBYTE conv);
UBYTE INLINE ReadDataFtr_NI(WorkData *wd,UBYTE crunch,UBYTE conv);

UBYTE INLINE ReadDataHdr_I(WorkData *wd,UBYTE crunch,UBYTE conv,APTR codecdata);
UBYTE INLINE ReadDataSub_I(WorkData *wd,UBYTE crunch,UBYTE conv);
UBYTE INLINE ReadDataFtr_I(WorkData *wd,UBYTE crunch,UBYTE conv);

//-- loader tools

ULONG ASM LIB_EXPORT(GetDecrunchedLength(REG(d0,UBYTE crunch),REG(d1,ULONG src_bytes)));
ULONG ASM LIB_EXPORT(ReadData(REG(a0,SInfo *si),REG(a1,LoadData *ldata),REG(d0,ULONG src_bytes),REG(d1,UBYTE crunch),REG(d2,UBYTE conv),REG(d3,UBYTE channel)));

ULONG ASM LIB_EXPORT(GetFileLength(REG(a0,STRPTR fn)));

//-- globals ------------------------------------------------------------------

//-- global and access macros for global codec data

struct GCD_FDPCM8_4 {
	WORD w;
};
#define FDPCM8_4_last	(((struct GCD_FDPCM8_4 *)codecvals)->w)

struct GCD_FDPCM16_6 {
	LONG l;
};
#define FDPCM16_6_last	(((struct GCD_FDPCM16_6 *)codecvals)->l)

struct GCD_EDPCM8_4 {
	WORD w;
};
#define EDPCM8_4_last	(((struct GCD_EDPCM8_4 *)codecvals)->w)

struct GCD_EDPCM16_5 {
	LONG l;
};
#define EDPCM16_5_last	(((struct GCD_EDPCM16_5 *)codecvals)->l)

struct GCD_VOC_ADPCM8_4 {
	UWORD w;
};
#define VOC_ADPCM8_4_last	(((struct GCD_VOC_ADPCM8_4 *)codecvals)->w)

struct GCD_VOC_ADPCM8_3 {
	UWORD w;
};
#define VOC_ADPCM8_3_last	(((struct GCD_VOC_ADPCM8_3 *)codecvals)->w)

struct GCD_VOC_ADPCM8_2 {
	UWORD w;
};
#define VOC_ADPCM8_2_last	(((struct GCD_VOC_ADPCM8_2 *)codecvals)->w)

struct GCD_MS_ADPCM16_4 {
	WORD w1,w2;
	WORD c1,c2;
	WORD d;
};
#define MS_ADPCM16_4_last1	(((struct GCD_MS_ADPCM16_4 *)codecvals)->w1)
#define MS_ADPCM16_4_last2	(((struct GCD_MS_ADPCM16_4 *)codecvals)->w2)
#define MS_ADPCM16_4_coef1	(((struct GCD_MS_ADPCM16_4 *)codecvals)->c1)
#define MS_ADPCM16_4_coef2	(((struct GCD_MS_ADPCM16_4 *)codecvals)->c2)
#define MS_ADPCM16_4_delta	(((struct GCD_MS_ADPCM16_4 *)codecvals)->d)

#define MS_ADPCM16_4_numsamples	(((UWORD *)codecdata)[0])
#define MS_ADPCM16_4_numcoef	(((UWORD *)codecdata)[1])
#define MS_ADPCM16_4_Coef1(ix)	(((WORD *)codecdata)[2+(ix<<1)])
#define MS_ADPCM16_4_Coef2(ix)	(((WORD *)codecdata)[3+(ix<<1)])

#define MS_ADPCM16_4_CSCALE          (8)
#define MS_ADPCM16_4_PSCALE          (8)
#define MS_ADPCM16_4_CSCALE_NUM      (1 << MS_ADPCM16_4_CSCALE)
#define MS_ADPCM16_4_PSCALE_NUM      (1 << MS_ADPCM16_4_PSCALE)
#define MS_ADPCM16_4_DELTA4_MIN      (16)
#define MS_ADPCM16_4_OUTPUT4_MAX     (7)
#define MS_ADPCM16_4_OUTPUT4_MIN     (-8)


//-- definitions --------------------------------------------------------------

//-- helper

void BitUnPack(BYTE *srcbuf,WORD *dstbuf,UBYTE bits,UBYTE anz) {
	register BYTE i,j;
	register BYTE by=0,bi=7;

	bits--;
	for(i=0;i<anz;i++) {
		dstbuf[i]=0;
		for(j=bits;j>=0;j--) {
			dstbuf[i]|=((srcbuf[by]&(1L<<bi))>>bi)<<j;
			bi--;
			if(bi==-1) {bi=7;by++;}
		}
	}
}

void PrepDecrunch(WORD *dbuf,BYTE *sbuf,UBYTE crunch,APTR codecvals,APTR codecdata)	{ // WORD *tempw,LONG *templ,ULONG offset) {
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
			dbuf[0]=((WORD)sbuf[0]<<8);
			dbuf[1]=((WORD)sbuf[1]<<8);
			FDPCM8_4_last=(WORD)sbuf[1];
			break;
		case CRUNCH_FDPCM16_6:
			FDPCM16_6_last=(LONG)(dbuf[0]=(((WORD)sbuf[0]<<8)|sbuf[1]));
			break;
		case CRUNCH_EDPCM8_4:
			dbuf[0]=((WORD)sbuf[0]<<8);
			dbuf[1]=((WORD)sbuf[1]<<8);
			EDPCM8_4_last=(WORD)sbuf[1];
			break;
		case CRUNCH_EDPCM16_5:
			EDPCM16_5_last=(LONG)(dbuf[0]=(((WORD)sbuf[0]<<8)|sbuf[1]));
			break;
		case CRUNCH_VOC_ADPCM8_4:
			dbuf[0]=(UWORD)sbuf[0]<<8;
			dbuf[1]=(UWORD)sbuf[1]<<8;
			VOC_ADPCM8_4_last=(UBYTE)sbuf[0];
			break;
		case CRUNCH_VOC_ADPCM8_3:
			dbuf[0]=(UWORD)sbuf[0]<<8;
			dbuf[1]=(UWORD)sbuf[1]<<8;
			VOC_ADPCM8_3_last=(UBYTE)sbuf[0];
			break;
		case CRUNCH_VOC_ADPCM8_2:
			dbuf[0]=(UWORD)sbuf[0]<<8;
			dbuf[1]=(UWORD)sbuf[1]<<8;
			VOC_ADPCM8_2_last=(UBYTE)sbuf[0];
			break;
		case CRUNCH_VOC_ADPCM16_4:
			break;
		case CRUNCH_MS_ADPCM16_4:
			MSG5("%2x %2x %2x %2x %2x",sbuf[0],sbuf[1],sbuf[2],sbuf[3],sbuf[4]);
			//      0 : 1 byte predictor per channel
			//      1 : 2 byte delta per channel
			//      3 : 2 byte first sample per channel
			//      5 : 2 byte second sample per channel			// byteswapping !!!
			MS_ADPCM16_4_delta	=        (((WORD)sbuf[2]<<8)|sbuf[1]);		// aiDelta
			MS_ADPCM16_4_last2	=dbuf[0]=(((WORD)sbuf[6]<<8)|sbuf[5]);		// aiSamp1
			MS_ADPCM16_4_last1	=dbuf[1]=(((WORD)sbuf[4]<<8)|sbuf[3]);		// aiSamp2
			MSG2("firstSamples : %6d %6d",dbuf[0],dbuf[1]);
			MSG1("Delta        : %6d",MS_ADPCM16_4_delta);
			if(sbuf[0]<MS_ADPCM16_4_numcoef) {								// bPredictor
				MS_ADPCM16_4_coef1	=MS_ADPCM16_4_Coef1(sbuf[0]);			// aiCoef1
				MS_ADPCM16_4_coef2	=MS_ADPCM16_4_Coef2(sbuf[0]);			// aiCoef2
				MSG2("Predictors   : %6d %6d",MS_ADPCM16_4_coef1,MS_ADPCM16_4_coef2);
			}
			break;
		case CRUNCH_IMA_ADPCM:
			// new
			break;
		case CRUNCH_DVI_ADPCM:
			// new
			break;
	}
}

void Decrunch(SAMPLE *dbuf,BYTE *sbuf,UBYTE crunch,APTR codecvals,ULONG datalen) {
	register ULONG i;// ,j;
	WORD exponent;
	UBYTE lawb;
	WORD laww,delta;
	WORD wbuf[10];
	LONG samp,pred;

	switch(crunch) {
		case CRUNCH_PCM32:
			for(i=0;i<datalen;i++) { *(dbuf++)=(((WORD)(*(sbuf++)))<<8)|((UBYTE)(*(sbuf++)));sbuf+=2; }
			break;
		case CRUNCH_PCM24:
			for(i=0;i<datalen;i++) { *(dbuf++)=(((WORD)(*(sbuf++)))<<8)|((UBYTE)(*(sbuf++)));sbuf++;}
			break;
		case CRUNCH_PCM16:
			//for(i=0;i<datalen;i++) *(dbuf++)=(((WORD)(*(sbuf++)))<<8)|((UBYTE)(*(sbuf++)));
			for(i=0;i<datalen;i++) { *(dbuf++)=*(WORD *)sbuf;sbuf+=2; }
			break;
		case CRUNCH_PCM12:
			for(i=0;i<datalen;i+=2) {
				*(dbuf++)=((WORD)(*(sbuf++))<<8)|((WORD)(*sbuf)&0xF0);
				*(dbuf++)=(((WORD)(*(sbuf++))&0x0F)<<4)|(((WORD)(*(sbuf++)))<<8);
			}
			break;
		case CRUNCH_PCM8:
			for(i=0;i<datalen;i++) *(dbuf++)=(WORD)(*(sbuf++))<<8;
			break;
		case CRUNCH_ULAW:
			for(i=0;i<datalen;i++) {
				lawb	=(UBYTE)(~(*(sbuf++)));
				exponent=(lawb>>4)&0x07;
				laww	=ulawtab[exponent]+((WORD)(lawb&0x0F)<<(exponent+3));
				if(lawb&0x80) laww=-laww;	/* pick off the sign bit */
				*(dbuf++)=laww;
			}
			break;
		case CRUNCH_ULAW_INV:
			for(i=0;i<datalen;i++) {
				lawb	=(UBYTE)(*(sbuf++));
				lawb	=(lawb<<4 & 0xf0)|(lawb>>4 & 0x0f);		/* invert bits */
				lawb	=(lawb<<2 & 0xcc)|(lawb>>2 & 0x33);
				lawb	=(lawb<<1 & 0xaa)|(lawb>>1 & 0x55);
				lawb	=~lawb;
				exponent=(lawb>>4)&0x07;
				laww	=ulawtab[exponent]+((WORD)(lawb&0x0F)<<(exponent+3));
				if(lawb&0x80) laww=-laww;	/* pick off the sign bit */
				*(dbuf++)=laww;
			}
			break;
		case CRUNCH_ALAW:
			for(i=0;i<datalen;i++) {
				lawb	=((UBYTE)((*(sbuf++))))^0x55;
				exponent=((WORD)((lawb>>4)&0x07))-1;
				laww	=(lawb&0x0F)<<4|0x08;
				if(!exponent) laww|=0x0100;
				else if(exponent>0) { laww|=0x0100;laww<<=exponent; }
				if(lawb&0x80) laww=-laww;	/* pick off the sign bit */
				*(dbuf++)=laww;
			}
			break;
		case CRUNCH_ALAW_INV:
			for(i=0;i<datalen;i++) {
				lawb	=(UBYTE)(*(sbuf++));
				lawb	=(lawb<<4 & 0xf0)|(lawb>>4 & 0x0f);		/* invert bits */
				lawb	=(lawb<<2 & 0xcc)|(lawb>>2 & 0x33);
				lawb	=(lawb<<1 & 0xaa)|(lawb>>1 & 0x55);
				lawb	^=0x55;
				exponent=((WORD)((lawb>>4)&0x07))-1;
				laww	=(lawb&0x0F)<<4|0x08;
				if(!exponent) laww|=0x0100;
				else if(exponent>0) { laww|=0x0100;laww<<=exponent; }
				if(lawb&0x80) laww=-laww;	/* pick off the sign bit */
				*(dbuf++)=laww;
			}
			break;
		case CRUNCH_IEEE64:
			for(i=0;i<datalen;i++) { *(dbuf++)=(SAMPLE)(*(double *)sbuf*32768.0);sbuf+=8; }
			break;
		case CRUNCH_IEEE32:
			for(i=0;i<datalen;i++) { *(dbuf++)=(SAMPLE)(*(float *)sbuf*32768.0);sbuf+=4; }
			break;
		case CRUNCH_FDPCM8_4:
			for(i=0;i<datalen;i+=2) {
				BitUnPack(sbuf++,wbuf,4,2);
				*(dbuf++)=((WORD)((FDPCM8_4_last)+=fibtab4[wbuf[0]])<<8);
				*(dbuf++)=((WORD)((FDPCM8_4_last)+=fibtab4[wbuf[1]])<<8);
			}
			break;
		case CRUNCH_FDPCM16_6:
			for(i=0;i<datalen;i+=4) {
				BitUnPack(sbuf,wbuf,6,4);sbuf+=3;
				*(dbuf++)=(WORD)((FDPCM16_6_last)+=fibtab6[wbuf[0]]);
				*(dbuf++)=(WORD)((FDPCM16_6_last)+=fibtab6[wbuf[1]]);
				*(dbuf++)=(WORD)((FDPCM16_6_last)+=fibtab6[wbuf[2]]);
				*(dbuf++)=(WORD)((FDPCM16_6_last)+=fibtab6[wbuf[3]]);
			}
			break;
		case CRUNCH_EDPCM8_4:
			for(i=0;i<datalen;i+=2) {
				BitUnPack(sbuf++,wbuf,4,2);
				*(dbuf++)=((WORD)((EDPCM8_4_last)+=exptab4[wbuf[0]])<<8);
				*(dbuf++)=((WORD)((EDPCM8_4_last)+=exptab4[wbuf[1]])<<8);
			}
			break;
		case CRUNCH_EDPCM16_5:
			for(i=0;i<datalen;i+=8) {
				BitUnPack(sbuf,wbuf,5,8);sbuf+=5;
				*(dbuf++)=(WORD)((EDPCM16_5_last)+=exptab5[wbuf[0]]);
				*(dbuf++)=(WORD)((EDPCM16_5_last)+=exptab5[wbuf[1]]);
				*(dbuf++)=(WORD)((EDPCM16_5_last)+=exptab5[wbuf[2]]);
				*(dbuf++)=(WORD)((EDPCM16_5_last)+=exptab5[wbuf[3]]);
				*(dbuf++)=(WORD)((EDPCM16_5_last)+=exptab5[wbuf[4]]);
				*(dbuf++)=(WORD)((EDPCM16_5_last)+=exptab5[wbuf[5]]);
				*(dbuf++)=(WORD)((EDPCM16_5_last)+=exptab5[wbuf[6]]);
				*(dbuf++)=(WORD)((EDPCM16_5_last)+=exptab5[wbuf[7]]);
			}
			break;
		case CRUNCH_VOC_ADPCM8_4:
			for(i=0;i<datalen;i+=2) {
				BitUnPack(sbuf++,wbuf,4,2);
				if(wbuf[0]&8) (VOC_ADPCM8_4_last)-=(wbuf[0]&7);	/* 1. */
				else			(VOC_ADPCM8_4_last)+=wbuf[0];
				if(		(VOC_ADPCM8_4_last)<  0) (VOC_ADPCM8_4_last)=0;
				else if((VOC_ADPCM8_4_last)>255) (VOC_ADPCM8_4_last)=255;
				*(dbuf++)=(WORD)((VOC_ADPCM8_4_last)<<8);
				if(wbuf[1]&8) (VOC_ADPCM8_4_last)-=(wbuf[1]&7);	/* 2. */
				else			(VOC_ADPCM8_4_last)+=wbuf[1];
				if(		(VOC_ADPCM8_4_last)<  0) (VOC_ADPCM8_4_last)=0;
				else if((VOC_ADPCM8_4_last)>255) (VOC_ADPCM8_4_last)=255;
				*(dbuf++)=(WORD)((VOC_ADPCM8_4_last)<<8);
			}
			break;
		case CRUNCH_VOC_ADPCM8_3:
			for(i=0;i<datalen;i+=3) {
				BitUnPack(sbuf++,wbuf,3,3);
				if(wbuf[0]&4)	(VOC_ADPCM8_3_last)-=(wbuf[0]&3);	/* 1. */
				else			(VOC_ADPCM8_3_last)+=wbuf[0];
				if(		(VOC_ADPCM8_3_last)<  0) (VOC_ADPCM8_3_last)=0;
				else if((VOC_ADPCM8_3_last)>255) (VOC_ADPCM8_3_last)=255;
				*(dbuf++)=(WORD)((VOC_ADPCM8_3_last)<<8);
				if(wbuf[1]&4) (VOC_ADPCM8_3_last)-=(wbuf[1]&3);	/* 2. */
				else			(VOC_ADPCM8_3_last)+=wbuf[1];
				if(		(VOC_ADPCM8_3_last)<  0) (VOC_ADPCM8_3_last)=0;
				else if((VOC_ADPCM8_3_last)>255) (VOC_ADPCM8_3_last)=255;
				*(dbuf++)=(WORD)((VOC_ADPCM8_3_last)<<8);
				wbuf[3]>>=1;							/* 3. */
				if(wbuf[3]&2) (VOC_ADPCM8_3_last)-=(wbuf[3]&1);
				else			(VOC_ADPCM8_3_last)+=wbuf[3];
				if(		(VOC_ADPCM8_3_last)<  0) (VOC_ADPCM8_3_last)=0;
				else if((VOC_ADPCM8_3_last)>255) (VOC_ADPCM8_3_last)=255;
				*(dbuf++)=(WORD)((VOC_ADPCM8_3_last)<<8);
			}
			break;
		case CRUNCH_VOC_ADPCM8_2:
			for(i=0;i<datalen;i+=4) {
				BitUnPack(sbuf++,wbuf,2,4);
				if(wbuf[0]&2) (VOC_ADPCM8_2_last)-=(wbuf[0]&1);	/* 1. */
				else			(VOC_ADPCM8_2_last)+=wbuf[0];
				if(		(VOC_ADPCM8_2_last)<  0) (VOC_ADPCM8_2_last)=0;
				else if((VOC_ADPCM8_2_last)>255) (VOC_ADPCM8_2_last)=255;
				*(dbuf++)=(WORD)((VOC_ADPCM8_2_last)<<8);
				if(wbuf[1]&2) (VOC_ADPCM8_2_last)-=(wbuf[1]&1);	/* 2. */
				else			(VOC_ADPCM8_2_last)+=wbuf[1];
				if(		(VOC_ADPCM8_2_last)<  0) (VOC_ADPCM8_2_last)=0;
				else if((VOC_ADPCM8_2_last)>255) (VOC_ADPCM8_2_last)=255;
				*(dbuf++)=(WORD)((VOC_ADPCM8_2_last)<<8);
				if(wbuf[3]&2) (VOC_ADPCM8_2_last)-=(wbuf[3]&1);	/* 3. */
				else			(VOC_ADPCM8_2_last)+=wbuf[3];
				if(		(VOC_ADPCM8_2_last)<  0) (VOC_ADPCM8_2_last)=0;
				else if((VOC_ADPCM8_2_last)>255) (VOC_ADPCM8_2_last)=255;
				*(dbuf++)=(WORD)((VOC_ADPCM8_2_last)<<8);
				if(wbuf[4]&2) (VOC_ADPCM8_2_last)-=(wbuf[4]&1);	/* 4. */
				else			(VOC_ADPCM8_2_last)+=wbuf[4];
				if(		(VOC_ADPCM8_2_last)<  0) (VOC_ADPCM8_2_last)=0;
				else if((VOC_ADPCM8_2_last)>255) (VOC_ADPCM8_2_last)=255;
				*(dbuf++)=(WORD)((VOC_ADPCM8_2_last)<<8);
			}
			break;
		case CRUNCH_VOC_ADPCM16_4:
			break;
		case CRUNCH_MS_ADPCM16_4:
			for(i=0;i<datalen;i+=2) {
				BitUnPack(sbuf++,wbuf,4,2);
				if(wbuf[0]&4)	wbuf[0]=-(wbuf[0]&3);		/* sign extension */
				if(wbuf[1]&4)	wbuf[1]=-(wbuf[1]&3);
				// DEBUG
				if(i<6) {
					MSG3("l[%1d] : %2d %3d",i,((UWORD)wbuf[0])&15,gaiP4tab[((UWORD)wbuf[0])&15]);
					MSG3("r[%1d] : %2d %3d",i,((UWORD)wbuf[1])&15,gaiP4tab[((UWORD)wbuf[1])&15]);
				}
				// DEBUG
				delta=MS_ADPCM16_4_delta;							/* 1. */
				MS_ADPCM16_4_delta=(WORD)(gaiP4tab[((UWORD)wbuf[0])&15]*delta)>>MS_ADPCM16_4_PSCALE;
				if(MS_ADPCM16_4_delta<MS_ADPCM16_4_DELTA4_MIN) MS_ADPCM16_4_delta=MS_ADPCM16_4_DELTA4_MIN;
				pred=(((LONG)MS_ADPCM16_4_last1 * MS_ADPCM16_4_coef1) + ((LONG)MS_ADPCM16_4_last2 * MS_ADPCM16_4_coef2)) >> MS_ADPCM16_4_CSCALE;
				samp=((LONG)wbuf[0]*delta)+pred;
				CLIPINT(samp);
				*(dbuf++)=(WORD)samp;
				MS_ADPCM16_4_last2=MS_ADPCM16_4_last1;
				MS_ADPCM16_4_last1=(WORD)samp;
				delta=MS_ADPCM16_4_delta;							/* 2. */
				MS_ADPCM16_4_delta=(WORD)(gaiP4tab[((UWORD)wbuf[1])&15]*delta)>>MS_ADPCM16_4_PSCALE;
				if(MS_ADPCM16_4_delta<MS_ADPCM16_4_DELTA4_MIN) MS_ADPCM16_4_delta=MS_ADPCM16_4_DELTA4_MIN;
				pred=(((LONG)MS_ADPCM16_4_last1 * MS_ADPCM16_4_coef1) + ((LONG)MS_ADPCM16_4_last2 * MS_ADPCM16_4_coef2)) >> MS_ADPCM16_4_CSCALE;
				samp=((LONG)wbuf[1]*delta)+pred;
				CLIPINT(samp);
				*(dbuf++)=(WORD)samp;
				MS_ADPCM16_4_last2=MS_ADPCM16_4_last1;
				MS_ADPCM16_4_last1=(WORD)samp;
			}
			break;
		case CRUNCH_IMA_ADPCM:
			// new
			break;
		case CRUNCH_DVI_ADPCM:
			// new
			break;
	}
}

void INLINE ConvREndian16(BYTE *buf,ULONG datalen) {
	register ULONG i;

	for(i=0;i<datalen;i++) { Swap(buf[0],buf[1]);buf+=2; }
}

void INLINE ConvREndian32(BYTE *buf,ULONG datalen) {
	register ULONG i;

	for(i=0;i<datalen;i++) { Swap(buf[0],buf[3]);Swap(buf[1],buf[2]);buf+=4; }
}

void INLINE ConvREndian64(BYTE *buf,ULONG datalen) {
	register ULONG i;

	for(i=0;i<datalen;i++) { Swap(buf[0],buf[7]);Swap(buf[1],buf[6]);Swap(buf[2],buf[5]);Swap(buf[2],buf[4]);buf+=8; }
}

void INLINE ConvRSignum(WORD *buf,ULONG datalen) {
	register ULONG i;

	for(i=0;i<datalen;i++) {
		*buf-=32768;
		buf++;
	}
}

void SplitChannels(BYTE *workbuf,BYTE *workbuf2[CH_MAX],ULONG len,UBYTE ch,UBYTE bytes) {
	register ULONG h,j;
	register UBYTE i;

	for(h=0;h<len;h+=bytes)							// for each bytebloc
		for(i=0;i<ch;i++)								// for each channel
			for(j=0;j<bytes;j++)							// copy byteblock to channel
				//*(workbuf2[i]++)=*(workbuf++);
				workbuf2[i][h+j]=*(workbuf++);
}

void CalcDecrunchedLengths(UBYTE crunch,ULONG *src_blkbytes,ULONG *dst_blkbytes,ULONG *src_hdrbytes,ULONG *dst_hdrbytes,UBYTE *ch_blkbytes) {
	*dst_blkbytes=GetRunTime(sfxprefs_vmem)->blksize;
	switch(crunch) {			//  Blocklänge vor Entpacken											~ nach Entpacken										Kopflänge vor Entp.	~ nach Entpacken	Kanalblockgruppe
		case CRUNCH_PCM32:			*src_blkbytes=MultipleOf(4,(ULONG)((double)*dst_blkbytes*2.00  ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/2.00  );	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=4;	break;
		case CRUNCH_PCM24:			*src_blkbytes=MultipleOf(3,(ULONG)((double)*dst_blkbytes*1.50  ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/1.50  );	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=3;	break;
		case CRUNCH_PCM16:			*src_blkbytes=MultipleOf(2,				   *dst_blkbytes		);	*dst_blkbytes=				  *src_blkbytes		   ;	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=2;	break;
		case CRUNCH_PCM12:			*src_blkbytes=MultipleOf(3,(ULONG)((double)*dst_blkbytes*0.75  ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.75  );	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=3;	break;
		case CRUNCH_PCM8:
		case CRUNCH_ULAW:
		case CRUNCH_ULAW_INV:
		case CRUNCH_ALAW:
		case CRUNCH_ALAW_INV:		*src_blkbytes=MultipleOf(1,(ULONG)((double)*dst_blkbytes*0.50  ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.50  );	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=1;	break;
		case CRUNCH_IEEE64:			*src_blkbytes=MultipleOf(8,(ULONG)((double)*dst_blkbytes*4.00  ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/4.00  );	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=8;	break;
		case CRUNCH_IEEE32:			*src_blkbytes=MultipleOf(4,(ULONG)((double)*dst_blkbytes*2.00  ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/2.00  );	*src_hdrbytes=0;	*dst_hdrbytes=0;	*ch_blkbytes=4;	break;
		case CRUNCH_FDPCM8_4:		*src_blkbytes=MultipleOf(1,(ULONG)((double)*dst_blkbytes*0.25  ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.25  );	*src_hdrbytes=2;	*dst_hdrbytes=4;	*ch_blkbytes=1;	break;
		case CRUNCH_FDPCM16_6:		*src_blkbytes=MultipleOf(3,(ULONG)((double)*dst_blkbytes*0.375 ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.375 );	*src_hdrbytes=2;	*dst_hdrbytes=2;	*ch_blkbytes=3;	break;
		case CRUNCH_EDPCM8_4:		*src_blkbytes=MultipleOf(1,(ULONG)((double)*dst_blkbytes*0.25  ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.25  );	*src_hdrbytes=2;	*dst_hdrbytes=4;	*ch_blkbytes=1;	break;
		case CRUNCH_EDPCM16_5:		*src_blkbytes=MultipleOf(5,(ULONG)((double)*dst_blkbytes*0.3125));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.3125);	*src_hdrbytes=2;	*dst_hdrbytes=2;	*ch_blkbytes=5;	break;
		case CRUNCH_VOC_ADPCM8_4:	*src_blkbytes=MultipleOf(1,(ULONG)((double)*dst_blkbytes*0.25  ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.25  );	*src_hdrbytes=2;	*dst_hdrbytes=4;	*ch_blkbytes=1;	break;
		case CRUNCH_VOC_ADPCM8_3:	*src_blkbytes=MultipleOf(1,(ULONG)((double)*dst_blkbytes*0.1875));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.1825);	*src_hdrbytes=2;	*dst_hdrbytes=4;	*ch_blkbytes=1;	break;
		case CRUNCH_VOC_ADPCM8_2:	*src_blkbytes=MultipleOf(1,(ULONG)((double)*dst_blkbytes*0.125 ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.125 );	*src_hdrbytes=2;	*dst_hdrbytes=4;	*ch_blkbytes=1;	break;
		// new
		case CRUNCH_MS_ADPCM16_4:	*src_blkbytes=MultipleOf(1,(ULONG)((double)*dst_blkbytes*0.125 ));	*dst_blkbytes=(ULONG)((double)*src_blkbytes/0.125 );	*src_hdrbytes=7;	*dst_hdrbytes=4;	*ch_blkbytes=1;	break;
		case CRUNCH_IMA_ADPCM:			break;
		case CRUNCH_DVI_ADPCM:			break;
	}
}

WorkData *ReadDataInit(SInfo *si,LoadData *ldata,ULONG src_bytes,UBYTE crunch,UBYTE channel) {
	register UBYTE i;
	ULONG memsize;
	WorkData *wd;

	if(wd=AllocVec(sizeof(WorkData),MEMF_ANY|MEMF_CLEAR)) {
		wd->sar.a4=__builtin_getreg(REG_A4);			    // file sampleacess
		wd->sar.a6=__builtin_getreg(REG_A6);
		wd->sar.vm_mode=VM_MODE_READ|VM_MODE_BLK|VM_MODE_VBLK;
		wd->sar.cblen=0;
		wd->sar.seganz=0;									// Anzahl der Segmente = 0, damit kein PreRead

		CalcDecrunchedLengths(crunch,&wd->src_blkbytes,&wd->dst_blkbytes,&wd->src_hdrbytes,&wd->dst_hdrbytes,&wd->ch_blkbytes);
		wd->blksamples=Bytes2Samples(wd->dst_blkbytes);
		wd->hdrsamples=Bytes2Samples(wd->dst_hdrbytes);

		MSG1("  in channel %ld",channel);
		switch(channel&~CHANNEL_INTERLEAVED) {
			case CHANNEL_STEREO: wd->ch=2;break;
			case CHANNEL_QUADRO: wd->ch=4;break;
			default: wd->ch=1;break;
		}

		if(channel&CHANNEL_INTERLEAVED) {
			wd->src_blkbytes_ch=wd->src_blkbytes*wd->ch;									// we read blkbytes for each channel
			wd->ch_bytes=src_bytes/wd->ch;													// Bytes per Channel
			wd->ch_bytes-=wd->src_hdrbytes;													// - Bytes per Header
			wd->blocs=wd->ch_bytes/wd->src_blkbytes;											// how many blocs
			wd->sar.blen=wd->src_blkbytes_ch;												// Bytes to read
		}
		else {
			if(ldata->flags&LDF_NOCHOFF) wd->ch_bytes=src_bytes;							// Bytes per Channel
			else wd->ch_bytes=src_bytes/wd->ch;
			wd->ch_bytes-=wd->src_hdrbytes;													// - Bytes per Header
			wd->blocs=wd->ch_bytes/wd->src_blkbytes;											// how many blocs
			wd->sar.blen=wd->src_blkbytes;													// Bytes to read
		}

		wd->samples=wd->hdrsamples+(ULONG)((double)(Bytes2Samples(wd->ch_bytes))*((double)wd->dst_blkbytes/(double)wd->src_blkbytes));			// how many samples will we get in the end
		wd->rest=(wd->blocs*wd->src_blkbytes<wd->ch_bytes);

		// DEBUG
		MSG3("  src_blkbytes %6ld  dst_blkbytes  %6ld  blksamples %6ld",wd->src_blkbytes,wd->dst_blkbytes,wd->blksamples);
		MSG3("  src_hdrbytes %6ld  dst_hdrbytes  %6ld  hdrsamples %6ld",wd->src_hdrbytes,wd->dst_hdrbytes,wd->hdrsamples);
		MSG2("  ch_blkbytes  %6ld  ch_bytes %6ld",wd->ch_blkbytes,wd->ch_bytes);
		MSG3("  blocs  %6ld  samples %6ld  channels %1d",wd->blocs,wd->samples,wd->ch);
		// DEBUG

		if(!si->memory[0]) {					// nothing allocated yet
			MSG("Allocate Sample Memory");
			if(!LIB_INTCALL(AllocSampleMem(si,wd->samples,wd->ch))) goto Error;
		}

		memsize=max(wd->src_blkbytes,wd->dst_blkbytes);
		for(i=0;i<CH_MAX;i++) {
			if(!(wd->sbuf2[i]=(BYTE *)AllocVec(memsize,MEMF_ANY))) goto Error;
		}

		if(!(wd->sbuf1=wd->sar.buffer1=AllocVec(wd->sar.blen,MEMF_ANY|MEMF_CLEAR))) goto Error;
		if(!(          wd->sar.buffer2=AllocVec(wd->sar.blen,MEMF_ANY|MEMF_CLEAR))) goto Error;
	}
	return(wd);
Error:
	ReadDataDone(wd);
	return(NULL);
}

void  ReadDataDone(WorkData *wd) {
	register UBYTE i;

	if(wd) {
		FreeVec(wd->sar.buffer1);
		FreeVec(wd->sar.buffer2);

		for(i=0;i<CH_MAX;i++) FreeVec((APTR)wd->sbuf2[i]);

		FreeVec(wd);
	}
}

UBYTE INLINE ReadDataHdr_NI(WorkData *wd,UBYTE crunch,UBYTE conv,APTR codecdata) {
	if(wd->src_hdrbytes) {											// process hdr (for some crunch-types)
		if(!LIB_INTCALL(ASRead(&wd->sar,wd->src_hdrbytes)) 							 // PreRead
		|| !LIB_INTCALL(ASRead(&wd->sar,wd->src_blkbytes))) return(FALSE);
		if(conv&CONV_ENDIAN_F) {
			switch(wd->ch_blkbytes) {
				case 2: ConvREndian16(wd->sbuf1,wd->hdrsamples);break;	// 16 bit
				case 4: ConvREndian32(wd->sbuf1,wd->hdrsamples);break;	// 32 bit
				case 8: ConvREndian64(wd->sbuf1,wd->hdrsamples);break;	// 64 bit
			}
		}
		PrepDecrunch(wd->dbuf,wd->sbuf1,crunch,&wd->codecvals[0],codecdata); // care about offset later on (e.g. VOC mit mehreren Blöcken)
		if(conv&CONV_SIGNUM_F) ConvRSignum(wd->dbuf,wd->hdrsamples);
		if(!wd->SAWrite[0](wd->saw[0],wd->dst_hdrbytes)) return(FALSE);
		wd->dbuf=(WORD *)(wd->saw[0]->buffer1);
	}
	else if(!LIB_INTCALL(ASRead(&wd->sar,wd->src_blkbytes))) return(FALSE);			// PreRead
	return(TRUE);
}

UBYTE INLINE ReadDataSub_NI(WorkData *wd,UBYTE crunch,UBYTE conv) {
	while(wd->curlen<wd->blocs) {
		if(!LIB_INTCALL(ASRead(&wd->sar,wd->src_blkbytes))) return(FALSE);
		if(conv&CONV_ENDIAN_F) {
			switch(wd->ch_blkbytes) {
				case 2: ConvREndian16(wd->sbuf1,wd->blksamples);break;	// 16 bit
				case 4: ConvREndian32(wd->sbuf1,wd->blksamples);break;	// 32 bit
				case 8: ConvREndian64(wd->sbuf1,wd->blksamples);break;	// 64 bit
			}
		}
		Decrunch(wd->dbuf,wd->sbuf1,crunch,&wd->codecvals[0],wd->blksamples);
		if(conv&CONV_SIGNUM_F) ConvRSignum(wd->dbuf,wd->blksamples);
		if(!wd->SAWrite[0](wd->saw[0],wd->dst_blkbytes)) return(FALSE);
		wd->dbuf=(WORD *)(wd->saw[0]->buffer1);
		wd->curlen++;
	}
	return(TRUE);
}

UBYTE INLINE ReadDataFtr_NI(WorkData *wd,UBYTE crunch,UBYTE conv) {
	ULONG src_tblkbytes,dst_tblkbytes,tblksamples;

	src_tblkbytes=wd->ch_bytes-(wd->blocs*wd->src_blkbytes);				// remaining bytes in stream
	dst_tblkbytes=(ULONG)(src_tblkbytes*((double)wd->dst_blkbytes/(double)wd->src_blkbytes));		// how many bytes will come out of this
	tblksamples=Bytes2Samples(dst_tblkbytes);								// and how many samples
	MSG3("  src_tblkbytes %6ld  dst_tblkbytes  %6ld  tblksamples %6ld",src_tblkbytes,dst_tblkbytes,tblksamples);
	if(!LIB_INTCALL(ASRead(&wd->sar,NULL))) return(FALSE);							   // don't care about the length, we're just fetching the last bloc
	if(conv&CONV_ENDIAN_F) {
		switch(wd->ch_blkbytes) {
			case 2: ConvREndian16(wd->sbuf1,tblksamples);break;	// 16 bit
			case 4: ConvREndian32(wd->sbuf1,tblksamples);break;	// 32 bit
			case 8: ConvREndian64(wd->sbuf1,tblksamples);break;	// 64 bit
		}
	}
	Decrunch(wd->dbuf,wd->sbuf1,crunch,&wd->codecvals[0],tblksamples);
	if(conv&CONV_SIGNUM_F) ConvRSignum(wd->dbuf,tblksamples);
	if(!wd->SAWrite[0](wd->saw[0],dst_tblkbytes)) return(FALSE);
	wd->curlen++;
	return(TRUE);
}

UBYTE INLINE ReadDataHdr_I(WorkData *wd,UBYTE crunch,UBYTE conv,APTR codecdata) {
	register UBYTE i;

	if(wd->src_hdrbytes) {											  // process hdr (for some crunch-types)
		if(!LIB_INTCALL(ASRead(&wd->sar,wd->src_hdrbytes*wd->ch))									// PreRead
		|| !LIB_INTCALL(ASRead(&wd->sar,wd->src_blkbytes_ch))) return(FALSE);
		SplitChannels(wd->sbuf1,wd->sbuf2,wd->src_hdrbytes,wd->ch,wd->src_hdrbytes);	// header is usualy not interleaved
		for(i=0;i<wd->ch;i++) {															// for each channel
			wd->dbuf=(WORD *)wd->saw[i]->buffer1;
			if(conv&CONV_ENDIAN_F) {
				switch(wd->ch_blkbytes) {
					case 2: ConvREndian16(wd->sbuf2[i],wd->hdrsamples);break;	// 16 bit
					case 4: ConvREndian32(wd->sbuf2[i],wd->hdrsamples);break;	// 32 bit
					case 8: ConvREndian64(wd->sbuf2[i],wd->hdrsamples);break;	// 64 bit
				}
			}
			PrepDecrunch(wd->dbuf,wd->sbuf2[i],crunch,&wd->codecvals[i],codecdata);		// care about offset later on
			if(conv&CONV_SIGNUM_F) ConvRSignum(wd->dbuf,wd->hdrsamples);
			if(!wd->SAWrite[i](wd->saw[i],wd->dst_hdrbytes)) return(FALSE);
		}
	}
	else if(!LIB_INTCALL(ASRead(&wd->sar,wd->src_blkbytes_ch))) return(FALSE);		    		// PreRead
	return(TRUE);
}

UBYTE INLINE ReadDataSub_I(WorkData *wd,UBYTE crunch,UBYTE conv) {
	register UBYTE i;

	while(wd->curlen<wd->blocs) {
		if(!LIB_INTCALL(ASRead(&wd->sar,wd->src_blkbytes_ch))) return(FALSE);
		SplitChannels(wd->sbuf1,wd->sbuf2,wd->src_blkbytes,wd->ch,wd->ch_blkbytes);
		for(i=0;i<wd->ch;i++) {														// for each channel
			wd->dbuf=(WORD *)wd->saw[i]->buffer1;
			if(conv&CONV_ENDIAN_F) {
				switch(wd->ch_blkbytes) {
					case 2: ConvREndian16(wd->sbuf2[i],wd->blksamples);break;	// 16 bit
					case 4: ConvREndian32(wd->sbuf2[i],wd->blksamples);break;	// 32 bit
					case 8: ConvREndian64(wd->sbuf2[i],wd->blksamples);break;	// 64 bit
				}
			}
			Decrunch(wd->dbuf,wd->sbuf2[i],crunch,&wd->codecvals[i],wd->blksamples);
			if(conv&CONV_SIGNUM_F) ConvRSignum(wd->dbuf,wd->blksamples);
			if(!wd->SAWrite[i](wd->saw[i],wd->dst_blkbytes)) return(FALSE);
		}
		wd->curlen++;
	}
	return(TRUE);
}

UBYTE INLINE ReadDataFtr_I(WorkData *wd,UBYTE crunch,UBYTE conv) {
	register UBYTE i;
	ULONG src_tblkbytes,dst_tblkbytes,tblksamples;

	src_tblkbytes=wd->ch_bytes-(wd->blocs*wd->src_blkbytes);					// remaining bytes in stream
	dst_tblkbytes=(ULONG)(src_tblkbytes*((double)wd->dst_blkbytes/(double)wd->src_blkbytes));		// how many bytes will come out of this
	tblksamples=Bytes2Samples(dst_tblkbytes);									// and how many samples
	MSG3("  src_tblkbytes %6ld  dst_tblkbytes  %6ld  tblksamples %6ld",src_tblkbytes,dst_tblkbytes,tblksamples);
	if(!LIB_INTCALL(ASRead(&wd->sar,0L))) return(FALSE);									// don't care about the length, we're just fetching the last bloc
	SplitChannels(wd->sbuf1,wd->sbuf2,src_tblkbytes,wd->ch,wd->ch_blkbytes);
	for(i=0;i<wd->ch;i++) {														// for each channel
		wd->dbuf=(WORD *)wd->saw[i]->buffer1;
		if(conv&CONV_ENDIAN_F) {
			switch(wd->ch_blkbytes) {
				case 2: ConvREndian16(wd->sbuf2[i],tblksamples);break;	// 16 bit
				case 4: ConvREndian32(wd->sbuf2[i],tblksamples);break;	// 32 bit
				case 8: ConvREndian64(wd->sbuf2[i],tblksamples);break;	// 64 bit
			}
		}
		Decrunch(wd->dbuf,wd->sbuf2[i],crunch,&wd->codecvals[i],tblksamples);
		if(conv&CONV_SIGNUM_F) ConvRSignum(wd->dbuf,tblksamples);
		if(!wd->SAWrite[i](wd->saw[i],dst_tblkbytes)) return(FALSE);
	}
	wd->curlen++;
	return(TRUE);
}

//-- loader tools

ULONG SAVEDS ASM LIB_EXPORT(GetDecrunchedLength(REG(d0,UBYTE crunch),REG(d1,ULONG src_bytes))) {
	ULONG dst_bytes=0;

	switch(crunch) {
		case CRUNCH_PCM32:			dst_bytes=   src_bytes	 /2.00;		break;
		case CRUNCH_PCM24:			dst_bytes=   src_bytes	 /1.50;		break;
		case CRUNCH_PCM16:			dst_bytes=   src_bytes;				break;
		case CRUNCH_PCM12:			dst_bytes=   src_bytes   /0.75;		break;
		case CRUNCH_PCM8:
		case CRUNCH_ULAW:
		case CRUNCH_ULAW_INV:
		case CRUNCH_ALAW:
		case CRUNCH_ALAW_INV:		dst_bytes=   src_bytes   /0.5;		break;
		case CRUNCH_IEEE64:			dst_bytes=   src_bytes	 /4.00;		break;
		case CRUNCH_IEEE32:			dst_bytes=   src_bytes	 /2.00;		break;
		case CRUNCH_FDPCM8_4:		dst_bytes=4+(src_bytes-2)/0.25;		break;
		case CRUNCH_FDPCM16_6:		dst_bytes=2+(src_bytes-2)/0.375;	break;
		case CRUNCH_EDPCM8_4:		dst_bytes=4+(src_bytes-2)/0.25;		break;
		case CRUNCH_EDPCM16_5:		dst_bytes=2+(src_bytes-2)/0.3125;	break;
		case CRUNCH_VOC_ADPCM8_4:	dst_bytes=4+(src_bytes-2)/0.25;		break;
		case CRUNCH_VOC_ADPCM8_3:	dst_bytes=4+(src_bytes-2)/0.1875;	break;
		case CRUNCH_VOC_ADPCM8_2:	dst_bytes=4+(src_bytes-2)/0.125;	break;
		// new
		case CRUNCH_MS_ADPCM16_4:	dst_bytes=4+(src_bytes-2)/0.125;	break;
		case CRUNCH_IMA_ADPCM:			break;
		case CRUNCH_DVI_ADPCM:			break;
	}
	return(dst_bytes);
}


ULONG SAVEDS ASM LIB_EXPORT(ReadData(REG(a0,SInfo *si),REG(a1,LoadData *ldata),REG(d0,ULONG src_bytes),REG(d1,UBYTE crunch),REG(d2,UBYTE conv),REG(d3,UBYTE channel))) {
	register UBYTE i;				// channels
	UBYTE fail=FALSE;
	ULONG samples=0;

	WorkData *wd;

	ProWinTitle title;
	PWData *pwd;

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(ldata,__FUNC__,"2.par",4));

	if(wd=ReadDataInit(si,ldata,src_bytes,crunch,channel)) {
		if(pwd=LIB_INTCALL(NewProWin())) {

			samples=wd->samples;	   // how many samples will we get in the end

			if(!(channel&CHANNEL_INTERLEAVED)) {
				MSG("  ## non interleaved");
				for(i=0;(i<wd->ch && !fail);i++) {													   // for each channel
					if((1<<i)&ldata->chmask) {												// which is active
						if(ldata->flags&LDF_NOCHOFF) wd->sar.start=ldata->offs;
						else wd->sar.start=ldata->offs+(i*(src_bytes/wd->ch));

						if(!LIB_INTCALL(ASOpen(&wd->sar,ldata->fn,O_RDONLY))) {
							if((wd->saw[0]=LIB_INTCALL(SAInit(si,i,0,samples,VM_MODE_WRITE|VM_MODE_BLK|VM_MODE_VBLK,VM_DEFAULT_BLKLEN))))	{
								wd->SAWrite[0]=wd->saw[0]->safunc;
								wd->dbuf=(WORD *)wd->saw[0]->buffer1;
								MSG1("    after SAInit[%d]",i);
								fail=!ReadDataHdr_NI(wd,crunch,conv,ldata->codecdata);
								MSG1("    after header[%d]",i);
								if(wd->blocs && !fail) {
									sprintf(title,LIB_INTCALL(GetString(msgProgressLoad)),i+1,wd->ch);
									LIB_INTCALL(StartProWin(pwd,&wd->curlen,title,wd->blocs+wd->rest));					 // den Statusbalken starten
									fail=!ReadDataSub_NI(wd,crunch,conv);
								}
								MSG1("    after main[%d]",i);
								if(wd->rest && !fail) {
									fail=!ReadDataFtr_NI(wd,crunch,conv);
								}
								MSG1("    after rest[%d]",i);
								// CHANGE
								//seek(sar->fh,...)	for multichannelsamples
								// CHANGE
								LIB_INTCALL(SADone(wd->saw[0],si));
							}
							LIB_INTCALL(ASClose(&wd->sar));
						}
						else { samples=0;LIB_INTCALL(StdCError(wd->sar.fn,errOpenFile,wd->sar.last_error_no,__FILE__,__LINE__)); }
					}
				}
			}
			else {
				MSG("  ## interleaved");
				wd->sar.start=ldata->offs;
				if(!LIB_INTCALL(ASOpen(&wd->sar,ldata->fn,O_RDONLY))) {
					for(i=0;i<wd->ch;i++) {
						if((wd->saw[i]=LIB_INTCALL(SAInit(si,i,0,samples,VM_MODE_WRITE|VM_MODE_BLK|VM_MODE_VBLK,VM_DEFAULT_BLKLEN)))) wd->SAWrite[i]=wd->saw[i]->safunc;
						else fail=TRUE;
					}
					if(!fail) {
						MSG("  after SAInit");
						fail=!ReadDataHdr_I(wd,crunch,conv,ldata->codecdata);
						MSG("    after header");
						if(wd->blocs && !fail) {
							sprintf(title,LIB_INTCALL(GetString(msgProgressLoad)),1,1);
							LIB_INTCALL(StartProWin(pwd,&wd->curlen,title,wd->blocs+wd->rest));					 // den Statusbalken starten
							fail=!ReadDataSub_I(wd,crunch,conv);
						}
						MSG("    after main");
						if(wd->rest && !fail) {
							fail=!ReadDataFtr_I(wd,crunch,conv);
						}
						MSG("    after rest");
					}
					else samples=0;
					for(i=0;i<wd->ch;i++) { if(wd->saw[i]) LIB_INTCALL(SADone(wd->saw[i],si)); }
					LIB_INTCALL(ASClose(&wd->sar));
				}
				else { samples=0;LIB_INTCALL(StdCError(wd->sar.fn,errOpenFile,wd->sar.last_error_no,__FILE__,__LINE__)); }
			}
			MSG("  now freeing resources");
			LIB_INTCALL(DelProWin(pwd));
		}
		ReadDataDone(wd);
	}
	else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));

	//MSG1("si->slen%ld",si->slen);
	//MSG1("sample=%ld",samples);
	OUTRO;
	return(samples);
}

ULONG SAVEDS ASM LIB_EXPORT(GetFileLength(REG(a0,STRPTR fn))) {
	struct FileLock *fl;
	struct FileInfoBlock fib;
	ULONG slen=0;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(fn,__FUNC__,"1.par",4));

	if(fl=(struct FileLock *)Lock(fn,ACCESS_READ)) {
		if(Examine((BPTR)fl,&fib)) slen=fib.fib_Size;
		UnLock((BPTR)fl);
	}
	//slen&=0xFFFFFFFE;		/* make odd */
	return(slen);
}

//-- eof ----------------------------------------------------------------------
