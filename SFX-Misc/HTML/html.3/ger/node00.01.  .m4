divert(-1)
define(`__TITLE',`Was ist SoundFX ?')
define(`__NODE',__file__)
include(`global.m4')
include(`header.m4')
include(`footer.m4')
include(`navbar.m4')
divert`'dnl
<html>
	__HEADER
	<body __PAGE_STYLE>
		__NAVBAR
		<!-- Contents -->
		<a name="#001">
		<table border="1" width="100%" cellpadding="2" cellspacing="0">
			<tr bgcolor="#3355AA"><td>
				<table border="0" width="100%" cellpadding="0" cellspacing="0"><tr>
					<td align="left">
						<b>__CHAPTER`'__TITLE</b>
					</td>
					<td align="right">
						<a href="#000"><img src="../_img/a_up.png" width="13" height="13" border="0"></a><img src="../_img/space.gif" width="2">
						<a href="#002"><img src="../_img/a_do.png" width="13" height="13" border="0"></a>
					</td>
				</tr></table>
			</td></tr>
			<tr bgcolor="#001177"><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify"><b>SoundFX</b> (folgend "SFX") ist ein Editor für digitalisierte Audiodaten (Samples).
					SFX ist modular aufgebaut und besitzt eine komfortable grafische Benutzeroberfläche.
					Mit SFX lassen sich Samples mit digitalen Effekten (welche einzigartig auf dem Amiga
					sind) versehen und nachbearbeiten.<br>
					Folgend nun eine Übersicht der Features des Programms :<ul>
						<li>mehr als 50 @{" Operatoren " link operators-index.guide/main } mit vielen Parametern und Modulationsmöglichkeiten wie z.B.:<ul>
							<li>Soundsynthesefunktionen<ul>
								<li>AM-Synthese (Amplitudenmodulation)</li>
								<li>CS-Synthese (Compositesynthese=Additative und Subtraktive Soundsynthese)</li></ul></li>
							<li>3D-Cube-Parametermodulation (Mix, Equalize)</li>
							<li>Effekte wie Hall, Echo, Delay, Chorus/Phaser, Morph, Pitchshift, ...</li>
							<li>Operatoren wie Resample, ZeroPass (FadeIn/FadeOut), Middle, Amplify, Mix, DeNoise, ConvertChannels ...</li>
							<li>2D/3D-Spectrumanalyse</li>
							<li>sehr gute Filter und Booster - resonanzfähig !!!</li>
							<li>viele, viele Modulationsmöglichkeiten<ul>
								<li>sogar Lautstärke und Frequenzverfolgung</li></ul></li>
							<li>mehr als 100 Presets werden  mitgeliefert</li></ul></li>
						<li>interne Signalauflösung von 32/16bit<ul>
							<li>32bit/Fließkomma während der Effektberechnung</li>
							<li>16bit im Samplepuffer (für Sounddaten völlig ausreichend)</li></ul></li>
						<li> hervorragende @{" Playroutinen " link players-index.guide/main }<ul>
							<li>8 bit Standart Player</li>
							<li>14 bit Cascade Player (ohne zusätzliche Hardware)</li>
							<li>14 bit kallibrierter Cascade Player (ohne zusätzliche Hardware)</li>
							<li>AHI-Player für Soundkarten</li>
							<li>Player spielen 16bit Samples direkt aus dem Fastram oder von HD ab und verbrauchen max 16 kByte Chipram während des Abspielens</li></ul></li>
						<li>Konvertierung verschiedener Soundsampleformate<ul>
							<li>IFF-8SVX/16SV/AIFF/AIFC/MAUD,RAW,RIFF-WAV,VOC,SND-AU,...</li>
							<li>mit Kompressionsunterstützung</li></ul></li>
						<li>arbeitet nun auch mit Samples, größer als der verfügbare Arbeitsspeicher</li>
						<li>arbeitet in Mono, Stereo und Quadro !!!</li>
						<li>Operatoren sind non-destructive, d.h. das Ausgangssample wird nicht überschrieben oder gelöscht</li>
						<li>umfangreiche Schnittfunktionen zur destruktiven Bearbeitung</li>
						<li>Freihandeditierung</li>
						<li>flexible Bildschirmdarstellung<ul>
							<li>beliebig viele Samplepuffer (nur vom System (RAM) begrenzt)</li>
							<li>jedes Sample hat eigenes Fenster, mit beliebiger Position und Größe</li>
							<li>stufenlos variables Zooming (auch <1.0)</li>
							<li>X- und Y-Zoom !!</li>
							<li>und Lineale mit konfigurierbaren Einheiten</li></ul></li>
						<li>HTML oder AmigaGuide-OnLine-Hilfe<ul>
							<li>durch drücken der "HELP"-Taste in jedem Fenster</li>
							<li>Asynchron (das Hilfe Fenster kann geöffnet bleiben)</li></ul></li>
						<li>Clippboard Unterstützung mit allen 256 Einträgen</li>
						<li>DataType Unterstützung (Loader)</li>
						<li>@{" ARexx-Port " link sfx-arexx.guide/Charpter3.1 }</li>
							<li>mit vielen Befehlen und Funktionen (z.Z. ca. 90)</li>
							<li>mit mehreren Beispielen</li>
							<li>ARexx-Scripts können direkt von SoundFX aus gestartet werden</li></ul></li>
						<li>systemkonforme-grafische-Benutzeroberfläche</li>
						<li>Font- und Screensensitiv</li>
						<li>modulares Konzept, d.h. beliebig viele<ul>
							<li>@{" Player         " link players-index.guide/main   }	(z.Z.  4)</li>
							<li>@{" Loader         " link loaders-index.guide/main   }	(z.Z. 17)</li>
							<li>@{" Saver          " link savers-index.guide/main    }	(z.Z. 13)</li>
							<li>@{" Operators      " link operators-index.guide/main }	(z.Z. 56)</li>
							<li>@{" ArexxOperators " link rexx-index.guide/main      }	(mehrere scripts mitgeliefert)</li></ul></li>
					</ul>
					In der <a href="node04.html">unregistrierten Version</a> können Sie ihre Samples nicht abspeichern und den ARexx-Port nicht benutzen !
					</p>
				</td></tr></table>
			</td></tr>
		</table>
		<br>
		<a name="#002">
		<table border="1" width="100%" cellpadding="2" cellspacing="0">
			<tr bgcolor="#3355AA"><td>
				<table border="0" width="100%" cellpadding="0" cellspacing="0"><tr>
					<td align="left">
						<b>Inhalt</b>
					</td>
					<td align="right">
						<a href="#001"><img src="../_img/a_up.png" width="13" height="13" border="0"></a><img src="../_img/space.gif" width="2">
						<a href="#003"><img src="../_img/a_do.png" width="13" height="13" border="0"></a>
					</td>
				</tr></table>
			</td></tr>
			<tr bgcolor="#001177"><td>
				<table border="0" cellpadding="4" cellspacing="0"><tr><td><table border=0 cellpadding="0">
					<tr><td><b>0</b></td><td><img src="../_img/space.gif" width="10"><a href="node00.  .  .html"><b>Einf&uuml;hrung</b></a></td></tr>
					<tr><td>   0.1  </td><td><img src="../_img/space.gif" width="20"><a href="node00.01.  .html">Was ist SoundFX ?</a></td></tr>
					<tr><td>   0.2  </td><td><img src="../_img/space.gif" width="20"><a href="node00.02.  .html">Wo l&auml;uft SoundFX ?</a></td></tr>
					<tr><td>   0.3  </td><td><img src="../_img/space.gif" width="20"><a href="node00.03.  .html">Copyright</a></td></tr>
					<tr><td>   0.4  </td><td><img src="../_img/space.gif" width="20"><a href="node00.04.  .html">Registration / Autor</a></td></tr>
					<tr><td><b>1</b></td><td><img src="../_img/space.gif" width="10"><a href="node01.  .  .html"><b>Bedienung</b></a></td></tr>
					<tr><td>   1.1  </td><td><img src="../_img/space.gif" width="20"><a href="node01.01.  .html">Allgemeines</a></td></tr>
					<tr><td>   1.2  </td><td><img src="../_img/space.gif" width="20"><a href="node01.02.  .html">Men&uuml;</a></td></tr>
					<tr><td>   1.3  </td><td><img src="../_img/space.gif" width="20"><a href="node01.03.  .html">Toolbars</a></td></tr>
					<tr><td>   1.3.1</td><td><img src="../_img/space.gif" width="30"><a href="node01.03.01.html">Loader</a></td></tr>
					<tr><td>   1.4  </td><td><img src="../_img/space.gif" width="20"><a href="node01.04.  .html">Statusbar</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		</table>
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
