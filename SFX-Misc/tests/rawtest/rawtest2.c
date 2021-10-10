// rawtest2.c

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
	WORD ret=0;
	ULONG r8,sr8,r16,sr16,er16,esr16,ul,al;
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
			for(i=0;


			FreeMem(buf,len);
		}
		else printf("ERROR : can't alloc mem !\n");
		fclose(infile);

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