/****h* SampleSplit/SampleSplit.h [4.2] *
*
*  NAME
*    SampleSplit.h
*  COPYRIGHT
*    $VER: SampleSplit.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SampleSplit core header file
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

#ifndef  SAMPLESPLIT_H
#define  SAMPLESPLIT_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;		/* Quelle */\
	Param					p_splitpos;	/* where to cut it*/\
	Param					p_splits;	/* how often cut it*/\
	\
	ULONG					splits,spos;\
	LONG					splitpos;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef SAMPLESPLIT_C
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

#endif   /* SAMPLESPLIT_H */

//-- eof ----------------------------------------------------------------------
