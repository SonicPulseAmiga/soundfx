/****h* SoundFX/SFX-CrashMark.c [4.2] *
*
*  NAME
*    SFX-CrashMark.c
*  COPYRIGHT
*    $VER: SFX-CrashMark.c 4.2 (14.07.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    crash recovery for sfx - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Jul.2003
*  MODIFICATION HISTORY
*    14.Jul.2003	V 4.2	most recent version
*    14.Jul.2003    V 4.2   initial version
*  NOTES
*
*******
*/

#define SFX_CRASHMARK_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void CheckCrashMark(void);
void DeleteCrashMark(void);

//-- helper

void BuildRecoveryList(struct List *fileList);
BOOL ReadSInfo(SInfo *si,STRPTR fn);
BOOL ShowRecoveryWin(struct List *fileList);
void RecoverFiles(struct List *fileList);

//-- defines ------------------------------------------------------------------

#define GADID_OKAY          0

#define GADID_FILELIST      (GADID_OKAY+1)
#define GADIX_FILELIST      0

#define LAST_GADIX          (GADIX_FILELIST+1)

//-- globals ------------------------------------------------------------------

struct FileNode {
	struct Node node;
	ULONG	slen;
	ULONG	srat;
	UBYTE	channels;
	char	*realName;
	char	*fileName;
};


//-- definitions --------------------------------------------------------------

void CheckCrashMark(void) {
	BPTR lock;

	INTRO;

	if(lock=Lock("progdir:data/sfx.runlock",ACCESS_READ)) {
		UnLock(lock);
		MSG("!!! prevous SFX has terminated unexpectedly");
		if(sfxprefs_vmem.enabled) {
			struct List fileList;
			struct FileNode *fileNode;

			BuildRecoveryList(&fileList);

			if(!IsListEmpty(&fileList)) {
				// show as multiselect multicolumn list with original name, length, srat, channels
				// ask which files to restore
				if(ShowRecoveryWin(&fileList)) {
					// restore, create SInfo for files and rename recovered files to new name
					RecoverFiles(&fileList);
				}
				while((fileNode=(struct FileNode *)RemHead(&fileList))) {
					if(fileNode->node.ln_Name) FreeVec(fileNode->node.ln_Name);
					if(fileNode->realName) FreeVec(fileNode->realName);
					if(fileNode->fileName) FreeVec(fileNode->fileName);
					FreeVec(fileNode);
				}
			}
			else MSG("no files to recover");
		}
	}
	else {
		int fh;
		if(fh=Open("progdir:data/sfx.runlock",MODE_NEWFILE)) Close(fh);
	}
	OUTRO;
}

void DeleteCrashMark(void) {
	INTRO;
	DeleteFile("progdir:data/sfx.runlock");
	OUTRO;
}

//-- helper

void BuildRecoveryList(struct List *fileList) {
	struct FileLock *dirlock;
	struct FileInfoBlock *InfoBlock;
	char drive[FILENAME_MAX],path[FILENAME_MAX],name[FILENAME_MAX],ext[FILENAME_MAX],fn[FILENAME_MAX];
	struct FileNode *fileNode;
	SInfo tsi;
	char infostr[255];
	char *channels[]={ "mono","stereo","","quadro" };

	//-- scan for *.cfg files from swap directory
	NewList(fileList);
	if(InfoBlock=(struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock),MEMF_ANY|MEMF_CLEAR)) {
		if(dirlock=(struct FileLock *)Lock(sfxprefs_vmem.swap_path,SHARED_LOCK)) {
			if(Examine((BPTR)dirlock,InfoBlock)) {
				if(InfoBlock->fib_DirEntryType>0 && InfoBlock->fib_DirEntryType!=3) {			/* it's a directory but no link */
					while(ExNext((BPTR)dirlock,InfoBlock)) {
						if(InfoBlock->fib_DirEntryType<0) {									/* it's a file */
							strsfn(InfoBlock->fib_FileName,drive,path,name,ext);
							if(ext[0] && !strnicmp(ext,"CFG",3)) {
								MSG1("  found file \"%s\"",name);
								if(ReadSInfo(&tsi,InfoBlock->fib_FileName)) {
									BOOL ok=TRUE;
									UBYTE l;
									BPTR lock;

									MSG4("  -> name=\"%s\" slen=%ld  srat=%ld chan=%ld",tsi.node.ln_Name,tsi.slen,tsi.srat,tsi.channels);
									// test if data files are there
									for(l=0;(ok && (l<(tsi.channels+1)));l++) {
										sprintf(path,"%s.%1d",name,l);
										strmfp(fn,sfxprefs_vmem.swap_path,path);
										MSG1("    \"%s\"",fn);
										if(lock=Lock(fn,ACCESS_READ)) UnLock(lock);
										else ok=FALSE;
									}
									if(ok) {
										MSG("  -> data files are available");
										if(fileNode=AllocVec(sizeof(struct FileNode),MEMF_ANY|MEMF_CLEAR)) {
											sprintf(infostr,"* \t03 %-8s \t14 %-20s \t37 %-8ld \t48 %-6ld \t57 %-6s",name,tsi.node.ln_Name,tsi.slen,tsi.srat,channels[tsi.channels]);
											fileNode->node.ln_Name=StringCopy(infostr);
											fileNode->node.ln_Type=TRUE;					// selected for recovery
											fileNode->slen=tsi.slen;
											fileNode->srat=tsi.srat;
											fileNode->channels=tsi.channels;
											fileNode->realName=tsi.node.ln_Name;tsi.node.ln_Name=NULL;
											fileNode->fileName=StringCopy(name);
											AddTail(fileList,fileNode);
										}
										else MSG("can't allocate memory for fileNode");
									}
									else {
										MSG("  -> data files are missing");
										// delete old cfg
										strmfp(fn,sfxprefs_vmem.swap_path,InfoBlock->fib_FileName);
										DeleteFile(fn);
									}
									if(tsi.node.ln_Name) FreeVec(tsi.node.ln_Name);
								}
							}
						}
					}
				}
			}
			UnLock((BPTR)dirlock);
		}
		FreeVec(InfoBlock);
	}
	else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
}

BOOL ReadSInfo(SInfo *si,STRPTR name) {
	APTR cfg;
	char fn[FILENAME_MAX];

	strmfp(fn,sfxprefs_vmem.swap_path,name);

	MSG1("  reading cfg \"%s\"",fn);

	if(cfg=CfgIO_NewConfig("SInfo"," %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s")) {
		CfgIO_ReadConfig(cfg,fn);

		si->node.ln_Name=StringCopy(CfgIO_ReadString(cfg,"SInfo","name","unnamed"));

		si->slen=		CfgIO_ReadUnsignedInteger(cfg,"SInfo","slen"    ,2);
		si->srat=		CfgIO_ReadUnsignedInteger(cfg,"SInfo","srat"    ,44100);
		si->channels=	CfgIO_ReadUnsignedInteger(cfg,"SInfo","channels",0);

		CfgIO_RemConfig(cfg);
		return(TRUE);
	}
	else ERR("Can't create SInfo config");
	return(FALSE);
}

BOOL ShowRecoveryWin(struct List *fileList) {
	struct NewGadget ng;
	struct Window *win;
	struct Gadget *g;
	struct Gadget *glist=NULL;
	struct Gadget *gadgets[LAST_GADIX];
	struct RastPort *bprp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	UBYTE ret=FALSE,runing=TRUE;
	UWORD ys1 =2;
	UWORD ys2 =ys1+1+ANYFRM_HEIGHT(1)+GetRunTime(scy[135]);
	UWORD winh=ys2+2+CMDFRM_HEIGHT;
	UWORD xs1 =2;
	UWORD winw=xs1+3+GetRunTime(scx[434]);
	UWORD xh;
	UBYTE scrfyoff=RunTime.scrfont->tf_YSize;
	ULONG css=0,csm=0,ccs=500,ccm=500;
	UBYTE akttest=FALSE,entry=0;
	struct Node *node;

	if(g=CreateContext(&glist)) {
		ng.ng_LeftEdge      =xs1+RunTime.scx[177];
		ng.ng_TopEdge       =ys2+RunTime.scy[2];
		ng.ng_Width         =RunTime.scx[80];
		ng.ng_Height        =RunTime.scy[11];
		ng.ng_GadgetText    ="_Okay";
		ng.ng_TextAttr      =RunTime.ScrFontB;
		ng.ng_GadgetID      =GADID_OKAY;
		ng.ng_Flags         =PLACETEXT_IN;
		ng.ng_VisualInfo    =RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge      =xs1+RunTime.scx[8];                            // FileList
		ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET+10];
		ng.ng_Width         =RunTime.scx[414];
		ng.ng_Height        =RunTime.scy[124];
		ng.ng_GadgetText    ="";
		ng.ng_TextAttr      =RunTime.ScrFont;
		ng.ng_GadgetID      =GADID_FILELIST;
		ng.ng_Flags         =PLACETEXT_LEFT;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,
			GTLV_Labels,fileList,
			GTLV_Selected,0,
			GTLV_Top,0,
			GTLV_ShowSelected,0,
			GTLV_CallBack,RunTime.tablelvhook,
			GTLV_MaxPen,7,
		TAG_DONE);
		gadgets[GADIX_FILELIST]=g;

		if(g) {
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)glist;
			Dialog_Wintags_Title		=(ULONG)"Recover Files";			/* @TODO: LOCALIZE */
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(win=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(win,NULL);
				bprp=win->RPort;SetFont(bprp,RunTime.scrfont);

				SetAPen(bprp,1);xh=RunTime.scx[6];
				Move(bprp,xs1+2+RunTime.scx[9]      ,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(bprp,"Sel"     ,3);
				Move(bprp,xs1+2+RunTime.scx[9]+ 3*xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(bprp,"Id"      ,2);
				Move(bprp,xs1+2+RunTime.scx[9]+14*xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(bprp,"Name"    ,4);
				Move(bprp,xs1+2+RunTime.scx[9]+37*xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(bprp,"SLen"    ,4);
				Move(bprp,xs1+2+RunTime.scx[9]+48*xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(bprp,"SRat"    ,4);
				Move(bprp,xs1+2+RunTime.scx[9]+57*xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(bprp,"Channels",8);

				xh=RunTime.scx[434];

				DrawTitle(bprp,"File List"          ,xs1,ys1,xh);

				DrawGadWB(bprp,xs1,ys1,xh,ANYFRM_HEIGHT(1)+RunTime.scy[135]);   /* Liste */
				DrawGadWB(bprp,xs1,ys2,xh,CMDFRM_HEIGHT);                       /* Okay */

				while(runing) {
					WaitPort(win->UserPort);
					while(imsg=GT_GetIMsg(win->UserPort)) {
						iclass  =imsg->Class;
						iadr    =imsg->IAddress;
						icode   =imsg->Code;
						GT_ReplyIMsg(imsg);
						if(iclass==IDCMP_GADGETUP) {
							switch(((struct Gadget *)iadr)->GadgetID) {
								case GADID_OKAY:
									ret=TRUE;runing=FALSE;
									break;
								case GADID_FILELIST:
									if(!akttest) { CurrentTime(&css,&csm);akttest=TRUE; }
									else {
										CurrentTime(&ccs,&ccm);
										if(DoubleClick(css,csm,ccs,ccm) && entry==icode) {
											node=GetNodeByIndex(fileList,entry);
											if(node->ln_Type) {
												node->ln_Type=FALSE;
												node->ln_Name[0]=' ';
											}
											else {
												node->ln_Type=TRUE;
												node->ln_Name[0]='*';
											}
											GT_SetGadgetAttrs(gadgets[GADIX_FILELIST],win,NULL,GTLV_Labels,fileList,TAG_DONE);
											css=0;csm=0;ccs=500;ccm=500;akttest=FALSE;
										}
										else { css=ccs;csm=ccm;ccs=500;ccm=500; }
									}
									entry=icode;
									break;
							}
						}
						if(iclass==IDCMP_VANILLAKEY) {
							switch(icode) {
								case KEY_ESC:               /* Cancel */
									ret=FALSE;runing=FALSE;break;
								case KEY_ENTER:             /* Okay */
								case KEY_RETURN:
								case 'O':
								case 'o':
									ret=TRUE;runing=FALSE;break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) { ret=FALSE;runing=FALSE; }
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP) ShowHelp("html/nodes/node01.05.12.html");
					}
				}
				CloseWindow(win);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
			FreeGadgets(glist);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	return(ret);
}

void RecoverFiles(struct List *fileList) {
	struct Node *node;
	struct FileNode *fileNode;
	SInfo *si;
	UBYTE l;
	char fn1[FILENAME_MAX],fn2[FILENAME_MAX],name[FILENAME_MAX];

	foreach(node,fileList) {
		if(node->ln_Type) {
			fileNode=(struct FileNode *)node;
			MSG1("  recover file \"%s\"",fileNode->realName);
			if(si=NewSample()) {
				si->zoomxl=si->slen=fileNode->slen;
				si->srat=fileNode->srat;
				si->channels=fileNode->channels;
				si->channelmask=CH_MASK(si->channels);
				si->storage=VM_DRIVE;
				// rename vmem-files to new address-name
				for(l=0;l<si->channels+1;l++) {
					sprintf(name,"%s.%1d",fileNode->fileName,l);
					strmfp(fn1,sfxprefs_vmem.swap_path,name);
					sprintf(name,"%08lX.%1d",si,l);
					strmfp(fn2,sfxprefs_vmem.swap_path,name);
					Rename(fn1,fn2);
				}
				AddSample(si,fileNode->realName);
				// delete old cfg
				sprintf(name,"%s.cfg",fileNode->fileName);
				strmfp(fn1,sfxprefs_vmem.swap_path,name);
				DeleteFile(fn1);
			}
			else MSG("can't creat SInfo");
		}
	}
}

//-- eof ----------------------------------------------------------------------
