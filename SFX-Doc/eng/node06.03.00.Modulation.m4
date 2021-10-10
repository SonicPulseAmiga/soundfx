divert(-1)
define(`__TITLE',`Modulation')
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
						Modulation is a process where a parameter gets controlled by signal.
						This can happen e.g. cyclic by a sine-wave or as well by an <a href="node06.03.00.Envelope.html">envelope</a>.
						In synthesizers one ooften finds so called LFOs (low-frequency-oszillators). These are used as a modulation source, that means they generate
						an slowly osillating signal, which changes a different pparameter (e.g. the pitch).
						An envelope is used to e.g. control the volume.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
