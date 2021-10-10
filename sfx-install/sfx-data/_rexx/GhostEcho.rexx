/* SFX GhostEcho */


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
	SFX_SelOperator 'Reverse'
	SFX_SetOperatorParam 'Reverse' 'P1S' '1.0'
	SFX_SetOperatorParam 'Reverse' 'P1ModShape' 'None'
	SFX_ProcessSample buf1
	IF RC=0 THEN buf1=RESULT
	ELSE buf1=-2
	IF buf1>-1 THEN DO
		SFX_GetLength buf1
		len=RESULT
		SFX_GetRate buf1
		rate=RESULT
		len2=len+(rate/2)
		SFX_SetLength buf1 len2
		SFX_SelOperator 'MultiDelay'
		SFX_ProcessSample buf1
		IF RC=0 THEN buf2=RESULT
		ELSE buf2=-2
		SFX_RemoveBuffer buf1
		IF buf2>-1 THEN DO
			SFX_SelOperator 'Reverse'
			SFX_ProcessSample buf2
			IF RC=0 THEN buf1=RESULT
			ELSE buf1=-1
			SFX_RemoveBuffer buf2
			IF buf1>-1 THEN DO
				SFX_RedrawBuffer buf1
				SFX_SetActiveBuffer buf1
				SFX_CleanUp 0
			END
			ELSE SFX_Message '"Operator Reverse failed !"'
		END
		ELSE SFX_Message '"Operator MultiDelay failed !"'
	END
	ELSE SFX_Message '"Operator Reverse failed !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate

EXIT
