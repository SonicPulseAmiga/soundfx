/****h* Convolve/Convolve.h [4.2] *
*
*  NAME
*    Convolve.h
*  COPYRIGHT
*    $VER: Convolve.h 4.2 (06.03.03) � by Stefan Kost 1998-2003
*  FUNCTION
*    Convolve core header file
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

#ifndef  CONVOLVE_H
#define  CONVOLVE_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src1;	/* Quelle */\
	Source					src2;	/* Quelle */\
	Modulator				mod1;	/* Effectpropotion */\
	Modulator				mod2;	/* Feedback */\
	Param					p_ampf;\
	\
	double					effs,effe,effd,eff;\
	double					fbs,fbe,fbd,fb;\
	double					ampf;\
	ULONG					dellen;\
	SAMPLE					*delbufbeg,*delbufend,*delbufptr;\
	double					*sirbufbeg;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef CONVOLVE_C
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

#endif   /* CONVOLVE_H */

//-- eof ----------------------------------------------------------------------
