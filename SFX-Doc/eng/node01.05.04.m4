divert(-1)
define(`__TITLE',`period choice window')
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
						__IMAGE(`win_period')
					</center>
					<br>
					<p align="justify">In this window you could select the <a href="node06.03.00.Samplingrate.html">sampling rate</a>. You can do it
					in the following ways :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>method        				</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">mouse		</td><td>Simply click on the wished note in the keyboard. Rate
																						and referring note will be shown in the fields below.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">keyboard	</td><td>Choose with F1-F5 the octave and select the
																						pitch with the following keys :<center><font face="courier">
																						 s d   g h j<br>
																						y x c v b n m
																						</font></center></td></tr>
					</table>
					</p>
					<p align="justify">Below the keyboard-image you see the rate, note and frequency. With the cycle-gadget
					below you can choose between the often used rates.
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>sampling rate 				</th><th>typical application</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 8000 Hz</td><td align="left">Sound boards (typical for SND-AU samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">11025 Hz</td><td align="left">Sound boards (typical for old samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">22050 Hz</td><td align="left">Sound boards (typical frequency oft most samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">28867 Hz</td><td align="left">maximum playback rate of the Paula-chip in normal mode</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">32000 Hz</td><td align="left">Consumer DATs and samplers</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">44100 Hz</td><td align="left">CD-Player</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">48000 Hz</td><td align="left">DAT-Recorder/Player</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">57734 Hz</td><td align="left">maximum playback rate of the Paula-chip in productivity mode</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">96000 Hz</td><td align="left">high quality audio processing</td></tr>
					</table>
					</p>
					<p align="justify">With the cycle gadget PlayMode, you can choose, if you would like
					to listen to the sample while choosing the playback rate. If you have chosen PlayMode=PlayAll
					and click onto the keyboard panel, you will immediately hear the sound in the respective tune.
					Of this only aplies to selecting the playback-rate of an already existing sound (when
					choosing the playback rate in operators like Noise, then there has not yet been calculated anything). 
					</p>
					<p align="justify">After clicking onto Okay the values will be accepted.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
