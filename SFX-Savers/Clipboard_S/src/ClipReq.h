/******************************************************************************
** ClipReq.h                                                                 **
**---------------------------------------------------------------------------**
** contains the ClipUnit Requester for the Cliboard Loader & Saver           **
**---------------------------------------------------------------------------**
** Version : V 0.1                                                           **
** Date    : 02.07.1997                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

/*-- protos -------------------------------------------------------------------------------------*/

WORD GetClipUnit(void);

void InitClipList(struct List *clipList,struct Node *clipNodes);
void FreeClipList(struct Node *clipNodes);

/*-- functions ----------------------------------------------------------------------------------*/

#define GADID_REQOKAY		0
#define GADID_DELETE	(GADID_REQOKAY+1)
#define GADIX_DELETE	(0)

#define GADID_LIST		(GADID_DELETE+1)
#define GADIX_LIST		(GADIX_DELETE+1)

WORD GetClipUnit(void)
{
	struct List clipList;
	struct Node clipNodes[256];
	struct NewGadget ng;
	struct Window *crWin;
	struct Gadget *g;
	struct Gadget *crGList=0l;
	struct Gadget *crGadgets[GADIX_LIST+1];
	struct RastPort *crrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	UBYTE quit=0,unit=0,tunit;
	ULONG css=0,csm=0,ccs=500,ccm=500;
	UBYTE infostr[256];
	UBYTE akttest=0;
	UWORD ys1=2;
	UWORD ys2=ys1+1+RunTime->scy[99];
	UWORD winh=ys2+2+RunTime->scy[14];
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime->scx[316];
	UWORD xh;
	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_ACTIVEWINDOW,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS,
		WA_Gadgets,			0l,
		WA_Title,			(ULONG)"Choose a clip unit !",
		WA_ScreenTitle,		(ULONG)VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};
	InitClipList(&clipList,clipNodes);

	if(g=CreateContext(&crGList))
	{
		ng.ng_LeftEdge		=xs1+RunTime->scx[39];
		ng.ng_TopEdge		=ys2+RunTime->scy[2];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	=GetString(strOkay);
		ng.ng_TextAttr		=RunTime->ScrFontB;
		ng.ng_GadgetID		=GADID_REQOKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime->VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime->scx[197];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="_Delete";
		ng.ng_GadgetID		=GADID_DELETE;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);
		crGadgets[GADIX_DELETE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[8];
		ng.ng_TopEdge		=ys1+RunTime->scy[10];
		ng.ng_Width			=RunTime->scx[300];
		ng.ng_Height		=RunTime->scy[88];
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_LIST;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,
							GTLV_Labels,&clipList,
							GTLV_Selected,unit,
							GTLV_Top,unit,
							GTLV_ShowSelected,0l,
							GTLV_CallBack,RunTime->tablelvhook,
							GTLV_MaxPen,7,
						TAG_DONE);
		crGadgets[GADIX_LIST]=g;

		if(g)
		{
			wintags[0].ti_Data=(RunTime->xres-winw)>>1;
			wintags[1].ti_Data=(RunTime->yres-winh)>>1;
			wintags[2].ti_Data=winw;
			wintags[3].ti_Data=winh;
			wintags[6].ti_Data=(ULONG)crGList;
			if(crWin=OpenWindowTagList(0l,wintags))
			{
				GT_RefreshWindow(crWin,0l);
				crrp=crWin->RPort;SetFont(crrp,RunTime->scrfont);

				xh=RunTime->scx[316];
				DrawGadWB(crrp,xs1,ys1,xh,RunTime->scy[99]);		/* List */
				DrawGadWB(crrp,xs1,ys2,xh,RunTime->scy[14]);		/* Okay */

				SetAPen(crrp,1);xh=RunTime->scx[6];
				Move(crrp,xs1+2+RunTime->scx[9]      ,ys1+RunTime->scy[7]);Text(crrp,"No." ,3);
				Move(crrp,xs1+2+RunTime->scx[9]+ 5*xh,ys1+RunTime->scy[7]);Text(crrp,"Smp" ,3);
				Move(crrp,xs1+2+RunTime->scx[9]+ 9*xh,ys1+RunTime->scy[7]);Text(crrp,"Type",4);
				Move(crrp,xs1+2+RunTime->scx[9]+21*xh,ys1+RunTime->scy[7]);Text(crrp,"Size",4);
				Move(crrp,xs1+2+RunTime->scx[9]+30*xh,ys1+RunTime->scy[7]);Text(crrp,"Name",4);

				if(clipNodes[unit].ln_Name[15]=='-') GT_SetGadgetAttrs(crGadgets[GADIX_DELETE],crWin,0l,GA_Disabled,TRUE,TAG_DONE);

				while(!quit)
				{
					WaitPort(crWin->UserPort);
					while(imsg=GT_GetIMsg(crWin->UserPort))
					{
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						switch(iclass)
						{
							case IDCMP_GADGETUP:
								switch((int)(((struct Gadget *)iadr)->GadgetID))
								{
									case GADID_REQOKAY:
										quit=1;
//										if(clipNodes[unit].ln_Name[9]=='*') quit=1;
//										else quit=2;
										break;
									case GADID_DELETE:
										if(Question(reqRemEntry,GetString(strYes),GetString(strNo))) {
											if(clipNodes[unit].ln_Name[9]!='*') {
												if((iff->iff_Stream=(ULONG)OpenClipboard(unit))) {
													InitIFFasClip(iff);
													if(!(OpenIFF(iff,IFFF_WRITE))) {
														CloseIFF(iff);
														FreeVec(clipNodes[unit].ln_Name);
														sprintf(infostr,"%03d \t05   \t09 ---- ---- \t21 000000 \t30",unit);
														clipNodes[unit].ln_Name=StringCopy(infostr);
														GT_SetGadgetAttrs(crGadgets[GADIX_LIST],crWin,0l,GTLV_Labels,&clipList,GTLV_Selected,unit,GTLV_Top,unit,TAG_DONE);
													}
													else MSG("OpenIFF failed");
													CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);
												}
												else MSG("OpenClipboard failed");
											}
										}
										break;
									case GADID_LIST:
										tunit=icode;
										if(!akttest) { CurrentTime(&css,&csm);akttest=1; }
										else
										{
											CurrentTime(&ccs,&ccm);
											if(DoubleClick(css,csm,ccs,ccm) && unit==tunit)
											{
												quit=1;
//												if(clipNodes[unit].ln_Name[9]=='*') quit=1;
//												else quit=2;
												css=0;csm=0;ccs=500;ccm=500;akttest=0;
											}
											else { css=ccs;csm=ccm;ccs=500;ccm=500; }
										}
										unit=tunit;
										if(clipNodes[unit].ln_Name[15]=='-') GT_SetGadgetAttrs(crGadgets[GADIX_DELETE],crWin,0l,GA_Disabled,TRUE,TAG_DONE);
										else								GT_SetGadgetAttrs(crGadgets[GADIX_DELETE],crWin,0l,GA_Disabled,FALSE,TAG_DONE);
										break;
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode)
								{
									case KEY_RETURN:
									case KEY_ENTER:
									case 'O':
									case 'o':
										quit=1;break;
									case KEY_ESC:
										quit=2;break;
								}
								break;
//							case IDCMP_RAWKEY:
//								if(icode==0x5F) SendAmigaGuideCmd(RunTime->aghandle,NULL,AGA_Context,14,TAG_DONE);
//								break;
							case IDCMP_CLOSEWINDOW: quit=2;break;
							case IDCMP_ACTIVEWINDOW: WindowToFront(crWin);break;
						}
					}
				}
				CloseWindow(crWin);
			}
			else Message(errOpenWindow,0l,__FILE__,__LINE__);
			FreeGadgets(crGList);
		}
		else Message(errCreateGadgets,0l,__FILE__,__LINE__);
	}
	else Message(errCreateContext,0l,__FILE__,__LINE__);
	FreeClipList(clipNodes);
	return((WORD)((quit==1) ? unit : -1));
}

void InitClipList(struct List *clipList,struct Node *clipNodes)
{
	register UWORD i;
	UBYTE infostr[256],idstr[10],nastr[32];
	ULONG size;
	UBYTE smp;
	struct ContextNode *cn;
	long error;

	NewList(clipList);
	for(i=0;i<256;i++)		// for every clipunit
	{
		smp=' ';size=0;
		strncpy(idstr,"---- ----",9);idstr[9]=0;
		nastr[0]=0;
		if((iff->iff_Stream=(ULONG)OpenClipboard(i)))
		{
			InitIFFasClip(iff);
			if(!(OpenIFF(iff,IFFF_READ)))
			{
				while(1)
				{
					error=ParseIFF(iff,IFFPARSE_RAWSTEP);
					if(error==IFFERR_EOC) continue;
					else if(error) break;		/* Leave the loop if there is any other error. */
					/* If we get here, error was zero. */
					if((cn=CurrentChunk(iff)))
					{
						if(cn->cn_Type==IFF_8SVX || cn->cn_Type==IFF_16SV) smp='*';
						switch(cn->cn_ID)
						{
							case IFF_FORM:
								sprintf(idstr,"FORM %c%c%c%c",(char)((cn->cn_Type&0xFF000000)>>24),(char)((cn->cn_Type&0x00FF0000)>>16),(char)((cn->cn_Type&0x0000FF00)>>8),(char)(cn->cn_Type&0x000000FF));
								size=cn->cn_Size+8L;
								break;
							case IFF_NAME:
								ReadChunkBytes(iff,&nastr,min(cn->cn_Size,31));nastr[min(cn->cn_Size,31)]=0;
								break;
						}
					}
				}
				CloseIFF(iff);
			}
			CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);
		}
		sprintf(infostr,"%03d \t05 %c \t09 %9s \t21 %06ld \t30 %s",i,smp,idstr,size,nastr);
		clipNodes[i].ln_Name=StringCopy(infostr);
		clipNodes[i].ln_Pred=clipNodes[i].ln_Succ=NULL;
		AddTail(clipList,&clipNodes[i]);
	}
}

void FreeClipList(struct Node *clipNodes)
{
	register UWORD i;
	
	for(i=0;i<256;i++) if(clipNodes[i].ln_Name) FreeVec(clipNodes[i].ln_Name);
}

/*-- eof ----------------------------------------------------------------------------------------*/