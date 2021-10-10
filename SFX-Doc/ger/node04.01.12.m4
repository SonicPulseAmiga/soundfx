divert(-1)
define(`__TITLE',`Kann &lt;...&gt; nicht &lt;...&gt; !')
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
					<!-- err???? -->
					<p align="justify">__SFX kann eine Ressource nicht belegen, da vermutlich nicht genügend freier
					Speicherplatz zur Verfügung steht oder die Ressource bereits belegt ist.
					Im ersten Fall beenden Sie bitte andere noch laufende Programme oder
					schließen sie große	Projekte um den notwendigen Speicher zu erhalten.
					Manchmal reicht auch schon die Eingabe folgenden Befehls in der Shell: "avail flush".
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
