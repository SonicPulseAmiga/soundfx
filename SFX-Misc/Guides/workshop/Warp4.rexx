/* Warp4.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SearchBuffer "'Warp3'"
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_SelOperator 'Slide'
	SFX_SetOperatorParam 'Slide' 'P1S' "'0 lv'"
	SFX_SetOperatorParam 'Slide' 'P1E' "'-25000 lv'"
	SFX_SetOperatorParam 'Slide' 'P1ModShape' 'Curve'
	SFX_SetOperatorParam 'Slide' 'P1CurveExp' '1.0'
	SFX_ProcessSample buf1
	buf2=RESULT
	IF buf2>-1 THEN DO
		SFX_RenameBuffer buf2 "'Warp4'"
		SFX_RemoveBuffer buf1
		SFX_SetActiveBuffer buf2
		SFX_CleanUp 0
	END
END
