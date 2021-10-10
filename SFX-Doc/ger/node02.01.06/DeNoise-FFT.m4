divert(-1)
define(`__TITLE',`DeNoise-FFT')
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
	__SECTION_HEADER(DeNoise-FFT)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Entrauscht ein Sample (Multifrequenz Noisegate)
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Attack)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Attack</nobr></td><td align="left">Ansprechdauer. Wenn der Operator Rauschen erkennt, wird dies sanft aus- und wieder eingeblendet.
Hiermit läßt sich einstellen wie schnell das geschieht.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Shape</nobr></td><td align="left">Hüllform für das Aus- und Einblenden.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Threshold</nobr></td><td align="left">Eine Signalkomponente die leiser als dieser Pegel ist, wird als Rauschen gewertet.
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
Wenn der Threshold zu hoch ist wird zuviel vom Sample unterdrückt.
Das Ergebnis kann in diesem Falle dumpf klingen.<br>
Der Attackwert sollte normalerweise recht klein sein. Wenn er aber zu klein ist,
kann sich das Ergebnis zerhackt anhören.<br>
Der Effekt kann gut mit Hilfe des <a href="Analyse-Spect2D.html">Analyse-Spect2D</a> Operators
und eines niedrigen Gamma-Wertes (z.B. 0.2) kontrolliert werden.
Nach dem Ausführen des DeNoise Operators prüfen sie erneut mit dem Analyser. Es sollte erkennbar sein
ob das Rauschen leiser geworden ist.<br>
Es ist sehr kompliziert, die richtigen Einstellungen zu finden.
Das Bearbeiten mit diesem Operator führt nahezu immer zu diversen
Klangverfremdungen, die allerdings teilweise sehr interessant sind.<br>
Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a> verwendet.<br>
Bevor man diesen Operator nutzt empfiehlt es sich, erst den
<a href="Middle.html">Middle</a> Operator anzuwenden, um das Sample vorzubereiten.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
