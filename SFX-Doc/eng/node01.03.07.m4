divert(-1)
define(`__TITLE',`Edit')
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
					<p align="justify">__SFX comes with plenty of editing functions (probably
					many more than in other software).
					Please keep in mind that these are destructive operations. That means you directly
					edit the sample - there will be no new buffer with the result and you can't take
					edits back. Better save the sample more often to disk.
					To select a range for processing, just click the starting point and move the mouse
					to the ending point while keeping the mouse-button pressed. While selecting, the range
					will be highlighted and the positions as well as the length can be seen in the
					<a href="node01.04.__.html">status-bar</a>. You can fine-tune the selection with the
					function from the <a href="node01.03.09.html">range-toolbar</a>.
					</p>
					<p align="justify">
					These functions are available (every button is followed by a menu) : 
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>button          </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 1</td><td>cut - cut selection into copy-buffer</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 2</td><td>copy - copy selection into copy-buffer</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 3</td><td>paste - paste contents of copy-buffer</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 4</td><td>erase - erase selection</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 5</td><td>grab - generate a new buffer containing the selection</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 6</td><td>zero - set selection to zero (absolute silence)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 7</td><td>overwrite - overwrite with contents of copy-buffer</td></tr>
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
