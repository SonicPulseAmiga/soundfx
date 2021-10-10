divert(-1)
define(`__TITLE',`Die Installation scheint nicht komplett zu sein ! ...')
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
					<!-- msgInstIncomplete -->
					<p align="justify">Bitte installieren Sie immer die sfx-bin_???, sfx-doc_??? und sfx-data Archive.
					Das Weglassen von Programmteilen kann zu einem unstabilen Programm führen! Bitte benutzen Sie den
					Installer und führen sie die Installation nicht von Hand aus.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
