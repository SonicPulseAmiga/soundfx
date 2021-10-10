divert(-1)
define(`__TITLE',`Can not read this &lt;...&gt; File !')
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
					<!-- errUnsupportedSubtype -->
					<p align="justify">__SFX does not understand this sub-type. You can get in touch with <a href="node00.05.__.html">me</a>
					and send the file in an email. And if you help me with to find information about this sub-type, chances are good that __SFX
					will be able to read this file soon.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
