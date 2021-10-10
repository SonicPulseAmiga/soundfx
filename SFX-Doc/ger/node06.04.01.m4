divert(-1)
define(`__TITLE',`Features')
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
						<b>F:</b> Wird __SFX virtuellen Speicher unterstützen?<br>
						<b>A:</b> Ja, dieser ist seit V 3.70 verfügbar.<br><br>

						<b>F:</b> Wird __SFX den DSP der Delfina Soundkarten unterstützen?<br>
						<b>A:</b> Höchstwarscheinlich nie, da mir dafür die Zeit fehlt.<br><br>

						<b>F:</b> Wird es __SFX mit PPC-Unterst&uuml;tzung geben?<br>
						<b>A:</b> Ich versuche dies zu realisieren, möchte aber nicht zu viel versprechen. Grundvoraussetzung hierfür ist das ich mir einen modernen PPC-Amiga kaufen kann.<br><br>

						<b>F:</b> Wird __SFX MPEG Dateien unterstützen? Wird __SFX RealAudio Dateien unterstützen?<br>
						<b>A:</b> MPEG kann geladen und gespeichert werden. Mit RealAudio sieht es da eher schlecht aus.<br><br>

						<b>F:</b> Wird __SFX in naher Zukunft aufnehmen können?<br>
						<b>A:</b> Ab der Version 4.00 ist dies realisiert.<br><br>

						<b>F:</b> Wird es __SFX für Windows/Linux/MorphOS/... geben?<br>
						<b>A:</b> Sowas ist absolut nicht so einfach wie es manchem erscheint. Wenn da was in Arbeit ist werde ich dies bekannt geben.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
