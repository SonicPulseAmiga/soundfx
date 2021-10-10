divert(-1)
define(`__TITLE',`Loop')
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
						Loops are used to repeat parts of a <a href="node06.03.00.Sample.html">sample</a>.
						This can be uused to sustain a sound during that phase longer.<br>
						The start- and end-point of a loop should lay on a zero crossing (or atleast on simmiliar levels)
						to avoid cracks.
						On the <a href="node01.03.09.html">range-toolbar</a> you find function to adjust loops-markers.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
