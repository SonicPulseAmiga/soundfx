divert(-1)
define(`__TITLE',`Warps')
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
						<li><a href="../../workshop/Warp1.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Noise<ul>
							<li>len=1.0 s</li></ul>
						</li>
						<li><a href="../../workshop/Warp2.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Chorus/Phaser</li>
						<li><a href="../../workshop/Warp3.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Synthesize_Add<ul>
							<li>generieren Sie eine halbe Sinus-Periode</li></ul>
						</li>
						<li><a href="../../workshop/Warp4.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Slide<ul>
							<li>source=sine</li>
							<li>slidedist=0 ... -25000, Linear</li></ul>
						</li>
						<li><a href="../../workshop/Warp5.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Amplify<ul>
							<li>maxvol</li></ul>
						</li>
						<li><a href="../../workshop/Warp6.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Detune<ul>
							<li>source=phaser-noise</li>
							<li>detune=0.1 ... 2.0, User 0, ModSource=half_sine, ModMode=stretch</li></ul>
						</li>
						<li><a href="../../workshop/Warp7.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Amplify<ul>
							<li>amplification=0 ... maxvol, User 0, ModSource=half_sine, ModMode=stretch</li></ul>
						</li>
						<li>eventuell Filter-LowPass (als Boost), Delay, ....</li>
						<li>mit Synthesize_Add nochmal einen halben Sinus generieren</li>
						<li>mit Resample diesen auf Länge ca. 2000 bringen</li>
						<li>Amplify<ul>
							<li>amplification=65 ... 105, User1, ModSource=half_sine</li></ul>
						</li>
						<li>mit Panorama2D und z.B. dem "RightToLeft"-Preset kann man einen Stereo-Variante erzeugen</li>
					</ul>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
