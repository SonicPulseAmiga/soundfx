/****h* Resample/Resample.h [4.2] *
*
*  NAME
*    Resample.h
*  COPYRIGHT
*    $VER: Resample.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Resample core header file
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

#ifndef  RESAMPLE_H
#define  RESAMPLE_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Interpolator			int1;	/* Interpolator */\
	WinFunction				wfkt1;	/* Fensterfunktion */\
	Param					p_factor;\
	Param					p_anz;\
	UBYTE					aliasflt;/* Aliasing filter on/off */\
	UBYTE					lock;	/* lock factor=0,srat=1,slen=2 */\
	\
	double					factor,step;\
	UWORD					anz;\
	InterpolFuncPtr			interpolfunc;\
	double					*matrix;\
	double					*win_fkt;\
	double					frq,ampf;\
	SAMPLE					*r1buf,*r1bufbase,*r1bufw;\
	ULONG					r1bufmask,r1bufsize;\
	SAMPLE					*r2buf,*r2bufbase,*r2bufw;\
	ULONG					r2bufmask,r2bufsize;\
	double					rptr;\
	ULONG					rpos;\
	ULONG					spos,dpos;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef RESAMPLE_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_flt(SFXCoreInstance *instance,unsigned long samples);
	extern void process_noflt(SFXCoreInstance *instance,unsigned long samples);
	extern void process_rest(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* RESAMPLE_H */

//-- eof ----------------------------------------------------------------------
