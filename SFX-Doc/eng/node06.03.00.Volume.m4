divert(-1)
define(`__TITLE',`Volume')
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
					<p align="justify">The volume of a soound can be given in several ways :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>kind</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td><nobr>maximum volume /</nobr> <nobr>peak volume</nobr></td><td>largest peak in amplitude</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td><nobr>average volume</nobr></td><td>average of all absolute amplitudes</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td><nobr>acoustical volume</nobr></td><td>energy of the sound</td></tr>
					</table>
					__SFX shows you all these levels inside the sample-window, if you have activated this in the <a href="node01.05.03.html">sample options</a>
					(or generell in the <a href="node01.06.02.html">setting for the samples</a>).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
