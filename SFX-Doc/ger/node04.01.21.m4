divert(-1)
define(`__TITLE',`Kann diese &lt;...&gt; Datei nicht lesen !')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_ger.m4')
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
					<!-- errUnsupportedSubtype -->
					<p align="justify">__SFX kann diese Variante noch nicht laden. Sie k�nnen mit <a href="node00.05.__.html">mir</a>
					Kontakt aufnehmen und mir eventuell die Datei per email zusenden. Wenn Sie mir bei der Informationsrecherche bez�glich des
					Unterformates helfen, steigen die Chancen betr�chtlich, da� __SFX diese Dateien bald lesen kann.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
