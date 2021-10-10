divert(-1)
define(`__TITLE',`Kann Funktionsbibliothek nicht öffnen !')
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
					<!-- errOpenLibrary -->
					<p align="justify">__SFX kann die angegebene Funktionsbibliothek mit der geforderten Mindestversion
					nicht öffnen. Testen sie, ob die Bibliothek vorhanden ist und ob sie aktuell genug ist. Letzteres
					erreichen Sie, wenn sie in einer Shell "version <libname> FULL" eingeben.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
