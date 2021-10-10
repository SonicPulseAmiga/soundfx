divert(-1)
define(`__TITLE',`CDDA-Direct_L')
define(`__NODE',__file__)
define(`__PATH',`../')
include(`global.m4')
include(`lang_eng.m4')
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
Copies files digitaly (1:1) from CDs. This has the advantage of very high
quality, because it avoids converting the data (digital-&gt;analog and again
analog-&gt;digital).<br>
Instead of a file requester, a track listing will appear, where you can
choose the desired track and set start/end/length.
Read the chapter about <a href="../node01.05.09.html">recording/sampling</a> as well.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Device</nobr></td><td align="left">Name of the device-driver which controls the cd-drive.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Unit</nobr></td><td align="left">Numer of device
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Method</nobr></td><td align="left">The method which should be used to read from the drive.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Memory</nobr></td><td align="left">Which memory should be used for internal read buffer.
<ul>
<li>Any : doesn't matter</li>
<li>Fast : choose only if you have some</li>
<li>24bit : go for this if you experience crashes</li>
</ul>
</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Notes)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
This won't work with all drives. At first not every cd-rom or cd-writer
is capable of DAE (Digital Audio Extraction) and what is worse, there is
no standart way of doing it. To check, if your drive can do it and if yes
how, see the list below.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_HEAD><th>Drive   </th><th>Type          </th><th>Interface</th><th>DAE</th><th>Modes</th></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Plextor </td><td>CD-ROM PX-32TS</td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Plextor </td><td>CD-ROM PX-40TS</td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Ricoh   </td><td>              </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM, Toshiba</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Teac    </td><td>CD-523S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Teac    </td><td>CD-R55S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Teac    </td><td>CD-R58S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Toshiba </td><td>              </td><td>SCSI     </td><td>yes</td><td>Toshiba</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Traxdata</td><td>CDR4120       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
</table>
Please send me reports to complete this list.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
