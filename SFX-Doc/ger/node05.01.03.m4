divert(-1)
define(`__TITLE',`HiHats')
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
						<li><a href="../../workshop/HiHat1.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Noise aufrufen<ul>
							<li>len="0.5 s"</li></ul>
						</li>
						<li><a href="../../workshop/HiHat2.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> Amplify<ul>
							<li>amplification=100 ... 0, Curve mit 1.0</li></ul>
						</li>
						<li><a href="../../workshop/HiHat3.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> nochmal Amplify<ul>
							<li>amplification=100 ... 0, Curve mit 0.5</li></ul>
						</li>
						<li>=&gt; Version 1 ist fertig</li>
						<li><a href="../../workshop/HiHat4.rexx"><img src="../_img/btn.gif" border="0" align="top"></a> für Version2 noch ein Resample<ul>
							<li>Faktor=0.5</li>
							<li>interpol=Lin</li>
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
