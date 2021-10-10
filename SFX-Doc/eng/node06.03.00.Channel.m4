divert(-1)
define(`__TITLE',`Channel')
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
					<p align="justify">
					One sound can consist of several single sounds, which are played back on separate speakers
					to generate spatial audio. Below are listed a few types :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>name          </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Mono          </td><td>only one channel and therefore no spacial information.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Stereo        </td><td>two separate channels (right and left)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Quattro       </td><td>four separate channels<ul>
																			<li>front left, front right, back left, back right</li>
																			<li>left, right, front, back</li>
																		</ul></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Pseudo Quattro</td><td>consists out of 3 or 4 channels<ul>
																			<li>3 : front left, front right, back middle</li>
																			<li>4 : front left, front right, back left, back right</li>
																		</ul>
																		This can be realised by a special connection scheme of 3 or 4 speakers
																		with a stereo-source.
																		</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Surround      </td><td>consists out of  4 or 5 channels<ul>
																			<li>4 : front left, front center, front right, back center</li>
																			<li>5 : front left, front center, front right, back left, back right</li>
																		</ul>
																		This version can be realised by a special connection scheme of 4 speakers
																		with a stereo-source. Much better results you woould gain with a real
																		decoder though.
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
