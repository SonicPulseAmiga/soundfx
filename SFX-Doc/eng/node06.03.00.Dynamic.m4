divert(-1)
define(`__TITLE',`Dynamic')
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
					<p align="justify">The dynamic meassures the span between the biggest and
					smallest amplitude (<a href="node06.03.00.Volume.html">volume</a>) of the signal.
					Usually it is been given in dezibel (db).<br>
					Music with a high dynamic requires a recording device which can capture this (means
					devices with high <a href="node06.03.00.Bitresolution">bitresolution</a>).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
