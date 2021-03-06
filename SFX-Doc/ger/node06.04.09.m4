divert(-1)
define(`__TITLE',`Operatoren : Filters, EQ')
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
						<b>F:</b> Wie kann ich die Frequenzbandzuordnung im <a href="node02.01.06/Equalize-FFT.html">Equalize-FFT</a> Operator ?ndern?
								  Das erste Band reicht bis zu 648 Hz und das letzte bis zu 22050 hz. Das n?tzt mir nicht viel.<br>
						<b>A:</b> Der aktuelle Equalizer basiert aft dem fft Algorithmus. Dieser spaltet den gesamten Frequenzbereich in feste Teile.
								  Dieser gesamte Bereich erstreckt sich von 0 Hz bis Samplefrequenz/2.<br>
								  In einer zuk?nftigen Version plane ich einen voll-parametrischen Equalizer anzubieten (n mittlere B?nder mit
								  editierbarer Aussteuerung und Breite plus ein Tiefpass und ein Hochpass mit editierbarer Aussteuerung und Cut-Off Frequenz).<br><br>

						<b>F:</b> Ich schaffe es nicht einen TB303 ?hnliche Effekt auf einen Rythmus anzuwenden. Bei diesem wandert ein schriller Klang von Tief nach Hoch -
								  das ist ein klassischer Sound in vielen Trance und House Songs, wo der ganze Rhythmus resoniert.<br>
								  Bekommt man das mit __SFX hin? <br>
						<b>A:</b> Ich denke schon. Dazu empfehle ich den <a href="node02.01.06/Filter-StateVariable.html">Filter-StateVariable</a>, da dieser schnell und trotzdem m?chtig ist.
								  Ein Filter hat drei wichtige Parameter : <ol>
									<li> Modell : Tiefpass, Hochpass, Bandpass, Bandsperre, ... - f?r diesen Fall brauchen wir einen Tiefpass</li>
									<li> die Cut-Off Frequenz : dies ist die Frequenz bei der die Laust?rke schon um 3db abgefallen ist</li>
									<li> die Resonanz : diese betont Frequenzen um die Cut-Off-Frequenz herum</li>
								  </ol>
								  __SFX erlaubt es Ihnen die meisten Parameter zu modulieren, also nicht nur unver?nderliche Werte
								  wie in anderen Programmen anzugeben. Sowohl Cut_off als auch die Resonanz sind also ver?nderbar.
								  Dazu gibt es die BlendShapes. Man w?hlt im Prinzip einfach einen unteren und einen oberen Wert aus und
								  das BlendShape wechselt zwischen diesen (siehe <a href="node02.01.02.html">__NODETITLE(`node02.01.02.html')</a>.<br>
						<b>A:</b> (Jan Krutisch) Meiner Meinung nach ist es gut mit dem <a href="node02.01.06/Filter-StateVariable.html">Filter-StateVariable</a> (wie Stefan empfohlen hat)
								  und einer Modulation der Cut-Off-Frequenz durch die Signalamplitude zu beginnen. Da ich __SFX seit einer
								  Weile nicht mehr genutzt habe, kann ich mich jedoch nicht ganz genau erinnern wie das funktioniert.
								  Auf alle F?lle muss man die Modulation auf "User" stellen. Dann w?hlt man noch zwischen Amplituden und Frequenz-
								  modulation. Das Einzige was nun noch bleibt ist den Bereich f?r die Cut-Off Frequenz vern?nftig zu setzen
								  (experimentieren !!!) und die Resonanz hoch zu drehen. Voila! Kraftvolle Filteranschl?ge.<br><br>

						<b>F:</b> Gibt es eine M?glichkeit die Aufl?sung der FFT zu erh?hen, damit man dann
								  hineinzoomen kann um exakte Spitzen zu finden?<br>
						<b>A:</b> Nicht so richtig. Die ist leider eine Beschr?nkung der FFT. Wenn sie nur an tiefen Frequenzen
								  interessiert sind gibt es jedoch einen Trick. Schicken sie das Signal durch einen Tiefpassfilter
								  und Resamplen sie es (daf?r k?nnen sie den eingebauten Aliasing-Filter des <a href="node02.01.06/Resample.html">Resample</a> Operators
								  verwenden). Dann starten sie <a href="node02.01.06/AnalyseSpect-2D.html">AnalyseSpect-2D</a>.<br>
								  Au?erdem k?nnte ich versuchen einen Spektral-Analyser auf der Basis von Bandpassfiltern zu bauen.
								  Dieser k?nnte dann (nahezu) unendlich tief in das Signal hineinzoomen.<br><br>

						<b>F:</b> Wenn ich ein Rauschsignal von 1 Sekunde mit dem <a href="node02.01.06/Noise.html">Noise</a> Operator erzeuge und dann dar?ber eine
								  Spektral-Analyse erstelle, bekomme ich ein Resultat welches alles andere als flach ist. Was stimmt hier nicht,
								  der Rausch-Algorithmus oder die FFT?<br>
						<b>A:</b> Das Spektrogramm kann nicht perfekt flach sein. je nach Qualit?t des Zufallsgenerators erh?lt
								  man ein mehr oder weniger perfektes Rauschen.<br><br>

						<b>F:</b> Ich habe ein Sample von gr??erer Entfernung aufgenommen und m?chte nun dessen Lautst?rke
								  anheben. Wenn ich dies tue bekomme ich einen st?renden Hintergrundklang mit.
								  Es sieht jedoch so aus, als ob das ger?usch oberhalb von 14kHz liegt.
								  Da meine Aufnahme nur Sprache enth?lt, k?nnte man da das Ger?usch nicht einfach
								  mit __SFX rausschneiden.<br>
						<b>A:</b> Ich gehe mal davon aus, das Sie die Aufnahme mit 16bit und 44100/48000 Hz durchgef?hrt haben.
								  Dann w?rde ich empfehlen einen Tiefpassfilter einzusetzen. Da sie eine gute
								  Ausl?schung des St?rsignals wollen, sollten sie den <a href="node02.01.06/Filter-FIRLowPass.html">Filter-FIRLowPass</a> (und nicht den
								  <a href="node02.01.06/Filter-StateVariable.html">Filter-StateVariable</a>) verwenden. Starten sie den Filter und stellen sie 13000 hz f?r die
								  Cut-Off Frequenz ein, setzen sie die Modulation auf "None", da Sie ja keine k?nstlerischen
								  Effekte erzeugen wollen. Der Parameter "Number" sollte z.B. 64 sein.<br>
								  Benutzen sie <a href="node02.01.06/AnalyseSpect-2D.html">AnalyseSpect-2D</a> nach der Operation, um sicherzustellen, das die hohen Frequenzen
								  ausgel?scht worden sind. Sie k?nnen den Filter auch mehrfach anwenden um die Steilheit und
								  die D?mpfung zu verbessern.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
