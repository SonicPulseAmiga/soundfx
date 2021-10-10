divert(-1)
define(`__TITLE',`Usage')
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
						<b>Q:</b> How can I make a sample window forget a range I marked, without 
								  cutting or copying or anything<br>
						<b>A:</b> You can select it from range-menu or with short-cut Amiga-H.<br><br>

						<b>Q:</b> I am very much intrigued as to why we have control over the vertical
								  component of ranges? Can we grab only peaks of samples?<br>
						<b>A:</b> This is currently only for zooming, i.e. you can roughly mark a range,
								  let __SFX extend this that it optimally encloses the peak values
								  and then enlarge the area.<br><br>

						<b>Q:</b> Having a keyboard shortcut for starting an operator would be nice (not Amiga-r,
								  but something to start the calculation in the operator).<br>
						<b>A:</b> There is one. Press "Enter/Return".<br><br>

						<b>Q:</b> How can I select the whole sample?<br>
						<b>A:</b> Again by using the range-menu or with the shortcut Amiga-A.<br><br>

						<b>Q:</b> About the Del key? I am used to use it for Cut operations, just like
								  SoundForge, CoolEdit, and also word-processors have it.<br>
						<b>A:</b> I think the best would be to make all short-cut user definable. Now __SFX
								  uses Amiga-x for "Cut" like all good Amiga-programs do.<br><br>

						<b>Q:</b> If you have a 600 Mb file that you want to process in __SFX, but not
								  room for a second (or third, or fourth) 600 Mb file, how do you
								  handle it?  The method of making a new sample every time you
								  apply an effect works well for short samples, but it is a problem for
								  whole recordings.<br>
						<b>A:</b> At first - short samples - that is what __SFX is for mainly. As it was
								  often asked for, __SFX learned how to cope with long recordings.
								  If there is no room left in memory, __SFX tries to swap out onto hd and
								  if even there is no space, the operation fails.<br>
								  If you have alternative ideas, they are very welcome. Just to mention
								  it, I already thought about reusing the space of the source samples
								  for the result. This would work with most effects, but not with all and is
								  sometimes tricky to handle too.<br><br>

						<b>Q:</b> Opening some program windows causes bad refreshing of
								  sample-windows when they are resized. If you open eg. a loader
								  prefs window, and then attempt to resize a sample-window,
								  its contents is not properly refreshed. The refresh is
								  completed only after you have closed the loader prefs window.<br>
								  Operator windows DO NOT cause this!<br>
						<b>A:</b> The operators are started as separated tasks, most other windows are not.
								  As I don't run them asynchronously, all events you cause
								  for other windows are queued until you close the blocking window.<br>
								  I don't know if it is worth the work, to make them all asynchronous.<br><br>

						<b>Q:</b> Will the new batch facility allow me to convert them all to WAV in one
								  go, recursively going through the drawers?<br>
						<b>A:</b> Yes, as this is exactly what it is for.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
