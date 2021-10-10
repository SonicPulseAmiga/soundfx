#include <stdlib.h>
#include <stdio.h>

#define try
#define throw(ex) goto error_##ex
#define catch(ex) error_##ex:
#define trace(ex) printf(__FILE__":%d:" #ex "\n",__LINE__);
#define finally

void func(int a) {
	try {
		if(!a) throw(null);
		if(a>3) throw(toobig);
		return;
	}
	catch(null) {
		trace(null);
		return;
	}
	catch(toobig) {
		trace(toobig");
		return;
	}
	finally {
	}
}


void main(int argc,char *argv[]) {
	if(argc>0) {
		func(atoi(argv[1]));
	}
}
