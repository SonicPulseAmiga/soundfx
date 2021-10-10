development of sfx-modules
===============================================================================

installation
-------------------------------------------------------------------------------
I use the following setup :
  * an assign 'sc:' points to where my SAS-C Development system has been
    installed to
  * somewhere else I've got a directory 'source' and inside a directory 'SFX'
  * when I work with SFX then there is an assign called 'project:' which
	points to 'data:source/SFX'

It is recommended that the situation looks simmiliar on your machine (just
create that assign 'project:'). Then copy the files :
  * copy the contents of 'sc_c' to sc:c
  * copy include to include:, lib to lib: and do on

I use an own debuging system. You find some guides enclosed. It only comes
into effect when building debug-versions.


building the example-operator
-------------------------------------------------------------------------------
Now try to get the supplied operator Invert to compile, by entering that
directory and enter 'sc'.


building different versions
-------------------------------------------------------------------------------
build.debug -- builds the debug-version from scratch
build.release -- builds all cpu/fpu-versions from scratch


developing an own operator
-------------------------------------------------------------------------------
* just clone one operator directory e.g. 'Invert'
* now enter it and rename the files starting with 'Invert'
* edit 'project.h'
  * reset version numbers
  * change name
* now edit the *.c file

the API
------------------------------------------------------------------------------

All operators are shared libraries (which get a new near-data segment for each
opener).
Theres are the routines :

int __asm __UserLibInit(REG(a6) struct Library *SFX_FXBase);		/* Initialisation and Destruction */
void __asm __UserLibCleanup(REG(a6) struct Library *SFX_FXBase);

	The usual library Init and Free routines.

void __asm LIBClientDataInit(REG(a0) RTime *RunTime_);				/* DataInitialisation */

	This enabled the operator to access a shared memory-structure of SoundFX.
	I know this is asking for trouble, but as long as all behave well it
    works fine. This will change in the future.
	This structure contains nearly all data which any part of the programm
	might need. Please just read from there.

UBYTE __asm LIBInterface(void);										/* Modulroutienen */

	This fucntion results in opening a window for editing the parameter.

UBYTE __asm LIBProcess(REG(d0) UBYTE callmd,REG(a0) SInfo *si,REG(a1) char *fn);

	This routine does all the hard work - the fx. To do some 'hardcore'
    optimizations I took out the core-fx into a define called FX_CORE. This is
    the fx algorithm. I use this define several times to save some overhead and
    allow better optimisations.


void __asm LIBHandleARexxCmd(REG(a0) char *params,REG(a1) char *ret);

	Cares about Arexx commands for this fx. SoundFX has an own AREXX-port and
    redirects commands to the modules.

See more comments in the files.
