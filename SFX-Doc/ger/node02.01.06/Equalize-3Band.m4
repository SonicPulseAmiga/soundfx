divert(-1)
define(`__TITLE',`Equalize-3Band')
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
	__SECTION_HEADER(Equalize-3Band)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Hohe, mittlere und tiefe Frequenzen können angehoben oder abgesenkt werden. Funktioniert wie die
Klangkontrolle an der Stereoanlage.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Lower Cut-Off (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Frequenz die das tiefe vom mittleren Band trennt, relative Frequenz von 0 Hz bis zur halben Samplingrate
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Higher Cut-Off (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Frequenz die das mittlere vom hohen Band trennt, relative Frequenz von 0 Hz bis zur halben Samplingrate
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Lower gain (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">Lautstärkeanpassung des tiefen Bandes
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Middle gain (<a href="../node03.02.01.html">P4</a>)</nobr></td><td align="left">Lautstärkeanpassung des mittleren Bandes
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Higher gain (<a href="../node03.02.01.html">P5</a>)</nobr></td><td align="left">Lautstärkeanpassung des höheren Bandes
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Ampf</nobr></td><td align="left">abschließende Lautstärkeanpassung
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
An Ihrere Stereoanlage werden sie die Filter Cut-Offs warscheinlich nicht ändern können. Im Zweifelsfalle
lassen sie die Werte einfach wie sie sind.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
