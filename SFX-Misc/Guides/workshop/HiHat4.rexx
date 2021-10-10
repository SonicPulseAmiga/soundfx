/* HiHat4.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SearchBuffer "'HiHat-Version1'"
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_GetRate buf1
	srat=RESULT
	SFX_SelOperator 'Resample'
	SFX_SetOperatorParam 'Resample' 'Factor' '0.5'
	SFX_SetOperatorParam 'Resample' 'I1IntType' 'Lin'
	SFX_ProcessSample buf1
	buf2=RESULT
	IF buf2>-1 THEN DO
		SFX_RenameBuffer buf2 "'HiHat-Version2'"
		SFX_SetRate buf2 srat
		SFX_SetActiveBuffer buf2
	END
END
