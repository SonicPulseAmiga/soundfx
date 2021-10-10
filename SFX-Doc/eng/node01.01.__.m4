divert(-1)
define(`__TITLE',`General')
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
					<p align="justify">Start __SFX by double-clicking the __SFX icon or by
					invoking it from the shell.
					This causes a window to be displayed, which informs you about each phase of the
					starting procedure.</p>
					<p align="justify">After loading the __SFX screen will appear. There all
					action will take place. The screen is a public screen, which means that
					other applications can open windows there too. The public screen is called
					"SFX_PubScreen". Pressing the "Help" Key activates the online-help for the
					active window. At the top screen border, you can see the screen bar:
					<center>
						__IMAGE(`scr_title')
					</center>
					Beside program name and version number, you find information about current memory
					usage here too.</p>
					<p align="justify">On the first start, loading the software takes a bit longer
					as it creates some indexes for the online-help and database-files for the external
					<a href="node02.__.__.html">modules</a>. On subsequent starts the files are only
					regenerated if changes to the installation have been made.
					</p>
					<p align="justify">When starting __SFX from the shell, you can pass filenames
					of audio files as arguments, which will be loaded then as well. Further you
					can enter __SFX as the default tool in icons of sound files. When double-clicking
					such an icon, __SFX and the sound file will be loaded.
					If __SFX already run, new files will be added to it.
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
