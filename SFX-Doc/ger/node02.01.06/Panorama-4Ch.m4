divert(-1)
define(`__TITLE',`Panorama-4Ch')
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
Verteilt ein Monosample auf 4 <a href="../node06.03.00.Channel.html">Kanäle</a>.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Left-Right Position (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Verhältnis für links und rechts. 0 % (oder 0.0) bedeutet links, 100 % (oder 1.0) rechts.</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Front-Back Position (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Verhältnis für vorne und hinten. 0 % (oder 0.0) bedeutet vorne, 100 % (oder 1.0) hinten.</td></tr>
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
Das Ergebnis kann per <a href="SurroundEncoder.html">SurroundEncoder</a> wieder in ein Stereosample gewandelt werden,
welches seinen vollen Raumklang entfaltet wenn es über einen SurroundDecoder abgespielt wird.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
