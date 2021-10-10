/****h* SurroundEncoder/SurroundEncoder.h [4.2] *
*
*  NAME
*    SurroundEncoder.h
*  COPYRIGHT
*    $VER: SurroundEncoder.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SurroundEncoder core header file
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

#ifndef  SURROUNDENCODER_H
#define  SURROUNDENCODER_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	WinFunction				wfkt1;	/* Phase Fensterfunktion */\
	WinFunction				wfkt2;	/* Rear Fensterfunktion */\
	UBYTE					mode;	/* Invert or Phaseshift */\
	Param					p_phnr;\
	UBYTE					rearfilt;/* On/Off */\
	Param					p_renr;\
	\
	UWORD					phnr,renr;\
	double					fc2;\
	SAMPLE					*r1buf,*r1bufbase,*r1bufw;\
	ULONG					r1bufmask,r1bufsize;\
	SAMPLE					*r2buf,*r2bufbase,*r2bufw;\
	ULONG					r2bufmask,r2bufsize;\
	double					*phmatrix1,*phmatrix2,*rematrix;\
	double					*phwin_fkt,*rewin_fkt;\
	\
	SAMPLE					*dbuf0,*dbuf1,*sbuf0,*sbuf1,*sbuf2,*sbuf3;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef SURROUNDENCODER_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_0(SFXCoreInstance *instance,unsigned long samples);
	extern void process_1(SFXCoreInstance *instance,unsigned long samples);
	extern void process_2(SFXCoreInstance *instance,unsigned long samples);
	extern void process_3(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* SURROUNDENCODER_H */

//-- eof ----------------------------------------------------------------------
