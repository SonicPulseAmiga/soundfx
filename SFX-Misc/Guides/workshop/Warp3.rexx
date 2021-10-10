/* Warp3.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SelOperator 'Synthesize-Add'
SFX_SetOperatorParam 'Synthesize-Add' 'Length' '42'
SFX_SetOperatorParam 'Synthesize-Add' 'SVal' '0' '1.0'
SFX_ProcessSample
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_RenameBuffer buf1 "'Warp3'"
END
