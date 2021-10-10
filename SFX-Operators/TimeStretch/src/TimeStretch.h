/****h* TimeStretch/TimeStretch.h [4.3] *
*
*  NAME
*    TimeStretch.h
*  COPYRIGHT
*    $VER: TimeStretch.h 4.3 (17.02.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    TimeStretch core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    17.Feb.2004	V 4.3	most recent version
*    22.May.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef TIMESTRETCH_H
#define TIMESTRETCH_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define ENVFC1	0.999995
#define ENVFC2	(1.0-ENVFC1)

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod1;	/* TimeStretch */\
	Param					p_winsize;\
	Param					p_smooth;\
	UBYTE					dyncor;\
	UBYTE					chanlink;\
	\
	double					timefs,timefe,timefd,timef;\
	SAMPLE					*rbuf,*rbufbase,*rbufw;\
	SAMPLE					*renv,*renvbase,*renvw;\
	ULONG					rbufmask,rbufsize;\
	ULONG					winsize,winsize2,winsize3,winsize4;\
	ULONG					smrng,smct;\
	ULONG					rlen;\
	double					rptr1,rptr2;\
	ULONG					wptr1,wptr2;\
	ULONG					spos,dpos,rest;\
	double					ampenv;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef TIMESTRETCH_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_seek(SFXCoreInstance *instance,unsigned long samples);
	extern void process_seek_dyncor(SFXCoreInstance *instance,unsigned long samples);
	extern void process_seek_rest(SFXCoreInstance *instance,unsigned long samples);
	extern void process_noseek(SFXCoreInstance *instance,unsigned long samples);
	extern void process_noseek_dyncor(SFXCoreInstance *instance,unsigned long samples);
	extern void process_noseek_rest(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#else	
	double __inline SeekContinue_0(ULONG curlen2,double rptr1,SAMPLE *rbufw,SAMPLE *rbufbase,ULONG rbufsize,ULONG rbufmask,ULONG winsize,ULONG winsize3,ULONG winsize4,BYTE dir);
	//double __inline SeekContinue_1(ULONG curlen2,double rptr1,SAMPLE *rbufw,SAMPLE *rbufbase,ULONG rbufsize,ULONG rbufmask,ULONG winsize,ULONG winsize3,ULONG winsize4,BYTE dir);
#endif

#endif   /* TIMESTRETCH_H */

//-- eof ----------------------------------------------------------------------
