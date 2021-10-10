divert(-1)
define(`__TITLE',`Errors')
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
						<b>Q:</b> When I try loading a sample of 10Mb, I sometimes get an "Out of memory", even though I still have 13 Mb free.<br>
						<b>A:</b> You need those 10 Mb as one block. Enter "avail" inside a shell-window. It will show the larges block available.<br><br>

						<b>Q:</b> I have a 10 Mb sample loaded and still 4 Mb free now I'm trying to cut something (e.g. 512 kb) and I get an "Out of memory".<br>
						<b>A:</b> When doing a Cut (or Erase) __SFX has to copy the sample data you want to keep to a new buffer and free the source one.<br><br>

						<b>Q:</b> When starting __SFX under OS3.5 I get the following error "Can&#39;t open amigaguide.library &gt;=V34 !".<br>
						<b>A:</b> Please check the installation of OS3.5. It seems that it sometimes installs the Data-Types to "libs:datatypes"
								  and not to "sys:classes/datatypes".<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
