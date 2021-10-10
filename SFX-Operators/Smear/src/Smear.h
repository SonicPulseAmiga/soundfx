/****h* Smear/Smear.h [4.3] *
*
*  NAME
*    Smear.h
*  COPYRIGHT
*    $VER: Smear.h 4.3 (23.11.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Smear core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    23.Nov.2003	V 4.3	most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  SMEAR_H
#define  SMEAR_H

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
	Modulator				mod2;	/* Feedback */\
	Interpolator			int1;	/* Interpolator */\
	\
	double					effs,effe,effd,eff,dry;\
	double					rngs,rnge,rngd,rng;\
	InterpolFuncPtr			interpolfunc;\
	SAMPLE					*rbuf,*rbufbase,*rbufw;\
	ULONG					rbufmask,rbufsize;\
	ULONG					spos,dpos,rest;\
	double					del,dell;\
	ULONG					off;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef SMEAR_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process(SFXCoreInstance *instance,unsigned long samples);
	extern void process_rest(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* SMEAR_H */

//-- eof ----------------------------------------------------------------------
