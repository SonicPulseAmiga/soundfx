/****h* Dynamic/Dynamic.h [4.3] *
*
*  NAME
*    Dynamic.h
*  COPYRIGHT
*    $VER: Dynamic.h 4.3 (12.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Dynamic core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    12.Mar.2004	V 4.3	most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  DYNAMIC_H
#define  DYNAMIC_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define KNEE_HARD 0
#define KNEE_SOFT 1

#define KNEE_LABELS { "Hard","Soft",NULL }

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod1;	/* Amplification Loud */\
	Modulator				mod2;	/* Amplification Quiet */\
	Modulator				mod3;	/* Threshhold */\
	UBYTE					knee;	/* hard/soft */\
	\
	double					rlos,rloe,rlod,rlo;\
	double					rqus,rque,rqud,rqu;\
	double					exts,exte,extd,ext;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef DYNAMIC_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_hard(SFXCoreInstance *instance,unsigned long samples);
	extern void process_soft(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* DYNAMIC_H */

//-- eof ----------------------------------------------------------------------
