divert(-1)
define(`__TITLE',`Zoom')
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
					<p align="justify">These functions allow you to enlarge or shrink the selected range freely,
					so that you can work more efficient. To select a range to zoom, just click the starting
					point and move the mouse to the ending point while keeping the mouse-button pressed.
					While selecting, the range will be highlighted and the positions as well as the length can
					be seen in the <a href="node01.04.__.html">status-bar</a>. You can fine-tune the selection
					with the function from the <a href="node01.03.09.html">range toolbar</a>.
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>button          </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 1</td><td>zoom-mode - zoom into which direction</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 2</td><td>enlarge, if no range is selected then enlarge by 2</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 3</td><td>shrink</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 4</td><td>1:1, means one pixel is one sample</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 5</td><td>show all (zoom out totally)</td></tr>
					</table>
					</p>
					<p align="justify">As these functions are used quite often, __SFX offers the following shortcuts :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>&nbsp;					</th><th>x-axis</th><th>y-axis</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><th align="right">zoom in	</th><td>"&lt;"	</td><td>CTRL+"&lt;"</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><th align="right">zoom out	</th><td>"&gt;"	</td><td>CTRL+"&gt;"</td></tr>
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
