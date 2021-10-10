/* BaseDrum3.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SearchBuffer "'BaseDrum1'"
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_SelOperator 'Amplify'
	SFX_SetOperatorParam 'Amplify' 'Wrap' '3'
	SFX_SetOperatorParam 'Amplify' 'P1S' '2.0'
	SFX_SetOperatorParam 'Amplify' 'P1E' '1.0'
	SFX_SetOperatorParam 'Amplify' 'P1ModShape' 'Curve'
	SFX_SetOperatorParam 'Amplify' 'P1CurveExp' '1.0'
	SFX_ProcessSample buf1
	buf2=RESULT
	IF buf2>-1 THEN DO
		SFX_RenameBuffer buf2 "'BaseDrum2'"
		SFX_SetActiveBuffer buf2
	END
END
