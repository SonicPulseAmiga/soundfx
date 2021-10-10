/* SFX WideStereo */

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
	IF cha=1 THEN DO
		SFX_SelOperator 'ConvertChannels'
		SFX_SetOperatorPreset 'ConvertChannels' 'MonoToWideStereo'
		SFX_ProcessSample buf1
		IF RC=0 THEN buf1=RESULT
		ELSE buf1=-2
		IF buf1>-1 THEN DO
			SFX_RedrawBuffer buf1
			SFX_SetActiveBuffer buf1
			SFX_CleanUp 0
		END
		ELSE SFX_Message '"Operator ConvertChannels failed !"'
	END
	ELSE SFX_Message '"Mono source required !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
