/****h* Mix-3D/Mix-3D.h [4.2] *
*
*  NAME
*    Mix-3D.h
*  COPYRIGHT
*    $VER: Mix-3D.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Mix-3D core header file
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

#ifndef  MIX_3D_H
#define  MIX_3D_H

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
	Source					src3;	/* Quelle */\
	Source					src4;	/* Quelle */\
	Source					src5;	/* Quelle */\
	Source					src6;	/* Quelle */\
	Source					src7;	/* Quelle */\
	Source					src8;	/* Quelle */\
	Modulator				mod1;	/* X-Axis */\
	Modulator				mod2;	/* X-Axis */\
	Modulator				mod3;	/* X-Axis */\
	UBYTE 					view;\
	UBYTE					prec;\
	\
	double					axis_xs,axis_xe,axis_x1,axis_x2,axis_xd;\
	double					axis_ys,axis_ye,axis_y1,axis_y2,axis_yd;\
	double					axis_zs,axis_ze,axis_z1,axis_z2,axis_zd;\
	ULONG					en1,en2,en3,en4,en5,en6,en7,en8;\
	\
	SAMPLE					*dbuf,*sbuf1,*sbuf2,*sbuf3,*sbuf4,*sbuf5,*sbuf6,*sbuf7,*sbuf8;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef MIX_3D_C
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

#endif   /* MIX_3D_H */

//-- eof ----------------------------------------------------------------------
