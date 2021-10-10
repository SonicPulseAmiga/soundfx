divert(-1)
define(`__TITLE',`Can not &lt;...&gt; &lt;...&gt; !')
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
					<!-- err???? -->
					<p align="justify">__SFX can not obtain a resource, as probably there
					is no free memory available or the resource is already in use.
					In the first case please end other running applications or
					close large projects, to free the needed memory.
					Sometimes it is already sufficient to enter the	following command in the shell: "avail flush".
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
