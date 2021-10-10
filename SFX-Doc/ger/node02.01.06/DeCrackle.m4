divert(-1)
define(`__TITLE',`DeCrackle')
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
	__SECTION_HEADER(DeCrackle)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Dämpft starke Pegelsprüge (Knackser)
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Dif.</nobr></td><td align="left">Pegelsprungschwellwert. Sobald ein Pegelsprung gegenüber den durchschnitlichen Pegelsprungwerten in der aktuellen
Umgebung soviel über diesem Wert liegt, wird er gedämpft.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Amp.</nobr></td><td align="left">Amplitudenschwellwert. Sobald die aktuelle Amplitude gegenüber der Durchschnittsamplitude der aktuellen
Umgebung soviel über diesem Wert liegt, wird er gedämpft.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Adjust</nobr></td><td align="left">Wie stark der Knackser gedämpft werden soll. 100 % entspricht der totalen Auslöschung.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Size</nobr></td><td align="left">The maximale Länge die ein Störsignal haben darf um als Knackser eingestuft zu werden.
Knackser sind normalerweise sehr kurz. Dieser Parameter dient dazu Knackser
von percusiven Klängen zu unterscheiden.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Test</nobr></td><td align="left">Startet den Operator ohne das Sample zu verändern und zeigt die Ergebnisse der Knackseranalyse an.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Stat.</nobr></td><td align="left">Die Menge der gefundenen Knackser (absolut und in Prozent relativ zur Länge) für jeden Kanal des Samples.
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
Dieser Operator entfernt bzw. dämpft Knackstellen in Samples.
Diese treten z.B. auf wenn man von einer Schallplatte sampelt
oder mal einen R/W-Fehler auf einem Datenträger hatte.<br>
Bevor man diesen Operator nutzt empfiehlt es sich, erst den
<a href="Middle.html">Middle</a> Operator, gefolgt vom
<a href="ZeroPass.html">ZeroPass</a> Operator und abschließend den
<a href="Amplify.html">Amplify</a> Operator mit der MaxVol Funktion
anzuwenden, um das Sample vorzubereiten.
Wenn die Ergebnisse dieses Operators dumpf klingen und die Anschläge fehlen,
dann erhöhen sie die Dif. und Amp. Werte, so das weniger Signale als
Knackser interpretiert werden. Wenn offensichtliche Knackser nicht entfernt werden,
vergrößern sie einen Solchen und betrachten sie die Länge. Stellen sie den
Size Parameter neu ein.
Sie können die Test Funktion benutzen um die Ergebnisse abzustimmen.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
