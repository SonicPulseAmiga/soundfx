divert(-1)
define(`__TITLE',`Benutzung')
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
						<b>F:</b> Wir kann ich in einem Sample den markierten Bereich abschalten,
								  ohne etwas kopieren oder ausschneiden zu müssen.<br>
						<b>A:</b> Benutzen Sie das Bereichs-Menü oder den shortcut Amiga-H.<br><br>

						<b>F:</b> Ich bin mir nicht ganz klar, wozu wir die Höhe eines Bereiches
								  beim Markieren kontrollieren können. Kann man nur die Spitzen abschneiden?<br>
						<b>A:</b> Die wird derzeit zum Zoomen verwendet. Sie können so z.B. ziemlich grob
								  einen Bereich markieren und diese dann von __SFX ausdehnen lassen,
								  so daß die Spitzen optimal mit eingeschlossen sind und dann den
								  Bereich vergrößern.<br><br>

						<b>F:</b> Es währe prima, wenn es einen Shortcut zum Starten eines Operators gäbe
								  (also nicht Amiga-r, sondern etwas zum Starten der Berechnung im Operator).<br>
						<b>A:</b> Den gibt es. Nutzen sie "Enter/Return"<br><br>

						<b>F:</b> Wie kann ich ein komplettes Sample markieren?<br>
						<b>A:</b> Ebenfalls über das Bereichs-Menü oder über den Shortcut Amiga-A.<br><br>

						<b>F:</b> Ach ja die Del Taste. Ich bin es gewöhnt diese zum Ausschneiden zu verwenden, wie in
								  SoundForge, CoolEdit und auch in Textverarbeitungen.<br>
						<b>A:</b> Es währe sicherlich am Besten, wenn alle Shortcuts frei definierbar währen. 
								  Derzeit benutzt __SFX, wie alle guten Amiga-Programme, Amiga-X für die Funktion
								  "Ausschneiden".<br><br>

						<b>F:</b> Wenn du eine 600 Mb mit __SFX bearbeitest und dann aber nicht mehr genug Platz
								  für eine zweite 600 Mb Datei (oder eine dritte, oder vierte) hast, wie wird
								  das gehandhabt? Die Methode für jeden Effekt ein neues Sample anzulegen mag
								  ja bei kurzen Samples gut funktionieren, ist doch aber bei Großen
								  problematisch.<br>
						<b>A:</b> Zuersteinmal - kurze Samples - das ist genaus wofür __SFX eigentlich mal
								  geschaffen wurde. Da nun aber viele Nutzer danach fragten, lernte __SFX
								  auch mit größeren Samples klarzukommen.
								  Wenn kein Platz mehr im Speicher frei ist, versucht __SFX auf die Festplatte
								  auszulagern und wenn selbst dort kein Platz mehr ist, schlägt die Operation
								  fehl.<br>
								  Wenn sie alternative Ideen haben, immer her damit. Und nur um es vorwegzunehmen,
								  ich habe schon darüber nachgedacht, das Ausgangssample zu überschreiben.
								  Dies würde mit den meisten Effekten funktionieren, aber halt nicht mit Allen
								  und es währe manchmal auch etwas kompliziert zu handhaben.<br><br>

						<b>F:</b> Wenn man manche Fenster öffnet, werden die Samples nicht mehr neugezeichnet
								  wenn man deren Fenster in der Größe ändert. Wenn man also z.B. ein Einstellungsfenster
								  eines Loaders öffnet und dann die Größe eines Samplefensters ändert, wird das
								  Sample nicht neu gezeichnet. Erst wenn das Loader-Einstellungsfenster geschlossen wurde,
								  wird die Darstellung aktualisiert. Operatoren verursachen allerdings keine solchen
								  Probleme.<br>
						<b>A:</b> Die Operatoren werden als eigenständige Tasks gestartet, viele andere Fenster jedoch nicht.
								  Da ich diese also nicht asynchron starte, werden alle Ereignisse in anderen Fenstern verzögert,
								  bis das blockierenden Fenster geschlossen wird.
								  Ich bin mir nicht sicher ob es die Arbeit lohnt alle Fenster asynchron zu machen.<br><br>

						<b>F:</b> Werde ich mit dem neuen Batchprozessor rekursiv alle Dateien eines Verzeichnisses in
								  einem Rutsch z.B. nach WAV konvertieren können.<br>
						<b>A:</b> Ja, genau dazu dient er.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
