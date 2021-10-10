/******************************************************************************
** SFX-FileDefs.h                                                            **
**---------------------------------------------------------------------------**
** SFX FileFormatDefinitions                                                 **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 06.01.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#define WAV_RIFF		MakeID('R','I','F','F')
#define WAV_WAVE		MakeID('W','A','V','E')
#define WAV_fmt_		MakeID('f','m','t',' ')
#define WAV_data		MakeID('d','a','t','a')

struct WaveFmt						/* common fields */
{
	UWORD	wFormatTag;
	UWORD	wChannels;
	ULONG	dwSamplesPerSec;
	ULONG	dwAvgBytesPerSec;
	UWORD	wBlockAlign;
	UWORD	wBitsPerSample;
};

/* format tags */

#define FORMAT_WAV_PCM16		 1

/* possible values for wFormatTag */

#define WAVE_FORMAT_PCM						0x0001		/* Microsoft Corporation */

/* possible bitrates */

#define BITRATE_MPEG_32		 0
#define BITRATE_MPEG_40		 1
#define BITRATE_MPEG_48		 2
#define BITRATE_MPEG_56		 3
#define BITRATE_MPEG_64		 4
#define BITRATE_MPEG_80		 5
#define BITRATE_MPEG_96		 6
#define BITRATE_MPEG_112	 7
#define BITRATE_MPEG_128	 8
#define BITRATE_MPEG_160	 9
#define BITRATE_MPEG_192	10
#define BITRATE_MPEG_224	11
#define BITRATE_MPEG_256	12
#define BITRATE_MPEG_320	13
