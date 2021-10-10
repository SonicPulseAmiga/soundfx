/****h* sfxsupport.library/msg.c [4.2] *
*
*  NAME
*    msg.c
*  COPYRIGHT
*    $VER: msg.c 4.2 (26.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    error & message windows - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    26.Nov.1995
*  MODIFICATION HISTORY
*    26.Apr.2002	V 4.2	most recent version
*							C: works without locale.library too
*    11.Jan.2001	V 4.1	most recent version
*    26.Jan.2000	V 1.30	most recent version
*    18.Sep.1998	V 1.20	most recent version
*    26.Nov.1995	V 1.00	initial version
*  NOTES
*
*******
*/

#define MSG_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Errors && Messages

void		ASM LIB_EXPORT(Message (REG(d0,ULONG messid),REG(a0,STRPTR mess),REG(a1,STRPTR file),REG(d1,ULONG line)));
long		ASM LIB_EXPORT(Question(REG(d0,ULONG messid),REG(a0,STRPTR yes ),REG(a1,STRPTR no)));
void		ASM LIB_EXPORT(LogText (REG(a0,STRPTR msg)));

//-- Locale

STRPTR 		ASM LIB_EXPORT(GetString(REG(d0,ULONG id)));

//-- global variables ---------------------------------------------------------

//-- definitions --------------------------------------------------------------

//-- Errors && Messages

void SAVEDS ASM LIB_EXPORT(Message(REG(d0,ULONG messid),REG(a0,STRPTR mess),REG(a1,STRPTR file),REG(d1,ULONG line))) {
	struct EasyStruct es;
	char mess2[500];
	char gadfmt[100],msgnode[FILENAME_MAX];
	LONG res;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(mess,__FUNC__,"1.par",4));

	es.es_StructSize=sizeof(struct EasyStruct);
	es.es_Flags=0L;
	es.es_Title=LIB_INTCALL(GetString(strSoundFXMessage));
	if(messid!=strNoID) {
		if(!mess) mess=LIB_INTCALL(GetString(messid));			// we have already supplied a message
		if(msgid2hlpid[messid]) {	// this is a standart message (with help node)
			sprintf(gadfmt,"%s|%s",LIB_INTCALL(GetString(strOkay)),LIB_INTCALL(GetString(strHelp)));	// Button Results : 1,0
			es.es_GadgetFormat=gadfmt;
		}
		else {											// this is just any message (with no help node)
			es.es_GadgetFormat=LIB_INTCALL(GetString(strOkay));
		}
	}
	else {
		es.es_GadgetFormat=LIB_INTCALL(GetString(strOkay));
	}
	ERRC(mess==NULL,"empty message");
	if(file) { sprintf(mess2,"%s %d:\n%s",(file?file:""),line,mess);es.es_TextFormat=mess2; }
	else es.es_TextFormat=mess;
	LIB_INTCALL(LogText(es.es_TextFormat));

	if(GetRunTime(quiet)) res=1;
	else {
		if(GetRunTime(winBackDrop)) res=EasyRequestArgs(GetRunTime(winBackDrop),&es,0,0);	// it doesn't matter, if the user presses 'okay' or 'help'
		else res=EasyRequestArgs(0L,&es,0,0);
	}
	if(msgid2hlpid[messid] && (res==0)) {				// this is a standart message (with help node)
		sprintf(msgnode,"html/nodes/node04.01.%02d.html",msgid2hlpid[messid]);
		LIB_INTCALL(ShowHelp(msgnode));
	}
}

long SAVEDS ASM LIB_EXPORT(Question(REG(d0,ULONG messid),REG(a0,STRPTR yes),REG(a1,STRPTR no))) {
	struct EasyStruct es;
	char gadfmt[100],msgnode[FILENAME_MAX];
	LONG res=2;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(yes,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(no,__FUNC__,"2.par",4));
	MSG1("messageid=%3ld",messid);

	es.es_StructSize=sizeof(struct EasyStruct);
	es.es_Flags=0L;
	es.es_Title=LIB_INTCALL(GetString(strSoundFXRequest));
	if((messid!=strNoID) && msgid2hlpid[messid]) {	// this is a standart message (with help node)
		sprintf(gadfmt,"%s|%s|%s",yes,LIB_INTCALL(GetString(strHelp)),no);	// Button Results : 1,2,0
		es.es_GadgetFormat=gadfmt;
	}
	else {
		sprintf(gadfmt,"%s|%s",yes,no);
		es.es_GadgetFormat=gadfmt;
	}
	es.es_TextFormat=LIB_INTCALL(GetString(messid));
	LIB_INTCALL(LogText(es.es_TextFormat));

	if(GetRunTime(quiet)) res=1;
	else {
		if(GetRunTime(winBackDrop)) {
			while(res==2) {
				res=EasyRequestArgs(GetRunTime(winBackDrop),&es,0,0);
				if(msgid2hlpid[messid] && (res==2)) {				// this is a standart message (with help node)
					sprintf(msgnode,"html/nodes/node04.02.%02d.html",msgid2hlpid[messid]);
					LIB_INTCALL(ShowHelp(msgnode));
					//SendAmigaGuideCmd(GetRunTime(aghandle),NULL,AGA_Context,msgid2hlpid[messid],TAG_DONE);
				}
			}
		}
		else {
			while(res==2) {
				res=EasyRequestArgs(0L,&es,0,0);
				if(msgid2hlpid[messid] && (res==2)) {				// this is a standart message (with help node)
					sprintf(msgnode,"html/nodes/node04.02.%02d.html",msgid2hlpid[messid]);
					LIB_INTCALL(ShowHelp(msgnode));
					//SendAmigaGuideCmd(GetRunTime(aghandle),NULL,AGA_Context,msgid2hlpid[messid],TAG_DONE);
				}
			}
		}
	}
	return(res);
}

void SAVEDS ASM LIB_EXPORT(LogText (REG(a0,STRPTR msg))) {
	FILE *log;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(msg,__FUNC__,"1.par",4));

	switch(GetRunTime(sfxprefs_misc)->logmode) {
		case LOGMODE_QUIETMODE:
			if(GetRunTime(quiet)) {
				if(log=fopen(GetRunTime(sfxprefs_misc)->log_path,"ab")) {
					fputs(msg,log);
					fclose(log);
				}
			}
			break;
		case LOGMODE_ALWAYS:
			if(log=fopen(GetRunTime(sfxprefs_misc)->log_path,"ab")) {
				fputs(msg,log);
				fclose(log);
			}
			break;
	}
}

//-- Locale

STRPTR SAVEDS ASM LIB_EXPORT(GetString(REG(d0,ULONG id))) {
	if(LocaleBase) return(GetCatalogStr(sfxcat,id,mess[id]));
	else return(mess[id]);
}

//-- eof ----------------------------------------------------------------------
