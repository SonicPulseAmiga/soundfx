/****h* SurroundDecoder/SurroundDecoder.h [4.3] *
*
*  NAME
*    SurroundDecoder.h
*  COPYRIGHT
*    $VER: SurroundDecoder.h 4.3 (22.08.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SurroundDecoder core header file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    22.Aug.2003
*  MODIFICATION HISTORY
*    22.Aug.2003	V 4.3	most recent version
*    22.Aug.2003	V 4.3	initial version
*  NOTES
*
*******
*/

#ifndef  SOUROUNDDECODER_H
#define  SURROUNDDECODER_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

#define MODE_PASSIVE	0
#define MODE_ACTIVE		1

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;		/* Quelle */\
	UBYTE					mode;\
	\
	SAMPLE					*dbuf0,*dbuf1,*dbuf2,*dbuf3,*sbuf0,*sbuf1;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef SURROUNDDECODER_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_passive(SFXCoreInstance *instance,unsigned long samples);
	extern void process_active(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* SURROUNDDECODER_H */

//-- eof ----------------------------------------------------------------------
