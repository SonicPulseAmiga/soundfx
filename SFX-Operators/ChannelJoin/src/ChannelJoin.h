/****h* ChannelJoin/ChannelJoin.h [4.2] *
*
*  NAME
*    ChannelJoin.h
*  COPYRIGHT
*    $VER: ChannelJoin.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    ChannelJoin core header file
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

#ifndef  CHANNELJOIN_H
#define  CHANNELJOIN_H

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
	\
	SAMPLE					*dbuf0,*dbuf1,*dbuf2,*dbuf3;\
	SAMPLE					*sbuf0,*sbuf1,*sbuf2,*sbuf3;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef CHANNELJOIN_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_m2s(SFXCoreInstance *instance,unsigned long samples);
	extern void process_s2q(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* CHANNELJOIN_H */

//-- eof ----------------------------------------------------------------------
