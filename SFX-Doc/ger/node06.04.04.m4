divert(-1)
define(`__TITLE',`Installation')
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
						<b>F:</b> Wenn ich SoundFX starte sind alle Operatoren, Loader und Savers leer !<br>
						<b>A:</b> Installieren Sie unbedingt immer ein sfx-bin, ein sfx-doc und das sfx-data Archiv.
								  Wenn die Installation unkomplett ist wird __SFX nicht funktionieren.<br><br>

						<b>F:</b> Wenn ich __SFX installiere läuft das alles ganz schnell durch, das Verzeichnis ist jedoch hinterher leer.<br>
						<b>A:</b> Entpacken Sie die lzx-Archive mit '-x' *nicht* mit '-e'. Nur die Option '-x' erzeugt die volle Verzeichnisstruktur.<br><br>

						<b>F:</b> Ich habe Probleme mit der Installation.<br>
						<b>A:</b> Generell empfehle ich alle drei Archive in das gleiche Zielverzeichnis (z.B. RAM:) zu entpacken und dann zu installieren.
								  Wenn Sie gefragt werden, ob Dateien überschrieben werden sollen, ist es egal was sie antworten.
								  Diese Dateien sind in allen drei Archiven enthalten. Jetzt können Sie in einem Rutsch installieren.<br><br>

						<b>F:</b> Wenn ich eine neue Version installiere, startet __SFX als Demoversion. Mu&szlig; ich eine Upgradegebühr bezahlen?<br>
						<b>A:</b> Nein! Alle neuen Versionen sind für registrierte Nutzer frei. Weitere Zahlungen sind absolut freiwilliger Natur.
								  Damit __SFX ihr Keyfile leichter finden kann, kopieren Sie es am besten nach 'devs:keyfiles/' als 'sfx.key'.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
