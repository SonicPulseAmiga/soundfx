divert(-1)
define(`__TITLE',`Subtract')
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
	__SECTION_HEADER(Subtract)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Subtrahiert die Sampledaten des 2. Samples vom 1. Sample
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Delay Source 2</nobr></td><td align="left">verzögert Source 2
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
Bei gleichen Puffern und einem Delay von 0 kommt es zur totalen Auslöschung.<br>
Diesen Effekt kann man auch verwenden um herauszufinden welche Änderungen eine
vorherige Aktion auf das Sample hatte. Wenden sie einen Effekt auf ein Sample an
und subtrahieren sie dann das Original von dem Effekt-Sample. Das Ergebnis
ist das reine Effekt-Signal. Eine interessante Anwendung dafür ist herauszubekommen
was beim Speichern mit kompression (wie mp3) verloren geht. Laden sie das Sample
nach dem Speichern einfach wieder ein und subtrahieren sie das komprimierte vom
originalen Sample.  
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
