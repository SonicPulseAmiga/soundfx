/* SFX FrqEnvTest */

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

	lastpos=0;
	energy=0;
	curlen=100;
	mins=0;

	SFX_NewBuffer slen srat 4
	buf2=RESULT
	IF buf2>-1 THEN DO
		brk=0
		SFX_InitProWin slen '"Working on FrqEnv[0]"'
		pwi=RESULT
		IF pwi>-1 THEN DO
			SFX_GetSampleValue buf1 0 0
			DO pos=0 TO slen
				SFX_RefrProWin pwi pos
				IF RESULT=0 THEN DO
					SFX_GetSampleValue buf1 0 pos
					aktval=RESULT

					energy=energy+aktval

					IF aktval<0 AND energy<10 THEN DO
						mins=1
					END
					IF aktval>0 AND energy>10 AND mins=1 THEN DO
						curlen=pos-lastpos
						lastpos=pos
						mins=0
						frq=srat/curlen
					END

					enval=energy/500.0

					SFX_PutSampleValue buf2 0 pos aktval
					SFX_PutSampleValue buf2 1 pos enval
					SFX_PutSampleValue buf2 2 pos mins
					SFX_PutSampleValue buf2 3 pos frq

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
