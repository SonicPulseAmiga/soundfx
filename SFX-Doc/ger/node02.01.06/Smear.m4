divert(-1)
define(`__TITLE',`Smear')
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
	__SECTION_HEADER(Smear)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<a href="../node06.03.00.Modulation.html">Moduliert</a> die Ausleseposition der Sampledaten und mischt die
gelesenen Werte auf die Orignalwerte.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">wie stark der Effekt in das Ergebnis einflie?t
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Smear Range (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">wie stark die Position verschoben werden soll
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Interpolation (<a href="../node03.02.02.html">I1</a>)</nobr></td><td align="left">wie sollen (sanfte) Zwischenwerte berechnet werden
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
Man sollte den Bereich f?r die Verschiebung nicht zu gro? w?hlen (d?rfte nur
selten gut klingen).<br>
Normalerweise sollte der Smear Range-Parameter Sample moduliert werden.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
