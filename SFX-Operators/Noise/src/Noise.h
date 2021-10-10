/****h* Noise/Noise.h [4.2] *
*
*  NAME
*    Noise.h
*  COPYRIGHT
*    $VER: Noise.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Noise core header file
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

#ifndef  NOISE_H
#define  NOISE_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define DYN_RNG	20
#define DYN_RNGS 3

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Modulator				mod1;	/* Min Change */\
	Modulator				mod2;	/* Max Change */\
	Param					p_srat;\
	Param					p_slen;\
	\
	double					mis,mie,mid,mi;\
	double					mas,mae,mad,ma;\
	LONG					val1;\
	BYTE					w;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef NOISE_C
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

#endif   /* NOISE_H */

//-- eof ----------------------------------------------------------------------
