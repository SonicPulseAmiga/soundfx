/* test mpeg-saver */

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

SFX_GetBufferName buf1
basename=RESULT

SFX_SelSaver 'MPEG'
SFX_SetSaverParam 'MPEG' 'Bitrate' '"128"'
SFX_SetSaverParam 'MPEG' 'Engine' '"8Hz mp3"'

SFX_SaveSample 'ram:8Hz-128.MP3'

SFX_RenameBuffer buf1 basename
EXIT
