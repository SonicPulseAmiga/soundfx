divert(-1)
define(`__TITLE',`Quantisierung')
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
					<p align="justify">
						Um ein Signal mit dem Rechner bearbeiten zu k�nnen brauchen wir es in digitaler Form.
						Dazu wird das Signal in kurzen Intervallen gemessen. Die Me�werte werden gerundet und aufgezeichnet.
						Bei diesem Vorgang wird das Signal also doppelt quantisiert (Zeit, Amplitude). Die Rate mit der abgetastet
						wird, hei�t <a href="node06.03.00.Samplingrate.html">Samplingrate</a> und die Genauigkeit der Wandlung
						entspricht der <a href="node06.03.00.Bitresolution.html">Bitaufl�sung</a> des Samples.
						F�r beide Werte gilt die Faustregel - je h�her, desto besser das Ergebnis, desto gr��er aber auch
						der Speicherverbrauch.<br>
						Wenn die zeitliche Quantisierung (<a href="node06.03.00.Samplingrate.html">Samplingrate</a>) zu gering ist,
						k�nnen nicht alle zum Signal geh�renden Frequenzbestandteile korrekt aufgenommen werden. Ungl�cklicherweise
						erscheinen diese als Artefakte in anderen Frequenzbereichen (<a href="node06.03.00.Aliasing.html">Aliasing</a>).<br>
						Bei dieser Wandlung entsteht au�erdem ein Fehler - die Differenz zwischen dem tats�chlichen Amplitudenwert
						und dem gewandelten (gerundeten) Wert. Dieser Fehler �u�ert sich im Quantisierungsrauschen. Je h�her die
						<a href="node06.03.00.Bitresolution.html">Bitaufl�sung</a> ist, desto geringer is das Rauschen.
						Wenn Sie ein 16bit-Sample in __SFX laden und dieses mit 8bit und mit 14/16bit abspielen, werden Sie den Unterschied h�ren.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
