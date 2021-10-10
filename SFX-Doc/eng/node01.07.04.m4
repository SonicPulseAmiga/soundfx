divert(-1)
define(`__TITLE',`User defined')
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
						__IMAGE(`mod_user')
					</center>
					<br>
					<p align="justify">This modulator allows using a sample project as modulation source. Below a list of available
					control types :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>variation       		</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Normal	</td><td>If the amplitude of the modulation buffer has reached its
																				negative maximum then this returns the value 0.0 and at the positive maximum 1.0.
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Abs		</td><td>Pretty much the same as 'Normal' with one difference. Sample
																				data of the value 0 (flat line:) gives you values of 0.0 for the
																				modulation curve, maximum negative or positive amplitude of the sample a 1.0.
																				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">AmpEnv	</td><td>This shape gives you the volume envelope of the modulating
																				sample (imagine you stretch a rubber band around the sample)
																				</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">FrqEnv	</td><td>This shape returns the pitch envelope of the modulating sample.</td></tr>
					</table>
					</p>
					<p align="justify">Eventually there are different algorithms available for AmpEnv and FrqEnv . These can then be chosen with
					the cycle-gadget labeled "Env".
					</p>
					<p align="justify">The sample buffer you want to use for the modulation curve can be of different
					length than your to-be-modulated sample. How to handle this is described her :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>variation       		</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Single	</td><td>If the sample is shorter, the rest will be filled with silence.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Repeat	</td><td>If the sample is shorter, it will be repeated for as many times as needed.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Stretch	</td><td>The sample will be stretched/shrinked to fit exactly.</td></tr>
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
