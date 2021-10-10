/****h* Synthesize-Add/Synthesize-Add.h [4.2] *
*
*  NAME
*    Synthesize-Add.h
*  COPYRIGHT
*    $VER: Synthesize-Add.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Synthesize-Add core header file
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

#ifndef  SYNTHESIZE_ADD_H
#define  SYNTHESIZE_ADD_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define WAVE_SIN	0
#define WAVE_TRI	1
#define WAVE_SAW	2
#define WAVE_SQR	3

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod1;	/* Frequency */\
	Modulator				mod2;	/* Amplitude */\
	Param					scale;	/* Gesamtscalierung */\
	Param					p_pitch;/* Toneadjust */\
	Param					p_slen;	/* Länge des Samples */\
	Param					p_srat;	/* Abspielrate des Samples */\
	Param					p_phase;/* Phasenverschiebung */\
	double					svals[64];/* Teilerwerte für Obertöne */\
	double					phdist;	/* Phasenzerstörung */\
	UWORD					oszi;	/* Oszilatortyp */\
	\
	double					frqs,frqe,frqd,frq;\
	double					amps,ampe,ampd,amp;\
	double					pitch,phase;\
	double					angle,oneperh;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef SYNTHESIZE_ADD_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_sin(SFXCoreInstance *instance,unsigned long samples);
	extern void process_tri(SFXCoreInstance *instance,unsigned long samples);
	extern void process_saw(SFXCoreInstance *instance,unsigned long samples);
	extern void process_sqr(SFXCoreInstance *instance,unsigned long samples);
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

#endif   /* SYNTHESIZE_ADD_H */

//-- eof ----------------------------------------------------------------------
