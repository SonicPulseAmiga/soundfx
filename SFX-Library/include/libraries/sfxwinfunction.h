/****h* sfxsupport.libary/sfxwinfunction.h [4.2] *
*
*  NAME
*    sfxwinfunction.h
*  COPYRIGHT
*    $VER: sfxwinfunction.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    winfunction related parts of sfxsupport.h
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Aug.2002
*  MODIFICATION HISTORY
*    19.Aug.2002	V 4.2	most recent version
*    19.Aug.2002	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXWINFUNCTION_H
#define  LIBRARIES_SFXWINFUNCTION_H

//-- Windowfunctions ------------------------------------------------------------------------------

typedef struct {
	ULONG	win;            // which kind of function
	double	par;            // it's parameter
	char	desc[64];
} WinFunction;

#define WFKT_IDCT   2
#define WFKT_IXCT   2

#define WIN_BARTLETT     0
#define WIN_BLACKMAN     1
#define WIN_FEJER        2
#define WIN_HALFSINE     3
#define WIN_HALFSINEQ    4
#define WIN_HALFSINES    5
#define WIN_HAMMING      6
#define WIN_HANNING      7
#define WIN_KAISER       8
#define WIN_RECTANGLE    9
#define WIN_WELCH       10

#define WIN_ANZ         11

// for PrefsData
#define PDS_WINFUNCTION     sizeof(WinFunction)

#endif   /* LIBRARIES_SFXWINFUNCTION_H */

//-- eof ----------------------------------------------------------------------
