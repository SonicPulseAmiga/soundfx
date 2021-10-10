/****h* SoundFX/ExpSmoothing.rexx [4.00] *
*  NAME
*    ExpSmoothing.rexx
*  COPYRIGHT
*    $VER: ExpSmoothing.rexx 4.00 (22.02.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    applies an exponential dampening to the active sample
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
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_GetLength buf1
	slen=RESULT
	SFX_GetRate buf1
	srat=RESULT
	SFX_GetChannels buf1
	cha=RESULT

	fc1=0.995
	fc2=1.0-fc1

	SFX_NewBuffer slen srat cha
	buf2=RESULT
	IF buf2>-1 THEN DO
		brk=0
		DO ch=0 TO cha
			SFX_IsChannelActive buf1 ch
			IF RESULT>0 THEN DO
				SFX_InitProWin slen '"Working on ExpSmoothing['ch']"'
				pwi=RESULT
				IF pwi>-1 THEN DO
					SFX_GetSampleValue buf1 ch 0
					oldval=RESULT
					DO pos=1 TO slen
						SFX_RefrProWin pwi pos
						IF RESULT=0 THEN DO
							SFX_GetSampleValue buf1 ch pos
							newval=RESULT
/*							newval=abs(RESULT)	*/				/* this would produre an envelope follower */
							oldval=(fc1*oldval)+(fc2*newval)
							SFX_PutSampleValue buf2 ch pos oldval
						END
						ELSE DO
							brk=1
							LEAVE pos
						END
					END
					SFX_RemoveProWin pwi
				END
				ELSE DO
					SFX_Message '"Can`t open ProceedWindow !"'
					LEAVE ch
				END
			END
			IF brk=1 THEN LEAVE ch
		END
		SFX_RedrawBuffer buf2
	END
	ELSE SFX_Message '"Can`t add samplebuffer !"'
END
ELSE SFX_Message '"No sample to operate on !"'
SFX_Activate
EXIT
