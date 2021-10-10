divert(-1)
define(`__TITLE',`status window')
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
						__IMAGE(`win_progress')
					</center>
					<br>
					<p align="justify">In this window the progress of an operation will be shown.
					Therefore __SFX utilizes a growing status-bar with a percentage-display.
					Additional information is given in the title of the window.
					</p>
					<p align="justify">The calculation can be stopped with one click at "Stop",
					pressing the keys "S","s","ESC"	or a click at the "Close"-gadget of the window.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
