/******************************************************************************
** logic2state.h                                                             **
**---------------------------------------------------------------------------**
** 2-state logic                                                             **
**---------------------------------------------------------------------------**
** Version : V 1.0                                                           **
** Date    : 13.05.1998                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#ifndef LOGIC2STATE_H
#define LOGIC2STATE_H

//-- includes -----------------------------------------------------------------

#include <exec/types.h>

//-- defines ------------------------------------------------------------------

#define LOGIC2_FALSE	 0
#define LOGIC2_AND		 1
#define LOGIC2_A_GT_B	 2
#define LOGIC2_A		 3
#define LOGIC2_B_GT_A	 4
#define LOGIC2_B		 5
#define LOGIC2_XOR		 6
#define LOGIC2_OR		 7
#define LOGIC2_NOR		 8
#define LOGIC2_EQU		 9
#define LOGIC2_NOT_B	10
#define LOGIC2_A_GTEQ_B	11
#define LOGIC2_NOT_A	12
#define LOGIC2_B_GTEQ_A	13
#define LOGIC2_NAND		14
#define LOGIC2_TRUE		15

#ifndef FALSE
	#define FALSE	0
#endif
#ifndef TRUE
	#define TRUE	1
#endif

//-- globals ------------------------------------------------------------------

UBYTE logic2_tab[16][4]=
{
/* B  FALSE,TRUE ,FALSE,TRUE  */
/* A  FALSE,FALSE,TRUE ,TRUE  */
	{ FALSE,FALSE,FALSE,FALSE },	// FALSE
	{ FALSE,FALSE,FALSE,TRUE  },	// AND
	{ FALSE,FALSE,TRUE ,FALSE },	// A_GT_B
	{ FALSE,FALSE,TRUE ,TRUE  },	// A
	{ FALSE,TRUE ,FALSE,FALSE },	// B_GT_A
	{ FALSE,TRUE ,FALSE,TRUE  },	// B
	{ FALSE,TRUE ,TRUE ,FALSE },	// XOR
	{ FALSE,TRUE ,TRUE ,TRUE  },	// OR
	{ TRUE ,FALSE,FALSE,FALSE },	// NOR
	{ TRUE ,FALSE,FALSE,TRUE  },	// EQU
	{ TRUE ,FALSE,TRUE ,FALSE },	// NOT_B
	{ TRUE ,FALSE,TRUE ,TRUE  },	// A_GTEQ_B
	{ TRUE ,TRUE ,FALSE,FALSE },	// NOT_A
	{ TRUE ,TRUE ,FALSE,TRUE  },	// B_GTEQ_A
	{ TRUE ,TRUE ,TRUE ,FALSE },	// NAND
	{ TRUE ,TRUE ,TRUE ,TRUE  },	// TRUE
/* B  FALSE,TRUE ,FALSE,TRUE  */
/* A  FALSE,FALSE,TRUE ,TRUE  */
};

//-- prototypes ---------------------------------------------------------------

UBYTE logic2_b(UBYTE a,UBYTE b,UBYTE *fc);
UWORD logic2_w(UWORD a,UWORD b,UBYTE *fc);
ULONG logic2_l(ULONG a,ULONG b,UBYTE *fc);

//-- functions ----------------------------------------------------------------

UBYTE logic2_b(UBYTE a,UBYTE b,UBYTE *fc)
{
	register UBYTE i;
	UBYTE ret=0;

	for(i=0;i<8;i++)
	{
		ret|=(fc[((a&1)<<1)|(b&1)]<<i);
		a>>=1;b>>=1;
	}
	return(ret);
}

UWORD logic2_w(UWORD a,UWORD b,UBYTE *fc)
{
	register UBYTE i;
	UWORD ret=0;

	for(i=0;i<16;i++)
	{
		ret|=(fc[((a&1)<<1)|(b&1)]<<i);
		a>>=1;b>>=1;
	}
	return(ret);
}

ULONG logic2_l(ULONG a,ULONG b,UBYTE *fc)
{
	register UBYTE i;
	ULONG ret=0;

	for(i=0;i<32;i++)
	{
		ret|=(fc[((a&1)<<1)|(b&1)]<<i);
		a>>=1;b>>=1;
	}
	return(ret);
}

#endif	/* LOGIC2STATE_H */

//-- eof ----------------------------------------------------------------------
