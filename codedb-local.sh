#!/bin/sh
#
# build module codedb

echo "scanning for function definitions ..."
rm *.funcs
for file in *.c; do echo $file; cat $file | perl ../scanfuncs.pl | sort >$file.funcs; done

echo "scanning for function calls ..."
rm *.calls *.calls.tmp
for file in *.c; do echo $file; cat $file | perl ../scancalls.pl | sort >$file.calls.tmp; uniq $file.calls.tmp >$file.calls; done

echo "building module catalogs ..."
rm .funcs .calls .calls.tmp .calls.count
cat *.funcs | sort >.funcs
cat *.calls | sort | uniq >.calls
cat *.calls.tmp | sort >.calls.tmp
uniq .calls.tmp -c >.calls.count
