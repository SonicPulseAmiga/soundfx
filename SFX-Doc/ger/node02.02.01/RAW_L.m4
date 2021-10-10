divert(-1)
define(`__TITLE',`RAW_L')
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
	__SECTION_HEADER(RAW_L)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Läd RAW Samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
</table>
RAW-Sample haben eigentlich gar kein Format. Hier werden nur die "rohen"
Sounddaten abgespeichert. Das hat den Vorteil, daß dieses Format sehr einfach
zu handhaben ist, aber auch den Nachteil, daß keinerlei zusätzliche Daten wie
Samplingrate, Loops, Bitauflösung usw. gespeichert werden können. __SFX
versucht daher zumindestens die Bitauflösung, den Vorzeichentyp und das
Endianformat (16 bit) zu erkennen.<br>
Als neues Feature ab V 3.70 können, sie den RAW-Loader nun selbst programmieren.
Wenn Sie also z.B. oft mit Daten von Audio-CD's arbeiten, nennen Sie diese Dateien
beispielsweise "<name>.cdda". Um den Loader zu programmieren, stellen Sie im
RAW-Loader auf der linken Seite alle Parameter wie folgt ein ein :
<table border="0">
<tr><td>Type</td><td>=PCM16</td></tr>
<tr><td>Endian</td><td>=no</td></tr>
<tr><td>Sign</td><td>=signed</td></tr>
<tr><td>Channel</td><td>=mono/stereo interleaved</td></tr>
<tr><td>SRate</td><td>=44100</td></tr>
<tr><td>Offs</td><td>=0</td></tr>
<tr><td>Check File Type</td><td>Aus</td></tr>
</table>
und speichern dies unter "cdda.cfg". Jetzt erstellen Sie mit Add einen
neuen Typ auf der rechten Seite (dazu muß CheckFileType an sein) und
tragen bei "extension/header" ".cdda" ein. Dann clicken Sie auf das
PopUp-Symbol und wählen die "cdda.cfg" aus. Jedes mal wenn eine Datei
auf ".cdda" endet, wird nun die "cdda.cfg" verwendet.<br>
Wenn Sie nicht die Dateiendung, sondern den Inhalt testen möchten, verwenden
Sie statt einem "." als erstes Zeichen ein "#" (Bsp. "#ALAW").
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Type</nobr></td><td align="left">Art der Kompression
<ul>
<li>PCM8 : ungepackt 8bit</li>
<li>PCM16 : ungepackt 16bit</li>
<li>PCM24 : ungepackt 24bit</li>
<li>PCM32 : ungepackt 32bit</li>
<li>µ-Law : µ-Law (14:8) gepackt 14bit</li>
<li>µ-Law Inv : µ-Law (14:8) gepackt 14bit, mit gespiegelten Bits (ISDN-Master)</li>
<li>A-Law : A-Law (14:8) gepackt 14bit</li>
<li>A-Law Inv : A-Law (14:8) gepackt 14bit, mit gespiegelten Bits (ISDN-Master)</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Endian</nobr></td><td align="left"></ul>
ob eine Endiankonvertierung durchgeführt werden soll. Intel-Prozessor basierte
Systeme speichern 16bit Wörter umgekehrt und diese Option  korregiert das.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Sign</nobr></td><td align="left">ob das Sample als vorzeichenbehaftetes oder nicht-vorzeichenbehaftetes geladen werden soll
<ul>
<li>signed : Amiga, Sgi</li>
<li>unsigned : Mac, Atari, PC</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Channel</nobr></td><td align="left"></ul>
wieviele Kanäle geladen werden sollen und wie sie aufgebaut sind.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>SRate</nobr></td><td align="left">welche Samplingrate soll eingetragen werden.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Offs</nobr></td><td align="left">Wieviele Bytes am Anfang Übersrungen werden sollen (um einen Kopfblock bekannter Länge zu überspringen).
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Check File Type</nobr></td><td align="left">Ob der Loader die Fileextension überprüfen bzw. die Daten statistisch auf
ihr Format untersuchen und die Ladeparameter dementspechend anpassen soll.
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
Der off-Parameter dient <b>nicht</b> zum vorspringen im Sample, auch wenn das funktioniert.
Bei 16bit Samples muß man dann aber sicherstellen, das man nur eine gerade Anzahl an Bytes vorspringt.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
