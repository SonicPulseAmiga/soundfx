/* Warp1.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SelOperator 'Noise'
SFX_SetOperatorParam 'Noise' 'Length' "'4.0 s'"
SFX_ProcessSample
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_RenameBuffer buf1 "'Warp1'"
END
