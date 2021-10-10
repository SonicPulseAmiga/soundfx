/****h* Distortion/Distortion.h [4.2] *
*
*  NAME
*    Distortion.h
*  COPYRIGHT
*    $VER: Distortion.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Distortion core header file
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

#ifndef  DISTORTION_H
#define  DISTORTION_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define MAP_FULLRANGE	0
#define MAP_MIRRORED	1

#define WRAP_NOCLIP	0
#define WRAP_CLIP	1
#define WRAP_WRAP1	2
#define WRAP_WRAP2	3
#define WRAP_ANZ	4

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod1;	/* Effektanteil */\
	Modulator				mod2;	/* Distortion */\
	UBYTE					map;\
	UBYTE					wrap;\
	\
	double					effs,effe,effd;\
	LONG					diss,dise,disd;\
	SAMPLE					*trans;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef DISTORTION_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_noclip(SFXCoreInstance *instance,unsigned long samples);
	extern void process_clip(SFXCoreInstance *instance,unsigned long samples);
	extern void process_wrap1(SFXCoreInstance *instance,unsigned long samples);
	extern void process_wrap2(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* DISTORTION_H */

//-- eof ----------------------------------------------------------------------
