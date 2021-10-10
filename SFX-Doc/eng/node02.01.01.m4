divert(-1)
define(`__TITLE',`Source selection')
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
				<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
					<center>
						__IMAGE(`fx_srcsel1')
					</center>
					<br>
					<center>
						__IMAGE(`fx_srcsel2')
					</center>
					<br>
					<p align="justify">These controls are for choosing a source to operate on. The cycle gadget right to the source allows you
					to choose the range which should be processed. __SFX automatically suggests the probably most desired mode, e.g.
					if you have marked a range, then range is preselected. The following variants are possible:
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>choice        		</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">All		</td><td>the whole sample will be processed</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Window	</td><td>only the currently visible part (zoomed) will
																				be processed</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Range	</td><td>only the marked range will be processed</td></tr>
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
