/* SFX RemQuantNoise */

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

SFX_SelOperator 'Resample'
SFX_SetOperatorParam 'Resample' 'I1IntType' 'Linear'

buf1=-1
SFX_GetActiveBuffer
buf2=RESULT
IF buf2>-1 THEN DO
	SFX_SetOperatorParam 'Resample' 'Factor' '0.5'
	SFX_ProcessSample buf2
	IF RC=0 THEN buf1=RESULT
	ELSE buf1=-1
END
ELSE SFX_Message '"No sample to operate on !"'
buf2=-1
IF buf1>-1 THEN DO
	SFX_SetOperatorParam 'Resample' 'Factor' '2.0'
	SFX_ProcessSample buf1
	IF RC=0 THEN buf2=RESULT
	ELSE buf2=-1
	SFX_RemoveBuffer buf1
END
ELSE SFX_Message '"Operator Resample failed !"'
buf1=-1
IF buf2>-1 THEN DO
	SFX_SetOperatorParam 'Resample' 'Factor' '0.5'
	SFX_ProcessSample buf2
	IF RC=0 THEN buf1=RESULT
	ELSE buf1=-1
	SFX_RemoveBuffer buf2
END
ELSE SFX_Message '"Operator Resample failed !"'
buf2=-1
IF buf1>-1 THEN DO
	SFX_SetOperatorParam 'Resample' 'Factor' '2.0'
	SFX_ProcessSample buf1
	IF RC=0 THEN buf2=RESULT
	ELSE buf2=-1
	SFX_RemoveBuffer buf1
END
ELSE SFX_Message '"Operator Resample failed !"'
IF buf2>-1 THEN DO
	SFX_SetActiveBuffer buf2
	SFX_CleanUp 0
END
SFX_Activate
EXIT
