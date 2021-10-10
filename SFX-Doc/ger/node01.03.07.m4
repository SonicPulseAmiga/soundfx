divert(-1)
define(`__TITLE',`Edit')
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
							<tr><td colspan="7">__IMAGE(`tb_edit')</td></tr>
							<tr align="center">
								<td width="26"><font size="-1">1</font></td>
								<td width="26"><font size="-1">2</font></td>
								<td width="26"><font size="-1">3</font></td>
								<td width="26"><font size="-1">4</font></td>
								<td width="26"><font size="-1">5</font></td>
								<td width="26"><font size="-1">6</font></td>
								<td width="26"><font size="-1">7</font></td>
							</tr>
						</table>
					</center>
					<p align="justify">__SFX stellt ihnen vielfältige Schnittfunktionen zur Verfügung (wesentlich mehr
					als sie in anderen Programmen finden werden).
					Bedenken Sie bitte das es sich hierbei um destruktive Operatoren handelt, d.h.
					sie nehmen direkte Änderungen an einem Sample vor - es wird kein neuer Puffer
					angelegt und die Änderungen sind nicht rückgängig zumachen. Deshalb empfiehlt
					es sich vorher lieber einmal mehr abzuspeichern.
					Den zu bearbeitenden Bereich markieren Sie, indem Sie den Startpunkt anklicken
					und mit gedrückter linker Maustaste bis zum Endpunkt fahren. Während der Mausbewegung
					wird der bisher markierte Bereich hervorgehoben und die Start- und Endpositionen, sowie die Länge in der
					<a href="node01.04.__.html">Statusleiste</a> angezeigt. Verwenden Sie auch die Funktionen der <a href="node01.03.09.html">Bereichs-Toolbar</a>
					um ihren Bereich optimal zu selektieren.
					</p>
					<p align="justify">
					Folgende Funktionen k&ouml;nnen Sie benutzen (hinter jedem Button verbergen sich Men&uuml;s) : 
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Button          </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 1</td><td>Cut - aktuellen Bereich ausschneiden</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 2</td><td>Copy - aktuellen Bereich in den Copypuffer &uuml;bernehmen</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 3</td><td>Paste - den Inhalt des Copypuffers einfügen</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 4</td><td>Erase - aktuellen Bereich l&ouml;schen</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 5</td><td>Grab - aktuellen Bereich in neues Fenster übernehmen</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 6</td><td>Zero - aktuellen Bereich &quot;ausnullen&quot;</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 7</td><td>Overwrite - den Inhalt des Copypuffers einsetzen</td></tr>
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
