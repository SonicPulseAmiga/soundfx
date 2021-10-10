/* SFX ZoomLoopStart */

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
	SFX_GetLoopMode buf1
	if RESULT=1 THEN DO
		SFX_GetLength buf1
		slen=RESULT
		SFX_GetLoopStart buf1
		ls=RESULT
		zs=ls-50
		IF zs<0 THEN zs=0
		zl=100
		IF (zs+zl)>=slen THEN zl=slen-zs
		SFX_SetZoomXStart buf1 zs
		SFX_SetZoomXLength buf1 zl
	END
	ELSE SFX_Message '"No loop is set !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
