divert(-1)
define(`__TITLE',`Wie ich Ihren bereits sagte`,' k?nnen Sie in der Demoversion den ARexx-Port nicht verwenden !')
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
					<!-- msgDemoNoARexx -->
					<p align="justify">Wenn Sie in der Demo-Version von __SFX versuchen den ARexxport zu benutzen,
					erscheint diese Nachricht. Um den ARexx-Port zu aktivieren, sollten Sie
					sich <a href="node00.04.__.html">registrieren</a>.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
