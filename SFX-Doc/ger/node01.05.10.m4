divert(-1)
define(`__TITLE',`Stapelverarbeitungsfenster')
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
						__IMAGE(`win_batch')
					</center>
					<br>
					<p align="justify">
					Die Stapelverarbeitung erm&ouml;glicht es ihnen eine Reihe von Operationen (Laden, Bearbeiten, Speichern) auf ein komplettes Verzeichnis mit Samples auszuf&uuml;hren.
					Damit können sie eine Reihe von Vorgängen auf viele und/oder lange Dateien automatisch anwenden lassen.<br>
					Schauen sie sich die mitgelieferten Vorlagen als mögliche Anwendungsbeispiele an.
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Bereich        						</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Batch List				</td><td>Diese Liste besteht immer aus einem <a href="node02.02._ .html">Loader</a> und einem <a href="node02.05.__.html">Saver</a>.
																									Zwischen diesen k&ouml;nnen sie nun beliebig viele <a href="node02.01.__.html">Effekte</a> einf&uuml;gen.
																									Jeder Operation kann au&szlig;erdem noch ein Preset zugeordnet werden.
																								</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Options					</td><td>Hier k&ouml;nnen sie w&auml;hlen, ob __SFX die Ausf&uuml;hrung mitprotokollieren soll und wenn ja in welche Datei.<br>
																									Weiterhin ist es m&ouml;glich anzugeben, das __SFX rekursiv in Unterverzeichnisse hinabsteigen soll.
																									Eine Tiefe von &quot;-1&quot; bedeutet dabei &quot;unbeschr&auml;nkte Tiefe&quot;.
																									Dies bewirkt das alle Dateien bearbeitet werden.<br>
																									Letztendlich kann __SFX die Ausgangsdateien nach der Bearbeitung löschen.
																									Dies spart Ihnen Platz auf der Festplatte, sie sollten die Dateien aber irgendwo anders gesichert haben.
																								</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Presets					</td><td>Analog zur <a href="node02.01.05.html">Presetauswahl</a> in den Operatorfenstern, k&ouml;nnen sie hier,
																									die auf der linken Seite vorgenommenen Einstellungen abspeichern und schnell wieder aufrufen.
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
