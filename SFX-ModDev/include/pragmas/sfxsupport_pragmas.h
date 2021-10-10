/**/
/* sfxsupport.library*/
/**/
/**/
/* V 1.0*/
/**/
#pragma libcall SFXSupportBase ServerDataInit 1e 801
#pragma libcall SFXSupportBase InitProWin 24 8002
#pragma libcall SFXSupportBase RefreshProWin 2a 0802
#pragma libcall SFXSupportBase RemoveProWin 30 801
#pragma libcall SFXSupportBase MyAllocMem 36 1002
#pragma libcall SFXSupportBase MyReAllocMem 3c 210804
#pragma libcall SFXSupportBase MyFreeMem 42 0802
#pragma libcall SFXSupportBase DrawGad 48 43210806
#pragma libcall SFXSupportBase RText 4e 910804
#pragma libcall SFXSupportBase CheckGad_int 54 210A9806
#pragma libcall SFXSupportBase CheckGad_dbl 5a 210A9806
#pragma libcall SFXSupportBase Swap_wrd 60 9802
#pragma libcall SFXSupportBase Swap_int 66 9802
#pragma libcall SFXSupportBase Swap_dbl 6c 9802
#pragma libcall SFXSupportBase Message 72 198004
#pragma libcall SFXSupportBase Question 78 98003
#pragma libcall SFXSupportBase GetListEntry 7e 0
#pragma libcall SFXSupportBase GetPeriode 84 8002
#pragma libcall SFXSupportBase Rate2KeyFrq 8a 98003
#pragma libcall SFXSupportBase Key2RateFrq 90 A9803
#pragma libcall SFXSupportBase Frq2RateKey 96 A9803
#pragma libcall SFXSupportBase GetBufferPtr 9c 1002
#pragma libcall SFXSupportBase GetBufferNr a2 001
#pragma libcall SFXSupportBase RefreshMod a8 543210A9809
#pragma libcall SFXSupportBase SetRngMode ae 801
#pragma libcall SFXSupportBase SetRngs b4 0A9804
#pragma libcall SFXSupportBase SetValidBuffer ba 801
#pragma libcall SFXSupportBase RecalcTraceData c0 801
#pragma libcall SFXSupportBase ZeroSearch c6 3210805
#pragma libcall SFXSupportBase PeakSearch cc 210804
#pragma libcall SFXSupportBase SetNewRate d2 0
#pragma libcall SFXSupportBase AddModulator d8 3210A9807
#pragma libcall SFXSupportBase SetModulator de 0A9804
#pragma libcall SFXSupportBase HandleModulator e4 3210CBA9809
#pragma libcall SFXSupportBase HandleParameter ea 0A9804
#pragma libcall SFXSupportBase AddSource f0 3210A9807
#pragma libcall SFXSupportBase SetSource f6 0A9804
#pragma libcall SFXSupportBase HandleSource fc 3210A9807
#pragma libcall SFXSupportBase AddInterpolator 102 3210A9807
#pragma libcall SFXSupportBase SetInterpolator 108 0A9804
#pragma libcall SFXSupportBase HandleInterpolator 10e 3210A9807
#pragma libcall SFXSupportBase AddWinFunction 114 3210A9807
#pragma libcall SFXSupportBase SetWinFunction 11a 0A9804
#pragma libcall SFXSupportBase HandleWinFunction 120 3210A9807
#pragma libcall SFXSupportBase AssignBShape 126 801
#pragma libcall SFXSupportBase FilterString 12c 801
#pragma libcall SFXSupportBase StripString 132 801
#pragma libcall SFXSupportBase StringCopy 138 801
/*StringCopy2(src)(a0)*/
#pragma libcall SFXSupportBase OpenMWindow 13e 0A9804
#pragma libcall SFXSupportBase CloseMWindow 144 9802
#pragma libcall SFXSupportBase BufferFill 14a 9802
#pragma libcall SFXSupportBase NewSample 150 0
#pragma libcall SFXSupportBase AllocSampleMem 156 10803
#pragma libcall SFXSupportBase AddSample 15c 9802
#pragma libcall SFXSupportBase RemSample 162 801
#pragma libcall SFXSupportBase SetSlider 168 0802
#pragma libcall SFXSupportBase SetTitle 16e 801
#pragma libcall SFXSupportBase SetName 174 9802
#pragma libcall SFXSupportBase ShowSample 17a 0
#pragma libcall SFXSupportBase HideSample 180 0
#pragma libcall SFXSupportBase SetOptWinBounds 186 109804
#pragma libcall SFXSupportBase LockBuffer 18c 801
#pragma libcall SFXSupportBase UnlockBuffer 192 801
#pragma libcall SFXSupportBase InitScrollerWinStuff 198 0
#pragma libcall SFXSupportBase FreeScrollerWinStuff 19e 0
#pragma libcall SFXSupportBase Help 1a4 10803
#pragma libcall SFXSupportBase Transform 1aa 10CBA9807
#pragma libcall SFXSupportBase InitFFT 1b0 0BA9805
#pragma libcall SFXSupportBase GenWindow 1b6 981004
#pragma libcall SFXSupportBase AssignInterpolFunc 1bc 8002
#pragma libcall SFXSupportBase GetDecrunchedLength 1c2 1002
#pragma libcall SFXSupportBase ReadData 1c8 32109806
/*ReadChunkHeader(type,size,infile)(a0/a1/a2)*/
#pragma libcall SFXSupportBase GetCrunchedLength 1ce 1002
#pragma libcall SFXSupportBase WriteData 1d4 2109805
/*WriteChunkHeader(type,size,outfile)(d0/d1,a0)*/
#pragma libcall SFXSupportBase GetFileLength 1da 801
#pragma libcall SFXSupportBase GenIOTables 1e0 0
#pragma libcall SFXSupportBase ConvMem2Drv 1e6 801
#pragma libcall SFXSupportBase ConvDrv2Mem 1ec 801
#pragma libcall SFXSupportBase SAInit 1f2 43210806
#pragma libcall SFXSupportBase SADone 1f8 9802
#pragma libcall SFXSupportBase CopyData 1fe 2109805
#pragma libcall SFXSupportBase BuildList 204 A98004
#pragma libcall SFXSupportBase SortAList 20a 8002
#pragma libcall SFXSupportBase match_nr2ix 210 81003
#pragma libcall SFXSupportBase match_name2nr 216 98003
#pragma libcall SFXSupportBase SaveModSettings 21c 9802
#pragma libcall SFXSupportBase LoadModSettings 222 9802
#pragma libcall SFXSupportBase InitFXParams 228 801
#pragma libcall SFXSupportBase DoneFXParams 22e 801
#pragma libcall SFXSupportBase ParseModulatorSet 234 A9803
#pragma libcall SFXSupportBase ParseModulatorGet 23a BA9804
#pragma libcall SFXSupportBase ParseInterpolatorSet 240 A9803
#pragma libcall SFXSupportBase ParseInterpolatorGet 246 BA9804
#pragma libcall SFXSupportBase ParseWinFunctionSet 24c A9803
#pragma libcall SFXSupportBase ParseWinFunctionGet 252 BA9804
#pragma libcall SFXSupportBase ASOpen 258 09803
#pragma libcall SFXSupportBase ASClose 25e 801
#pragma libcall SFXSupportBase ASRead 264 0802
#pragma libcall SFXSupportBase ASWrite 26a 0802
/**/
/* V 1.2*/
/**/
#pragma libcall SFXSupportBase GetString 270 001
#pragma libcall SFXSupportBase NewProWin 276 0
#pragma libcall SFXSupportBase DelProWin 27c 801
#pragma libcall SFXSupportBase StartProWin 282 0A9804
#pragma libcall SFXSupportBase RecalcSampleLines 288 801
#pragma libcall SFXSupportBase CopyDataCh 28e 32109806
#pragma libcall SFXSupportBase NewRingBuf 294 A98004
#pragma libcall SFXSupportBase DelRingBuf 29a 0802
#pragma libcall SFXSupportBase FreeSampleMem 2a0 801
#pragma libcall SFXSupportBase ASReadIRQ 2a6 0802
#pragma libcall SFXSupportBase TrimString 2ac 801
#pragma libcall SFXSupportBase ServerDataDone 2b2 0
#pragma libcall SFXSupportBase FreeFXParams 2b8 801
/**/
/* V 1.3*/
/**/
#pragma libcall SFXSupportBase InitCfgSel 2be 9802
#pragma libcall SFXSupportBase DoneCfgSel 2c4 801
#pragma libcall SFXSupportBase AddCfgSel 2ca 43210A9808
#pragma libcall SFXSupportBase SetCfgSel 2d0 0A9804
#pragma libcall SFXSupportBase HandleCfgSel 2d6 543210A9809
#pragma libcall SFXSupportBase NG_HandleEvents 2dc A98321007
#pragma libcall SFXSupportBase SetStatus 2e2 801
/**/
/* V 4.1*/
/**/
#pragma libcall SFXSupportBase GetSFXVersion 2e8 0
#pragma libcall SFXSupportBase DrawGadWB 2ee 3210805
#pragma libcall SFXSupportBase DrawGadBW 2f4 3210805
#pragma libcall SFXSupportBase DrawTitle 2fa 2109805
#pragma libcall SFXSupportBase ShowHelp 300 801
#pragma libcall SFXSupportBase GetNodeInteractive 306 0802
#pragma libcall SFXSupportBase AddCfgSelConfig 30c 9802
/**/
/**/
/**/
