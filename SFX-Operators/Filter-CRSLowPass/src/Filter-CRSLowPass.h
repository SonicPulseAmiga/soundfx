/****h* Filter-CRSLowPass/Filter-CRSLowPass.h [4.2] *
*
*  NAME
*    Filter-CRSLowPass.h
*  COPYRIGHT
*    $VER: Filter-CRSLowPass.h 4.2 (06.03.03) ? by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-CRSLowPass core header file
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

#ifndef  FILTER_CRSLOWPASS_H
#define  FILTER_CRSLOWPASS_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod1;	/* Effektanteil */\
	Modulator				mod2;	/* Delay */\
	Modulator				mod3;	/* Amplitude */\
	Param					p_amps;\
	Param					p_ampe;\
	\
	double					effs,effe,effd,eff,dry;\
	double					rngs,rnge,rngd,rng;\
	double					fbs,fbe,fbd,fb;\
	double					amps,ampe,ampd,amp;\
	SAMPLE					*rbuf,*rbufbase,*rbufw;\
	ULONG					delay,mrng,rbufmask,rbufsize;\
	double					*matrix;\
	SAMPLE					*fbbuf;\
	ULONG					fbptr;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef FILTER_CRSLOWPASS_C
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

#endif   /* FILTER_CRSLOWPASS_H */

//-- eof ----------------------------------------------------------------------
