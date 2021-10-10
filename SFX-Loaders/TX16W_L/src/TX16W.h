/******************************************************************************
** SFX-FileDefs.h                                                            **
**---------------------------------------------------------------------------**
** SFX FileFormatDefinitions                                                 **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 06.01.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

struct SampleHeader
{
//	UBYTE filetype[6] = "LM8953";
	UBYTE nulls[10];
	UBYTE dummy_aeg[6];		/* space for the AEG (never mind this) */
	UBYTE format;			/* 0x49 = looped, 0xC9 = non-looped */
	UBYTE sample_rate;		/* 1 = 33 kHz, 2 = 50 kHz, 3 = 16 kHz */
	UBYTE atc_length[3];	/* I'll get to this... */
	UBYTE rpt_length[3];
	UBYTE unused[2];		/* set these to null, to be on the safe side */
};

#define FORMAT_LOOPED		0x49
#define FORMAT_NOTLOOPED	0xC9

#define RATE_33000			1
#define RATE_50000			2
#define RATE_16000			3

#define RATE_33000_0		0x06
#define RATE_33000_1		0x52
#define RATE_50000_0		0x10
#define RATE_50000_1		0x00
#define RATE_16000_0		0xF6
#define RATE_16000_1		0x52
