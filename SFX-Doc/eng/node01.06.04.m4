divert(-1)
define(`__TITLE',`miscellaneous preferences')
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
				<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
					<center>
						__IMAGE(`prefs_misc')
					</center>
					<br>
					<p align="justify">In this window you can change some more settings. Here's a description of the
					various buttons and functions :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>button						</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Sig-Audio		</td><td>If activated, a signal-sound indicates that calculations are complete</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Sig-Screen		</td><td>If activated, will make to __SFX&#39;s screen pop to the front
																						when a calculation has been finished.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Real-Mem		</td><td>Should the free memory and the largest available memory block
																						be displayed in the title-bar?</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Virt-Mem		</td><td>Should the free virtual memory (space on your hard-disk in the swap
																						directory) be displayed in the title-bar?</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Background Logo	</td><td>If checked, a __SFX logo will appear in the screen background</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Source Select	</td><td>Which way do you want to select source-samples (e.g. in operator-windows)?</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Context Button	</td><td>Which mouse-button __SFX should use for popup-menus</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Window Mode		</td><td>The <a href="node01.03.10.html">window-mode</a> __SFX uses initialy</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Ask Exit		</td><td>How __SFX should behave on exit</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Logging			</td><td>Specify what __SFX should log and choose the path of the logfile.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Save Icons		</td><td>Should __SFX create icons when saving samples?</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">DefTool			</td><td>The default tool is the application that gets started, when one double-clicks a file icon.
																							__SFX can keep the default tool of the icon, enter __SFX if no entry exists or always enter __SFX.
																						</td></tr>
					</table>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
