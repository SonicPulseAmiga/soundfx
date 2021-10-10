/**/
/* configio.library*/
/* $VER: cfgio-lib.fd 1.10 (07.09.98) © by Stefan Kost 1998-1998*/
/**/
/**/
/* V 1.00*/
/**/
#pragma libcall ConfigIOBase CfgIO_NewConfig 1e 9802
#pragma libcall ConfigIOBase CfgIO_RemConfig 24 801
#pragma libcall ConfigIOBase CfgIO_WriteConfig 2a 9802
#pragma libcall ConfigIOBase CfgIO_ReadConfig 30 9802
/**/
#pragma libcall ConfigIOBase CfgIO_DeleteItem 36 A9803
#pragma libcall ConfigIOBase CfgIO_DeleteSection 3c 9802
#pragma libcall ConfigIOBase CfgIO_DeleteConfig 42 801
#pragma libcall ConfigIOBase CfgIO_GetSubItem 48 09803
/**/
#pragma libcall ConfigIOBase CfgIO_WriteString 4e BA9804
#pragma libcall ConfigIOBase CfgIO_ReadString 54 BA9804
/**/
#pragma libcall ConfigIOBase CfgIO_WriteBool 5a 0A9804
#pragma libcall ConfigIOBase CfgIO_ReadBool 60 0A9804
/**/
#pragma libcall ConfigIOBase CfgIO_WriteSignedInteger 66 0A9804
#pragma libcall ConfigIOBase CfgIO_ReadSignedInteger 6c 0A9804
/**/
#pragma libcall ConfigIOBase CfgIO_WriteUnsignedInteger 72 0A9804
#pragma libcall ConfigIOBase CfgIO_ReadUnsignedInteger 78 0A9804
/**/
#pragma libcall ConfigIOBase CfgIO_WriteFloat 7e BA9804
#pragma libcall ConfigIOBase CfgIO_ReadFloat 84 BA9804
/**/
/* V 1.10*/
/**/
#pragma libcall ConfigIOBase CfgIO_MatchString 8a 9802
/**/
/**/
/**/
