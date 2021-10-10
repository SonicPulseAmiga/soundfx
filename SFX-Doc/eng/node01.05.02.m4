divert(-1)
define(`__TITLE',`information window')
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
						__IMAGE(`win_info')
					</center>
					<br>
					<p align="justify">
					Information displays, as its name suggests, useful information about the program like :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Range        						</th><th>Description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">program name			</td><td>If this not reads &quot;__SFX&quot;, you are using the wrong software ;-)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">version number			</td><td>Please always include this, when contacting me with a problem</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">copyright &amp; author	</td><td>...</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">list of samples			</td><td>a list of loaded samples. After selection one entry, additional
																								information will appear in the fields below.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">registration information</td><td>Your registration number and name (if there is a name, then it is 
																								hopefully yours !!! ).</td></tr>
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
