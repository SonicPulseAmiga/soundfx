/****h* Gamma/gamma.h [4.2] *
*
*  NAME
*    gamma.h
*  COPYRIGHT
*    $VER: gamma.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Gamma core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    13.Feb.2003
*  MODIFICATION HISTORY
*    06.Mar.2003	V 4.2	most recent version
*    13.Feb.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  GAMMA_H
#define  GAMMA_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod;	/* Gammaexp. */\
	\
	double					gammas,gammae,gammad;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef GAMMA_C
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

#endif   /* GAMMA_H */

//-- eof ----------------------------------------------------------------------
