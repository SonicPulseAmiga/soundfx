/****h* ZeroPass/ZeroPass.h [4.2] *
*
*  NAME
*    ZeroPass.h
*  COPYRIGHT
*    $VER: ZeroPass.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    ZeroPass core header file
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

#ifndef  ZEROPASS_H
#define  ZEROPASS_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define CURVE_LINEAR	0
#define CURVE_SPEEDUP	1
#define CURVE_SLOWDOWN	2

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;		/* Quelle */\
	Modulator				mod1,mod2;\
	Param					ranges,rangee;\
	UBYTE					shapes,shapee;\
	\
	ULONG					poss,pose,lens,lene;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef ZEROPASS_C
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

#endif   /* ZEROPASS_H */

//-- eof ----------------------------------------------------------------------
