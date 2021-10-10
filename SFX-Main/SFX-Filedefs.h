/******************************************************************************
** SFX-FileDefs.h                                                            **
**---------------------------------------------------------------------------**
** SFX FileFormatDefinitions                                                 **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 06.01.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#ifndef SFX_FILEDEFS_H
#define SFX_FILEDEFS_H

#define TYPE	chunktype.type_long
#define STRTYPE	chunktype.type_str

// endian conversions

#define LitEnd2BigEnd_uword(w)	( (w&0xFF)<<8 | (w&0xFF00)>>8 )
#define LitEnd2BigEnd_ulong(l)	( (l&0xFF)<<24 | (l&0xFF00)<<8 | (l&0xFF0000)>>8 | (l&0xFF000000)>>24 )

union typekludge
{
	char type_str[4];
	long type_long;
};

struct ChunkHeader
{
	union typekludge chunktype;
	long chunksize;
};

#define WriteChunkHeader(type,size,file)	\
{											\
	struct ChunkHeader ch;					\
	ch.TYPE=type;							\
	ch.chunksize=size;						\
	if(!fwrite(&ch,sizeof(struct ChunkHeader),1,file)) { Message(errWriteData,0l,__FILE__,__LINE__);fclose(file);return(0); }	\
}

/* Defines */

#define ValidChunkID(str) (isprint(str[0]) && isprint(str[1]) && isprint(str[2]) && isprint(str[3]))

#define CONV_SIGNUM_B	0
#define CONV_SIGNUM_F	(1L<<CONV_SIGNUM_B)
#define CONV_ENDIAN_B	1
#define CONV_ENDIAN_F	(1L<<CONV_ENDIAN_B)

#define CONV(endian,signum) ((UBYTE)((endian<<CONV_ENDIAN_B)|(signum<<CONV_SIGNUM_B)))

#define ENDIAN_L_B		1			/* für Load */
#define ENDIAN_B_L		1			/* für Save */

#define SIGNUM_U_S		1			/* für Load */
#define SIGNUM_S_U		1			/* für Save */

#define CRUNCH_PCM16			 0		/* 16bit */
#define CRUNCH_PCM8				 1		/* Quantisierung 8bit */
#define CRUNCH_FDPCM8_4			 2		/* FibonacciDelta 2:1 (8bit) */
#define CRUNCH_FDPCM16_6		 3		/* FibonacciDelta 8:3 (16bit) */
#define CRUNCH_EDPCM8_4			 4		/* ExponentialDelta 2:1 (8bit) */
#define CRUNCH_EDPCM16_5		 5		/* ExponentialDelta 16:5 (16bit) */
#define CRUNCH_ULAW				 6		/* µLaw - Kompression ca.12bit:8bit */
#define CRUNCH_ALAW				 7		/* ALaw - Kompression */
#define CRUNCH_VOC_ADPCM8_4		 8		/* AdaptiveDeltaPCM 2:1 */
#define CRUNCH_VOC_ADPCM8_3		 9		/* AdaptiveDeltaPCM 8:3 */
#define CRUNCH_VOC_ADPCM8_2		10		/* AdaptiveDeltaPCM 4:1 */
#define CRUNCH_VOC_ADPCM16_4	11		/* AdaptiveDeltaPCM 16:4 */
#define CRUNCH_PCM12			12		/* Quantisierung 12bit */
#define CRUNCH_ALAW_INV			13		/* ALaw - Kompression mit Bitinvertierung */
#define CRUNCH_ULAW_INV			14		/* µLaw - Kompression mit Bitinvertierung */
#define CRUNCH_PCM24			15		/* Quantisierung 24bit */
#define CRUNCH_PCM32			16		/* Quantisierung 32bit */
#define CRUNCH_IEEE32			17		/* IEEE 32bit */
#define CRUNCH_IEEE64			18		/* IEEE 64bit */
#define CRUNCH_MS_ADPCM16_4		19		/* AdaptiveDeltaPCM 16:4 */
#define CRUNCH_IMA_ADPCM		20		/* AdaptiveDeltaPCM */
#define CRUNCH_DVI_ADPCM		21		/* AdaptiveDeltaPCM */

#define STR_PCM32				"PCM 32"
#define STR_PCM24				"PCM 24"
#define STR_PCM16				"PCM 16"
#define STR_PCM12				"PCM 12"
#define STR_PCM8				"PCM 8"
#define STR_PCM16c				"PCM 16 comb"
#define STR_FDPCM8_4			"FDPCM 8:4"
#define STR_FDPCM16_6			"FDPCM 16:6"
#define STR_EDPCM8_4			"EDPCM 8:4"
#define STR_EDPCM16_5			"EDPCM 16:5"
#define STR_ULAW				"µ-Law"
#define STR_ULAW_INV			"µ-Law inv"
#define STR_ALAW				"A-Law"
#define STR_ALAW_INV			"A-Law inv"
#define STR_VOC_ADPCM8_4		"VOC ADPCM 8:4"
#define STR_VOC_ADPCM8_3		"VOC ADPCM 8:3"
#define STR_VOC_ADPCM8_2		"VOC ADPCM 8:2"
#define STR_VOC_ADPCM16_4		"VOC ADPCM 16:4"
#define STR_IEEE32				"IEEE 32"
#define STR_IEEE64				"IEEE 64"
#define STR_MS_ADPCM			"MS ADPCM"
#define STR_IMA_ADPCM			"IMA ADPCM"
#define STR_DVI_ADPCM			"DVI ADPCM"

#define CHANNEL_INTERLEAVED 1

#define CHANNEL_MONO	0
#define CHANNEL_STEREO	2
#define CHANNEL_STEREOI (CHANNEL_STEREO|CHANNEL_INTERLEAVED)
#define CHANNEL_QUADRO	4
#define CHANNEL_QUADROI (CHANNEL_QUADRO|CHANNEL_INTERLEAVED)

/* Mac-Resource ----------------------------------------------------------------------------------- */
//
//struct MacResMain
//{
//	UWORD	zero;
//	ULONG	datalen;
//	UWORD	type;
//};
//
//struct MacResHdr1
//{
//	UWORD	unknown01_w;		/* 0001										*/
//	UWORD	unknown02_w;		/* 0005										*/
//	ULONG	unknown03_l;		/* 0000 0080 , 0000 00A0					*/
//	ULONG	unknown04_l;		/* 0001 8051 -> 98385						*/
//	ULONG	unknown05_l;		/* 0000 0000								*/
//	ULONG	unknown06_l;		/* 0014 0000 -> 20 bzw. 1310720				*/
//	ULONG	unknown07_l;		/* 0000 0000 , 0000 0001					*/
//	UWORD	lensub1;			/* datalen-42								*/
//	UWORD	rate;				/* 56EE -> 22254 , 2B77 -> 11127			*/
//	UWORD	unknown08_w;		/* 8BA3 bei rate=56EE , 45D1 bei rate=2B77	*/
//	UWORD	unknown09_w;		/* 0000										*/
//	UWORD	lensub3;			/* datalen-44 , 0000						*/
//	UWORD	unknown10_w;		/* 0000										*/
//	UWORD	lensub2;			/* datalen-43 , 0000						*/
//	UWORD	end_of_data;		/* 003C -> 60								*/
//};

//struct MacResHdr2
//{
//	ULONG	unknown01_l;		/* 0000 0001								*/
//	ULONG	unknown02_l;		/* 8050 0000 -> 32848 bzw. 2152762528		*/
//	ULONG	unknown03_l;		/* 0000 000E								*/
//	ULONG	unknown04_l;		/* 0000 0000								*/
//	UWORD	lensub1;			/* datalen-36								*/
//	UWORD	rate;				/* 56EE -> 22254 , 2B77 -> 11127			*/
//	UWORD	unknown08_w;		/* 8BA3 bei rate=56EE , 45D1 bei rate=2B77	*/
//	UWORD	unknown09_w;		/* 0000										*/
//	UWORD	lensub3;			/* datalen-38 , 0000						*/
//	UWORD	unknown10_w;		/* 0000										*/
//	UWORD	lensub2;			/* datalen-39 , 0000						*/
//	UWORD	end_of_data;		/* 0048 -> 72								*/
//};

/* eof -------------------------------------------------------------------------------------------- */

#endif	/* SFX_FILEDEFS_H */
