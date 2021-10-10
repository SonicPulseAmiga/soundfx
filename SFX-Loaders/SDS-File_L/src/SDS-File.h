/******************************************************************************
** SFX-FileDefs.h                                                            **
**---------------------------------------------------------------------------**
** SFX FileFormatDefinitions                                                 **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 06.01.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#define SYSEX_START		0xF07E
#define SYSEX_END		0xF7

#define SDS_HEADER		0x01
#define SDS_BLOCK		0x02

struct SampleHeader {	/* 21 bytes */
	UWORD	sox;			/* 0xF07E */
	UBYTE	channelnr;
	UBYTE	type;			/* always 01 */

	UBYTE	samplenr0,samplenr1;
	UBYTE	bits;
	UBYTE	rate0,rate1,rate2;
	UBYTE	length0,length1,length2;
	UBYTE	loopstart0,loopstart1,loopstart2;
	UBYTE	loopend0,loopend1,loopend2;
	UBYTE	loopmode;
	UBYTE	eox;			/* 0xF7 */
};

#define SDS_LOOP_FORWARD	0x00
#define SDS_LOOP_PINGPONG	0x01
#define SDS_LOOP_NONE		0x7F

struct SampleBlock {	/* 127 bytes */
	UWORD	sox;			/* 0xF07E */
	UBYTE	channelnr;
	UBYTE	type;			/* always 02 */

	UBYTE	blockct;
	UBYTE	data[120];
	UBYTE	chksum;			/* XOR of all data up to here */
	UBYTE	eox;			/* 0xF7 */
};
