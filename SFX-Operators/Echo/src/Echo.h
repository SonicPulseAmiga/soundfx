/****h* Echo/Echo.h [4.2] *
*
*  NAME
*    Echo.h
*  COPYRIGHT
*    $VER: Echo.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Echo core header file
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

#ifndef  ECHO_H
#define  ECHO_H

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
	Interpolator			int1;	/* Interpolator */\
	Param					p_anz;\
	Param					p_ampf;\
	\
	double					effs,effe,effd,eff,dry;\
	double					dels,dele,deld;\
	double					amps,ampe,ampd;\
	double					ampf;\
	ULONG					anz;\
	ULONG					delay;\
	SAMPLE					*rbuf,*rbufbase,*rbufw;\
	ULONG					rbufmask,rbufsize;\
	InterpolFuncPtr			interpolfunc;\
	double					*amp,*del;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef ECHO_C
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

#endif   /* ECHO_H */

//-- eof ----------------------------------------------------------------------
