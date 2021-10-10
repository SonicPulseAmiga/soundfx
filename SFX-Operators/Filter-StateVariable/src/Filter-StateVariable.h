/****h* Filter-StateVariable/Filter-StateVariable.h [4.2] *
*
*  NAME
*    Filter-StateVariable.h
*  COPYRIGHT
*    $VER: Filter-StateVariable.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-StateVariable core header file
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

#ifndef  FILTER_STATEVARIABLE_H
#define  FILTER_STATEVARIABLE_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h>
#include <libraries/sfxoperator.h>

//-- defines ------------------------------------------------------------------

#define TYPE_LOWPASS	0
#define TYPE_HIGHPASS	1
#define TYPE_BANDPASS	2
#define TYPE_BANDSTOP	3

#define FILTERTYPE_LABELS { "LowPass","HighPass","BandPass","BandStop",0l }

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod1;	/* Effektanteil */\
	Modulator				mod2;	/* Cut-Off */\
	Modulator				mod3;	/* Resonance */\
	Param					p_ampf;	/* Verstärkung/Abschwächung */\
	UBYTE					type;	/* LowPass,HighPass,BandPass,BandStop */\
	\
	double					effs,effe,effd,eff,dry;\
	double					frqs,frqe,frqd,frq;\
	double					ress,rese,resd,res;\
	double					low,mid,high;\
	double					ampf;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef FILTER_STATEVARIABLE_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_lowpass(SFXCoreInstance *instance,unsigned long samples);
	extern void process_highpass(SFXCoreInstance *instance,unsigned long samples);
	extern void process_bandpass(SFXCoreInstance *instance,unsigned long samples);
	extern void process_bandstop(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* FILTER_STATEVARIABLE_H */

//-- eof ----------------------------------------------------------------------
