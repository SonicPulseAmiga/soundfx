divert(-1)
define(`__TITLE',`Preset selection')
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
						__IMAGE(`fx_cfgsel')
					</center>
					<br>
					<p align="justify">At the right border of nearly all operators you can see a group
					of buttons helping you to manage you presets comfortably. A
					preset is a set of parameters, which you can save for later
					reuse under a expressive name.<br>
					An already existing preset can be activated by performing a
					single click onto that list item. This causes the preset to
					be loaded immediately. A double click starts the calculation.
					The preset name can be changed by entering a new name into the
					input field below the list.<br>
					The 'Add' button saves the current entered value under a new
					name.<br>
					The 'Del' button removes the current selected preset.<br>
					If you save a preset under the name 'default.cfg', then these
					values will be taken as initial settings.
					</p>
					<p align="justify"><b>If you have mode own presets, which are useful
					for others too, the please mail them to <a href="node00.05.__.html">me</a>.</b>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
