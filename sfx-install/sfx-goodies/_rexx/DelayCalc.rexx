/*
   $VER: DelayCalc.rexx 1.0 for SoundFX (14.8.2002)
   © by Dan Jedlicka

   Opens a Delay Calculator on SoundFX screen.

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

SFX_GetProgDir
path=RESULT

ADDRESS COMMAND path||'/_rexx/DelayCalc_EXE'
