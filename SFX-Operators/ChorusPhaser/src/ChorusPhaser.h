/****h* ChorusPhaser/ChorusPhaser.h [4.2] *
*
*  NAME
*    ChorusPhaser.h
*  COPYRIGHT
*    $VER: ChorusPhaser.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    ChorusPhaser core header file
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

#ifndef  CHORUSPHASER_H
#define  CHORUSPHASER_H

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
	Modulator				mod2;	/* Voice 1 */\
	Modulator				mod3;	/* Voice 2 */\
	Modulator				mod4;	/* Voice 3 */\
	Modulator				mod5;	/* Voice 4 */\
	Interpolator			int1;	/* Interpolation */\
	Param					p_ampf;\
	\
	double					effs,effe,effd,eff,dry;\
	double					del1s,del1e,del1d,del1;\
	double					del2s,del2e,del2d,del2;\
	double					del3s,del3e,del3d,del3;\
	double					del4s,del4e,del4d,del4;\
	double					ampf;\
	double					rblen;\
	double					maxdelay;\
	SAMPLE					*rbuf,*rbufbase,*rbufw;\
	ULONG					rbufmask,rbufsize;\
	InterpolFuncPtr			interpolfunc;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef CHORUSPHASER_C
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

#endif   /* CHORUSPHASER_H */

//-- eof ----------------------------------------------------------------------
