divert(-1)
define(`__TITLE',`Sample')
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
					<p align="justify">A sample is digitaly recorded audiodata. The name comes from the fact that we take probes,
					which are called "samples" as well.
					They are recorded with a device called sampler (which are exist in different version, from cheap to very
					expensive) and the process is called sampling or digitizing or in the technical sense as
					<a href="node06.03.00.Quantisation.html">quantisation</a>.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
