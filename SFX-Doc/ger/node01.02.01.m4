divert(-1)
define(`__TITLE',`Hauptmen&uuml;')
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
					<p align="justify">Dieses Menü ist erreichbar, wenn keine weiteren Dialog-Fenster aktiv sind.</p>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Hauptmen&uuml;  </th><th>Untermen&uuml;            </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Projekt         </td><td>Neu                       </td><td>Öffnet einen Dialog zum Anlegen eines leeren Samples.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Laden                     </td><td>Laden von Samples mit dem aktuellen Loader (siehe auch <a href="node01.03.01.html">Auswahl eines Lademoduls</a>)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Speichern                 </td><td>Speichern eines Samples mit dem aktuellen Saver (siehe auch <a href="node01.03.02.html">Auswahl eines Speichermoduls</a>)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Schließen                 </td><td>Entfernt die ausgewählten Samples nach einer Sicherheitsabfrage</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Ausf&uuml;hren            </td><td>Starten des aktuellen Operators (siehe auch <a href="node01.03.03.html">Auswahl eines Operators</a>)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Rexx ausf&uuml;hren       </td><td>Starten des aktuellen Rexx-Scriptes (siehe auch <a href="node01.03.04.html">Auswahl eines Rexx-Scripts</a>)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Alles abspielen           </td><td>Abspielen des gesamten Samples</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Bereich abspielen         </td><td>Abspielen des ausgewählten Bereiches</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Anhalten                  </td><td>Stoppen des Abspielvorganges</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Aufnahme                  </td><td>Aufruf des <a href="node01.05.09.html">Aufnahmefensters</a> (ben&ouml;tigt AHI)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Stapelverarbeitung        </td><td>Aufruf des <a href="node01.05.10.html">Stapelverarbeitungsfensters</a></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Informationen             </td><td>Aufruf des <a href="node01.05.02.html">Informationsfensters</a></td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>MRU (5x)                  </td><td>die 5 zuletzt geladenen Samples können hiermit direkt wieder geladen werden.<br>
																													Die Einträge werden in der Datei &quot;data/MRU.cfg&quot; gespeichert.
																											   </td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Quit                      </td><td>Beenden nach einer Sicherheitsabfrage</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Bearbeiten      </td><td>...                       </td><td>Analog zur <a href="node01.03.07.html">Edittoolbar</a></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Bereich         </td><td>...                       </td><td>dient dem Setzten, Angleichen und R&uuml;cksetzen von Bereichen</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Ausschnitt      </td><td>...                       </td><td>Analog zur <a href="node01.03.08.html">Zoomtoolbar</a></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Aufr&auml;umen  </td><td>Current                   </td><td>ordne aktuelles Samplefenster neu an</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>All                       </td><td>ordne alle Samplefenster an</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>All normal                </td><td>ordne alle Samplefenster und bring sie auf normale Gr&ouml;&szlig;e</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>All zoomed                </td><td>ordne alle Samplefenster und bring sie auf kleine Gr&ouml;&szlig;e</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Hilfsmittel     </td><td>Tausche Byteordnung       </td><td>repariere Dateien, die mit der falschen Byteordnung gespeichert wurden</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Tausche Vorzeichen        </td><td>repariere Dateien, die mit dem falschen Vorzeichen gespeichert wurden</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Kanäle verschränken       </td><td>repariere Dateien, die im falschen Kanalformat gespeichert wurden</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Kanäle auftrennen         </td><td>repariere Dateien, die im falschen Kanalformat gespeichert wurden</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Voreinstellungen</td><td>GUI                       </td><td>Voreinstellungen für das GUI</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Sample                    </td><td>Voreinstellungen für das Samplefenster</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>virtueller Speicher       </td><td>Voreinstellungen für den Virtuellen Speicher</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Verschiedenes             </td><td>verschiedene Voreinstellungen</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Benutzen                  </td><td>Merkt sich die aktuellen Einstellungen solange der Rechner an ist</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Speichern                 </td><td>Speichert die aktuellen Einstellungen dauerhaft</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>Zuletzt benutzte laden    </td><td>Läd die zuletzt gespeicherten Einstellungen</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;          </td><td>Zuletzt gespeicherte laden</td><td>(macht noch nichts)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;          </td><td>auf Vorgaben zurücksetzen </td><td>(macht noch nichts)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Hilfe           </td><td>...                       </td><td>Aufrufen des gewählten Themas der Online-Hilfe, Sprung zur Supportseite
																											 im Internet, Schreiben einer EMail an den Autor oder Anzeige von
																											 Versionsinformationen zur Software.</td></tr>
					</table>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
