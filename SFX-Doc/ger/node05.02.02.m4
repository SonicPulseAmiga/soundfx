divert(-1)
define(`__TITLE',`Technosounds')
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
					<ul>
						<li>erstellen Sie zwei Samples gleicher Länge mit Synthesize_Add (können unterschiedlich hoch sein und sollten verschieden klingen)</li>
						<li>generieren sie eine Rechteckschwingung der selben Länge mit Synthesize_Add wählen Sie dafür eine hohe Frequenz (C-4/C-5)</li>
						<li>die einzelnen Frequenzen sollten zueinander passen (Akkord/oktavenweise gestimmt)</li>
						<li>rufen Sie Mix auf</li>
						<li>stellen Sie als Sources die ersten beiden Synthsounds ein</li>
						<li>setzen Sie die Mixparameter auf "100 %" und "0 %"<ul>
							  <li>Blendshape 		: User1</li>
							  <li>Modulationsbuffer : Rechtecksound</li>
							  <li>Modulationsmode 	: Single</li></ul>
						</li>
					</ul>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
