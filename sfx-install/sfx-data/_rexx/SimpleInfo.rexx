/* SFX Info */

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
	SFX_GetBufferName buf1
	name=RESULT
	SFX_GetLength buf1
	slen=RESULT
	SFX_GetRate buf1
	srat=RESULT
	SFX_GetChannels buf1
	scha=RESULT

	info='"sampleparameter for active buffer ["' || name || '"]:"' || D2C(10)
	info=info || '"  samplerate     : "' || srat || D2C(10)
	info=info || '"  samplelength   : "' || slen || D2C(10)
	info=info || '"  nr of channels : "' || scha
	SFX_Message info
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
