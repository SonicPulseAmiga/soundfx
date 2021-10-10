/******************************************************************************
** wfkt.c                                                                    **
**---------------------------------------------------------------------------**
** WindowFunktion Tester                                                     **
**---------------------------------------------------------------------------**
** Version : V 0.1                                                           **
** Date    : 17.11.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

/*-- includes & defines -------------------------------------------------------------------------*/

#define __USE_SYSBASE

#include <exec/types.h>
#include <dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <dos/dostags.h>
#include <exec/devices.h>
#include <exec/exec.h>
#include <exec/lists.h>
#include <exec/memory.h>
#include <exec/nodes.h>
#include <graphics/gfx.h>
#include <graphics/gfxmacros.h>
#include <graphics/display.h>
#include <graphics/displayinfo.h>
#include <intuition/gadgetclass.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/imageclass.h>
#include <intuition/icclass.h>
#include <intuition/screens.h>
#include <intuition/sghooks.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

// Windowfunktionen

#define WIN_RECTANGLE	0
#define WIN_BARTLETT	1
#define WIN_FEJER		2
#define WIN_WELCH		3
#define WIN_HANNING		4
#define WIN_HAMMING		5
#define WIN_BLACKMAN	6
#define WIN_KAISER		7
#define WIN_HALFSINE	8
#define WIN_HALFSINEQ	9
#define WIN_HALFSINES	10

#define SIZE 256
#define FFT_M 128

//#define SIZE 64
//#define FFT_M 32

/*-- protos -------------------------------------------------------------------------------------*/

void  Transform(double *fftbuf_r,double *fftbuf_i,UWORD *BitRevTab,BYTE dir,UWORD m);
void  InitBitRev(UWORD *BitRevTab,UWORD m);
UWORD BitRev(UWORD in,UWORD nu);

void GenWindow(UWORD len,UBYTE typ,double *buf);
double KaiserI(double x);

struct Window *DrawData(double *data,UWORD skip,ULONG size,char *title);

/*-- external functions -------------------------------------------------------------------------*/

extern void __regargs __asm __chkabort(void);

/*-- external variables -------------------------------------------------------------------------*/

/*-- global variables ---------------------------------------------------------------------------*/

/*-- functions ----------------------------------------------------------------------------------*/

void Transform(double *fftbuf_r,double *fftbuf_i,UWORD *BitRevTab,BYTE dir,UWORD m)
{
	register UBYTE  l;
	register UWORD j,k;
	UWORD *brtptr;
	UWORD i1,i2,i3,i4;
	UWORD mm=m+m;
	double mmd=(double)mm;
	register WORD p,nu=0,nu1=m;
	double pi2f=6.283185307/mmd,arc,c,s;
	double tmp_r,tmp_i;
	double *ibuf_r,*ibuf_i,*kbuf_r,*kbuf_i;

	while(nu1) { nu1>>=1;nu++; }		/* nu=log2(m)+1; */

	i1=m;
	i2=1;
	for(l=0;l<nu;l++)
	{
		i3=0;i4=i1;
		brtptr=BitRevTab;
		for(j=0;j<i2;j++)
		{
			arc=pi2f*(double)(*brtptr);brtptr+=2;
			c=cos(arc);s=(double)dir*sin(arc);				// sintab, costab
			ibuf_r=&fftbuf_r[i3+i1];ibuf_i=&fftbuf_i[i3+i1];
			kbuf_r=&fftbuf_r[i3];kbuf_i=&fftbuf_i[i3];
			for(k=i3;k<i4;k++)
			{
				tmp_r=c*(*ibuf_r)-s*(*ibuf_i);
				tmp_i=c*(*ibuf_i)+s*(*ibuf_r);
				*(ibuf_r++)=(*kbuf_r)-tmp_r;
				*(ibuf_i++)=(*kbuf_i)-tmp_i;
				*(kbuf_r++)+=tmp_r;
				*(kbuf_i++)+=tmp_i;
			}
			i3+=(i1<<1);
			i4+=(i1<<1);
		}
		i1>>=1;
		i2<<=1;
	}

	kbuf_r=fftbuf_r;kbuf_i=fftbuf_i;
	brtptr=BitRevTab;
	for(k=0;k<mm;k++)
	{
		if((p=*(brtptr++))>k)
		{
			tmp_r=*kbuf_r;				/* swap real */
			*kbuf_r=fftbuf_r[p];
			fftbuf_r[p]=tmp_r;
			tmp_i=*kbuf_i;				/* swap imag */
			*kbuf_i=fftbuf_i[p];
			fftbuf_i[p]=tmp_i;
		}
		kbuf_r++;kbuf_i++;
	}

	if(dir==-1)
	{
		kbuf_r=fftbuf_r;kbuf_i=fftbuf_i;
		for(k=0;k<mm;k++) { *(kbuf_r++)/=mmd;*(kbuf_i++)/=mmd; }
	}
}

void InitBitRev(UWORD *BitRevTab,UWORD m)
{
	register UWORD i;
	UWORD mm=m<<1;
	UWORD nu=0,nu1=m;

	while(nu1) { nu1>>=1;nu++; }		/* nu=log2(mm); */

//	for(i=0;i<mm;i++) BitRevTab[i]=BitRev(i,nu);
	for(i=0;i<mm;i++) *(BitRevTab++)=BitRev(i,nu);
}

UWORD BitRev(UWORD in,UWORD nu)
{
	register UBYTE h,h2;
	register UWORD out=0;

	for(h=0,h2=(nu-1);h<nu;h++,h2--)
	{
		out|=((in&(1<<h))>>h)<<h2;
	}
	return(out);
}

void GenWindow(UWORD len,UBYTE typ,double *buf)
{
	register UWORD i,len2=len>>1,len3=len-1;
	double l=(double)len;
	double pih=3.141592654/l,pih2=2.0*pih,pih4=4.0*pih;
	double ak,t;

	switch(len)
	{
		case 1:
			buf[0]=1.0;
			break;
		case 2:
			buf[0]=0.5;
			buf[1]=0.5;
			break;
		default:
			switch(typ)
			{
				case WIN_RECTANGLE:
					printf("Rectangle ...\n");
					for(i=0;i<len;i++) buf[i]=1.0;
					break;
				case WIN_BARTLETT:
					printf("Bartlett ...\n");
					for(i=0;i<len2;i++) buf[len3-i]=buf[i]=(double)(i<<1)/l;
					break;
				case WIN_FEJER:
					printf("Fejer ...\n");
					for(i=0;i<len2;i++) buf[len3-i]=buf[i]=0.5+(double)i/l;
					break;
				case WIN_WELCH:
					printf("Welch ...\n");
					t=len2*len2;
					for(i=0;i<len2;i++) buf[len2+i]=buf[len2-i]=1.0-(i*i/t);
					break;
				case WIN_HANNING:
					printf("Hanning ...\n");
					for(i=0;i<len2;i++) buf[len3-i]=buf[i]=0.5-0.5*cos(pih2*i);
					break;
				case WIN_HAMMING:
					printf("Hamming ...\n");
					for(i=0;i<len2;i++) buf[len3-i]=buf[i]=0.54-0.46*cos(pih2*i);
					break;
				case WIN_BLACKMAN:
					printf("Blackman ...\n");
					for(i=0;i<len2;i++) buf[len3-i]=buf[i]=0.42-0.5*cos(pih2*i)+0.08*cos(pih4*i);
					break;
				case WIN_KAISER:
					printf("Kaiser ...\n");
					ak=KaiserI(3.3);
//					printf("ak : %10.8lf\n",ak);
					for(i=0;i<len2;i++)
					{
						t=((double)i/l);
						buf[len3-i]=buf[i]=KaiserI(6.6*sqrt(t-(t*t)))/ak;
//						printf("%3d : %6.4lf\n",i,buf[i]);
					}
					break;
				case WIN_HALFSINE:
					printf("Halfsine ...\n");
					for(i=0;i<len2;i++) buf[len3-i]=buf[i]=sin((double)i*pih);
					break;
				case WIN_HALFSINEQ:
					printf("HalfsineQ ...\n");
					for(i=0;i<len2;i++) buf[len3-i]=buf[i]=(t=sin((double)i*pih))*t;
					break;
				case WIN_HALFSINES:
					printf("HalfsineS ...\n");
					for(i=0;i<len2;i++) buf[len3-i]=buf[i]=sqrt(sqrt(sin((double)i*pih)));
					break;
			}
			break;
	}
}

double KaiserI(double x)
{
	register UBYTE v;
	double vf[11]={1.0,2.0,6.0,24.0,120.0,720.0,5040.0,40320.0,362880.0,3628800.0,39916800.0 };
	double sum=0.0,x2=x/2.0,x2i=x2;

	for(v=0;v<11;v++)		/* normalerweise v=1...inf. */
	{
		sum+=(x2/vf[v]);
		x2*=x2i;
	}
	return(sum);
}

struct Window *DrawData(double *data,UWORD skip,ULONG size,char *title)
{
	register UWORD i;
	struct Window *win;
	struct RastPort *rp;
	double off,scl,mi,ma;

	if((win=OpenWindowTags(0l,WA_InnerWidth,size,WA_InnerHeight,size,WA_IDCMP,IDCMP_CLOSEWINDOW,WA_Flags,WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO,WA_Title,title,TAG_DONE)))
	{
		rp=win->RPort;
		SetRast(rp,2);SetAPen(rp,1);
		mi=99999;ma=-99999;
		if(skip>1)
		{
			for(i=0;i<size/skip;i++)					// find max and min
			{
				if(data[i]<mi) mi=data[i];
				if(data[i]>ma) ma=data[i];
			}
			scl=(double)size/(ma-mi);					// scale & move
			off=(double)size+(scl*mi);
//			printf("mi  %6.4lf  ma  %6.4lf\n",mi,ma);
//			printf("scl %6.4lf  off %6.4lf\n",scl,off);
		}
		else
		{
			scl=off=(double)size;
		}
		Move(rp,0,size);							// draw
		for(i=0;i<size;i+=skip)
		{
//			printf("win[%2d]=%10.8lf\n",i/skip,data[i/skip]);
			Draw(rp,i,(ULONG)(off-(scl*data[i/skip])));
		}
		Draw(rp,size,size);
	}
	return(win);
}

void main(int argc,char *argv[])
{
	register UWORD i;
	double buf_r[SIZE*2],buf_i[SIZE*2];
	UWORD brt[SIZE*2];
	UBYTE winnr=WIN_RECTANGLE;
	struct Window *win1,*win2;
	struct IntuiMessage *imsg;
	ULONG maskin,maskout,sig1=0,sig2=0;
	UBYTE end=0;

	if(argc>1) winnr=atoi(argv[1]);

	GenWindow(SIZE,winnr,buf_r);
	for(i=0;i<SIZE;i++)
	{
//		printf("win[%2d]=%6.4lf\n",i,buf_r[i]);
		buf_i[i]=0;
	}
	if((win1=DrawData(buf_r,1,SIZE,"Fensterfkt : Zeitbereich"))) sig1=(1L<<win1->UserPort->mp_SigBit);
	InitBitRev(brt,FFT_M);
	Transform(buf_r,buf_i,brt,-1,FFT_M);
	for(i=0;i<SIZE;i++)
	{
		buf_r[i]=20.0*log(sqrt(buf_r[i]*buf_r[i]+buf_i[i]*buf_i[i]))/log(10.0);
//		buf_r[i]*=buf_r[i];
	}
	if((win2=DrawData(buf_r,2,SIZE,"Fensterfkt : Bildbereich"))) sig2=(1L<<win2->UserPort->mp_SigBit);

	if((maskin=sig1|sig2))
	{
		while(!end)
		{
			maskout=Wait(maskin);
			if(maskout&sig1)
			{
				imsg=(struct IntuiMessage *)GetMsg(win1->UserPort);
				if(imsg->Class==IDCMP_CLOSEWINDOW) end=1;
				ReplyMsg((struct Message *)imsg);
			}
			if(maskout&sig2)
			{
				imsg=(struct IntuiMessage *)GetMsg(win2->UserPort);
				if(imsg->Class==IDCMP_CLOSEWINDOW) end=1;
				ReplyMsg((struct Message *)imsg);
			}
		}
	}

	if(win1) CloseWindow(win1);
	if(win2) CloseWindow(win2);
}

/*-- EOF ----------------------------------------------------------------------------------------*/
