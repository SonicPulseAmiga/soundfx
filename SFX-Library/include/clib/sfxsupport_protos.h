#ifndef  CLIB_SFXSUPPORT_PROTOS_H
#define  CLIB_SFXSUPPORT_PROTOS_H

/*
**	$VER: sfxsupport_protos.h 4.2 (05.02.02)
**
**	(C) Copyright 1993-2002 Stefan Kost
**	    All Rights Reserved
*/

#include <exec/types.h>

#include <libraries/sfxsupport.h>

/*****************************************************************************/

//-- functions used in all parts of soundfx (NOT related to GUI)
//==> sfx-common.library
	ULONG			GetSFXVersion(void);

	void			LogText(STRPTR msg);

	STRPTR			GetString(ULONG id);

	APTR 			ReAllocVec(APTR addro,ULONG sizeo,ULONG sizen,ULONG typen);

	//-- prefix with "SInfo_"
	void			BufferFill(SInfo *srcbuf,SInfo *dstbuf);
	SInfo *			NewSample(void);
	void			AddSample(SInfo *si,STRPTR name);
	void			RemSample(SInfo *si);
	UBYTE			SetRngMode(SInfo *si);

	//-- prefix with "SInfo_"
	UBYTE			AllocSampleMem(SInfo *srcbuf,ULONG slen,UBYTE chan);
	void			FreeSampleMem(SInfo *si);

	SInfo *			GetBufferPtr(ULONG id,WORD nr);
	UBYTE  			GetBufferNr(ULONG id);

	//-- prefix with "Str_"
	void			FilterString(STRPTR strbuf);		// rename to Str_Filter()
	void			StripString(STRPTR strbuf);		// rename to Str_Strip()
	void			TrimString(STRPTR strbuf);		// rename to Str_Trim()
	STRPTR 			StringCopy(STRPTR src);			// rename to Str_Clone()

	UBYTE			Rate2KeyFrq(ULONG  srat,STRPTR key ,double *frq);
	UBYTE			Key2RateFrq(STRPTR  key,ULONG *srat,double *frq);
	UBYTE			Frq2RateKey(double *frq,ULONG *srat,STRPTR  key);

//-- functions used in all parts of soundfx (related to GUI)
//==> sfx-gui-amiga.library
	void			Message(ULONG messid,STRPTR mess,STRPTR file,ULONG line);
	long			Question(ULONG messid,STRPTR yes,STRPTR no);
	
	void			ADosError(STRPTR filename,ULONG messid,ULONG adoserror,STRPTR file,ULONG line);
	void			StdCError(STRPTR filename,ULONG messid,ULONG stdcerror,STRPTR file,ULONG line);

	void			ShowHelp(STRPTR nodename);

	PWData *		NewProWin(void);
	void			DelProWin(PWData *pwd);
	void			StartProWin(PWData *pwd,ULONG *curlen,STRPTR title,ULONG maxlen);

	void 			DrawGadWB(struct RastPort *rp,UWORD x,UWORD y,UWORD w,UWORD h);
	void 			DrawGadBW(struct RastPort *rp,UWORD x,UWORD y,UWORD w,UWORD h);
	void 			DrawTitle(struct RastPort *rp,STRPTR title,UWORD x,UWORD y,UWORD w);
	void			RText(struct RastPort *rp,STRPTR text,UWORD x,UWORD y);
	void			CheckGad_int(struct Gadget *g,struct Window *win,LONG   *val,UBYTE chkrng,LONG minv,LONG maxv);
	void			CheckGad_dbl(struct Gadget *g,struct Window *win,double *val,UBYTE chkrng,LONG minv,LONG maxv);
	void 			HandleParameter(STRPTR src,struct Window *win,struct Gadget *g);

	ULONG			GetPeriode(ULONG srat,SInfo *si);

//-- functions used in soundfx main only
	long			ServerDataInit(RTime *RunTime_);
	void			ServerDataDone(void);

	void			MemUsage(STRPTR str);								// called by sfx-common.library too

	void			NG_HandleEvents(ULONG iclass,UWORD icode,WORD imx,WORD imy,APTR iadr,struct Window *iwin,BOOL *closedd);
	void			SetStatus(STRPTR msg);								// used to show the shortinfo NG_HandleEvents ->HighlightGadgets->SetStatus

	void			GenIOTables(void);									// initializes data used by loadertools,savertools

	//-- prefix with "SInfo_"
	void			SetSlider(SInfo *si,UBYTE mode);
	void			SetTitle(SInfo *si);
	void			SetName(SInfo *si,STRPTR name);
	STRPTR 	 		JoinSampleNames(STRPTR name1,STRPTR name2);
	void			SetOptWinBounds(UWORD *x,UWORD *y,UWORD w,UWORD h);
	void			ShowSample(void);									// rename to SInfo_Show
	void			HideSample(void);									// rename to SInfo_Hide
	void			RecalcSampleLines(SInfo *si);

	SInfo * 		GetListEntry(void);
	struct Node *	GetNodeInteractive(struct List *list,ULONG index);

	struct Window *	OpenMWindow(struct TagItem *wintags,struct MsgPort *userport,UBYTE *ct,ULONG idcmps);
	void			CloseMWindow(struct Window *win,UBYTE *ct);

	UBYTE			InitScrollerWinStuff(void);
	void			FreeScrollerWinStuff(void);

	ULONG			ZeroSearch(SInfo *si,ULONG aktpos,BYTE dir,BYTE dir2,UBYTE chan);
	ULONG			PeakSearch(SInfo *si,ULONG beg,ULONG len,BYTE dir);

	void			SetNewPlaybackRate(void);

	UBYTE 			ConvMem2Drv(SInfo *si);
	UBYTE 			ConvDrv2Mem(SInfo *si);
	void			ExchangeData(SInfo *src,SInfo *dst);
	void            RecalcLoopPlayData(void);


//-- functions used in all plugins (loader,operator,player,saver), but not in main

	SInfo *			LockBuffer(SInfo *si);
	void			UnlockBuffer(SInfo *si);

	void			SetRngs(SInfo *si,ULONG *rm_start,ULONG *rm_len,UBYTE rmode);

#if defined(SFXLibrary) || defined(SFXMod_Operator) || defined(SFXMod_Saver)
	struct Gadget * AddSource(Source *src,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y);
	void			SetSource(Source *src,struct Window *win,struct Gadget *gad[],ULONG firstix);
	UBYTE			HandleSource(Source *src,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode);
#endif

	void			InitCfgSel(CfgSel *cfgsel,STRPTR path);
	void			DoneCfgSel(CfgSel *cfgsel);
	struct Gadget * AddCfgSel(CfgSel *cfgsel,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y,UWORD h);
	void			SetCfgSel(CfgSel *cfgsel,struct Window *win,struct Gadget *gad[],ULONG firstix);
	UBYTE			HandleCfgSel(CfgSel *cfgsel,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode,ULONG sec,ULONG mic);
	struct Node *	AddCfgSelConfig(CfgSel *cfgsel,STRPTR name);

#if defined(SFXLibrary) || defined(SFXMod_Operator) || defined(SFXMod_Saver) || defined(SFXMod_Loader)
	void			InitFXParams(PrefsData *fxdesc);
	void			DoneFXParams(PrefsData *fxdesc);
	void			FreeFXParams(PrefsData *fxdesc);

	void 			SaveModSettings(APTR cfg,PrefsData *fxdesc);
	UBYTE 			LoadModSettings(APTR cfg,PrefsData *fxdesc);
#endif

	// used by vm,loat,savt
	UBYTE			ASOpen	 (SmpAccess *sa,STRPTR fn,int file_mode);
	void			ASClose	 (SmpAccess *sa);
	UBYTE			ASRead	 (SmpAccess *sa,ULONG length);
	UBYTE			ASReadIRQ(SmpAccess *sa,ULONG length);
	UBYTE			ASWrite	 (SmpAccess *sa,ULONG length);

	SmpAccess		*SAInit(SInfo *si,UBYTE chan,ULONG start,ULONG len,UBYTE vm_mode,ULONG blksize);
	void			SADone(SmpAccess *sa,SInfo *si);
	UBYTE 			CopyData(SInfo *src,SInfo *dst,ULONG srcpos,ULONG dstpos,ULONG len);
	UBYTE			CopyDataCh(SInfo *src,SInfo *dst,UBYTE ch,ULONG srcpos,ULONG dstpos,ULONG len);

//-- functions used in all loaders
#if defined(SFXLibrary) || defined(SFXMod_Loader)
	ULONG			GetDecrunchedLength(UBYTE crunch,ULONG srclen);
	ULONG			ReadData(SInfo *si,LoadData *ldata,ULONG srclen,UBYTE crunch,UBYTE conv,UBYTE channel);
#endif

//-- functions used in all savers
#if defined(SFXLibrary) || defined(SFXMod_Saver)
	ULONG			GetCrunchedLength(UBYTE cruch,ULONG srclen);
	UBYTE		    WriteData(SInfo *si,SaveData *sdata,UBYTE crunch,UBYTE conv,UBYTE channel);
#endif

//-- functions used in savers and loaders
#if defined(SFXMod_Saver) || defined(SFXMod_Loader)
	ULONG 			GetFileLength(STRPTR fn);
	void			SetSampleComment(SInfo *si,STRPTR fn,STRPTR module,STRPTR fmt);
#endif

//-- functions used in all operators
#if defined(SFXLibrary) || defined(SFXMod_Operator)
	void			AssignBShape(Modulator *mod);

	void			AssignInterpolFunc(UBYTE typ,InterpolFuncPtr *InterpolFunc);

	void			Transform(double *fftbuf_r,double *fftbuf_i,UWORD *BitRevTab,double *SinTab,double *CosTab,BYTE dir,UWORD m);
	UBYTE			NewFFTTabs(UWORD **bitrevtab,double **sintab1,double **sintab2,double **costab,UWORD m);
	void			DelFFTTabs(UWORD *bitrevtab,double *sintab1,double *sintab2,double *costab);
	double *		NewWindowFkt(UWORD len,UBYTE typ,double *par);
	void			DelWindowFkt(double *buf);

	SAMPLE			*NewRingBuf(ULONG minlen,SAMPLE **baseptr,ULONG *addrmask,ULONG *size);
	void			DelRingBuf(SAMPLE *rbuf);

	struct Gadget * AddModulator(Modulator *mod,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y);
	void			SetModulator(Modulator *mod,struct Window *win,struct Gadget *gad[],ULONG firstix);
	UBYTE			HandleModulator(Modulator *mod,Source *src1,Source *src2,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode);
	UBYTE			ParseModulatorSet(Modulator *mod,STRPTR prefix,STRPTR params);
	UBYTE			ParseModulatorGet(Modulator *mod,STRPTR prefix,STRPTR params,STRPTR ret);

	struct Gadget * AddInterpolator(Interpolator *inter,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y);
	void			SetInterpolator(Interpolator *inter,struct Window *win,struct Gadget *gad[],ULONG firstix);
	UBYTE			HandleInterpolator(Interpolator *inter,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode);
	UBYTE			ParseInterpolatorSet(Interpolator *inter,STRPTR prefix,STRPTR params);
	UBYTE			ParseInterpolatorGet(Interpolator *inter,STRPTR prefix,STRPTR params,STRPTR ret);

	struct Gadget * AddWinFunction(WinFunction *wfkt,struct Gadget *gad[],struct Gadget *g,ULONG firstid,ULONG firstix,UWORD x,UWORD y);
	void			SetWinFunction(WinFunction *wfkt,struct Window *win,struct Gadget *gad[],ULONG firstix);
	UBYTE			HandleWinFunction(WinFunction *wfkt,struct Window *win,struct Gadget *gad[],ULONG firstid,ULONG firstix,ULONG id,USHORT icode);
	UBYTE			ParseWinFunctionSet(WinFunction *wfkt,STRPTR prefix,STRPTR params);
	UBYTE			ParseWinFunctionGet(WinFunction *wfkt,STRPTR prefix,STRPTR params,STRPTR ret);
#endif

#endif	 /* CLIB_SFXSUPPORT_PROTOS_H */
