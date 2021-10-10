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
#define IFF_AIFC		MakeID('A','I','F','C')
#define IFF_FVER		MakeID('F','V','E','R')
#define IFF_MARK		MakeID('M','A','R','K')
#define IFF_INST		MakeID('I','N','S','T')
#define IFF_COMM		MakeID('C','O','M','M')
#define IFF_SSND		MakeID('S','S','N','D')
#define IFF_COMT		MakeID('C','O','M','T')
#define IFF_NAME		MakeID('N','A','M','E')
#define IFF_COPY		MakeID('(','c',')',' ')
#define IFF_AUTH		MakeID('A','U','T','H')
#define IFF_ANNO		MakeID('A','N','N','O')
#define IFF_APPL		MakeID('A','P','P','L')

#define AIFCVersion1 0xA2805140 					// "AIFC" file format version #1

struct ExtCommonChunk {
	UWORD				numChannels;		// Number of channels
	ULONG				numSampleFrames;	// Number of sample frames
	UWORD				sampleSize; 		// Number of bits per sample point
	UBYTE				sampleRate[10];		// Replay rate in samples per second
	LONG				compressionType;	// Compression type
};

struct SampledSoundHeader {
	ULONG	offset; 			// Offset to sound data, for block alignment
	ULONG	blockSize;			// Size of block data is aligned to
};

struct Marker {
	UWORD	id;
	ULONG	pos;
	char	*name;
};

struct Instrument {
	UBYTE	note;
	UBYTE	detune;
	UBYTE	lownote;
	UBYTE	highnote;
	UBYTE	lowvelo;
	UBYTE	highvelo;
	UWORD	gain;
	UWORD	susloopmode,susloopmark1,susloopmark2;
	UWORD	relloopmode,relloopmark1,relloopmark2;
};

#define NO_COMPRESSION	MakeID('N','O','N','E') 	// No sound compression									"not compressed"
#define COMP_ACE2		MakeID('A','C','E','2') 	// 2-to-1 IIGS ACE (Audio Compression/Expansion)		"ACE 2-to-1"
#define COMP_ACE8		MakeID('A','C','E','8') 	// 8-to-3 IIGS ACE (Audio Compression/Expansion)		"ACE 8-to-3"
#define COMP_MAC3		MakeID('M','A','C','3') 	// 3-to-1 MACE (Macintosh Audio Compression/Expansion)	"MACE 3-to-1"
#define COMP_MAC6		MakeID('M','A','C','6') 	// 6-to-1 MACE (Macintosh Audio Compression/Expansion)	"MACE 6-to-1"
#define COMP_DWVW		MakeID('D','W','V','W') 	// Delta With Word Variable Width
#define COMP_ALAW		MakeID('a','l','a','w') 	// A-Law												"ALaw 2:1"
#define COMP_ULAW		MakeID('u','l','a','w') 	// µ-Law												"µLaw 2:1"

#define LOOP_NONE		0
#define LOOP_FORWARD	1
#define LOOP_REVERSED	2

/* format tags */

#define FORMAT_AIFC_PCM8	0
#define FORMAT_AIFC_PCM16	1
#define FORMAT_AIFC_PCM24	2
#define FORMAT_AIFC_PCM32	3
#define FORMAT_AIFC_ALAW	4
#define FORMAT_AIFC_ULAW    5

#define FORMAT_AIFC_ACE2	6		// not yet supportet
#define FORMAT_AIFC_ACE8	7
#define FORMAT_AIFC_MAC3	8
#define FORMAT_AIFC_MAC6	9
#define FORMAT_AIFC_DWVW	10
