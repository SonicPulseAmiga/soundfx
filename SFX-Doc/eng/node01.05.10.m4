divert(-1)
define(`__TITLE',`batch processor window')
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
						__IMAGE(`win_batch')
					</center>
					<br>
					<p align="justify">
					The batch processor allows you to apply a couple of operations (loading, processing, saving) to a whole directory of samples.
					Therwith you can run a set of processes onto many and/or long files automatically.<br>
					Have a look at the presets for examples of operation.
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Range        						</th><th>Description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Batch List				</td><td>This list always consists of one <a href="node02.02.__.html">loader</a> and one <a href="node02.05.__.html">saver.
																									Inbetween you can add as many <a href="node02.01.__.html">operators</a> as you like.
																									Furthermore you can assign a preset to each operation.
																								</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Options					</td><td>Here you can choose, if you want __SFX to log the execution to a file and specify to which file the log should go to.<br>
																									Additionally you can ask __SFX to recursively descend directories.
																									A depth of &quot;-1&quot; means &quot;unlimited depth&quot;.
																									This will cause all files to be processed.<br>
																									Finally you can choose that __SFX deletes the source files after processing.
																									This saves space on you harddisk, but be sure to have the files backed up somewhere else.
																								</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Presets					</td><td>Like the <a href="node02.01.05.html">preset-selection</a> in the operator-windows,
																									you can save the setting made at the left side and quickly recall them.
																								</td></tr>
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
