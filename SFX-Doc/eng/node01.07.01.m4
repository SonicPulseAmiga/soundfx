divert(-1)
define(`__TITLE',`Curve')
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
						__IMAGE(`mod_curve')
					</center>
					<br>
					<p align="justify">This modulator generates a bended course. The bend can be adjusted with the
					parameter "exponent" and will be displayed graphically or can be changed with the
					mouse by dragging it to a desired shape.
					Below some examples :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>variation        				</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Linear (exp=1.0)	</td><td>Runs values from 0.0 at sample start to 1.0 at the end of
																							the sample in the linear way</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">SpeedUp (exp>1.0)	</td><td>Similar to the above but the values run at "slow" rate and "faster" to the end</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">SlowDown (exp<1.0)	</td><td>Opposite of "SpeedUp"</td></tr>
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
