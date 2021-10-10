/****h* Equalize-3Band/Equalize-3Band.h [4.2] *
*
*  NAME
*    Equalize-3Band.h
*  COPYRIGHT
*    $VER: Equalize-3Band.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Equalize-3Band core header file
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

#ifndef  EQUALIZE_3BAND_H
#define  EQUALIZE_3BAND_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;		/* Quelle */\
	Modulator				mod1;		/* Lower-Cut-Off */\
	Modulator				mod2;		/* Higher-Cut-Off */\
	Modulator				mod3;		/* Verstärkung/Abschwächung Low */\
	Modulator				mod4;		/* Verstärkung/Abschwächung Mid */\
	Modulator				mod5;		/* Verstärkung/Abschwächung High */\
	Param 					p_ampf;		/* Verstärkung/Abschwächung */\
	\
	double					frqls,frqle,frqld,frql;\
	double					frqhs,frqhe,frqhd,frqh;\
	double					ampls,ample,ampld,ampl;\
	double					ampms,ampme,ampmd,ampm;\
	double					amphs,amphe,amphd,amph;\
	double					ampf;\
	double					llow,lmid,lhigh;\
	double					hlow,hmid,hhigh;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef EQUALIZE_3BAND_C
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

#endif   /* EQUALIZE_3BAND_H */

//-- eof ----------------------------------------------------------------------
