/****h* Synthesize-FM/Synthesize-FM.h [4.2] *
*
*  NAME
*    Synthesize-FM.h
*  COPYRIGHT
*    $VER: Synthesize-FM.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Synthesize-FM core header file
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

#ifndef  SYNTHESIZE_FM_H
#define  SYNTHESIZE_FM_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define WAVE_SIN	0
#define WAVE_TRI	1
#define WAVE_SAW	2
#define WAVE_SQR	3

#define ANZ_OSZIS	6									// maybe we need more later on

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;		/* Quelle */\
	Modulator				mod1[ANZ_OSZIS];	/* Frequency */\
	Modulator				mod2[ANZ_OSZIS];	/* Amplitude */\
	Param					p_ampf;				/* Gesamtverstärkung */\
	Param					p_pitch;			/* BasePitch */\
	Param					p_slen;				/* Länge des Samples */\
	Param					p_srat;				/* Abspielrate des Samples */\
	/*Param					amount[ANZ_OSZIS];*//* Modulationstiefe */\
	Param					p_phase [ANZ_OSZIS];/* Phasenverschiebung */\
	UWORD					oszi  [ANZ_OSZIS];	/* Oszilatortyp */\
	UBYTE					matrix[ANZ_OSZIS][ANZ_OSZIS];	/* Modulationmatrix [Mod=Src][Car=Dst] */\
	\
	double					amps [ANZ_OSZIS],ampe [ANZ_OSZIS],ampd [ANZ_OSZIS],amp;\
	double					frq1s[ANZ_OSZIS],frq1e[ANZ_OSZIS],frq1d[ANZ_OSZIS],frq1;\
	double					lastval[ANZ_OSZIS];\
	double					*modptr[ANZ_OSZIS][ANZ_OSZIS+1];\
	double					phase[ANZ_OSZIS],angle[ANZ_OSZIS];\
	UBYTE					audible[ANZ_OSZIS];\
	double					ampf;\
	double					pitch;\
	double					oneperh;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef SYNTHESIZE_FM_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);

	extern double sfxtri(double angle);
	extern double sfxsaw(double angle);
	extern double sfxsqr(double angle);
#else
	double sfxtri(double angle);
	double sfxsaw(double angle);
	double sfxsqr(double angle);
#endif

#endif   /* SYNTHESIZE_FM_H */

//-- eof ----------------------------------------------------------------------
