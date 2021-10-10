divert(-1)
define(`__TITLE',`Installation')
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
						<b>Q:</b> When I load __SFX all Operators, Loader and Savers are empty !<br>
						<b>A:</b> Make sure that you have installed a sfx-bin, a sfx-doc and the sfx-data archive.
								  If installation is incomplete __SFX will not run properly.<br><br>

						<b>Q:</b> When I install __SFX installation succeeds quickly, but afterwards the installation directory is empty.<br>
						<b>A:</b> Unarchive the lzx-files with '-x' *not* with '-e'. Only '-x' will recreate the full directory structure.<br><br>

						<b>Q:</b> I have got problems with the installation.<br>
						<b>A:</b> I generally recommend unpacking all three archive to the same destination (e.g. RAM:) and install afterwards.
								  When you will be asked if you want to overwrite some files during unpacking, your answer does not matter.
								  These files are just the same. This way you can install in one go.<br><br>

						<b>Q:</b> When I install a new version, __SFX starts as a demo version. Do I have to pay for the upgrade?<br>
						<b>A:</b> No! All new versions are free for registered users. Paying is strictly voluntarily. To make __SFX easily finding
								  the key-file, it is the best to put it into 'devs:keyfiles/' under the name 'sfx.key'.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
