divert(-1)
define(`__TITLE',`Support')
define(`__NODE',__file__)
define(`__PATH',`')
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
			__SECTION_HEADER(__CHAPTER`'__TITLE)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify">Bitte lesen sie den Abschnitt <a href="node06.04.__.html">FAQ</a>
					bevor sie __AUTHOR schreiben. Vielleicht gibt es ja schon eine Lösung zu ihrem Problem.<br>
					Wenn sie mir schreiben, senden sie bitte folgende Angaben mit:<ul>
						<li>ihre Rechnerkonfiguration, am Besten sie benutzen dazu "sys:Tools/ShowConfig"</li>
						<li>welche SFX-Version und welche SFX-CPUversion Sie benutzen, "version FILE=SoundFX FULL"</li>
						<li>wo der Fehler auftritt. Je genauer diese Beschreibung ist, desto leichter
							kann ich den Fehler beheben. Bitte keine Anfragen der Art "... geht nicht". Beschreiben
							sie mir möglichst genau, was sie gemacht haben und was sie machen wollen. Jedes
							kleine Detail könnte von Bedeutung sein.<br>
							Senden sie eventuell Samples mit (wenn es was damit zu tun haben kann), aber bitte nicht
							unbedingt mehr als 0.5 Mb. Wenn ein bestimmter Operator Probleme macht, schicken
							sie mir doch einfach ihre Einstellungen als Preset.</li>
					</ul></p>
					<p align="justify">Wie gesagt, ich bin bemüht das Programm so gut wie möglich zu machen und Sie können mir
					dabei helfen. Ich versuche auf jede Email zu antworten, kann aber nicht alle Briefe beantworten
					(Ich lese alle Briefe und versuche die berichteten Fehler zu eliminieren).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(`BugTracker')
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify">Wenn sie eine Internetverbindung haben, empfehle ich ihnen sich den BugTracker
					auf <a href="http://www.sonicpulse.de/phpbt/" target="new">www.sonicpulse.de/phpbt/</a> anzusehen
					und ihre Bug-Reports, Feature-Request oder Help-Request dort einzutragen.
					Dies erleichtert das Sammeln von Informationen zur einem Problemfall und hilft mir den Überblick
					zu behalten.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(`Werkzeuge für Fehlerberichte')
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify">
						Weiterhin ist es hilfreich mit Tools wie Snoopdos, Enforcer, Mungwall usw. zu testen
						und mir die Ausgaben mitzusenden.
					</p>
					<p align="justify">
						Snoopdos ist das richtige Werkzeug, um herauszufinden ob Dateien fehlen
						(unvollständige Installation, Fehler auf der Festplatte).
					</p>
					<p align="justify">
						Wenn das sich das Programm merkwürdig verhält oder gar abstürzt, kann eine Werkzeugekette bestehend aus<ul>
							<li>SegTracker + MuForce + MuGuardianAngel</li>
							<li>SegTracker + WipeOut + CyberGuard</li>
							<li>SegTracker + Mungwall + Enforcer</li>
						</ul>
						helfen die Ursache des Problems zu finden.<br>
						SegTracker muß zeitig in der startup-sequence gestarted werden. Es braucht keine Parameter.
						Um Fehler zu suchen, starte ich MuForce und MuGuardianAngel wie folgt
<pre>
run &gt;NIL: sashimi CONSOLE WINDOW="CON:0/14/800/200/Sashimi [Ctrl]+E=Empty [Ctrl]+F=File [Ctrl]+D=Reset console/CLOSE/WAIT/INACTIVE/NOFNC/JUMP/FASTUPDATE" ON NOPROMPT TIMERON
run &gt;nil: &lt;nil: MuForce AREGCHECK DREGCHECK STACKCHECK CAPTURESUPER LED 0 RAWIO DISABLEBELL ON
run &gt;nil: &lt;nil: MuGuardianAngel AREGCHECK DREGCHECK STACKCHECK SHOWHUNK LED 0 DISABLEBELL WAITFORMUFORCE
</pre><br>
						Die erste Zeile statet Sashimi - ein Hilfsprogramm das die Log-Ausgaben der nächsten beiden
						Programme abfängt und in einem Shell-Fenster anzeigt. Wenn sie eine Shell-Erweiterung wie
						KingCON nutzen, können sie die Ausgaben abspeichern und mir senden.<br>
						Dabei ist es wichtig das der SegTracker läuft. Sie sehen dies daran, das in den Ausgaben
						Zeilen mit HUNK und OFFSET Werten auftauchen. Dies sind die Daten die ich brauche um die
						Fehlerstelle im Quelltext zu finden.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
