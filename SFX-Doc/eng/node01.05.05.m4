divert(-1)
define(`__TITLE',`window function window')
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
						__IMAGE(`win_winfunc')
					</center>
					<br>
					<p align="justify">You can select a windowfunction and eventually adjust a parameter in this
					window. The chosen function will be displayed graphically. The upper graph shows the course
					of the windowing function in the time-domain and the lower graph shows its effect in the
					frequency-domain. This way one can see, that some functions filter better in the stop-band,
					but make the slope less steep.
					This window usually get opened from an <a href="node02.01.__.html">operator</a> (<a href="node02.01.04.html">choice of window function</a>).
					</p>
					<p align="justify">The choice of a windowing function is always a compromise.
					Here an example	for a FIR-filter :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Window        			</th><th>Description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Rectangle	</td><td>+ good slope<br>
																						- bad gain</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Hamming		</td><td>- bad slope<br>
																						+ good gain</td></tr>
					</table>
					</p>
					<p align="justify">Multiple application of a filter let both characteristics become better.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
