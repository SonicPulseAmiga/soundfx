/****h* SoundFX/SFX-Edit.c [4.2] *
*
*  NAME
*    SFX-Edit.c
*  COPYRIGHT
*    $VER: SFX-Edit.c 4.2 (30.10.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    edit functions - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    20.Jun.1998
*  MODIFICATION HISTORY
*    30.Oct.2002	V 4.2	most recent version
*    23.Feb.2000	V 4.00	most recent version
*    03.Dec.1999	V 3.81	most recent version
*							* be quite about no empty clip
*    17.Jan.1999	V 3.80	most recent version
*    31.Oct.1998	V 3.71	most recent version
*							* three new editing functions
*    12.Aug.1998	V 3.70	most recent version
*    20.Jun.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_Edit_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

//-- Helper

void InitCopyBuffer(void);
void FreeCopyBuffer(void);
void CorrectLoop(SInfo *entry,UBYTE op);
void CorrectZoom(SInfo *entry,UBYTE op);

//-- Editfuntions
		
void Edit_Cut(SInfo *entry,UBYTE mode);
void Edit_Copy(SInfo *entry,UBYTE mode);
void Edit_Grab(SInfo *entry,UBYTE mode);
void Edit_Paste(SInfo *entry,UBYTE mode);
void Edit_Erase(SInfo *entry,UBYTE mode);
void Edit_Zero(SInfo *entry,UBYTE mode);
void Edit_Overwrite(SInfo *entry,UBYTE mode);
void Edit_Exchange(SInfo *entry);
void Edit_Replace(SInfo *entry);

//-- defines ------------------------------------------------------------------

#define OP_CUT		0
#define OP_PASTE	1
#define OP_ERASE	2

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

//-- Helper

void InitCopyBuffer(void) {
	clip=NULL;
	temp.id=1;	//-- reserved id
	temp.storage=VM_NODATA;
	temp.memory[0]=temp.memory[1]=temp.memory[2]=temp.memory[3]=NULL;
}

void FreeCopyBuffer(void) {
	if(clip) RemSample(clip);
	InitCopyBuffer();
}

void CorrectLoop(SInfo *entry,UBYTE op) {
	ULONG loope=entry->loops+entry->loopl,markxe=entry->markxs+entry->markxl;
	
	switch(op) {
		case OP_CUT:
		case OP_ERASE:
			if(entry->markxs<=entry->loops && markxe<=entry->loops) {					// ms me ls le
				entry->loops-=entry->markxl;
			}
			if(entry->markxs<=entry->loops && markxe>entry->loops && markxe<=loope)	{	// ms ls me le
				entry->loops=0;entry->loopl-=entry->markxl;
			}
			if(entry->markxs<=entry->loops && markxe>loope) {							// ms ls le me
				entry->loops=0;entry->loopl=0;entry->loop=0;
			}
			if(entry->markxs>entry->loops && entry->markxs<=loope && markxe>loope) {	// ls ms le me
				entry->loopl=entry->markxs-entry->loops;
			}
			// ls le ms me
			if(entry->markxs>entry->loops && entry->markxs<=loope && markxe>entry->loops && markxe<=loope) {	// ls ms me le
				entry->loopl-=entry->markxl;
			}
			break;
		case OP_PASTE:
			if(entry->markxs<=entry->loops) {							// ms ls le
				entry->loops+=clip->slen;
			}
			if(entry->markxs>entry->loops && entry->markxs<=loope) {	// ls ms le
				entry->loopl+=clip->slen;
			}
			// ls le ms
			break;
	}
}

void CorrectZoom(SInfo *entry,UBYTE op) {
	ULONG zoomxe=entry->zoomxs+entry->zoomxl,markxe=entry->markxs+entry->markxl;
	
	switch(op) {
		case OP_CUT:
		case OP_ERASE:
			if(entry->markxs<=entry->zoomxs && markxe<=entry->zoomxs) {						// ms me zs ze
				entry->zoomxs-=entry->markxl;
			}
			if(entry->markxs<=entry->zoomxs && markxe>entry->zoomxs && markxe<=zoomxe) {	// ms zs me ze
				entry->zoomxl-=(entry->markxs+entry->markxl)-entry->zoomxs;
				entry->zoomxs=0;
			}
			if(entry->markxs<=entry->zoomxs && markxe>zoomxe) {								// ms zs ze me
				entry->zoomxs=0;entry->zoomxl=entry->slen;
			}
			if(entry->markxs>entry->zoomxs && entry->markxs<=zoomxe && markxe>zoomxe) {		// zs ms ze me
				entry->zoomxl=entry->markxs-entry->zoomxs;
			}
			// zs ze ms me
			if(entry->markxs>entry->zoomxs && entry->markxs<=zoomxe && markxe>entry->zoomxs && markxe<=zoomxe) {	// zs ms me ze
				entry->zoomxl-=entry->markxl;
			}
			break;
		case OP_PASTE:
			if(entry->markxs<=entry->zoomxs) {							// ms zs ze
				entry->zoomxs+=clip->slen;
			}
			if(entry->markxs>entry->zoomxs && entry->markxs<=zoomxe) {	// zs ms ze
				entry->zoomxl+=clip->slen;
			}
			// zs ze ms
			break;
	}
}

//-- Editfuntions

void Edit_Cut(SInfo *entry,UBYTE mode) {
	ULONG es,ee,el;
	UBYTE okay=TRUE;
	char titlestr[256];

	if(RunTime.play && entry==entry) StopSample();
	switch(mode) {
		case EDIT_2BEGIN:
			es=0;
			ee=entry->markxs;
			el=entry->markxl=entry->markxs;
			entry->markxs=0;
			break;
		case EDIT_2END:
			es=entry->markxs+entry->markxl;
			ee=entry->slen;
			el=entry->markxl=entry->slen-(entry->markxs+entry->markxl);
			break;
		case EDIT_RANGE:
		default:
			es=entry->markxs;
			ee=entry->markxs+entry->markxl;
			el=entry->markxl;
			break;
	}
	FreeCopyBuffer();
	MSG4("Cut from %ld to %ld = %ld of %ld",es,ee,el,entry->slen);
	if((clip=NewSample())) {
		if(AllocSampleMem(clip , el             ,(entry->channels+1))
		&& AllocSampleMem(&temp,(entry->slen-el),(entry->channels+1))) {
			MSG("  memory okay");
			clip->channels=temp.channels=entry->channels;
			//-- do the edit-operation
			if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: cutting ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
			if(es)				okay&=CopyData(entry,&temp,0,0,es);
								okay&=CopyData(entry, clip,es,0,el);
			if(entry->slen-ee)	okay&=CopyData(entry,&temp,ee,es,(entry->slen-ee));
			if(okay) {
				//-- cleanup
				ExchangeData(&temp,entry);CorrectZoom(entry,OP_CUT);
				if(entry->loop) CorrectLoop(entry,OP_CUT);
				entry->slen-=el;
				entry->markxs=0;entry->markxl=0;entry->mark=0;
				//-- recalc lines
				entry->lines=FALSE;RecalcSampleLines(entry);
				SetSlider(entry,1);SetTitle(entry);DrawSample(entry,0);
				Refr_ToolbarRange();
			}
			else {
				FreeSampleMem(&temp);RemSample(clip);clip=NULL;Message(errAllocateMemory,NULL,__FILE__,__LINE__);
				if(SFX_SOUND_IS_SHOWN(entry)) SetWindowTitles(entry->win,entry->title,(UBYTE *)-1);
			}
		}
		else { RemSample(clip);clip=NULL;Message(errAllocateMemory,NULL,__FILE__,__LINE__); }
	}
	else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__); }
}

void Edit_Copy(SInfo *entry,UBYTE mode) {
	ULONG es,el;
	char titlestr[256];

	switch(mode) {
		case EDIT_2BEGIN:
			es=0;
			el=entry->markxs;
			break;
		case EDIT_2END:
			es=entry->markxs+entry->markxl;
			el=entry->slen-(entry->markxs+entry->markxl);
			break;
		case EDIT_RANGE:
		default:
			es=entry->markxs;
			el=entry->markxl;
			break;
	}
	FreeCopyBuffer();
	MSG4("Copy from %ld to %ld = %ld of %ld",es,(entry->markxs+entry->markxl),el,entry->slen);
	if(clip=NewSample()) {
		if(AllocSampleMem(clip,el,(entry->channels+1))) {
			clip->channels=entry->channels;
			//-- do the edit-operation
			if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: copying ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
			if(CopyData(entry,clip,es,0,el)) {
				//-- cleanup
				/** @TODO: option ? */
				//DrawMark(entry);
				//entry->markxs=entry->markxl=0;entry->mark=0;
				Refr_ToolbarRange();
			}
			else { RemSample(clip);clip=NULL;Message(errAllocateMemory,NULL,__FILE__,__LINE__); }
			if(SFX_SOUND_IS_SHOWN(entry)) SetWindowTitles(entry->win,entry->title,(UBYTE *)-1);
		}
		else { RemSample(clip);clip=NULL;Message(errAllocateMemory,NULL,__FILE__,__LINE__); }
	}
	else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__); }
}

void Edit_Grab(SInfo *entry,UBYTE mode) {
	SInfo *si;
	ULONG es,el;
	char titlestr[256];

	switch(mode) {
		case EDIT_2BEGIN:
			es=0;
			el=entry->markxs;
			break;
		case EDIT_2END:
			es=entry->markxs+entry->markxl;
			el=entry->slen-(entry->markxs+entry->markxl);
			break;
		case EDIT_RANGE:
		default:
			es=entry->markxs;
			el=entry->markxl;
			break;
	}
	MSG4("Grab from %ld to %ld = %ld of %ld",es,(entry->markxs+entry->markxl),el,entry->slen);
	if(si=NewSample()) {
		if(AllocSampleMem(si,el,(entry->channels+1))) {
			si->channels=entry->channels;
			si->channelmask=entry->channelmask;
			//-- do the edit-operation
			if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: grabbing ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
			if(CopyData(entry,si,es,0,el)) {
				//-- cleanup
				BufferFill(entry,si);
				si->zoomxl=si->slen=el;
				si->zoomxs=si->markxs=si->markxl=si->loops=si->loopl=0;
				si->mark=si->loop=0;
				/** @TODO: option ? */
				//DrawMark(entry);
				//entry->markxs=entry->markxl=0;entry->mark=0;

				//sprintf(fn,"%s.",entry->ln_Name);
				AddSample(si,((struct Node *)entry)->ln_Name);
				Refr_ToolbarRange();
			}
			else { RemSample(si);Message(errAllocateMemory,NULL,__FILE__,__LINE__); }
			if(SFX_SOUND_IS_SHOWN(entry)) SetWindowTitles(entry->win,entry->title,(UBYTE *)-1);
		}
		else { RemSample(si);Message(errAllocateMemory,NULL,__FILE__,__LINE__); }
	}
	else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__); }
}

void Edit_Paste(SInfo *entry,UBYTE mode) {
	ULONG es;
	UBYTE okay=TRUE;
	char titlestr[256];

	if(clip && clip->slen && entry) {
		if(RunTime.play && entry==entry) StopSample();
		switch(mode) {
			case EDIT_2BEGIN:
				es=entry->markxs=0;
				break;
			case EDIT_RANGE:
			case EDIT_2END:
			default:
				es=entry->markxs+entry->markxl;
				break;
		}

		if(AllocSampleMem(&temp,(entry->slen+clip->slen),(entry->channels+1))) {
			temp.channels=entry->channels;
			//-- do the edit-operation
			if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: pasting ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
			if(es)				okay&=CopyData(entry,&temp,0,0,es);
								okay&=CopyData(clip ,&temp,0,es,clip->slen);
			if(entry->slen-es)	okay&=CopyData(entry,&temp,es,es+clip->slen,(entry->slen-es));
			if(okay) {
				//-- cleanup
				ExchangeData(&temp,entry);CorrectZoom(entry,OP_PASTE);
				if(entry->loop) CorrectLoop(entry,OP_PASTE);
				entry->slen+=clip->slen;
				entry->markxs=0;entry->markxl=0;entry->mark=0;
				//-- recalc lines
				entry->lines=FALSE;RecalcSampleLines(entry);
				SetSlider(entry,1);SetTitle(entry);DrawSample(entry,0);
				Refr_ToolbarRange();
			}
			else {
				FreeSampleMem(&temp);Message(errAllocateMemory,NULL,__FILE__,__LINE__);
				if(SFX_SOUND_IS_SHOWN(entry)) SetWindowTitles(entry->win,entry->title,(UBYTE *)-1);
			}
		}
		else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
	}
	//else Message(errEmptyClip,NULL,__FILE__,__LINE__);		// don't shout, just ignore it
}

void Edit_Erase(SInfo *entry,UBYTE mode) {
	ULONG es,ee,el;
	UBYTE okay=TRUE;
	char titlestr[256];

	if(RunTime.play && entry==entry) StopSample();
	switch(mode) {
		case EDIT_2BEGIN:
			es=0;
			ee=entry->markxs;
			el=entry->markxl=entry->markxs;
			entry->markxs=0;
			break;
		case EDIT_2END:
			es=entry->markxs+entry->markxl;
			ee=entry->slen;
			el=entry->markxl=entry->slen-(entry->markxs+entry->markxl);
			break;
		case EDIT_RANGE:
		default:
			es=entry->markxs;
			ee=entry->markxs+entry->markxl;
			el=entry->markxl;
			break;
	}
	MSG4("Erase from %ld to %ld = %ld of %ld",es,ee,el,entry->slen);
	if(AllocSampleMem(&temp,(entry->slen-el),(entry->channels+1))) {
		temp.channels=entry->channels;
		//-- do the edit-operation
		if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: erasing ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
		if(es)				okay&=CopyData(entry,&temp,0,0,es);
		if(entry->slen-ee)	okay&=CopyData(entry,&temp,ee,es,(entry->slen-ee));
		if(okay) {
			//-- cleanup
			ExchangeData(&temp,entry);CorrectZoom(entry,OP_CUT);
			if(entry->loop) CorrectLoop(entry,OP_CUT);
			entry->slen-=el;
			entry->markxs=0;entry->markxl=0;entry->mark=0;
			//-- recalc lines
			entry->lines=FALSE;RecalcSampleLines(entry);
			SetSlider(entry,1);SetTitle(entry);DrawSample(entry,0);
			Refr_ToolbarRange();
		}
		else {
			FreeSampleMem(&temp);Message(errAllocateMemory,NULL,__FILE__,__LINE__);
			if(SFX_SOUND_IS_SHOWN(entry)) SetWindowTitles(entry->win,entry->title,(UBYTE *)-1);
		}
	}
	else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
}

void Edit_Zero(SInfo *entry,UBYTE mode) {
	ULONG es,ee,el;
	UBYTE okay=TRUE;
	char titlestr[256];

	if(RunTime.play && entry==entry) StopSample();
	switch(mode) {
		case EDIT_2BEGIN:
			es=0;
			ee=entry->markxs;
			el=entry->markxs;
			break;
		case EDIT_2END:
			es=entry->markxs+entry->markxl;
			ee=entry->slen;
			el=entry->slen-(entry->markxs+entry->markxl);
			break;
		case EDIT_RANGE:
		default:
			es=entry->markxs;
			ee=entry->markxs+entry->markxl;
			el=entry->markxl;
			break;
	}
	if(AllocSampleMem(&temp,entry->slen,(entry->channels+1))) {
		temp.channels=entry->channels;
		//-- do the edit-operation
		if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: zeroing ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
		if(es)				okay&=CopyData(entry,&temp,0,0,es);
							okay&=CopyData(NULL ,&temp,es,es,el);
		if(entry->slen-ee)	okay&=CopyData(entry,&temp,ee,ee,(entry->slen-ee));
		if(okay) {
			//-- cleanup
			ExchangeData(&temp,entry);
			/** @TODO: option ? */
			//entry->markxs=0;entry->markxl=0;entry->mark=0;
			//-- recalc lines
			entry->lines=FALSE;RecalcSampleLines(entry);
			DrawSample(entry,0);
		}
		else { FreeSampleMem(&temp);Message(errAllocateMemory,NULL,__FILE__,__LINE__); }
		if(SFX_SOUND_IS_SHOWN(entry)) SetWindowTitles(entry->win,entry->title,(UBYTE *)-1);
	}
	else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
}

void Edit_Overwrite(SInfo *entry,UBYTE mode) {
	ULONG es,ee;
	UBYTE okay=TRUE;
	char titlestr[256];

	if(clip && clip->slen && entry) {
		if(RunTime.play && entry==entry) StopSample();
		switch(mode) {
			case EDIT_2BEGIN:
				es=0;
				break;
			case EDIT_RANGE:
			//case EDIT_2END
			default:
				es=entry->markxs;
				break;
		}

		ee=es+clip->slen;
		if(AllocSampleMem(&temp,entry->slen,(entry->channels+1))) {
			temp.channels=entry->channels;
			//-- do the edit-operation
			if(SFX_SOUND_IS_SHOWN(entry)) { sprintf(titlestr,"%s: overwriting ...",((struct Node *)entry)->ln_Name);SetWindowTitles(entry->win,titlestr,(UBYTE *)-1); }
			if(es)				okay&=CopyData(entry,&temp,0,0,es);
								okay&=CopyData(clip ,&temp,0,es,clip->slen);
			if(entry->slen-es)	okay&=CopyData(entry,&temp,ee,es+clip->slen,(entry->slen-ee));
			if(okay) {
				//-- cleanup
				ExchangeData(&temp,entry);
				entry->markxs=0;entry->markxl=0;entry->mark=0;
				//-- recalc lines
				entry->lines=FALSE;RecalcSampleLines(entry);
				DrawSample(entry,0);
			}
			else { FreeSampleMem(&temp);Message(errAllocateMemory,NULL,__FILE__,__LINE__); }
			if(SFX_SOUND_IS_SHOWN(entry)) SetWindowTitles(entry->win,entry->title,(UBYTE *)-1);
		}
		else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
	}
	//else Message(errEmptyClip,NULL,__FILE__,__LINE__);		// don't shout, just ignore it
}

// exchange marked range with contents of clipboard

void Edit_Exchange(SInfo *entry) {
	/** @TODO implement me */
}

// replace marked range with contents of clipboard

void Edit_Replace(SInfo *entry) {
	/** @TODO implement me */
}

//-- eof ----------------------------------------------------------------------
