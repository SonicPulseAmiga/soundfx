/****h* RIFF-WAV/RIFF-WAV.h [4.3] *
*
*  NAME
*    RIFF-WAV.h
*  COPYRIGHT
*    $VER: RIFF-WAV.c 4.3 (21.02.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    RIFF-WAV fileformat definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    21.Feb.2004	V 4.3	most recent version
*    18.Jun.2000	V 1.20	most recent version
*    15.Sep.1998	V 1.10	most recent version
*    02.Jul.1997	V 1.00	initial version
*  NOTES
*
*******
*/

#define WAV_RIFF		MakeID('R','I','F','F')
#define WAV_WAVE		MakeID('W','A','V','E')
#define WAV_fmt_		MakeID('f','m','t',' ')
#define WAV_fact		MakeID('f','a','c','t')
#define WAV_smpl		MakeID('s','m','p','l')
#define WAV_inst		MakeID('i','n','s','t')
#define WAV_data		MakeID('d','a','t','a')

#define WAV_LIST		MakeID('L','I','S','T')
#define WAV_INFO		MakeID('I','N','F','O')
#define WAV_IARL		MakeID('I','A','R','L')			// archive location
#define WAV_IART		MakeID('I','A','R','T')			// artist
#define WAV_ICMT		MakeID('I','C','M','T')			// general comment
#define WAV_ICOP		MakeID('I','C','O','P')			// copyright
#define WAV_ICRD		MakeID('I','C','R','D')			// creation date
#define WAV_IDIT		MakeID('I','D','I','T')			// digitization time
#define WAV_IENG		MakeID('I','E','N','G')			// engineer
#define WAV_IGNR		MakeID('I','G','N','R')			// genere
#define WAV_IKEY		MakeID('I','K','E','Y')			// keywords
#define WAV_IMED		MakeID('I','M','E','D')			// original medium
#define WAV_INAM		MakeID('I','N','A','M')			// name
#define WAV_IPRD		MakeID('I','P','R','D')			// product
#define WAV_ISBJ		MakeID('I','S','B','J')			// subject
#define WAV_ISFT		MakeID('I','S','F','T')			// software
#define WAV_ISMT		MakeID('I','S','M','T')			// SMPTE time code
#define WAV_ISRC		MakeID('I','S','R','C')			// source
#define WAV_ISRF		MakeID('I','S','R','F')			// source form (like IMED)
#define WAV_ITCH		MakeID('I','T','C','H')			// technician

struct WaveFmt						/* common fields */
{
	UWORD	wFormatTag;
	UWORD	wChannels;
	ULONG	dwSamplesPerSec;
	ULONG	dwAvgBytesPerSec;
	UWORD	wBlockAlign;
	UWORD	wBitsPerSample;
};

struct WaveFmt_ADPCM				/* for WAVE_FORMAT_ADPCM */
{
	UWORD	wSamplesPerBlock;
	UWORD	wNumCoef;
};

struct WaveFmt_ADPCM_Coeff			/* for WAVE_FORMAT_ADPCM */
{
	WORD	iCoef1;
	WORD	iCoef2;
};

struct WaveFmt_DVIIMA_ADPCM			/* for WAVE_FORMAT_DVI_ADPCM,WAVE_FORMAT_IMA_ADPCM */
{
	UWORD	wSamplesPerBlock;
};

struct WaveFmt_SONARC				/* for WAVE_FORMAT_SONARC */
{
	UWORD	wCompType;
};

struct WaveFmt_DSPGROUP_TRUESPEECH	/* for WAVE_FORMAT_DSPGROUP_TRUESPEECH */
{
	UWORD	wRevision;
	UWORD	nSamplesPerBlock;
	UBYTE	abReserved[28];
};

struct WaveFmt_CREATIVE_ADPCM		/* for WAVE_FORMAT_CREATIVE_ADPCM */
{
	UWORD	wRevision;
};

struct WaveFmt_MEDIASPACE_ADPCM		/* for WAVE_FORMAT_MEDIASPACE_ADPCM */
{
	UWORD	wRevision;
};

struct WaveFmt_SIERRA_ADPCM			/* for WAVE_FORMAT_SIERRA_ADPCM */
{
	UWORD	wRevision;
};

struct WaveFmt_DOLBY_AC2			/* for WAVE_FORMAT_DOLBY_AC2 */
{
	UWORD	nAuxBitsCode;
};

struct WaveFact
{
	ULONG	dwFileSize;
};

struct WaveCue
{
	ULONG	dwName;
	ULONG	dwPosition;
	ULONG	fccChunk;
	ULONG	dwChunkStart;
	ULONG	dwBlockStart;
	ULONG	dwSampleOffset;
};

struct WaveSmpl
{            
	ULONG	dwIMAManufacturer;		// IMA Manufacturer ID Code, 0 if no sampler specific info
									// 47h=71 Akai
	ULONG	dwIMAProduct;			// IMA Product ID Code, 0 if no sampler specific info
	ULONG	dwSamplePeriod;			// Length of one sample in nanoseconds
	ULONG	dwUnityMIDINote;		// MIDI Note at original pitch 60=Middle C
	ULONG	dwPitchFraction;		// Micro-tuning, 0..0xFFFFFFFF==100 cents.
	ULONG	dwSMPTEType;			// FramesPerSecond; 30 seems to be usual
	ULONG	dwSMPTEOffset;
	ULONG	dwNumLoops;
	ULONG	dwSamplerSpecificSize;
//	struct WaveSmpl_Loop Loopdata[];
//  ...     SamplerSpecific;
};

struct WaveSmpl_Loop
{
	ULONG			dwLoopID;
	ULONG			dwType;
	ULONG			dwStart;		// offset in samples, first sample played in loop
	ULONG			dwEnd;			// offset in samples, last sample played in loop
	ULONG			dwFraction;		// End fraction, 0..0xFFFFFFFF==1 sample
	LONG			lPlayCount;		// indefinitely sustained loops == 0x80000000 or 0
};

#define WAV_SMPL_LOOPFWD	  0		// Loop Forward (Normal)
#define WAV_SMPL_LOOPBID	  1		// Alternating Loop (Forward/Backward)
//							2-16	// Reserved for Sampler Specific Loops
#define WAV_SMPL_LOOPOFF	255		// Off

#define WAV_SMPL_LOOPINF	0x80000000	// indefinitely sustained loops -> 0x80000000 or 0

struct WaveInst
{
	UBYTE	bUnshiftedNote;
	BYTE	chFineTune;
	BYTE	chGain;
	UBYTE	bLowNote;
	UBYTE	bHighNote;
	UBYTE	bLowVelocity;
	UBYTE	bHighVelocity;
};

/* format tags */

#define FORMAT_WAV_PCM8			 0
#define FORMAT_WAV_PCM16		 1
#define FORMAT_WAV_PCM24		 2
#define FORMAT_WAV_PCM32		 3
#define FORMAT_WAV_ULAW			 4
#define FORMAT_WAV_ALAW			 5
#define FORMAT_WAV_IEEE32	     6
#define FORMAT_WAV_IEEE64		 7
#define FORMAT_WAV_MS_ADPCM_SP	 8
#define FORMAT_WAV_MS_ADPCM_MP	 9
#define FORMAT_WAV_IMA_ADPCM_2	10
#define FORMAT_WAV_IMA_ADPCM_3	11
#define FORMAT_WAV_IMA_ADPCM_4	12
#define FORMAT_WAV_IMA_ADPCM_5	13


/* possible values for wFormatTag */

#define	WAVE_FORMAT_UNKNOWN					0x0000		/* Microsoft Corporation */
#define WAVE_FORMAT_PCM						0x0001		/* Microsoft Corporation */
#define	WAVE_FORMAT_ADPCM					0x0002		/* Microsoft Corporation */
#define WAVE_FORMAT_IEEE_FLOAT				0x0003		/* Microsoft Corporation, IEEE754: range (+1, -1], 32-bit/64-bit format as defined by MSVC++ float/double type */
#define WAVE_FORMAT_IBM_CVSD				0x0005		/* IBM Corporation */
#define	WAVE_FORMAT_ALAW					0x0006		/* Microsoft Corporation */
#define	WAVE_FORMAT_MULAW					0x0007		/* Microsoft Corporation */
#define	WAVE_FORMAT_OKI_ADPCM				0x0010		/* OKI */
#define WAVE_FORMAT_DVI_ADPCM				0x0011		/* Intel Corporation */
#define WAVE_FORMAT_IMA_ADPCM				WAVE_FORMAT_DVI_ADPCM
#define WAVE_FORMAT_MEDIASPACE_ADPCM		0x0012		/* Videologic */
#define WAVE_FORMAT_SIERRA_ADPCM			0x0013		/* Sierra Semiconductor Corp */
#define WAVE_FORMAT_G723_ADPCM				0x0014		/* Antex Electronics Corporation */
#define	WAVE_FORMAT_DIGISTD					0x0015		/* DSP Solutions, Inc. */
#define	WAVE_FORMAT_DIGIFIX					0x0016		/* DSP Solutions, Inc. */
#define WAVE_FORMAT_DIALOGIC_OKI_ADPCM		0x0017		/* Dialogic Corporation */
#define WAVE_FORMAT_YAMAHA_ADPCM			0x0020		/* Yamaha Corporation of America */
#define WAVE_FORMAT_SONARC					0x0021		/* Speech Compression */
#define WAVE_FORMAT_DSPGROUP_TRUESPEECH		0x0022		/* DSP Group, Inc */
#define WAVE_FORMAT_ECHOSC1					0x0023		/* Echo Speech Corporation */
#define WAVE_FORMAT_AUDIOFILE_AF36			0x0024
#define WAVE_FORMAT_APTX					0x0025		/* Audio Processing Technology */
#define WAVE_FORMAT_AUDIOFILE_AF10			0x0026
#define WAVE_FORMAT_DOLBY_AC2				0x0030		/* Dolby Laboratories */
#define WAVE_FORMAT_GSM610					0x0031		/* Microsoft Corporation */
#define WAVE_FORMAT_ANTEX_ADPCME			0x0033		/* Antex Electronics Corporation */
#define WAVE_FORMAT_CONTROL_RES_VQLPC		0x0034		/* Control Resources Limited */
#define WAVE_FORMAT_DIGIREAL				0x0035		/* DSP Solutions, Inc. */
#define WAVE_FORMAT_DIGIADPCM				0x0036		/* DSP Solutions, Inc. */
#define WAVE_FORMAT_CONTROL_RES_CR10		0x0037		/* Control Resources Limited */
#define WAVE_FORMAT_NMS_VBXADPCM			0x0038		/* Natural MicroSystems */
#define WAVE_FORMAT_CS_IMAADPCM				0x0039		/* Crystal Semiconductor IMA ADPCM */
#define WAVE_FORMAT_G721_ADPCM				0x0040		/* Antex Electronics Corporation  */
#define WAVE_FORMAT_MPEG					0x0050		/* Microsoft Corporation */
#define WAVE_FORMAT_MPEGLAYER3				0x0055		/* ISO/MPEG Layer3 Format Tag  @todo xpksfx:sample_sfx/wav/maxhead01.wav */
#define WAVE_FORMAT_IBM_MULAW				0x0101		/* IBM Corporation */
#define WAVE_FORMAT_IBM_ALAW				0x0102		/* IBM Corporation */
#define WAVE_FORMAT_IBM_ADPCM				0x0103		/* IBM Corporation */
#define WAVE_FORMAT_CREATIVE_ADPCM			0x0200		/* Creative Labs, Inc */
#define WAVE_FORMAT_CREATIVE_FASTSPEECH8	0x0202		/* Creative Labs, Inc */
#define WAVE_FORMAT_CREATIVE_FASTSPEECH10	0x0203		/* Creative Labs, Inc */
#define WAVE_FORMAT_FM_TOWNS_SND			0x0300		/* Fujitsu Corp. */
#define WAVE_FORMAT_OLIGSM					0x1000		/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_OLIADPCM				0x1001		/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_OLICELP					0x1002		/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_OLISBC					0x1003		/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_OLIOPR					0x1004		/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_DEVELOPMENT         	0xFFFF
