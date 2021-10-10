/****h* Swap/Swap.h [4.2] *
*
*  NAME
*    Swap.h
*  COPYRIGHT
*    $VER: Swap.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Swap core header file
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

#ifndef  SWAP_H
#define  SWAP_H

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
	Modulator				mod2;	/* Swap Range */\
	\
	double					effs,effe,effd,eff,dry;\
	ULONG					rngs,rnge,rngd,rng,mrng,off;\
	ULONG					rpos,spos,dpos;\
	SAMPLE					*rbufw,*rbufr1,*rbufr2;\
	SAMPLE					*rbuf,*rbufbase;\
	ULONG					rbufmask,rbufsize;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef SWAP_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process(SFXCoreInstance *instance,unsigned long samples);
	extern void process_rest(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* SWAP_H */

//-- eof ----------------------------------------------------------------------
