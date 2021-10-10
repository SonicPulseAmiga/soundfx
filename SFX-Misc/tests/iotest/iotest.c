//
// iotest.c
//
//

#include <exec/types.h>
#include <exec/exec.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <proto/dos.h>
#include <proto/exec.h>

time_t t1,t2;

UBYTE ansi_level2test(char *fn,void *mem,ULONG blksize,ULONG blkanz);
UBYTE ansi_level1test(char *fn,void *mem,ULONG blksize,ULONG blkanz);
UBYTE  dos_level1test(char *fn,void *mem,ULONG blksize,ULONG blkanz);

UBYTE ansi_level2test(char *fn,void *mem,ULONG blksize,ULONG blkanz)
{
	register ULONG i;
	FILE *fp;

	if((fp=fopen(fn,"wb")))
	{
		t1=time(NULL);
		for(i=0;i<blkanz;i++) fwrite(mem,blksize,1,fp);fflush(stdout);
		t2=time(NULL);
		printf("%4ld s ",(ULONG)difftime(t2,t1));
		fclose(fp);
	}
	else { printf("ERROR : fopen failed\n");return(1); }
	if((fp=fopen(fn,"rb")))
	{
		t1=time(NULL);
		for(i=0;i<blkanz;i++) fread(mem,blksize,1,fp);
		t2=time(NULL);
		printf("%4ld s ",(ULONG)difftime(t2,t1));fflush(stdout);
		fclose(fp);
		remove(fn);
	}
	else { printf("ERROR : fopen failed\n");return(1); }
	return(0);
}

UBYTE ansi_level1test(char *fn,void *mem,ULONG blksize,ULONG blkanz)
{
	register ULONG i;
	int fh;

	if((fh=open(fn,O_WRONLY|O_CREAT,0l))!=-1)
	{
		t1=time(NULL);
		for(i=0;i<blkanz;i++) write(fh,mem,blksize);
		t2=time(NULL);
		printf("%4ld s ",(ULONG)difftime(t2,t1));fflush(stdout);
		close(fh);
	}
	else { printf("ERROR : open failed\n");return(1); }
	if((fh=open(fn,O_RDONLY,0l))!=-1)
	{
		t1=time(NULL);
		for(i=0;i<blkanz;i++) read(fh,mem,blksize);
		t2=time(NULL);
		printf("%4ld s ",(ULONG)difftime(t2,t1));fflush(stdout);
		close(fh);
		remove(fn);
	}
	else { printf("ERROR : open failed\n");return(1); }
	return(0);
}

UBYTE dos_level1test(char *fn,void *mem,ULONG blksize,ULONG blkanz)
{
	register ULONG i;
	BPTR fh;

	if((fh=Open(fn,MODE_NEWFILE)))
	{
		t1=time(NULL);
		for(i=0;i<blkanz;i++) Write(fh,mem,blksize);
		t2=time(NULL);
		printf("%4ld s ",(ULONG)difftime(t2,t1));fflush(stdout);
		Close(fh);
	}
	else { printf("ERROR : open failed\n");return(1); }
	if((fh=Open(fn,MODE_OLDFILE)))
	{
		t1=time(NULL);
		for(i=0;i<blkanz;i++) Read(fh,mem,blksize);
		t2=time(NULL);
		printf("%4ld s ",(ULONG)difftime(t2,t1));fflush(stdout);
		Close(fh);
		remove(fn);
	}
	else { printf("ERROR : open failed\n");return(1); }
	return(0);
}

void main(int argc,char *argv[])
{
	ULONG minblksize,maxblksize,blksize,blkanz;
	void *mem;

	if(argc>3)
	{
		minblksize=atoi(argv[2]);			// should be dual power
		maxblksize=atoi(argv[3]);			// should be dual power
		if(minblksize>maxblksize)			// swap
		{
			blksize=minblksize;
			minblksize=maxblksize;
			maxblksize=blksize;
		}
		blksize=minblksize;
		if((mem=AllocMem(maxblksize,MEMF_ANY|MEMF_CLEAR)))
		{
			printf("blksize blkanz  | AnsiLevel2    AnsiLevel1    DosLevel1\n");
			printf("                | Write  Read   Write Read    Write Read\n");
			printf("----------------+-----------------------------------------\n");
			while(blksize<=maxblksize)
			{
				blkanz=16777216/blksize;			// 16 mb
				printf("%7ld %7ld |",blksize,blkanz);fflush(stdout);
				if(ansi_level2test(argv[1],mem,blksize,blkanz)) blksize=maxblksize;
				if(ansi_level1test(argv[1],mem,blksize,blkanz)) blksize=maxblksize;
				if(dos_level1test (argv[1],mem,blksize,blkanz)) blksize=maxblksize;
				blksize<<=1;
				printf("\n");
			}
			FreeMem(mem,maxblksize);
		}
		else printf("ERROR : malloc failed\n");
	}
	else
	{
		printf("Usage : iotest <testfile> <minblksize> <maxblksize>");
	}
}
