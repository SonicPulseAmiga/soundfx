divert(-1)
define(`__TITLE',`Kein AHI-System bzw. ungültiger Audiomodus !')
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
					<!-- errNoAHI -->
					<p align="justify">F&uuml;r den AHI-Player benötigen Sie das <a href="http://www.lysator.liu.se/~lcs/ahi.html" taget="new">AHI-System</a>.<br>
					Falls Sie AHI bereits installiert haben, liegt es wahrscheinlich daran, da&szlig; sie noch keinen
					Audiomodus ausgewählt haben. Klicken Sie dazu einfach auf den '?'-Knopf neben der Playerauswahl.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
