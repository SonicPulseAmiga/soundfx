divert(-1)
define(`__TITLE',`SoundFX läuft bereits ! Soll ich es nochmals starten ?')
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
					<!-- reqRunAgain -->
					<p align="justify">Sie haben __SFX ein weiteres Mal gestartet. Wenn Sie Okay anwählen bleibt es
					dabei, falls Sie Cancel anwählen wird das Programm sofort wieder verlassen. Bedenken Sie
					jedoch, daß der ARexx-Port nur in dem zu erst gestartetem __SFX vorhanden ist.
					Dies liegt daran, daß Sie den Portnamen in Ihrem Script angeben müssen, es aber unmöglich ist
					mehrere Ports mit gleichem Namen anzulegen.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
