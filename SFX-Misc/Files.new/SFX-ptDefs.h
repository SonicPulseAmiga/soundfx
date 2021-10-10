/******************************************************************************
** SFX-PTDefs.h                                                              **
**---------------------------------------------------------------------------**
** SFX ProtrackerFileFormatDefinitions                                       **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 21.12.1994                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

struct PT_Short_Header
{
	char	name[20];
}

struct PT_Samples						/* 31 times */
{
	char	name[22];
	UWORD	len;
	UBYTE	finetune;					/* lower 4 bit -> signed 4 bit value */
	UBYTE	volume;						/* von 0 bis 64 */
	UWORD	loopstart;					/* multiply by 2 */
	UWORD	looplen;					/* multiply by 2 */
}

struct PT_Long_Header
{
	UBYTE	len;						/* von 1 bis 128 */
	UBYTE	dummy1;						/* set to 127 */
	UBYTE	songpos[128];				/* songpositions ; jede von 0 bis 63 */
	char	id[4];						/* M.K.,FLT4,FLT8 */
}

struct PT_Pattern
{
	ULONG	notes[64][4];				/* patterndata */
}

/*
Speed		BPM			ptsteps/sec		1/(ptstep/sec)	time/pattern
-----------------------------------------------------------------------
0xF			 50.0		 3.3_			0.3		(-0.2)	19.2	(-1.28)
0xE			 53.5714285	 3.571428571	0.28			17.92
0xD			 57.6923076	 3.846153846	0.26			16.64
0xC			 62.5		 4.16_			0.24			15.36
0xB			 68.18__	 4.54__			0.22			14.08
0xA			 75.0		 5.0			0.2				12.8
  9			 83.3_		 5.5_			0.18			11.52
  8			 93.75		 6.25			0.16			10.24
  7			107.1428571	 7.142857143	0.14			 8.96
  6			125.0		 8.3_			0.12			 7.68
  5			150.0		10.0			0.1				 6.4
  4			187.5		12.5			0.08			 5.12
  3			250.0		16.6_			0.06			 3.84
  2			375.0		25.0			0.04			 2.56
  1			750.0		50.0			0.02			 1.28
*/
