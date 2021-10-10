divert(-1)
define(`__TITLE',`Sample')
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
					<p align="justify">Als Sample bezeichnet man digital aufgezeichnete Audiodaten. Der Name kommt
					aus dem englischen, wobei ein "Sample" eine Probe ist.
					Aufgenommen werden Sie mit einem Sampler (in den verschiedensten Ausführunge erhältlich;
					von billig bis sehr teuer) und den Vorgang bezeichnet man als Sampling oder
					Digitalisierung bzw. im technischen Sinne als <a href="node06.03.00.Quantisation.html">Quantisierung</a>.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
