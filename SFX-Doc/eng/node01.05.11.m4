divert(-1)
define(`__TITLE',`batch processor status window')
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
						__IMAGE(`win_batchprogress')
					</center>
					<br>
					<p align="justify">In this window the progress of the batch processing will be shown.
					This happens in three areas. The upper one gives a main overview. The field
					&quot;Stack&quot; tells how many files are in the queue. This number may raise during
					the operation, if further subdirectories are found. The filed &quot;Done&quot; counts the
					samples which have been processed and the field &quot;File&quot; informs about the
					current sample. The two status bars below show the progress for the current file and
					for the current operation.
					</p>
					<p align="justify">The calculation can be stopped with one click at "Stop",
					pressing the keys "S","s","ESC"	or a click at the "Close"-gadget of the window.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
