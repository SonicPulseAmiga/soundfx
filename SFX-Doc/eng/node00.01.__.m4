divert(-1)
define(`__TITLE',`What is SoundFX ?')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_eng.m4')
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
					<p align="justify">__SFX is an editor for digitized audio data (samples). __SFX
					is built in a modular way and features a comfortable GUI (Graphic User Interface).
					With __SFX you can add effects (which are really unique on AMIGA) to your
					samples and edit them extensively. Think of __SFX as a swiss army knife for sounds!<br>
					Here is an overview of its features :<ul>
						<li>more than 65 <a href="node02.01.__.html">operators</a> with lots of parameters and possibilities for modulation such as :<ul>
							<li>sound synthesis<ul>
								<li>AM-Synthesis (amplitude modulation)</li>
								<li>CS-Synthesis (composite synthesis = additive and subtractive sound synthesis)</li></ul></li>
								<li>FM-Synthesis (frequency modulation)</li>
							<li>3D-cube parameter modulation (Mix, Equalize)</li>
							<li>effects such as Hall, Echo, Delay, Chorus/Phaser, Morph, Pitchshift, Timestretch ...</li>
							<li>operators like Resample, ZeroPass (FadeIn/FadeOut), Middle, Amplify, Mix, DeNoise, ConvertChannels ...</li>
							<li>2D/3D-spectral analysis</li>
							<li>very good filters and boosters with resonance !!!</li>
							<li>several types of modulation<ul>
								<li>even volume and pitch tracking</li></ul></li>
							<li>more then 250 presets included</li></ul></li>
						<li>internal signal resolution of 80/16 bit<ul>
							<li>80 bit floating point during calculations</li>
							<li>16 bit in in the sample buffer</li></ul></li>
						<li>good <a href="node02.03.__.html">play routines</a><ul>
							<li>8 bit standard player</li>
							<li>14 bit cascade player (without additional hardware)</li>
							<li>14 bit calibrated cascade player (without additional hardware)</li>
							<li>AHI-player for sound cards</li>
							<li>plays samples directly from fast-ram or from hard-disk while using max 16 kByte chip-ram during playback</li></ul></li>
						<li>conversion of different sound sample formats<ul>
							<li>IFF-8SVX/16SV/AIFF/AIFC/MAUD,RAW,RIFF-WAV,VOC,SND-AU,...</li>
							<li>with compression support</li></ul></li>
						<li>works now also with samples bigger than available memory</li>
						<li>works in mono, stereo and quadro !!!</li>
						<li>operations are non-destructive, so the source sample will be neither overwritten nor deleted</li>
						<li>extensive number of cut-functions</li>
						<li>freehand-edit</li>
						<li>flexible screen display<ul>
							<li>number of sample buffers is limited by your system resources only</li>
							<li>each sample has its own window, with changeable position and size</li>
							<li>smooth variable zooming (can be <1.0)</li>
							<li>X- and Y-zoom !!</li>
							<li>and rulers with configurable units</li></ul></li>
						<li>HTML online help<ul>
							<li>by pressing the "HELP"-key in any window</li>
							<li>asynchronous (the help window could stay opened)</li></ul></li>
						<li>clipboard support with all 256 entries</li>
						<li>datatype support (loader)</li>
						<li><a href="node03.__.__.html">arexx-port</a><ul>
							<li>with many procedures and functions (actually about 90)</li>
							<li>with several examples</li>
							<li>arexx-scripts can be started directly from __SFX</li></ul></li>
						<li>system conform GUI</li>
						<li>font- and screen-sensitive</li>
						<li>modular concept, means unlimited<ul>
							<li><a href="node02.01.__.html">operators</a> ( 65 at this time)</li>
							<li><a href="node02.02.__.html">loader</a> (19 at this time)</li>
							<li><a href="node02.03.__.html">player</a> (4 at this time)</li>
							<li><a href="node02.04.__.html">rexx-macros</a> (several scripts included)</li>
							<li><a href="node02.05.__.html">saver</a> (15 at this time)</li></ul></li>
						<li>supports AMIGA-specific functions<ul>
							<li>file format information in filenotes</li>
							<li>generation of projekt-icons</li>
							<li>applikation-icon</li></ul></li>
					</ul>
					In the <a href="node00.04.__.html">unregistered version</a> saving of samples and using the arexx-port is not available !
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
