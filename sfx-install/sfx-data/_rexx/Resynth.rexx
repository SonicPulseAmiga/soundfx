/****h* SoundFX/Resynth.rexx [4.00] *
*  NAME
*    Resynth.rexx
*  COPYRIGHT
*    $VER: Resynth.rexx 4.00 (22.02.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    synthesizes (sub)bass-waves and filtered noise modulated by the current
*    sample 
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    22.Feb.2000
*  MODIFICATION HISTORY
*    22.Feb.2000	V 4.00	most recent version
*    22.Feb.2000	V 1.00	initial version
*  NOTES
*
*******
*/

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
sbuf=RESULT
IF sbuf>-1 THEN DO
	SFX_GetLength sbuf
	slen=RESULT
	slenstr='"'||slen||' sv"'
	SFX_GetRate sbuf
	srat=RESULT
	sratstr='"'||srat||' Hz"'
	SFX_GetChannels sbuf
	scha=RESULT

	SFX_SelOperator 'Filter-StateVariable'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P1S' '1.0'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P1ModShape' 'None'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P2S' '0.1'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P2ModShape' 'None'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P3S' '1.0'
	SFX_SetOperatorParam 'Filter-StateVariable' 'P3ModShape' 'None'
	SFX_SetOperatorParam 'Filter-StateVariable' 'Type' 'LowPass'
	SFX_ProcessSample sbuf
	IF RC=0 THEN tbuf1=RESULT
	ELSE tbuf1=-2

	SFX_SetOperatorParam 'Filter-StateVariable' 'Type' 'HighPass'
	SFX_ProcessSample sbuf
	IF RC=0 THEN tbuf2=RESULT
	ELSE tbuf2=-2

	IF tbuf1>-1 AND tbuf2>-1 THEN DO
		SFX_SelOperator 'Noise'
		SFX_SetOperatorParam 'Noise' 'P1S' '0.0'
		SFX_SetOperatorParam 'Noise' 'P1E' '0.0'
		SFX_SetOperatorParam 'Noise' 'P1ModShape' 'None'
		SFX_SetOperatorParam 'Noise' 'P2S' '1.0'
		SFX_SetOperatorParam 'Noise' 'P2S' '0.1'
		SFX_SetOperatorParam 'Noise' 'P2ModShape' 'User'
		SFX_SetOperatorParam 'Noise' 'P2UserModBuf' tbuf1
		SFX_SetOperatorParam 'Noise' 'P2UserMode' 'Repeat'
		SFX_SetOperatorParam 'Noise' 'P2UserType' 'AmpEnv'
		SFX_SetOperatorParam 'Noise' 'P2UserAmpEnv' 'MaxDecay'
		SFX_SetOperatorParam 'Noise' 'Rate' sratstr
		SFX_SetOperatorParam 'Noise' 'Length' slenstr
		SFX_ProcessSample
		IF RC=0 THEN tmpbuf1=RESULT
		ELSE tmpbuf1=-2

		SFX_SelOperator 'Filter-StateVariable'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P1S' '1.0'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P1ModShape' 'None'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P2S' '0.0'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P2E' '1.0'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P2ModShape' 'User'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P2UserModBuf' tbuf2
		SFX_SetOperatorParam 'Filter-StateVariable' 'P2UserMode' 'Repeat'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P2UserType' 'AmpEnv'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P2UserAmpEnv' 'MaxDecay'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P3S' '1.0'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P3E' '25.0'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P3ModShape' 'User'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P3UserModBuf' tbuf2
		SFX_SetOperatorParam 'Filter-StateVariable' 'P3UserMode' 'Repeat'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P3UserType' 'AmpEnv'
		SFX_SetOperatorParam 'Filter-StateVariable' 'P3UserAmpEnv' 'MaxDecay'
		SFX_SetOperatorParam 'Filter-StateVariable' 'Type' 'LowPass'
		SFX_SetOperatorParam 'Filter-StateVariable' 'Ampf' '0.8'
		SFX_ProcessSample tmpbuf1
		IF RC=0 THEN tmpbuf2=RESULT
		ELSE tmpbuf2=-2

		SFX_SelOperator 'Amplify'
		SFX_SetOperatorParam 'Amplify' 'P1S' '0.0'
		SFX_SetOperatorParam 'Amplify' 'P1E' '1.0'
		SFX_SetOperatorParam 'Amplify' 'P1ModShape' 'User'
		SFX_SetOperatorParam 'Amplify' 'P1UserModBuf' tbuf2
		SFX_SetOperatorParam 'Amplify' 'P1UserMode' 'Repeat'
		SFX_SetOperatorParam 'Amplify' 'P1UserType' 'AmpEnv'
		SFX_SetOperatorParam 'Amplify' 'P1UserAmpEnv' 'MaxDecay'
		SFX_SetOperatorParam 'Amplify' 'MaxVol' '0'
		SFX_ProcessSample tmpbuf2
		IF RC=0 THEN dbuf1=RESULT
		ELSE dbuf1=-2

		SFX_SelOperator 'Synthesize-Add'
		SFX_SetOperatorPreset 'Synthesize-Add' 'SimpleSaw'
		SFX_SetOperatorParam 'Synthesize-Add' 'P2S' '0.0'
		SFX_SetOperatorParam 'Synthesize-Add' 'P2E' '1.0'
		SFX_SetOperatorParam 'Synthesize-Add' 'P2ModShape' 'User'
		SFX_SetOperatorParam 'Synthesize-Add' 'P2UserModBuf' tbuf1
		SFX_SetOperatorParam 'Synthesize-Add' 'P2UserMode' 'Repeat'
		SFX_SetOperatorParam 'Synthesize-Add' 'P2UserType' 'AmpEnv'
		SFX_SetOperatorParam 'Synthesize-Add' 'P2UserAmpEnv' 'MaxDecay'
		SFX_SetOperatorParam 'Synthesize-Add' 'Rate' sratstr
		SFX_SetOperatorParam 'Synthesize-Add' 'Length' slenstr
		SFX_SetOperatorParam 'Synthesize-Add' 'Pitch' '"65.406391 Hz"'
		SFX_ProcessSample
		IF RC=0 THEN dbuf2=RESULT
		ELSE dbuf2=-2

		IF dbuf1>-1 AND dbuf2>-1 THEN DO
			SFX_SelOperator 'Mix'
			SFX_SetOperatorParam 'Mix' 'P1S' '0.3'
			SFX_SetOperatorParam 'Mix' 'P1ModShape' 'None'
			SFX_ProcessSample dbuf1 dbuf2
			IF RC=0 THEN dbuf=RESULT
			ELSE dbuf=-2
		END
		ELSE SFX_Message '"Operators Noise, Filter-StateVariable,Amplify or Synthesize-Add failed !"'
	END
	ELSE SFX_Message '"Operator Filter-StateVariable failed !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
