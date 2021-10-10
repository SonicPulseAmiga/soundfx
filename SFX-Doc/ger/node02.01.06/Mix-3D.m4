divert(-1)
define(`__TITLE',`Mix-3D')
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
	__SECTION_HEADER(Mix-3D)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Mischt 8 Samples über Pfad in einen Würfel
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Sources</nobr></td><td align="left">Die Ausgangssamples die gemischt werden.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>X-Axis (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Position des Punktes auf der X-Achse
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Y-Axis (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Position des Punktes auf der Y-Achse
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Z-Axis (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">Position des Punktes auf der Z-Achse
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Path</nobr></td><td align="left">In diesem Feld wird der Pfad, als Kurve im Würfel, dargestellt. Während
der Berechnung wird ein Punkt entlang der Kurve vom Begin bis zum Ende
wandern. Die Entfernung dies Punktes zu den Ecken bestimmt wie laut die
zu diesen Ecken zugeordneten Samples in das Ergebnis gemischt werden.<br>
Mit "View" läßt sich der Ansichtspunkt festlegen und mit "Prec." die
Genauigkeit mit der die Kurve gezeichnet wird.
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
Mixen sie doch mal verschiedene Variationen eines Samples zusammen. Wiederholen
sie dies mit einer anderen Kurve und verbinden sie die Resultate zu einem
Stereosample.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
