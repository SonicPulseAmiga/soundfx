/****h* sfxsupport.library/bshapegui-vector.c [4.3] *
*
*  NAME
*    bshapegui-vector.c
*  COPYRIGHT
*    $VER: bshapegui-vector.c 4.3 (13.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Modulator BlendShapes GUI : Vector - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    26.Nov.1995
*  MODIFICATION HISTORY
*    13.Mar.2004	V 4.3	most recent version
*    07.Jul.2003	V 4.2	most recent version
*    25.Apr.2001	V 4.1	most recent version
*							C: ModWin*(),RefreshMW*() naming
*							A: Describe*() functions
*							A: Vector_FrqEnv_Energy (beta)
*							C: BShape naming
*    31.May.2000	V 1.30	most recent version
*    29.Apr.1999	V 1.20	most recent version
*    26.Nov.1995	V 1.00	initial version
*  IDEAS
*  NOTES
*
*******
*/

#define BSHAPEGUI_VECTOR_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Modulator Vector

void ShowWin_ModVector(Modulator *mod,Source *src1,Source *src2);
void RefreshWin_ModVector1(struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1);
void RefreshWin_ModVector2(struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1,UWORD aktnr);

void Load_ModVector_Params(ModVector *mv,STRPTR fname);
void Save_ModVector_Params(ModVector *mv,STRPTR fname);

//-- definitions --------------------------------------------------------------

//-- Modulator Vector

#define GADID_OKAY		0

#define GADID_CFGSEL    (GADID_OKAY+1)
#define GADIX_CFGSEL    0
#define GADID_CFGLIST   (GADID_CFGSEL)
#define GADIX_CFGLIST   (GADIX_CFGSEL)
#define GADID_CFGNAME   (GADID_CFGLIST+1)
#define GADIX_CFGNAME   (GADIX_CFGLIST+1)
#define GADID_CFGADD    (GADID_CFGNAME+1)
#define GADIX_CFGADD    (GADIX_CFGNAME+1)
#define GADID_CFGDEL    (GADID_CFGADD+1)
#define GADIX_CFGDEL    (GADIX_CFGADD+1)

#define GADID_VEC_ADD	(GADID_CFGDEL+1)
#define GADIX_VEC_ADD	(GADIX_CFGDEL+1)

#define GADID_VEC_SUB	(GADID_VEC_ADD+1)
#define GADIX_VEC_SUB	(GADIX_VEC_ADD+1)

#define GADID_VEC_FLIPX	(GADID_VEC_SUB+1)
#define GADID_VEC_FLIPY	(GADID_VEC_FLIPX+1)

#define GADID_VEC_NR	(GADID_VEC_FLIPY+1)
#define GADIX_VEC_NR	(GADIX_VEC_SUB+1)

#define GADID_VEC_POS	(GADID_VEC_NR+1)
#define GADIX_VEC_POS	(GADIX_VEC_NR+1)

#define GADID_VEC_LEVEL	(GADID_VEC_POS+1)
#define GADIX_VEC_LEVEL	(GADIX_VEC_POS+1)

void ShowWin_ModVector(Modulator *mod,Source *src1,Source *src2) {
	struct NewGadget ng;
	struct Gadget *g;
	struct RastPort *mwrp;
	struct Window *mwwin;
	struct Gadget *mwGadgets[GADIX_VEC_LEVEL+1];
	struct Gadget *mwGList=NULL;
	struct IntuiMessage *imsg;
	ULONG iclass,isec,imic;
	APTR iadr;
	USHORT icode;
	ULONG gnr;
	UBYTE quit=FALSE,mouseb=FALSE,mouseo,gh,nr;
	UBYTE aktnr=0,oldnr,cont;
	UWORD mousex,mousey;
	UBYTE omptr=MPTR_STANDARD;
	WORD x1,x2,y1,y2;
	UBYTE col=GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_LINE]^GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_BACK];
	double newpos,newlev;
	LONG val1;
	UWORD ys1 =2;
	UWORD ys2 =ys1+1+ANYFRM_HEIGHT(0)+GetRunTime(scy[104]);
	UWORD ys3 =ys2+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1 =2;
	UWORD xs2 =xs1+1+GetRunTime(scx[614]);
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;
	UWORD xs=xs1+GetRunTime(scx[5])+1,ys=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET])+1;
	UWORD xe=xs1+GetRunTime(scx[5+600])+2,ye=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+100])+2;
	float xm=(float)(GetRunTime(scx[600])-1),ym=(float)(GetRunTime(scy[100])-1);
	register UWORD i;
	struct Node *node;
	CfgSel cfgsel;
	Param paramp,paraml,param;
	char fn[FILENAME_MAX],cn[FNSIZE];
	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE,
		WA_Gadgets,			0l,
		WA_Title,			(ULONG)"Vector",
		WA_ScreenTitle,		(ULONG)VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};
	ModVector *mv=(ModVector *)(mod->bshpdata);

	if(g=CreateContext(&mwGList)) {
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[267]);
		ng.ng_TopEdge		=ys3+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[80]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=GetRunTime(ScrFontB);
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		LIB_INTCALL(InitCfgSel(&cfgsel,"progdir:data/modvector"));
		node=RemHead(&(cfgsel.cfgs));FreeVec(node->ln_Name);FreeVec(node);
		node=RemHead(&(cfgsel.cfgs));FreeVec(node->ln_Name);FreeVec(node);
		g=LIB_INTCALL(AddCfgSel(&cfgsel,mwGadgets,g,GADID_CFGSEL,GADIX_CFGSEL,xs2,ys1,winh-2));

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[5]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[FRMCONTENT_YOFFSET]);
		ng.ng_Width			=GetRunTime(scx[20]);
		ng.ng_GadgetText	="_+";
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetID		=GADID_VEC_ADD;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);
		mwGadgets[GADIX_VEC_ADD]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[26]);
		ng.ng_GadgetText	="_-";
		ng.ng_GadgetID		=GADID_VEC_SUB;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);
		mwGadgets[GADIX_VEC_SUB]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[47]);
		ng.ng_Width			=GetRunTime(scx[50]);
		ng.ng_GadgetText	="Flip_X";
		ng.ng_GadgetID		=GADID_VEC_FLIPX;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[98]);
		ng.ng_GadgetText	="Flip_Y";
		ng.ng_GadgetID		=GADID_VEC_FLIPY;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[180]);
		ng.ng_Width			=GetRunTime(scx[50]);
		ng.ng_GadgetText	="Nr";
		ng.ng_Flags			=PLACETEXT_LEFT;
		ng.ng_GadgetID		=GADID_VEC_NR;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,3,GTIN_Number,aktnr,TAG_DONE);
		mwGadgets[GADIX_VEC_NR]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[280]);
		ng.ng_Width			=GetRunTime(scx[80]);
		ng.ng_GadgetText	="Pos";
		ng.ng_GadgetID		=GADID_VEC_POS;
		sprintf(paramp,"%5.3lf",mv->pos[aktnr]);
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,paramp,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=GetRunTime(stringextend);
		g->Activation|=GACT_STRINGEXTEND;
		mwGadgets[GADIX_VEC_POS]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[400]);
		ng.ng_GadgetText	="Lev";
		ng.ng_GadgetID		=GADID_VEC_LEVEL;
		sprintf(paraml,"%5.3lf",mv->lev[aktnr]);
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,paraml,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=GetRunTime(stringextend);
		g->Activation|=GACT_STRINGEXTEND;
		mwGadgets[GADIX_VEC_LEVEL]=g;

		if(g) {
			wintags[0].ti_Data=(GetRunTime(xres)-winw)>>1;
			wintags[1].ti_Data=(GetRunTime(yres)-winh)>>1;
			wintags[2].ti_Data=winw;
			wintags[3].ti_Data=winh;
			wintags[6].ti_Data=(ULONG)mwGList;
			if(mwwin=OpenWindowTagList(0l,wintags)) {
				GT_RefreshWindow(mwwin,0l);
				mwrp=mwwin->RPort;SetFont(mwrp,GetRunTime(scrfont));

				xh=GetRunTime(scx[614]);

				LIB_INTCALL(DrawTitle(mwrp,"Envelope"     ,xs1,ys1,xh));
				LIB_INTCALL(DrawTitle(mwrp,"Parameters"   ,xs1,ys2,xh));
				LIB_INTCALL(DrawTitle(mwrp,"Presets"      ,xs2,ys1,CFGSEL_WIDTH));

                                     
				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys1,xh,ANYFRM_HEIGHT(0)+GetRunTime(scy[104])));	/* Env-Field */
				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys2,xh,ANYFRM_HEIGHT(1)));					/* Par */
				LIB_INTCALL(DrawGadWB(mwrp,xs1,ys3,xh,CMDFRM_HEIGHT));						/* Okay */
				LIB_INTCALL(DrawGadWB(mwrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT));			/* CfgSel */

				LIB_INTCALL(DrawGadBW(mwrp,xs1+GetRunTime(scx[5]),ys1+GetRunTime(scy[FRMCONTENT_YOFFSET]),GetRunTime(scx[600])+3,GetRunTime(scy[100])+3));	 /* Curve */

				RefreshWin_ModVector1(mwrp,mod,src1,src2,xs1,ys1);

				if((!aktnr) || (aktnr==(mv->anz-1))) {
					GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_SUB],mwwin,0l,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS],mwwin,0l,GTST_String,paramp,GA_Disabled,TRUE,TAG_DONE);
				}
				if(mv->anz==MV_MAX_VECTORS) {                                                
					GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_ADD],mwwin,0l,GA_Disabled,TRUE,TAG_DONE);
				}

				while(!quit) {
					WaitPort(mwwin->UserPort);
					while(imsg=GT_GetIMsg(mwwin->UserPort)) {
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						isec    =imsg->Seconds;
						imic    =imsg->Micros;
						mousex	=imsg->MouseX-mwwin->BorderLeft;
						mousey	=imsg->MouseY-mwwin->BorderTop;
						GT_ReplyIMsg(imsg);
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								gh=LIB_INTCALL(HandleCfgSel   (&cfgsel      ,mwwin,mwGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic));
								if(!gh) {
									switch(gnr) {
										case GADID_OKAY:
											quit=TRUE;break;
										case GADID_CFGLIST:
										case GADID_CFGNAME:
										case GADID_CFGDEL:
											switch(cfgsel.action) {
												case CFGSEL_ACTION_LOAD:
													MSG1("akt config : %s",cfgsel.aktcfg->ln_Name);
													//if(!strnicmp(cfgsel.aktcfg->ln_Name," default",8)) Recall_Def_ModVector_Params(mv);
													//else {
														//if(!strnicmp(cfgsel.aktcfg->ln_Name," current",8)) Recall_Last_ModVector_Params(mv);
														//else {
															sprintf(fn,"%s/%s.cfg",cfgsel.path,cfgsel.aktcfg->ln_Name);Load_ModVector_Params(mv,fn);
														//}
													//}
													RefreshWin_ModVector1(mwrp,mod,src1,src2,xs1,ys1);
													break;
												case CFGSEL_ACTION_GO:
													quit=TRUE;
													break;
											}
											break;
										case GADID_CFGADD:
											nr=0;sprintf(cn,"New%02d.cfg",nr);
											while((FindName(&(cfgsel.cfgs),cn)) && (nr<100)) { nr++;sprintf(cn,"New%02d.cfg",nr); }
											if(nr<100) {
												strmfp(fn,cfgsel.path,cn);Save_ModVector_Params(mv,fn);
												strsfn(fn,NULL,NULL,cn,NULL);
												if(node=(struct Node *)AllocVec(sizeof(struct Node),MEMF_ANY|MEMF_CLEAR)) {
													node->ln_Name=LIB_INTCALL(StringCopy(cn));InsertByNameAsc(&(cfgsel.cfgs),node);
													cfgsel.aktcfg=node;LIB_INTCALL(SetCfgSel(&cfgsel,mwwin,mwGadgets,GADIX_CFGSEL));
													ActivateGadget(mwGadgets[GADIX_CFGNAME],mwwin,0l);
												}
												else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));
											}
											break;
										case GADID_VEC_ADD:
											if(aktnr<(mv->anz-1)) oldnr=aktnr;		// neue Position
											else oldnr=aktnr-1;
											SetABPenDrMd(mwrp,col,col,COMPLEMENT);SetWriteMask(mwrp,col);	// Kurve updaten
											x1=(WORD)(xm*mv->pos[oldnr  ]);y1=(WORD)(ym*mv->lev[oldnr  ]);Move(mwrp,xs+x1,ye-y1);
											x1=(WORD)(xm*mv->pos[oldnr+1]);y1=(WORD)(ym*mv->lev[oldnr+1]);Draw(mwrp,xs+x1,ye-y1);
											SetWriteMask(mwrp,0xFF);SetDrMd(mwrp,JAM1);
											newpos=mv->pos[oldnr+1]-mv->pos[oldnr];
											newlev=mv->lev[oldnr+1]-mv->lev[oldnr];
											for(i=mv->anz;i>oldnr;i--) {				// Platz machen
												mv->pos[i]=mv->pos[i-1];
												mv->lev[i]=mv->lev[i-1];
											}
											mv->pos[oldnr+1]=mv->pos[oldnr]+(newpos/2.0);
											mv->lev[oldnr+1]=mv->lev[oldnr]+(newlev/2.0);
											aktnr=oldnr+1;mv->anz++;
											RefreshWin_ModVector2(mwrp,mod,src1,src2,xs1,ys1,aktnr);

											sprintf(param,"%f",mv->pos[aktnr]);
											UC_ConvertParameter(paramp,param,UGRP_PROPOTION,UC_GetParameterUnit(paramp,UGRP_PROPOTION));
											sprintf(param,"%f",mv->lev[aktnr]);
											UC_ConvertParameter(paraml,param,UGRP_AMPLITUDE,UC_GetParameterUnit(paraml,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_NR   ],mwwin,0l,GTIN_Number,aktnr,TAG_DONE);
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_LEVEL],mwwin,0l,GTST_String,paraml,TAG_DONE);
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS  ],mwwin,0l,GTST_String,paramp,GA_Disabled,FALSE,TAG_DONE);
											if(mv->anz==MV_MAX_VECTORS) GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_ADD],mwwin,0l,GA_Disabled,TRUE,TAG_DONE);
											if(mv->anz==2) GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_SUB],mwwin,0l,GA_Disabled,FALSE,TAG_DONE);
											break;
										case GADID_VEC_SUB:
											RefreshWin_ModVector2(mwrp,mod,src1,src2,xs1,ys1,aktnr);
											for(i=aktnr;i<mv->anz;i++) {				// Platz löschen
												mv->pos[i]=mv->pos[i+1];
												mv->lev[i]=mv->lev[i+1];
											}
											mv->anz--;
											//RefreshWin_ModVector2(mwrp,mod,src1,src2,xs1,ys1,aktnr);
											SetABPenDrMd(mwrp,col,col,COMPLEMENT);SetWriteMask(mwrp,col);	// Kurve updaten
											x1=(WORD)(xm*mv->pos[aktnr-1]);y1=(WORD)(ym*mv->lev[aktnr-1]);Move(mwrp,xs+x1,ye-y1);
											x1=(WORD)(xm*mv->pos[aktnr  ]);y1=(WORD)(ym*mv->lev[aktnr  ]);Draw(mwrp,xs+x1,ye-y1);
											SetWriteMask(mwrp,0xFF);SetDrMd(mwrp,JAM1);

											sprintf(param,"%f",mv->pos[aktnr]);
											UC_ConvertParameter(paramp,param,UGRP_PROPOTION,UC_GetParameterUnit(paramp,UGRP_PROPOTION));
											sprintf(param,"%f",mv->lev[aktnr]);
											UC_ConvertParameter(paraml,param,UGRP_AMPLITUDE,UC_GetParameterUnit(paraml,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_NR   ],mwwin,0l,GTIN_Number,aktnr,TAG_DONE);
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_LEVEL],mwwin,0l,GTST_String,paraml,TAG_DONE);
											if((aktnr!=0) && (aktnr!=(mv->anz-1))) {
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS],mwwin,0l,GTST_String,paramp,GA_Disabled,FALSE,TAG_DONE);
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_SUB],mwwin,0l,GA_Disabled,FALSE,TAG_DONE);
											}
											else {
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS],mwwin,0l,GTST_String,paramp,GA_Disabled,TRUE,TAG_DONE);
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_SUB],mwwin,0l,GA_Disabled,TRUE,TAG_DONE);
											}
											if(mv->anz<MV_MAX_VECTORS) GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_ADD],mwwin,0l,GA_Disabled,FALSE,TAG_DONE);
											break;
										case GADID_VEC_FLIPX:
											for(i=0;i<mv->anz;i++) mv->pos[i]=1.0-mv->pos[i];
											for(i=0;i<(mv->anz>>1);i++) {
												newpos=mv->pos[i];
												mv->pos[i]=mv->pos[(mv->anz-1)-i];
												mv->pos[(mv->anz-1)-i]=newpos;
												newlev=mv->lev[i];
												mv->lev[i]=mv->lev[(mv->anz-1)-i];
												mv->lev[(mv->anz-1)-i]=newlev;
											}
											RefreshWin_ModVector1(mwrp,mod,src1,src2,xs1,ys1);
											sprintf(param,"%f",mv->pos[aktnr]);
											UC_ConvertParameter(paramp,param,UGRP_PROPOTION,UC_GetParameterUnit(paramp,UGRP_PROPOTION));
											sprintf(param,"%f",mv->lev[aktnr]);
											UC_ConvertParameter(paraml,param,UGRP_AMPLITUDE,UC_GetParameterUnit(paraml,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_LEVEL],mwwin,0l,GTST_String,paraml,TAG_DONE);
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS  ],mwwin,0l,GTST_String,paramp,TAG_DONE);
											break;
										case GADID_VEC_FLIPY:
											for(i=0;i<mv->anz;i++) mv->lev[i]=1.0-mv->lev[i];
											RefreshWin_ModVector1(mwrp,mod,src1,src2,xs1,ys1);
											sprintf(param,"%f",mv->pos[aktnr]);
											UC_ConvertParameter(paramp,param,UGRP_PROPOTION,UC_GetParameterUnit(paramp,UGRP_PROPOTION));
											sprintf(param,"%f",mv->lev[aktnr]);
											UC_ConvertParameter(paraml,param,UGRP_AMPLITUDE,UC_GetParameterUnit(paraml,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_LEVEL],mwwin,0l,GTST_String,paraml,TAG_DONE);
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS  ],mwwin,0l,GTST_String,paramp,TAG_DONE);
											break;
										case GADID_VEC_NR:
											LIB_INTCALL(CheckGad_int(mwGadgets[GADIX_VEC_NR],mwwin,&val1,1,0,MV_MAX_VECTORS));
											aktnr=(UBYTE)val1;
											sprintf(param,"%f",mv->pos[aktnr]);
											UC_ConvertParameter(paramp,param,UGRP_PROPOTION,UC_GetParameterUnit(paramp,UGRP_PROPOTION));
											sprintf(param,"%f",mv->lev[aktnr]);
											UC_ConvertParameter(paraml,param,UGRP_AMPLITUDE,UC_GetParameterUnit(paraml,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_LEVEL],mwwin,0l,GTST_String,paraml,TAG_DONE);
											if((aktnr!=0) && (aktnr!=(mv->anz-1))) {
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS],mwwin,0l,GTST_String,paramp,GA_Disabled,FALSE,TAG_DONE);
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_SUB],mwwin,0l,GA_Disabled,FALSE,TAG_DONE);
												ActivateGadget(mwGadgets[GADIX_VEC_POS],mwwin,0L);
											}
											else {
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS],mwwin,0l,GTST_String,paramp,GA_Disabled,TRUE,TAG_DONE);
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_SUB],mwwin,0l,GA_Disabled,TRUE,TAG_DONE);
												ActivateGadget(mwGadgets[GADIX_VEC_LEVEL],mwwin,0L);
											}
											break;
										case GADID_VEC_POS:
											LIB_INTCALL(HandleParameter(paramp,mwwin,mwGadgets[GADIX_VEC_POS]));
											UC_ConvertParameter(param,paramp,UGRP_PROPOTION,PROP_FC);
											newpos=atof(param);
											if((aktnr!=0) && (aktnr!=(mv->anz-1))) {
												if((newpos<mv->pos[aktnr-1]) || (newpos>mv->pos[aktnr+1])) newpos=(mv->pos[aktnr+1]+mv->pos[aktnr-1])/2.0;
											}
											else {
												if((aktnr==0)			&& (newpos<0.0)) newpos=0.0;
												if((aktnr==(mv->anz-1)) && (newpos>1.0)) newpos=1.0;
											}
											RefreshWin_ModVector2(mwrp,mod,src1,src2,xs1,ys1,aktnr);
											mv->pos[aktnr]=newpos;
											RefreshWin_ModVector2(mwrp,mod,src1,src2,xs1,ys1,aktnr);
											sprintf(param,"%f",mv->pos[aktnr]);
											UC_ConvertParameter(paramp,param,UGRP_PROPOTION,UC_GetParameterUnit(paramp,UGRP_PROPOTION));
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS  ],mwwin,0l,GTST_String,paramp,TAG_DONE);
											ActivateGadget(mwGadgets[GADIX_VEC_LEVEL],mwwin,0L);
											break;
										case GADID_VEC_LEVEL:
											LIB_INTCALL(HandleParameter(paraml,mwwin,mwGadgets[GADIX_VEC_LEVEL]));
											UC_ConvertParameter(param,paraml,UGRP_AMPLITUDE,PROP_FC);
											newlev=atof(param);
											if(newlev<0.0) newlev=0.0;
											else if(newlev>1.0) newlev=1.0;
											RefreshWin_ModVector2(mwrp,mod,src1,src2,xs1,ys1,aktnr);
											mv->lev[aktnr]=newlev;
											RefreshWin_ModVector2(mwrp,mod,src1,src2,xs1,ys1,aktnr);
											sprintf(param,"%f",mv->lev[aktnr]);
											UC_ConvertParameter(paraml,param,UGRP_AMPLITUDE,UC_GetParameterUnit(paraml,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_LEVEL],mwwin,0l,GTST_String,paraml,TAG_DONE);
											ActivateGadget(mwGadgets[GADIX_VEC_NR],mwwin,0L);
											break;
									}
								}
								break;
							case IDCMP_MOUSEBUTTONS:
								if(icode==SELECTDOWN) {
									mouseb=FALSE;                    
									if(mousex>=xs && mousex<=xe && mousey>=ys && mousey<=ye) {
										oldnr=aktnr;
										for(i=0;i<mv->anz;i++) {
											x1=(WORD)(xm*mv->pos[i])-3;x2=x1+6;
											y1=(WORD)(ym*mv->lev[i])-2;y2=y1+4;
											x1=max(x1,0);x2=min(x2,(WORD)xm);
											y1=max(y1,0);y2=min(y2,(WORD)ym);
											if(mousex>=xs+x1 && mousex<=xs+x2 && mousey>=ye-y2 && mousey<=ye-y1) { aktnr=i;i=mv->anz;mouseb=TRUE; }
										}
										if(mouseb && aktnr!=oldnr) {
											sprintf(param,"%f",mv->pos[aktnr]);
											UC_ConvertParameter(paramp,param,UGRP_PROPOTION,UC_GetParameterUnit(paramp,UGRP_PROPOTION));
											sprintf(param,"%f",mv->lev[aktnr]);
											UC_ConvertParameter(paraml,param,UGRP_AMPLITUDE,UC_GetParameterUnit(paraml,UGRP_AMPLITUDE));
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_NR   ],mwwin,0l,GTIN_Number,aktnr,TAG_DONE);
											GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_LEVEL],mwwin,0l,GTST_String,paraml,TAG_DONE);
											if(aktnr!=0 && aktnr!=(mv->anz-1)) {
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS],mwwin,0l,GTST_String,paramp,GA_Disabled,FALSE,TAG_DONE);
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_SUB],mwwin,0l,GA_Disabled,FALSE,TAG_DONE);
											}
											else {
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS],mwwin,0l,GTST_String,paramp,GA_Disabled,TRUE,TAG_DONE);
												GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_SUB],mwwin,0l,GA_Disabled,TRUE,TAG_DONE);
											}
										}
									}
								}
								if(icode==SELECTUP) {
									mouseb=FALSE;
									sprintf(param,"%f",mv->pos[aktnr]);
									UC_ConvertParameter(paramp,param,UGRP_PROPOTION,UC_GetParameterUnit(paramp,UGRP_PROPOTION));
									sprintf(param,"%f",mv->lev[aktnr]);
									UC_ConvertParameter(paraml,param,UGRP_AMPLITUDE,UC_GetParameterUnit(paraml,UGRP_AMPLITUDE));
									GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_LEVEL],mwwin,0l,GTST_String,paraml,TAG_DONE);
									if(aktnr!=0 && aktnr!=(mv->anz-1))
										GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS  ],mwwin,0l,GTST_String,paramp,GA_Disabled,FALSE,TAG_DONE);
									else
										GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS  ],mwwin,0l,GTST_String,paramp,GA_Disabled,TRUE,TAG_DONE);
								}		/* No break !!! */
							case IDCMP_MOUSEMOVE:
								if((mousex>=xs) && (mousex<=xe) && (mousey>=ys) && (mousey<=ye)) {
									if(mouseb) {
										newlev=(ye-mousey)/(double)GetRunTime(scy[100]);
										if((aktnr!=0) && (aktnr!=(mv->anz-1))) newpos=(mousex-xs)/(double)GetRunTime(scx[600]);
										else newpos=mv->pos[aktnr];
										cont=1;
										if((aktnr>0) && (newpos<=mv->pos[aktnr-1])) cont=0;
										if((aktnr<(mv->anz-1)) && (newpos>=mv->pos[aktnr+1])) cont=0;
										if(cont) {
											if(newlev!=mv->lev[aktnr] || newpos!=mv->pos[aktnr]) {
												RefreshWin_ModVector2(mwrp,mod,src1,src2,xs1,ys1,aktnr);
												if(newlev!=mv->lev[aktnr]) {
													if(newlev>1.0) newlev=1.0;
													else if(newlev<0.0) newlev=0.0;
													mv->lev[aktnr]=newlev;
													sprintf(param,"%f",mv->lev[aktnr]);
													UC_ConvertParameter(paraml,param,UGRP_AMPLITUDE,UC_GetParameterUnit(paraml,UGRP_AMPLITUDE));
													GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_LEVEL],mwwin,0l,GTST_String,paraml,TAG_DONE);
												}
												if(newpos!=mv->pos[aktnr]) {
													//if(newpos>1.0) newpos=1.0;
													//else if(newpos<0.0) newpos=0.0;
													mv->pos[aktnr]=newpos;
													sprintf(param,"%f",mv->pos[aktnr]);
													UC_ConvertParameter(paramp,param,UGRP_PROPOTION,UC_GetParameterUnit(paramp,UGRP_PROPOTION));
													GT_SetGadgetAttrs(mwGadgets[GADIX_VEC_POS  ],mwwin,0l,GTST_String,paramp,TAG_DONE);
												}
												RefreshWin_ModVector2(mwrp,mod,src1,src2,xs1,ys1,aktnr);
											}
										}
										if(omptr!=MPTR_MOVE) { SetWindowPointer(mwwin,WA_Pointer,GetRunTime(MPtrMove)->Pointer,TAG_DONE);omptr=MPTR_MOVE; }
									}
									else {
										mouseo=FALSE;
										for(i=0;i<mv->anz;i++) {
											x1=(WORD)(xm*mv->pos[i])-3;x2=x1+6;
											y1=(WORD)(ym*mv->lev[i])-2;y2=y1+4;
											x1=max(x1,0);x2=min(x2,(WORD)xm);
											y1=max(y1,0);y2=min(y2,(WORD)ym);
											if(mousex>=xs+x1 && mousex<=xs+x2 && mousey>=ye-y2 && mousey<=ye-y1) {
												if(omptr!=MPTR_MOVE) { SetWindowPointer(mwwin,WA_Pointer,GetRunTime(MPtrMove)->Pointer,TAG_DONE);omptr=MPTR_MOVE; }
												mouseo=TRUE;
												break;
											}
										}
										if((!mouseo) && (omptr!=MPTR_STANDARD)) { SetWindowPointer(mwwin,TAG_DONE);omptr=MPTR_STANDARD; }
									}
								}
								else {
									if(omptr!=MPTR_STANDARD) { SetWindowPointer(mwwin,TAG_DONE);omptr=MPTR_STANDARD; }
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									case KEY_ENTER:
									case KEY_RETURN:
									case 'O':
									case 'o':
										quit=1;break;
									case KEY_TAB:
										ActivateGadget(mwGadgets[GADIX_VEC_NR],mwwin,0l);
										break;
									case KEY_ESC:
										quit=2;break;
								}
								break;
							case IDCMP_CLOSEWINDOW:
								quit=2;break;
							case IDCMP_RAWKEY:
								if(icode==RKEY_HELP)
									LIB_INTCALL(ShowHelp("html/nodes/node01.07.03.html"));
								break;
						}
					}
				}
				CloseWindow(mwwin);
			}
			else LIB_INTCALL(Message(errOpenWindow,NULL,__FILE__,__LINE__));
		}
		else LIB_INTCALL(Message(errCreateGadgets,NULL,__FILE__,__LINE__));
		FreeGadgets(mwGList);
		LIB_INTCALL(DoneCfgSel(&cfgsel));
	}
	else LIB_INTCALL(Message(errCreateContext,NULL,__FILE__,__LINE__));
	if(quit!=2) Describe_ModVector(mod);
}

void RefreshWin_ModVector1(struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1) {
	register UWORD i;
	LONG xoff=xs1+GetRunTime(scx[5])+1,yoff=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+100])+2;
	WORD x1,x2,y1,y2;
	float xm=(float)(GetRunTime(scx[600])-1),ym=(float)(GetRunTime(scy[100])-1);
	ModVector *mv=((ModVector *)mod->bshpdata);
	UBYTE col=GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_LINE]^GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_BACK];

	SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_BACK]);
	RectFill(rp,xs1+GetRunTime(scx[5])+2,ys1+GetRunTime(scy[FRMCONTENT_YOFFSET])+2,xs1+GetRunTime(scx[5+600])+1,ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+100])+1);
	SetABPenDrMd(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_GRID],GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_BACK],JAM2);
	Move(rp,xoff				     ,yoff-GetRunTime(scy[ 50]));Draw(rp,xoff+GetRunTime(scx[600]),yoff-GetRunTime(scy[50]));
	Move(rp,xoff+GetRunTime(scx[300]),yoff-GetRunTime(scy[100]));Draw(rp,xoff+GetRunTime(scx[300]),yoff);
	SetDrPt(rp,0xAAAA);
	Move(rp,xoff				     ,yoff-GetRunTime(scy[ 25]));Draw(rp,xoff+GetRunTime(scx[600]),yoff-GetRunTime(scy[25]));
	Move(rp,xoff				     ,yoff-GetRunTime(scy[ 75]));Draw(rp,xoff+GetRunTime(scx[600]),yoff-GetRunTime(scy[75]));
	Move(rp,xoff+GetRunTime(scx[150]),yoff-GetRunTime(scy[100]));Draw(rp,xoff+GetRunTime(scx[150]),yoff);
	Move(rp,xoff+GetRunTime(scx[450]),yoff-GetRunTime(scy[100]));Draw(rp,xoff+GetRunTime(scx[450]),yoff);
	SetDrPt(rp,0xFFFF);
	SetABPenDrMd(rp,col,col,COMPLEMENT);SetWriteMask(rp,col);

	x1=(WORD)(xm*mv->pos[0]);
	y1=(WORD)(ym*mv->lev[0]);
	Move(rp,xoff+x1,yoff-y1);			// curve
	for(i=1;i<mv->anz;i++) {
		x1=(WORD)(xm*mv->pos[i]);
		y1=(WORD)(ym*mv->lev[i]);
		Draw(rp,xoff+x1,yoff-y1);
	}
	for(i=0;i<mv->anz;i++) {			// handles
		x1=(WORD)(xm*mv->pos[i])-3;x2=x1+6;
		y1=(WORD)(ym*mv->lev[i])-2;y2=y1+4;
		x1=max(x1,0);x2=min(x2,(WORD)xm);
		y1=max(y1,0);y2=min(y2,(WORD)ym);
		RectFill(rp,xoff+x1,yoff-y2,xoff+x2,yoff-y1);
	}
	SetWriteMask(rp,0xFF);SetDrMd(rp,JAM1);
}

void RefreshWin_ModVector2(struct RastPort *rp,Modulator *mod,Source *src1,Source *src2,UWORD xs1,UWORD ys1,UWORD aktnr) {
	register UWORD i;
	LONG xoff=xs1+GetRunTime(scx[5])+1,yoff=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+100])+2;
	WORD x1,x2,y1,y2,pr,ne;
	float xm=(float)(GetRunTime(scx[600])-1),ym=(float)(GetRunTime(scy[100])-1);
	ModVector *mv=((ModVector *)mod->bshpdata);
	UBYTE col=GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_LINE]^GetRunTime(sfxprefs_gui)->Pens[PEN_SMP_BACK];

	pr=max(0,aktnr-1);
	ne=min(mv->anz,aktnr+2);

	SetABPenDrMd(rp,col,col,COMPLEMENT);SetWriteMask(rp,col);

	x1=(WORD)(xm*mv->pos[pr]);						// curve
	y1=(WORD)(ym*mv->lev[pr]);
	Move(rp,xoff+x1,yoff-y1);
	for(i=pr+1;i<ne;i++) {
		x1=(WORD)(xm*mv->pos[i]);
		y1=(WORD)(ym*mv->lev[i]);
		Draw(rp,xoff+x1,yoff-y1);
	}

	x1=(WORD)(xm*mv->pos[aktnr])-3;x2=x1+6;			// handle
	y1=(WORD)(ym*mv->lev[aktnr])-2;y2=y1+4;
	x1=max(x1,0);x2=min(x2,(WORD)xm);
	y1=max(y1,0);y2=min(y2,(WORD)ym);
	RectFill(rp,xoff+x1,yoff-y2,xoff+x2,yoff-y1);

	SetWriteMask(rp,0xFF);SetDrMd(rp,JAM1);
}

void Load_ModVector_Params(ModVector *mv,STRPTR fname) {
	APTR cfg;
	register UWORD j;
	char strItemName[20],*strItemData;

	if(cfg=CfgIO_NewConfig("ModVector"," %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s")) {
		CfgIO_ReadConfig(cfg,fname);

		mv->anz=CfgIO_ReadUnsignedInteger(cfg,"ModVector","Ve_Anz"    ,11);
		for(j=0;j<mv->anz;j++) {
			sprintf(strItemName,"Ve_Point%03d",j);
			strItemData=CfgIO_ReadString      (cfg,"ModVector",strItemName,"1.0,0.0");
			mv->pos[j]=atof(CfgIO_GetSubItem(strItemData,&strItemData,','));
			mv->lev[j]=atof(CfgIO_GetSubItem(strItemData,&strItemData,','));
		}
		CfgIO_RemConfig(cfg);
	}
}

void Save_ModVector_Params(ModVector *mv,STRPTR fname) {
	APTR cfg;
	register UWORD j;
	char strItemName[20],strItemData[50];

	if(cfg=CfgIO_NewConfig("ModVector"," %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s")) {
		CfgIO_WriteUnsignedInteger(cfg,"ModVector","Ve_Anz"    ,mv->anz);
		for(j=0;j<mv->anz;j++) {
			sprintf(strItemName,"Ve_Point%03d",j);
			sprintf(strItemData,"%8.6lf,%8.6lf",mv->pos[j],mv->lev[j]);
			CfgIO_WriteString     (cfg,"ModVector",strItemName,strItemData);
		}

		CfgIO_WriteConfig(cfg,fname);
		CfgIO_RemConfig(cfg);
	}
}

//-- eof ----------------------------------------------------------------------
