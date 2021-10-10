divert(-1)
define(`__TITLE',`Aufnahmefenster')
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
						__IMAGE(`win_record')
					</center>
					<br>
					<p align="justify">Mit __SFX k�ennen Sie nat�rlich auch eigene Sounds von extenen
					Tonquellen (z.B. Mikrophone) aufnehmen. __SFX benutzt AHI f�r die Aufnahme.
					Wenn sie direkt von einer CD aufnehmen wollen, probieren
					sie bitte den <a href="node02.02.01/CDDA-Direct_L.html">CDDA-Loader</a>.<br>
					Dieses Fenster bietet ihnen folgende Funktionen :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Gadget        				</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">AHI Record Mode	</td><td>W�ehlen Sie den Audiomodus f�r die Aufnahme.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Record Source	</td><td>Dies ist eine Liste der vorhandenen Aufnahmequellen.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Record Gain		</td><td>Hiermit steuern Sie die Lautst�rke der Aufnahme aus.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Record Auto		</td><td>Dies ist eine Besonderheit von __SFX.
																							Ziehen Sie den Lautst�rkeregler einfach voll nach rechts und aktivieren sie 'Auto'.
																							Jetzt wird __SFX die Lautst�rke solange zur�cknehmen,
																							bis keine �bersteuerungen auftreten.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Monitor Source	</td><td>Dies ist eine Liste der Abh�rausg�nge.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Monitor Gain	</td><td>Hiermit steuern Sie die Lautst�rke zum Abh�ren.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Level Meter		</td><td>Die Levelmeter zeigen ihnen die Lautst�rke am Eingang an.
																							Die roten Striche markieren den Maximalwert. Die Werte
																							rechts neben den Levelmetern zeigen den aktuellen und
																							den maximalen Wert an.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Status			</td><td>Zeigt an, wieviel bereits aufgenommen wurde.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Reset			</td><td>Dient zum Zur�cksetzen der Maximalwertanzeige.</td></tr>
					</table>
					</p>
					<p align="justify">Ein Klick auf "Record" startet die Aufnahme. Die l�ufende L�nge wird im Infofeld
					angezeigt. "Stop" h�lt die Aufnahme wieder an. Wenn __SFX aufnimmt, sind die Levelmeter inaktiv um
					Rechenleistung zu sparen.
					</p>
					<p align="justify">Bitte beachten sie, das AHI derzeit immer im Stereo 16 bit Format aufnimmt. Zuk�nftige
					Versionen unterst�tzen m�glicherweise auch Mono Aufnahmen. F�r __SFX gibt es keine derzeit keine einfache
					M�glichkeit das Problem zu umgehen. Sie k�nnen jedoch den Convert-Channels Operator nach der Aufnahme
					einsetzen.<br>
					Ein anderes Problem ist, das sie m�glicherweise die Gain-Regler nicht nutzen k�nnen. Das kann daran liegen,
					das die Aufnahmehardware und/oder der AHI Treiber dies nicht unterst�tzen.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
