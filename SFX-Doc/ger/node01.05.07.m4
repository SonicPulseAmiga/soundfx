divert(-1)
define(`__TITLE',`Statusfenster')
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
						__IMAGE(`win_progress')
					</center>
					<br>
					<p align="justify">In diesem Fenster wird der Fortgang der Operationen angezeigt. Dies geschieht
					über einen Statusbalken mit inneliegender Prozentanzeige. Zus&auml;tzlich wird in der
					Titelleiste des Fensters wird noch einmal angezeigt, was überhaupt gemacht wird.
					</p>
					<p align="justify">Die Operation kann jederzeit mit einem Klick auf "Stop", dem Drücken
					der Tasten "S","s","ESC" oder einem Klick auf das "Close"-Gadget des Fensters abgebrochen werden.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
