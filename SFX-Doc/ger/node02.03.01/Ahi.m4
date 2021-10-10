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
	__SECTION_HEADER(Funktion)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Spielt das aktive Sample über das AHI Audio System von Martin Blom ab.
Dieses könen sie von folgenden Quellen herunterladen :<br>
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
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Audiomode</nobr></td><td align="left">Hier kann man den Audiomodus (also welche Audiohardware, wieviele Kan&auml;le,...)und die zu verwendende Mischfrequenz (Samplingrate f&uuml; das Abspielen) festlegen.</td></tr>
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
