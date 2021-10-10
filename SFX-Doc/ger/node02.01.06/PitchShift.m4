divert(-1)
define(`__TITLE',`PitchShift')
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
	__SECTION_HEADER(PitchShift)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Ändert die Tonhöhe eines Samples, ohne das es kürzer oder länger wird.
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
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>PitchShift Factor (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Faktor für die Tonhöhenänderung
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Window</nobr></td><td align="left">Fensterbereich; gute Ergebnisse erhält man mit Werten von 5 bis 100 ms
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Smooth</nobr></td><td align="left">Über wieviel Prozent des Fensterbereiches übergeblendet werden soll;
üblicherweise zweischen 25 % und 50 %
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
Bevor ich etwas speziellere Tips gebe, möchte ich allgemein beschreiben, wie dies
alles funktioniert. Wenn sie einen Sound höher stimmen möchten, können Sie dies
erreichen indem sie den Klang schneller abspielen und damit die Wellenform stauchen
(auf der Zeitachse). Leider wird der Sound dadurch kürzer. Um dies zu kompensieren,
wird __SFX kleine Stückchen des Sounds wiederholen um das Sample zu strecken.
Dabei muß __SFX aufpassen, daß die Stückchen möglichst nahtlos zusammenpassen.
Der WinSize Parameter gibt an wie weit __SFX maximal nach einem guten Übergang
sucht. Die Größe hängt jedoch vom zu beabeitenden Material ab. Ich empfehle kleinere
Werte (30-50 ms) für perkussive Samples (dies verhindert, daß man die Anschläge
mehrfach hört) und längere Werte (100-200 ms) für Synth/Pad/String-Sounds
(um ein mögliches Leiern zu vermeiden).<br>
Wenn man eine Sinuswelle als <a href="../noed06.03.00.Modulation.html">Modulator</a>
nimmt eine geringe Verstimmung (+/- 10 ct) einstellt, erhält man einen Vibratoeffekt.<br>
Falls man synthetische Wellenformen mit konstanter Periode bearbeiten möchte, sollte
man die Periode bei WinSize eintragen. Dadurch erhält man sehr saubere Pitchshifts.<br>
Der Faktor sollte nicht größer als 4.0 genommen werden. Bei solch hohen Faktoren wird
das Sample schnell unsauber (liegt in der Funktionsweise des Pitchshifters begründet).
Synthetische Wellenformen lassen sich alledings nahezu beliebig "pitchshiften".<br>
Wenn das resultierende Sample Knackser aufweist, ändern sie den WinSize-Parameter
etwas und/oder erhöhen sie smooth.<br>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
