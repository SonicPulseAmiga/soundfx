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
								  die Tonh�he eines Klanges ver�ndern.<br>
						<b>A:</b> Detune und Resample sind sich relativ �hnlich. Beide
								  geben eingehende Samplewerte mit einer andern
								  Geschwindigkeit aus, und �ndern somit auch die L�nge des
								  Samples w�hrend sie die Tonh�he variieren.<br>
								  Der Unterschied hierbei ist, da� Resample dies mit einer
								  konstanten Rate macht (z.B. 3 Ausgangswerte f�r 2
								  Eingangswerte erzeugt), w�hrend die Rate bei Detune
								  variabel ist.
								  Resample bietet zus�tzlich ein paar Funktionen, um
								  ungew�nschte Nebeneffekte (Aliasing) zu vermeiden.
								  Um dies zu illustrieren ein Beispiel : stellen Sie sich
								  folgendes Sample vor<br>
								  +-+-+-+-<br>
								  wobei + eine positive maximale Amplitude und - eine negative
								  maximale Amplitude darstellt.
								  Jetzt rechnen wir das um den Faktor 2 herunter (wir
								  �berspringen jeden 2-ten Wert) und erhalten<br>
								  ++++<br>
								  Die hohe Frequenz hat sich ausgel�scht bzw. ist sogar
								  ein Gleichspannungoffset geworden. Und es wird sogar noch
								  schlimmer, wenn man gebrochenen Faktoren (z.B. 1.5) benutzt.
								  Dann w�rde man etwas wie folgendes bekommen<br>
								  +--+-<br>
								  Es gibt da noch einen einfachen Weg diesen Effekt zu sehen
								  und zu h�ren. Benutzen sie Synthesize-Add um einen
								  Sinusverlauf von 1000 Hz bis z.B. 100000 Hz zu erzeugen.
								  W�hlen sie dabei eine Samplingrate von z.B. 44100 Hz.
								  Das Ergebnis sollte ein Ton wie puuuuuiiiiiiieeeee ;-)
								  sein. Es wird sich allerdings mehr nach puuuuiiiiieeeeeiiieeeeuuueeeeiii.
								  anh�ren.<br>
								  Benutzen Sie Analyse-Spect-2D um es darzustellen und
								  Sie werden verstehen warum.<br><br>
	
								  In der Praxis verwendet man Resample, wenn eine Aufnahme mit
								  z.B. 20050 Hz hat, diese aber in einer Software einsetzen
								  m�chte die 44100 Hz verlangt, da Resample die Aufnahme so
								  ver�ndert das sie bei 44100 nun genauso klingt wie zuvor bei
								  20050 Hz.<br>
								  Im Gegensatz dazu dient Detun f�r Effekt wie die Simulation
								  eines Stromausfalles oder Alien-Stimmen.<br><br>

								  Nun also noch zu PitchShift. Dessen Spezialit�t ist es die
								  Tonh�he zu ver�ndern ohne die L�nge zu beeinflussen, indem
								  es reichlich Magie einsetzt (intelligentes Wiederholen oder
								  Auslassen kurzer Samplefragmente).
								  �hnlich dazu gibt es noch einen Effekt, welcher fr�her oder
								  sp�ter (ich hoffe eher) auch mal in __SFX erscheinen wird.
								  Er hei�t Timestretch und macht ein Sample l�nger oder k�rzer
								  ohne die Tonh�he zu beeinflussen. Grunds�tzlich ist es ein
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
