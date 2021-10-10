/****h* SoundFX/ConvertDir.rexx [0.00] *
*
*  NAME
*    ConvertDir.rexx
*  COPYRIGHT
*    $VER: ConvertDir.rexx 0.00 (15.11.99) © by Stefan Kost 1998-1999
*  FUNCTION
*    convert all files in current dir to the currently selected fileformat
*  SYNOPSIS
*    rx ConvertDir <workdir> <newext>
*  EXAMPLE
*    rx ConvertDir.rexx data:swap/conv .wav
*  AUTHOR
*    Stefan Kost			sk
*  CREATION DATE
*    15.Nov.1999
*  MODIFICATION HISTORY
*    15.Nov.1999	V 0.00	most recent version
*    15.Nov.1999	V 1.00	initial version
*  NOTES
*
*******
*/

PARSE ARG workdir newext
OPTIONS RESULTS
ADDRESS 'SFX_REXX'

IF workdir="" || newext="" THEN DO
	Say "Usage:"
	Say "  rx ConvertDir <workdir> <newext>"
	EXIT
END

workdir=Strip(workdir);
newext=Strip(newext);

SFX_SelLoader 'Universal'
/*SFX_SelSaver 'RIFF-WAV'*/					/* we allow any destination format */

ADDRESS COMMAND 'list >t:filelist '||workdir||' LFORMAT="%f%s"'

IF ~Open(direntries,'t:filelist','R') THEN DO

END
ELSE DO
	DO UNTIL Eof(direntries)
		sfilename=ReadLn(direntries)
		ln=Length(sfilename)
		IF ln>0 THEN DO
			dfilename=sfilename||newext
			DO p=ln TO 1 BY -1
				IF Substr(sfilename,p,1)='.' THEN DO
					dfilename=Substr(sfilename,1,(p-1))||newext
					BREAK
				END
			END
	    	Say sfilename||' -> '||dfilename
			SFX_LoadSample sfilename
			buf1=RESULT
			SFX_SaveSample dfilename
			SFX_RemoveBuffer buf1
		END
	END
END
CALL Close(direntries);
EXIT
