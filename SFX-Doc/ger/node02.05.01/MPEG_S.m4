divert(-1)
define(`__TITLE',`MPEG_S')
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
	__SECTION_HEADER(MPEG_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Speichert hochkomprimierte MPEG Samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja</td></tr>
</table>
Wegen der hohen Kompression, kann das Abspeichern eine Weile dauern. Es
ist empfehlenswert mindestens einen 68060'er für soetwas zu besitzen.<br>
Dieses Modul benutzt externe Encoder (separate Programme).
Deshalb habe ich versucht es möglichst konfigurierbar zu halten.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Encoder</nobr></td><td align="left">Wählen sie die Programmdatei des zu verwendenten Encoders. Es wurden bisher
der mitgeliferte 8Hz, sowie Pegase, Lame und Ncoder getestet.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Parameter</nobr></td><td align="left">Dies ist die Parameter Vorlage, die dem Encoder auf der Kommandozeile übergeben wird
(der Encoder wird als Hintergrundprozess gestartet und vom __SFX mit Daten versorgt).
Folgende Platzhalter sind hier derzeit erlaubt :
<ul>
<li>%b : die Bitrate</li>
<li>%c : der Parameterstring für Mono/Stereodateien (siehe unten)</li>
<li>%i : der Name der Eingangsdatei</li>
<li>%o : der Name der Ergebnisdatei</li>
<li>%r0 : die Samplingrate in Hz</li>
<li>%r1 : die Samplingrate in kHz (zur Zeit nur 32, 44.1, 48)</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>MonoStr</nobr></td><td align="left"></ul>
Der Parameter für Monodateien welcher oben in Verbindung mit "%c" genutzt wird.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>StereoStr</nobr></td><td align="left">Der Parameter für Stereodateien welcher oben in Verbindung mit "%c" genutzt wird.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Wave</nobr></td><td align="left">Hiermit geben sie an, in welchem Format die Sampledaten dem Encoder übergeben
werden sollen.
<ul>
<li>CDDA</li>
<li>RIFF-WAV</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>BitRate</nobr></td><td align="left"></ul>
Stärke der Kompression. Gibt an, wieviele Bits pro Sekunde erlaubt sind.
Je niedriger der Wert ist, desto niedriger ist die Qualität.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Pipe</nobr></td><td align="left">Wenn sie Probleme mit dem pipe: Gerät haben, konnen sie auch eine Pipe
wie apipe: oder awnpipe: einsetzen.
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
Es werden presets für die gängigen Encoder mitgeliefert. Es ist jedoch noch
notwendig, das sie die Pfade der entsprechenden Encoder anpassen oder
die Programme in das sfx/_savers Verzeichnis unter dem entspechenden namen kopieren.<br>
Wenn sie Anpassungen für weitere Encoder erstellen möchten, empehle ich sich
die beliegenden Prests anzusehen.<br>
Der Saver benutzt ein Pipe Gerät um die Daten zu dem Encoder zu senden.
Bitte stellen sie sicher das die gewählte Pipe gemountet ist.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
