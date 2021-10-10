divert(-1)
define(`__TITLE',`preferences for the sample windows')
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
						__IMAGE(`prefs_sample')
					</center>
					<br>
					<p align="justify">In this window you can change various parameters regarding to sample projects.
					Here&#39;s a description of the various buttons and functions :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>button					 </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Loader/Saver Path	 </td><td>These are the default paths for the file-requests for loading and saving samples.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Axis Font	 </td><td>This font will be used for the rulers inside the sample window.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Safe Check	 </td><td>Here you can choose, how the program should prevents
																					you from discarding unsaved samples :<ul>
																						<li>never : request appears never</li>
																						<li>if unsaved : request appears only if the sample has not been saved yet</li>
																						<li>always : request appears ever</li>
																					</ul></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Storage		 </td><td>Herewith you can specify, if a sample should be kept in memory
																					or swapped to hard-disk. __SFX normally decides this automatically.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Draw Mode	 </td><td>This cycle-gadget lets you choose a drawing style for the sample waveform.
																					This is you choice :<ul>
																						<li>1. Lines</li>
																						<li>2. Dots</li>
																						<li>3. DotAbs</li>
																						<li>4. Filled</li>
																						<li>5. FilledAbs</li>
																						<li>6. FilledHQ (very exact, but slow)</li>
																					</ul></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Quick Draw	 </td><td>If this is selected, the drawing of raster and Max-, RMS- and AvgLines	will
																					be switched off during scrolling.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Raster X/Y	 </td><td>With these check boxes you could disable the drawing of the raster.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Axis X/Y	 </td><td>And with those you can disable the axis. This enlarges the drawing space for the waveform.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Unit X/Y	 </td><td>These gadgets are for choosing the unit to be used for each axis. This unit
																					will also being used by the <a href="node01.04.__.html">status-bar</a>.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Max-Lines	 </td><td>You could disable the calculation of the max. amplification lines. This speeds
																					up the drawing, especially of longer samples.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">RMS-Lines	 </td><td>These lines are showing you the real acoustic volume of a sample. Calculating
																					this and also the next may take a while (for long samples.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Avg-Lines	 </td><td>These lines are showing you the real acoustic volume of a sample. Calculating
																					this and also the next may take a while (for long samples).</td></tr>

						<tr bgcolor=__COL_TBL_ROW1><td valign="top">No Lines	    </td><td>Should __SFX leave out Max-, RMS- and AvgLines for long samples?</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Size Threshold</td><td>Tell __SFX here what you regard as long samples (number of sample-values)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Window Size	 </td><td>Finally you can choose the default sizes of sample windows here. This
																					can be done by entering absolute values (in pixel) or entering relative values
																					(which are in per thousand of the screensize)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Info Strings	</td><td>Here you can change the comments, which are saved along with the samples.</td></tr>
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
