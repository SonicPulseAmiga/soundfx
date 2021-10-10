divert(-1)
define(`__TITLE',`Amplify')
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
	__SECTION_HEADER(Amplify)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Ändert die Lautstärke eines Samples
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Amplification (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Dieser Wert gibt die Lautstärkeänderung an. Die Lautstärke kann angehoben und/oder abgesenkt werden.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>MaxVol</nobr></td><td align="left">Durch einen Klick auf diesen Knopf, wird das Sample gescannt und die maximale
Verstärkung errechnet die möglich ist, ohne das Signal zu übersteuern.
Das Ergebnis wird in Par0 eingetragen und die Modulation wird auf "None" gesetzt.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Wrap</nobr></td><td align="left">Gibt an, wie eine mögliche Übersteuerung des Signals behandelt werden soll.
Hierbei gibt es 4 Modi :
<ul>
<li>NoClip : es wird nicht auf Übersteuerte Werte gestestet; erzeugt verzerrte Klänge wenn die Lautstärke über das Maximum hinaus angehoben wird</li>
<li>Clip : die übersteuerten Werte werden auf Maximum bzw. Minimum gesetzt</li>
<li>Wrap1 : der übersteuerte Anteil wird an der anderen Seite wieder hereingeschoben, und zwar solange, bis er komplett im Normalbereich ist.</li>
<li>Wrap2 : der übersteuerte Anteil wird solange an der Ober- und Unterkante umgeklappt, bis er komplett im Normalbereich ist.</li>
</ul>
Diese Modi sollte man ruhig mal ausprobieren. Dazu nimmt man einen lange Sinus
und übersteuert diesen langsam.
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
Schlagzeuginstrumente ( besonders Basedrums ) können ruhig mal
etwas übersteuert werden (ca. 120 %). Eine solche leichte
<a href="../node06.03.00.Overdrive.html">Übersteuerung</a> ergibt den typischen Overdriveeffekt, durch
die gekappten Samplewerte.<br>
Die Verstärkung die ohne Übersteuerung möglich ist, kann an den
Min- und Maxlinien im <a href="../node01.05.01.html">Samplefenster</a> abgeschätzt werden.<br>
Dieser Operator läßt sich außerdem noch zur Amplituden- und Ringmodulation
verwenden, wodurch sich weitere Synthesemöglichkeiten ergeben.
Erzeugen Sie dazu z.B. einen Sinus mit normaler Periode
und einen weiteren mit doppelter. Jetzt wählen Sie ein Sample
als Source und stellen als Modulation User/Normal ein. Par0
setzen Sie auf 0.0 und Par1 auf 1.0. Als Modulator nehmen Sie den
anderen Sinus. Lassen Sie das neue Sample erzeugen und schauen
Sie es sich an (eventuell vergrößern).
Was sie getan haben nennt man Ringmodulation. Wenn sie den Modulationsbereich
auf -1.0 bis 1.0 ausdehnen erhalten sie eine Amplitudenmodulation.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
