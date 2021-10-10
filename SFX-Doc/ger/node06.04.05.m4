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
								  ohne etwas kopieren oder ausschneiden zu m�ssen.<br>
						<b>A:</b> Benutzen Sie das Bereichs-Men� oder den shortcut Amiga-H.<br><br>

						<b>F:</b> Ich bin mir nicht ganz klar, wozu wir die H�he eines Bereiches
								  beim Markieren kontrollieren k�nnen. Kann man nur die Spitzen abschneiden?<br>
						<b>A:</b> Die wird derzeit zum Zoomen verwendet. Sie k�nnen so z.B. ziemlich grob
								  einen Bereich markieren und diese dann von __SFX ausdehnen lassen,
								  so da� die Spitzen optimal mit eingeschlossen sind und dann den
								  Bereich vergr��ern.<br><br>

						<b>F:</b> Es w�hre prima, wenn es einen Shortcut zum Starten eines Operators g�be
								  (also nicht Amiga-r, sondern etwas zum Starten der Berechnung im Operator).<br>
						<b>A:</b> Den gibt es. Nutzen sie "Enter/Return"<br><br>

						<b>F:</b> Wie kann ich ein komplettes Sample markieren?<br>
						<b>A:</b> Ebenfalls �ber das Bereichs-Men� oder �ber den Shortcut Amiga-A.<br><br>

						<b>F:</b> Ach ja die Del Taste. Ich bin es gew�hnt diese zum Ausschneiden zu verwenden, wie in
								  SoundForge, CoolEdit und auch in Textverarbeitungen.<br>
						<b>A:</b> Es w�hre sicherlich am Besten, wenn alle Shortcuts frei definierbar w�hren. 
								  Derzeit benutzt __SFX, wie alle guten Amiga-Programme, Amiga-X f�r die Funktion
								  "Ausschneiden".<br><br>

						<b>F:</b> Wenn du eine 600 Mb mit __SFX bearbeitest und dann aber nicht mehr genug Platz
								  f�r eine zweite 600 Mb Datei (oder eine dritte, oder vierte) hast, wie wird
								  das gehandhabt? Die Methode f�r jeden Effekt ein neues Sample anzulegen mag
								  ja bei kurzen Samples gut funktionieren, ist doch aber bei Gro�en
								  problematisch.<br>
						<b>A:</b> Zuersteinmal - kurze Samples - das ist genaus wof�r __SFX eigentlich mal
								  geschaffen wurde. Da nun aber viele Nutzer danach fragten, lernte __SFX
								  auch mit gr��eren Samples klarzukommen.
								  Wenn kein Platz mehr im Speicher frei ist, versucht __SFX auf die Festplatte
								  auszulagern und wenn selbst dort kein Platz mehr ist, schl�gt die Operation
								  fehl.<br>
								  Wenn sie alternative Ideen haben, immer her damit. Und nur um es vorwegzunehmen,
								  ich habe schon dar�ber nachgedacht, das Ausgangssample zu �berschreiben.
								  Dies w�rde mit den meisten Effekten funktionieren, aber halt nicht mit Allen
								  und es w�hre manchmal auch etwas kompliziert zu handhaben.<br><br>

						<b>F:</b> Wenn man manche Fenster �ffnet, werden die Samples nicht mehr neugezeichnet
								  wenn man deren Fenster in der Gr��e �ndert. Wenn man also z.B. ein Einstellungsfenster
								  eines Loaders �ffnet und dann die Gr��e eines Samplefensters �ndert, wird das
								  Sample nicht neu gezeichnet. Erst wenn das Loader-Einstellungsfenster geschlossen wurde,
								  wird die Darstellung aktualisiert. Operatoren verursachen allerdings keine solchen
								  Probleme.<br>
						<b>A:</b> Die Operatoren werden als eigenst�ndige Tasks gestartet, viele andere Fenster jedoch nicht.
								  Da ich diese also nicht asynchron starte, werden alle Ereignisse in anderen Fenstern verz�gert,
								  bis das blockierenden Fenster geschlossen wird.
								  Ich bin mir nicht sicher ob es die Arbeit lohnt alle Fenster asynchron zu machen.<br><br>

						<b>F:</b> Werde ich mit dem neuen Batchprozessor rekursiv alle Dateien eines Verzeichnisses in
								  einem Rutsch z.B. nach WAV konvertieren k�nnen.<br>
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
