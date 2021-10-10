divert(-1)
define(`__TITLE',`Universal_L')
define(`__NODE',__file__)
define(`__PATH',`../')
include(`global.m4')
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
	__SECTION_HEADER(Funktion)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Der Universalloader versucht die Sampleformate zu identifizieren und l‰d dann
mittels des entsprechenden Lademoduls. Dabei geht er folgendermaﬂen vor:
<ul>
<li>1.) Zuerst versucht er die Samples anhand ihrer Extension (Namenserweiterung) zu erkennen.</li>
<li>2.) Falls dies nicht gelingt, versucht er das Format anhand spezifischer Zeichenketten zu identifizieren.</li>
<li>3.) Insofern auch dies nichts bringt handelt es sich warscheinlich um ein RAW-Sample und es wird als solches geladen.</li>
</ul>
Wenn ein Sample nicht korrekt geladen wurde und sie das Format kennen, versuchen
sie auch mal den entsprechenden Loader direkt aufzurufen.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>keine</nobr></td><td align="left"></td></tr>
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
