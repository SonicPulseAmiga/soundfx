divert(-1)
define(`__TITLE',`Players')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_eng.m4')
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
							<tr><td colspan="7">__IMAGE(`tb_player')</td></tr>
							<tr align="center">
								<td width="94"><font size="-1">1</font></td>
								<td width="27"><font size="-1">2</font></td>
								<td width="27"><font size="-1">3</font></td>
								<td width="27"><font size="-1">4</font></td>
								<td width="27"><font size="-1">5</font></td>
								<td width="27"><font size="-1">6</font></td>
								<td width="27"><font size="-1">7</font></td>
							</tr>
						</table>
					</center>
					<br>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>button          </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 1</td><td>name of the active player-module</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 2</td><td>opens the drop-down list</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 3</td><td>opens the settings-window for the selected player</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 4</td><td>plays the active sample with loop</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 5</td><td>plays the selected range</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 6</td><td>stops the player</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 7</td><td>opens the <a href="node01.05.09.html">recording window</a> (requires AHI)</td></tr>
					</table>
					<p align="justify">You can read about the player-modules in detail in <a href="node02.03.__.html">section 2.3</a>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
