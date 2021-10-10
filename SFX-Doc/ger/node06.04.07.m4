divert(-1)
define(`__TITLE',`Operatoren : Amplitude, Dynamics')
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
						<b>F:</b> Ich m�chte mit wenigen Schritten eine H�llkurven-erkennung durchf�hren (z.B. als ARexx Script).
								  Wie kann ich ein Sample gleichrichten (also den negativen Teil nach oben spiegeln) - Nat�rlich
								  nicht nur in der Anzeige sondern in den Sampledaten. Dann w�rde ich das Signal mit einem
								  Tiefpass filtern - welche Cut-Off Frequenz soll ich da nehmen.<br>
						<b>A:</b> Der erste Teil ist ganz einfach. nehmen Sie <a href="node02.01.06/AmplifySplit.html">AmplifySplit</a>. Da k�nnen sie den oberen und unteren
								  Teil getrennt verst�rken. Somit sollten sie also "1.0" f�r den oberen Teil und "-1.0" f�r den
								  unteren Teil verwenden, um einfach den unteren Bereich zu invertieren. Dann wenden sie einen
								  Tiefpass mit einer CutOff-Frequenz von ungef�hr "150 hz" an.<br>
								  Eine andere Variante ist es das Signal mit einer verz�gerten Version von sich selbst zu mixen
								  (w�hlen sie ein Delay von "25 ms" im <a href="node02.01.06/Mix.html">Mix</a> Operator).<br>
								  Die <a href="node02.01.06/AmplifySplit.html">AmplifySplit</a> und LPF Kombination funktioniert jedoch einfach prima.
								  Am Besten sie verwenden dazu einen <a href="node02.01.06/Filter-StateVariable.html">Filter-StateVariable</a> (Cut-Off
								  zwischen "50" und "200 Hz" und Resonance=1). Mit diesen Werten bekommen sie sch�ne sanfte H�llkurven, die
								  sich prima zum Modulieren von anderen Effekten eignen.<br><br>


						<b>F:</b> Die H�llkurve die ich nun habe, liegt komplett im oberen Bereich des Samples. Ich ben�tige
								  sie aber �ber den kompletten Bereich.<br>
						<b>A:</b> Sie k�nnen den <a href="node02.01.06/Slide.html">Slide</a> Operator verwenden um sie um "50%" nach unten zu verschieben und dann
								  den <a href="node02.01.06/Amplify.html">Amplify</a> Operator um sie auf "200 %" zu verst�rklen. Wenn Sie dies dann in __SFX zur
								  Modulation verwenden, nehmen sie den Modulationsmodus "abs".<br>
								  Wesentlich einfacher geht es, wenn sie sich die H�llkurven von __SFX automatisch erzeugen lassen.
								  Wussten Sie schon das das geht ? W�hlen sie das "Blend-Shape=User", rufen dann die
								  Einstellungen auf, w�hlen dort das Quellsample (von dem Sie die H�llkurve �bernehmen wollen)
								  und aktivieren den "ModulationsTyp=AmpEnv" (AmplitudeEnvelope".<br><br>

						<b>F:</b> Wo liegt der Schwellwert, �ber welchen __SFX im Operator <a href="node02.01.06/Dynamic.html">Dynamic</a> entscheided, was leise
								  und was laute Samples sind? Kann Dieser gesetzt werden? <br>
						<b>A:</b> In �lteren SoundFX Versionen hie� dieser Operator "CompressorExpander". Ein solcher Effekt
								  ben�tigt einen Schwellwert zur Bearbeitung. Ich habe diesen Operator in __SFX als <a href="node02.01.06/Dynamic.html">Dynamic</a>
								  umbenannt, da er anders funktioniert. Die Ergebnisse sind allerdings �hnlich.<br>
								  Sie k�nnen einen Faktor f�r den lautesten und einen f�r den leisesten Samplewert einstellen.
								  F�r dazwischenliegende Werte interpoliert der Effekt den Faktor linear.<br><br>

						<b>F:</b> Wenn ich einen negativen Faktor f�r leise Samples angebe, werden diese dann bei 0 bleiben oder
								  schlagen sie auf die andere Seite um.<br>
						<b>A:</b> __SFX wird ihnen niemals einen Parameter abweisen, blo&szlig; weil er ungew�hlich ist. Desshalb
								  k�nnen sie auch soviele unterschiedliche Effekte mit ein und dem selben Operator erzeugen.<br>
								  Wenn sie nun einen negativen Wert f�r leise Samples eingeben, werden leise Sounds invertiert
								  und die interpolation wird zu den (m�glicherweise) positiven Werten f�r laute Sample �berblenden.<br><br>

						<b>F:</b> Als ich letztes Wochenende CDs gemixt habe, ist mir aufgefallen, das einige Titel leiser als
								  Andere sind. Kann __SFX diese auf volle Lautst�rke bringen? Genauer gesagt, kann es die leisen Titel
								  lauter machen und die Lauten eventuell etwas leiser.<br>
						<b>A:</b> Sie alle laut zu machen ist ganz einfach. Nehmen sie den <a href="node02.01.06/Amplify.html">Amplify</a> Operator daf�r. Einfach auf
								  MaxVol klicken, dann berechnet __SFX Ihnen die optimale Verst�rkung. Wenn sie gleich in
								  einem Rutsch mehrere Dateien bearbeiten wollen, nutzen sie doch die Stapelverarbeitung :<ol>
								    <li>Loader : Universal</li>
								    <li>Operator : Amplify, Preset : MaxVol</li>
								    <li>Saver : e.g. IFF-AIFC</li>
								  </ol>Dann auf Start klicken und noch das Quellverzeichnis mit den Dateien sowie das Zielverzeichnis
								  ausw�hlen.<br>
								  Wenn jedoch alle Datei schon die maximale Lautst�rke haben wird es etwas komplizierter.#
								  Sie m�ssten <a href="node02.01.06/Analyse-Data.html">Analyse-Data</a> verwenden und f�r jeden Titel die "RMS-Lautst�rke" aufschreiben.
								  Dann k�nnten sie die lauten Titel leiser machen (mit <a href="node02.01.06/Amplify.html">Amplify</a>) oder den <a href="node02.01.06/Dynamic.html">Dynamic</a> Operator
								  verwenden um die leisen Titel zu komprimieren (z.B. laute Werte=1.0 und leise Werte=1.5).<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
