/* SFX Rexx-Test */

SIGNAL ON SYNTAX
OPTIONS RESULTS
IF (LEFT(ADDRESS(), 8) ~= "SFX_REXX") THEN DO	/* not started by SoundFX ? */
	IF ~SHOW("Ports","SFX_REXX") THEN EXIT 10
    ADDRESS 'SFX_REXX'
END

SFX_Message 'Hi'
EXIT

SYNTAX:
	SAY "Error #"||RC||":" ERRORTEXT(RC) "in line "||SIGL
EXIT
