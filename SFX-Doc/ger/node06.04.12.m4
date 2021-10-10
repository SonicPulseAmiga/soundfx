divert(-1)
define(`__TITLE',`Operatoren : Tuning')
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
					<p align="justify">
						<b>F:</b> Was ist eigentlich genau der Unterschied zwischen Detune,
								  PitchShift und Resample, in der Hinsicht das doch alle 3
								  die Tonhöhe eines Klanges verändern.<br>
						<b>A:</b> Detune und Resample sind sich relativ ähnlich. Beide
								  geben eingehende Samplewerte mit einer andern
								  Geschwindigkeit aus, und ändern somit auch die Länge des
								  Samples während sie die Tonhöhe variieren.<br>
								  Der Unterschied hierbei ist, daß Resample dies mit einer
								  konstanten Rate macht (z.B. 3 Ausgangswerte für 2
								  Eingangswerte erzeugt), während die Rate bei Detune
								  variabel ist.
								  Resample bietet zusätzlich ein paar Funktionen, um
								  ungewünschte Nebeneffekte (Aliasing) zu vermeiden.
								  Um dies zu illustrieren ein Beispiel : stellen Sie sich
								  folgendes Sample vor<br>
								  +-+-+-+-<br>
								  wobei + eine positive maximale Amplitude und - eine negative
								  maximale Amplitude darstellt.
								  Jetzt rechnen wir das um den Faktor 2 herunter (wir
								  überspringen jeden 2-ten Wert) und erhalten<br>
								  ++++<br>
								  Die hohe Frequenz hat sich ausgelöscht bzw. ist sogar
								  ein Gleichspannungoffset geworden. Und es wird sogar noch
								  schlimmer, wenn man gebrochenen Faktoren (z.B. 1.5) benutzt.
								  Dann würde man etwas wie folgendes bekommen<br>
								  +--+-<br>
								  Es gibt da noch einen einfachen Weg diesen Effekt zu sehen
								  und zu hören. Benutzen sie Synthesize-Add um einen
								  Sinusverlauf von 1000 Hz bis z.B. 100000 Hz zu erzeugen.
								  Wählen sie dabei eine Samplingrate von z.B. 44100 Hz.
								  Das Ergebnis sollte ein Ton wie puuuuuiiiiiiieeeee ;-)
								  sein. Es wird sich allerdings mehr nach puuuuiiiiieeeeeiiieeeeuuueeeeiii.
								  anhören.<br>
								  Benutzen Sie Analyse-Spect-2D um es darzustellen und
								  Sie werden verstehen warum.<br><br>
	
								  In der Praxis verwendet man Resample, wenn eine Aufnahme mit
								  z.B. 20050 Hz hat, diese aber in einer Software einsetzen
								  möchte die 44100 Hz verlangt, da Resample die Aufnahme so
								  verändert das sie bei 44100 nun genauso klingt wie zuvor bei
								  20050 Hz.<br>
								  Im Gegensatz dazu dient Detun für Effekt wie die Simulation
								  eines Stromausfalles oder Alien-Stimmen.<br><br>

								  Nun also noch zu PitchShift. Dessen Spezialität ist es die
								  Tonhöhe zu verändern ohne die Länge zu beeinflussen, indem
								  es reichlich Magie einsetzt (intelligentes Wiederholen oder
								  Auslassen kurzer Samplefragmente).
								  Ähnlich dazu gibt es noch einen Effekt, welcher früher oder
								  später (ich hoffe eher) auch mal in __SFX erscheinen wird.
								  Er heißt Timestretch und macht ein Sample länger oder kürzer
								  ohne die Tonhöhe zu beeinflussen. Grundsätzlich ist es ein
								  PitchShift gefolgt von einer Resample-Operation (kann also auch
								  schon jetzt in __SFX simuliert werden).<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
