/****h* Analyse-Spect2D/Analyse-Spect2D.h [4.2] *
*
*  NAME
*    Analyse-Spect2D.h
*  COPYRIGHT
*    $VER: Analyse-Spect2D.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Analyse-Spect2D core header file
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

#ifndef  ANALYSE_SPECT2D_H
#define  ANALYSE_SPECT2D_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	WinFunction				wfkt1;	/* Fensterfunktion */\
	Param					p_lines;\
	Param					p_gamma;\
	UBYTE					bands;\
	UBYTE					prec;	/* highest -> normal(1:1) -> lowest */\
	UBYTE					pal;	/* color/gray */\
	UBYTE					maxscan;/* collect maximum amplitudes ? */\
	\
	UWORD					m,ml;\
	double					gamma;\
	float					*raster;/* Puffer für Spectrogammergebisse */\
	float					*rmaxa;	/* Puffer für MaxAmp Graph */\
	\
	SAMPLE					*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef ANALYSE_SPECT2D_C
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

#endif   /* ANALYSE_SPECT2D_H */

//-- eof ----------------------------------------------------------------------
