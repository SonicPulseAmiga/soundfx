divert(-1)
define(`__TITLE',`Loaders')
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
						<b>F:</b> Es w&auml;hre prima, wenn es einen CDDA Loader und Saver in __SFX g&auml;be.
								  Wenn dann auch noch Motorola Byte Order CDDA Datei ünterstützen werden würden,
								  könnte man solche Dateien konvertieren.<br>
						<b>A:</b> All dies ist bereits möglich. __SFX&#39;s <a href="node02.02.01/RAW_L.html">RAW</a> Loader hat dazu eine nüntzliche Funktion -
								  die konfigurierbare Formaterkennung. Dies bedeutet, dass sie eine Dateiendung oder
								  ein Muster in der Datei mit einem Satz von Einstellungen verknüpfen können.<br>
								  Um nun CDDA Dateien automatisch richtig zu laden, erstellen sie folgende Einstellungen,
								  welche sie dann als cdda-Preset speichern :<br>
								    &nbsp;&nbsp; Format=16 bit signed<br>
								    &nbsp;&nbsp; Endian=Intel<br>
								    &nbsp;&nbsp; Channels=stereo interleaved<br>
								  und mit der Endung '.cdda' verknüpfen.
								  Dann aktivieren sie noch die Autoerkennung und speichern die Einstellungen als
								  'default.cfg'. Wenn sie nun Datei über den <a href="node02.02.01/Universal_L.html">Universal</a>-Loader oder <a href="node02.02.01/RAW_L.html">RAW</a>-Loader laden
								  werden CDDA Dateien korrekt erkannt und konvertiert.<br><br>

						<b>F:</b> Es währe schön, wenn man die Laufwerkseinheit und die Deviceeinstellungen des <a href="node02.02.01/CDDA-Direct_L.html">CDDA-Direct</a>
								  Loaders permanent speichern könnte.<br>
						<b>A:</b> Nehmen die einfach die Einstellungen vor und speichern sie dies als 'default.cfg'.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
