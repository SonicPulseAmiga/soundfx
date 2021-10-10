divert(-1)
define(`__TITLE',`Saver')
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
					<p align="justify">A saver is a module which stores sample-data is a certain format. __SFX
					offers you nearly all common variants to use.
					</p>
					<p align="justify">Nearly all savers have a few things in common, which I will describe below.
					If you have choosen the option "save icons" in the <a href="node01.06.04.html">prefs</a>
					the savers will create a standard-icon for the sample. Further all savers generate a file-comment
					with information like format, channels and length.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0"><tr><td><table border=0 cellpadding="0">
					<tr><td>   2.5.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.05.01.html">__NODETITLE(`node02.05.01.html')</a></td></tr>
					<tr><td>   2.5.2</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.05.02.html">__NODETITLE(`node02.05.02.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
