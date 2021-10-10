rm golded:add-ons/c++/syntax/dictionaries/soundfx
rm golded:add-ons/c++/quickinfo/soundfx.words
touch golded:add-ons/c++/syntax/dictionaries/soundfx
touch golded:add-ons/c++/quickinfo/soundfx.words

cd sfx-main
for file in *.c; do echo $file; cat $file | perl ../buildgedlookups.pl; done

cd ../sfx-library/src
for file in *.c; do echo $file; cat $file | perl ../../buildgedlookups.pl; donecd sfx-li	

cd ../..
