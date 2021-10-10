divert(-1)
define(`__TITLE',`VOC_S')
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
	__SECTION_HEADER(VOC_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Speichert SoundBlaster-VOC Samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorw�rts)</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kan�le     </td><td>ja (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,ADPCM-8:4,ADPCM-8:3,ADPCM-8:2,A-LAW,�-LAW)</td></tr>
</table>
Dieses Format wurde von der Firma "Creative Labs", dem Hersteller der
SoundBlaster-Karten f�r PCs eingef�rt. Das Format ist f�r das direkte
Abspielen der Samples von dem Datentr�ger ausgelegt und hat in dieser
Richtung mehrere Vorteile. Allerdings ist dieses Format etwas inkonsequent
geplant worden, so das einige Erweiterungen notwendig wurden, die die
Handhabung des Formates sehr erschweren. Die meisten Programme k�nnen
lediglich die Formatversion 1.1 lesen. __SFX kann alle bekannten Versionen
des Formates laden und Speichern.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Type</nobr></td><td align="left">Art der Kompression
<ul>
<li>PCM8 : ungepackt 8bit</li>
<li>PCM16 : ungepackt 16bit</li>
<li>ADPCM8_4 : AdaptiveDelta (2:1) gepackt 8bit</li>
<li>ADPCM8_3 : AdaptiveDelta (3:1) gepackt 8bit</li>
<li>ADPCM8_2 : AdaptiveDelta (4:1) gepackt 8bit</li>
<li>�-Law : �-Law (14:8) gepackt 14bit</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Header</nobr></td><td align="left"></ul>
Dateiformatversion :
<ul>
<li>1.20 : Es wird der Blocktyp 9 f�r den Soundheader verwendet.</li>
<li>1.15 : Es werden die Blocktypen 8 und 1 f�r den Soundheader verwendet.</li>
<li>1.10 : Es wird nur der Blocktyp 1 verwendet.</li>
</ul>
Ich habe die Erfahrung gemacht, da� die meisten Programme die  neueren
VOC-Files nicht lesen. Deshalb habe ich die M�glichkeit offengelassen,
auch die �lteren Formate zu speichern. Am sichersten ist die Version 1.10.
Allerdings sollte mann folgende Einschr�nkungen bedenken:
<ul>
<li>1.15 : nur 8-bit Samples</li>
<li>1.10 : nur Mono und 8-bit Samples</li>
</ul>
</td></tr>
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
keine
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
