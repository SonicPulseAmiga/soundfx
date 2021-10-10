/****h* PitchShift/PitchShift.h [4.3] *
*
*  NAME
*    PitchShift.h
*  COPYRIGHT
*    $VER: PitchShift.h 4.3 (17.02.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    PitchShift core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    17.Feb.2004	V 4.3	most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  PITCHSHIFT_H
#define  PITCHSHIFT_H

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
	Modulator				mod2;	/* Pitch */\
	Interpolator			int1;	/* Interpolator */\
	Param					p_winsize;\
	Param					p_smooth;\
	UBYTE					chanlink;\
	\
	double					effs,effe,effd,eff,dry;\
	double					pitchs,pitche,pitchd,pitch;\
	SAMPLE					*rbuf,*rbufbase,*rbufw;\
	ULONG					rbufmask,rbufsize;\
	ULONG					winsize,winsize2,winsize3,winsize4;\
	ULONG					smrng,smct;\
	InterpolFuncPtr			interpolfunc;\
	double					rptr1,rptr2;\
	ULONG					spos,dpos,rest;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef PITCHSHIFT_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_noseek(SFXCoreInstance *instance,unsigned long samples);
	extern void process_noseek_rest(SFXCoreInstance *instance,unsigned long samples);
	extern void process_seek(SFXCoreInstance *instance,unsigned long samples);
	extern void process_seek_rest(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#else	
	static double INLINE SeekContinue_0(ULONG curlen2,double rptr1,SAMPLE *rbufw,SAMPLE *rbufbase,ULONG rbufsize,ULONG rbufmask,ULONG winsize,ULONG winsize3,ULONG winsize4,BYTE dir);
	//static double INLINE SeekContinue_1(ULONG curlen2,double rptr1,SAMPLE *rbufw,SAMPLE *rbufbase,ULONG rbufsize,ULONG rbufmask,ULONG winsize,ULONG winsize3,ULONG winsize4,BYTE dir);
	static double INLINE getRPtr(SAMPLE *rbufw,ULONG rbufmask,ULONG rbufsize,double offs);
#endif

#endif   /* PITCHSHIFT_H */

//-- eof ----------------------------------------------------------------------
