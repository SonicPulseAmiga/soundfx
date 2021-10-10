divert(-1)
define(`__TITLE',`Zoom')
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
							<tr><td colspan="5">__IMAGE(`tb_zoom')</td></tr>
							<tr align="center">
								<td width="26"><font size="-1">1</font></td>
								<td width="26"><font size="-1">2</font></td>
								<td width="26"><font size="-1">3</font></td>
								<td width="26"><font size="-1">4</font></td>
								<td width="26"><font size="-1">5</font></td>
							</tr>
						</table>
					</center>
					<p align="justify">Diese Operationen ermöglichen es Ihnen Bereiche eines Sample beliebig zu
					vergrößern und zu verkleinern, so das Sie optimal arbeiten können. Den zu zoomenden Bereich
					markieren Sie, indem Sie den Startpunkt anklicken und mit gedrükkter linker Maustaste bis
					zum Endpunkt fahren. Während der Mausbewegung wird der bisher markierte Bereich hervorgehoben
					und die Start- und Endpositionen, sowie die L&auml;nge wird in der <a href="node01.04.__.html">Statusleiste</a> angezeigt.
					Verwenden Sie auch die Funktionen der <a href="node01.03.09.html">Bereichs-Toolbar</a>
					um ihren Bereich optimal zu selektieren.
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Button          </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 1</td><td>Zoommodus - welche Richtung soll vergr&ouml;&szlig;ert/verkleinert werden</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 2</td><td>vergr&ouml;&szlig;ern, wenn kein Bereich markiert ist,
																			wird 2-fach vergrößert.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 3</td><td>verkleinert</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 4</td><td>1:1, d.h. ein Pixel entspricht einem Samplewert</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 5</td><td>alles darstellen (maximal herauszoomen)</td></tr>
					</table>
					</p>
					<p align="justify">Da diese Funktionen recht h&auml;ufig ben&ouml;tigt werden, stellt __SFX ihnen folgende
					Tastaturkommandos zur Verf&uuml;gung :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>&nbsp;					</th><th>X-Achse</th><th>Y-Achse</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><th align="right">Zoom In	</th><td>"&lt;"	</td><td>CTRL+"&lt;"</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><th align="right">Zoom Out	</th><td>"&gt;"	</td><td>CTRL+"&gt;"</td></tr>
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
