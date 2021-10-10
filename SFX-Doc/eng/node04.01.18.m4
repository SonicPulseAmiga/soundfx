divert(-1)
define(`__TITLE',`No AHI System or invalid AudioMode !')
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
					<!-- errNoAHI -->
					<p align="justify">The AHI player requires an installed <a href="http://www.lysator.liu.se/~lcs/ahi.html" taget="new">AHI system</a>.<br>
					If it is installed, than you have probably not yet choosen an audio-mode to use.
					Just klick the '?'-button next to the player selection.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
