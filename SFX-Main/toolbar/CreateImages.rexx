/* $VER: CreateImages V0.1 (22.Jan.2000) ? by Stefan Kost 2000 */

strBaseName="data:source/sfx/sfx-main/toolbar/img"

posX=1
posY=1
sizeX=26
sizeY=26
stepX=28
stepY=28
step3Y=stepY+stepY+stepY

sizeX2=18
stepX2=20

/* the images to grab */

curX=posX
curY=posY
nr=0
Brushs.nr.Name=strBaseName||"New"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"Load"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"Save"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"DropDown"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"PlayAll"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"PlayRng"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"Stop"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"Record"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"RunFX"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"ZoomIn"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"ZoomOut"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"ZoomPixel"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"ZoomFull"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
/*
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveBeg"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveLeftF"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveLeftS"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveLeftN"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveRightN"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveRightS"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveRightF"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveEnd"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveTop"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveUpF"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveUpS"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveUpP"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveDownP"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveDownS"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveDownF"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveBottom"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
*/
curX=curX+(17*stepX)
nr=nr+1

Brushs.nr.Name=strBaseName||"Zoom"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"Mark"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"Loop"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"Trace"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"LockOp"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"LockCl"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"RunRexx"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY

curX=posX
curY=posY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"CutMen"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"Cut"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"CutL"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"CutR"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
curY=posY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"CopyMen"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"Copy"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"CopyL"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"CopyR"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
curY=posY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"PasteMen"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"Paste"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"PasteL"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"PasteR"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
curY=posY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"EraseMen"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"Erase"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"EraseL"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"EraseR"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
curY=posY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"ZeroMen"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"Zero"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"ZeroL"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"ZeroR"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
curY=posY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"GrabMen"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"Grab"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"GrabL"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"GrabR"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
curY=posY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"OverwrMen"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"Overwr"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"OverwrL"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curY=curY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"OverwrR"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY

curX=curX+stepX
curY=posY+step3Y
nr=nr+1
Brushs.nr.Name=strBaseName||"Prefs"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"ZoomBoth"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"ZoomX"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"ZoomY"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"Hidden"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"Visible"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"ChgBeg"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1
Brushs.nr.Name=strBaseName||"ChgEnd"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX
Brushs.nr.H=sizeY
curX=curX+stepX
nr=nr+1

Brushs.nr.Name=strBaseName||"MoveBeg"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveLeftF"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveLeftS"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveLeftN"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveRightN"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveRightS"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveRightF"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveEnd"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveTop"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveUpF"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveUpS"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveUpP"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveDownP"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveDownS"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveDownF"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY
curX=curX+stepX2
nr=nr+1
Brushs.nr.Name=strBaseName||"MoveBottom"
Brushs.nr.X=curX
Brushs.nr.Y=curY
Brushs.nr.W=sizeX2
Brushs.nr.H=sizeY

/* and go ... */

ADDRESS 'PPAINT'
SetCurrentBrush '1'

/*
ADDRESS 'rexx_ced'
QUIET
*/

ADDRESS command
delete "img#?.h" FORCE QUIET


do i=0 to nr
	curX=Brushs.i.X
	curY=Brushs.i.Y
	curW=Brushs.i.W
	curH=Brushs.i.H
	strBrushName=Brushs.i.Name
	say COPIES(" ",2-LENGTH(i))||i||"/"nr||" "||strBrushName

	ADDRESS 'PPAINT'
    do j=0 to 2
		DefineBrush curX curY curX+curW curY+curH
		SaveBrush strBrushName||j||".h" 'FORCE QUIET NOPROGRESS FORMAT "C SRC"'
		curY=curY+stepY
	end


	ADDRESS 'rexx_ced'
	do j=0 to 2
		OPEN strBrushName||j||".h"
		LL 6
		MARK
		LL 36
		CUT
		SAVE
		CLOSE
	end
	CLEAR
	INCLUDE FILE strBrushName||0||".h"
	INCLUDE FILE strBrushName||1||".h"
	INCLUDE FILE strBrushName||2||".h"
	SAVE AS strBrushName||".h"

	ADDRESS command
	delete strBrushName||0||".h" strBrushName||1||".h" strBrushName||2||".h" FORCE QUIET
end

ADDRESS 'rexx_ced'
CLEAR

/*-- eof --------------------------------------------------------------------*/
