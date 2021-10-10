#!/bin/sh
#
# build global codedb

echo "building global catalogs ..."
rm .funcs .calls .calls.count
cat sfx-main/.funcs sfx-library/.funcs | sort >.funcs
cat sfx-main/.calls sfx-library/.calls | sort | uniq >.calls
cat sfx-main/.calls.tmp sfx-library/.calls.tmp | sort | uniq -c >.calls.count
