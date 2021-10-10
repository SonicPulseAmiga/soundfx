divert(-1)
define(`__TITLE',`Problems')
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
						<b>Q:</b> When I play back files from HD, then the sound is interrupted by cracks.<br>
						<b>A:</b> At first use a separate partition for swapped files (choose in prefs/vmem). Further
								  use a big block size for this partition (changeable in HDToolBox etc). I
								  recommend 8192..16384 Bytes. WARNING : Changing the block-size will destroy
								  all data on this partition.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
