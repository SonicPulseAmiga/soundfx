divert(-1)
define(`__TITLE',`CDDA-Direct_L')
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
	__SECTION_HEADER(CDDA-Direct_L)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Kopiert Daten digital (1:1) von CDs. Dies hat den Vorteil einer sehr hohen
Qualität, da die Daten nicht gewandelt werden müßen (digital-&gt;analog und
wieder analog-&gt;digital).<br>
Anstatt eines Filerequesters erscheint ein Tracklisting der CD, in dem Sie
den gewünschten Titel auswählen und Start/Ende/Länge einstelllen können.
Lesen sie auch den Abschnitt über <a href="../node01.05.09.html">Aufnahme/Sampling</a>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Device</nobr></td><td align="left">Name des Gerätetreibers an dem das CD-Laufwerk angeschlossen ist.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Unit</nobr></td><td align="left">Nummer des Gerätes
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Method</nobr></td><td align="left">Methode mit der versucht wird vom Laufwerk zu lesen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Memory</nobr></td><td align="left">In welchem Speichertyp der interne Lesepuffer angelegt weren soll.
<ul>
<li>Any : egal</li>
<li>Fast :	nur anwählen, wenn sie auch welches haben</li>
<li>24bit : falls es zu Abstürzen kommt</li>
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
Dies funktioniert nicht mit allen Laufwerken. Zum Einen sind nicht alle
CD-Roms und CD-Writer in der Lage DAE (Digital Audio Extraction) anzubieten
und was noch viel schlimmer ist, es gibt kein Standardverfahren dafür.
Um herauszufinden ob Ihr Laufwerk so etwas kann und wenn ja wie, schauen
sie bitte in der folgenden Liste nach.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_HEAD><th>Laufwerk</th><th>Typ           </th><th>Anschluß </th><th>DAE</th><th>Modi</th></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Plextor </td><td>CD-ROM PX-32TS</td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Plextor </td><td>CD-ROM PX-40TS</td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Ricoh   </td><td>              </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM, Toshiba</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Teac    </td><td>CD-523S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Teac    </td><td>CD-R55S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Teac    </td><td>CD-R58S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Toshiba </td><td>              </td><td>SCSI     </td><td>yes</td><td>Toshiba</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Traxdata</td><td>CDR4120       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
</table>
Bitte senden Sie mir ihre Erfahrungen zu um die Liste zu kompletieren.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
