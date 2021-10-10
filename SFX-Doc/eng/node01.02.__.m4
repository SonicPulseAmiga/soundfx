divert(-1)
define(`__TITLE',`Menus')
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
					<p align="justify">Depending on which __SFX window is active, you can access
					one of the pull-down menus described in the next sub chapters.</p>
					<p align="justify">Grayed menu entries signal that the menu entry is
					not available at the moment. This happens e.g. if you have not yet loaded
					samples or have not marked a range.</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0" width="100%"><tr><td><table border=0 cellpadding="0" width="100%">
					<tr><td>   1.2.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.02.01.html">__NODETITLE(`node01.02.01.html')</a></td></tr>
					<tr><td>   1.2.2</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.02.02.html">__NODETITLE(`node01.02.02.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
