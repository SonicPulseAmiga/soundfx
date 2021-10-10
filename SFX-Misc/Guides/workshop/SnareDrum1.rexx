/* SnareDrum1.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SelOperator 'Noise'
SFX_SetOperatorParam 'Noise' 'Length' "'0.5 s'"
SFX_ProcessSample
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_RenameBuffer buf1 "'SnareDrum1'"
END
