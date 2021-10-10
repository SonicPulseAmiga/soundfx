divert(-1)
define(`__TITLE',`Distortion')
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
	__SECTION_HEADER(Distortion)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Erzeugt Distortion und Fuzz Effekte.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">wie stark der Effekt in das Ergebnis einfließt
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Distortion Shape (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">diese Form bestimmt die Art und die Stärke der Verzerrungen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Map</nobr></td><td align="left">die Kurve kann auf verschiedene Arten übertragen werden :
<ul>
<li>full range : so wie sie ist [-max to max]</li>
<li>mirrored : oder auch kopiert und um ihren Ursprung gedreht werden [-max to 0]=-[0 to max], was die gleichen Kurven für positive wie auch negative Samplewerte ergibt</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Wrap</nobr></td><td align="left"></ul>
Gibt an, wie eine mögliche Übersteuerung des Signals behandelt werden soll.
Hierbei gibt es 4 Modi :
<ul>
<li>NoClip: es wird nicht auf Übersteuerte Werte gestestet.</li>
<li>Clip : die übersteuerten Werte werden auf Maximum bzw. Minimum gesetzt</li>
<li>Wrap1 : der übersteuerte Anteil wird an der anderen Seite wieder hereingeschoben, und zwar solange, bis er komplett im Normalbereich ist.</li>
<li>Wrap2 : der übersteuerte Anteil wird solange an der Ober- und Unterkante umgeklappt, bis er komplett im Normalbereich ist.</li>
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
Die Kurve dient als eine Art Übersetzungstabelle. Wenn die Kurve eine gerade
Linie (von links unten nach rechts oben) ist, würde sich nichts am Klang ändern.
Je mehr die Kurve jedoch davon abweicht, desto verzerrter wird der Sound klingen.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
