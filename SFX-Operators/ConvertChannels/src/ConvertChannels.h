/****h* ConvertChannels/ConvertChannels.h [4.2] *
*
*  NAME
*    ConvertChannels.h
*  COPYRIGHT
*    $VER: ConvertChannels.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    ConvertChannels core header file
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

#ifndef  CONVERTCHANNELS_H
#define  CONVERTCHANNELS_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Param					mat[CH_MAX][CH_MAX];	/* Verknüpfungmatrix */\
	\
	double					mat0[CH_MAX],mat1[CH_MAX],mat2[CH_MAX],mat3[CH_MAX];\
	UBYTE					srcch;\
	\
	SAMPLE					*sbuf[CH_MAX],*dbuf[CH_MAX];

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef CONVERTCHANNELS_C
	extern int init(void);
	extern int instantiate(SFXCoreInstance *instance);
	extern int activate(SFXCoreInstance *instance);
	extern void activate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void process_m(SFXCoreInstance *instance,unsigned long samples);
	extern void process_s(SFXCoreInstance *instance,unsigned long samples);
	extern void process_q(SFXCoreInstance *instance,unsigned long samples);
	extern void deactivate_ch(SFXCoreInstance *instance,unsigned char ch);
	extern void deactivate(SFXCoreInstance *instance);
	extern void destroy(SFXCoreInstance *instance);
	extern void done(void);
#endif

#endif   /* CONVERTCHANNELS_H */

//-- eof ----------------------------------------------------------------------
