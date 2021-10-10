/* SFX DeCrackleTest */

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
	SFX_GetLength buf1
	slen=RESULT
	SFX_GetRate buf1
	srat=RESULT

	fc1=0.95
	fc2=1.0-fc1

	SFX_NewBuffer slen srat 4
	buf2=RESULT
	IF buf2>-1 THEN DO
		brk=0
		SFX_InitProWin slen '"Working on DeCrackleTest[0]"'
		pwi=RESULT
		IF pwi>-1 THEN DO
			SFX_GetSampleValue buf1 0 0
			oldval=RESULT
			ampenv=oldval
			difenv=oldval
			DO pos=1 TO slen
				SFX_RefrProWin pwi pos
				IF RESULT=0 THEN DO
					SFX_GetSampleValue buf1 0 pos
					newval=RESULT
					amp=abs(newval)
					dif=abs(newval-oldval)
					ampenv=(fc1*ampenv)+(fc2*amp)
					difenv=(fc1*difenv)+(fc2*dif)
					SFX_PutSampleValue buf2 0 pos amp
					SFX_PutSampleValue buf2 1 pos ampenv
					SFX_PutSampleValue buf2 2 pos dif
					SFX_PutSampleValue buf2 3 pos difenv
					oldval=newval
				END
				ELSE DO
					brk=1
					LEAVE pos
				END
			END
			SFX_RemoveProWin pwi
		END
		ELSE SFX_Message '"Can`t open ProceedWindow !"'
		SFX_RedrawBuffer buf2
	END
	ELSE SFX_Message '"Can`t add samplebuffer !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
