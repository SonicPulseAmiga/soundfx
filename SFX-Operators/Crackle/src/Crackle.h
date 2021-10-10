/****h* Crackle/Crackle.h [4.2] *
*
*  NAME
*    Crackle.h
*  COPYRIGHT
*    $VER: Crackle.h 4.2 (06.08.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Crackle core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    06.Aug.2003
*  MODIFICATION HISTORY
*    06.Aug.2003	V 4.2	most recent version
*    06.Aug.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  CRACKLE_H
#define  CRACKLE_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h>
#include <libraries/sfxoperator.h>

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod;	/* Crackdensity */\
	\
	double					crds,crde,crdd;\
	ULONG					density_counter;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef Crackle_C
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

#endif   /* CRACKLE_H */

//-- eof ----------------------------------------------------------------------