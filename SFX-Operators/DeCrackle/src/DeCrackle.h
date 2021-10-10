/****h* DeCrackle/DeCrackle.h [4.3] *
*
*  NAME
*    DeCrackle.h
*  COPYRIGHT
*    $VER: DeCrackle.h 4.3 (05.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    DeCrackle core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    05.Mar.2004	V 4.3	most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  DECRACKLE_H
#define  DECRACKLE_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define ENVFC_A1  0.8
#define ENVFC_A2  (1.0-ENVFC_A1)

#define ENVFC_D1  0.5
#define ENVFC_D2  (1.0-ENVFC_D1)

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Param					p_threshd,p_thresha;\
	Param					p_adj;\
	Param					p_cracklesize;\
	\
	double					threshd,thresha,adj;\
	LONG					lval,sval;\
	double					amp,dif,nrj;\
	double					samp,sdif,snrj;\
	SAMPLE					*rbuf,*rbufbase,*rbufw,*rbufr;\
	ULONG					rbufmask,rbufsize;\
	ULONG					cracklesize;\
	ULONG					crackle_len_ct;\
	BOOL					crackle_scan;\
	ULONG					fadesize;\
	ULONG					crackle_ct,crackle_cts[CH_MAX];\
	ULONG					spos,dpos,rest;\
	double 					low,mid,high;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef DECRACKLE_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_dry(SFXCoreInstance *instance,unsigned long samples);
	extern void process_dry_rest(SFXCoreInstance *instance,unsigned long samples);
	extern void process(SFXCoreInstance *instance,unsigned long samples);
	extern void process_rest(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* DECRACKLE_H */

//-- eof ----------------------------------------------------------------------
