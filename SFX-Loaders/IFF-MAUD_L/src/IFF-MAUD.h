/******************************************************************************
** SFX-FileDefs.h                                                            **
**---------------------------------------------------------------------------**
** SFX FileFormatDefinitions                                                 **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 06.01.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#define IFF_FORM		MakeID('F','O','R','M')
#define IFF_MAUD		MakeID('M','A','U','D')
#define IFF_MHDR		MakeID('M','H','D','R')
#define IFF_MDAT		MakeID('M','D','A','T')
#define IFF_MINF		MakeID('M','I','N','F')
#define IFF_NAME		MakeID('N','A','M','E')
#define IFF_COPY		MakeID('(','c',')',' ')
#define IFF_AUTH		MakeID('A','U','T','H')
#define IFF_ANNO		MakeID('A','N','N','O')

struct MaudHeader {
	ULONG	Samples;		/* number of samples stored in MDAT */
	UWORD	SampleSizeC;	/* number of bits per sample as stored in MDAT */
	UWORD	SampleSizeU;	/* number of bits per sample after decompression */
	ULONG	RateSource;	/* clock source frequency (see maud.doc) */
	UWORD	RateDevide;	/* clock devide           (see maud.doc) */
	UWORD	ChannelInfo;	/* channel information (see below) */
	UWORD	Channels;		/* number of channels (mono: 1, stereo: 2, ...) */
	UWORD	Compression;	/* compression type (see below) */
	ULONG	Reserved1;		/* MUST be set to 0 when saving */
	ULONG	Reserved2;		/* MUST be set to 0 when saving */
	ULONG	Reserved3;		/* MUST be set to 0 when saving */
};

/* format tags */

#define FORMAT_MAUD_PCM8		 0
#define FORMAT_MAUD_PCM16		 1
#define FORMAT_MAUD_PCM24		 2
#define FORMAT_MAUD_PCM32		 3
#define FORMAT_MAUD_FDPCM8_4	 4
#define FORMAT_MAUD_ULAW		 5
#define FORMAT_MAUD_ALAW		 6
#define FORMAT_MAUD_ADPCM16_5	 7
#define FORMAT_MAUD_ADPCM16_4	 8
#define FORMAT_MAUD_ADPCM16_3	 9
#define FORMAT_MAUD_ADPCM16_2	10
#define FORMAT_MAUD_LONGDAT		11

/* possible values for mhdr_ChannelInfo */

#define MCI_MONO			0	/* mono */
#define MCI_STEREO			1	/* stereo */
#define MCI_MULTIMONO		2	/* mono multichannel (channels can be 2, 3, 4, ...) */
#define MCI_MULTISTEREO		3	/* stereo multichannel (channels must be 4, 6, 8, ...) */
#define MCI_MULTICHANNEL	4	/* multichannel (requires additional MINF-chunks) (future) */

/* possible values for mhdr_Compression */

#define COMPMAUD_PCM		0	/* no compression */
#define COMPMAUD_FDPCM		1	/* 'Fibonacci Delta Compression' as used in 8SVX */
#define COMPMAUD_ALAW		2	/* 16->8 bit, European PCM standard A-Law */
#define COMPMAUD_ULAW		3	/* 16->8 bit, American PCM standard µ-Law */
#define COMPMAUD_ADPCM2		4	/* 16->2 bit, ADPCM compression */
#define COMPMAUD_ADPCM3		5	/* 16->3 bit, ADPCM compression */
#define COMPMAUD_ADPCM4		6	/* 16->4 bit, ADPCM compression */
#define COMPMAUD_ADPCM5		7	/* 16->5 bit, ADPCM compression */
#define COMPMAUD_LONGDAT	8	/* 16->12 bit, used for DAT-longplay */
