divert(-1)
define(`__TITLE',`Dynamic')
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
	__SECTION_HEADER(Dynamic)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Verst�rkt bzw. verringert die Lautst�rke des Samples in Abh�ngigkeit von seiner Amplitude.
Erm�glicht komplexe Eingriffe in die <a href="../node06.03.00.Dynamic.html">Dynamik</a> des Samples.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">wie stark der Effekt in das Ergebnis einflie�t
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Ratio loud (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Lautst�rke�nderung f�r die lauten Signale
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Ratio quiet (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">Lautst�rke�nderung f�r die leisen Signale
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Threshold (<a href="../node03.02.01.html">P4</a>)</nobr></td><td align="left">bestimmt den �bergangspunkt zwischen leiser und lauter Ratio - immer wenn die Amplitude
des Signals den Schwellwert �berschreitet wird die 'loud Ratio' angewendet, sonst wird
die 'quiet Ratio' gew�hlt
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Knee</nobr></td><td align="left">es gibt zwei Varianten, eine eckige und eine gegl�ttete
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Characteristics</nobr></td><td align="left">Diese Kurven zeigen die Auswirkungen der Einstellungen. Lesen sie es wie eine �bersetzungstabelle - die
Lautst�rke des Eingangssignals bestimmt die x Position, dann kann �ber die Kurve die zugeh�rige y Postion
bestimmt werden, welche die die Ausgangslautst�rke darstellt.
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
Folgend noch ein paar Beispiele:<ul>
<li>Compresor: staucht das Sample<br>Ratio loud &lt;100 %, Ratio quiet &gt;100 %</li>
<li>Expander: zerrt das Sample auseinander<br>Ratio loud &gt;100, Ratio quiet &lt;100 %</li>
<li>Limiter: verst�rkt die leisen Teile des Samples<br>Ratio loud =100, Ratio quiet &gt;100 %</li>
<li>Delimiter: verst�rkt die lauten Teile des Samples<br>Ratio loud &gt;100, Ratio quiet =100 %</li>
</ul>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
