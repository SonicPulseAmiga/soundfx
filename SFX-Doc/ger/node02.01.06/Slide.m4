divert(-1)
define(`__TITLE',`Slide')
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
	__SECTION_HEADER(Slide)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Verschiebt die Samplewerte vertikal.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Distance (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Wert, um den das Sample verschoben werden soll. Negative Werte ergeben eine
Verschiebung nach unten, positive Werte eine nach oben.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Wrap</nobr></td><td align="left">Gibt an, wie eine m�gliche �bersteuerung des Signals behandelt werden soll.
Hierbei gibt es 4 Modi : 
<ul>
<li>NoClip: es wird nicht auf �bersteuerte Werte gestestet.</li>
<li>Clip : die �bersteuerten Werte werden auf Maximum bzw. Minimum gesetzt</li>
<li>Wrap1 : der �bersteuerte Anteil wird an der anderen Seite wieder hereingeschoben, und zwar solange, bis er komplett im Normalbereich ist.</li>
<li>Wrap2 : der �bersteuerte Anteil wird solange an der Ober- und Unterkante umgeklappt, bis er komplett im Normalbereich ist.</li>
</ul>
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
keine
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
