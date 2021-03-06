divert(-1)
define(`__TITLE',`Sampleoptionsfenster')
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
						__IMAGE(`win_sampleoptions')
					</center>
					<br>
					<p align="justify">In diesem Fenster lassen sich Sample-spezifische Einstellungen setzen und verändern.
					Dazu stehen folgende Schalter zur Verfügung :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Gadget					</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Draw Mode	</td><td>Über dieses Cycle-Gadget kann man auswählen, wie das Sample
																					gezeichnet werden soll. Es sehen folgende Modi zur Verfügung :<ul>
																						<li>1. Lines</li>
																						<li>2. Dots</li>
																						<li>3. DotAbs</li>
																						<li>4. Filled</li>
																						<li>5. FilledAbs</li>
																						<li>6. FilledHQ (sehr exaktes aber langsames Zeichnen)</li>
																					</ul></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Loop		</td><td>Mit diesem Schalter kann der Loopmodus gew&auml;hlt werden.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Storage		</td><td>Hiermit können Sie einstellen, ob das Sample im Speicher
																					gehalten wird oder auf die Festplatte ausgelagert werden soll.
																					__SFX entscheidet dies normalerweise automatisch. Sie können
																					dies aber nutzen, wenn sie ein Sample für eine Weile nicht
																					benötigen, den Speicher aber für andere Samples brauchen.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Channel		</td><td>Auswahl des Kanals, der im Fenster dargestellt werden soll.
																					Über die einzelnen Buttons können die entsprechenden Kanäle
																					an- und ausgeschalten werden. In die nachfolgenden Operationen
																					werden nur selektierte Kanäle einbezogen.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Raster X/Y	</td><td>Mit diesem Checkboxen können Sie das Zeichnen des Rasters ausschalten.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Axis X/Y	</td><td>Und mit diesen k&ouml;nnen Sie die Achsen abschalten. Damit vergr&ouml;ssern Sie den Zeichenbereich für die Wellenform.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Unit X/Y	</td><td>Mit diesen Gadgets können Sie die Einheiten für jede Achse
																				 	auswählen. Diese wird auch von der <a href="node01.04.__.html">Statusbar</a> benutzt.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Max-Lines	</td><td>Die MaxLines zeigen die maximale Aussteuerung eines Samples an.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">RMS-Lines	</td><td>Diese Linien zeigen die wirkliche akustische Lautstärke. Die
																					Berechnung dieser und auch der nächsten, kann etwas dauern (bei längern Samples).</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Avg-Lines	</td><td>Und diese zeigen die durchschnittliche Lautstärke an.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Quick Draw	</td><td>Wenn dies an ist, wird während des Scrollens das Raster und die Max-, RMS- und AvgLines weggelassen.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">SLen		</td><td>Hier können Sie die Länge des Samples ändern. Dies ist notwendig wenn Sie ein kurzes Sample geladen haben, und darauf z.B.
																					ein 'Echo' berechnen wollen. Tragen Sie hier einfach einen
																					größeren Wert ein und schließen Sie die Eingabe mit Enter ab.
																					__SFX hängt jetzt einen Leerbereich an das Sample an. Jetzt haben
																					Sie genug Platz für das Effektsignal.<br>
																					__SFX zeigt ihnen au&szlig;erdem gleich die Länge in der aktuellen Einheit an.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">SRat		</td><td>Es gibt folgende drei Möglichkeiten die Abspielrate zu ändern.
																					<table border="0">
																						<tr>
																							<td valign="top">PopUp-Button</td>
																							<td>Hiermit gelangen Sie in das
																								<a href="node01.05.04.html">Periodenauswahlfenster</a>.
																								Die ausgewählten Werte werden dann in den nebenstehenden
																								Gadgets eingetragen.</td>
																						</tr>
																						<tr>
																							<td valign="top">Raten Gadget</td>
																							<td>Hier können Sie die Rate direkt eingeben. Je
																								größer der Wert, desto höher wird das Sample
																								abgespielt. Normale Abspielwerte liegen zwischen
																								8000 und 96000. Nach erfolgter Eingaben wird
																								die entsprechende Note im nebenstehenden
																								Gadget eingetragen. Falls für die eingegebene
																								Periode keine Note existiert wird "---" angezeigt.</td>
																						</tr>
																						<tr>
																							<td valign="top">Noten Gadget</td>
																							<td>Hier können Sie die Note direkt eingeben.
																								Diese muß folgendes Format haben.<ul>
																									<li>1. Zeichen : Ton="C,D,E,F,G,A,H"</li>
																									<li>2. Zeichen : weiße Tasten="-", schwarze Tasten="#"</li>
																									<li>3. Zeichen : Oktave="0,1,2,3,4,5,6,7"</li>
																								</ul>
																								Beispiele : "C#3", "E-0", "H-7"<br>
																								Der zugehörige Periodenwert (Protracker) wird danach im
																								nebenstehenden Gadget eingetragen.</td>
																						</tr>
																					</table>
																					Wenn Sie die Rate des Puffers ändern, der gerade abgespielt
																					wird, hören Sie die neue Rate sofort.</td></tr>
					</table>
					</p>
					<p align="justify">Weiterhin können Sie die Texte die mit dem Sample in einigen Formaten gespeichert werden ändern.
					</p>
					<p align="justify">Mit "Okay" wird das Fenster geschlossen und mit "Reset" werden die Standardeinstellungen die sie
					In den <a href="node01.06.02.html">Preferences f&uuml;r Samplefenster</a> eingestellt haben wiederhergestellt (bitte
					beachten Sie jedoch, da&szlig; die Einstellungen f&uuml;r "SLen" und "SRat" nicht wiederhergestellt werden).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
