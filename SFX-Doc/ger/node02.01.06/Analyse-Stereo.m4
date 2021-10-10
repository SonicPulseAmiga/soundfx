divert(-1)
define(`__TITLE',`Analyse-Stereo')
define(`__NODE',__file__)
define(`__PATH',`../')
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
	__SECTION_HEADER(Analyse-Stereo)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Erzeugt eine Darstellung der Raumverteilung eines Samples
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>keine</nobr></td><td align="left">&nbsp;</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Hinweise)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Dies ist auch als Phasen-Diagramm bekannt.<br>
Wenn das Berechnungen fertig sind, wird ein neues Fenster geöffnet und der
Graph gezeichnet.
Ein Signal, dessen beide Kanäle exakt gleich sind, wird als Linie von der Mitte
nach oben (Center) erscheinen. Wenn Sie es sich mit Kopfhörern anhören, werden sie
den Sound in ihrem Kopf warnehmen. Die Phase eines solchen Signales ist absolut synchron.
Ein komplett gegenphasiges Signal (ein Kanal ist die invertierte kopie des Anderen),
wird als Linie nach unten (Wide) erscheinen. Wenn man sich dies mit Kopfhörern anhört,
klingt es als ob der Sound von außen kommt. Ein solches Signal ist nicht mono-kompatibel, d.h.
wenn dies jemand auf seinem Mono-Küchenradio anhört, wird er/sie absolut garnichts hören.
Wenn man ein echtes Stereosignal analysiert, zeigt der Graph mit Nadeln nach Links und Rechts
wie "stereo" das Signal ist. Idealerweise zeigt der Graph eine stachelige Kugel in der
Mitte mit einer Spitze nach oben.
<br>
Wenn sie die erzeugten Graphen als Bilder speichern wollen, dann empfehle ich ihnen dafür einen
image-grabber wie SGrab zu verwenden, welchen sie aus dem Aminet beziehen können.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
