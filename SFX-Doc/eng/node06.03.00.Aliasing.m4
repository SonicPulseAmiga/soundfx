divert(-1)
define(`__TITLE',`Aliasing')
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
					<p align="justify">When recording a sound, one has to choose a <a href="node06.03.00.Samplingrate.html">samplingrate</a>
					high enough to support the highest freuqency in the sound. Otherwise there is aliasing introduced.
					This means, frequencies which are too high (above the half of the samplingrate) are mirrored around it.
					So a frequency which is a little too high reappears a little below the boundary.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
