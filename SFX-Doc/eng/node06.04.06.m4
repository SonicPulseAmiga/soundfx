divert(-1)
define(`__TITLE',`Loaders')
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
						<b>Q:</b> It would be nice if there were a selection in the loader and saver list-views specifically
								  labeled CDDA load & save. And does __SFX support Motorola byte order CDDA files as well?
								  That would allow conversion of CDDA files from 1 format to the other.<br>
						<b>A:</b> All can be done. __SFX&#39;s <a href="node02.02.01/RAW_L.html">RAW</a> loader has a nice feature - a configurable
								  'auto-dectection' for raw-files. That means you can associate a file extension
								  or a pattern (some id) in the file with a set of options.<br>
								  To load cdda-file automatically in the right way, you would create a cdda-preset
								  and edit it's setting to your desire. e.g.:<br>
								    &nbsp;&nbsp; Format=16 bit signed<br>
								    &nbsp;&nbsp; Endian=Intel<br>
								    &nbsp;&nbsp; Channels=stereo interleaved<br>
								  and associate this with the ending '.cdda'.
								  Then you enable the auto-detection and save this settings as 'default.cfg'. Every time
								  you load a cdda-file via <a href="node02.02.01/Universal_L.html">Universal</a>-loader or <a href="node02.02.01/RAW_L.html">RAW</a>-loader then, the format gets
								  detected properly and converted as said above<br><br>

						<b>Q:</b> It would be nice if you could permanently set the drive unit and
								  device preferences that the <a href="node02.02.01/CDDA-Direct_L.html">CDDA-Direct</a> loader uses.<br>
						<b>A:</b> Just choose your device and unit and save this as 'default.cfg'.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
