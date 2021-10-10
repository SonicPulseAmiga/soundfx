/* Warp6.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SearchBuffer "'Warp6'"
buf1=RESULT
SFX_SearchBuffer "'Warp5'"
buf2=RESULT
IF buf1>-1 AND buf2>-1 THEN DO
	SFX_SelOperator 'Amplify'
	SFX_SetOperatorParam 'Amplify' 'P1S' '0.0'
	SFX_SetOperatorParam 'Amplify' 'P1E' '1.0'
	SFX_SetOperatorParam 'Amplify' 'P1ModShape' 'User'
	SFX_SetOperatorParam 'Amplify' 'P1UserType' 'Normal'
	SFX_SetOperatorParam 'Amplify' 'P1UserMode' 'Stretch'
	SFX_SetOperatorParam 'Amplify' 'P1UserModBuf' buf2
	SFX_ProcessSample buf1
	buf3=RESULT
	IF buf3>-1 THEN DO
		SFX_RenameBuffer buf3 "'Warp7'"
		SFX_RemoveBuffer buf1
		SFX_RemoveBuffer buf2
		SFX_SetActiveBuffer buf3
		SFX_CleanUp 0
	END
END
