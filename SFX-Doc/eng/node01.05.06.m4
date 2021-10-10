divert(-1)
define(`__TITLE',`interpolation type window')
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
						__IMAGE(`win_interpol')
					</center>
					<br>
					<p align="justify">You can select an interpolationtype and eventually adjust a parameter in this
					window. The effect of the chosen type will be displayed graphically.
					When digitizing a sound, the hardware takes probes after very short intervals. This results
					in the digitized wave-form. But some effects need values between these probe-points. Here too __SFX is flexible
					and offers a rich choice:
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>choice        		</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">None	</td><td>no interpolation (the nearest value will be taken)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Lin		</td><td>linear interpolation</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Si		</td><td>curved interpolation over <n> points</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Lagrange</td><td>curved interpolation over <n> points</td></tr>
					</table>
					For the last two methods it is necessary to specify the size of the interpolation range, which is how many surrounding
					values should be taken into account to calculate an inbetween value. Do not make this too big (bigger that 10).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
