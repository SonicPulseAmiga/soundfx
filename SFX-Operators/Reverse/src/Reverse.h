/****h* Reverse/Reverse.h [4.2] *
*
*  NAME
*    Reverse.h
*  COPYRIGHT
*    $VER: Reverse.h 4.2 (06.03.03) ? by Stefan Kost 1998-2003
*  FUNCTION
*    Reverse core header file
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

#ifndef  REVERSE_H
#define  REVERSE_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;		/* Quelle */\
	Modulator				mod;		/* Effektanteil */\
	\
	double					effs,effe,effd,eff,dry;\
	ULONG					spos1,spos2,dpos,seek;\
	\
	SAMPLE					*dbuf,*sbuf0,*sbuf1;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef REVERSE_C
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

#endif   /* REVERSE_H */

//-- eof ----------------------------------------------------------------------
