divert(-1)
define(`__TITLE',`metallische Sounds')
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
						<li>Noise<ul>
							<li>Länge=0.1 s</li></ul>
						</li>
						<li>oder Synthesize_Add<ul>
							<li>Länge=0.1 s</li>
							<li>hellen Klang erzeugen (Sägezahn)</li></ul>
						</li>
						<li>Länge auf "44100" in den Sampleoptions erweitern</li>
						* Hall aufrufen
							* Early=100
							* Main=100
							* Diff=100
							* Ampf=225 %
						* Amplify
							* maxvol
						* Hall erneut aufrufen
							* Early=100
							* Main=250
							* Diff=175
							* Ampf=200 %
						* Amplify
							* maxvol
						* fertig ist der Mega-hall-gong-sound !
					</ul>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
