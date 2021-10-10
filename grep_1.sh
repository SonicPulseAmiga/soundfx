#!/gg/bin/sh

find . -name "*.c" -exec grep 'Message(' {} \; | sed "s/^.*\(Message(.*__)\);.*$/\1/g" | sort | uniq
