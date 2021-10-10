divert(-1)
define(`__TITLE',`ConvertChannels')
define(`__NODE',__file__)
define(`__PATH',`../')
include(`global.m4')
include(`lang_ger.m4')
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
	__SECTION_HEADER(ConvertChannels)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Wandelt zwischen verschiedenen <a href="../node06.03.00.Channel.html">Kanal</a>formaten
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Matrix (Mat x y)</nobr></td><td align="left">Die Eingangswerte werden mit diesen Faktoren multipliziert und als Summe ausgegeben.
Sinnvolle Werte für die Faktoren liegen zwischen -1.0 und 1.0.
</td></tr>
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
Hiermit lassen sich nahezu alle denkbaren Kanalverwandlungen
realisieren. Das Sample liegt an der Sourceseite an und gelangt
an der Zielseite heraus. Das Ergebnis wird soviele Kanäle haben,
wie belegte Zielspalten existieren.<br>
Es liegen viele Presets bei, die die Arbeitsweise gut verdeutlichen.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
