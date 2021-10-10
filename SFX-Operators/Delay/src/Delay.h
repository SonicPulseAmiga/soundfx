/****h* Delay/Delay.h [4.2] *
*
*  NAME
*    Delay.h
*  COPYRIGHT
*    $VER: Delay.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Delay core header file
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

#ifndef  DELAY_H
#define  DELAY_H

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
	Modulator				mod3;	/* Delay */\
	Interpolator			int1;	/* Interpolator */\
	Param 					p_ampf;	/* Verstärkung/Abschwächung */\
	UBYTE					drymode;/* Modus */\
	\
	double					effs,effe,effd;\
	double					fbs,fbe,fbd;\
	double					dels,dele,deld;\
	double					ampf;\
	SAMPLE					*rbuf,*rbufbase,*rbufw;\
	ULONG					delay,rbufmask,rbufsize;\
	InterpolFuncPtr			interpolfunc;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef DELAY_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_0(SFXCoreInstance *instance,unsigned long samples);
	extern void process_1(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* DELAY_H */

//-- eof ----------------------------------------------------------------------
