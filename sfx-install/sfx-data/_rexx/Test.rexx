/* SFX External Operator Test */

SIGNAL ON SYNTAX
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

SFX_SetQuietMode 1
old_quiet=RESULT

SFX_GetActiveBuffer
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_SelOperator 'Amplify'
	SFX_SetOperatorParam 'Amplify' 'P1S' '"100 %"'
	SFX_SetOperatorParam 'Amplify' 'P1E' '"0 %"'
	SFX_SetOperatorParam 'Amplify' 'P1ModShape' 'Curve'
	SFX_SetOperatorParam 'Amplify' 'P1CurveExp' '1.0'
	SFX_ProcessSample buf1
	IF RC=0 THEN DO
		buf2=RESULT
		SFX_RenameBuffer buf2 '"Amplified sample"'
		SFX_ShowBuffer buf2
	END
	ELSE SFX_Message '"Operator Amplify failed !"'
END
ELSE SFX_Message '"No sample to operate on !"'
/*SFX_Activate*/												/* fertig */

SFX_SetQuietMode old_quiet

EXIT

SYNTAX:
	SAY "Error #"||RC||":" ERRORTEXT(RC) "in line "||SIGL
EXIT
