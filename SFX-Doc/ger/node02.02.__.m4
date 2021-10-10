divert(-1)
define(`__TITLE',`Loader')
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
					<p align="justify">Ein Loader ist ein Modul welches Samples von einem bestimmten Format l&auml;d. __SFX
					bietet ihnen nahezu alle gebr&auml;uchlichen Formate zum Laden an.
					</p>
					<p align="justify">Wenn Sie ein Sample haben, welches nicht gelesen werden kann, gibt es daf&uuml;r zwei Hauptursachen :<br>
					1. ich habe Mist programmiert<br>
					2. ich kenne oder unterst&uuml;tze das Format noch nicht<br>
					Im ersten Falle schicken sie <a href="node00.05.__.html">mir</a> bitte das Sample zu. Im zweiten Falle nat&uuml;rlich auch. Hier sollten sie mir
					allerdings noch so viele Infos wie m&ouml;glich mitsenden. Wenn sie also irgendwo in den Weiten des Netzes eine
					Beschreibung des Formates finden, steigt die Wahrscheinlichkeit, da&szlig; das n&auml;chste __SFX das
					Sample l&auml;d ph&auml;nomenal. Falls das Format mehrere Varianten (Kompression, Bittiefen, etc.) unterst&uuml;tzt,
					z&ouml;gern sie nicht mir einen umfangreichen Satz an Testdaten zuzusenden.
					</p>
					< align="justify">Fast alle Saver haben Einiges gemeinsam, was ich im folgenden beschreiben werde.
					Nach dem Laden generieren die Loader, wenn der Datenträger nicht schreibgeschützt ist einen
					Dateikommentar mit Informationen wie Format, Kanäle und Länge. Ein bereits bestehender Kommentar
					wird jedoch nicht überschrieben.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0"><tr><td><table border=0 cellpadding="0">
					<tr><td>   2.2.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.02.01.html">__NODETITLE(`node02.02.01.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
