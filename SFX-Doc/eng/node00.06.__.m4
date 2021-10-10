divert(-1)
define(`__TITLE',`The most important chapters')
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
					<p align="justify">Because you will probably not read all this from
					the beginning to the very end right now, I have prepared a list with
					the most important chapters below. To use __SFX effectively I strongly
					recommend, that you have a quick look at these chapters at least. Otherwise it may
					happen that you probably never learn about some features.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0" width="100%"><tr><td><table border=0 cellpadding="0" width="100%">
					<tr><td>   1.5.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.05.01.html">__NODETITLE(`node01.05.01.html')</a></td></tr>
					<tr><td>   1.7  </td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.07.__.html">__NODETITLE(`node01.07.__.html')</a></td></tr>
					<tr><td>   2    </td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.__.__.html">__NODETITLE(`node02.__.__.html')</a></td></tr>
					<tr><td>   2.1  </td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.__.html">__NODETITLE(`node02.01.__.html')</a></td></tr>
					<tr><td>   6.4  </td><td><img src="../_img/space.gif" height="0" width="30"><a href="node06.04.__.html">__NODETITLE(`node06.04.__.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
