/* SFX MultiBandDelay */

OPTIONS RESULTS
IF (LEFT(ADDRESS(), 8) ~= "SFX_REXX") THEN DO	/* not started by SoundFX ? */
	PARSE ARG opts
	sfxport=WORD(opts,1)
	IF SHOW("Ports",sfxport) THEN DO
		ADDRESS VALUE sfxport
	END
	ELSE DO
		IF ~SHOW("Ports","SFX_REXX") THEN EXIT 10
    	ADDRESS 'SFX_REXX'
	END
END

SFX_GetActiveBuffer
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_SelOperator 'Filter-StateVariable'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P1S' '1.0'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P1ModShape' 'None'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P2S' '0.05'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P2ModShape' 'None'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P3S' '1.0'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P3ModShape' 'None'
	SFX_SetOperatorParam 'Filter-StateVariable' 'Type' 'LowPass'
	SFX_ProcessSample buf1
	IF RC=0 THEN buf2=RESULT
	ELSE buf2=-2

	SFX_SetOperatorParam 'Filter-StateVariable' 'Type' 'HighPass'
	SFX_ProcessSample buf1
	IF RC=0 THEN buf3=RESULT
	ELSE buf3=-2

	IF buf2>-1 AND buf3>-1 THEN DO
		SFX_SelOperator 'Delay'
		SFX_SetOperatorParam 'Delay' 'P1S' '0.6'
		SFX_SetOperatorParam 'Delay' 'P1ModShape' 'None'
		SFX_SetOperatorParam 'Delay' 'P2S' '0.3'
		SFX_SetOperatorParam 'Delay' 'P2ModShape' 'None'
/*		SFX_SetOperatorParam 'Delay' 'P3S' '"237.0 ms"'*/
		SFX_SetOperatorParam 'Delay' 'P3S' '0.25'
		SFX_SetOperatorParam 'Delay' 'P3ModShape' 'None'
		SFX_SetOperatorParam 'Delay' 'Ampf' '1.6'
		SFX_ProcessSample buf2
		IF RC=0 THEN buf4=RESULT
		ELSE buf4=-2

		SFX_SetOperatorParam 'Delay' 'P2S' '0.5'
		SFX_SetOperatorParam 'Delay' 'P3S' '0.0625'
		SFX_ProcessSample buf3
		IF RC=0 THEN buf5=RESULT
		ELSE buf5=-2

		IF buf4>-1 AND buf5>-1 THEN DO
			SFX_SelOperator 'Mix'
			SFX_SetOperatorParam 'Mix' 'P1S' '0.5'
			SFX_SetOperatorParam 'Mix' 'P1ModShape' 'None'
			SFX_ProcessSample buf4 buf5
			IF RC=0 THEN buf6=RESULT
			ELSE buf6=-2
		END
		ELSE SFX_Message '"Operator Delay failed !"'
	END
	ELSE SFX_Message '"Operator Filter-StateVariable failed !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
