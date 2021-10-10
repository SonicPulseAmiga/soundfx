/****h* DeNoise-FFT/DeNoise-FFT.h [4.2] *
*
*  NAME
*    DeNoise-FFT.h
*  COPYRIGHT
*    $VER: DeNoise-FFT.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    DeNoise-FFT core header file
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

#ifndef  DENOISE_FFT_H
#define  DENOISE_FFT_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define ENVFC1	0.995
#define ENVFC2	(1.0-ENVFC1)

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	WinFunction				wfkt1;	/* Fensterfunktion */\
	Param					p_attack;\
	Param					p_thresh;\
	UBYTE					shape;\
	UBYTE					bands;\
	UBYTE					steps;\
	\
	SAMPLE					*irbuf,*irbufbase;\
	ULONG					irbufmask,irbufsize;\
	SAMPLE					*orbuf,*orbufbase;\
	ULONG					orbufmask,orbufsize;\
	SAMPLE					*irbufw,*orbufw;\
	UWORD					mb,mw,ms,mv;\
	double					*flbuf_r,*flbuf_i;\
	double					*win_fkt1,*win_fkt2;\
	UWORD					*bitrevtab;\
	double					*sintab1,*sintab2,*costab;\
	double					thresh;\
	ULONG					pos,attack;\
	double					*ampenv,*fadearr;\
	UBYTE					*fo;\
	ULONG					*fop;\
	double					expon;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef DENOISE_FFT_C
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

#endif   /* DENOISE_FFT_H */

//-- eof ----------------------------------------------------------------------
