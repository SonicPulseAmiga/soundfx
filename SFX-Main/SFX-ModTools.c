/****h* SoundFX/SFX-ModTools.c [4.2] *
*
*  NAME
*    SFX-ModTools.c
*  COPYRIGHT
*    $VER: SFX-ModTools.c 4.2 (17.01.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Modulsupportfunktionen - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    17.Jan.2003	V 4.2	most recent version
*							D: amigaguide stuff
*    23.Dec.2000	V 4.1	most recent version
*    17.Apr.2000	V 4.00	most recent version
*    26.Sep.1999	V 3.81	most recent version
*                           C: db-files may now reside in "progdir:Data/"
*                           C: dir-scanner now ignores all files without extension
*    08.Dec.1998	V 3.80	most recent version
*    13.Aug.1998	V 3.70	most recent version
*    13.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_ModTools_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

//-- get installed modules

UWORD GetModuleInfos(char *ln,char *dn,struct List *modlist,UBYTE namecorr);
UWORD ScanDirectory(char *ln,char *dn,struct List *modlist,UBYTE namecorr);
UBYTE CheckDBExistence(STRPTR dbpath,STRPTR ln);
UBYTE CheckDBExpiration(STRPTR dbpath,STRPTR dn);
UBYTE CheckValidModule(char *dn,char *fn);
void ParseModInfo(char *dn,char *fn,ModNode *node);

//-- module index 

void BuildIndexHtml(struct List *list,char *typ);

//-- preset-handling

void SetPreset(STRPTR group,STRPTR mod, STRPTR cfg);

//-- user notification

void NotifyUser(void);

//-- definitions --------------------------------------------------------------

//-- get installed modules

UWORD GetModuleInfos(char *ln,char *dn,struct List *modlist,UBYTE namecorr)
{
	UWORD ret;
	
	ret=ScanDirectory(ln,dn,modlist,namecorr);
	BuildIndexHtml(modlist,ln);
	return(ret);
}

/**
*/
UWORD ScanDirectory(char *ln,char *dn,struct List *modlist,UBYTE namecorr)
{
	register UWORD numberOfModules=0,j;
	struct FileLock *dirlock;
	struct FileInfoBlock *InfoBlock;
	char drive[FILENAME_MAX],path[FILENAME_MAX],name[FILENAME_MAX],ext[FILENAME_MAX],dbpath[FILENAME_MAX];
	ULONG sl;
	UBYTE db_okay;
	ModNode *modul;
	struct Node *node;
	FILE *dbfile;


	sprintf(dbpath,"progdir:Data/%s.db",ln);

	//-- check for database existence (and care about old location(s))
	db_okay=CheckDBExistence(dbpath,ln);

	//-- if db exists, check if the directory has more recently changed than the db
	if(db_okay) db_okay=CheckDBExpiration(dbpath,dn);

	//-- if no or expired database ...
	if(!db_okay) {
		//-- generate database
		if(InfoBlock=(struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock),MEMF_ANY|MEMF_CLEAR)) {
			if(dirlock=(struct FileLock *)Lock(dn,SHARED_LOCK)) {
				if(Examine((BPTR)dirlock,InfoBlock)) {
					if(InfoBlock->fib_DirEntryType>0 && InfoBlock->fib_DirEntryType!=3) {			/* it's a directory but no link */
						NewList(modlist);
						while(ExNext((BPTR)dirlock,InfoBlock)) {
							if(InfoBlock->fib_DirEntryType<0) {									/* it's a file */
								strsfn(InfoBlock->fib_FileName,drive,path,name,ext);
								if(namecorr && (sl=strlen(name))>2 && name[sl-2]=='_') name[sl-2]=0;	/* cut off the '_L' or '_S' */
								if((ext[0])
								&& (strnicmp(ext,"HTML",4))
								&& (strnicmp(ext,"GUIDE",5))
								&& (strnicmp(ext,"MODINFO",7))) {	/* es ist nicht die .guide oder .modinfo Datei*/
									if(CheckValidModule(dn,name)) {							/* gültiges Modul */
										if(!FindName(modlist,name)) {							/* noch nicht in der Liste */
											if(modul=(ModNode *)AllocVec(sizeof(ModNode),MEMF_ANY|MEMF_CLEAR)) {
												modul->node.ln_Name=modul->name;
												strcpy(modul->name,name);
												ParseModInfo(dn,name,modul);
												InsertByNameAsc(modlist,(struct Node*)modul);numberOfModules++;
											}
											else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
										}
									}
								}
							}
						}
					}
				}
				UnLock((BPTR)dirlock);
			}
			FreeVec(InfoBlock);
		}
		else { Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(0); }
		db_okay=TRUE;
		//-- write database
		//-- we already check for zero modules in sfx.c
		if(dbfile=fopen(dbpath,"wb")) {
			sl=numberOfModules;
			if(fwrite(&sl,4,1,dbfile)) {										   // store number of modules
				foreach(node,modlist) {													// store modules
					if(!(fwrite(((ModNode *)node)->name   ,MODINFO_NAMELEN,1,dbfile))) { db_okay=FALSE;break; }  // store name itself
					if(!(fwrite(&(((ModNode *)node)->info),sizeof(ModInfo),1,dbfile))) { db_okay=FALSE;break; }  // store modinfo
				}
			}
			else db_okay=FALSE;
			fclose(dbfile);
			if(!db_okay) DeleteFile(dbpath); /* delete faulty database */
		}
		else StdCError(dbpath,errOpenFile,errno,__FILE__,__LINE__);
	}
	else {
		//-- read database
		NewList(modlist);
		if(dbfile=fopen(dbpath,"rb")) {
			//-- read number of modules
			if(fread(&sl,4,1,dbfile)) {
				numberOfModules=sl;
				for(j=0;(j<numberOfModules && !feof(dbfile) && db_okay);j++) {
					if(modul=(ModNode *)AllocVec(sizeof(ModNode),MEMF_ANY|MEMF_CLEAR)) {
						modul->node.ln_Name=modul->name;
						//-- read name
						if(fread(modul->name,MODINFO_NAMELEN,1,dbfile)) {
							//modul->name[MODINFO_NAMELEN]='\0';
							//-- read modinfo
							if(fread(&(modul->info),sizeof(ModInfo),1,dbfile)) {
								AddTail(modlist,modul);
							}
							else { Message(errReadData,NULL,__FILE__,__LINE__);FreeVec(modul);db_okay=FALSE; }
						}
						else { Message(errReadData,NULL,__FILE__,__LINE__);FreeVec(modul);db_okay=FALSE; }
					}
					else { Message(errAllocateMemory,NULL,__FILE__,__LINE__);db_okay=FALSE; }
				}
			}
			else { Message(errReadData,NULL,__FILE__,__LINE__);db_okay=FALSE; }
			fclose(dbfile);
		}
		else StdCError(dbpath,errOpenFile,errno,__FILE__,__LINE__);
		if(!db_okay) numberOfModules=0;
	}
	return(numberOfModules);
}

UBYTE __inline CheckDBExistence(STRPTR dbpath,STRPTR ln)
{
	FILE *dbfile;
	char name[FILENAME_MAX];
	UBYTE ret=FALSE;

	if(!(dbfile=fopen(dbpath,"rb"))) {
		//-- try old path
		sprintf(name,"progdir:%s.db",ln);
		if(dbfile=fopen(name,"rb")) {
			fclose(dbfile);
			Rename(name,dbpath);
			ret=TRUE;
		}
	}
	else {
		fclose(dbfile);
		ret=TRUE;
	}
	return(ret);
}

UBYTE __inline CheckDBExpiration(STRPTR dbpath,STRPTR dn)
{
	struct DateStamp dsdir,dsdb,dsnow;
	struct FileLock *dirlock,*filelock;
	struct FileInfoBlock *InfoBlock;
	UBYTE ret=TRUE;

	if(InfoBlock=(struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock),MEMF_ANY|MEMF_CLEAR)) {
		DateStamp(&dsnow);
		if(dirlock=(struct FileLock *)Lock(dn,SHARED_LOCK))	{			/* Lock auf directory */
			if(Examine((BPTR)dirlock,InfoBlock)) dsdir=InfoBlock->fib_Date;
			UnLock((BPTR)dirlock);
			if(CompareDates(&dsdir,&dsnow)<=0) {
				ERR1("dir \"%s\" has future filesystem date",dn);
				SetFileDate(dn,&dsnow);
				memcpy(&dsdir,&dsnow,sizeof(struct DateStamp));
			}
		}
		if(filelock=(struct FileLock *)Lock(dbpath,SHARED_LOCK)) {		/* Lock auf database */
			if(Examine((BPTR)filelock,InfoBlock)) dsdb=InfoBlock->fib_Date;
			UnLock((BPTR)filelock);
			if(CompareDates(&dsdb,&dsnow)<=0) {
				ERR1("database \"%s\" has future filesystem date",dbpath);
				SetFileDate(dbpath,&dsnow);
				memcpy(&dsdb,&dsnow,sizeof(struct DateStamp));
				SetFileDate(dn,&dsnow);									/* force recreation of database */
				memcpy(&dsdir,&dsnow,sizeof(struct DateStamp));
			}
		}
		/* test if database is expired (datestamp_dir is newer or equal than datestamp_db) */
		if(CompareDates(&dsdir,&dsdb)<=0) ret=FALSE;
		FreeVec(InfoBlock);
	}
	return(ret);
}

UBYTE CheckValidModule(char *dn,char *fn)
{
	char name[FILENAME_MAX];
//	char buf[8];
//	char *key="SoundFX";
//	UBYTE found=FALSE;
//	UBYTE guide=FALSE;
	UBYTE modinfo=FALSE;
	FILE *infile;

//	sprintf(name,"%s/%s.guide",dn,fn);
//	if(infile=fopen(name,"rb")) { guide=TRUE;fclose(infile); }
//	sprintf(name,"%s/%s.html",dn,fn);
//	if(infile=fopen(name,"rb")) { guide=TRUE;fclose(infile); }

	sprintf(name,"%s/%s.modinfo",dn,fn);
	if(infile=fopen(name,"rb")) { modinfo=TRUE;fclose(infile); }

//	buf[7]=0;
//	sprintf(name,"%s/%s",dn,fn);
//	if(infile=fopen(name,"rt")) {
//		while(!feof(infile) && !found) {
//			memmove(buf,buf+1,6);
//			fread(buf+6,1,1,infile);
//			if(!(stricmp(buf,key))) found=TRUE;
//		}
//		fclose(infile);
//	}
//	return((UBYTE)(found&guide&modinfo));
//	return((UBYTE)(guide&modinfo));
	return(modinfo);
}

#define MI_GUI		1
#define MI_FSEL		2
#define MI_EXT		3
#define MI_MAGIC	4
#define MI_DESC		5

void ParseModInfo(char *dn,char *fn,ModNode *node)
{
	register UBYTE i;
	char name[FILENAME_MAX],line[100],lineold[100],*sptr1,*sptr2;
	FILE *infile;
	ULONG sl;
	UBYTE typ=0,anzext=0,anzmag=0;
	UBYTE p0,p1;

	for(i=0;i<10;i++) node->info.ext[i][0]=0;
	for(i=0;i<5;i++) node->info.magic_str[i][0][0]=node->info.magic_str[i][1][0]=node->info.magic_str[i][2][0]=node->info.magic_str[i][3][0]=node->info.magic_str[i][4][0]=0;

	sprintf(name,"%s/%s.modinfo",dn,fn);
	if(infile=fopen(name,"rb")) {
		while(!feof(infile)) {
			if(fgets(line,100,infile)) {
				line[99]=0;strcpy(lineold,line);
				StripString(line);
				if(!strnicmp(line,"#GUI"  ,4)) typ=MI_GUI;
				if(!strnicmp(line,"#FSEL" ,5)) typ=MI_FSEL;
				if(!strnicmp(line,"#EXT"  ,4)) typ=MI_EXT;
				if(!strnicmp(line,"#MAGIC",6)) typ=MI_MAGIC;
				if(!strnicmp(line,"#DESC" ,5)) typ=MI_DESC;
				if(strnicmp(line,"#",1)) {				/* wenn keine 'typ'-Zeile */
					switch(typ) {
						case MI_GUI:
//							printf("\tGUI   : <%s>\n",line);
							node->info.gui=atoi(line)&1;
							break;
						case MI_FSEL:
//							printf("\tFSEL  : <%s>\n",line);
							node->info.fsel=atoi(line)&1;
							break;
						case MI_EXT:
//							printf("\tEXT   : <%s>\n",line);
							if(anzext<10) {
								strncpy(node->info.ext[anzext],line,9);node->info.ext[anzext][9]=0;
								anzext++;
							}
							break;
						case MI_MAGIC:
//							printf("\tMAGIC : <%s>\n",line);
							if(anzmag<5) {
								sl=strlen(line);
								p0=p1=0;while(line[p1]!=',' && line[p1]) p1++;
								if(p1<sl) {
									line[p1]=0;
									node->info.magic_off[anzmag]=atoi(line);
									line[p1]=',';
									for(i=0;(p1<sl && i<5);i++) {
										p0=(++p1);while(line[p1]!=',' && line[p1]) p1++;
										if(p1<sl) {
											line[p1]=0;
											strncpy(node->info.magic_str[anzmag][i],&line[p0],29);node->info.magic_str[anzmag][i][29]=0;
											line[p1]=',';
										}
									}
									i--;
									strncpy(node->info.magic_str[anzmag][i],&line[p0],29);node->info.magic_str[anzmag][i][29]=0;
									anzmag++;
								}
							}
							break;
						case MI_DESC:
//							printf("\tDESC  : <%s>\n",line);
							sptr1=node->info.description;sptr2=&lineold[1];
							while(*sptr2!='\n') *(sptr1++)=*(sptr2++);
							node->info.description[99]=*sptr1='\0';
							break;
					}				
				}
			}
		}
		fclose(infile);
	}
}

void BuildIndexHtml(struct List *list,char *typ)
{
	char fn[FILENAME_MAX],tfn[FILENAME_MAX],dn[FILENAME_MAX],pn[FILENAME_MAX],line[300];
	FILE *outfile,*infile;
	struct Node *node;
	UBYTE cont=TRUE;
	struct FileLock *lock;
	struct FileInfoBlock *InfoBlock;
	struct DateStamp dsdir,dsdb,dsnow;
	UBYTE state;

	if(!stricmp(typ,"operators"))	{ sprintf(fn,"html/nodes/node02.01.06.html");sprintf(pn,"node02.01.06"); }
	if(!stricmp(typ,"loaders"))		{ sprintf(fn,"html/nodes/node02.02.01.html");sprintf(pn,"node02.02.01"); }
	if(!stricmp(typ,"players"))		{ sprintf(fn,"html/nodes/node02.03.01.html");sprintf(pn,"node02.03.01"); }
	if(!stricmp(typ,"rexx"))		{ sprintf(fn,"html/nodes/node02.04.01.html");sprintf(pn,"node02.04.01"); }
	if(!stricmp(typ,"savers"))		{ sprintf(fn,"html/nodes/node02.05.02.html");sprintf(pn,"node02.05.02"); }
	sprintf(tfn,"%s.tmp",fn);
	sprintf(dn,"_%s",typ);
	if(outfile=fopen(fn,"rb")) {										/* html-index wurde schon erzeugt, testen ob aktuell */
		fclose(outfile);
		if(InfoBlock=(struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock),MEMF_ANY|MEMF_CLEAR)) {
			DateStamp(&dsnow);
			if(lock=(struct FileLock *)Lock(dn,SHARED_LOCK)) {				/* Lock auf directory */
				if(Examine((BPTR)lock,InfoBlock)) dsdir=InfoBlock->fib_Date;
				UnLock((BPTR)lock);
				if(CompareDates(&dsdir,&dsnow)<=0) {
					ERR1("dir \"%s\" has future filesystem date",dn);
					SetFileDate(dn,&dsnow);
					memcpy(&dsdir,&dsnow,sizeof(struct DateStamp));
				}
			}
			if(lock=(struct FileLock *)Lock(fn,SHARED_LOCK)) {				/* Lock auf html-index */
				if(Examine((BPTR)lock,InfoBlock)) dsdb=InfoBlock->fib_Date;
				UnLock((BPTR)lock);
				if(CompareDates(&dsdb,&dsnow)<=0) {
					ERR1("index \"%s\" has future filesystem date",fn);
					SetFileDate(fn,&dsnow);
					memcpy(&dsdb,&dsnow,sizeof(struct DateStamp));
					SetFileDate(dn,&dsnow);									/* force recreation of index */
					memcpy(&dsdir,&dsnow,sizeof(struct DateStamp));
				}
			}
			if(CompareDates(&dsdir,&dsdb)>=0) cont=0;						   /* testen ob html-index veraltet ist */
			FreeVec(InfoBlock);
		}
	}
	if(cont) {							/* html-index neu erzeugen */
		cont=FALSE;
		if(infile=fopen(fn,"rb")) {
			if(outfile=fopen(tfn,"wb")) {
				state=0;
				while(fgets(line,300,infile)) {
					switch(state) {
						case 0:		// seek for <!--LIST_BEG--> and copy all lines from input to output
							if(!strnicmp(line,"<!--LIST_BEG-->",15)) {
								state=1;fputs(line,outfile);
 								foreach(node,list) {
									if(node->ln_Name) {
//										fprintf(outfile,"<tr><td><img src=\"../_img/space.gif\" height=\"1\" width=\"30\"><a href=\"../../_%s/%s.html\">%s</a></td><td>%s</td></tr>\n",typ,node->ln_Name,node->ln_Name,((ModNode *)node)->info.description);
										fprintf(outfile,"<tr><td><img src=\"../_img/space.gif\" height=\"1\" width=\"30\"><a href=\"%s/%s.html\">%s</a></td><td>%s</td></tr>\n",pn,node->ln_Name,node->ln_Name,((ModNode *)node)->info.description);
									}
								}
							}
							else fputs(line,outfile);
							break;
						case 1:		// seek for <!--LIST_END--> and skip lines
							if(!strnicmp(line,"<!--LIST_END-->",15)) {
								state=2;fputs(line,outfile);
							}
							break;
						case 2:		// copy all lines from input to output until eof
							fputs(line,outfile);
							break;
					}
				}
				cont=TRUE;
				fclose(outfile);
			}
			fclose(infile);
		}
		if(cont) {					/* delete old one and rename new one */
			DeleteFile(fn);
			Rename(tfn,fn);
		}
	}
}

//-- preset-handling

void SetPreset(STRPTR group,STRPTR mod,STRPTR cfg)
{
	char ifilename[FILENAME_MAX],ofilename[FILENAME_MAX];
	FILE *ifile,*ofile;
	char buf[1024];
	ULONG anz;

	if(cfg[0]==' ') cfg=&cfg[1];	// default, current
	if     (!strcmp(group,"loaders"))	{ sprintf(ifilename,"progdir:_%s/%s_L/%s.cfg",group,mod,cfg);sprintf(ofilename,"env:sfx/%s/%s_L.cfg",group,mod); }
	else if(!strcmp(group,"savers"))	{ sprintf(ifilename,"progdir:_%s/%s_S/%s.cfg",group,mod,cfg);sprintf(ofilename,"env:sfx/%s/%s_S.cfg",group,mod); }
	else								{ sprintf(ifilename,"progdir:_%s/%s/%s.cfg"  ,group,mod,cfg);sprintf(ofilename,"env:sfx/%s/%s.cfg"  ,group,mod); }
//	MSG5("SetPreset(%s,%s,%s) -> %s,%s",group,mod,cfg,ifilename,ofilename);
	if(ifile=fopen(ifilename,"rb")) {
//		MSG("  ifile ok");
		if(ofile=fopen(ofilename,"wb")) {
//			MSG("  ofile ok");
			while(!feof(ifile)) {
				if(anz=fread(buf,1,1024,ifile)) fwrite(buf,1,anz,ofile);
			}
			fclose(ofile);
		}
		fclose(ifile);
	}
}

//-- user notification

void NotifyUser(void)
{
	if(!RunTime.quiet) {
		if(sfxprefs_misc.sig_s) ScreenToFront(RunTime.sfxpubscr);
		if(sfxprefs_misc.sig_a) DisplayBeep(RunTime.sfxpubscr);
	}
}

//-- eof ----------------------------------------------------------------------
