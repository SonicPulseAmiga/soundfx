divert(-1)
define(`__TITLE',`Amiga-14bit')
define(`__NODE',__file__)
define(`__PATH',`../')
include(`global.m4')
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
	__SECTION_HEADER(Function)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Plays the active sample over cascaded soundchannels in 14-bit, whithout extra hardware.
The maximum playbackrate on PAL/NTSC screens is about 29Khz and on Productivity
screens about 58kHz.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>HFilter</nobr></td><td align="left">Hardware filter on/off (Power LED)</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>RateClip</nobr></td><td align="left">maximum playbackrate, if the samplerate is higher, SFX resamples whileplaying, so the pitch is right.</td></tr>
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
none
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
