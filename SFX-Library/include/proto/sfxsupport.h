#ifndef PROTO_SFXSUPPORT_H
#define PROTO_SFXSUPPORT_H
#include <exec/types.h>
#define SFXSupportName "sfxsupport.library"
#define SFXSupportVersion 4
#include <clib/sfxsupport_protos.h>
#ifdef AMIGA
extern struct Library *SFXSupportBase;
#include <pragmas/sfxsupport_pragmas.h>
#endif /* AMIGA */
#endif /* PROTO_SFXSUPPORT_H */
