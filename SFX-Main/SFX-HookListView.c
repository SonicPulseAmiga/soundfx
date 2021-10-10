/****h* SoundFX/SFX-HookListView.c [3.70] *
*
*  NAME
*    SFX-HookListView.c
*  COPYRIGHT
*    $VER: SFX-HookListView.c 3.70 (12.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    listview-gadget displayhook replacements - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    12.Aug.1998
*  MODIFICATION HISTORY
*    12.Aug.1998	V 3.70	most recent version
*    12.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_HookListView_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

ULONG __asm __saveds LVTableHook(register __a1 struct LVDrawMsg *Msg,register __a2 struct Node *Node);
ULONG __asm __saveds LVColorHook(register __a1 struct LVDrawMsg *Msg,register __a2 struct Node *Node);

//-- private

void GhostRectangle(struct RastPort *RastPort,UWORD Pen,UWORD x0,UWORD y0,UWORD x1,UWORD y1);

//-- globals ------------------------------------------------------------------

struct Hook tablelvhook={	{0,0},LVTableHook,0,0 };
struct Hook colorlvhook={	{0,0},LVColorHook,0,0 };

static UWORD GhostedAreaPattern[2]=
{
     0x4444,
     0x1111,
};

//-- definitions --------------------------------------------------------------

/********************************* Misc Hooks *****************************************
 *                                                                                    *
 * ULONG LVTableHookFunction(REG(a1) struct LVDrawMsg *Msg,REG(a2) struct Node *Node) *
 *                                                                                    *
 * ListViewHook, der eine Tabelle darstellen kann. Dabei wird der String  auf \t ge-  *
 * scannt und die dahinterstehende dezimale _zweistellige_ Zahl als neue Spalte be-   *
 * nutzt.                                                                             *
 *                                                                                    *
 **************************************************************************************/

ULONG __asm __saveds LVTableHook(register __a1 struct LVDrawMsg *Msg,register __a2 struct Node *Node) {
	register ULONG	i;
	UBYTE			State;
	ULONG			APen,BPen,RetCode=LVCB_OK;
	UWORD			*Pens,LinePos,FontWidth,FontBase;
	WORD			x,y,xo,io,sl;
	STRPTR			Name;
	struct RastPort	*RastPort;

	if(Msg->lvdm_MethodID==LV_DRAW) { // Methode==LV_Draw ? Etwas anderes versteht dieser Hook nicht
		RastPort  =Msg->lvdm_RastPort;			// Werte aus Message cachen
		State     =Msg->lvdm_State;
		Pens      =Msg->lvdm_DrawInfo->dri_Pens;

		FontWidth =RastPort->Font->tf_XSize;
		FontBase  =RastPort->Font->tf_Baseline;

		APen      =Pens[FILLTEXTPEN];
		BPen      =Pens[FILLPEN];

		if((State==LVR_NORMAL) || (State==LVR_NORMALDISABLED)) {		// Normaler Text (d.h. keine Markierung darüber) ?
			APen  =Pens[TEXTPEN];
			BPen  =Pens[BACKGROUNDPEN];
		}

		SetAPen(RastPort,BPen);					// Zeile löschen
		RectFill(RastPort,Msg->lvdm_Bounds.MinX,Msg->lvdm_Bounds.MinY,Msg->lvdm_Bounds.MaxX, Msg->lvdm_Bounds.MaxY);

		SetABPenDrMd(RastPort,APen,BPen,JAM1);	// Pens setzen

		Name=Node->ln_Name;						// Text auslesen
								
		x=Msg->lvdm_Bounds.MinX+2;				// Koordinaten berechnen
		y=Msg->lvdm_Bounds.MinY+FontBase;

		xo=0;io=0;
		for(i=0;i<=strlen(Name);i++) {			// aktuellen String nach '\t' durchsuchen
			if((Name[i]=='\t') || !Name[i]) {
				SetAPen(RastPort,APen);
				sl=i-io;
				while(sl && ((x+xo+TextLength(RastPort,&Name[io],sl)) >= (Msg->lvdm_Bounds.MaxX - 2))) sl--;
				if(sl>0) {
					Move(RastPort,x+xo,y);		// Text zeichnen
					Text(RastPort,&Name[io],sl);
				}
				if(Name[i]) {								// es geht noch weiter
					xo=FontWidth*((signed short)(Name[i+1]-'0')*10+(signed short)(Name[i+2]-'0'));	// xOffset für nächsten Text
					io=i+3;																			// StringOffset für nächsten Text

					LinePos=(xo-FontWidth)+(FontWidth>>1);											// TableLine einzeichnen
					if(LinePos+x < Msg->lvdm_Bounds.MaxX - 2) {
						SetAPen(RastPort, Pens[SHADOWPEN]);
						Move(RastPort, LinePos + x, Msg->lvdm_Bounds.MinY);
						Draw(RastPort, LinePos + x, Msg->lvdm_Bounds.MaxY);

						SetAPen(RastPort, Pens[SHINEPEN]);
						Move(RastPort, LinePos + x + 1, Msg->lvdm_Bounds.MinY);
						Draw(RastPort, LinePos + x + 1, Msg->lvdm_Bounds.MaxY);
					}
				}
				i+=2;			// i weiterschalten
			}
		}
		if((State==LVR_NORMALDISABLED) || (State==LVR_SELECTEDDISABLED))		// Evtl. ghosten
			GhostRectangle(RastPort,Pens[BLOCKPEN],Msg->lvdm_Bounds.MinX,Msg->lvdm_Bounds.MinY,Msg->lvdm_Bounds.MaxX,Msg->lvdm_Bounds.MaxY);
	}
	else RetCode = LVCB_UNKNOWN;

	return(RetCode);
}

/********************************* Misc Hooks *****************************************
 *                                                                                    *
 * ULONG LVColorHookFunction(REG(a1) struct LVDrawMsg *Msg,REG(a2) struct Node *Node) *
 *                                                                                    *
 * ListViewHook, der eine Farbliste darstellen kann. Dabei wird das erste Zeichen aus *
 * den String als Farbstiftnummer interpretiert.                                      *
 *                                                                                    *
 **************************************************************************************/

ULONG __asm __saveds LVColorHook(register __a1 struct LVDrawMsg *Msg,register __a2 struct Node *Node)
{
	UBYTE			State;
	ULONG			APen,BPen,RetCode=LVCB_OK;
	UWORD			*Pens,FontWidth,FontBase;
	WORD			x,y,xo,sl,xw;
	STRPTR			Name;
	struct RastPort	*RastPort;

	if(Msg->lvdm_MethodID==LV_DRAW) { // Methode==LV_Draw ? Etwas anderes versteht dieser Hook nicht
		RastPort  =Msg->lvdm_RastPort;			// Werte aus Message cachen
		State     =Msg->lvdm_State;
		Pens      =Msg->lvdm_DrawInfo->dri_Pens;

		FontWidth =RastPort->Font->tf_XSize;
		FontBase  =RastPort->Font->tf_Baseline;

		APen      =Pens[FILLTEXTPEN];
		BPen      =Pens[FILLPEN];

		if((State==LVR_NORMAL) || (State==LVR_NORMALDISABLED)) {		// Normaler Text (d.h. keine Markierung darüber) ?
			APen  =Pens[TEXTPEN];
			BPen  =Pens[BACKGROUNDPEN];
		}

		SetAPen(RastPort,BPen);					// Zeile löschen
		RectFill(RastPort,Msg->lvdm_Bounds.MinX,Msg->lvdm_Bounds.MinY,Msg->lvdm_Bounds.MaxX, Msg->lvdm_Bounds.MaxY);

		SetABPenDrMd(RastPort,APen,BPen,JAM1);		// Pens setzen

		Name=Node->ln_Name;							// Text auslesen
								
		x=Msg->lvdm_Bounds.MinX+2;					// Koordinaten berechnen
		y=Msg->lvdm_Bounds.MinY+FontBase;

		sl=strlen(&Name[4]);						// Text ausgeben
		xw=Msg->lvdm_Bounds.MaxY-Msg->lvdm_Bounds.MinY;
		xo=x+xw+FontWidth;
		while(sl && ((xo+TextLength(RastPort,&Name[1],sl))>=(Msg->lvdm_Bounds.MaxX-2))) sl--;
		if(sl>0) { Move(RastPort,xo,y);Text(RastPort,&Name[4],sl); }

		SetAPen(RastPort,Pens[TEXTPEN]);			// Farbfläche zeichnen
		RectFill(RastPort,x,Msg->lvdm_Bounds.MinY+1,x+xw,Msg->lvdm_Bounds.MaxY-1);
		Name[3]='\0';SetAPen(RastPort,atoi(Name));Name[3]=' ';
		RectFill(RastPort,x+1,Msg->lvdm_Bounds.MinY+2,x+xw-1,Msg->lvdm_Bounds.MaxY-2);

		if((State==LVR_NORMALDISABLED) || (State==LVR_SELECTEDDISABLED))		// Evtl. ghosten
			GhostRectangle(RastPort,Pens[BLOCKPEN],Msg->lvdm_Bounds.MinX,Msg->lvdm_Bounds.MinY,Msg->lvdm_Bounds.MaxX,Msg->lvdm_Bounds.MaxY);
	}
	else RetCode=LVCB_UNKNOWN;

	return(RetCode);
}

//-- private

void GhostRectangle(struct RastPort *RastPort,UWORD Pen,UWORD x0,UWORD y0,UWORD x1,UWORD y1)
{
	SetABPenDrMd(RastPort,Pen,0,JAM1);
	SetAfPt(RastPort,GhostedAreaPattern,1);
	RectFill(RastPort,x0,y0,x1,y1);
	SetAfPt(RastPort,NULL,0);
}

//-- eof ----------------------------------------------------------------------
