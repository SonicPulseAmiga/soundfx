divert(-1)
define(`__TITLE',`Morph-FFT')
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
	__SECTION_HEADER(Morph-FFT)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Wandelt das Frequenzspektrum von Source 1 in das von Source 2 um.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Morph (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Kontrolliert den Übergang von Source1 nach Source2
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Bands</nobr></td><td align="left">Wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
die Frequenzauflösung ist dann aber auch nicht so fein.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Steps</nobr></td><td align="left">Aller wieviel Samples eine Transformation erstellt werden soll. Je öfters diese berechnet
werden, desto genauer das Ergebnis und desto 	höher auch die Rechenzeit. Steps darf max. halb
so groß wie die Nummer der Bänder sein.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left">welche Fensterfunktion verwendet wird.
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
Versuchen Sie z.B. mal zwei lange Sinustöne mit verschieder Tonhöhe langsam
zu morphen.<br>
Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a> verwendet.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
