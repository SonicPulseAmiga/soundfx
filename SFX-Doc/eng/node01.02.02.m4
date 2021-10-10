divert(-1)
define(`__TITLE',`Module menu')
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
					<p align="justify">You find this menu in the settings windows of the <a href="node02.__.__.html">modules</a>.</p>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>main menu     </th><th>sub menu      </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Project       </td><td>Load          </td><td>load settings</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Save          </td><td>save current settings</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Start         </td><td>start the current module</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Reset         </td><td>reactivate last settings</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Default       </td><td>reset to initial settings</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Help          </td><td>open help about the current module</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>About         </td><td>open an information window</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Quit          </td><td>close the module</td></tr>
					</table>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
