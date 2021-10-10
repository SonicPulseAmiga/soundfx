divert(-1)
define(`__TITLE',`Kann den Bildschirm nicht schlieﬂen ! Bitte schlieﬂen Sie zuerst alle Gastfenster !')
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
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<!-- errVisitorWindows -->
					<p align="justify">Auf dem __SFX Bildschirm befinden sich noch Gastfenster. Bitte schlie&szlig;en Sie diese,
					sonst kann der __SFX Bildschirm nicht geschlossen werden.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
