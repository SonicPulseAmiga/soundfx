divert(-1)
define(`__TITLE',`Bitresolution')
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
					<p align="justify">The bitresolution determines, with which pressicion the analogue
					audiodata has been capured. The higher the bitresolution is, the smaller the errors made by
					digitizing are (quantisation error) and the more authentic the <a href="node06.03.00.Sample.html">sample</a> would sound.
					Common bitresolutions are 8-, 12-, 16-bit und 24-bit. Below a small table with resolution,
					resulting range and usual application :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>bits            </th><th colspan="3">range                                               </th><th>application</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">8 </td><td align="right">    -128</td><td>...</td><td align="right">    127</td><td>home, multimedia</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">12</td><td align="right">   -2048</td><td>...</td><td align="right">   2047</td><td>home, multimedia</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">14</td><td align="right">   -8192</td><td>...</td><td align="right">   8191</td><td>semiprofesionell area</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">16</td><td align="right">  -32768</td><td>...</td><td align="right">  32767</td><td>semiprofesioneller area, homestudio</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">24</td><td align="right">-8388608</td><td>...</td><td align="right">8388608</td><td>professional studio</td></tr>
					</table>
					One can clearly see, that already adding one bit, dramatically increases the range
					and therewith quality.<br>
					The amiga audio hardware normaly only support playback with 8 bits. By using a trick though,
					you can get 12-bit or even 14-bit.<br>
					You can easilly notice the difference, by doing as following :<ul>
						<li>load a 16-bit sample (for an 8-bit sample ofcourse both players do sound the same),
						use a sample with a long decay (e.g. a basedrum, which becomes quite deep to the end).</li>
						<li>pülay the sample with high volume with both players (eventually use headphones).</li>
					</ul>
					Noticed the difference in the end?
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
