/****h* Filter-FIRMutate/Filter-FIRMutate.h [4.2] *
*
*  NAME
*    Filter-FIRMutate.h
*  COPYRIGHT
*    $VER: Filter-FIRMutate.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Filter-FIRMutate core header file
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

#ifndef  FILTER_FIRMUTATE_H
#define  FILTER_FIRMUTATE_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src1;	/* Quelle */\
	Source					src2;	/* Quelle */\
	Modulator				mod1;	/* effect propotion */\
	Modulator				mod2;	/* filter offset */\
	Modulator				mod3;	/* filter stretch */\
	WinFunction				wfkt1;	/* Fensterfunktion */\
	Interpolator			int1;	/* Interpolator */\
	Param					p_anz;\
	Param					p_ampf;\
	\
	double					effs,effe,effd,eff,dry;\
	double					offs,offe,offd,off;\
	double					strs,stre,strd,str;\
	UWORD					anz;\
	double					ampf;\
	InterpolFuncPtr			interpolfunc;\
	ULONG					mstr;\
	SAMPLE					*rbuf1,*rbufbase1,*rbufw1;\
	ULONG					rbufmask1,rbufsize1;\
	SAMPLE					*rbuf2,*rbufbase2,*rbufw2;\
	ULONG					rbufmask2,rbufsize2;\
	double					*win_fkt;\
	ULONG					rm_start2,rm_len2;\
	\
	SAMPLE					*dbuf,*sbuf1,*sbuf2;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef FILTER_FIRMUTATE_C
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

#endif   /* FILTER_FIRMUTATE_H */

//-- eof ----------------------------------------------------------------------
