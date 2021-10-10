/******************************************************************************
** SFX-FileDefs.h                                                            **
**---------------------------------------------------------------------------**
** SFX FileFormatDefinitions                                                 **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 06.01.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#define SUN__SND		MakeID('.','s','n','d')
#define SUN_ISND		MakeID('d','n','s','.')
#define SUN__DEC		MakeID('.','s','n','\0')
#define SUN_IDEC		MakeID('\0','n','s','.')

struct SunHeader
{
	ULONG	id;
	ULONG	hdrlen;
	ULONG	datalen;
	ULONG	format;
	ULONG	rate;
	ULONG	channels;
};

/* format tags */

#define FORMAT_SND_PCM8		0
#define FORMAT_SND_PCM16	1
#define FORMAT_SND_PCM24	2
#define FORMAT_SND_PCM32	3
#define FORMAT_SND_ULAW		4
#define FORMAT_SND_ALAW		5
#define FORMAT_SND_IEEE32	6
#define FORMAT_SND_IEEE64	7

/* herder tags */

#define HEADER__SND			0
#define HEADER__DEC			1
#define HEADER_ISND			2
#define HEADER_IDEC			3

/* possible value for length */

#define SND_LENGHT_ILLEGAL 0xFFFFFFFF		/* unbekannte länge -> filelen-headerlen */

/* possible values for format */

#define SND_FORMAT_UNSPECIFIED				 0	/* unspecified format */
#define SND_FORMAT_MULAW_8					 1	/* 8-bit mu-law samples */
#define SND_FORMAT_LINEAR_8					 2	/* 8-bit linear samples */
#define SND_FORMAT_LINEAR_16				 3	/* 16-bit linear samples */
#define SND_FORMAT_LINEAR_24				 4	/* 24-bit linear samples */
#define SND_FORMAT_LINEAR_32				 5	/* 32-bit linear samples */
#define SND_FORMAT_FLOAT					 6	/* IEEE FP 32 bits */
#define SND_FORMAT_DOUBLE					 7	/* IEEE FP 64 bits */
#define SND_FORMAT_INDIRECT					 8	/* fragmented sampled data */
#define SND_FORMAT_NESTED					 9	/* ? */
#define SND_FORMAT_DSP_CORE					10	/* DSP program */
#define SND_FORMAT_DSP_DATA_8				11	/* 8-bit fixed-point samples */
#define SND_FORMAT_DSP_DATA_16				12	/* 16-bit fixed-point samples */
#define SND_FORMAT_DSP_DATA_24				13	/* 24-bit fixed-point samples */
#define SND_FORMAT_DSP_DATA_32				14	/* 32-bit fixed-point samples */
/*		SND_FORMAT_??						15     ? */
#define SND_FORMAT_DISPLAY					16	/* non-audio display data */
#define SND_FORMAT_MULAW_SQUELCH			17	/* ? */
#define SND_FORMAT_EMPHASIZED				18	/* 16-bit linear with emphasis */
#define SND_FORMAT_COMPRESSED				19	/* 16-bit linear with compression */
#define SND_FORMAT_COMPRESSED_EMPHASIZED	20	/* A combination of the two above */
#define SND_FORMAT_DSP_COMMANDS				21	/* Music Kit DSP commands */
#define SND_FORMAT_DSP_COMMANDS_SAMPLES		22	/* ? */
#define SND_FORMAT_ADPCM_G721				23	/* CCITT G.721 4-bits ADPCM */				/* * */
#define SND_FORMAT_ADPCM_G722				24	/*   */										/* * */
#define SND_FORMAT_ADPCM_G723_3				25	/* CCITT G.723 3-bits ADPCM  */				/* * */
#define SND_FORMAT_ADPCM_G723_5				26	/* CCITT G.723 5-bits ADPCM  */				/* * */
#define SND_FORMAT_ALAW_8					27	/* 8-bit a-law sample */
