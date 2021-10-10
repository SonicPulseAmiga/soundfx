/****h* Equalize-FFT-3D/Equalize-FFT-3D.h [4.2] *
*
*  NAME
*    Equalize-FFT-3D.h
*  COPYRIGHT
*    $VER: Equalize-FFT-3D.h 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Equalize-FFT-3D core header file
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

#ifndef  EQUALIZE_FFT_3D_H
#define  EQUALIZE_FFT_3D_H

//-- includes -----------------------------------------------------------------

#include <libraries/sfxsupport.h> 
#include <libraries/sfxoperator.h> 

//-- defines ------------------------------------------------------------------

/* platform independent fx data */

#define SFXCoreInstanceFields \
	SFXCommonCoreInstanceFields\
	\
	Source					src;	/* Quelle */\
	Modulator				mod1;	/* X-Axis */\
	Modulator				mod2;	/* X-Axis */\
	Modulator				mod3;	/* X-Axis */\
	WinFunction				wfkt1;	/* Fensterfunktion */\
	double					eqf1[65];/* EQ-Shape */\
	double					eqf2[65];/* EQ-Shape */\
	double					eqf3[65];/* EQ-Shape */\
	double					eqf4[65];/* EQ-Shape */\
	double					eqf5[65];/* EQ-Shape */\
	double					eqf6[65];/* EQ-Shape */\
	double					eqf7[65];/* EQ-Shape */\
	double					eqf8[65];/* EQ-Shape */\
	double *				eqf[8];\
	char					name1[40];\
	char					name2[40];\
	char					name3[40];\
	char					name4[40];\
	char					name5[40];\
	char					name6[40];\
	char					name7[40];\
	char					name8[40];\
	char *					name[8];/* pointer to names */\
	UBYTE 					view;\
	UBYTE					prec;\
	UBYTE					bands;\
	UBYTE					steps;\
	Param					eqfstr;\
	\
	double					axis_xs,axis_xe,axis_x1,axis_x2,axis_xd;\
	double					axis_ys,axis_ye,axis_y1,axis_y2,axis_yd;\
	double					axis_zs,axis_ze,axis_z1,axis_z2,axis_zd;\
	SAMPLE					*irbuf,*irbufbase;\
	ULONG					irbufmask,irbufsize;\
	SAMPLE					*orbuf,*orbufbase;\
	ULONG					orbufmask,orbufsize;\
	SAMPLE					*irbufw,*orbufw;\
	UWORD					mb,mw,ms,mv;\
	double					*flbuf_r,*flbuf_i;\
	double					*win_fkt1,*win_fkt2;\
	UWORD					*bitrevtab;\
	double					*sintab1,*sintab2,*costab;\
	ULONG					pos;\
	\
	SAMPLE					*dbuf,*sbuf;

typedef struct {
	SFXCoreInstanceFields
} SFXCoreInstance;

//-- prototypes ---------------------------------------------------------------

#ifndef EQUALIZE_FFT_3D_C
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

#endif   /* EQUALIZE_FFT_3D_H */

//-- eof ----------------------------------------------------------------------
