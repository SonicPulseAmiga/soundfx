divert(-1)
define(`__TITLE',`Savers')
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
							<tr><td colspan="4">__IMAGE(`tb_saver')</td></tr>
							<tr align="center">
								<td width="100"><font size="-1">1</font></td>
								<td width="27"><font size="-1">2</font></td>
								<td width="27"><font size="-1">3</font></td>
								<td width="27"><font size="-1">4</font></td>
							</tr>
						</table>
					</center>
					<br>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Button          </th><th>Description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 1</td><td>name of the active saver-module</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 2</td><td>opens the drop-down list</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 3</td><td>opens the settings-window for the selected saver. you can start the saver from there as well.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 4</td><td>starts the selected saver</td></tr>
					</table>
					<p align="justify">You can read about theses modules in detail in <a href="node02.05.__.html">section 2.5</a>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
