divert(-1)
define(`__TITLE',`NoiseGate')
define(`__NODE',__file__)
define(`__PATH',`../')
include(`global.m4')
include(`lang_eng.m4')
include(`header.m4')
include(`footer.m4')
include(`navbar.m4')
divert`'dnl
<html>
	__HEADER
	<body __PAGE_STYLE>
		__NAVBAR
		<!-- Contents -->
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(NoiseGate)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Fades parts which are quieter than the threshold out.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Threshold (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Amplitude which serves as a threshold for starting to fade out
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Attack</nobr></td><td align="left">the sound will not just get muted, it be be faded out and back in.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Shape</nobr></td><td align="left">type of the fade
</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Notes)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Can be used with solo recordings (e.g. speech, guitar, ...) that contain
noisy pauses.<br>
For percussive material I recommend using shorter attack values (e.g. 0.5 ms),
otherwise the attack can be a bit longer (e.g. 1.0 ms).<br>
Bevor man diesen Operator nutzt empfiehlt es sich, erst den
<a href="Middle.html">Middle</a> Operator anzuwenden, um das Sample vorzubereiten.<br>
Remember this operator a lot more effective with real 16-bit samples
than with 8-bit samples.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
