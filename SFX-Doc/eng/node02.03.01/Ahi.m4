divert(-1)
define(`__TITLE',`Ahi')
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
Plays the active sample with the AHI audio system by Martin Blom. This can be
downloaded from the following sources :<br>
Aminet:<br>
<a href="ftp://ftp.germany.aminet.org/pub/aminet/dev/misc/ahidev.lha">ahidev.lha</a><br>
<a href="ftp://ftp.germany.aminet.org/pub/aminet/mus/misc/ahiusr.lha">ahiusr.lha</a><br>
<a href="ftp://ftp.germany.aminet.org/pub/aminet/docs/misc/ahiman.lha">ahiman.lha</a><br>
<br>
WWW:<br>
<a href="http://www.lysator.liu.se/~lcs/ahi.html">http://www.lysator.liu.se/~lcs/ahi.html</a><br>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Audiomode</nobr></td><td align="left">Here you can choose the auidomode (which audiohardware, how many channels,...)and what mixing frequency (sampling rate for playback) should be used.</td></tr>
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
