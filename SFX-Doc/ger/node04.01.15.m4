divert(-1)
define(`__TITLE',`Kann Bildschirm nicht als öffentlich deklarieren !')
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
					<!-- errMakeScreenPublic -->
					<p align="justify">Es scheint bereits ein Bildschirm mit dem Namen __SFX offen zu sein. Wenn sie
					diesen nicht schlie&szlig;en köennen, müssen sie ihren Rechner neu starten um __SFX benutzen zu können.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
