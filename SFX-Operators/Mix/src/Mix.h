/****h* Mix/Mix.h [4.2] *
*
*  NAME
*    Mix.h
*  COPYRIGHT
*    $VER: Mix.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Mix core header file
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

#ifndef  MIX_H
#define  MIX_H

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
	Modulator				mod;	/* Mixpropotion src1 */\
	Param					p_delay2;\
	\
	double					mix1s,mix1e,mix1d;\
	ULONG					delay2;\
	SAMPLE					*rbuf,*rbufbase,*rbufr,*rbufw;\
	ULONG					rbufmask,rbufsize;\
	\
	SAMPLE					*dbuf,*sbuf1,*sbuf2;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef MIX_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_0(SFXCoreInstance *instance,unsigned long samples);
	extern void process_1(SFXCoreInstance *instance,unsigned long samples);
	extern void process_2(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* MIX_H */

//-- eof ----------------------------------------------------------------------
