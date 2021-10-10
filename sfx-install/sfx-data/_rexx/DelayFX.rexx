/* SFX DelayFX */

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
	SFX_GetChannels buf1
	cha=RESULT
	IF cha=2 THEN DO
		SFX_SelOperator 'Delay'
		SFX_SetOperatorParam 'Delay' 'P1ModShape' 'Curve'
		SFX_SetOperatorParam 'Delay' 'P1CurveExp' '1.0'
		SFX_SetOperatorParam 'Delay' 'P2S' '0.4'
		SFX_SetOperatorParam 'Delay' 'P2ModShape' 'None'
		SFX_SetOperatorParam 'Delay' 'P3S' '0.0625'
		SFX_SetOperatorParam 'Delay' 'P3ModShape' 'None'
		SFX_SetOperatorParam 'Delay' 'Ampf' '1.25'
		SFX_DisableChannel buf1 1
		SFX_SetOperatorParam 'Delay' 'P1S' '0.0'
		SFX_SetOperatorParam 'Delay' 'P1E' '0.5'
		SFX_ProcessSample buf1
		IF RC=0 THEN buf1=RESULT
		ELSE buf1=-2
		buf2=-1
		IF buf1>-1 THEN DO
			SFX_EnableChannel buf1 1
			SFX_DisableChannel buf1 0
			SFX_SetOperatorParam 'Delay' 'P1S' '0.5'
			SFX_SetOperatorParam 'Delay' 'P1E' '0.0'
			SFX_ProcessSample buf1
			IF RC=0 THEN buf2=RESULT
			ELSE buf2=-2
			SFX_RemoveBuffer buf1
		END
		ELSE SFX_Message '"Operator Delay failed !"'
		IF buf2>-1 THEN DO
			SFX_EnableChannel buf2 0
			SFX_RedrawBuffer buf2
			SFX_SetActiveBuffer buf2
			SFX_CleanUp 0
		END
		ELSE SFX_Message '"Operator Delay failed !"'
	END
	ELSE SFX_Message '"Stereo source required !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
