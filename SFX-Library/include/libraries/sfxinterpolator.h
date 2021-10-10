/****h* sfxsupport.library/sfxinterpolator.h [4.3] *
*
*  NAME
*    sfxinterpolator.h
*  COPYRIGHT
*    $VER: sfxinterpolator.h 4.3 (08.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    interpolator related parts of sfxsupport.h
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Aug.2002
*  MODIFICATION HISTORY
*    08.Apr.2004    V 4.3   most recent version
*    19.Aug.2002	V 4.2	most recent version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXINTERPOLATOR_H
#define  LIBRARIES_SFXINTERPOLATOR_H

//-- Interpolator ---------------------------------------------------------------------------------------

typedef struct {
	ULONG	type;			// which kind of function
	double	range;			// the length of interpol data
	char	desc[64];
} Interpolator;

#define INT_IDCT    2
#define INT_IXCT    2

#define INT_TYPE_NONE       0
#define INT_TYPE_LIN        1
#define INT_TYPE_SI         2
#define INT_TYPE_LAGRANGE   3

#define INT_TYPE_ANZ        4

// for PrefsData
#define PDS_INTERPOLATOR    sizeof(Interpolator)

typedef double ASM (*InterpolFuncPtr)(REG(a0,SAMPLE *data),REG(a1,double *pos),REG(d0,ULONG len),REG(d1,ULONG range));

#endif   /* LIBRARIES_SFXINTERPOLATOR_H */

//-- eof ----------------------------------------------------------------------
