#ifndef  CLIB_SFXSUPPORT_PROTOS_H
#define  CLIB_SFXSUPPORT_PROTOS_H

/*
**	$VER: sfxsupport_protos.h 3.0 (17.03.96)
**
**	(C) Copyright 1993-1996 Stefan Kost
**	    All Rights Reserved
*/

#include <exec/types.h>

#include <libraries/sfxsupport.h>

/*****************************************************************************/

long			ServerDataInit(RTime *RunTime_);
void			ServerDataDone(void);

PWInfo *		InitProWin(ULONG slen,char *title);
UBYTE 			RefreshProWin(PWInfo *pwi,ULONG pos);
void			RemoveProWin(PWInfo *pwi);

APTR 			MyAllocMem(ULONG size,ULONG type);
APTR 			MyReAllocMem(APTR addro,ULONG sizeo,ULONG sizen,ULONG typen);
void 			MyFreeMem(APTR addr,ULONG size);

void 			DrawGad(struct RastPort *rp,UWORD x,UWORD y,UWORD w,UWORD h,UBYTE akt);
void 			DrawGadWB(struct RastPort *rp,UWORD x,UWORD y,UWORD w,UWORD h);
void 			DrawGadBW(struct RastPort *rp,UWORD x,UWORD y,UWORD w,UWORD h);
void 			DrawTitle(struct RastPort *rp,STRPTR title,UWORD x,UWORD y,UWORD w);
void			RText(struct RastPort *rp,char *text,UWORD x,UWORD y);
void			CheckGad_int(struct Gadget *Gadget,struct Window *win,LONG   *val,UBYTE chkrng,LONG minv,LONG maxv);
void			CheckGad_dbl(struct Gadget *Gadget,struct Window *win,double *val,UBYTE chkrng,LONG minv,LONG maxv);
void			Swap_wrd(WORD   *val1,WORD   *val2);
void			Swap_int(LONG   *val1,LONG   *val2);
void			Swap_dbl(double *val1,double *val2);

void			Message(ULONG messid,char *mess,char *file,ULONG line);
long			Question(ULONG messid,char *yes,char *no);
STRPTR			GetString(ULONG id);

SInfo * 		GetListEntry(void);
struct Node *	GetNodeInteractive(struct List *list,ULONG index);

ULONG			GetPeriode(ULONG srat,SInfo *entry);
UBYTE			Rate2KeyFrq(ULONG  srat,char  *key ,double *frq);
UBYTE			Key2RateFrq(char   *key,ULONG *srat,double *frq);
UBYTE			Frq2RateKey(double *frq,ULONG *srat,char   *key);

SInfo *			GetBufferPtr(ULONG id,WORD nr);
UBYTE  			GetBufferNr(ULONG id);

void			RefreshMod(struct Window *win,struct Gadget *gad[],SInfo *modbuf,UBYTE bshape,UBYTE bmode,UWORD gnr1,UWORD gnr2,UWORD gnr3,UWORD gnr4);
UBYTE			SetRngMode(SInfo *entry);
void			SetRngs(SInfo *entry,ULONG *rm_start,ULONG *rm_len,UBYTE rmode);
SInfo *			SetValidBuffer(ULONG *modid);
ULONG			ZeroSearch(SInfo *entry,ULONG aktpos,BYTE dir,BYTE dir2,UBYTE chan);
ULONG			PeakSearch(SInfo *entry,ULONG beg,ULONG len,BYTE dir);
void			SetNewRate(void);

struct Gadget * AddModulator(Modulator *mod,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y);
void			SetModulator(Modulator *mod,struct Window *win,struct Gadget *gad[],ULONG firstix);
UBYTE			HandleModulator(Modulator *mod,Source *src1,Source *src2,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode);
void 			HandleParameter(char *src,struct Window *win,struct Gadget *g);

struct Gadget * AddSource(Source *src,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y);
void			SetSource(Source *src,struct Window *win,struct Gadget *gad[],ULONG firstix);
UBYTE			HandleSource(Source *src,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode);

struct Gadget * AddInterpolator(Interpolator *inter,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y);
void			SetInterpolator(Interpolator *inter,struct Window *win,struct Gadget *gad[],ULONG firstix);
UBYTE			HandleInterpolator(Interpolator *inter,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode);

struct Gadget * AddWinFunction(WinFunction *wfkt,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y);
void			SetWinFunction(WinFunction *wfkt,struct Window *win,struct Gadget *gad[],ULONG firstix);
UBYTE			HandleWinFunction(WinFunction *wfkt,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode);

void			InitCfgSel(CfgSel *cfgsel,STRPTR path);
void			DoneCfgSel(CfgSel *cfgsel);
struct Gadget * AddCfgSel(CfgSel *cfgsel,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y,UWORD h);
void			SetCfgSel(CfgSel *cfgsel,struct Window *win,struct Gadget *gad[],ULONG firstix);
UBYTE			HandleCfgSel(CfgSel *cfgsel,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode,ULONG sec,ULONG mic);
struct Node *	AddCfgSelConfig(CfgSel *cfgsel,STRPTR name);

void			InitFXParams(PrefsData *fxdesc);
void			DoneFXParams(PrefsData *fxdesc);
void			FreeFXParams(PrefsData *fxdesc);

void			AssignBShape(Modulator *mod);

void			FilterString(char *strbuf);
void			StripString(char *strbuf);
void			TrimString(char *strbuf);
char *			StringCopy(char *src);
//char *			StringCopy2(char *src);

struct Window *	OpenMWindow(struct TagItem *wintags,struct MsgPort *userport,UBYTE *ct,ULONG idcmps);
void			CloseMWindow(struct Window *win,UBYTE *ct);

void			BufferFill(SInfo *srcbuf,SInfo *dstbuf);
SInfo *			NewSample(void);
UBYTE			AllocSampleMem(SInfo *srcbuf,ULONG slen,UBYTE chan);
void			FreeSampleMem(SInfo *si);
void			AddSample(SInfo *si,char *name);
void			RemSample(SInfo *si);
void			SetSlider(SInfo *si,UBYTE mode);
void			SetTitle(SInfo *si);
void			SetName(SInfo *si,char *name);
void			ShowSample(void);
void			HideSample(void);
void			SetOptWinBounds(UWORD *x,UWORD *y,UWORD w,UWORD h);
SInfo *			LockBuffer(SInfo *si);
void			UnlockBuffer(SInfo *si);

void			RecalcTraceData(SInfo *entry);
void			RecalcSampleLines(SInfo *entry);

UBYTE			InitScrollerWinStuff(void);
void			FreeScrollerWinStuff(void);

void			Help(char *nodename,UWORD six,UWORD tix);
void			ShowHelp(char *nodename);

void			BuildList(UBYTE anz,struct Node *Nodes,struct List *List,ArrangeList *alist);
void			SortAList(UBYTE anz,ArrangeList *alist);
UBYTE			match_nr2ix(UBYTE anz,UBYTE nr,ArrangeList *alist);
UBYTE			match_name2nr(UBYTE anz,UBYTE *name,ArrangeList *alist);

void			Transform(double *fftbuf_r,double *fftbuf_i,UWORD *BitRevTab,double *SinTab,double *CosTab,BYTE dir,UWORD m);
void			InitFFT(UWORD *BitRevTab,double *SinTab1,double *SinTab2,double *CosTab,UWORD m);
void			GenWindow(UWORD len,UBYTE typ,double *par,double *buf);

void			AssignInterpolFunc(UBYTE typ,InterpolFuncPtr *InterpolFunc);

ULONG			GetDecrunchedLength(UBYTE crunch,ULONG srclen);
ULONG			ReadData(SInfo *si,LoadData *ldata,ULONG srclen,UBYTE crunch,UBYTE conv,UBYTE channel);
//UBYTE			ReadChunkHeader(ULONG *type,ULONG *size,FILE *infile);

ULONG			GetCrunchedLength(UBYTE cruch,ULONG srclen);
void			WriteData(SInfo *si,SaveData *sdata,UBYTE crunch,UBYTE conv,UBYTE channel);
//UBYTE			WriteChunkHeader(ULONG type,ULONG size,FILE *outfile);
ULONG 			GetFileLength(char *fn);

void			GenIOTables(void);

PWData *		NewProWin(void);
void			DelProWin(PWData *pwd);
void			StartProWin(PWData *pwd,ULONG *curlen,char *title,ULONG maxlen);

UBYTE 			ConvMem2Drv(SInfo *si);
UBYTE 			ConvDrv2Mem(SInfo *si);
SmpAccess		*SAInit(SInfo *si,UBYTE chan,ULONG start,ULONG len,UBYTE mode,ULONG blksize);
void			SADone(SmpAccess *sa,SInfo *si);
void 			CopyData(SInfo *src,SInfo *dst,ULONG srcpos,ULONG dstpos,ULONG len);
void			CopyDataCh(SInfo *src,SInfo *dst,UBYTE ch,ULONG srcpos,ULONG dstpos,ULONG len);

void 			SaveModSettings(APTR cfg,PrefsData *fxdesc);
UBYTE 			LoadModSettings(APTR cfg,PrefsData *fxdesc);

UBYTE			ParseModulatorSet(Modulator *mod,char *prefix,char *params);
UBYTE			ParseModulatorGet(Modulator *mod,char *prefix,char *params,char *ret);

UBYTE			ParseInterpolatorSet(Interpolator *inter,char *prefix,char *params);
UBYTE			ParseInterpolatorGet(Interpolator *inter,char *prefix,char *params,char *ret);

UBYTE			ParseWinFunctionSet(WinFunction *wfkt,char *prefix,char *params);
UBYTE			ParseWinFunctionGet(WinFunction *wfkt,char *prefix,char *params,char *ret);

UBYTE			ASOpen	 (SmpAccess *sa,char *fn,int mode);
void			ASClose	 (SmpAccess *sa);
void			ASRead	 (SmpAccess *sa,ULONG length);
void			ASReadIRQ(SmpAccess *sa,ULONG length);
void			ASWrite	 (SmpAccess *sa,ULONG length);

SAMPLE			*NewRingBuf(ULONG minlen,SAMPLE **baseptr,ULONG *addrmask,ULONG *size);
void			DelRingBuf(SAMPLE *rbuf,ULONG addrmask);

void			NG_HandleEvents(ULONG iclass,UWORD icode,WORD imx,WORD imy,APTR iadr,struct Window *iwin,BOOL *closedd);
void			SetStatus(STRPTR msg);

ULONG			GetSFXVersion(void);

#endif	 /* CLIB_SFXSUPPORT_PROTOS_H */
