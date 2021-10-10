/****h* MultiDelay/MultiDelay.h [4.2] *
*
*  NAME
*    MultiDelay.h
*  COPYRIGHT
*    $VER: MultiDelay.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    MultiDelay core header file
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

#ifndef  MULTIDELAY_H
#define  MULTIDELAY_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define MAX_DELAYS	8

struct delay {
	Param			del,fbl,amp,fbg;
};

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	struct delay			delays[MAX_DELAYS];\
	Param 					p_ampf;\
	Param					p_dry;\
	UBYTE					anz;\
	\
	double					ampf;\
	double					dry;\
	SAMPLE					*rbuf[MAX_DELAYS],*rbufbase[MAX_DELAYS];\
	ULONG					delay[MAX_DELAYS],rbufmask[MAX_DELAYS],rbufsize[MAX_DELAYS];\
	double					fbl[MAX_DELAYS],fbg[MAX_DELAYS],dr[MAX_DELAYS],amp[MAX_DELAYS];\
	double					out2[MAX_DELAYS];\
	SAMPLE					*rbufw[MAX_DELAYS],*rbufr[MAX_DELAYS];\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef MULTIDELAY_C
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

#endif   /* MULTIDELAY_H */

//-- eof ----------------------------------------------------------------------
