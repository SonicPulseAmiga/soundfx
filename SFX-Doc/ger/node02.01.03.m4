divert(-1)
define(`__TITLE',`Interpolator')
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
				<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
					<center>
						__IMAGE(`fx_intsel')
					</center>
					<br>
					<p align="justify">Effekte die auf Samples zwischen zwei Samplewerten zugreifen
					müssen nutzen dazu einen Interpolator. Nach einem Click auf das Popup-symbol erscheint das
					<a href="node01.05.06.html">Interpolationstyp-Fenster</a> in dem sie einen solchen auswählen können.<br>
					In der Textbox rechts neben dem Popup-Symbol wird die Kurzform der aktuellen Einstellung angezeigt.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
