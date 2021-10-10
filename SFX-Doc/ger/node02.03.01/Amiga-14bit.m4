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
	__SECTION_HEADER(Funktion)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Spielt das Sample des aktuellen Puffers über Cascadierung der Soundkanäle in 14-bit ab.
Es wird keine zusätzliche Hardware benötigt. Die maximale Abspielrate beträgt auf
PAL/NTSC-Bildschirmen ca. 29Khz und auf Productivity-Bildschirmen ca. 58kHz.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>HFilter</nobr></td><td align="left">ob der Hardwarefilter an- oder ausgeschaltet werden soll.</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>RateClip</nobr></td><td align="left">max. abspielbare Rate, wenn die Samplingrate des Samples größer ist, wirdes während des Abspielens resampled, so das es wie mit der entsprechenden Rate abgespielt klingt.</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Hinweise)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
keine
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
