#!/bin/sh

for file in `ls *.m4`; do
	echo $file
	title=`cat $file | perl ../getm4title.pl`
	filenote $file "$title"
done
