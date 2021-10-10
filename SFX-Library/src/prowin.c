/****h* sfxsupport.library/prowin.c [4.2] *
*
*  NAME
*    prowin.c
*  COPYRIGHT
*    $VER: prowin.c 4.2 (05.06.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    progress bar windows - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    16.Jul.1998
*  MODIFICATION HISTORY
*    05.Jun.2002	V 4.2	most recent version
*    26.Jan.2001	V 4.1	most recent version
*							A : removed usage of synchronous prowin
*							A : support for GetRunTime(quiet)
*    31.Jul.2000	V 1.30	most recent version
*    10.Nov.1998	V 1.20	most recent version
*    16.Jul.1998	V 1.00	initial version
*  NOTES
*
*******
*/

#define PROWIN_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- asynchronous

PWData * ASM LIB_EXPORT(NewProWin(void));
void	 ASM LIB_EXPORT(DelProWin(REG(a0,PWData *pwd)));
void     ASM LIB_EXPORT(StartProWin(REG(a0,PWData *pwd),REG(a1,ULONG *curlen),REG(a2,STRPTR title),REG(d0,ULONG maxlen)));

void 	ASM ProWin_Task(void);

//-- definitions --------------------------------------------------------------

//-- asynchronous

#define PWD_STATUS_OKAY		-1

PWData * SAVEDS ASM LIB_EXPORT(NewProWin(void)) {
	static char name[120];
	STRPTR tname;
	struct TagItem ProcTags[]={
/* 0 */	NP_Entry,		(ULONG)ProWin_Task,
/* 1 */	NP_StackSize,	10000,
/* 2 */	NP_Name,		0L,
/* 3 */	NP_Priority,	0L,
/* 4 */	TAG_DONE
	};
	struct Process *ThisTask=(struct Process *)FindTask(NULL),*ThatTask;
	PWData *pwd;

	if((pwd=AllocVec(sizeof(PWData),MEMF_ANY|MEMF_CLEAR))) {
		if(GetRunTime(quiet)) {
			if(GetRunTime(pwd)) pwd->type=PWT_EMB;
			else pwd->type=PWT_HID;
		}
		else pwd->type=PWT_STD;
//		MSG1("pwd->type=%d",pwd->type);
//		pwd->maxlen=maxlen;							// copy parameters
//		pwd->curlen=curlen;
//		pwd->title=title;
		if(pwd->type<PWT_HID) {		// == PWT_STD, PWD_EMB
			pwd->a4=__builtin_getreg(REG_A4);
			pwd->a6=__builtin_getreg(REG_A6);
			pwd->mainsig=AllocSignal(-1);
			pwd->maintask=(struct Process *)FindTask(NULL);
			if(pwd->mainsig!=-1) {
				pwd->status=PWD_STATUS_OKAY;				// alles okay

				tname=((struct Node *)ThisTask)->ln_Name;	// build taskname
				sprintf(name,SFX_PREFIX"ProWin.%s",tname);				  // better allocate it ?, but when to free it
				ProcTags[2].ti_Data=(ULONG)name;
				ProcTags[3].ti_Data=(ULONG)(((struct Node *)ThisTask)->ln_Pri+1);	// make this one higher than calling task
				Forbid();
				if(ThatTask=CreateNewProc(ProcTags)) ThatTask->pr_Task.tc_UserData=pwd;		// create new process & pass parameters
				Permit();
				if(pwd->status!=PWD_STATUS_OKAY) {											// ProWinTask has a problem
					LIB_INTCALL(Message(pwd->status,NULL,__FILE__,__LINE__));
					FreeSignal(pwd->mainsig);
					FreeVec(pwd);
					pwd=NULL;
				}
			}
			else {
				LIB_INTCALL(Message(errAllocateSignal,NULL,__FILE__,__LINE__));
				FreeVec(pwd);
				pwd=NULL;
			}
		}
	}
	else LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__));
	return(pwd);
}

void SAVEDS ASM LIB_EXPORT(DelProWin(REG(a0,PWData *pwd))) {
	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(pwd,__FUNC__,"1.par",4));

	if(pwd->curlen) {	// it may happen, that StartProWin() never gets called
		*(pwd->curlen)=pwd->maxlen;
	}
	//MSG1("pwd->type=%d",pwd->type);
	if(pwd->type<PWT_HID) {		// == PWT_STD, PWD_EMB
		//MSG("1< : PluginTask.Del : Wait for ProgressBar GUI or job finished");
		Wait(1L<<pwd->mainsig);						// wait for response (avoid modifying data while they're in use) WAIT:1
		pwd->maxlen=0;								// copy parameters
		pwd->curlen=NULL;
		pwd->title=NULL;
		//MSG("2> : PluginTask.Del : About to tell ProgressBar Task that we've got nothing more to do");
		Signal(pwd->worktask,(1L<<pwd->worksig));	// contact worker SIG:2
		//MSG("3< : PluginTask.Del : Wait for ProgressBar Task to get the message");
		Wait(1L<<pwd->mainsig);						// wait for response WAIT:3
		//MSG("-- : PluginTask.Del : got reply");
		FreeSignal(pwd->mainsig);
	}
	else {
		pwd->maxlen=0;								// copy parameters
		pwd->curlen=NULL;
	}
	FreeVec(pwd);
	OUTRO;
}

void SAVEDS ASM LIB_EXPORT(StartProWin(REG(a0,PWData *pwd),REG(a1,ULONG *curlen),REG(a2,STRPTR title),REG(d0,ULONG maxlen))) {
	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(pwd,__FUNC__,"1.par",4));

	if(pwd->type<PWT_HID) {		// == PWT_STD, PWD_EMB
		//MSG("1< : PluginTask.Start : Wait for ProgressBar GUI or job finished");
		Wait(1L<<pwd->mainsig);						// wait for response (avoid modifying data while they're in use) WAIT:1
		*curlen=0;									// !!! do it here (after wait)! otherwise ProgressTask would loop infinitly
		pwd->maxlen=maxlen;							// copy parameters
		pwd->curlen=curlen;
		pwd->title=title;
		pwd->ptr=(double)(pwd->optr=pwd->gx1);
		//MSG("2> : PluginTask.Start : About to tell ProgressBar Task that we've got work to do");
		Signal(pwd->worktask,(1L<<pwd->worksig));	// contact worker SIG:2
		//MSG("3< : PluginTask.Start : Wait for ProgressBar Task to get the message");
		Wait(1L<<pwd->mainsig);						// wait for response WAIT:3
		//MSG("-- : PluginTask.Start : got reply");
	}
	else {
		*curlen=0;									// !!! do it here (after wait)! otherwise ProgressTask would loop infinitly
		pwd->maxlen=maxlen;							// copy parameters
		pwd->curlen=curlen;
	}
	OUTRO;
}

void SAVEDS ASM ProWin_Task(void)
{
	struct Process *ThisTask;			// do not initialize any variables here !!!
	struct NewGadget ng;
	struct Gadget 	*g;
	struct RastPort *prorp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	USHORT icode;
	PWData *pwd;
	register ULONG *curlen,maxlen,oldlen;
	UBYTE proz,running=TRUE,embed=FALSE;
	UWORD ys1=2;
	UWORD ys2;
	UWORD winh;
	UWORD xs1=2;
	UWORD winw;
	UWORD xh,yh,xs,ys;
	double h1,h2;
	struct TagItem wintags[11];	//	can't set data here, because a4 isn't set properly at this time

	ThisTask=(struct Process *)FindTask(NULL);
	if(pwd=ThisTask->pr_Task.tc_UserData) {
		__builtin_putreg(REG_A4,pwd->a4);
		__builtin_putreg(REG_A6,pwd->a6);

		ThisTask->pr_WindowPtr=(APTR)GetRunTime(winBackDrop);
		pwd->worksig =AllocSignal(-1);
		pwd->worktask=ThisTask;
		if(pwd->worksig!=-1) {
			ys2=ys1+7+GetRunTime(scy[24]);
			winh=ys2+2+GetRunTime(scy[14]);
			winw=xs1+3+GetRunTime(scx[300]);

			if(pwd->type==PWT_STD) {
				if(g=CreateContext(&(pwd->proGList))) {
					ng.ng_LeftEdge		=xs1+GetRunTime(scx[110]);
					ng.ng_TopEdge		=ys2+GetRunTime(scy[2]);
					ng.ng_Width			=GetRunTime(scx[80]);
					ng.ng_Height		=GetRunTime(scy[11]);
					ng.ng_GadgetText	="_Stop";
					ng.ng_TextAttr		=GetRunTime(ScrFont);
					ng.ng_GadgetID		=0;
					ng.ng_Flags			=PLACETEXT_IN;
					ng.ng_VisualInfo	=GetRunTime(VisualInfo);
					g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

					if(g) {
						wintags[ 0].ti_Tag=WA_Left;			wintags[0].ti_Data=(ULONG)(GetRunTime(xres)-winw)>>1;
						wintags[ 1].ti_Tag=WA_Top;			wintags[1].ti_Data=(ULONG)(GetRunTime(yres)-winh)>>1;
						wintags[ 2].ti_Tag=WA_InnerWidth;	wintags[2].ti_Data=(ULONG)winw;
						wintags[ 3].ti_Tag=WA_InnerHeight;	wintags[3].ti_Data=(ULONG)winh;
						wintags[ 4].ti_Tag=WA_IDCMP;		wintags[4].ti_Data=(ULONG)BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW/*|IDCMP_INTUITICKS*/;
						wintags[ 5].ti_Tag=WA_Flags;		wintags[5].ti_Data=WFLG_SMART_REFRESH|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
						wintags[ 6].ti_Tag=WA_Gadgets;		wintags[6].ti_Data=(ULONG)pwd->proGList;
						wintags[ 7].ti_Tag=WA_Title;		wintags[7].ti_Data=NULL;
						wintags[ 8].ti_Tag=WA_ScreenTitle;	wintags[8].ti_Data=(ULONG)VERS;			// todo : MemUsage ...
						wintags[ 9].ti_Tag=WA_CustomScreen;	wintags[9].ti_Data=(ULONG)GetRunTime(sfxpubscr);
						wintags[10].ti_Tag=TAG_DONE;
						//-- don't activate statusbar-windows
						//if(GetRunTime(sfxpubscr)==IntuitionBase->FirstScreen) wintags[5].ti_Data|=WFLG_ACTIVATE;
						if(pwd->pw=OpenWindowTagList(0l,wintags)) {
							GT_RefreshWindow(pwd->pw,0l);
							pwd->pw->UserData=WT_ProWin;
							prorp=pwd->pw->RPort;SetFont(prorp,GetRunTime(scrfont));

							xh=GetRunTime(scx[300]);yh=6+GetRunTime(scy[24]);
							//LIB_INTCALL(DrawGadWB(prorp,2,ys1,xh,yh));		// Bar
							Move(prorp,2,ys1+yh);
							SetAPen(prorp,2);Draw(prorp,2,ys1);Draw(prorp,2+xh,ys1);
							SetAPen(prorp,6);Draw(prorp,2+xh,ys1+yh);Draw(prorp,2,ys1+yh);

							yh=GetRunTime(scy[14]);
							//LIB_INTCALL(DrawGadWB(prorp,2,ys2,xh,  GetRunTime(scy[14])));		// Go
							Move(prorp,2,ys2+yh);
							SetAPen(prorp,2);Draw(prorp,2,ys2);Draw(prorp,2+xh,ys2);
							SetAPen(prorp,6);Draw(prorp,2+xh,ys2+yh);Draw(prorp,2,ys2+yh);

							xh=2+GetRunTime(scx[280]);yh=2+GetRunTime(scy[8]);
							xs=xs1+GetRunTime(scx[9]);ys=ys1+GetRunTime(scy[5]);
							//LIB_INTCALL(DrawGadBW(prorp,xs1+GetRunTime(scx[9]),ys1+GetRunTime(scy[5]),2+GetRunTime(scx[280]),2+GetRunTime(scy[8])));	// Progressbar
							Move(prorp,xs,ys+yh);
							SetAPen(prorp,6);Draw(prorp,xs,ys);Draw(prorp,xs+xh,ys);
							SetAPen(prorp,2);Draw(prorp,xs+xh,ys+yh);Draw(prorp,xs,ys+yh);
							SetAPen(prorp,5);RectFill(prorp,xs1+1+GetRunTime(scx[9]),ys1+1+GetRunTime(scy[5]),xs1+1+GetRunTime(scx[289]),ys1+1+GetRunTime(scy[13]));

							h2=(2+GetRunTime(scx[280]))>>3;

							yh=ys1+3+GetRunTime(scy[13]);								// Markers
							SetAPen(prorp,1);h1=xs1+GetRunTime(scx[9]);
							Move(prorp,(int)h1,yh-1);Draw(prorp,(int)h1,yh+6);h1+=h2;
							Move(prorp,(int)h1,yh  );Draw(prorp,(int)h1,yh+2);h1+=h2;
							Move(prorp,(int)h1,yh  );Draw(prorp,(int)h1,yh+4);h1+=h2;
							Move(prorp,(int)h1,yh  );Draw(prorp,(int)h1,yh+2);h1+=h2;
							Move(prorp,(int)h1,yh  );Draw(prorp,(int)h1,yh+6);h1+=h2;
							Move(prorp,(int)h1,yh  );Draw(prorp,(int)h1,yh+2);h1+=h2;
							Move(prorp,(int)h1,yh  );Draw(prorp,(int)h1,yh+4);h1+=h2;
							Move(prorp,(int)h1,yh  );Draw(prorp,(int)h1,yh+2);h1=xs1+1+GetRunTime(scx[289]);
							Move(prorp,(int)h1,yh  );Draw(prorp,(int)h1,yh+6);

							SetAPen(prorp,2);h1=xs1+GetRunTime(scx[9])+1;
							Move(prorp,(int)h1,yh);Draw(prorp,(int)h1,yh+6);h1+=h2;
							Move(prorp,(int)h1,yh);Draw(prorp,(int)h1,yh+2);h1+=h2;
							Move(prorp,(int)h1,yh);Draw(prorp,(int)h1,yh+4);h1+=h2;
							Move(prorp,(int)h1,yh);Draw(prorp,(int)h1,yh+2);h1+=h2;
							Move(prorp,(int)h1,yh);Draw(prorp,(int)h1,yh+6);h1+=h2;
							Move(prorp,(int)h1,yh);Draw(prorp,(int)h1,yh+2);h1+=h2;
							Move(prorp,(int)h1,yh);Draw(prorp,(int)h1,yh+4);h1+=h2;
							Move(prorp,(int)h1,yh);Draw(prorp,(int)h1,yh+2);h1=xs1+2+GetRunTime(scx[289]);
							Move(prorp,(int)h1,yh);Draw(prorp,(int)h1,yh+6);

							yh=ys1+8+GetRunTime(scy[19]);
							SetAPen(prorp,1);
							Move(prorp,xs1+GetRunTime(scx[  9]),yh);Text(prorp,  "0%",2);
							Move(prorp,xs1+GetRunTime(scx[ 70]),yh);Text(prorp, "25%",3);
							Move(prorp,xs1+GetRunTime(scx[140]),yh);Text(prorp, "50%",3);
							Move(prorp,xs1+GetRunTime(scx[210]),yh);Text(prorp, "75%",3);
							Move(prorp,xs1+GetRunTime(scx[265]),yh);Text(prorp,"100%",4);

							pwd->tx =xs1+GetRunTime(scx[137]);
							pwd->ty =ys1+GetRunTime(scy[ 11]);
							pwd->gy1=ys1+GetRunTime(scy[  5])+1;
							pwd->gy2=pwd->gy1+GetRunTime(scy[8]);
							pwd->ptr=(double)(pwd->optr=pwd->gx1=xs1+1+GetRunTime(scx[9]));
						}
						else { pwd->status=errOpenWindow;FreeGadgets(pwd->proGList); }
					}
					else pwd->status=errCreateGadgets;
				}
				else pwd->status=errCreateContext;
			}
			else if(pwd->type==PWT_EMB) {
				embed=TRUE;
				pwd->pw=GetRunTime(pwd)->pw;
				pwd->tx=GetRunTime(pwd)->tx;
				pwd->ty=GetRunTime(pwd)->ty;
				pwd->gy1=GetRunTime(pwd)->gy1;
				pwd->gy2=GetRunTime(pwd)->gy2;
				pwd->gx1=GetRunTime(pwd)->gx1;
				pwd->ptr=GetRunTime(pwd)->ptr;
				pwd->optr=GetRunTime(pwd)->optr;
				prorp=pwd->pw->RPort;
			}
			//MSG("1> : ProgressBarTask : GUI is ready");
			Signal(pwd->maintask,(1L<<pwd->mainsig));					// contact caller (GUI is ready) SIG:1
			while(running) {
				//MSG("2< : ProgressBarTask : waiting for job");
				Wait(1L<<pwd->worksig);										// wait for response (wait for job) WAIT:2
				//MSG("-- : ProgressBarTask : got job");
				if(pwd->title) {
					//MSG("3> : ProgressBarTask : send reply, start to work");
					Signal(pwd->maintask,(1L<<pwd->mainsig));				// contact caller (got it) SIG:3
					curlen=pwd->curlen;						// speedup;
					maxlen=pwd->maxlen;
					if(pwd->pw) {
						SetWindowTitles(pwd->pw,pwd->title,(UBYTE *)~0);

						pwd->ptrs=(double)GetRunTime(scx[280])/(double)maxlen;
						pwd->prozh=100.0/(double)maxlen;

						SetABPenDrMd(prorp,5,0,JAM1);
						RectFill(prorp,pwd->optr,pwd->gy1,pwd->optr+GetRunTime(scx[280]),pwd->gy2);

						SetABPenDrMd(prorp,3,0,COMPLEMENT);SetWriteMask(prorp,0x07);
						pwd->pstr[2]=pwd->pstr[0]=' ';pwd->pstr[1]='0';pwd->pstr[3]='%';pwd->pstr[4]=0;	// init percentage string
						Move(prorp,pwd->tx,pwd->ty);Text(prorp,pwd->pstr,4);
						SetAPen(prorp,3);

						//oldlen=*curlen;
						while((*curlen)<maxlen) {
							Delay(15);
							//Wait(1L<<pwd->pw->UserPort->mp_SigBit);
							oldlen=*curlen;
							while(imsg=(struct IntuiMessage *)GetMsg(pwd->pw->UserPort)) {
								iclass	=imsg->Class;
								icode	=imsg->Code;
								ReplyMsg((struct Message *)imsg);
								if(iclass==IDCMP_GADGETUP) { *curlen=ProWinEnd;if(embed) GetRunTime(pwd)=NULL; }
								if(iclass==IDCMP_CLOSEWINDOW) { *curlen=ProWinEnd;if(embed) GetRunTime(pwd)=NULL; }
								if(iclass==IDCMP_VANILLAKEY && (icode=='S' || icode=='s')) { *curlen=ProWinEnd;if(embed) GetRunTime(pwd)=NULL; }
								if(iclass==IDCMP_RAWKEY) {
									if(icode==RKEY_ESC) { *curlen=ProWinEnd;if(embed) GetRunTime(pwd)=NULL; }
									if(icode==RKEY_HELP) LIB_INTCALL(ShowHelp("html/nodes/node01.05.07.html"));
								}
							}
							//if((*curlen)>oldlen) {
								Move(prorp,pwd->tx,pwd->ty);Text(prorp,pwd->pstr,4);
								SetDrMd(prorp,JAM1);SetWriteMask(prorp,0xFF);
								RectFill(prorp,pwd->optr,pwd->gy1,(pwd->optr=pwd->ptr+(UWORD)((double)oldlen*pwd->ptrs)),pwd->gy2);
								SetDrMd(prorp,COMPLEMENT|JAM1);SetWriteMask(prorp,0x07);
								proz=(UBYTE)((double)oldlen*pwd->prozh);
								//MSG1("-- : ProgressBarTask : progress=%3d",proz);
								if(proz>=100) proz=99; /* avoid 100% as this needs another digit */
								if(proz>9) pwd->pstr[0]=48+(proz/10);
								else pwd->pstr[0]=' ';
								pwd->pstr[1]=48+(proz%10);
								Move(prorp,pwd->tx,pwd->ty);Text(prorp,pwd->pstr,4);
							//}
						}
					}
					else {
						while((*curlen)<maxlen) {
							Delay(15);
							//Wait(1L<<pwd->pw->UserPort->mp_SigBit);
						}
					}
					//MSG("1> : ProgressBarTask : finished work, send signal");
					Signal(pwd->maintask,(1L<<pwd->mainsig));					// contact caller (GUI is ready) SIG:1
				}
				else {
					running=FALSE;
					//MSG("3> : ProgressBarTask : finishing");
				}
			}
			if(!embed && pwd->pw) {
				CloseWindow(pwd->pw);
				FreeGadgets(pwd->proGList);
			}
			FreeSignal(pwd->worksig);

		}
		else pwd->status=errAllocateSignal;
		Signal(pwd->maintask,(1L<<pwd->mainsig));							// contact caller SIG:1 & 3
	}
}

//-- eof ----------------------------------------------------------------------
