divert(-1)
define(`__TITLE',`Dynamik')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_ger.m4')
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
					<p align="justify">Die Dynamik ist die Spanne zwischen der größten und der kleinsten Amplitude
					(<a href="node06.03.00.Volume.html">Lautstärke</a>) des Signals.
					Sie wird meist in Dezibel (db) angegeben.<br>
					Musik mit einer hohen Dynamik erfordert auch Aufnahmegeräte die dies erfassen können
					(also Geräte mit hoher <a href="node06.03.00.Bitresolution">Bitauflösung</a>).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
