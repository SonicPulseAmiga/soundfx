divert(-1)
define(`__TITLE',`Basedrums')
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
						<li><a href="../../workshop/BaseDrum1.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Synthesize_Add aufrufen<ul>
							<li>len="0.1 s"</li>
							<li>pitch=130.8127.... Hz (C-1)</li>
							<li>waveform=sin</li>
							<li>1. Slider auf max (=1), rest to 0.0</li>
							<li>Frequency=1.0 ... 0.0001, Curve mit 2.0</li>
							<li>Amplitude=0.0 ... 1.0, Vector mit 5 Punkten<ul>
								<li>0 : Pos=0.0 , Lev=0.0</li>
								<li>1 : Pos=0.02, Lev=1.0</li>
								<li>2 : Pos=0.15, Lev=1.0</li>
								<li>3 : Pos=0.2 , Lev=0.3</li>
								<li>4 : Pos=1.0 , Lev=0.0</li></ul>
							</li></ul>
						</li>
						<li>wer es ein bi&szlig;chen härter mag, ...</li>
						<li><a href="../../workshop/BaseDrum2.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Amplify<ul>
							<li>amplification=150 % ... 100 %, Curve mit fc=1.0</li>
							<li>wrap=Clip</li></ul>
						</li>
						<li><a href="../../workshop/BaseDrum3.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> oder<ul>
							<li>amplification=200 % ... 100 %, Curve mit fc=1.0</li>
							<li>wrap=Wrap2</li></ul>
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
