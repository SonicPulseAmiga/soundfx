divert(-1)
define(`__TITLE',`Window funtion selection')
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
						__IMAGE(`fx_winfuncsel')
					</center>
					<br>
					<p align="justify">FX which are using digital filters or utilizing the fast Fourier
					tansformation (FFT), need a window-function. After a click onto the popup-symbol appears
					the <a href="node01.05.05.html">window functions window</a> where you can choose one.<br>
					The textbox right to the popup-symbol shows a shortened version of the active settings.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
