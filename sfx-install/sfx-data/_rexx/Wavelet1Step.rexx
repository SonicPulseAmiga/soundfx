/* SFX Wavelet 1 Step */

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
	dlen=slen/2
	SFX_GetRate sbuf
	srat=RESULT
	drat=srat/2
	SFX_GetChannels sbuf
	cha=RESULT


	SFX_NewBuffer dlen drat cha
	dbufs=RESULT
	SFX_NewBuffer dlen drat cha
	dbufd=RESULT
	IF dbufs>-1 AND dbufd>-1 THEN DO
		brk=0
		DO ch=0 TO cha
			SFX_IsChannelActive sbuf ch
			IF RESULT>0 THEN DO
				SFX_InitProWin dlen '"Working on Wavelet1Step['ch']"'
				pwi=RESULT
				IF pwi>-1 THEN DO
					spos=0
					DO dpos=0 TO dlen
						SFX_RefrProWin pwi dpos
						IF RESULT=0 THEN DO
							SFX_GetSampleValue sbuf ch spos
							val1=RESULT
							spos=spos+1
							SFX_GetSampleValue sbuf ch spos
							val2=RESULT
							spos=spos+1
							dif=(val1-val2)/2
							SFX_PutSampleValue dbufd ch dpos dif
							sum=(val1+val2)/2
							SFX_PutSampleValue dbufs ch dpos sum
						END
						ELSE DO
							brk=1
							LEAVE dpos
						END
					END
					SFX_RemoveProWin pwi
				END
				ELSE DO
					SFX_Message '"Can`t open ProgressWindow !"'
					LEAVE ch
				END
			END
			IF brk=1 THEN LEAVE ch
		END
		SFX_RedrawBuffer dbufs
		SFX_RedrawBuffer dbufd
	END
	ELSE SFX_Message '"Can`t add samplebuffer !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
