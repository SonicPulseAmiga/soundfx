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
#define IFF_AIFF		MakeID('A','I','F','F')
#define IFF_COMM		MakeID('C','O','M','M')
#define IFF_SSND		MakeID('S','S','N','D')
#define IFF_COMT		MakeID('C','O','M','T')
#define IFF_NAME		MakeID('N','A','M','E')
#define IFF_COPY		MakeID('(','c',')',' ')
#define IFF_AUTH		MakeID('A','U','T','H')
#define IFF_ANNO		MakeID('A','N','N','O')
#define IFF_APPL		MakeID('A','P','P','L')

struct CommonChunk {
	UWORD	numChannels;
	ULONG	numSampleFrames;
	UWORD	sampleSize;
	UBYTE	sampleRate[10];		/* typed as Extended in official documentation */
};

struct SampledSoundHeader {
	ULONG	offset; 			// Offset to sound data, for block alignment
	ULONG	blockSize;			// Size of block data is aligned to
};

/* format tags */

#define FORMAT_AIFF_PCM8	0
#define FORMAT_AIFF_PCM16	1
#define FORMAT_AIFF_PCM24	2
#define FORMAT_AIFF_PCM32	3
