// rawtest.c

#include <exec/types.h>
#include <exec/memory.h>
#include <dos.h>
#include <dos/dos.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <proto/dos.h>
#include <proto/exec.h>

ULONG GetFileLength(char *fn);

int test(char *fn);

ULONG GetFileLength(char *fn)
{
	struct FileLock *fl;
	struct FileInfoBlock fib;
	ULONG slen=0;

	if(fl=(struct FileLock *)Lock(fn,ACCESS_READ))
	{
		if(Examine((BPTR)fl,&fib)) slen=fib.fib_Size;
		UnLock((BPTR)fl);
	}
//	slen&=0xFFFFFFFE;		/* make odd */
	return(slen);
}

int test(char *fn)
{
	FILE *infile;
	register int i;
	ULONG len;
	UBYTE *buf;
	UBYTE manr8=0,minr8=0,manr16=0,minr16=0,manr16e=0,minr16e=0;
	WORD bins8[4]={0},mas8=0,mis8=32767;
	WORD bins16[4]={0},mas16=0,mis16=32767;
	WORD bins16e[4]={0},mas16e=0,mis16e=32767;
	double ant8,ver8,div,ant16,ver16,ant16e,ver16e;
	double antr16,antr16e,verr16,verr16e;
	WORD ret=0;
	#define RAW8	1
	#define SRAW8	2
	#define RAW16	3
	#define SRAW16	4
	#define RAW16E	5
	#define SRAW16E	6
	#define ULAW	7
	#define ALAW	8

	char *types[]={ "raw8   ","sraw8  ","raw16  ","sraw16 ","raw16e ","sraw16e","ulaw   ","alaw   " };

	len=GetFileLength(fn);
	len=min(len,5000);			/* maximal die ersten 5000 Bytes lesen */

	if(infile=fopen(fn,"rb"))
	{
		if(buf=(UBYTE *)AllocMem(len,MEMF_ANY))
		{
			if(!(fread(buf,len,1,infile))) { printf("ERROR : can't read data !\n");fclose(infile);return(0); }
			for(i=0;i<len;i++) bins8[buf[i]>>6]++;			// histogramm for 8 bit
			for(i=0;i<len;i+=2)
			{
				bins16[buf[i]>>6]++;						// histogramm for 16 bit
				bins16e[buf[i+1]>>6]++;						// histogramm for 16 bit endian
			}
			FreeMem(buf,len);
		}
		else printf("ERROR : can't alloc mem !\n");
		fclose(infile);
		for(i=0;i<4;i++)
		{
			if(bins8[i]>mas8) { manr8=i;mas8=bins8[i]; }			// min,max for 8 bit
			if(bins8[i]<mis8) { minr8=i;mis8=bins8[i]; }
			if(bins16[i]>mas16) { manr16=i;mas16=bins16[i]; }		// min,max for 16 bit
			if(bins16[i]<mis16) { minr16=i;mis16=bins16[i]; }
			if(bins16e[i]>mas16e) { manr16e=i;mas16e=bins16e[i]; }	// min,max for 16 bit endian
			if(bins16e[i]<mis16e) { minr16e=i;mis16e=bins16e[i]; }
		}
		if(bins8[minr8]!=0) ant8=(double)bins8[manr8]/(double)bins8[minr8];
		else ant8=99999.0;
		div=(double)(bins8[1]+bins8[2]);
		if(div!=0) ver8=(double)(bins8[0]+bins8[3])/div;
		else ver8=99999.0;

		if(bins16[minr16]!=0) ant16=(double)bins16[manr16]/(double)bins16[minr16];
		else ant16=99999.0;
		div=(double)(bins16[1]+bins16[2]);
		if(div!=0) ver16=(double)(bins16[0]+bins16[3])/div;
		else ver16=99999.0;
		verr16=(ver16*100.0)/ver8;
		antr16=(ant16*100.0)/ant8;

		if(bins16e[minr16e]!=0) ant16e=(double)bins16e[manr16e]/(double)bins16e[minr16e];
		else ant16e=99999.0;
		div=(double)(bins16e[1]+bins16e[2]);
		if(div!=0) ver16e=(double)(bins16e[0]+bins16e[3])/div;
		else ver16e=99999.0;
		verr16e=(ver16e*100.0)/ver8;
		antr16e=(ant16e*100.0)/ant8;

		if((antr16>100.0 && antr16<150.0 && antr16e>75.0 && antr16e<100.0) || (antr16e>100.0 && antr16e<110.0 && antr16>90.0 && antr16<100.0))		// (s)raw8,ulaw,alaw
		{
			if(ant8<2.0)
			{
				if(ver8>1.0) ret=RAW8;
				else ret=SRAW8;
			}
			else
			{
				if(ver8<0.1) ret=RAW8;
				if(!ret && ver8<1.0) ret=ALAW;
				if(!ret && ver8>1.0 && ver8<2.0) ret=ULAW;
				if(!ret && ver8>3.0) ret=SRAW8;
			}
		}
		else
		{
			if(bins16[1]<5 && bins16[2]<5)			// (s)raw8 -> (s)raw16
			{
				if(ver16>5.0) ret=SRAW16E;
				else ret=RAW16E;			
			}
			else
			{
				if(bins16e[1]<5 && bins16e[2]<5)		// (s)raw8 -> (s)raw16 endian
				{
					if(ver16>5.0) ret=SRAW16;
					else ret=RAW16;		
				}
			}
			if(!ret)	// (s)raw16 (endian)
			{
				if(ant16>ant16e)	// no endian
				{
					if(ver16>5.0) ret=SRAW16;
					else ret=RAW16;
				}
				else				// endian
				{
					if(ver16e>5.0) ret=SRAW16E;
					else ret=RAW16E;
				}
			}
		}
		printf("%s <%s>\n",((ret) ? types[ret-1] : "       "),fn);
		printf("%d %d   %10.4lf %10.4lf %10.4lf %10.4lf   %4d %4d %4d %4d\n",minr8  ,manr8  ,ver8  ,100.0  ,ant8  ,100.0  ,bins8  [0],bins8  [1],bins8  [2],bins8  [3]);
		printf("%d %d   %10.4lf %10.4lf %10.4lf %10.4lf   %4d %4d %4d %4d\n",minr16 ,manr16 ,ver16 ,verr16 ,ant16 ,antr16 ,bins16 [0],bins16 [1],bins16 [2],bins16 [3]);
		printf("%d %d   %10.4lf %10.4lf %10.4lf %10.4lf   %4d %4d %4d %4d\n",minr16e,manr16e,ver16e,verr16e,ant16e,antr16e,bins16e[0],bins16e[1],bins16e[2],bins16e[3]);
	}
	else printf("ERROR : can't open file !\n");
}

void main(int argc,char *argv[])
{
	char fn[256];
	
	if(argc==2)
	{
		sprintf(fn,"%s.raw8",argv[1]);test(fn);
		sprintf(fn,"%s.sraw8",argv[1]);test(fn);
		sprintf(fn,"%s.raw16",argv[1]);test(fn);
		sprintf(fn,"%s.sraw16",argv[1]);test(fn);
		sprintf(fn,"%s.raw16e",argv[1]);test(fn);
		sprintf(fn,"%s.sraw16e",argv[1]);test(fn);
		sprintf(fn,"%s.ulaw",argv[1]);test(fn);
		sprintf(fn,"%s.alaw",argv[1]);test(fn);
	}
}