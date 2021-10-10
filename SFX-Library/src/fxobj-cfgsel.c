/****h* sfxsupport.library/fxobj-cfgsel.c [4.1] *
*
*  NAME
*    fxobj-cfgsel.c
*  COPYRIGHT
*    $VER: fxobj-cfgsel.c 4.1 (29.10.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    configuration select object - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    27.Apr.1999
*  MODIFICATION HISTORY
*    29.Oct.2000	V 4.1	most recent version
*    16.Jul.2000	V 1.30	most recent version
*    03.May.1999	V 1.20	most recent version
*    27.Apr.1999	V 1.20	initial version
*  NOTES
*
*******
*/

#define FXOBJ_CFGSEL_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

void			ASM LIB_EXPORT(InitCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,STRPTR path)));
void			ASM LIB_EXPORT(DoneCfgSel(REG(a0,CfgSel *cfgsel)));

struct Gadget * ASM LIB_EXPORT(AddCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y),REG(d4,UWORD h)));
void 			ASM LIB_EXPORT(SetCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
UBYTE			ASM LIB_EXPORT(HandleCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode),REG(d4,ULONG sec),REG(d5,ULONG mic)));

struct Node *	ASM LIB_EXPORT(AddCfgSelConfig(REG(a0,CfgSel *cfgsel),REG(a1,STRPTR name)));

//-- defines ------------------------------------------------------------------

#define GADID_CFGLIST	 0
#define GADIX_CFGLIST	 0
#define GADID_CFGNAME	 (GADID_CFGLIST+1)
#define GADIX_CFGNAME	 (GADIX_CFGLIST+1)
#define GADID_CFGADD	 (GADID_CFGNAME+1)
#define GADIX_CFGADD	 (GADIX_CFGNAME+1)
#define GADID_CFGDEL	 (GADID_CFGADD+1)
#define GADIX_CFGDEL	 (GADIX_CFGADD+1)

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

void SAVEDS ASM LIB_EXPORT(InitCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,STRPTR path))) {
	struct Node *node;
	struct FileInfoBlock *InfoBlock;
	struct FileLock *dirlock;
	char name[FILENAME_MAX];
	struct List *cfgs=&(cfgsel->cfgs);

	NewList(cfgs);
	strcpy(cfgsel->path,path);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(cfgsel,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(path,__FUNC__,"2.par",4));

	if(InfoBlock=(struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock),MEMF_ANY|MEMF_CLEAR)) {
		if(dirlock=(struct FileLock *)Lock(path,SHARED_LOCK)) {
			if(Examine((BPTR)dirlock,InfoBlock)) {
				if(InfoBlock->fib_DirEntryType>0 && InfoBlock->fib_DirEntryType!=3) {		/* it's a directory but no link */
					while(ExNext((BPTR)dirlock,InfoBlock)) {
						if(InfoBlock->fib_DirEntryType<0) {									/* it's a file */
							strsfn(InfoBlock->fib_FileName,NULL,NULL,name,NULL);
							if(strncmp("default",name,7)) {									/* we add 'default' already below */
								if(node=(struct Node *)AllocVec(sizeof(struct Node),MEMF_ANY|MEMF_CLEAR)) {
									node->ln_Name=LIB_INTCALL(StringCopy(name));
									InsertByNameAsc(cfgs,node);
								}
								else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));
							}
						}
					}
				}
			}
			UnLock((BPTR)dirlock);
		}
		FreeVec(InfoBlock);
	}
	else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));
	if(node=(struct Node *)AllocVec(sizeof(struct Node),MEMF_ANY|MEMF_CLEAR)) {
		node->ln_Name=LIB_INTCALL(StringCopy(" default"));
		AddHead(cfgs,node);
	}
	else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));
	if(node=(struct Node *)AllocVec(sizeof(struct Node),MEMF_ANY|MEMF_CLEAR)) {
		node->ln_Name=LIB_INTCALL(StringCopy(" current"));
		AddHead(cfgs,node);
		cfgsel->aktcfg=node;
	}
	else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));
	cfgsel->action=0;
	//-- for GUI interaction
	cfgsel->ss=1000;
	cfgsel->sm=1000;
	cfgsel->akttest=0;
	cfgsel->aktcfgnr=0xFFFFFFFF;
}

void SAVEDS ASM LIB_EXPORT(DoneCfgSel(REG(a0,CfgSel *cfgsel))) {
	struct List *cfgs=&(cfgsel->cfgs);
	struct Node *node;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(cfgsel,__FUNC__,"1.par",4));

	while(node=RemTail(cfgs)) {
		FreeVec(node->ln_Name);
		FreeVec(node);
	}
	NewList(cfgs);
	cfgsel->aktcfg=NULL;
	cfgsel->action=0;
}

struct Gadget * SAVEDS ASM LIB_EXPORT(AddCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y),REG(d4,UWORD h))) {
	struct NewGadget ng;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(cfgsel,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(g,__FUNC__,"3.par",4));

	ng.ng_LeftEdge		=x+GetRunTime(scx[2]);
	ng.ng_TopEdge		=y+GetRunTime(scy[10]);
	ng.ng_Width			=GetRunTime(scx[126]);
	ng.ng_Height		=h-GetRunTime(scy[27+8]);
	ng.ng_TextAttr		=GetRunTime(ScrFont);
	ng.ng_GadgetText	="";
	ng.ng_GadgetID		=firstid+GADID_CFGLIST;
	ng.ng_Flags			=PLACETEXT_IN;
	ng.ng_VisualInfo	=GetRunTime(VisualInfo);
	g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,&(cfgsel->cfgs),GTLV_Selected,1,GTLV_Top,0,GTLV_ShowSelected,0l,TAG_DONE);
	gad[firstix+GADIX_CFGLIST]=g;

	ng.ng_TopEdge		=y+(h-GetRunTime(scy[25]));
	ng.ng_Height		=GetRunTime(scy[11]);
	ng.ng_GadgetID		=firstid+GADID_CFGNAME;
	g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,FILENAME_MAX,GTST_String,NULL,TAG_DONE);
	gad[firstix+GADIX_CFGNAME]=g;

	ng.ng_TopEdge		=y+(h-GetRunTime(scy[13]));
	ng.ng_Width			=GetRunTime(scx[63]);
	ng.ng_GadgetText	="Add";
	ng.ng_GadgetID		=firstid+GADID_CFGADD;
	g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
	gad[firstix+GADIX_CFGADD]=g;

	ng.ng_LeftEdge		=x+GetRunTime(scx[2+63]);
	ng.ng_GadgetText	="Del";
	ng.ng_GadgetID		=firstid+GADID_CFGDEL;
	g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
	gad[firstix+GADIX_CFGDEL]=g;

	return(g);
}

void SAVEDS ASM LIB_EXPORT(SetCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix))) {
	register ULONG i;
	ULONG aktcfg=0;
	struct List *cfgs=&(cfgsel->cfgs);
	struct Node *node=(struct Node *)(cfgs->lh_Head);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(cfgsel,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"3.par",4));

	if(!cfgsel->aktcfg) {
		GT_GetGadgetAttrs(gad[firstix+GADIX_CFGLIST],win,0l,GTLV_Selected,&aktcfg,TAG_DONE);
		for(i=0;(i<aktcfg && !EndOfList(cfgs,node));i++) node=node->ln_Succ;
		cfgsel->aktcfg=node;
		GT_SetGadgetAttrs(gad[firstix+GADIX_CFGLIST],win,0l,GTLV_Labels,cfgs,TAG_DONE);
	}
	else {
		for(aktcfg=0;node!=cfgsel->aktcfg;aktcfg++) node=node->ln_Succ;
		GT_SetGadgetAttrs(gad[firstix+GADIX_CFGLIST],win,0l,GTLV_Labels,cfgs,GTLV_Selected,aktcfg,GTLV_Top,aktcfg,TAG_DONE);
	}
	if(cfgsel->aktcfg->ln_Name[0]!=' ')	{
		GT_SetGadgetAttrs(gad[firstix+GADIX_CFGNAME],win,0l,GA_Disabled,FALSE,GTST_String,cfgsel->aktcfg->ln_Name,TAG_DONE);
		GT_SetGadgetAttrs(gad[firstix+GADIX_CFGDEL ],win,0l,GA_Disabled,FALSE,TAG_DONE);
	}
	else {
		GT_SetGadgetAttrs(gad[firstix+GADIX_CFGNAME],win,0l,GA_Disabled,TRUE,GTST_String,cfgsel->aktcfg->ln_Name,TAG_DONE);
		GT_SetGadgetAttrs(gad[firstix+GADIX_CFGDEL ],win,0l,GA_Disabled,TRUE,TAG_DONE);
	}
}

UBYTE SAVEDS ASM LIB_EXPORT(HandleCfgSel(REG(a0,CfgSel *cfgsel),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode),REG(d4,ULONG sec),REG(d5,ULONG mic))) {
	UBYTE ret=0;
	ULONG cs,cm;
	char fn1[FILENAME_MAX],fn2[FILENAME_MAX];
	struct List *cfgs=&(cfgsel->cfgs);
	struct Node *node;
	BPTR file;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(cfgsel,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"3.par",4));

	cfgsel->action=CFGSEL_ACTION_NONE;
	switch(id-firstid) {
		case GADID_CFGLIST:
			//GT_GetGadgetAttrs(gad[firstix+GADIX_CFGLIST],win,0l,GTLV_Selected,&aktcfg,TAG_DONE);
			if(!cfgsel->akttest) { /*CurrentTime(&ss,&sm);*/cfgsel->ss=sec;cfgsel->sm=mic;cfgsel->akttest=1; }
			else {
				/*CurrentTime(&cs,&cm);*/cs=sec;cm=mic;
				if(DoubleClick(cfgsel->ss,cfgsel->sm,cs,cm) && (cfgsel->aktcfgnr==icode)) {
					cfgsel->action=CFGSEL_ACTION_GO;
					cfgsel->ss=0;cfgsel->sm=0;cfgsel->akttest=0;
				}
				else { cfgsel->ss=cs;cfgsel->sm=cm; }
			}
			if(cfgsel->aktcfgnr!=icode) {
				cfgsel->aktcfgnr=icode;cfgsel->aktcfg=NULL;
				LIB_INTCALL(SetCfgSel(cfgsel,win,gad,firstix));
				if(!cfgsel->action) cfgsel->action=CFGSEL_ACTION_LOAD;
			}
			break;
		case GADID_CFGNAME:
			sprintf(fn1,"%s/%s.cfg",cfgsel->path,cfgsel->aktcfg->ln_Name);
			sprintf(fn2,"%s/%s.cfg",cfgsel->path,(((struct StringInfo *)(gad[firstix+GADIX_CFGNAME]->SpecialInfo))->Buffer));
			if(strcmp(fn1,fn2)) {			/* if name has changed */
				FreeVec(cfgsel->aktcfg->ln_Name);			/* free old name and set new name */
				cfgsel->aktcfg->ln_Name=LIB_INTCALL(StringCopy(((struct StringInfo *)(gad[firstix+GADIX_CFGNAME]->SpecialInfo))->Buffer));
				Remove(cfgsel->aktcfg);					/* unlink node */
				if(file=Open(fn2,MODE_OLDFILE)) {		/* check if destination file exists */
					Close(file);
					DeleteFile(fn2);						/* yes, delete file and get (already existing) node */
					cfgsel->aktcfg=GetNodeByName(&(cfgsel->cfgs),(((struct StringInfo *)(gad[firstix+GADIX_CFGNAME]->SpecialInfo))->Buffer));
				}
				else InsertByNameAsc(&(cfgsel->cfgs),cfgsel->aktcfg);	/* neu einfügen */
				Rename(fn1,fn2);								/* und datei umbenennen */
				LIB_INTCALL(SetCfgSel(cfgsel,win,gad,firstix));
			}
			ret=1;
			break;
		case GADID_CFGADD:
			break;
		case GADID_CFGDEL:
			node=cfgsel->aktcfg;
			if(!EndOfList(cfgs,node->ln_Succ)) cfgsel->aktcfg=cfgsel->aktcfg->ln_Succ;
			else {
				cfgsel->aktcfg=cfgsel->aktcfg->ln_Pred;
				if(cfgsel->aktcfgnr) cfgsel->aktcfgnr--;
			}
			Remove(node);
			sprintf(fn1,"%s/%s.cfg",cfgsel->path,node->ln_Name);
			DeleteFile(fn1);
			FreeVec(node->ln_Name);
			FreeVec(node);
			LIB_INTCALL(SetCfgSel(cfgsel,win,gad,firstix));
			cfgsel->action=CFGSEL_ACTION_LOAD;
			break;
	}
	return(ret);
}

struct Node * SAVEDS ASM LIB_EXPORT(AddCfgSelConfig(REG(a0,CfgSel *cfgsel),REG(a1,STRPTR name))) {
	struct Node *node;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(cfgsel,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(name,__FUNC__,"2.par",4));

	if(node=(struct Node *)AllocVec(sizeof(struct Node),MEMF_ANY|MEMF_CLEAR)) {
		node->ln_Name=LIB_INTCALL(StringCopy(name));
		InsertByNameAsc(&(cfgsel->cfgs),node);
	}
	return(node);
}

//-- eof ----------------------------------------------------------------------
