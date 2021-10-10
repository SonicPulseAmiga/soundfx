divert(-1)
define(`__TITLE',`Where does SoundFX run ?')
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
					<p align="justify">The program runs on all AMIGA computers with AmigaOS
					3.0 or greater.
					I have stopped building the version for plain 68000 system (but could
					immediately do this again, if there is really someone who need it).
					As some effects rely heavily on CPU power (or FPU for those who have
					it:) and the GUI can become quite complex, an accelerator card (with
					a FPU) is recommended. In addition to this, memory usage can increase
					greatly with use of 16/32 bit processing. Finally, a graphic-card helps
					to prevent loosing overview.
					</p>
					<p align="justify">__SFX can be used on MorphOS systems (with 68k emulation)
					and on Amiga-emulators (Amithlon and UAE).
					</p>
					<p align="justify">Ideally you system should look like mine - then it
					would be unlikely that __SFX will not work ;-). This would then be an A2000
					with a 060 based board (64 Mb RAM) and SCSI controller, graphic-board (PicassoIV),
					sound-card (Prelude &amp; Repulse) and OS3.5.
					</p>
					<p align="justify">
					Further I recommend installing the programs listed below for an enhanced GUI and
					increased productivity :<ul>
						<li>MagicMenu</li>
						<li>ReqAttack</li>
						<li>VisulaPrefs (this is definitely worth registering too)</li>
					</ul></p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
