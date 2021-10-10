/****h* DeNoise-FIR/DeNoise-FIR.h [4.2] *
*
*  NAME
*    DeNoise-FIR.h
*  COPYRIGHT
*    $VER: DeNoise-FIR.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    DeNoise-FIR core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  DENOISE_FIR_H
#define  DENOISE_FIR_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h>
#include <libraries/sfxoperator.h>

//-- defines ------------------------------------------------------------------

#define MAX_BANDS 128

#define CURVE_LINEAR	0
#define CURVE_SPEEDUP	1
#define CURVE_SLOWDOWN	2

#define ENVFC1	0.995
#define ENVFC2	(1.0-ENVFC1)

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	WinFunction				wfkt1;	/* Fensterfunktion */\
	Param					p_attack;\
	Param					p_threshs;\
	Param					p_threshe;\
	Param					p_bands;\
	Param					p_anz;\
	UBYTE					shape;\
	\
	SAMPLE					*rbuf,*rbufbase,*rbufw;\
	ULONG					rbufmask,rbufsize;\
	double					*matrix[MAX_BANDS];\
	double					*win_fkt;\
	ULONG					attack;\
	double					*thresh,*ampenv,*fadearr;\
	ULONG					*fop;\
	UBYTE					*fo;\
	double					expon;\
	double					threshs,threshe;\
	UWORD					anz;\
	UBYTE					bands;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef DENOISE_FIR_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* DENOISE_FIR_H */

//-- eof ----------------------------------------------------------------------
