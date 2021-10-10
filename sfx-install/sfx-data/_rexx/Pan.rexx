/* SFX Pan */

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
		SFX_SelOperator 'ConvertChannels'					/* make Stereo */
		SFX_SetOperatorParam 'ConvertChannels' 'Dest' '1'
		SFX_ProcessSample buf1
		IF RC=0 THEN buf1=RESULT
		ELSE buf1=-2
		buf2=-1
		IF buf1>-1 THEN DO							/* RampUp first channel */
			SFX_SelOperator 'Amplify'
			SFX_SetOperatorParam 'Amplify' 'Wrap' '0'
			SFX_SetOperatorParam 'Amplify' 'P1ModShape' 'Linear'
			SFX_DisableChannel buf1 1
			SFX_SetOperatorParam 'Amplify' 'AmpfS' '0.0'
			SFX_SetOperatorParam 'Amplify' 'AmpfE' '1.0'
			SFX_ProcessSample buf1
			IF RC=0 THEN buf2=RESULT
			ELSE buf2=-2
			SFX_RemoveBuffer buf1
		END
		ELSE SFX_Message '"Operator ConvertChannels failed !"'
		buf1=-1
		IF buf2>-1 THEN DO							/* RampDown second channel */
			SFX_EnableChannel buf2 1
			SFX_DisableChannel buf2 0
			SFX_SetOperatorParam 'Amplify' 'AmpfS' '1.0'
			SFX_SetOperatorParam 'Amplify' 'AmpfE' '0.0'
			SFX_ProcessSample buf2
			IF RC=0 THEN buf1=RESULT
			ELSE buf1=-2
			SFX_RemoveBuffer buf2
		END
		ELSE Message '"Operator Amplify failed !"'
		IF buf1>-1 THEN DO
			SFX_EnableChannel buf1 0
			SFX_RedrawBuffer buf1
			SFX_SetActiveBuffer buf1
			SFX_CleanUp 0
		END
		ELSE SFX_Message '"Operator Amplify failed !"'
	END
	ELSE SFX_Message '"Mono source required !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
