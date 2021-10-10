divert(-1)
define(`__TITLE',`fette analoge Lead-Sounds')
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
						<li>einfach (mono)<ul>
							<li>generieren Sie z.B. ein Sägezahnsample in Synthesize_Add</li>
							<li>rufen Sie Chorus_Phaser mit dem "Fat1" Preset auf</li></ul>
						</li>
						<li>etwas aufwendiger (stereo)<ul>
							<li>generieren Sie z.B. ein Sägezahnsample in Synthesize_Add</li>
							<li>benutzen Sie ConvertChannels mit dem "MonoToStereo" Preset</li>
							<li>öffnen Sie die Sampleoptionen und schalten Sie den zweiten Kanal ab</li>
							<li>rufen Sie den Chorus_Phaser mit dem "Fat1" Preset auf</li>
							<li>öffnen Sie die Sampleoptionen, schalten Sie ersten Kanal ab und den zweiten an.</li>
							<li>starten Sie Chorus_Phaser mit dem "Fat2" Preset</li>
							<li>öffnen Sie die Sampleoptionen und schalten Sie den ersten Kanal ein</li></ul>
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
