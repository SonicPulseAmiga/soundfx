/* SFX Rexx-Test */

SIGNAL ON SYNTAX
OPTIONS RESULTS
sfxport=ADDRESS()
IF (LEFT(sfxport, 8) ~= "SFX_REXX") THEN DO	/* not started by SoundFX ? */
	IF ~SHOW("Ports","SFX_REXX") THEN EXIT 10
    sfxport='SFX_REXX'
	SAY "default addr"
END
ELSE SAY "preselected addr"

SAY "[" || sfxport || "]"
ADDRESS VALUE sfxport

SFX_Message 'Hi'
EXIT

SYNTAX:
	SAY "Error #"||RC||":" ERRORTEXT(RC) "in line "||SIGL
EXIT
