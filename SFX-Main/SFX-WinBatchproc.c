/****h* SoundFX/SFX-WinBatchProc.c [4.3] *
*
*  NAME
*    SFX-WinBatchproc.c
*  COPYRIGHT
*    $VER: SFX-WinBatchproc.c 4.3 (28.01.04) © by Stefan Kost 2000-2004
*  FUNCTION
*    Batch Processor - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    18.Jun.2000
*  MODIFICATION HISTORY
*    28.Jan.2004	V 4.3	most recent version
*    07.Jul.2003    V 4.2   most recent version
*    07.Mar.2001    V 4.1   most recent version
*                           A : batch status
*                           C : does not replace files
*    22.Jul.2000    V 4.0   most recent version
*    18.Jun.2000    V 4.0   initial version
*  NOTES
*
*  BUGS
*
*  IDEAS
*    * list of file extensions to omit/copy (*.info,*.txt,...)
*******
*/

#define SFX_WinBatchproc_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void BatchProcWin(void);
void BatchProc(void);

//-- helper

void Refr_BatchProcWin(struct Gadget *gadgets[],struct Window *win);
void BuildBatchProcNode(struct Node *batchNode,STRPTR mod,STRPTR cfg,ULONG typ);
void FreeBatchProcParams(void);

void LoadBatchProcParams        (STRPTR fname);
void SaveBatchProcParams        (STRPTR fname);
void DefaultBatchProcParams     (void);
void RecallLastBatchProcParams  (void);
void RecallDefBatchProcParams   (void);

void AddToFileList(struct List *list,STRPTR name,UBYTE type);
void BatchLog(STRPTR filename,STRPTR fmt,...);

//-- defines ------------------------------------------------------------------

#define GADID_OKAY          0

#define GADID_CFGSEL        (GADID_OKAY+1)
#define GADIX_CFGSEL        0
#define GADID_CFGLIST       (GADID_CFGSEL)
#define GADIX_CFGLIST       (GADIX_CFGSEL)
#define GADID_CFGNAME       (GADID_CFGLIST+1)
#define GADIX_CFGNAME       (GADIX_CFGLIST+1)
#define GADID_CFGADD        (GADID_CFGNAME+1)
#define GADIX_CFGADD        (GADIX_CFGNAME+1)
#define GADID_CFGDEL        (GADID_CFGADD+1)
#define GADIX_CFGDEL        (GADIX_CFGADD+1)

#define GADID_BATCHLIST     (GADID_CFGDEL+1)
#define GADIX_BATCHLIST     (GADIX_CFGDEL+1)
#define GADID_BATCHADD      (GADID_BATCHLIST+1)
#define GADIX_BATCHADD      (GADIX_BATCHLIST+1)
#define GADID_BATCHDEL      (GADID_BATCHADD+1)
#define GADIX_BATCHDEL      (GADIX_BATCHADD+1)
#define GADID_BATCHMODPOPUP (GADID_BATCHDEL+1)
#define GADIX_BATCHMODPOPUP (GADIX_BATCHDEL+1)
#define GADID_BATCHMOD      (GADID_BATCHMODPOPUP+1)
#define GADIX_BATCHMOD      (GADIX_BATCHMODPOPUP+1)
#define GADID_BATCHCFGPOPUP (GADID_BATCHMOD+1)
#define GADIX_BATCHCFGPOPUP (GADIX_BATCHMOD+1)
#define GADID_BATCHCFG      (GADID_BATCHCFGPOPUP+1)
#define GADIX_BATCHCFG      (GADIX_BATCHCFGPOPUP+1)

#define GADID_OPTLOG        (GADID_BATCHCFG+1)
#define GADIX_OPTLOG        (GADIX_BATCHCFG+1)
#define GADID_OPTLOGPOPUP   (GADID_OPTLOG+1)
#define GADIX_OPTLOGPOPUP   (GADIX_OPTLOG+1)
#define GADID_OPTLOGFN      (GADID_OPTLOGPOPUP+1)
#define GADIX_OPTLOGFN      (GADIX_OPTLOGPOPUP+1)
#define GADID_OPTREC        (GADID_OPTLOGFN+1)
#define GADIX_OPTREC        (GADIX_OPTLOGFN+1)
#define GADID_OPTRECDEPTH   (GADID_OPTREC+1)
#define GADIX_OPTRECDEPTH   (GADIX_OPTREC+1)
#define GADID_OPTDELETE		(GADID_OPTRECDEPTH+1)
#define GADIX_OPTDELETE		(GADIX_OPTRECDEPTH+1)

#define LAST_GADIX          (GADIX_OPTDELETE+1)

#define BATCH_NODETYP_LO    0
#define BATCH_NODETYP_SA    1
#define BATCH_NODETYP_FX    2

//-- global -------------------------------------------------------------------

struct batchparam {
	struct List batchList;
	UBYTE       log;
	UBYTE       rec;
	char        logfn[FILENAME_MAX];
	LONG        recdepth;
	UBYTE		delfiles;
} batchParam={0};

struct batchnode {
	struct Node node;
	ULONG       typ;
	char        mod[FILENAME_MAX];
	char        cfg[FILENAME_MAX];
};

UBYTE aktnodenr,aktnodect;
struct batchnode *aktBatchNode;
APTR batchcfg=NULL;

//-- definitions --------------------------------------------------------------

void BatchProcWin(void)
{
	struct NewGadget ng;
	struct Window *win;
	struct Gadget *g;
	struct Gadget *glist=NULL;
	struct Gadget *gadgets[LAST_GADIX];
	struct RastPort *bprp;
	struct IntuiMessage *imsg;
	ULONG iclass,isec,imic;
	APTR iadr;
	UWORD icode;
	ULONG gnr;
	UBYTE ret=FALSE,runing=TRUE,gh,nr;
	UWORD ys1 =2;
	UWORD ys2 =ys1+1+ANYFRM_HEIGHT(1)+GetRunTime(scy[135]);
	UWORD ys3 =ys2+1+ANYFRM_HEIGHT(3);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1 =2;
	UWORD xs2 =xs1+1+GetRunTime(scx[354]);
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;
	UBYTE scrfyoff=RunTime.scrfont->tf_YSize;
	struct Node *node;
	struct batchnode *batchNode;
	ModNode *modul;
	CfgSel cfgsel,modcfgsel;
	char modcfgdir[FILENAME_MAX],fn[FILENAME_MAX],cn[FNSIZE];

	if(batchcfg=CfgIO_NewConfig("SoundFXBatchProcessor"," %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s")) {
		UnLock(CreateDir("progdir:data/batch"));
		RecallLastBatchProcParams();            // Default oder LastUsed values setzen
		aktnodenr=0;aktnodect=GetNumberOfNodes(&batchParam.batchList);
		aktBatchNode=(struct batchnode *)GetFirstNode(&batchParam.batchList);
		if(g=CreateContext(&glist)) {
			ng.ng_LeftEdge      =xs1+RunTime.scx[137];
			ng.ng_TopEdge       =ys3+RunTime.scy[2];
			ng.ng_Width         =RunTime.scx[80];
			ng.ng_Height        =RunTime.scy[11];
			ng.ng_GadgetText    ="_Okay";
			ng.ng_TextAttr      =RunTime.ScrFontB;
			ng.ng_GadgetID      =GADID_OKAY;
			ng.ng_Flags         =PLACETEXT_IN;
			ng.ng_VisualInfo    =RunTime.VisualInfo;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

			InitCfgSel(&cfgsel,"progdir:data/batch");
			g=AddCfgSel(&cfgsel,gadgets,g,GADID_CFGSEL,GADIX_CFGSEL,xs2,ys1,winh-2);

			ng.ng_LeftEdge      =xs1+RunTime.scx[8];                            // BatchList
			ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET+10];
			ng.ng_Width         =RunTime.scx[334];
			ng.ng_Height        =RunTime.scy[124];
			ng.ng_GadgetText    ="";
			ng.ng_TextAttr      =RunTime.ScrFont;
			ng.ng_GadgetID      =GADID_BATCHLIST;
			ng.ng_Flags         =PLACETEXT_LEFT;
			g=CreateGadget(LISTVIEW_KIND,g,&ng,
				GTLV_Labels,&batchParam.batchList,
				GTLV_Selected,aktnodenr,
				GTLV_Top,0,
				GTLV_ShowSelected,0,
				GTLV_CallBack,RunTime.tablelvhook,
				GTLV_MaxPen,7,
			TAG_DONE);
			gadgets[GADIX_BATCHLIST]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[8];
			ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET+134];
			ng.ng_Width         =RunTime.scx[20];
			ng.ng_Height        =RunTime.scy[11];
			ng.ng_GadgetText    ="_+";
			ng.ng_GadgetID      =GADID_BATCHADD;
			ng.ng_Flags         =PLACETEXT_IN;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);
			gadgets[GADIX_BATCHADD]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[28];
			ng.ng_GadgetText    ="_-";
			ng.ng_GadgetID      =GADID_BATCHDEL;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);
			gadgets[GADIX_BATCHDEL]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[70];               // Batch Module
			ng.ng_Width         =RunTime.scx[15];
			ng.ng_TextAttr      =RunTime.SigFont;
			ng.ng_GadgetText    =IMAGE_PopUp;
			ng.ng_GadgetID      =GADID_BATCHMODPOPUP;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
			gadgets[GADIX_BATCHMODPOPUP]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[86];
			ng.ng_Width         =RunTime.scx[120];
			ng.ng_TextAttr      =RunTime.ScrFont;
			ng.ng_GadgetText    ="";
			ng.ng_GadgetID      =GADID_BATCHMOD;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,NULL,GTTX_CopyText,FALSE,TAG_DONE);
			gadgets[GADIX_BATCHMOD]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[210];              // Batch Module Config
			ng.ng_Width         =RunTime.scx[15];
			ng.ng_TextAttr      =RunTime.SigFont;
			ng.ng_GadgetText    =IMAGE_PopUp;
			ng.ng_GadgetID      =GADID_BATCHCFGPOPUP;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
			gadgets[GADIX_BATCHCFGPOPUP]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[226];
			ng.ng_Width         =RunTime.scx[100];
			ng.ng_TextAttr      =RunTime.ScrFont;
			ng.ng_GadgetText    ="";
			ng.ng_GadgetID      =GADID_BATCHCFG;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,NULL,GTTX_CopyText,FALSE,TAG_DONE);
			gadgets[GADIX_BATCHCFG]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[70];                   // Option : Logging ?
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET];
			ng.ng_Width         =RunTime.scx[26];
			ng.ng_GadgetText    ="Log";
			ng.ng_GadgetID      =GADID_OPTLOG;
			ng.ng_Flags         =PLACETEXT_LEFT;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,batchParam.log,GTCB_Scaled,TRUE,TAG_DONE);
			gadgets[GADIX_OPTLOG]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[150];                  // Logpath
			ng.ng_Width         =RunTime.scx[15];
			ng.ng_TextAttr      =RunTime.SigFont;
			ng.ng_GadgetText    =IMAGE_DirReq;
			ng.ng_GadgetID      =GADID_OPTLOGPOPUP;
			ng.ng_Flags         =PLACETEXT_IN;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
			gadgets[GADIX_OPTLOGPOPUP]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[166];
			ng.ng_Width         =RunTime.scx[180];
			ng.ng_TextAttr      =RunTime.ScrFont;
			ng.ng_GadgetText    ="";
			ng.ng_GadgetID      =GADID_OPTLOGFN;
			ng.ng_Flags         =PLACETEXT_LEFT;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,100,GTST_String,batchParam.logfn,TAG_DONE);
			gadgets[GADIX_OPTLOGFN]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[70];           // Option : Recursiv ?
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET+12];
			ng.ng_Width         =RunTime.scx[26];
			ng.ng_GadgetText    ="Recursive";
			ng.ng_GadgetID      =GADID_OPTREC;
			ng.ng_Flags         =PLACETEXT_LEFT;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,batchParam.rec,GTCB_Scaled,TRUE,TAG_DONE);
			gadgets[GADIX_OPTREC]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[150];          // Logdepth
			ng.ng_Width         =RunTime.scx[100];
			ng.ng_GadgetText    ="Depth";
			ng.ng_GadgetID      =GADID_OPTRECDEPTH;
			g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,10,GTIN_Number,batchParam.recdepth,TAG_DONE);
			gadgets[GADIX_OPTRECDEPTH]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[150];           // Option : Delete Source Files ?
			ng.ng_TopEdge       =ys2+RunTime.scy[FRMCONTENT_YOFFSET+24];
			ng.ng_Width         =RunTime.scx[26];
			ng.ng_GadgetText    ="Delete Source Files";
			ng.ng_GadgetID      =GADID_OPTDELETE;
			ng.ng_Flags         =PLACETEXT_LEFT;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,batchParam.delfiles,GTCB_Scaled,TRUE,TAG_DONE);
			gadgets[GADIX_OPTDELETE]=g;

			if(g) {
				MemUsage(sfxtitle);
				Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
				Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
				Dialog_Wintags_InnerWidth	=winw;
				Dialog_Wintags_InnerHeight	=winh;
				Dialog_Wintags_IDCMP		=BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
				Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
				Dialog_Wintags_Gadgets		=(ULONG)glist;
				Dialog_Wintags_Title		=(ULONG)GetString(menPrjBatchProc);
				Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
				if(win=OpenWindowTagList(NULL,Dialog_Wintags)) {
					GT_RefreshWindow(win,NULL);
					bprp=win->RPort;SetFont(bprp,RunTime.scrfont);

					SetAPen(bprp,1);xh=RunTime.scx[6];
					Move(bprp,xs1+2+RunTime.scx[9]      ,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(bprp,"Type"  ,4);
					Move(bprp,xs1+2+RunTime.scx[9]+10*xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(bprp,"Module",6);
					Move(bprp,xs1+2+RunTime.scx[9]+33*xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(bprp,"Preset",6);

					//MSG3("list-labels : y=%ld+%ld+%ld",ys1,scrfyoff,RunTime.scy[FRMCONTENT_YOFFSET]);

					RText(bprp,"Logfile",xs1+RunTime.scx[142],ys2+RunTime.scy[FRMTEXT_YPOS(1)]);

					xh=RunTime.scx[354];

					DrawTitle(bprp,"Batch List"         ,xs1,ys1,xh);
					DrawTitle(bprp,"Options"            ,xs1,ys2,xh);
					DrawTitle(bprp,"Presets"            ,xs2,ys1,CFGSEL_WIDTH);

					DrawGadWB(bprp,xs1,ys1,xh,ANYFRM_HEIGHT(1)+RunTime.scy[135]);   /* Liste */
					DrawGadWB(bprp,xs1,ys2,xh,ANYFRM_HEIGHT(3));                    /* Options */
					DrawGadWB(bprp,xs1,ys3,xh,CMDFRM_HEIGHT);                       /* Okay */
					DrawGadWB(bprp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);             /* CfgSel */

					Refr_BatchProcWin(gadgets,win);

					while(runing) {
						WaitPort(win->UserPort);
						while(imsg=GT_GetIMsg(win->UserPort)) {
							iclass  =imsg->Class;
							iadr    =imsg->IAddress;
							icode   =imsg->Code;
							isec    =imsg->Seconds;
							imic    =imsg->Micros;
							GT_ReplyIMsg(imsg);
							if(iclass==IDCMP_GADGETUP) {
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								gh=HandleCfgSel   (&cfgsel      ,win,gadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic);
								if(!gh) {
									switch(gnr) {
										case GADID_OKAY:
											SaveBatchProcParams("env:sfx/batch.cfg");           // store current settings
											ret=TRUE;runing=FALSE;
											break;
										case GADID_CFGLIST:
										case GADID_CFGNAME:
										case GADID_CFGDEL:
											switch(cfgsel.action) {
												case CFGSEL_ACTION_LOAD:
													MSG1("akt config : %s",cfgsel.aktcfg->ln_Name);
													FreeBatchProcParams();              		// free old settings
													if(!strnicmp(cfgsel.aktcfg->ln_Name," default",8)) RecallDefBatchProcParams();
													else {
														if(!strnicmp(cfgsel.aktcfg->ln_Name," current",8)) RecallLastBatchProcParams();
														else { sprintf(fn,"%s/%s.cfg",cfgsel.path,cfgsel.aktcfg->ln_Name);LoadBatchProcParams(fn); }
													}
													aktnodenr=0;aktnodect=GetNumberOfNodes(&batchParam.batchList);
													Refr_BatchProcWin(gadgets,win);
													break;
												case CFGSEL_ACTION_GO:
													SaveBatchProcParams("env:sfx/batch.cfg");   // store current settings
													ret=TRUE;runing=FALSE;
													break;
											}
											break;
										case GADID_CFGADD:
											nr=0;sprintf(cn,"New%02d.cfg",nr);
											while((FindName(&(cfgsel.cfgs),cn)) && (nr<100)) { nr++;sprintf(cn,"New%02d.cfg",nr); }
											if(nr<100) {
												strmfp(fn,cfgsel.path,cn);SaveBatchProcParams(fn);
												strsfn(fn,NULL,NULL,cn,NULL);
												if(node=(struct Node *)AllocVec(sizeof(struct Node),MEMF_ANY|MEMF_CLEAR)) {
													node->ln_Name=StringCopy(cn);InsertByNameAsc(&(cfgsel.cfgs),node);
													cfgsel.aktcfg=node;SetCfgSel(&cfgsel,win,gadgets,GADIX_CFGSEL);
													ActivateGadget(gadgets[GADIX_CFGNAME],win,NULL);
												}
												else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
											}
											break;
										case GADID_BATCHLIST:
											aktnodenr=icode;
											Refr_BatchProcWin(gadgets,win);
											break;
										case GADID_BATCHADD:
											if(batchNode=AllocVec(sizeof(struct batchnode),MEMF_ANY|MEMF_CLEAR)) {
												BuildBatchProcNode(batchNode,"Amplify"," default",BATCH_NODETYP_FX);
												if(aktnodenr<(aktnodect-1)) aktnodenr++;    // it's not the last
												InsertAt(&batchParam.batchList,batchNode,aktnodenr);
												aktnodect++;
												//MSG2("uadd : 0x%08lx 0x%08lx",batchNode,batchNode->node.ln_Name);
												Refr_BatchProcWin(gadgets,win);
											}
											break;
										case GADID_BATCHDEL:
											if(batchNode=(struct batchnode *)RemoveAt(&batchParam.batchList,aktnodenr)) {
												//MSG2("udel : 0x%08lx 0x%08lx",batchNode,batchNode->node.ln_Name);
												if(batchNode->node.ln_Name) FreeVec(batchNode->node.ln_Name);
												FreeVec(batchNode);
												aktnodenr--;aktnodect--;
												Refr_BatchProcWin(gadgets,win);
											}
											break;
										case GADID_BATCHMODPOPUP:
											switch(aktBatchNode->typ) {
												case BATCH_NODETYP_LO: modul=(ModNode *)GetNodeInteractive(&LoaderList  ,GetIndexByName(&LoaderList ,aktBatchNode->mod));break;
												case BATCH_NODETYP_SA: modul=(ModNode *)GetNodeInteractive(&SaverList   ,GetIndexByName(&SaverList  ,aktBatchNode->mod));break;
												case BATCH_NODETYP_FX: modul=(ModNode *)GetNodeInteractive(&EffektList  ,GetIndexByName(&EffektList ,aktBatchNode->mod));break;
											}
											if(modul) {
												BuildBatchProcNode(aktBatchNode,modul->node.ln_Name," default",aktBatchNode->typ);
												Refr_BatchProcWin(gadgets,win);
											}
											break;
										case GADID_BATCHCFGPOPUP:
											switch(aktBatchNode->typ) {
												case BATCH_NODETYP_LO: sprintf(modcfgdir,"progdir:_loaders/%s_L",aktBatchNode->mod);break;
												case BATCH_NODETYP_SA: sprintf(modcfgdir,"progdir:_savers/%s_S",aktBatchNode->mod);break;
												case BATCH_NODETYP_FX: sprintf(modcfgdir,"progdir:_operators/%s",aktBatchNode->mod);break;
											}
											InitCfgSel(&modcfgsel,modcfgdir);
											if((node=GetNodeInteractive(&(modcfgsel.cfgs),GetIndexByName(&(modcfgsel.cfgs),aktBatchNode->cfg)))) {
												BuildBatchProcNode(aktBatchNode,aktBatchNode->mod,node->ln_Name,aktBatchNode->typ);
												Refr_BatchProcWin(gadgets,win);
											}

											DoneCfgSel(&modcfgsel);
											break;
										case GADID_OPTLOG:
											if((batchParam.log=((gadgets[GADIX_OPTLOG]->Flags&GFLG_SELECTED)!=0))) {
												GT_SetGadgetAttrs(gadgets[GADIX_OPTLOGPOPUP],win,0l,GA_Disabled,FALSE,TAG_DONE);
												GT_SetGadgetAttrs(gadgets[GADIX_OPTLOGFN   ],win,NULL,GA_Disabled,FALSE,TAG_DONE);
											}
											else {
												GT_SetGadgetAttrs(gadgets[GADIX_OPTLOGPOPUP],win,NULL,GA_Disabled,TRUE ,TAG_DONE);
												GT_SetGadgetAttrs(gadgets[GADIX_OPTLOGFN   ],win,NULL,GA_Disabled,TRUE ,TAG_DONE);
											}
											break;
										case GADID_OPTLOGPOPUP:
											SetInitialFileAndDrawer((IsNotEmptyString(batchParam.logfn) ? batchParam.logfn : "T:sfxbatch.log"));
											File_Reqtags_TitleText		=(ULONG)GetString(strSelectLogPath);
											File_Reqtags_DoMultiSelect	=FALSE;
											if(AslRequest(File_Request,File_Reqtags)) {
												strmfp(batchParam.logfn,File_Request->fr_Drawer,File_Request->fr_File);
												GT_SetGadgetAttrs(gadgets[GADIX_OPTLOGFN],win,NULL,GTST_String,batchParam.logfn,TAG_DONE);
											}
											break;
										case GADID_OPTLOGFN:
											strcpy(batchParam.logfn,((struct StringInfo *)(gadgets[GADIX_OPTLOGFN]->SpecialInfo))->Buffer);
											break;
										case GADID_OPTREC:
											if((batchParam.rec=((gadgets[GADIX_OPTREC]->Flags&GFLG_SELECTED)!=0))) {
												GT_SetGadgetAttrs(gadgets[GADIX_OPTRECDEPTH],win,NULL,GA_Disabled,FALSE,TAG_DONE);
											}
											else {
												GT_SetGadgetAttrs(gadgets[GADIX_OPTRECDEPTH],win,NULL,GA_Disabled,TRUE ,TAG_DONE);
											}
											break;
										case GADID_OPTRECDEPTH:
											batchParam.recdepth=atoi(((struct StringInfo *)(gadgets[GADIX_OPTLOGFN]->SpecialInfo))->Buffer);
											break;
										case GADID_OPTDELETE:
											batchParam.delfiles=((gadgets[GADIX_OPTDELETE]->Flags&GFLG_SELECTED)!=0);
											break;
									}
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
							if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP) ShowHelp("html/nodes/node01.05.10.html");
						}
					}
					CloseWindow(win);
				}
				else Message(errOpenWindow,NULL,__FILE__,__LINE__);
				FreeGadgets(glist);
				DoneCfgSel(&cfgsel);
			}
			else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateContext,NULL,__FILE__,__LINE__);
		if(ret) BatchProc();    // start batch processing
		FreeBatchProcParams();
		CfgIO_RemConfig(batchcfg);
	}
}

#define FT_FILE 0
#define FT_DIR  1

#define GADID_STOP          0

#define GADID_STACK         (GADID_STOP+1)
#define GADIX_STACK         0

#define GADID_DONE          (GADID_STACK+1)
#define GADIX_DONE          (GADIX_STACK+1)

#define GADID_FILE          (GADID_DONE+1)
#define GADIX_FILE          (GADIX_DONE+1)

#define LAST_GADIX2         (GADIX_FILE+1)

void BatchProc()
{
	struct NewGadget ng;
	struct Window *win;
	struct Gadget *g;
	struct Gadget *glist=NULL;
	struct Gadget *gadgets[LAST_GADIX2];
	struct RastPort *bprp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	UWORD icode;
	UWORD ys1 =2;
	UWORD ys2 =ys1+1+ANYFRM_HEIGHT(2);
	UWORD ys3 =ys2+1+RunTime.scy[FRMCONTENT_YOFFSET+24];
	UWORD ys4 =ys3+1+RunTime.scy[FRMCONTENT_YOFFSET+24];
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1 =2;
	UWORD winw=xs1+3+RunTime.scx[300];
	UWORD xh,yh;
	double h1,h2;
	UWORD lw,tl;	// for fitting filename into the label;

	struct List files;
	struct Node *node,*bjnode;
	struct batchnode *bnode;
	ModNode *fmodul,*lmodul;    // first & last module
	char fn[FILENAME_MAX],fn2[FILENAME_MAX],fn3[FILENAME_MAX],pattern[256],src[FILENAME_MAX],dst[FILENAME_MAX],dir[FILENAME_MAX];
	BOOL cont=TRUE;
	struct FileRequester *batchFRequest;
	struct TagItem BatchFReqtags[]={
/* 00 */ASLFR_TitleText,        NULL,
/* 01 */ASLFR_PubScreenName,    (ULONG)SFX_PUBSCRNAME,
/* 02 */ASLFR_InitialLeftEdge,  180,
/* 03 */ASLFR_InitialTopEdge,   15,
/* 04 */ASLFR_InitialWidth,     300,
/* 05 */ASLFR_InitialHeight,    184,
/* 06 */ASLFR_InitialDrawer,    (ULONG)"progdir:",
/* 07 */ASLFR_InitialFile,      (ULONG)"",
/* 08 */ASLFR_DoSaveMode,       FALSE,
/* 09 */ASLFR_DoMultiSelect,    FALSE,
/* 10 */ASLFR_DoPatterns,       FALSE,
/* 11 */ASLFR_DrawersOnly,      TRUE,
/* 12 */ASLFR_RejectIcons,      TRUE,
/* 13 */ASLFR_FilterDrawers,    FALSE,
		TAG_DONE
	};
	struct FileLock *dirlock;
	struct FileInfoBlock InfoBlock;
	FILE *testf;                    // for checking if a file already exists
	UBYTE postfix;                  // nr to append, when file already exists
	UBYTE aktlo,aktsa,aktfx;							/* backup data */
	char  sample_lo_path[SFXPrefs_SBuf_SamplePathLen];  /* backup of Load-Filerequester */
	char  sample_sa_path[SFXPrefs_SBuf_SamplePathLen];  /* backup of Save-Filerequester */
	ULONG wbid1,wbid2;
	LONG recdepth,res;
	SInfo *si;
	char params[3*MaxParLen],parstr[20];
	UBYTE oldquiet;
	ULONG stack=0,done=0;           // full status
	PWData pwdf,pwds;               // file and step status
	ULONG step;                     // for step status
	UBYTE proz;

	INTRO;

	//-- remember current active modules
	aktlo=RunTime.aloader;
	aktsa=RunTime.asaver;
	aktfx=RunTime.aktfx;
	//-- remember current loader/saver paths
	strcpy(sample_lo_path,sfxprefs_sbuf.sample_lo_path);
	strcpy(sample_sa_path,sfxprefs_sbuf.sample_sa_path);

//  MSG1("current Loader : %d",aktlo);
//  MSG1("current Saver  : %d",aktsa);
//  MSG1("current Effect : %d",aktfx);

	NewList(&files);
	if(!asl_prefs) {
		BatchFReqtags[2].ti_Data=(sfxprefs_gui.xres-300)>>1;
		BatchFReqtags[5].ti_Data= sfxprefs_gui.yres-30;
	}
	else {
		BatchFReqtags[2].ti_Tag=BatchFReqtags[3].ti_Tag=BatchFReqtags[4].ti_Tag=BatchFReqtags[5].ti_Tag=TAG_IGNORE;
	}
	if(batchFRequest=AllocAslRequest(ASL_FileRequest,BatchFReqtags)) {
		//-- loader
		bnode=(struct batchnode *)GetFirstNode(&batchParam.batchList);
		fmodul=(ModNode *)GetNodeByName(&LoaderList,bnode->mod);
		if(fmodul->info.fsel) {
			// get source rootdir and pattern via Filerequest
			BatchFReqtags[ 0].ti_Data=(ULONG)GetString(strLoad);            // titel text
			BatchFReqtags[ 6].ti_Data=(ULONG)sfxprefs_sbuf.sample_lo_path;  // initial drawer
			BatchFReqtags[ 8].ti_Data=(ULONG)FALSE;                         // DoSaveMode
			if(AslRequest(batchFRequest,BatchFReqtags)) {
				//-- adjust path in prefs, so that modules can access that
				strcpy(sfxprefs_sbuf.sample_lo_path,batchFRequest->fr_Drawer);
				sfxprefs_sbuf.sample_lo_path[strlen(batchFRequest->fr_Drawer)]='\0';

				strmfp(src,batchFRequest->fr_Drawer,batchFRequest->fr_File);
				sprintf(fn,"%s, 0",src);                                // initial recdepth
				if(ParsePatternNoCase(batchFRequest->fr_Pattern,pattern,250)==-1) cont=0;
				// add rootdir to filelist
				AddToFileList(&files,fn,FT_DIR);
			}
			else cont=FALSE;
		}
		else {
			char buffer[512],*fnptr,*fnptr2;            /* max 100 tracks and 4 chars per track */
			if((res=GetIndexByName(&LoaderList,bnode->mod))>-1) {
				RunTime.aloader=res;
				strcpy(buffer,"?");
				// call Process() with filename "?"
				LoadSampleRexx(buffer);
//              MSG1("tracks : %s",buffer);
				fnptr=buffer;
				// add returned files to filelist
				while(*fnptr) {
					fnptr[3]='\0';
					fnptr2=fnptr;
					while(*fnptr2==' ') fnptr2++;
					AddToFileList(&files,fnptr2,FT_FILE);stack++;
					fnptr+=4;
				}
			}
		}
		if(cont) {
			//-- saver
			bnode=(struct batchnode *)GetLastNode(&batchParam.batchList);
			lmodul=(ModNode *)GetNodeByName(&SaverList,bnode->mod);
			if(lmodul->info.fsel) {
				// get dest. rootdir via Filerequest
				BatchFReqtags[ 0].ti_Data=(ULONG)GetString(strSaveAs);          // titel text
				BatchFReqtags[ 6].ti_Data=(ULONG)sfxprefs_sbuf.sample_sa_path;  // initial drawer
				BatchFReqtags[ 8].ti_Data=(ULONG)TRUE;                          // DoSaveMode
				if(AslRequest(batchFRequest,BatchFReqtags)) {
					//-- adjust path in prefs, so that modules can access that
					strcpy(sfxprefs_sbuf.sample_sa_path,batchFRequest->fr_Drawer);
					sfxprefs_sbuf.sample_sa_path[strlen(batchFRequest->fr_Drawer)]='\0';

					strmfp(dst,batchFRequest->fr_Drawer,batchFRequest->fr_File);
				}
				else cont=FALSE;
			}
		}
		FreeAslRequest(batchFRequest);
	}
	if(cont) {
		if(g=CreateContext(&glist)) {
			ng.ng_LeftEdge      =xs1+RunTime.scx[110];
			ng.ng_TopEdge       =ys4+RunTime.scy[2];
			ng.ng_Width         =RunTime.scx[80];
			ng.ng_Height        =RunTime.scy[11];
			ng.ng_GadgetText    ="_Stop";
			ng.ng_TextAttr      =RunTime.ScrFont;
			ng.ng_GadgetID      =GADID_STOP;
			ng.ng_Flags         =PLACETEXT_IN;
			ng.ng_VisualInfo    =RunTime.VisualInfo;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

			ng.ng_LeftEdge      =xs1+RunTime.scx[50];
			ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET];
			ng.ng_Width         =RunTime.scx[60];
			ng.ng_GadgetText    ="Stack";
			ng.ng_GadgetID      =GADID_STACK;
			ng.ng_Flags         =PLACETEXT_LEFT;
			sprintf(parstr,"%d",stack);
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,parstr,GTTX_CopyText,TRUE,TAG_DONE);
			gadgets[GADIX_STACK]=g;

			ng.ng_LeftEdge      =xs1+RunTime.scx[230];
			ng.ng_GadgetText    ="Done";
			ng.ng_GadgetID      =GADID_DONE;
			sprintf(parstr,"%d",done);
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,parstr,GTTX_CopyText,TRUE,TAG_DONE);
			gadgets[GADIX_DONE]=g;

			lw=RunTime.scx[240];
			ng.ng_LeftEdge      =xs1+RunTime.scx[50];
			ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET+12];
			ng.ng_Width         =lw;lw-=8;
			ng.ng_GadgetText    ="File";
			ng.ng_GadgetID      =GADID_FILE;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,NULL,GTTX_CopyText,TRUE,GTTX_Clipped,TRUE,TAG_DONE);
			gadgets[GADIX_FILE]=g;

			if(g) {
				MemUsage(sfxtitle);
				Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
				Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
				Dialog_Wintags_InnerWidth	=winw;
				Dialog_Wintags_InnerHeight	=winh;
				Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
				Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
				Dialog_Wintags_Gadgets		=(ULONG)glist;
				Dialog_Wintags_Title		=(ULONG)GetString(menPrjBatchProc);
				Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
				if(win=OpenWindowTagList(NULL,Dialog_Wintags)) {
					GT_RefreshWindow(win,NULL);
					bprp=win->RPort;SetFont(bprp,RunTime.scrfont);

					xh=RunTime.scx[300];
					DrawTitle(bprp,"overall status"     ,xs1,ys1,xh);
					DrawTitle(bprp,"file status"        ,xs1,ys2,xh);
					DrawTitle(bprp,"step status"        ,xs1,ys3,xh);

					DrawGadWB(bprp,xs1,ys1,xh,ANYFRM_HEIGHT(2));                    /* full Status */
					DrawGadWB(bprp,xs1,ys2,xh,RunTime.scy[FRMCONTENT_YOFFSET+24]);  /* file Status */
					DrawGadWB(bprp,xs1,ys3,xh,RunTime.scy[FRMCONTENT_YOFFSET+24]);  /* step Status */
					DrawGadWB(bprp,xs1,ys4,xh,CMDFRM_HEIGHT);                       /* Stop */

					DrawGadBW(bprp,xs1+RunTime.scx[9],ys2+RunTime.scy[FRMCONTENT_YOFFSET+3],2+RunTime.scx[280],2+RunTime.scy[8]);   // Progressbar
					DrawGadBW(bprp,xs1+RunTime.scx[9],ys3+RunTime.scy[FRMCONTENT_YOFFSET+3],2+RunTime.scx[280],2+RunTime.scy[8]);   // Progressbar

					h2=(2+RunTime.scx[280])>>3;

					yh=ys2+2+RunTime.scy[FRMCONTENT_YOFFSET+11];                                // Markers
					SetAPen(bprp,1);h1=xs1+RunTime.scx[9];
					Move(bprp,(int)h1,yh-1);Draw(bprp,(int)h1,yh+6);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+4);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+6);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+4);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+2);h1=xs1+1+RunTime.scx[289];
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+6);

					SetAPen(bprp,2);h1=xs1+RunTime.scx[9]+1;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+6);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+4);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+6);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+4);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+2);h1=xs1+2+RunTime.scx[289];
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+6);

					yh=ys2+7+RunTime.scy[FRMCONTENT_YOFFSET+17];
					SetAPen(bprp,1);
					Move(bprp,xs1+RunTime.scx[  9],yh);Text(bprp,"0%",2);
					Move(bprp,xs1+RunTime.scx[ 70],yh);Text(bprp,"25%",3);
					Move(bprp,xs1+RunTime.scx[140],yh);Text(bprp,"50%",3);
					Move(bprp,xs1+RunTime.scx[210],yh);Text(bprp,"75%",3);
					Move(bprp,xs1+RunTime.scx[265],yh);Text(bprp,"100%",4);

					yh=ys3+2+RunTime.scy[FRMCONTENT_YOFFSET+11];                                // Markers
					SetAPen(bprp,1);h1=xs1+RunTime.scx[9];
					Move(bprp,(int)h1,yh-1);Draw(bprp,(int)h1,yh+6);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+4);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+6);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+4);h1+=h2;
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+2);h1=xs1+1+RunTime.scx[289];
					Move(bprp,(int)h1,yh  );Draw(bprp,(int)h1,yh+6);

					SetAPen(bprp,2);h1=xs1+RunTime.scx[9]+1;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+6);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+4);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+6);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+2);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+4);h1+=h2;
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+2);h1=xs1+2+RunTime.scx[289];
					Move(bprp,(int)h1,yh);Draw(bprp,(int)h1,yh+6);

					yh=ys3+7+RunTime.scy[FRMCONTENT_YOFFSET+17];
					SetAPen(bprp,1);
					Move(bprp,xs1+RunTime.scx[  9],yh);Text(bprp,"0%",2);
					Move(bprp,xs1+RunTime.scx[ 70],yh);Text(bprp,"25%",3);
					Move(bprp,xs1+RunTime.scx[140],yh);Text(bprp,"50%",3);
					Move(bprp,xs1+RunTime.scx[210],yh);Text(bprp,"75%",3);
					Move(bprp,xs1+RunTime.scx[265],yh);Text(bprp,"100%",4);

					pwdf.tx=xs1+RunTime.scx[137];                           // file status
					pwdf.ty=ys2+RunTime.scy[FRMCONTENT_YOFFSET+9];
					pwdf.gy1=ys2+RunTime.scy[FRMCONTENT_YOFFSET+3]+1;
					pwdf.gy2=pwdf.gy1+RunTime.scy[8];
					pwdf.ptr=(double)(pwdf.optr=pwdf.gx1=xs1+1+RunTime.scx[9]);
					pwdf.maxlen=GetNumberOfNodes(&batchParam.batchList);
					pwdf.ptrs=(double)RunTime.scx[280]/(double)pwdf.maxlen;
					pwdf.prozh=100.0/(double)pwdf.maxlen;

					pwds.pw=win;                                          // step status
					pwds.tx=xs1+RunTime.scx[137];
					pwds.ty=ys3+RunTime.scy[FRMCONTENT_YOFFSET+9];
					pwds.gy1=ys3+RunTime.scy[FRMCONTENT_YOFFSET+3]+1;
					pwds.gy2=pwds.gy1+RunTime.scy[8];
					pwds.ptr=(double)(pwds.optr=pwds.gx1=xs1+1+RunTime.scx[9]);
					pwds.type=PWT_EMB;

					oldquiet=RunTime.quiet;
					RunTime.quiet=TRUE;         /* switch on quiet mode */
					RunTime.pwd=&pwds;
					if(batchParam.log) BatchLog(batchParam.logfn,"Batchprocess started\n");
					while(!IsListEmpty(&files) && RunTime.pwd) {
						node=RemHead(&files);       // get filename from the head of filelist
						//MSG1("  --------->  [%s]",node->ln_Name);
						if(node->ln_Type==FT_DIR) {
							recdepth=atoi(&node->ln_Name[strlen(node->ln_Name)-2]);         // get recdepth from node-name
							strcpy(fn2,node->ln_Name);fn2[strlen(fn2)-3]='\0';
							//MSG2("  scanning   [%2d:%s]",recdepth,fn2);

							if(batchParam.log) BatchLog(batchParam.logfn,"  scanning \"%s\"\n",fn2);

							if(dirlock=(struct FileLock *)Lock(fn2,SHARED_LOCK)) {      // read directory and add entries to the end
								if(Examine((BPTR)dirlock,&InfoBlock)) {
									if((InfoBlock.fib_DirEntryType>0) && (InfoBlock.fib_DirEntryType!=3)) { // it's a directory but no link
										while(ExNext((BPTR)dirlock,&InfoBlock)) {
											strmfp(fn,fn2,InfoBlock.fib_FileName);
											//MSG1("    -->       [%s]",fn);
											if((InfoBlock.fib_DirEntryType<0) || (InfoBlock.fib_DirEntryType==3)) { // it's a file or a link
												if(MatchPatternNoCase(pattern,InfoBlock.fib_FileName)) {
													//-- TODO : skip .info files
													AddToFileList(&files,fn,FT_FILE);stack++;
												}
											}
											else {                                                                  // it's a directory
												//MSG2("%d < %d ?",recdepth,batchParam.recdepth);
												if(batchParam.rec && ((recdepth<batchParam.recdepth) || (batchParam.recdepth==-1))) {
													sprintf(fn3,"%s,% 2d",fn,recdepth+1);
													AddToFileList(&files,fn3,FT_DIR);
													strcpy(dir,&fn[strlen(src)]);                   // get local directory
													if(strlen(dir)) {
														//MSG1("    subdir    [%s]",fn);
														sprintf(fn,"%s%s",dst,dir);
														UnLock(CreateDir(fn));
													}
												}
											}
										}
									}
								}
								sprintf(parstr,"%d",stack);GT_SetGadgetAttrs(gadgets[GADIX_STACK],win,NULL,GTTX_Text,parstr,GTTX_CopyText,TRUE,TAG_DONE);
								UnLock((BPTR)dirlock);
							}
						}
						else {
							stack--;sprintf(parstr,"%d",stack);GT_SetGadgetAttrs(gadgets[GADIX_STACK],win,NULL,GTTX_Text,parstr,GTTX_CopyText,TRUE,TAG_DONE);
							//-- update statuswin
							// does the Text fit into the label ?
							tl=TextLength(bprp,node->ln_Name,strlen(node->ln_Name));
							if(tl<lw) {	// yes
								GT_SetGadgetAttrs(gadgets[GADIX_FILE],win,NULL,GTTX_Text,node->ln_Name,TAG_DONE);
							}
							else {		// no, add "..." to the begining and remove char by char until it fits
								char *dstr,*sstr;
								WORD sl=strlen(node->ln_Name)+3;

								if(dstr=malloc(sl)) {
									sstr=&node->ln_Name[1];
									do {
										sprintf(dstr,"...%s",sstr);
										sl--;sstr++;
										tl=TextLength(bprp,dstr,sl);
									} while(tl>lw && *sstr && sl);
									GT_SetGadgetAttrs(gadgets[GADIX_FILE],win,NULL,GTTX_Text,dstr,TAG_DONE);
									free(dstr);
								}
								else {
									GT_SetGadgetAttrs(gadgets[GADIX_FILE],win,NULL,GTTX_Text,node->ln_Name,TAG_DONE);
								}
							}
							pwdf.ptr=(double)(pwdf.optr=pwdf.gx1);
							SetABPenDrMd(bprp,5,0,JAM1);
							RectFill(bprp,pwdf.optr,pwdf.gy1,pwdf.optr+RunTime.scx[280],pwdf.gy2);
							SetABPenDrMd(bprp,3,0,COMPLEMENT);SetWriteMask(bprp,0x07);
							pwdf.pstr[2]=pwdf.pstr[0]=' ';pwdf.pstr[1]='0';pwdf.pstr[3]='%';pwdf.pstr[4]=0; // init percentage string
							Move(bprp,pwdf.tx,pwdf.ty);Text(bprp,pwdf.pstr,4);
							
							if(batchParam.log) BatchLog(batchParam.logfn,"  processing \"%s\"\n",node->ln_Name);

							//MSG1("  processing  [%s]",node->ln_Name);
							stcgfp(fn,node->ln_Name);                   // get local directory
							if(strlen(src)<strlen(fn)) sprintf(dir,"%s/",&fn[strlen(src)]);
							else dir[0]='\0';
							//MSG1("  src         [%s]",src);
							//MSG1("  path        [%s]",fn);

							wbid1=wbid2=0L;
							step=0;
							foreach(bjnode,&batchParam.batchList) {
								//-- update statuswin
								step++;
								Move(bprp,pwdf.tx,pwdf.ty);Text(bprp,pwdf.pstr,4);
								SetDrMd(bprp,JAM1);SetWriteMask(bprp,0xFF);
								RectFill(bprp,pwdf.optr,pwdf.gy1,(pwdf.optr=pwdf.ptr+(UWORD)((double)step*pwdf.ptrs)),pwdf.gy2);
								SetDrMd(bprp,COMPLEMENT|JAM1);SetWriteMask(bprp,0x07);
								proz=(UBYTE)((double)step*pwdf.prozh);
								if(proz>9) pwdf.pstr[0]=48+(proz/10);
								else pwdf.pstr[0]=' ';
								pwdf.pstr[1]=48+(proz%10);
								Move(bprp,pwdf.tx,pwdf.ty);Text(bprp,pwdf.pstr,4);

								//-- perform step
								//MSG3("    modul     [%d,%s,%s]",((struct batchnode *)bjnode)->typ,((struct batchnode *)bjnode)->mod,((struct batchnode *)bjnode)->cfg);
								switch(((struct batchnode *)bjnode)->typ) {
									case BATCH_NODETYP_LO:                      //-- call Loader
										if(batchParam.log) BatchLog(batchParam.logfn,"    loader \"%s\"\n",((struct batchnode *)bjnode)->mod);
										//MSG1("    loader    [%s]",((struct batchnode *)bjnode)->mod);
										if((res=GetIndexByName(&LoaderList,((struct batchnode *)bjnode)->mod))>-1) {
											RunTime.aloader=res;
											SetPreset("loaders",((struct batchnode *)bjnode)->mod,((struct batchnode *)bjnode)->cfg);
											if(LoadSampleRexx(node->ln_Name)) {
												wbid1=RunTime.aktbuf->id;
												//-- build filename for result
												if(lmodul->info.ext[0][0])  sprintf(fn,"%s%s%s.%s",dst,dir,RunTime.aktbuf->node.ln_Name,lmodul->info.ext[0]);
												else                        sprintf(fn,"%s%s%s",   dst,dir,RunTime.aktbuf->node.ln_Name);
												//-- check if file already exists
												postfix=0;
												while(testf=fopen(fn,"rb")) {
													fclose(testf);
													if(lmodul->info.ext[0][0])  sprintf(fn,"%s%s%s%d.%s",dst,dir,RunTime.aktbuf->node.ln_Name,postfix,lmodul->info.ext[0]);
													else                        sprintf(fn,"%s%s%s%d",   dst,dir,RunTime.aktbuf->node.ln_Name,postfix);
													postfix++;
												}
												//MSG3("    result [%s:%7ld:%5ld]",((struct Node *)RunTime.aktbuf)->ln_Name,RunTime.aktbuf->slen,RunTime.aktbuf->srat);
											}
											else {
												cont=FALSE;
												if(batchParam.log) BatchLog(batchParam.logfn,"    !!! failed to load sample\n");
											}
										}
										else {
											cont=FALSE;
											if(batchParam.log) BatchLog(batchParam.logfn,"    !!! failed to activate loader \"%s\"\n",((struct batchnode *)bjnode)->mod);
										}
										break;
									case BATCH_NODETYP_SA:                      //-- call Saver
										if(batchParam.log) {
											BatchLog(batchParam.logfn,"    saver  \"%s\"\n",((struct batchnode *)bjnode)->mod);
											BatchLog(batchParam.logfn,"    = \"%s\"\n",fn);
										}
										//MSG1("    saver     [%s]",((struct batchnode *)bjnode)->mod);
										if((res=GetIndexByName(&SaverList,((struct batchnode *)bjnode)->mod))>-1) {
											RunTime.asaver=res;
											SetPreset("savers",((struct batchnode *)bjnode)->mod,((struct batchnode *)bjnode)->cfg);
											SaveSampleRexx(fn);
										}
										else {
											cont=FALSE;
											if(batchParam.log) BatchLog(batchParam.logfn,"    !!! failed to activate saver \"%s\"\n",((struct batchnode *)bjnode)->mod);
										}
										break;
									case BATCH_NODETYP_FX:                      //-- call Operator
										if(batchParam.log) BatchLog(batchParam.logfn,"    fx     \"%s\"\n",((struct batchnode *)bjnode)->mod);
										//MSG1("    effect    [%s]",((struct batchnode *)bjnode)->mod);
										if((res=GetIndexByName(&EffektList,((struct batchnode *)bjnode)->mod))>-1) {
											RunTime.aktfx=res;
											SetPreset("operators",((struct batchnode *)bjnode)->mod,((struct batchnode *)bjnode)->cfg);
											sprintf(RexxPar1,"%d",wbid1);
											sprintf(RexxPar2,"\0");
											if(ProcessSampleRexx(params)) {
												wbid2=RunTime.aktbuf->id;
												if(wbid2!=wbid1) {
													//MSG3("    result [%s:%7ld:%5ld]",((struct Node *)RunTime.aktbuf)->ln_Name,RunTime.aktbuf->slen,RunTime.aktbuf->srat);
													if(si=GetBufferPtr(wbid1,-1)) { RemSample(si); }
													if(si=GetBufferPtr(wbid2,-1)) { RunTime.aktbuf=si;wbid1=wbid2; }
												}
												else {
													cont=FALSE;
													if(batchParam.log) BatchLog(batchParam.logfn,"    !!! failed to get result\n");
												}
											}
											else {
												cont=FALSE;
												if(batchParam.log) BatchLog(batchParam.logfn,"    !!! failed to process sample\n");
											}
										}
										else {
											cont=FALSE;
											if(batchParam.log) BatchLog(batchParam.logfn,"    !!! failed to activate operator \"%s\"\n",((struct batchnode *)bjnode)->mod);
										}
										break;
								}
								//-- check for events here too, as one step ,may be done so fast that the events doesn't gets recognised by the prowin
								while(imsg=(struct IntuiMessage *)GetMsg(win->UserPort)) {
									iclass  =imsg->Class;
									icode   =imsg->Code;
									ReplyMsg((struct Message *)imsg);
									if(iclass==IDCMP_GADGETUP) RunTime.pwd=NULL;
									if(iclass==IDCMP_CLOSEWINDOW) RunTime.pwd=NULL;
									if(iclass==IDCMP_VANILLAKEY && (icode=='S' || icode=='s')) RunTime.pwd=NULL;
									if(iclass==IDCMP_RAWKEY) {
										if(icode==RKEY_ESC) RunTime.pwd=NULL;
										if(icode==RKEY_HELP) ShowHelp("html/nodes/node01.05.11.html");
									}
								}
								//MSG1("    continue ?  : %s",(cont?"yes":"no"));
								if(!cont) { cont=TRUE;break; }
								if(!RunTime.pwd) break;
							}
							if(si=GetBufferPtr(wbid1,-1)) { RemSample(si); }
							done++; sprintf(parstr,"%d",done );GT_SetGadgetAttrs(gadgets[GADIX_DONE ],win,NULL,GTTX_Text,parstr,GTTX_CopyText,TRUE,TAG_DONE);
							if(batchParam.delfiles) {
								MSG1("XXXX removing file \"%s\"",node->ln_Name);
								DeleteFile(node->ln_Name);
							}
						}
						if(node->ln_Name) FreeVec(node->ln_Name);
						FreeVec(node);
					}
					//-- if user has canceled, then empty list
					while(node=RemHead(&files)) {
						if(node->ln_Name) FreeVec(node->ln_Name);
						FreeVec(node);
					}
					if(batchParam.log && !RunTime.pwd) BatchLog(batchParam.logfn,"  User has canceled execution\n");
					CloseWindow(win);
				}
				else Message(errOpenWindow,NULL,__FILE__,__LINE__);
				FreeGadgets(glist);
			}
			else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateContext,NULL,__FILE__,__LINE__);
		if(batchParam.log) BatchLog(batchParam.logfn,"Batchprocess finished (%ld files)\n",done);
		RunTime.quiet=oldquiet;         /* switch off quiet mode */
	}

	//-- restore active modules
	RunTime.aloader =aktlo;
	RunTime.asaver  =aktsa;
	RunTime.aktfx   =aktfx;
	//-- restore saver/loader paths
	strcpy(sfxprefs_sbuf.sample_lo_path,sample_lo_path);
	strcpy(sfxprefs_sbuf.sample_sa_path,sample_sa_path);

	OUTRO;
}

//-- helper

void Refr_BatchProcWin(struct Gadget *gadgets[],struct Window *win)
{
	aktBatchNode=(struct batchnode *)GetNodeByIndex(&batchParam.batchList,aktnodenr);
	
	GT_SetGadgetAttrs(gadgets[GADIX_BATCHLIST],win,NULL,GTLV_Labels,&batchParam.batchList,GTLV_Selected,aktnodenr,TAG_DONE);

	if((aktnodenr==0) || (aktnodenr==(aktnodect-1))) {
		GT_SetGadgetAttrs(gadgets[GADIX_BATCHDEL],win,NULL,GA_Disabled,TRUE,TAG_DONE);
	}
	else {
		GT_SetGadgetAttrs(gadgets[GADIX_BATCHDEL],win,NULL,GA_Disabled,FALSE,TAG_DONE);
	}

	GT_SetGadgetAttrs(gadgets[GADIX_BATCHMOD],win,NULL,GTTX_Text,aktBatchNode->mod,GTTX_CopyText,FALSE,TAG_DONE);
	GT_SetGadgetAttrs(gadgets[GADIX_BATCHCFG],win,NULL,GTTX_Text,aktBatchNode->cfg,GTTX_CopyText,FALSE,TAG_DONE);
}

void BuildBatchProcNode(struct batchnode *batchNode,STRPTR mod,STRPTR cfg,ULONG typ)
{
	char infostr[255];

	if(batchNode->node.ln_Name) FreeVec(batchNode->node.ln_Name);

	strcpy(batchNode->mod,mod);
	strcpy(batchNode->cfg,cfg);
	batchNode->typ=typ;

	switch(typ) {
		case BATCH_NODETYP_LO: sprintf(infostr,"%-8s \t10 %-20s \t33 %-20s","Loader"  ,mod,cfg);break;
		case BATCH_NODETYP_SA: sprintf(infostr,"%-8s \t10 %-20s \t33 %-20s","Saver"   ,mod,cfg);break;
		case BATCH_NODETYP_FX: sprintf(infostr,"%-8s \t10 %-20s \t33 %-20s","Operator",mod,cfg);break;
	}
	batchNode->node.ln_Name=StringCopy(infostr);
}

void FreeBatchProcParams(void)
{
	struct batchnode *batchNode;
	
	while((batchNode=(struct batchnode *)RemHead(&batchParam.batchList))) {
//      MSG2("adel : 0x%08lx 0x%08lx",batchNode,batchNode->node.ln_Name);
		if(batchNode->node.ln_Name) FreeVec(batchNode->node.ln_Name);
		FreeVec(batchNode);
	}
	NewList(&batchParam.batchList);
}

void LoadBatchProcParams(STRPTR fname)
{
	register ULONG nr,anz;
	struct batchnode *batchNode;
	char sectName[20];
	
	CfgIO_ReadConfig(batchcfg,fname);

	batchParam.log          =CfgIO_ReadBool             (batchcfg,"General" ,"Log"          ,TRUE);
	strcpy(batchParam.logfn,CfgIO_ReadString            (batchcfg,"General" ,"LogFile"      ,"T:sfxbatch.log"));
	batchParam.rec          =CfgIO_ReadBool             (batchcfg,"General" ,"Recursive"    ,TRUE);
	batchParam.recdepth     =CfgIO_ReadSignedInteger    (batchcfg,"General" ,"RecDepth"     ,-1);

	anz                     =CfgIO_ReadUnsignedInteger  (batchcfg,"General" ,"NrNodes"      ,0);
	NewList(&batchParam.batchList);
	for(nr=0;nr<anz;nr++) {
		sprintf(sectName,"Node%03d",nr);
		if(batchNode=AllocVec(sizeof(struct batchnode),MEMF_ANY|MEMF_CLEAR)) {
			BuildBatchProcNode(batchNode,
				CfgIO_ReadString(batchcfg,sectName,"Module",""),
				CfgIO_ReadString(batchcfg,sectName,"Config","default"),
				CfgIO_ReadUnsignedInteger(batchcfg,sectName,"Type",0)
			);
			AddTail(&batchParam.batchList,batchNode);
		}
	}
}

void SaveBatchProcParams(STRPTR fname)
{
	register ULONG nr;
	struct Node *batchNode;
	char sectName[20];

	CfgIO_WriteBool             (batchcfg,"General" ,"Log"          ,batchParam.log);
	CfgIO_WriteString           (batchcfg,"General" ,"LogFile"      ,batchParam.logfn);
	CfgIO_WriteBool             (batchcfg,"General" ,"Recursive"    ,batchParam.rec);
	CfgIO_WriteSignedInteger    (batchcfg,"General" ,"RecDepth"     ,batchParam.recdepth);

	CfgIO_WriteUnsignedInteger  (batchcfg,"General" ,"NrNodes"      ,GetNumberOfNodes(&batchParam.batchList));

	nr=0;
	foreach(batchNode,&batchParam.batchList) {
		sprintf(sectName,"Node%03d",nr);nr++;
		CfgIO_WriteUnsignedInteger  (batchcfg,sectName  ,"Type"         ,((struct batchnode *)batchNode)->typ);
		CfgIO_WriteString           (batchcfg,sectName  ,"Module"       ,((struct batchnode *)batchNode)->mod);
		CfgIO_WriteString           (batchcfg,sectName  ,"Config"       ,((struct batchnode *)batchNode)->cfg);
	}

	CfgIO_WriteConfig(batchcfg,fname);
}

void DefaultBatchProcParams(void)
{
	struct batchnode *batchNode;

	//-- set defaults
	NewList(&batchParam.batchList);
	if(batchNode=AllocVec(sizeof(struct batchnode),MEMF_ANY|MEMF_CLEAR)) {
		BuildBatchProcNode(batchNode,"Universal"," default",BATCH_NODETYP_LO);
		AddTail(&batchParam.batchList,batchNode);
//      MSG2("aadd : 0x%08lx 0x%08lx",batchNode,batchNode->node.ln_Name);
	}
	if(batchNode=AllocVec(sizeof(struct batchnode),MEMF_ANY|MEMF_CLEAR)) {
		BuildBatchProcNode(batchNode,"RIFF-WAV"," default",BATCH_NODETYP_SA);
		AddTail(&batchParam.batchList,batchNode);
//      MSG2("aadd : 0x%08lx 0x%08lx",batchNode,batchNode->node.ln_Name);
	}
	batchParam.log=1;strcpy(batchParam.logfn,"T:sfxbatch.log");
	batchParam.rec=1;batchParam.recdepth=-1;
}

void RecallLastBatchProcParams(void)
{
	if      (!access( "env:sfx/batch.cfg",F_OK|R_OK))             LoadBatchProcParams( "env:sfx/batch.cfg");
	else if (!access("progdir:data/batch/default.cfg",F_OK|R_OK)) LoadBatchProcParams("progdir:data/batch/default.cfg");
	else                                                          DefaultBatchProcParams();
}

void RecallDefBatchProcParams(void)
{
	if      (!access("progdir:data/batch/default.cfg",F_OK|R_OK)) LoadBatchProcParams("progdir:data/batch/default.cfg");
	else                                                          DefaultBatchProcParams();
}

void AddToFileList(struct List *list,STRPTR name,UBYTE type)
{
	struct Node *node;
	
	if(node=AllocVec(sizeof(struct Node),MEMF_ANY|MEMF_CLEAR)) {
		node->ln_Name=StringCopy(name);
		node->ln_Type=type;
		AddTail(list,node);
	}
}

void BatchLog(STRPTR filename,STRPTR fmt,...)
{
	time_t t;
	struct tm *p;
	char logdate[30];
	va_list arglist;
	FILE *log;

	if(filename && (log=fopen(filename,"ab"))) {
		time(&t);p=localtime(&t);					// get date & time
		strftime(logdate,30,"%d-%b-%Y %H:%M:%S",p);
		fprintf(log,"%s : ",logdate);
		va_start(arglist,fmt);
		vfprintf(log,fmt,arglist);
		va_end(arglist);
		fclose(log);
	}
}

//-- eof ----------------------------------------------------------------------
