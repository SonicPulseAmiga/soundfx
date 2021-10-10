divert(-1)
define(`__TITLE',`Aliasing')
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
					<p align="justify">Wenn man einen Klang aufnimmt, muß man eine <a href="node06.03.00.Samplingrate.html">Samplingrate</a>
					auswählen, die hoch genug ist auch die höchste Frequenz im Klang zu unterstützen. Sonst erzeugt man Aliasing.
					Dies bedeutet, das Frequenzen welche zu hoch sind (über der Hälfte der Samplingrate) an dieser gespiegelt werden.
					Eine Frequenz welche also ein bischen zu hoch ist, taucht dann ein bischen unterhalb der Grenze auf.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
