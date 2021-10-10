divert(-1)
define(`__TITLE',`Statusbar')
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
							<tr><td colspan="14">__IMAGE(`sb')</td></tr>
							<tr align="center">
								<td width="170"><font size="-1">1</font></td>
								<td width="5"><img src="../_img/space.gif" width="5" height="1"></td>
								<td width="50"><font size="-1">2</font></td>
								<td width="50"><font size="-1">3</font></td>
								<td width="50"><font size="-1">4</font></td>
								<td width="5"><img src="../_img/space.gif" width="5" height="1"></td>
								<td width="50"><font size="-1">5</font></td>
								<td width="50"><font size="-1">6</font></td>
								<td width="50"><font size="-1">7</font></td>
								<td width="5"><img src="../_img/space.gif" width="5" height="1"></td>
								<td width="50"><font size="-1">8</font></td>
								<td width="50"><font size="-1">9</font></td>
								<td width="50"><font size="-1">10</font></td>
								<td width="5"><img src="../_img/space.gif" width="5" height="1"></td>
							</tr>
						</table>
					</center>
					<br>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Button          </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 1</td><td>Schnellhilfe - bewegen sie mal die Maus &uuml;ber die <a href="node01.03.__.html">Toolbars</a> ...</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 2</td><td>Start des X-Bereiches</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 3</td><td>Ende des X-Bereiches</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 4</td><td>L&auml;nge des X-Bereiches</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 5</td><td>Start des Y-Bereiches</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 6</td><td>Ende des Y-Bereiches</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 7</td><td>L&auml;nge des Y-Bereiches</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 8</td><td>X-Wert unter dem Mauspfeil</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 9</td><td>Y-Wert unter dem Mauspfeil</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">10</td><td>Y-Wert im Sample an der Stelle des Mauspfeils</td></tr>
					</table>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
