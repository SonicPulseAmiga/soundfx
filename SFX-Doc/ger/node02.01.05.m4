divert(-1)
define(`__TITLE',`Presetauswahl')
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
				<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
					<center>
						__IMAGE(`fx_cfgsel')
					</center>
					<br>
					<p align="justify">Am rechten Rand fast aller Operatoren wird eine Gruppe von
					Schaltern eingeblendet, mit deren Hilfe man komfortabel Presets
					verwalten kann. Ein Preset ist ein Set von Parametern, welches
					man zur späteren Wiederverwendung unter einem aussagekräftigen
					Namen abspeichern kann.<br>
					Ein bereits existierendes Preset wird durch Anklicken in der
					Liste aktiviert, dabei werden die gespeicherten Parameter
					sofort geladen. Ein Doppelklick führt zum Start der Berechnung.
					Der Name eines Presets kann über das unter der Liste liegende
					Eingabefeld verändert werden.<br>
					Mit dem Button 'Add' werden die aktuellen Einstellungen als
					Preset abgespeichert.<br>
					Mit dem Button 'Del' wird das aktuell ausgewählte Preset ge-
					löscht.<br>
					Wenn sie ein Preset unter dem Namen 'default.cfg' abspeichern,
					dann werden diese Werte als initiale Einstellungen genommen.
					</p>
					<p align="justify"><b>Wenn sie eigene Presets erstellt haben die auch f&uuml;r andere
					Nutzer n&uuml;tzlich sind, dann senden sie <a href="node00.05.__.html">mir</a> die bitte zu.</b>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
