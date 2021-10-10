divert(-1)
define(`__TITLE',`Envelope')
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
					<p align="justify">An envelope is a segmented curve with a minimal level of e.g. 0.0
					and a maximum level of 1.0. Such a curve is used to <a href="node06.03.00.Modulation.html">modulate</a>
					a parameter of an effect. Below an example :
						<!-- @TODO: add graphics -->
					If you would e.g. modulate the volume of a sample by this curve, then it would
					become lounder in the beginning, reaches then its maximum and would then fade
					to silence slowly towards the end.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
