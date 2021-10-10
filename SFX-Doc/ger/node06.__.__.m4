divert(-1)
define(`__TITLE',`Anhang')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_ger.m4')
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
					<p align="justify">Hier finden sie z.B. verschiedene &Uuml;bersichten und Tabellen zum Nachschlagen.
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
					<tr><td>   6.1  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node06.01.__.html">__NODETITLE(`node06.01.__.html')</a></td></tr>
					<tr><td>   6.2  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node06.02.__.html">__NODETITLE(`node06.02.__.html')</a></td></tr>
					<tr><td>   6.3  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node06.03.__.html">__NODETITLE(`node06.03.__.html')</a></td></tr>
					<tr><td>   6.4  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node06.04.__.html">__NODETITLE(`node06.04.__.html')</a></td></tr>
					<tr><td>   6.5  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node06.05.__.html">__NODETITLE(`node06.05.__.html')</a></td></tr>
					<tr><td>   6.6  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node06.06.__.html">__NODETITLE(`node06.06.__.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
