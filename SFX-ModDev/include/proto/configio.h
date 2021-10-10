#ifndef PROTO_CONFIGIO_H
#define PROTO_CONFIGIO_H
#include <exec/types.h>
extern struct Library *ConfigIOBase;
#define ConfigIOName "configio.library"
#define ConfigIOVersion 1
#include <clib/configio_protos.h>
#include <pragmas/configio_pragmas.h>
#endif
