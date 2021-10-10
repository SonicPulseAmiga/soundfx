/****h* Panorama-4Ch/Panorama-4Ch.h [4.2] *
*
*  NAME
*    Panorama-4Ch.h
*  COPYRIGHT
*    $VER: Panorama-4Ch.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Panorama-4Ch core header file
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

#ifndef  PANORAMA_4CH_H
#define  PANORAMA_4CH_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define MODE_AUDIO	0
#define MODE_VIDEO	1

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod1;	/* L-R Pos */\
	Modulator				mod2;	/* V-H Pos */\
	UBYTE					mode;	/* speaker setting */\
	\
	double					lrs,lre,lrd,lr;\
	double					vhs,vhe,vhd,vh;\
	double					voll,volr,volv,volh,v;\
	\
	SAMPLE					*sbuf,*dbuf0,*dbuf1,*dbuf2,*dbuf3;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef PANORAMA_4CH_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_audio(SFXCoreInstance *instance,unsigned long samples);
	extern void process_video(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* PANORAMA_4CH_H */

//-- eof ----------------------------------------------------------------------
