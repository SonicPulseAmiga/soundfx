divert(-1)
define(`__TITLE',`SoundFX is already running ! Should I start it again ?')
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
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<!-- reqRunAgain -->
					<p align="justify">You have started __SFX another time. If you choose okay, then
					it will be this way, otherwise it will be exited immediately. Please remeber that
					only the first __SFX has an AREXX port.
					This is because you have to supply the port name in your scripts and there can
					only be one port of that name.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
