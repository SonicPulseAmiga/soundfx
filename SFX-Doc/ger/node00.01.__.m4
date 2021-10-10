divert(-1)
define(`__TITLE',`Was ist SoundFX ?')
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
					<p align="justify">__SFX ist ein Editor f�r digitalisierte Audiodaten (Samples).
					__SFX ist modular aufgebaut und besitzt eine komfortable grafische Benutzeroberfl�che.
					Mit __SFX lassen sich Samples mit digitalen Effekten (welche einzigartig auf dem AMIGA
					sind) versehen und nachbearbeiten. Stellen Sie sich __SFX einfach als schweizer Taschenmesser
					f�r Sounds vor!<br>
					Folgend nun eine �bersicht der Features des Programms :<ul>
						<li>mehr als 65 <a href="node02.01.__.html">Operatoren</a> mit vielen Parametern und Modulationsm�glichkeiten wie z.B.:<ul>
							<li>Soundsynthesefunktionen<ul>
								<li>AM-Synthese (Amplitudenmodulation)</li>
								<li>CS-Synthese (Compositesynthese=Additative und Subtraktive Soundsynthese)</li></ul></li>
								<li>FM-Synthese (Frequenzmodulation)</li>
							<li>3D-Cube-Parametermodulation (Mix, Equalize)</li>
							<li>Effekte wie Hall, Echo, Delay, Chorus/Phaser, Morph, Pitchshift, Timestretch ...</li>
							<li>Operatoren wie Resample, ZeroPass (FadeIn/FadeOut), Middle, Amplify, Mix, DeNoise, ConvertChannels ...</li>
							<li>2D/3D-Spektralanalyse</li>
							<li>sehr gute Filter und Booster - resonanzf�hig !!!</li>
							<li>viele, viele Modulationsm�glichkeiten<ul>
								<li>sogar Lautst�rke und Frequenzverfolgung</li></ul></li>
							<li>mehr als 250 Presets werden  mitgeliefert</li></ul></li>
						<li>interne Signalaufl�sung von 80/16 bit<ul>
							<li>80 bit Flie�komma w�hrend der Effektberechnung</li>
							<li>16 bit im Samplepuffer</li></ul></li>
						<li>gute <a href="node02.03.__.html">Playroutinen</a><ul>
							<li>8 bit Standard Player</li>
							<li>14 bit Cascade Player (ohne zus�tzliche Hardware)</li>
							<li>14 bit kallibrierter Cascade Player (ohne zus�tzliche Hardware)</li>
							<li>AHI-Player f�r Soundkarten</li>
							<li>Player spielen Samples direkt aus dem Fastram oder von HD ab und verbrauchen max 16 kByte Chipram w�hrend des Abspielens</li></ul></li>
						<li>Konvertierung verschiedener Soundsampleformate<ul>
							<li>IFF-8SVX/16SV/AIFF/AIFC/MAUD,RAW,RIFF-WAV,VOC,SND-AU,...</li>
							<li>mit Kompressionsunterst�tzung</li></ul></li>
						<li>arbeitet nun auch mit Samples, gr��er als der verf�gbare Arbeitsspeicher</li>
						<li>arbeitet in Mono, Stereo und Quadro !!!</li>
						<li>Operatoren sind nicht-destruktiv, d.h. das Ausgangssample wird nicht �berschrieben oder gel�scht</li>
						<li>umfangreiche Schnittfunktionen zur destruktiven Bearbeitung</li>
						<li>Freihandeditierung</li>
						<li>flexible Bildschirmdarstellung<ul>
							<li>beliebig viele Samplepuffer (nur von den Systemresources begrenzt)</li>
							<li>jedes Sample hat eigenes Fenster, mit beliebiger Position und Gr��e</li>
							<li>stufenlos variables Zooming (auch <1.0)</li>
							<li>X- und Y-Zoom !!</li>
							<li>und Lineale mit konfigurierbaren Einheiten</li></ul></li>
						<li>HTML OnLine-Hilfe<ul>
							<li>durch dr�cken der "HELP"-Taste in jedem Fenster</li>
							<li>Asynchron (das Hilfe Fenster kann ge�ffnet bleiben)</li></ul></li>
						<li>Clippboard Unterst�tzung mit allen 256 Eintr�gen</li>
						<li>DataType Unterst�tzung (Loader)</li>
						<li><a href="node03.__.__.html">ARexx-Port</a><ul>
							<li>mit vielen Befehlen und Funktionen (z.Z. ca. 90)</li>
							<li>mit mehreren Beispielen</li>
							<li>ARexx-Scripts k�nnen direkt von __SFX aus gestartet werden</li></ul></li>
						<li>systemkonforme grafische Oberfl�che</li>
						<li>Font- und Screensensitiv</li>
						<li>modulares Konzept, d.h. beliebig viele<ul>
							<li><a href="node02.01.__.html">Operatoren</a> (z.Z. 65)</li>
							<li><a href="node02.02.__.html">Loader</a> (z.Z. 19)</li>
							<li><a href="node02.03.__.html">Player</a> (z.Z.  4)</li>
							<li><a href="node02.04.__.html">Rexx-Makros</a> (mehrere Skripte mitgeliefert)</li>
							<li><a href="node02.05.__.html">Saver</a> (z.Z. 15)</li></ul></li>
						<li>Unterst�tzung AMIGA-spezifischer Funktionen<ul>
							<li>Dateiformatinformationen in Filenotes</li>
							<li>Erzeugung von Projekt-Piktogrammen</li>
							<li>Applikation-Icon</li></ul></li>
					</ul>
					In der <a href="node00.04.__.html">unregistrierten Version</a> k�nnen Sie ihre Samples nicht abspeichern und den ARexx-Port nicht benutzen !
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
