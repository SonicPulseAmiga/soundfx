divert(-1)
define(`__TITLE',`IFF-8SVX_S')
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
	__SECTION_HEADER(IFF-8SVX_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Speichert IFF-8SVX Samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorw?rts)</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kan?le     </td><td>ja (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,FDPCM-8:4,FDPCM-16:6,EDPCM-8:4,EDPCM-16:5)</td></tr>
</table>
Dies ist das am meisten verbreitetste Sound-Dateiformat auf dem Amiga. Es
ist nach dem IFF-Standart aufgebaut und ist so leicht den eigenen W?nschen
anzupassen, ohne das die Kompatibilit?t beeintr?chtigt wird. Das IFF-8SVX
Format geh?rt zu den wenigen Formattypen die Loops mit abspeichern.<br>
__SFX unterst?tzt auch Quadrosamples und 16-bit bzw. combined Samples.
Den Aufbau der Combined-Samples habe ich aus der Dokumentation des Freeware-
programmes <b>SoundBox</b> von Richard K?rber entnommen. Dieses Format
speichert die vollen 16bit-Daten eines Samples. Wenn man dieses Sample in
ein herk?mliches Programm (das nur normale IFF-8SVX-Samples kennt) einl?d,
so wird das Sample automatisch als 8-bit Sample geladen. Ein Programm das
den Aufbau kennt, l?d es als 16bit-Sample.
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
<li>PCM24 : ungepackt 24bit</li>
<li>PCM32 : ungepackt 32bit</li>
<li>PCM16c : ungepackt 16bit kombiniert</li>
<li>FDPCM8_4 : FibonacciDelta (2:1) gepackt 8bit</li>
<li>FDPCM16_6 : FibonacciDelta (8:3) gepackt 16bit</li>
<li>EDPCM8_4 : ExponentialDelta (2:1) gepackt 8bit</li>
<li>EDPCM16_5 : ExponentialDelta (16:5) gepackt 16bit</li>
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
Bei Speicherung als 16-bit-Sample legt __SFX einen "BITS"-Chunk an. Dieser
ist wie folgt aufgebaut :<br>
<code>struct chunk_bits {</code><br>
<code>  char  id[4];  // "BITS"</code><br>
<code>  ULONG len;    // 4L</code><br>
<code>  ULONG bits;   // 8/16 bit so far supported</code><br>
<code>};</code><br>
Au?erdem wurde der "CHAN"-Chunk erweitert. Bei einem Datenwert von 30, handelt
es sich um ein Quadrosample.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
