divert(-1)
define(`__TITLE',`sample window')
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
						__IMAGE(`win_sample')
					</center>
					<br>
					<p align="justify">
					<b>the window :</b><br>
					When a sample has been loaded or generated, it is then displayed within its own
					window. Size and position can be changed via the windows gadgets. Several lines are
					drawn to help reading positions and levels of the sample. Additional lines can be
					drawn to display the maximum, average and real (acoustical) amplitude.<br>
					If 'Loop' is activated and start and repeat lengths are set, vertical lines
					with boxes attached to the top will visualize  the looping part. If some range is
					selected (marked), this is shown by an highlighted filled box.<br>
					In the window title bar __SFX displays the samples name, playback rate and
					length. While playing a sample you see the play-position there.
					</p>
					<p align="justify">
					<b>actions inside the window :</b><br>
					When moving the mouse around the mouse pointer will change its shape to indicate
					what action can be performed.<br>
					The loop lines can be moved by clicking and holding the left mouse-button onto
					the box and moving the mouse.<br>
					Clicking down the left mouse-button inside the sample window but outside of the
					loop boxes or a previously marked area will start a new marking operation. When
					clicking inside a mark (not near the borders), it can be moved around, while
					holding the left mouse-button. When clicking inside a mark at the borders, the
					range can be modified into that direction. Here is a 'picture' to make it more
					clearly (anyway the mouse pointer shape should clearly show the available
					action) :
					</p>
					<center>
						<table border="0" cellspacing="1" cellpadding="4">
							<tr bgcolor=__COL_TBL_HEAD>
								<td>
									<table border="2" cellspacing="1" cellpadding="4">
										<tr height="15%">
											<td width="20%" bgcolor=__COL_TBL_ROW1 align="center" valign="center">1</td>
											<td width="60%" bgcolor=__COL_TBL_ROW2 align="center" valign="center">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
											<td width="20%" bgcolor=__COL_TBL_ROW1 align="center" valign="center">3</td>
										</tr>
										<tr height="70%">
											<td width="20%" bgcolor=__COL_TBL_ROW2 align="center" valign="center">4</td>
											<td width="60%" bgcolor=__COL_TBL_ROW1 align="center" valign="center"><br><br>5<br><br><br></td>
											<td width="20%" bgcolor=__COL_TBL_ROW2 align="center" valign="center">6</td>
										</tr>
										<tr height="15%">
											<td width="20%" bgcolor=__COL_TBL_ROW1 align="center" valign="center">7</td>
											<td width="60%" bgcolor=__COL_TBL_ROW2 align="center" valign="center">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;8&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
											<td width="20%" bgcolor=__COL_TBL_ROW1 align="center" valign="center">9</td>
										</tr>
									</table>
								</td>
								<td>
									1 change left and upper border<br>
									2 change upper border<br>
									3 change right and upper border<br>
									4 change left border<br>
									5 move around<br>
									6 change right border<br>
									7 change left and lower border<br>
									8 change lower border<br>
									9 change right and lower border<br>
								</td>
							</tr>
						</table>
					</center>
					<p align="justify">
					This area (or range) can be magnified, cut or copied. If an area was magnified,
					moving the slider at the bottom or right of the window, will move the display
					through the sample data. This area will be continuously updated while sliding.
					While modifying loops, marking ranges and zooming areas __SFX displays information
					about start, end and length in the <a href="node01.04.__.html">status bar</a>.<br>
					If you have zoomed you view more than 1:1 and selected "Trace" in the <a href="node01.03.09.html">range-toolbar</a>,
					it is possible to draw directly into the sample data while left mouse-button is pressed.
					With this function you can manually remove errors (cracks). The sample display will be
					refreshed when you release the mouse-button.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
