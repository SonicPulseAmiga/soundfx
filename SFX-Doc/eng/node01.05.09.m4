divert(-1)
define(`__TITLE',`record window')
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
						__IMAGE(`win_record')
					</center>
					<br>
					<p align="justify">__SFX naturally offers functionality for recording own sounds
					from external sources (e.g. microphone). __SFX uses AHI for recording.
					If you want to record audio directly from a CD then please have a look at the
					 <a href="node02.02.01/CDDA-Direct_L.html">CDDA-Loader</a>.<br>
					This window offers the following functions :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>gadget        				</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">AHI Record Mode	</td><td>choose an audio-mode for recording.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Record Source	</td><td>a list of available recording sources.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Record Gain		</td><td>for adjusting the record gain.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Record Auto		</td><td>This is a special feature of __SFX.
																							Move the gain to full right and activate 'Auto'.
																							Now __SFX will lower the gain continously,
																							until it does not clip anymore.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Monitor Source	</td><td>a list of monitor-outputs .</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Monitor Gain	</td><td>for adjusting the monitor volume.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Level Meter		</td><td>These level-meters show the volume of the input signal.
																							The red bars mark the maximum value. The values to the
																							right of the meters show the current input and maximum.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Status			</td><td>Shows how much data already has been recorded.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Reset			</td><td>for resetting the maximum-display.</td></tr>
					</table>
					</p>
					<p align="justify">A click on "Record" obviously starts recording, where a click on "Stop" ends it.
					When __SFX records audio the level-meters are inactive to save processing power.
					</p>
					<p align="justify">Please note, that AHI currently always records in stereo 16 bit format.
					Future version might record in mono as well. For __SFX there is currently no easy workaround available,
					other than using the convert-channels operator afterwards.<br>
					Another problem is, that some of you may not be able to use the gain-sliders. The reason for it is that
					the recording-hardware and/or the AHI-driver does not support this.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
