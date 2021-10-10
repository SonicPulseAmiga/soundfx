divert(-1)
define(`__TITLE',`Vector')
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
						__IMAGE(`mod_vector')
					</center>
					<br>
					<p align="justify">This modulator generates an envelope with a maximum of 20 segments.
					With "+" and "-" you can add and remove points.
					With "FlipX" and "FlipY" you can mirror the envelope.
					In "Nr" you can directly choose a point and position it in the next two
					fields. Of course, you can use the mouse as well, to move the points.<br>
					This modulator supports presets. Therewith you can store generated envelopes and recall
					them again (they are available in all operators then).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
