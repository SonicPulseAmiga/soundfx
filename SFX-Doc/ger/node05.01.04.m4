divert(-1)
define(`__TITLE',`Snaredrums')
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
						<li><a href="../../workshop/SnareDrum1.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Noise aufrufen<ul>
							<li>len="0.5 s"</li></ul>
						</li>
						<li><a href="../../workshop/SnareDrum2.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Amplify<ul>
							<li>amplification=20 % ... 20 %, None</li></ul>
						</li>
						<li><a href="../../workshop/SnareDrum3.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Synthesize_Add<ul>
							<li>len="0.5 s"</li>
							<li>pitch=130.8127.... Hz (C-1)</li>
							<li>waveform=sin</li>
							<li>1. Slider auf max (=1), rest to 0.0</li>
							<li>Frequency=1.0 ... 0.1, Curve mit fc=0.5</li>
							<li>Amplitude=1.0 ... 1.0, None</li></ul>
						</li>
						<li><a href="../../workshop/SnareDrum4.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Slide<ul>
							<li>Source=leises Rauschen</li>
							<li>slidedistance=-25000 ... 25000, User 0, ModSource=Sinus</li></ul>
						</li>
						<li><a href="../../workshop/SnareDrum5.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Amplify<ul>
							<li>kurzer Attack</li>
							<li>ca. 2/3 der Hüllkurve sehr leise, dabei relativ abrupter Wechsel von laut zu leise</li></ul>
						</li>
						<li>=&gt; Version1 ist fertig</li>
						<li>weiterhin Amplify<ul>
							<li>amplification=150 ... 100, Linear, Wrap=Wrap2</li></ul>
						</li>
						<li>oder Filter-LowPass<ul>
							<li>Effektanteil=-100 ... -100, None</li>
							<li>range/cut-off=9.0 ... 9.0, None</li></ul>
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
