divert(-1)
define(`__TITLE',`Introduction')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_eng.m4')
include(`header.m4')
include(`footer.m4')
include(`navbar.m4')
divert`'dnl
<!-- NEW PAGE -->
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
					<p align="justify">Below I start with some general overview of __SFX,
					followed by some legal talk and - very important - the information about registration along with contact data
					about the author [thats me ;-)].
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0" width="100%"><tr><td><table border="0" cellpadding="0" width="100%">
					<tr><td>   0.1  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node00.01.__.html">__NODETITLE(`node00.01.__.html')</a></td></tr>
					<tr><td>   0.2  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node00.02.__.html">__NODETITLE(`node00.02.__.html')</a></td></tr>
					<tr><td>   0.3  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node00.03.__.html">__NODETITLE(`node00.03.__.html')</a></td></tr>
					<tr><td>   0.4  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node00.04.__.html">__NODETITLE(`node00.04.__.html')</a></td></tr>
					<tr><td>   0.5  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node00.05.__.html">__NODETITLE(`node00.05.__.html')</a></td></tr>
					<tr><td>   0.6  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node00.06.__.html">__NODETITLE(`node00.06.__.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
