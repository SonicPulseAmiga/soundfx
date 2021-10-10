divert(-1)
define(`__TITLE',`Features')
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
					<p align="justify">
						<b>Q:</b> Will __SFX support virtual memory?<br>
						<b>A:</b> Yes, it is available now with V3.70<br><br>

						<b>Q:</b> Will __SFX support the DSP on the Delfina Soundboard?<br>
						<b>A:</b> Probably not, because this means lot of work to me and I don't have the time for it.<br><br>

						<b>Q:</b> Will there be a __SFX with PPC support?<br>
						<b>A:</b> I'll try to do this, but can't promise anything yet. A prerequisite is that I can buy a modern PPC based AMIGA system.<br><br>

						<b>Q:</b> Will __SFX support MPEG Files? Will __SFX support RealAudio files?<br>
						<b>A:</b> MPEG can be loaded and saved. With RealAudio I have my doubts.<br><br>

						<b>Q:</b> Will __SFX support support recording in the near future ?<br>
						<b>A:</b> It does it since version 4.00.<br><br>

						<b>Q:</b> Will there be __SFX for Windows/Linux/MorphOS/... ?<br>
						<b>A:</b> Such things are as simple as it appears to some people. If there is something in work I will give notice.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
