;guides2text
;
; converts sfx online-help into a printable ascii-text
; (Tab=4)
;
echo "Building list ." NOLINE
echo >>T:guide1.lst "type >t:sfx.guide guides/SFX.guide"
list >>T:guide1.lst guides/SFX-#?.guide	LFORMAT "type >>t:sfx.guide guides/%N"
echo "." NOLINE
echo >>T:guide2.lst "type >>t:sfx.guide guides/Operators.guide"
list >>T:guide2.lst _operators/#?.guide	LFORMAT "type >>t:sfx.guide _operators/%N"
echo "." NOLINE
echo >>T:guide3.lst "type >>t:sfx.guide guides/Rexx.guide"
list >>T:guide3.lst _rexx/#?.guide		LFORMAT "type >>t:sfx.guide _rexx/%N"
echo "." NOLINE
echo >>T:guide4.lst "type >>t:sfx.guide guides/Loader.guide"
list >>T:guide4.lst _loader/#?.guide	LFORMAT "type >>t:sfx.guide _loader/%N"
echo "." NOLINE
echo >>T:guide5.lst "type >>t:sfx.guide guides/Saver.guide"
list >>T:guide5.lst _saver/#?.guide  	LFORMAT "type >>t:sfx.guide _saver/%N"
echo "." NOLINE
echo >>T:guide6.lst "type >>t:sfx.guide guides/Player.guide"
list >>T:guide6.lst _player/#?.guide	LFORMAT "type >>t:sfx.guide _player/%N"
echo "."

echo "Sorting ..."
sort T:guide1.lst T:guide1.slst
sort T:guide2.lst T:guide2.slst
sort T:guide3.lst T:guide3.slst
sort T:guide4.lst T:guide4.slst
sort T:guide5.lst T:guide5.slst
sort T:guide6.lst T:guide6.slst

echo "Concating ..."
execute T:guide1.lst
execute T:guide2.slst
execute T:guide3.slst
execute T:guide4.slst
execute T:guide5.slst
execute T:guide6.slst

echo "Converting ..."
ag2txt fname=T:sfx.guide

delete >NIL: T:guide#?.lst
delete >NIL: T:guide#?.slst
delete >NIL: T:parts.lst
delete >NIL: T:part#?.guide
delete >NIL: T:sfx.guide
echo "Ready !"
