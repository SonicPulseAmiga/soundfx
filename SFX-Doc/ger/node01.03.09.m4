divert(-1)
define(`__TITLE',`Bereich')
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
						<table border="0" cellspacing="0" cellpadding="0">
							<tr><td colspan="19">__IMAGE(`tb_range')</td></tr>
							<tr align="center">
								<td width="26"><font size="-1">1</font></td>
								<td width="26"><font size="-1">2</font></td>
								<td width="26"><font size="-1">3</font></td>
								<td width="17"><font size="-1">4</font></td>
								<td width="17"><font size="-1">5</font></td>
								<td width="17"><font size="-1">6</font></td>
								<td width="17"><font size="-1">7</font></td>
								<td width="17"><font size="-1">8</font></td>
								<td width="17"><font size="-1">9</font></td>
								<td width="17"><font size="-1">10</font></td>
								<td width="17"><font size="-1">11</font></td>
								<td width="17"><font size="-1">12</font></td>
								<td width="17"><font size="-1">13</font></td>
								<td width="17"><font size="-1">14</font></td>
								<td width="17"><font size="-1">15</font></td>
								<td width="17"><font size="-1">16</font></td>
								<td width="17"><font size="-1">17</font></td>
								<td width="17"><font size="-1">18</font></td>
								<td width="17"><font size="-1">19</font></td>
							</tr>
						</table>
					</center>
					<p align="justify">Mit diesen Funktionen können Sie die Looppunkte, den markierten Bereich und den
					angezeigten Ausschnitt genau justieren :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Button          </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 1</td><td>Bereichsmodus :
																			<table border="0">
																				<tr>
																					<td valign="top"><nobr>Loop :</nobr></td>
																					<td>bearbeiten des Wiederholungsabschnittes</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>Mark :</nobr></td>
																					<td>bearbeiten des hervorgehobenen Bereiches</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>Zoom :</nobr></td>
																					<td>bearbeiten des vergr&ouml;&szlig;erten Bereiches</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>Trace :</nobr></td>
																					<td>inspizieren der Samplewerte und Freihandkorrektur</td>
																				</tr>
																			</table>
																			Die einzelnen Modi werden bei folgenden Aktionen automatisch
																			ausgewählt :<br>
																			<table border="0">
																				<tr>
																					<td valign="top"><nobr>Loop :</nobr></td>
																					<td>an- und abschalten von Loop in den Options</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>Mark :</nobr></td>
																					<td>markieren eines Bereiches mit der Maus</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>Zoom :</nobr></td>
																					<td>drücken der Hotkeys zum Zoomen bzw. betätigen eines Buttons der <a href="node01.03.08.html">Zoom-Toolbar</a></td>
																				</tr>
																			</table>
																			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 2</td><td>Anfang oder Ende feststellen (wird nicht mit verschoben)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 3</td><td>Anfang oder Ende verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 4</td><td>zum linken Rand verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 5</td><td>schnell nach links verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 6</td><td>langsam nach links verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 7</td><td>zum n&auml;chsten linken Nulldurchgang verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 8</td><td>zum n&auml;chsten rechten Nulldurchgang verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 9</td><td>langsam nach rechts verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">10</td><td>schnell nach rechts verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">11</td><td>zum rechten Rand verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">12</td><td>zum oberen Rand verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">13</td><td>schnell nach oben verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">14</td><td>langsam nach oben verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">15</td><td>zum oberen Maximalwert (Peak) verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">16</td><td>zum unteren Maximalwert (Peak) verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">17</td><td>langsam nach unten verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">18</td><td>schnell nach unten verschieben</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">19</td><td>zum unteren Rand verschieben</td></tr>
					</table>
					</p>
					<p align="justify">
					Die Nulldurchgangssuche ist hervorragend dazu geeignet, um knackfreie Loops zu erzeugen.
					Setzen Sie dazu manuell die Looppunkte. Lassen Sie das Sample abspielen. Jetzt werden
					Sie sicherlich bei jedem Rücksprung zum Loopbegin ein Knackgeräusch hören. Aktivieren
					Sie "Lock" (2) und klicken solange auf "&lt;0" (7) für den Startpunkt und auf "0&gt;" (8)
					für den Endpunkt, bis das Knackgeräusch minimal oder weg ist.
					</p>
					<p align="justify">
					Wenn Sie "Trace" ausgewählt und ein Samplefenster aktiviert haben, wird in den
					Feldern (8) und (9) der <a href="node01.04.__.html">Statusleiste</a> der Wert unter dem Mauszeiger angezeigt. Der aktuelle
					Samplewert wird in (10) angezeigt und kann dort auch geändert werden.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
