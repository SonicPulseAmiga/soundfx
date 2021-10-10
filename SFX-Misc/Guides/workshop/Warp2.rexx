/* Warp2.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SearchBuffer "'Warp1'"
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_SelOperator 'ChorusPhaser'
	SFX_ProcessSample buf1
	buf2=RESULT
	IF buf2>-1 THEN DO
		SFX_RenameBuffer buf2 "'Warp2'"
		SFX_RemoveBuffer buf1
		SFX_SetActiveBuffer buf2
		SFX_CleanUp 0
	END
END
