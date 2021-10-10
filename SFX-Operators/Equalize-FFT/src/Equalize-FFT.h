/****h* Equalize-FFT/Equalize-FFT.h [4.2] *
*
*  NAME
*    Equalize-FFT.h
*  COPYRIGHT
*    $VER: Equalize-FFT.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Equalize-FFT core header file
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

#ifndef  EQUALIZE_FFT_H
#define  EQUALIZE_FFT_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define MAX_BANDS	512

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	WinFunction				wfkt1;	/* Fensterfunktion */\
	double					eqf[65];/* EQ-Shape */\
	UBYTE					mode;	/* All/Akt */\
	UBYTE					bands;\
	UBYTE					steps;\
	Param					p_zeros;/* Pad x % with zeros */\
	Param					eqfstr;\
	Source					asrc;	/* Source für Kurvenanpassung (Noiseprint) */\
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
	double					zeros;\
	ULONG					pos;\
	double					teqf[MAX_BANDS+2];\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef EQUALIZE_FFT_C
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

#endif   /* EQUALIZE_FFT_H */

//-- eof ----------------------------------------------------------------------
