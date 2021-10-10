divert(-1)
define(`__TITLE',`Allgemeines')
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
					<p align="justify">Starten Sie __SFX durch Doppelklicken auf das __SFX Icon oder
					durch Aufrufen aus der Shell.
					Danach wird ein Fenster angezeigt, welches Sie &uuml;ber die einzelnen
					Phasen des Startvorganges informiert.</p>
					<p align="justify">Wenn der Ladevorgang beendet ist, erscheint der __SFX
					Bildschirm. Auf diesem finden sämtliche Programmaktionen statt. Dieser
					Bildschirm ist ein Public-Screen (&ouml;ffentlicher Bildschirm), d.h. auch
					andere Programme können auf diesem ihre Fenster öffnen. Der PublicScreen
					Name lautet "SFX_PubScreen". In jedem Fenster kann durch Druck auf die "Help"
					Taste die Online-Hilfe aktiviert werden. Am oberen Bildschirmrand finden
					sie die Bildschirmleiste:
					<center>
						__IMAGE(`scr_title')
					</center>
					Neben Programmname und Versionsnummer finden sie hier ebenfalls Informationen
					über den aktuellen Speicherverbrauch.</p>
					<p align="justify">Beim erstmaligen Starten dauert es etwas länger, da hier
					Indexdateien f&uuml;r die OnLine-Hilfe und Datenbankfiles f&uuml;r die externen
					<a href="node02.__.__.html">Module</a> erstellt werden. Bei weiteren Starts
					werden diese Dateien nur neugeneriesrt, wenn sich an der Installation etwas
					ge&auml;ndert hat.
					</p>
					<p align="justify">Wenn sie __SFX aus der Shell heraus starten, können sie
					Sampledateien als Argumente angeben, die dann mitgeladen werden. Weiterhin können sie
					__SFX auch als Default Tool in Icons von Sounddateien eintragen.
					Bei einem Doppelklick auf ein solches Icon, werden __SFX und die Sounddatei geladen.
					Falls __SFX schon läuft werden neue Dateien einfach hinzugeladen.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
