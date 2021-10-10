/****h* sfxsupport.library/sfxplayeropts.h [4.2] *
*
*  NAME
*    sfxplayeropts.h
*  COPYRIGHT
*    $VER: sfxplayeropts.h 4.2 (11.02.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    player options parts of sfxsupport.h
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    11.Feb.2003
*  MODIFICATION HISTORY
*    11.Feb.2003	V 4.2	most recent version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXPLAYEROPTS_H
#define  LIBRARIES_SFXPLAYEROPTS_H

//-- Player-Options -------------------------------------------------------------------------------

typedef struct
{
	UBYTE hfilter;
	UBYTE rateclip;			/* 0 -> 28867, 1 -> 57734, 2 -> none */
} OPTP_Amiga_8bit;

typedef struct
{
	UBYTE hfilter;
	UBYTE rateclip;			/* 0 -> 28867, 1 -> 57734, 2 -> none */
} OPTP_Amiga_14bit;

typedef struct
{
	UBYTE hfilter;
	UBYTE rateclip;			/* 0 -> 28867, 1 -> 57734, 2 -> none */
} OPTP_Amiga_14cbit;

typedef struct
{
	ULONG playmode;
	UWORD playrate;
	ULONG recomode;
	UWORD recorate;
} OPTP_Ahi;

#endif   /* LIBRARIES_SFXPLAYEROPTS_H */

//-- eof ----------------------------------------------------------------------
