/* SnareDrum5.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SearchBuffer "'SnareDrum4'"
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_SelOperator 'Amplify'
	SFX_SetOperatorParam 'Amplify' 'P1S' '0.0'
	SFX_SetOperatorParam 'Amplify' 'P1E' '1.0'
	SFX_SetOperatorParam 'Amplify' 'P1ModShape' 'Vector'
	SFX_SetOperatorParam 'Amplify' 'P1VectorAnz' '5'
	SFX_SetOperatorParam 'Amplify' 'P1VectorPos' '0' '0.0'
	SFX_SetOperatorParam 'Amplify' 'P1VectorLev' '0' '0.0'
	SFX_SetOperatorParam 'Amplify' 'P1VectorPos' '1' '0.02'
	SFX_SetOperatorParam 'Amplify' 'P1VectorLev' '1' '1.0'
	SFX_SetOperatorParam 'Amplify' 'P1VectorPos' '2' '0.15'
	SFX_SetOperatorParam 'Amplify' 'P1VectorLev' '2' '1.0'
	SFX_SetOperatorParam 'Amplify' 'P1VectorPos' '3' '0.2'
	SFX_SetOperatorParam 'Amplify' 'P1VectorLev' '3' '0.3'
	SFX_SetOperatorParam 'Amplify' 'P1VectorPos' '4' '1.0'
	SFX_SetOperatorParam 'Amplify' 'P1VectorLev' '4' '0.0'
	SFX_ProcessSample buf1
	buf2=RESULT
	IF buf2>-1 THEN DO
		SFX_RenameBuffer buf2 "'SnareDrum-Version1'"
		SFX_RemoveBuffer buf1
		SFX_SetActiveBuffer buf2
		SFX_CleanUp 0
	END
END
