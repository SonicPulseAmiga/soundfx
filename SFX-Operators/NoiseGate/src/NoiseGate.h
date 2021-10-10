/****h* NoiseGate/NoiseGate.h [4.2] *
*
*  NAME
*    NoiseGate.h
*  COPYRIGHT
*    $VER: NoiseGate.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    NoiseGate core header file
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

#ifndef  NOISEGATE_H
#define  NOISEGATE_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h>
#include <libraries/sfxoperator.h>

//-- defines ------------------------------------------------------------------

#define ENVFC1	0.995
#define ENVFC2	(1.0-ENVFC1)

#define CURVE_LINEAR	0
#define CURVE_SPEEDUP	1
#define CURVE_SLOWDOWN	2

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod;	/* Threshold */\
	Param					p_attack;\
	UBYTE					shape;\
	\
	double					thrs,thre,thrd,thr;\
	\
	ULONG					attack;\
	double					ampenv,expon;\
	ULONG					fop;\
	UBYTE					fo;\
	double					*fadearr;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef NOISEGATE_C
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

#endif   /* NOISEGATE_H */

//-- eof ----------------------------------------------------------------------
