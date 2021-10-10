divert(-1)
define(`__TITLE',`Stereospread')
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
						<li>Sample laden (auch hier macht sich ein Drumloop gut)</li>
						<li>starten sie PitchShift</li>
						<li>w‰hlen sie ein leichtes Vibrato<ul>
							<li>bshape=cyclic, sine, 1 rpts, 0∞</li>
							<li>PitchFactor 0.995 ... 1.005</li></ul>
						</li>
						<li>starten sie PitchShift nochmals und w‰hlen sie wieder das geladenen Sample und ‰ndern sie einfach die Effekteinstellungen etwas, z.B. :<ul>
							<li>bshape=cyclic, sine, 1 rpts, 45∞</li></ul>
						</li>
						<li>rufen sie ChannelJoin auf, um ein Stereosample aus den leicht gepitchten Varianten zu machen<br><br></li>
						<li>!!! versuchen Sie das auch mit anderen Effekten</li>
						<li>beachten sie nur, daﬂ der Effekt nur leicht wirkt (der Effekt soll den Klang nur so leicht ‰ndern, das es noch wie zuvor klingt)</li>
					</ul>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
