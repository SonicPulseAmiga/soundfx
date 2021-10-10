divert(-1)
define(`__TITLE',`Samplingrate')
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
					<p align="justify"> Die Samplingrate gibt an, wieviele digitale Samplewerte pro Sekunde wiedergegeben werden.
					Die Einheit der Samplingrate ist Hz (Schwingungen pro Sekunde).
					Der Hälfte der Samplingrate (Nyquist-Frequenz), gibt die höchste Frequenz an, die in den Sampledaten erfaßt wird.
					Dafür gibt es eine einfache Erklärung: um eine Frequenz zu erkennen, braucht man eine Periode der Schwingung
					und das sind mindestens zwei Werte.<br>
					Da der Mensch maximal bis ca. 20 kHz hört, sind Sampling über 40 kHz normalerweise nicht notwendig.
					Folgend sind noch ein paar typische Samplingraten aufgeführt :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Samplingrate  </th><th>Anwendung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 8000 Hz</td><td>Soundkarten (typisch für SND-AU Samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">11025 Hz</td><td>Soundkarten (typisch bei alten Samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">22050 Hz</td><td>Soundkarten (typische Frequenz bei vielen Samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">28867 Hz</td><td>max. Abspielrate des Paulachips im normalen Modus</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">32000 Hz</td><td>Consumer DATs und Sampler</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">44100 Hz</td><td>CD-Player, Soundkarten</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">48000 Hz</td><td>DAT-Recorder/Player</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">57734 Hz</td><td>max. Abspielrate des Paulachips im Productivity-Modus</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">96000 Hz</td><td>professionelle Studiogräte</td></tr>
					</table>
					Die Amiga-Audiohardware unterstüzt eine Samplingrate bis ca. 28kHz unter
					normalen Bildschirmmodi und bis ca. 56kHz unter Bildschirmmodi mit verdoppelte
					DMA-Rate z.B. "Productivity" (Aktivieren Sie eine solche Auflösung nur dann,
					wenn Ihr Monitor das auch aushält oder sie eine Grafikkarte haben und am normalen
					Monitorausgang nix angeschlossen ist.).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
