divert(-1)
define(`__TITLE',`Range')
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
					<p align="justify">These functions are for finely adjusting loops, marked ranges and the zoomed area :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>button          </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 1</td><td>range mode :
																			<table border="0">
																				<tr>
																					<td valign="top"><nobr>Loop :</nobr></td>
																					<td>editing the looped part</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>Mark :</nobr></td>
																					<td>editing the highlighted area</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>Zoom :</nobr></td>
																					<td>editing the enlarged area</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>Trace :</nobr></td>
																					<td>inspect sample values and freehand correction</td>
																				</tr>
																			</table>
																			some actions automatically switching the mode :<br>
																			<table border="0">
																				<tr>
																					<td valign="top"><nobr>loop :</nobr></td>
																					<td>switching loop on or off in the options</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>mark :</nobr></td>
																					<td>select a range with the mouse</td>
																				</tr>
																				<tr>
																					<td valign="top"><nobr>zoom :</nobr></td>
																					<td>use hotkeys for zooming or use buttons of the <a href="node01.03.08.html">zoom-toolbar</a></td>
																				</tr>
																			</table>
																			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 2</td><td>lock begin or end (will not be moved on subsequent edits)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 3</td><td>move begin or end</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 4</td><td>move to the left border</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 5</td><td>move to left fast</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 6</td><td>move to left slowly</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 7</td><td>move to the next left zero-crossing</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right"> 8</td><td>move to the next right zero-crossing</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 9</td><td>move to right slowly</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">10</td><td>move to right fast</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">11</td><td>move to the right border</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">12</td><td>move to the upper border</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">13</td><td>move upwards fast</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">14</td><td>move upwards slowly</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">15</td><td>move to the upper peak</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">16</td><td>move to the lower peak</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">17</td><td>move downwards slowly</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">18</td><td>move downwards fast</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">19</td><td>move to the lower border</td></tr>
					</table>
					</p>
					<p align="justify">
					The facility for seeking zero-crossings is excellently suitable for generating crackle-free loops.
					Just set the loop points manually first. Then play the sample. On every retrace you will
					quite likely hear a crack. Now activate "lock" (2) and click on "&lt;0" (7) to adjust the start and on
					"0&gt;" (8) to adjust the endpoint until the crack is minimal or even gone.
					</p>
					<p align="justify">
					If you have chosen "trace" and activated a sample-window, you will see the value under the mouse-pointer
					in fields (8) and (9) of the <a href="node01.04.__.html">status bar</a>. The sample-value will be show in
					field (10) and can even be changed there.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
