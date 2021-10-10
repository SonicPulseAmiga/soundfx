/******************************************************************************
** logic3state.h                                                             **
**---------------------------------------------------------------------------**
** 3-state logic                                                             **
**---------------------------------------------------------------------------**
** Version : V 1.0                                                           **
** Date    : 13.05.1998                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#ifndef LOGIC3STATE_H
#define LOGIC3STATE_H

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
#define UNKNOWN	1
#ifndef TRUE
	#define TRUE	2
#endif

//-- globals ------------------------------------------------------------------

BOOL logic3_tab[64][9]=
{					/*  FALSE                             AND      A_GT_B   A        B_GT_A   B        XOR      OR       NOR      EQU     
/* FALSE  ,FALSE   */ { FALSE  ,FALSE  ,FALSE  ,FALSE  ,  FALSE  , FALSE  , FALSE  , FALSE  , FALSE  , FALSE  , FALSE  , TRUE   , TRUE   ,}, /* FALSE  ,FALSE   */
/* FALSE  ,UNKNOWN */ { FALSE  ,FALSE  ,FALSE  ,FALSE  ,  FALSE  , FALSE  , FALSE  , UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,}, /* FALSE  ,UNKNOWN */
/* FALSE  ,TRUE    */ { FALSE  ,FALSE  ,FALSE  ,FALSE  ,  FALSE  , FALSE  , FALSE  , TRUE   , TRUE   , TRUE   , TRUE   , FALSE  , FALSE  ,}, /* FALSE  ,TRUE    */
/* UNKNOWN,FALSE   */ { FALSE  ,FALSE  ,FALSE  ,FALSE  ,  FALSE  , UNKNOWN, UNKNOWN, FALSE  , FALSE  , UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,}, /* UNKNOWN,FALSE   */
/* UNKNOWN,UNKNOWN */ { FALSE  ,FALSE  ,FALSE  ,FALSE  ,  UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,}, /* UNKNOWN,UNKNOWN */
/* UNKNOWN,TRUE    */ { FALSE  ,FALSE  ,FALSE  ,FALSE  ,  UNKNOWN, FALSE  , UNKNOWN, UNKNOWN, TRUE   , UNKNOWN, TRUE   , FALSE  , UNKNOWN,}, /* UNKNOWN,TRUE    */
/* TRUE   ,FALSE   */ { FALSE  ,FALSE  ,FALSE  ,FALSE  ,  FALSE  , TRUE   , TRUE   , FALSE  , FALSE  , TRUE   , TRUE   , FALSE  , FALSE  ,}, /* TRUE   ,FALSE   */
/* TRUE   ,UNKNOWN */ { FALSE  ,FALSE  ,FALSE  ,UNKNOWN,  UNKNOWN, UNKNOWN, TRUE   , FALSE  , UNKNOWN, UNKNOWN, TRUE   , FALSE  , UNKNOWN,}, /* TRUE   ,UNKNOWN */
/* TRUE   ,TRUE    */ { FALSE  ,UNKNOWN,TRUE   ,FALSE  ,  TRUE   , FALSE  , TRUE   , FALSE  , TRUE   , FALSE  , TRUE   , FALSE  , TRUE   ,}  /* TRUE   ,TRUE    */
}

//-- prototypes ---------------------------------------------------------------

UBYTE logic2_b(UBYTE a,UBYTE b,UBYTE fc);
UWORD logic2_w(UWORD a,UWORD b,UBYTE fc);
ULONG logic2_l(ULONG a,ULONG b,UBYTE fc);

//-- functions ----------------------------------------------------------------

UBYTE logic_b(UBYTE a,UBYTE b,UBYTE fc)
{
	register UBYTE i;
	UBYTE ret;

	for(i=0;i<8;i++)
	{
		ret|=(logic2_tab[fc][((a&1)<<1)|(b&1)]<<i);
		a>>=1;b>>=1;
	}
	return(ret);
}

UWORD logic_w(UWORD a,UWORD b,UBYTE fc)
{
	register UBYTE i;
	UWORD ret;

	for(i=0;i<16;i++)
	{
		ret|=(logic2_tab[fc][((a&1)<<1)|(b&1)]<<i);
		a>>=1;b>>=1;
	}
	return(ret);
}

ULONG logic_l(ULONG a,ULONG b,UBYTE fc)
{
	register UBYTE i;
	ULONG ret;

	for(i=0;i<32;i++)
	{
		ret|=(logic2_tab[fc][((a&1)<<1)|(b&1)]<<i);
		a>>=1;b>>=1;
	}
	return(ret);
}

#endif	/* LOGIC3STATE_H */

//-- eof ----------------------------------------------------------------------
