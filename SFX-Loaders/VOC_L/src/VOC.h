/******************************************************************************
** SFX-FileDefs.h                                                            **
**---------------------------------------------------------------------------**
** SFX FileFormatDefinitions                                                 **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 06.01.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#define VOC_CREA		MakeID('C','r','e','a')

struct VocHdr
{
	char	ID[20];
	UWORD 	BlockLength;
	UBYTE	Version_LSB;
	UBYTE	Version_MSB;
	UBYTE	ChkVersion_LSB;				/* 0x1233-Version */
	UBYTE	ChkVersion_MSB;
};

#define VOC_TERMINATOR		 0
#define VOC_DATA			 1
#define VOC_CONTINUATION	 2
#define VOC_SILENCE			 3
#define VOC_MARKER			 4
#define VOC_ASCII			 5
#define VOC_LOOPSTART		 6
#define VOC_LOOPEND			 7
#define VOC_EXTENDED		 8
#define VOC_NEWDATA			 9

struct VocBlk1							/* Datablock + Info */
{
	UBYTE size[3];
	UBYTE tc;
	UBYTE pack;
};

#define VocBlk1Size	(sizeof(struct VocBlk1)-3)

struct VocBlk2							/* Datablock */
{
	UBYTE size[3];
};

#define VocBlk2Size	(sizeof(struct VocBlk2)-3)

struct VocBlk3							/* Silence */
{
	UBYTE size[3];
	UWORD period;
	UBYTE tc;
};

#define VocBlk3Size	(sizeof(struct VocBlk3)-3)

struct VocBlk4							/* Marker */
{
	UBYTE size[3];
	UWORD marker;
};

#define VocBlk4Size	(sizeof(struct VocBlk4)-3)

struct VocBlk5							/* Ascii-Block */
{
	UBYTE size[3];
};

#define VocBlk5Size	(sizeof(struct VocBlk5)-3)

struct VocBlk6							/* Start Loop Block */
{
	UBYTE size[3];
	UWORD repeat;
};

#define VocBlk6Size	(sizeof(struct VocBlk6)-3)

struct VocBlk7							/* Repeat loop Block */
{
	UBYTE size[3];
};

#define VocBlk7Size	(sizeof(struct VocBlk7)-3)

struct VocBlk8							/* Ext Infoblock */
{
	UBYTE size[3];
	UWORD tc;
	UBYTE pack;
	UBYTE mode;
};

#define VocBlk8Size	(sizeof(struct VocBlk8)-3)

struct VocBlk9							/* Ext Data + Info Block */
{
	UBYTE size[3];
	ULONG frequency;
	UBYTE width;
	UBYTE channel;
	ULONG pack;
	UBYTE reserved[4];
};

#define VocBlk9Size	(sizeof(struct VocBlk9)-3)

/* format tags */

#define FORMAT_VOC_PCM8			 0
#define FORMAT_VOC_PCM16		 1
#define FORMAT_VOC_ADPCM8_4		 2
#define FORMAT_VOC_ADPCM8_3		 3
#define FORMAT_VOC_ADPCM8_2		 4
#define FORMAT_VOC_ULAW			 5
#define FORMAT_VOC_ALAW			 6
#define FORMAT_VOC_ADPCM16_4	 7

/* version tags */

#define VERSION_1_10	0
#define VERSION_1_15	1
#define VERSION_1_20	2

/* possible values for pack */

#define COMPVOC_PCM8		0x0			/* 8-bit unsigned */
#define COMPVOC_ADPCM8_4	0x1			/* 8-bit to 4-bit ADPCM */
#define COMPVOC_ADPCM8_3	0x2			/* 8-bit to 3-bit ADPCM */
#define COMPVOC_ADPCM8_2	0x3			/* 8-bit to 2-bit ADPCM */
#define COMPVOC_PCM16		0x4			/* 16-bit signed */
#define COMPVOC_ALAW		0x5
#define COMPVOC_ULAW		0x6
#define COMPVOC_ADPCM16_4	0x2000		/* 16-bit to 4-bit ADPCM */
