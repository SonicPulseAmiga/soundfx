divert(-1)
define(`__TITLE',`Bitrate')
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
					<p align="justify">Die Bitrate gibt an wieviele Bits pro Sekunde für einen Klang benötigt werden.
					Mittels dieser Einheit sieht man z.B. welcher Datendurchsatz erforderlich ist um
					eine Datei von Festplatte oder vom Internet abzuspielen. Kompressionsverfahren
					können die Bitrate einer Audiodatei erheblich verringern. Folgende Tabelle gibt
					einen Überblick über Formate und deren Bitraten :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Format</th><th>Bitrate</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td><nobr>PCM, 8bit,22050Hz,mono  </nobr></td><td>172.265.. kbit/s</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td><nobr>PCM,16bit,44100Hz,mono  </nobr></td><td>689.0625 kbit/s</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td><nobr>PCM,16bit,44100Hz,stereo</nobr></td><td>1378.125 kbit/s</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td><nobr>MP3,16bit,44100Hz,stereo</nobr></td><td>z.B. 128.0 kbit/s</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td><nobr>RealAudio,16bit,22050Hz,mono</nobr></td><td>z.B. 32.0 kbit/s</td></tr>
					</table>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
