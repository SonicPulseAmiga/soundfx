divert(-1)
define(`__TITLE',`Window function')
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
					<p align="justify">
					These parameters of a <a href="node02.01.04.html">window function</a> can be changed with ARexx.
					You can find the required &lt;prefix&gt; (e.g. W1) inside the documentation of the operators.<br>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>parameter             </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;prefix&gt;WinType </td><td>"Rectangle","Bartlett","Fejer","Welch","Hanning","Hamming","Blackman","Kaiser","HalfSine","HalfSineQ","HalfSineS"</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;prefix&gt;WinPar  </td><td>parameter for the window function</td></tr>
					</table>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
