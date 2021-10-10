/* BaseDrum1.rexx */

OPTIONS RESULTS
ADDRESS REXX_SFX

SFX_SelOperator 'Synthesize-Add'
SFX_SetOperatorParam 'Synthesize-Add' 'Length' "'0.5 s'"
SFX_SetOperatorParam 'Synthesize-Add' 'Pitch' "'130.8127 Hz'"
SFX_SetOperatorParam 'Synthesize-Add' 'P1S' "'3.0'"
SFX_SetOperatorParam 'Synthesize-Add' 'P1E' "'0.0001'"
SFX_SetOperatorParam 'Synthesize-Add' 'P1ModShape' 'Curve'
SFX_SetOperatorParam 'Synthesize-Add' 'P1CurveExp' "'0.1'"
SFX_SetOperatorParam 'Synthesize-Add' 'P2S' "'0.0'"
SFX_SetOperatorParam 'Synthesize_Add' 'P2E' "'1.0'"
SFX_SetOperatorParam 'Synthesize-Add' 'P2ModShape' 'Vector'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorAnz' '5'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorPos' '0' '0.0'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorLev' '0' '0.0'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorPos' '1' '0.02'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorLev' '1' '1.0'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorPos' '2' '0.15'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorLev' '2' '1.0'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorPos' '3' '0.2'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorLev' '3' '0.3'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorPos' '4' '1.0'
SFX_SetOperatorParam 'Synthesize-Add' 'P2VectorLev' '4' '0.0'
SFX_ProcessSample
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_RenameBuffer buf1 "'BaseDrum1'"
END
