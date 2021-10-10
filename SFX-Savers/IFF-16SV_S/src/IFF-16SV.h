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
#define IFF_16SV		MakeID('1','6','S','V')
#define IFF_VHDR		MakeID('V','H','D','R')
#define IFF_NAME		MakeID('N','A','M','E')
#define IFF_COPY		MakeID('(','c',')',' ')
#define IFF_AUTH		MakeID('A','U','T','H')
#define IFF_ANNO		MakeID('A','N','N','O')
#define IFF_ATAK		MakeID('A','T','A','K')
#define IFF_RLSE		MakeID('R','L','S','E')
#define IFF_CHAN		MakeID('C','H','A','N')
#define IFF_BITS		MakeID('B','I','T','S')
#define IFF_BODY		MakeID('B','O','D','Y')
#define IFF_LBDY		MakeID('L','B','D','Y')

struct Voice8Header
{
	ULONG	oneShotHiSamples;	/* # samples in the high octave 1-shot part */
	ULONG	repeatHiSamples;	/* # samples in the high octave repeat part */
	ULONG	samplesPerHiCycle;	/* # samples/cycle in high octave, else 0 */
	UWORD	samplesPerSec;		/* data sampling rate */
	UBYTE	ctOctave;			/* # of octaves of waveforms */
	UBYTE	sCompression;		/* data compression technique used */
	LONG	volume;				/* playback nominal volume from 0 to Unity */
};

/* format tags */

#define FORMAT_16SV_PCM16		0
#define FORMAT_16SV_FDPCM16_6	1
#define FORMAT_16SV_EDPCM16_5	2

/* possible values for sCompression */

#define COMP16SV_PCM	0
#define COMP16SV_FDPCM	1
#define COMP16SV_EDPCM	2
