/* SnareDrum4.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SearchBuffer "'SnareDrum2'"
buf1=RESULT
SFX_SearchBuffer "'SnareDrum3'"
buf2=RESULT
IF buf1>-1 AND buf2>-1 THEN DO
	SFX_SelOperator 'Slide'
	SFX_SetOperatorParam 'Slide' 'Wrap' '1'
	SFX_SetOperatorParam 'Slide' 'P1S' "'-0.6'"
	SFX_SetOperatorParam 'Slide' 'P1E' "' 0.6'"
	SFX_SetOperatorParam 'Slide' 'P1ModShape' 'User'
	SFX_SetOperatorParam 'Slide' 'P1UserType' 'normal'
	SFX_SetOperatorParam 'Slide' 'P1UserModBuf' buf2
	SFX_ProcessSample buf1
	buf3=RESULT
	IF buf3>-1 THEN DO
		SFX_RenameBuffer buf3 "'SnareDrum4'"
		SFX_RemoveBuffer buf1
		SFX_RemoveBuffer buf2
		SFX_SetActiveBuffer buf3
		SFX_CleanUp 0
	END
END
