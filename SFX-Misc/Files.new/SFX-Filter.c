/******************************************************************************
** SFX-Filter.c                                                              **
**---------------------------------------------------------------------------**
** SFX-FilterModule                                                          **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 08.08.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

/*
** S. 80 ff
**
** FIR
**	Filter 1. Grades : 
**
** 		y[n]=(c[0]*x[n]+c[1]*x[n-1]);
**
**		      c[0]*z+c[1]
**		H1(z)=-----------
**		           z
**
**	Filter 2. Grades
**
**		y[n]=(c[0]*x[n]+c[1]*x[n-1]+c[2]*x[n-2]);
**
**		      c[0]*z^2+c[1]*z+c[2]
**		H2(z)=--------------------
**		           z^2
**
** Formeln f?r Koeffizientenberechnung :
** -------------------------------------
** fs=Signalfrequenz
** fg=Grenzfrequenz
** f1,f2=Grenzfrequenzen
**
** k=0..N
**
** c(k)=h(k)*win(k)
** win(k)=Fensterfunktion z.B. cos^2
**
** * Tiefpass:.............................................
**
**                  2*pi*fg
**             sin (-------*k)
**      2*fg        fs
** h(k)=---- * ----------------
**      fs          2*pi*fg
**                  -------*k
**                  fs
** * Hochpass:.............................................
**
**          2*fg
** h(0)=1 - ----
**          fs
**
**                   2*pi*fg
**              sin (-------*k)
**      -2*fg        fs
** h(k)=----- * ----------------
**      fs           2*pi*fg
**                   -------*k
**                   fs
**
** * Bandpass:.............................................
**
**                                           pi*(f2-f1)
**                                      sin (----------*k)
**      2*(f2-f1)       pi*(f2+f1)           fs
** h(k)=--------- * cos(----------*k) * ----------------
**      fs              fs                   pi*(f2-f1)
**                                           ----------*k
**                                           fs           
**
** * Bandsperre:...........................................
**
**          2*(f2-f1)
** h(0)=1 - ---------
**          fs
**
**                                            pi*(f2-f1)
**                                       sin (----------*k)
**      -2*(f2-f1)       pi*(f2+f1)           fs
** h(k)=---------- * cos(----------*k) * ----------------
**      fs               fs                   pi*(f2-f1)
**                                            ----------*k
**                                            fs           
**
** IIR
**	Filter 1. Grades : 
**
** 		y[n]=(c[0]*x[n])-(d[1]*y[n-1]);
**
**		      z
**		H1(z)=-----------
**		      z+d[1]
**
**	Filter 2. Grades
**
**		y[n]=(c[0]*x[n])-(d[1]*y[n-1]+d[2]*y[n-2]);
**
**		      z^2
**		H2(z)=--------------------
**		      z^2+d[1]*z+d[2]
**
** BIQU
**	Filter 1. Grades : 
**
** 		y[n]=(c[0]*x[n]+c[1]*x[n-1])			-(d[1]*y[n-1]);
**
**		      c[0]*z+c[1]
**		H1(z)=-----------
**		           z+d[1]
**
**	Filter 2. Grades
**
**		y[n]=(c[0]*x[n]+c[1]*x[n-1]+c[2]*x[n-2])-(d[1]*y[n-1]+d[2]*y[n-2]);
**
**		      c[0]*z^2+c[1]*z+c[2]
**		H2(z)=--------------------
**		           z^2+d[1]*z+d[2]
**
** AllPass ??
**	Filter 1. Grades : 
**
** 		y[n]=(c[1]*x[n])						-(c[1]*y[n-1]);
**
**		      c[1]*z+1
**		H1(z)=-----------
**		           z+c[1]
**
**	Filter 2. Grades
**
**		y[n]=(c[2]*x[n]+c[1]*x[n-1])-(d[1]*y[n-1]+d[2]*y[n-2]);
**
**		      c[2]*z^2+c[1]*z+1
**		H2(z)=--------------------
**		           z^2+c[1]*z+c[2]
**
**
** S. 50 ff ?bergang von H(z) -> H(_o_)
**
** Nullstellen z0 und Polstellen zp berechnen und in 2.59 einsetzen
**
*/


/* Includes und Defines */

#include "SFX-INCLUDES.h"

/* Protos */

WORD Filter_FIR(WORD *datax,double *c,UBYTE n);
WORD Filter_IIR(WORD *datay,double *d,UBYTE n);
WORD Filter_BIQU(WORD *datax,double *c,UBYTE n1,WORD *datay,double *d,UBYTE n);
WORD Filter_COMB();

/* Globale Vars */

extern struct ExecBase 		*SysBase;
extern struct IntuitionBase	*IntuitionBase;
extern struct GfxBase	 	*GfxBase;
extern struct DosLibrary	*DOSBase;
extern struct Library 		*AslBase,*DiskfontBase,*UtilityBase,*AmigaGuideBase,*GadToolsBase;

/* Funktions */

/****************************************************************************************
** FIR - Finite Impulse Response (S.22)                                                **
**-------------------------------------------------------------------------------------**
**                                                                                     **
**    +-(*)----------------------|+|                                                   **
**    |  |                       |+|                                                   **
**    |  c(0)                    |+|                                                   **
**    |     +-(*)----------------|+|                                                   **
**    |     |  |                 |+|                                                   **
**    |     |  c(1)              |+|                                                   **
**    |     |     +-(*)----------|+|                                                   **
**    |     |     |  |           |+|                                                   **
**    |     |     |  c(2)        |+|                                                   **
**    |     |     |              |+|                                                   **
** x -+-[D]-+-[D]-+-...-[D]-(*)--|+|-(*)-- y                                           **
**                           |        |                                                **
**                           c(n)     c(n+1)                                           **
**                                                                                     **
****************************************************************************************/

WORD Filter_FIR(WORD *datax,double *c,UBYTE n)
{
	register UBYTE i;
	double e=0.0;

	for(i=0;i<n;i++)
	{
		e+=(double)(*datax)*(*c);
		datax++;c++;
	}
	return((WORD)(e*(*c)));
}

/****************************************************************************************
** IIR - Infinite Impulse Response (S.22)                                              **
**-------------------------------------------------------------------------------------**
**                                                                                     **
**    |+|-------------(*)-------+                                                      **
**    |+|              |        |                                                      **
**    |+|              d(n)     |                                                      **
**    |+|-------(*)---+         |                                                      **
**    |+|        |    |         |                                                      **
**    |+|        d(1) |         |                                                      **
**    |+|-(*)---+     |         |                                                      **
**    |+|  |    |     |         |                                                      **
**    |+|  d(0) |     |         |                                                      **
**    |+|       |     |         |                                                      **
** x -|+|-+-[D]-+-[D]-+-...-[D]-+  d(n+1)                                              **
**        |                        |                                                   **
**        +-----------------------(*)-- y                                              **
**                                                                                     **
** VORHER SOURCE-SAMPLE in ZIELPUFFER UMKOPIEREN !!!!                                  **
**                                                                                     **
****************************************************************************************/

WORD Filter_IIR(WORD *datay,double *d,UBYTE n)
{
	register UBYTE i;
	double e=0.0;

	for(i=0;i<n;i++)
	{
		e+=(double)(*datay)*(*d);
		datay++;d++;
	}
	return((WORD)(e*(*d)));
}

/****************************************************************************************
** BIQU - BiQuad (FIR+IIR) (S.22)                                                      **
**-------------------------------------------------------------------------------------**
**                                                                                     **
**    +-(*)----------------------|+|        |+|-------------(*)-------+                **
**    |  |                       |+|        |+|              |        |                **
**    |  c(0)                    |+|        |+|              d(n)     |                **
**    |     +-(*)----------------|+|        |+|-------(*)---+         |                **
**    |     |  |                 |+|        |+|        |    |         |                **
**    |     |  c(1)              |+|        |+|        d(1) |         |                **
**    |     |     +-(*)----------|+|        |+|-(*)---+     |         |                **
**    |     |     |  |           |+|        |+|  |    |     |         |                **
**    |     |     |  c(2)        |+|        |+|  d(0) |     |         |                **
**    |     |     |              |+|        |+|       |     |         |                **
** x -+-[D]-+-[D]-+-...-[D]-(*)--|+|-(*)----|+|-+-[D]-+-[D]-+-...-[D]-+  d(n+1)        **
**                           |        |         |                        |             **
**                           c(n)     c(n+1)    +-----------------------(*)-- y        **
**                                                                                     **
****************************************************************************************/

WORD Filter_BIQU(WORD *datax,double *c,UBYTE n1,WORD *datay,double *d,UBYTE n2)
{
	register UBYTE i;
	double e1=0.0,e2=0.0;

	for(i=0;i<n1;i++)
	{
		e1+=(double)(*datax)*(*c);
		datax++;c++;
	}
	datay[n2-1]=(BYTE)(e1*=(*c));
	for(i=0;i<n2;i++)
	{
		e2+=(double)(*datay)*(*d);
		datay++;d++;
	}
	return((WORD)(e2/(*d)));
}

/****************************************************************************************
** COMB - Kammfilter                                                                   **
**-------------------------------------------------------------------------------------**
**                                                                                     **
****************************************************************************************/

WORD Filter_COMB()
{
}
