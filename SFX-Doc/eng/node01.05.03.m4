divert(-1)
define(`__TITLE',`sample options window')
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
						__IMAGE(`win_sampleoptions')
					</center>
					<br>
					<p align="justify">This windows allows to modify options for the sample windows,
					which are described in detail below :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>gadget					</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Draw Mode	</td><td>With this cycle button you can choose how
																						the sample should be drawn. Available are the following modes :<ul>
																						<li>1. Lines</li>
																						<li>2. Dots</li>
																						<li>3. DotAbs</li>
																						<li>4. Filled</li>
																						<li>5. FilledAbs</li>
																						<li>6. FilledHQ (very exact, but slow)</li>
																					</ul></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Loop		</td><td>For switching the loop mode.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Storage		</td><td>Herewith you can specify, if a sample should be kept in memory
																					or swapped to hard-disk. __SFX normally decides this automatically.
																					This can come handy, when you don't need a sample for a while and want
																					to free that memory for other samples.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Channel		</td><td>Determines which channel should be displayed in the window
																					(makes only sense with stereo/quadro samples). Each button corresponds
																					to one channel. Following operations will be limited to these channels only.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Raster X/Y	</td><td>With these check boxes you can disable the drawing of the raster. This speeds up the drawing.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Axis X/Y	</td><td>And with those you can disable the axis. This enlarges the drawing space for the waveform.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Unit X/Y	</td><td>These gadgets are for choosing the unit to be used for each
																					axis. These units will also be used by the <a href="node01.04.__.html">status bar</a>.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Max-Lines	</td><td>These lines show the maximum volume of a sample.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">RMS-Lines	</td><td>The rms-lines show you the real acoustic volume of a
																					sample. Calculating this and also the next may take a while	(for long samples).</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Avg-Lines	</td><td>And these lines show the average volume.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Quick Draw	</td><td>If this is selected, the drawing of raster and Max-,RMS-, Avg-lines will	be left out during scrolling.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">SLen		</td><td>Here you can change the length of the sample. This is
																					necessary should you want to for example do a 'Echo' effect on a short sample
																					that is supposed to be longer than the sample itself. Simply enter desired
																					length and __SFX will add the empty section you requested, giving you the room
																					you need for the effects stuff.<br>
																					Additionally __SFX shows you the length in the current unit.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">SRat		</td><td>To change the playback rate you can choose one of these three options
																					<table border="0">
																						<tr>
																							<td valign="top">pop-up button</td>
																							<td>Will put you into the 
																								<a href="node01.05.04.html">period choice window</a>.
																								The results are then entered into the gadgets at the right.</td>
																						</tr>
																						<tr>
																							<td valign="top">rate gadget</td>
																							<td>Change the playback rate 'directly' by entering
																								the rate in Hz. Higher rates give you higher pitches.
																								8000 to 48000 would constitute normal playback rates.
																								The gadget next to ours will display the note
																								(as seen in trackers) after entering the playback
																								rate. If there is no note equivalent to the period
																								a '---' string is shown.</td>
																						</tr>
																						<tr>
																							<td valign="top">note gadget</td>
																							<td>Here you can enter a desired note, which must comply with this format :<ul>
																									<li>1. char : key="C,D,E,F,G,A,H"</li>
																									<li>2. char : white keys="-", black keys="#"</li>
																									<li>3. char : octave="0,1,2,3,4,5,6,7"</li>
																								</ul>
																								examples : "C#3", "E-0", "H-7"<br>
																								__SFX will display the period (ProTracker) for the
																								chosen note in the corresponding gadget.</td>
																						</tr>
																					</table>
																					If you change the rate during the active-buffer plays, you will
																					hear the changes immediately.</td></tr>
					</table>
					</p>
					<p align="justify">Further you can change the strings, which are saved with the sample in some file formats.
					</p>
					<p align="justify">A click at "Okay" closes the window and one at "Reset" restores the settings to the choices
					made in the <a href="node01.06.02.html">sample window preferences</a> (please note that the parameters "SLen"
					and "SRat" are not restored).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
