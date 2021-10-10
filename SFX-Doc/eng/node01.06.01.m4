divert(-1)
define(`__TITLE',`Preferences for the GUI')
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
						__IMAGE(`prefs_gui')
					</center>
					<br>
					<p align="justify">In this window you can change various parameters related to the __SFX-GUI. Below comes a
					description of the various buttons and functions :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>button        				</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Palette	(left)	</td><td>Choose a color in the palette which you want to change.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Red,Green,Blue	</td><td>Change the individual color components for the chosen color.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Screen			</td><td>In the following window you can choose a screen mode for
																						your __SFX screen (displays only useful modes).<br>
																						Please note, that when choosing highcolor (15/16 bit) or truecolor (24 bit) graphic modes
																						the presentation of the samples can look a bit different
																						(marked ranges and loops).</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Scr. Font		</td><td>Here you can choose a font for the layout. Now non-proportional
																						fonts are available too. But they might sometimes lead to too wide
																						windows and gadgets. The default font (Trinomic.font) is only
																						6-points high and is necessary if you want to use __SFX on a
																						Hires-NoLace-Screen (640x256). On a resolution of 1024x768
																						I use XHelvetica with size 11.<br>
																						Do only use larger fonts, if you have chosen a higher
																						screen-resolution</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Stat. Font		</td><td>This font gets used for the status bar fields. I recommend
																						using a fairly small font like e.g. Tinomic in size 6 or
																						XHelvetica in size 9.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Palette	(right) </td><td>Here you can choose a color which you want to assign to a pen.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Pens			</td><td>Choose a pen which you want to change.</td></tr>
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
