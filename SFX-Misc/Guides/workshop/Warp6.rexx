/* Warp6.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SearchBuffer "'Warp2'"
buf1=RESULT
SFX_SearchBuffer "'Warp5'"
buf2=RESULT
IF buf1>-1 AND buf2>-1 THEN DO
	SFX_SelOperator 'Detune'
	SFX_SetOperatorParam 'Detune' 'P1S' '0.1'
	SFX_SetOperatorParam 'Detune' 'P1E' '2.0'
	SFX_SetOperatorParam 'Detune' 'P1ModShape' 'User'
	SFX_SetOperatorParam 'Detune' 'P1UserType' 'Normal'
	SFX_SetOperatorParam 'Detune' 'P1UserMode' 'Stretch'
	SFX_SetOperatorParam 'Detune' 'P1UserModBuf' buf2
	SFX_ProcessSample buf1
	buf3=RESULT
	IF buf3>-1 THEN DO
		SFX_RenameBuffer buf3 "'Warp6'"
		SFX_RemoveBuffer buf1
		SFX_SetActiveBuffer buf3
		SFX_CleanUp 0
	END
END
