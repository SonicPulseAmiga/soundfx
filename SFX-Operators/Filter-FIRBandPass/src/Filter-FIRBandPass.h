/****h* Filter-FIRBandPass/Filter-FIRBandPass.h [4.2] *
*
*  NAME
*    Filter-FIRBandPass.h
*  COPYRIGHT
*    $VER: Filter-FIRBandPass.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-FIRBandPass core header file
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

#ifndef  FILTER_FIRBANDPASS_H
#define  FILTER_FIRBANDPASS_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod1;	/* Effektanteil */\
	Modulator				mod2;	/* Cut-Off1 */\
	Modulator				mod3;	/* Cut-Off2 */\
	WinFunction				wfkt1;	/* Fensterfunktion */\
	Param					p_anz;\
	\
	double					effs,effe,effd,eff,dry;\
	double					frq1s,frq1e,frq1d,frq1;\
	double					frq2s,frq2e,frq2d,frq2;\
	\
	UWORD					anz;\
	double					*win_fkt;\
	SAMPLE					*rbuf,*rbufbase,*rbufw;\
	ULONG					rbufmask,rbufsize;\
	double					*matrix;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef FILTER_FIRBANDPASS_C
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

#endif   /* FILTER_FIRBANDPASS_H */

//-- eof ----------------------------------------------------------------------
