divert(-1)
define(`__TITLE',`preferences for virtual memory')
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
						__IMAGE(`prefs_vmem')
					</center>
					<br>
					<p align="justify">In this window you can change some settings related to virtual memory. Here's a
					description of the various buttons and functions :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>button					</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Enable		</td><td>Should __SFX use virtual memory at all.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Swap Path	</td><td>Enter the default path samples are swapped to or choose the
																						path by clicking on the pop-up. __SFX will use as much space
																						there, as it needs.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Blk Size		</td><td>Size of the i/o-buffer in bytes __SFX will use for drive access.
																						This has nothing to do with your hard-disk&#39;s block-size.</td></tr>
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
