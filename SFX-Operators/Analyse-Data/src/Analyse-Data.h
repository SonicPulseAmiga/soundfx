/****h* Analyse-Data/Analyse-Data.h [4.2] *
*
*  NAME
*    Analyse-Data.h
*  COPYRIGHT
*    $VER: Analyse-Data.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Analyse-Data core header file
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

#ifndef ANALYSE_DATA_H
#define ANALYSE_DATA_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	\
	ULONG					hista [CH_MAX][128];/* histogramm amplitude */\
	ULONG					histd [CH_MAX][256];/* histogramm deltas */\
	LONG					maxa  [CH_MAX];		/* maximum amplitude */\
	LONG					maxd  [CH_MAX];		/* maximum delta */\
	LONG					mins  [CH_MAX];		/* min sample */\
	LONG					maxs  [CH_MAX];		/* max sample */\
	double					avg   [CH_MAX];		/* avarage volume */\
	double					rms   [CH_MAX];		/* root mean square volume */\
	double					dc    [CH_MAX];		/* sample offset */\
	LONG					avgdif[CH_MAX];		/* average delta */\
	ULONG					pcs   [CH_MAX];		/* probably clipped samples */\
	UBYTE					numch;\
	ULONG					*chista,*chistd,cpcs;\
	LONG					cmaxa,cmaxd;\
	LONG					cmins,cmaxs;\
	double					cavg,crms,cdc,cavgdif;\
	LONG					lstv;\
	double					div1,div2;\
	\
	SAMPLE					*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef ANALYSE_DATA_C
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

#endif   /* ANALYSE_DATA_H */

//-- eof ----------------------------------------------------------------------
