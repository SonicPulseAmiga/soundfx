/****h* Hall/Hall.h [4.3] *
*
*  NAME
*    Hall.h
*  COPYRIGHT
*    $VER: Hall.h 4.3 (29.01.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Hall core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    29.Jan.2004	V 4.3	most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  HALL_H
#define  HALL_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h>
#include <libraries/sfxoperator.h>

//-- defines ------------------------------------------------------------------

#define AANZ 4

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;				/* Quelle */\
	Modulator				mod1;				/* Effektanteil */\
	Modulator				mod2;				/* feedback, early reflections */\
	Modulator				mod3;				/* volume, early reflections */\
	Modulator				mod4;				/* feedback, main hall */\
	Param 					p_erdels,p_erdele;	/* early reflections start & end */\
	Param					p_ernr;				/* number of echos */\
	Param 					p_mrdels,p_mrdele;	/* main reflections start & end */\
	Param					p_mrnr;				/* number of echos */\
	Param 					p_drfc;				/* diffusor (allpass) */\
	Param 					p_ampf;\
	Param					p_bright;			/* controls the main reflections filter */\
	\
	double					effs,effe,effd,eff,dry;\
	double					erfbs,erfbe,erfbd,erfb,erdr;\
	double					mrfbs,mrfbe,mrfbd,mrfb,mrdr;\
	double					ervols,ervole,ervold;\
	double					out,out1,out2,out3,in;\
	double					erdels,erdele,erdeld,erdel;\
	double					mrdels,mrdele,mrdeld,mrdel;\
	double					drfc,ampf,bright;\
	\
	ULONG					vanz;				/* er */\
	ULONG					*delv,delv0;\
	double					*faccv;\
	\
	ULONG					kanz;				/* mr */\
	ULONG					*delk,*mdelk;\
	double					*facbk;\
	double					*fbbk;\
	\
	ULONG					mdelv;\
	ULONG					dela[AANZ],mdela[AANZ];\
	double					facda[AANZ],facdaa[AANZ];\
	double					*bufv,**bufk,*bufa[AANZ];\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef HALL_C
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

#endif   /* HALL_H */

//-- eof ----------------------------------------------------------------------
