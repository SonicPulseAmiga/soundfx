/****h* Analyse-Stereo/Analyse-Stereo.h [4.2] *
*
*  NAME
*    Analyse-Stereo.h
*  COPYRIGHT
*    $VER: Analyse-Stereo.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Analyse-Stereo core header file
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

#ifndef  ANALYSE_STEREO_H
#define  ANALYSE_STEREO_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define RES			512
#define RES_FC		256.0
#define RES_VALS	(RES+RES+1)

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	\
	ULONG					sum[RES_VALS];\
	\
	SAMPLE					*sbuf0,*sbuf1;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef ANALYSE_STEREO_C
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

#endif   /* ANALYSE_STEREO_H */

//-- eof ----------------------------------------------------------------------
