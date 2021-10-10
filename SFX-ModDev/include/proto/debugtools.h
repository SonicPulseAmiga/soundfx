#ifndef PROTO_DEBUGTOOLS_H
	#define PROTO_DEBUGTOOLS_H
	#include <exec/types.h>
	extern struct Library *DebugToolsBase;
	#define DebugToolsName "debugtools.library"
	#define DebugToolsVersion 1
	#include <clib/debugtools_protos.h>
	#include <pragmas/debugtools_pragmas.h>
#endif
