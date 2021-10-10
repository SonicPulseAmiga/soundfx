/******************************************************************************
** SFX-FileDefs.h                                                            **
**---------------------------------------------------------------------------**
** SFX FileFormatDefinitions                                                 **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 06.01.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#define STUDIO16_KWIK		MakeID('K','W','K','3')

#define	S16_FILTER_INIT	1L	/* use may also use S16S_RATE (S16R_RATE/2). */
#define	S16_FLAG_INIT	0L	/* init S16S_FLAGS with this value. */

/*  Volumes

 Calc: S16F_VOLUME/S16_VOL_STEPS-100 = x dB
       value = (x dB + 100) x S16_VOL_STEPS */

#define	S16_VOL_0	0x0C80	/*  +0 dB */
#define	S16_VOL_OFF	0x0000	/*  oo dB */

#define	S16_VOL_STEPS	0x0020	/* you may use 1/4th of this step. */

/*  for convinience only: */

#define	S16_VOL_DOUBLE	0x0D40	/*  +6.0 dB (200 % volume). */
#define	S16_VOL_NORMAL	0x0C80	/*  +0.0 dB (100 % volume). */
#define	S16_VOL_HALF	0x0BC0	/*  -6.0 dB ( 50 % volume). */
#define	S16_VOL_QUATER	0x0B00	/* -12.0 dB ( 25 % volume). */
#define	S16_VOL_BACK	0x09C0	/* -22.0 dB (music for voiceover). */

/* Pans

  Calc: S16F_PAN/S16_PAN_STEPS = Pan pos 0-200, 100=center
        value = Pan x S16_PAN_STEPS */

#define	S16_PAN_LEFT	0x0000	/* full left. */
#define	S16_PAN_MID		0x0C80	/* center. */
#define	S16_PAN_RIGHT	0x1900	/* full right. */

#define	S16_PAN_STEPS	0x0020	/* you may use 1/4th of this step. */

/*  for convinience only: */

#define	S16_PAN_LEFT50	0x0640	/* 50% to left. */
#define	S16_PAN_RIGHT50	0x12C0	/* 50% to right. */

struct	S16SMPTEStamp
{
	UBYTE	hours;	/* number of hours */
	UBYTE	min;	/* number of minuttes. */
	UBYTE	sec;	/* number of seconds. */
	UBYTE	frames;	/* number of frames rel. to PAL/NTSC etc. */
}; /* length 4 bytes */

struct SampleInfo
{
	ULONG	Rate;				/* sampling rate */
	ULONG	Filter3db;			/* Anti-aliasing filter value */
	UWORD	Volume;				/* prefered volume level */
	struct S16SMPTEStamp StartTC;
	float	SmpteSamplingRate;
	ULONG	Pan;
	ULONG	Flags;
	ULONG	Reserved[1];

	ULONG	RealSize;	/* number of samples (bytesize/2). 		length of samp with no data clips */
	ULONG	EditSize;	/* number of samples after editlist.	length of samp with using data clips */

}; /* length 30 bytes */

struct DiskRegion			/* DiskRegion is saved to disk */
{
	char	region_name[40];
	ULONG	Start;				/* starting sample, inclusive */
	ULONG	End;				/* ending sample, inclusive */
	struct SampleInfo params;	/* filter, rate, volume,etc. */
	ULONG	Flags;
}; /* length 82 bytes */

struct SampleDataClip
{
	ULONG Start;				/* inclusive */
	ULONG End;					/* inclusive */
}; /* length 8 bytes */

#define SFT_NUM_REGIONS 32
#define DS_NUM_CLIPS 128

struct SampleFileTag
{
	struct SampleInfo params;
	struct SampleDataClip dclips[DS_NUM_CLIPS];			/* 128 *  8 Bytes -> 1024 */
	struct DiskRegion dregions[SFT_NUM_REGIONS];		/*  32 * 82 Bytes -> 2624 */
};
