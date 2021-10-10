/* HiHat3.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SearchBuffer "'HiHat2'"
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_SelOperator 'Amplify'
	SFX_SetOperatorParam 'Amplify' 'P1S' '1.0'
	SFX_SetOperatorParam 'Amplify' 'P1E' '0.0'
	SFX_SetOperatorParam 'Amplify' 'P1ModShape' 'Curve'
	SFX_SetOperatorParam 'Amplify' 'P1CurveExp' '0.5'
	SFX_ProcessSample buf1
	buf2=RESULT
	IF buf2>-1 THEN DO
		SFX_RenameBuffer buf2 "'HiHat-Version1'"
		SFX_RemoveBuffer buf1
		SFX_SetActiveBuffer buf2
		SFX_CleanUp 0
	END
END
