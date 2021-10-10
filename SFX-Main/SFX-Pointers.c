/****h*SoundFX/SFX-Pointers.c [3.71] *
*
*  NAME
*    SFX-Pointers.c
*  COPYRIGHT
*    $VER: SFX-Pointers.c 3.71 (13.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    additional Mousepointers for SoundFX - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Sep.1998
*  MODIFICATION HISTORY
*    13.Sep.1998	V 3.71	most recent version
*    11.Sep.1998	V 3.71	initial version
*  NOTES
*
*******
*/

#define SFX_Pointers_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

MPointer *ReadPointerImage(const STRPTR ptrname);
void	  FreePointerImage(const MPointer *mptr);

//-- defines ------------------------------------------------------------------

#define PREF_FORM		MakeID('F','O','R','M')
#define PREF_PREF		MakeID('P','R','E','F')
#define PREF_PNTR		MakeID('P','N','T','R')

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

MPointer *ReadPointerImage(const STRPTR ptrname)
{
	MPointer *mptr=NULL;
	FILE *ptrfile;
	struct PointerPrefs ptrprefs;
	ULONG subsize,id,planesize,wordsize;
	LONG formsize;
	BOOL run=TRUE;

//	MSG("ReadPointerImage beg");

	if(!(ptrfile=fopen(ptrname,"rb"))) { MSG("Can't open ptrfile");goto Error; }

	fread(&id,4,1,ptrfile);if(id!=PREF_FORM) { MSG("Ptrfile is not an iff file");goto Error; }
	fread(&formsize,4,1,ptrfile);
	fread(&id,4,1,ptrfile);if(id!=PREF_PREF) { MSG("Ptrfile is not an iff-pref file");goto Error; }
	while(run && (formsize>0)) {
		fread(&id,4,1,ptrfile);
		fread(&subsize,4,1,ptrfile);
		formsize-=8;
//		MSG4("id : %c%c%c%c",((char *)&id)[0],((char *)&id)[1],((char *)&id)[2],((char *)&id)[3]);
		switch(id) {
			case PREF_PNTR:
				fread(&ptrprefs,sizeof(struct PointerPrefs),1,ptrfile);
				if(ptrprefs.pp_Which==WBP_NORMAL) {
					if((mptr=AllocVec(sizeof(MPointer),MEMF_ANY|MEMF_CLEAR))) {
						register UBYTE i;

						InitBitMap(&(mptr->ptrbm),ptrprefs.pp_Depth,ptrprefs.pp_Width,ptrprefs.pp_Height);
						wordsize=WordSize(ptrprefs.pp_Width);
//						MSG1("wordsize  : %ld",wordsize);
						planesize=wordsize*ptrprefs.pp_Height;
//						MSG1("planesize : %ld (in words)",planesize);
						fseek(ptrfile,(3*((1<<ptrprefs.pp_Depth)-1)),SEEK_CUR);
						fread(mptr->ptrdata,(ptrprefs.pp_Depth*(planesize<<1)),1,ptrfile);
						for(i=0;i<ptrprefs.pp_Depth;i++) mptr->ptrbm.Planes[i]=(PLANEPTR)&(mptr->ptrdata[i*planesize]);
						if(!(mptr->Pointer=NewObject(NULL,"pointerclass",
							POINTERA_BitMap,		&(mptr->ptrbm),
							POINTERA_XOffset,		ptrprefs.pp_X,
							POINTERA_YOffset,		ptrprefs.pp_Y,
							POINTERA_WordWidth,		wordsize,
							POINTERA_XResolution,	POINTERXRESN_DEFAULT,
							POINTERA_YResolution,	POINTERYRESN_SCREENRESASPECT,
							TAG_DONE
						))) MSG("Can't create pointer object");
						run=FALSE;
					}
					else MSG("Can't allocate memory");
				}
				else MSG("need the normal ptr image");
				break;
			default:
				fseek(ptrfile,subsize,SEEK_CUR);
		}
		formsize-=subsize;
		if(subsize&1) { formsize--;fseek(ptrfile,1L,SEEK_CUR); }			/* odd-length chunk */
	}
Error:
	if(ptrfile)	fclose(ptrfile);

//	MSG("ReadPointerImage end");

	return(mptr);
}

void FreePointerImage(const MPointer *mptr)
{
	if(mptr)
	{
		if(mptr->Pointer) DisposeObject(mptr->Pointer);
		FreeVec((APTR)mptr);
	}
}

//-- eof ----------------------------------------------------------------------
