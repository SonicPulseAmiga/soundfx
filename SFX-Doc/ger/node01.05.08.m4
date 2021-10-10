divert(-1)
define(`__TITLE',`Quellenauswahlfenster')
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
						__IMAGE(`win_sourcesel')
					</center>
					<br>
					<p align="justify">Dieses Fenster dient dem Auswählen eines Eintrages aus einer Liste.
					Es durch einen Klick auf das PopUp-Symbol aufgerufen. Der ausgewählte
					Eintrag wird danach in dem Feld neben dem PopUp-Symbol dargestellt.
					Die Auswahl kann mit einem Doppelklick auf einen Eintrag oder mit einen Klick
					auf "Okay" erfolgen.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
