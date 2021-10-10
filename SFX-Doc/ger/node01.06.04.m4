divert(-1)
define(`__TITLE',`verschiedene Einstellungen')
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
				<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
					<center>
						__IMAGE(`prefs_misc')
					</center>
					<br>
					<p align="justify">In diesem Fenster finden Sie noch ein paar weitere Einstellungen. Folgend die
					Beschreibung der einzelnen Funktionen :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Gadget						</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Sig-Audio		</td><td>Wenn dieser Schalter angewählt ist, ertönt nach fertiger
																						Berechnung ein Signalton.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Sig-Screen		</td><td>Wenn dieser Schalter angewählt ist, wird nach fertiger
																						Berechnung der SFX-Screen nach vorne gebracht.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Real-Mem			</td><td>Soll der freie Speicher und der gr&ouml;&szlig;te verfügbare
																						Speicherblock in der Titelzeile angezeigt werden?</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Virt-Mem			</td><td>Soll der freie virtuelle Speicher (Platz auf ihrer Festplatte
																						im Auslagerungsverzeichnis) in der Titelzeile angezeigt werden?</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Background Logo	</td><td>Wenn abgehakt, wird ein SFX-Logo im Bildschirmhintergrund dargestellt.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Source Select	</td><td>In welcher Art und Weise die Auswahl der Samples (z.B. in Operator-Fenstern)
																						erfolgen soll.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Context Button	</td><td>Welche Maustaste __SFX für die Kontextmenüs verwenden soll</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Window Mode		</td><td>Der <a href="node01.03.10.html">Fenstermodus</a> in dem sich __SFX nach dem Start befindet</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Ask Exit		</td><td>Wie sich __SFX beim Beenden verhalten soll</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Logging			</td><td>Geben sie an was __SFX mitprotokollieren soll und wählen sie einen Pfad für das Logfile aus.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Save Icons		</td><td>Soll __SFX beim Speichern von Samples Icons anlegen?</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">DefTool			</td><td>Das DefaultTool ist das Programm welches gestartet wird, wenn sie das Dateiicon doppelklicken.
																							__SFX kann das DefaultTool vom Icon beibehalten, __SFX eintragen wenn kein Eintrag existiert oder immer __SFX eintragen.
																						</td></tr>
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
