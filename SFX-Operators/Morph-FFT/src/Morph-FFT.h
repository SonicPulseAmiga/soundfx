/****h* Morph-FFT/Morph-FFT.h [4.2] *
*
*  NAME
*    Morph-FFT.h
*  COPYRIGHT
*    $VER: Morph-FFT.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Morph-FFT core header file
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

#ifndef  MORPH_FFT_H
#define  MORPH_FFT_H

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
	Modulator				mod;	/* Eff */\
	WinFunction				wfkt1;	/* Fensterfunktion */\
	Param					p_ampf;\
	UBYTE					bands;	/* Anzahl der Frequenzbänder */\
	UBYTE					steps;\
	\
	double					mors,more,mord,mor;\
	double					ampf;\
	UWORD					m,mm,m2;\
	double					*flbuf1_r,*flbuf1_i;\
	double					*flbuf2_r,*flbuf2_i;\
	double					*flbuf3_r,*flbuf3_i;\
	double					*win_fkt1,*win_fkt2;\
	SAMPLE					*irbuf1,*irbufbase1;\
	SAMPLE					*irbuf2,*irbufbase2;\
	ULONG					irbufmask,irbufsize;\
	SAMPLE					*orbuf,*orbufbase;\
	ULONG					orbufmask,orbufsize;\
	SAMPLE					*irbufw1,*irbufw2,*orbufw;\
	UWORD					*bitrevtab;\
	double					*sintab1,*sintab2,*costab;\
	ULONG					pos;\
	\
	SAMPLE					*dbuf,*sbuf1,*sbuf2;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef MORPH_FFT_C
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

#endif   /* MORPH_FFT_H */

//-- eof ----------------------------------------------------------------------
