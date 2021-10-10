/* BaseDrum3.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SelOperator 'Synthesize-Add'
SFX_SetOperatorParam 'Synthesize-Add' 'Length' "'0.5 s'"
SFX_SetOperatorParam 'Synthesize-Add' 'Pitch' "'130.8127 Hz'"
SFX_SetOperatorParam 'Synthesize-Add' 'P1S' "'1.0'"
SFX_SetOperatorParam 'Synthesize-Add' 'P1E' "'0.1'"
SFX_SetOperatorParam 'Synthesize-Add' 'P1ModShape' 'Curve'
SFX_SetOperatorParam 'Synthesize-Add' 'P1CurveExp' '0.5'
SFX_SetOperatorParam 'Synthesize-Add' 'P2S' "'1.0'"
SFX_SetOperatorParam 'Synthesize_Add' 'P2E' "'1.0'"
SFX_SetOperatorParam 'Synthesize-Add' 'P2ModShape' 'None'
SFX_ProcessSample
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_RenameBuffer buf1 "'SnareDrum3'"
END
