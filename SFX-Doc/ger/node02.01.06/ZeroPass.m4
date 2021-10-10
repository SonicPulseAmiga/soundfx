divert(-1)
define(`__TITLE',`ZeroPass')
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
	__SECTION_HEADER(ZeroPass)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Blendet die Lautstärke am Start von 0 ein und am Ende zu 0 aus
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>FadeIn/Range (SRange)</nobr></td><td align="left">Bereich der für die Einblendung genutzt werden soll
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>FadeIn/Shape (SModShape)</nobr></td><td align="left">Hüllform, wie übergeblendet werden soll.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>FadeOut/Range (ERange)</nobr></td><td align="left">Bereich der für die Ausblendung genutzt werden soll
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>FadeOut/Shape (EModShape)</nobr></td><td align="left">Hüllform, wie übergeblendet werden soll.
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
Falls ein Sample über/unter der 0-Linie beginnt/endet, hört man dies als
Knacken. Diese Funktion legt den Anfang bzw. das Ende auf 0 und blendet
dann innerhalb des Bereichs (RangeS/RangeE) zu den Originalwerten über.
Die Kurve &quot;slowdown&quot;  erzeugt eine Überblendung, die das Ohr
als linear empfindet.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
