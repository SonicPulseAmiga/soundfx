divert(-1)
define(`__TITLE',`Danksagung')
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
					<p align="justify">Ich danke allen die mir das Schreiben dieses Programmes ermöglichten oder mir
					irgendwie dabei halfen. Ohne all die Mails von Ihnen w&auml;re __SFX nicht das was es heute ist.<br>
					Ich habe beschlossen hier keine Namen direkt aufzuf&uuml;hren, da ich garantiert jemanden vergessen würde.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
