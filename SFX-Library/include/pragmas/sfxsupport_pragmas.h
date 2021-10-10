/**/
/* sfxsupport.library*/
/**/
/**/
/* V 1.0*/
/**/
#pragma libcall SFXSupportBase ServerDataInit 1e 801
#pragma libcall SFXSupportBase ReAllocVec 24 210804
#pragma libcall SFXSupportBase RText 2a 109804
#pragma libcall SFXSupportBase CheckGad_int 30 210A9806
#pragma libcall SFXSupportBase CheckGad_dbl 36 210A9806
#pragma libcall SFXSupportBase Message 3c 198004
#pragma libcall SFXSupportBase Question 42 98003
#pragma libcall SFXSupportBase GetListEntry 48 0
#pragma libcall SFXSupportBase GetPeriode 4e 8002
#pragma libcall SFXSupportBase Rate2KeyFrq 54 98003
#pragma libcall SFXSupportBase Key2RateFrq 5a A9803
#pragma libcall SFXSupportBase Frq2RateKey 60 A9803
#pragma libcall SFXSupportBase GetBufferPtr 66 1002
#pragma libcall SFXSupportBase GetBufferNr 6c 001
#pragma libcall SFXSupportBase SetRngMode 72 801
#pragma libcall SFXSupportBase SetRngs 78 0A9804
#pragma libcall SFXSupportBase ZeroSearch 7e 3210805
#pragma libcall SFXSupportBase PeakSearch 84 210804
#pragma libcall SFXSupportBase SetNewPlaybackRate 8a 0
#pragma libcall SFXSupportBase AddModulator 90 3210A9807
#pragma libcall SFXSupportBase SetModulator 96 0A9804
#pragma libcall SFXSupportBase HandleModulator 9c 3210CBA9809
#pragma libcall SFXSupportBase HandleParameter a2 A9803
#pragma libcall SFXSupportBase AddSource a8 3210A9807
#pragma libcall SFXSupportBase SetSource ae 0A9804
#pragma libcall SFXSupportBase HandleSource b4 3210A9807
#pragma libcall SFXSupportBase AddInterpolator ba 3210A9807
#pragma libcall SFXSupportBase SetInterpolator c0 0A9804
#pragma libcall SFXSupportBase HandleInterpolator c6 3210A9807
#pragma libcall SFXSupportBase AddWinFunction cc 3210A9807
#pragma libcall SFXSupportBase SetWinFunction d2 0A9804
#pragma libcall SFXSupportBase HandleWinFunction d8 3210A9807
#pragma libcall SFXSupportBase AssignBShape de 801
#pragma libcall SFXSupportBase FilterString e4 801
#pragma libcall SFXSupportBase StripString ea 801
#pragma libcall SFXSupportBase StringCopy f0 801
#pragma libcall SFXSupportBase OpenMWindow f6 0A9804
#pragma libcall SFXSupportBase CloseMWindow fc 9802
#pragma libcall SFXSupportBase BufferFill 102 9802
#pragma libcall SFXSupportBase NewSample 108 0
#pragma libcall SFXSupportBase AllocSampleMem 10e 10803
#pragma libcall SFXSupportBase AddSample 114 9802
#pragma libcall SFXSupportBase RemSample 11a 801
#pragma libcall SFXSupportBase SetSlider 120 0802
#pragma libcall SFXSupportBase SetTitle 126 801
#pragma libcall SFXSupportBase SetName 12c 9802
#pragma libcall SFXSupportBase ShowSample 132 0
#pragma libcall SFXSupportBase HideSample 138 0
#pragma libcall SFXSupportBase SetOptWinBounds 13e 109804
#pragma libcall SFXSupportBase LockBuffer 144 801
#pragma libcall SFXSupportBase UnlockBuffer 14a 801
#pragma libcall SFXSupportBase InitScrollerWinStuff 150 0
#pragma libcall SFXSupportBase FreeScrollerWinStuff 156 0
#pragma libcall SFXSupportBase Transform 15c 10CBA9807
#pragma libcall SFXSupportBase AssignInterpolFunc 162 8002
#pragma libcall SFXSupportBase GetDecrunchedLength 168 1002
#pragma libcall SFXSupportBase ReadData 16e 32109806
#pragma libcall SFXSupportBase GetCrunchedLength 174 1002
#pragma libcall SFXSupportBase WriteData 17a 2109805
#pragma libcall SFXSupportBase GetFileLength 180 801
#pragma libcall SFXSupportBase GenIOTables 186 0
#pragma libcall SFXSupportBase ConvMem2Drv 18c 801
#pragma libcall SFXSupportBase ConvDrv2Mem 192 801
#pragma libcall SFXSupportBase SAInit 198 43210806
#pragma libcall SFXSupportBase SADone 19e 9802
#pragma libcall SFXSupportBase CopyData 1a4 2109805
#pragma libcall SFXSupportBase SaveModSettings 1aa 9802
#pragma libcall SFXSupportBase LoadModSettings 1b0 9802
#pragma libcall SFXSupportBase InitFXParams 1b6 801
#pragma libcall SFXSupportBase DoneFXParams 1bc 801
#pragma libcall SFXSupportBase ParseModulatorSet 1c2 A9803
#pragma libcall SFXSupportBase ParseModulatorGet 1c8 BA9804
#pragma libcall SFXSupportBase ParseInterpolatorSet 1ce A9803
#pragma libcall SFXSupportBase ParseInterpolatorGet 1d4 BA9804
#pragma libcall SFXSupportBase ParseWinFunctionSet 1da A9803
#pragma libcall SFXSupportBase ParseWinFunctionGet 1e0 BA9804
#pragma libcall SFXSupportBase ASOpen 1e6 09803
#pragma libcall SFXSupportBase ASClose 1ec 801
#pragma libcall SFXSupportBase ASRead 1f2 0802
#pragma libcall SFXSupportBase ASWrite 1f8 0802
/**/
/* V 1.2*/
/**/
#pragma libcall SFXSupportBase GetString 1fe 001
#pragma libcall SFXSupportBase NewProWin 204 0
#pragma libcall SFXSupportBase DelProWin 20a 801
#pragma libcall SFXSupportBase StartProWin 210 0A9804
#pragma libcall SFXSupportBase RecalcSampleLines 216 801
#pragma libcall SFXSupportBase CopyDataCh 21c 32109806
#pragma libcall SFXSupportBase NewRingBuf 222 A98004
#pragma libcall SFXSupportBase DelRingBuf 228 801
#pragma libcall SFXSupportBase FreeSampleMem 22e 801
#pragma libcall SFXSupportBase ASReadIRQ 234 0802
#pragma libcall SFXSupportBase TrimString 23a 801
#pragma libcall SFXSupportBase ServerDataDone 240 0
#pragma libcall SFXSupportBase FreeFXParams 246 801
/**/
/* V 1.3*/
/**/
#pragma libcall SFXSupportBase InitCfgSel 24c 9802
#pragma libcall SFXSupportBase DoneCfgSel 252 801
#pragma libcall SFXSupportBase AddCfgSel 258 43210A9808
#pragma libcall SFXSupportBase SetCfgSel 25e 0A9804
#pragma libcall SFXSupportBase HandleCfgSel 264 543210A9809
#pragma libcall SFXSupportBase NG_HandleEvents 26a A98321007
#pragma libcall SFXSupportBase SetStatus 270 801
/**/
/* V 4.1*/
/**/
#pragma libcall SFXSupportBase GetSFXVersion 276 0
#pragma libcall SFXSupportBase DrawGadWB 27c 3210805
#pragma libcall SFXSupportBase DrawGadBW 282 3210805
#pragma libcall SFXSupportBase DrawTitle 288 2109805
#pragma libcall SFXSupportBase ShowHelp 28e 801
#pragma libcall SFXSupportBase GetNodeInteractive 294 0802
#pragma libcall SFXSupportBase AddCfgSelConfig 29a 9802
#pragma libcall SFXSupportBase LogText 2a0 801
/**/
/* V 4.2*/
/**/
#pragma libcall SFXSupportBase MemUsage 2a6 801
/**/
#pragma libcall SFXSupportBase NewFFTTabs 2ac 0BA9805
#pragma libcall SFXSupportBase DelFFTTabs 2b2 BA9804
/**/
#pragma libcall SFXSupportBase NewWindowFkt 2b8 81003
#pragma libcall SFXSupportBase DelWindowFkt 2be 801
/**/
#pragma libcall SFXSupportBase ExchangeData 2c4 9802
#pragma libcall SFXSupportBase RecalcLoopPlayData 2ca 0
/**/
/* V 4.3*/
/**/
#pragma libcall SFXSupportBase JoinSampleNames 2d0 9802
/**/
#pragma libcall SFXSupportBase SetSampleComment 2d6 BA9804
/**/
#pragma libcall SFXSupportBase ADosError 2dc 2910805
#pragma libcall SFXSupportBase StdCError 2e2 2910805
/**/
