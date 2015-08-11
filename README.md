# HTBP
Hazard Team BSP Profiler

Hazard Team BSP Profiler
==========

Copyright © 2015 by DKY (Ryan Lam)

The __Hazard Team BSP Profiler (HTBP)__ is an experimental tool for Source Engine level designers. It can be used to profile a [Source Engine BSP file (compiled map file)](https://developer.valvesoftware.com/wiki/Source_BSP_File_Format) and find out which portions of the file are taking up the most amount of space. Armed with this knowledge, a level designer will then be able to focus on the portions of the level that need the most space optimization.

It simply loads the BSP header struct from the file and walks through each of the lump header structs, each of which has metadata containing the size of each lump.

Current version: ___0.0.0 DEV___

###Requirements
* A working C compiler and C standard library.
* Some form of `make` (technically optional, if you know what you're doing).

###Optional Requirements
* UPX 3.91 or equivalent (for executable compression... although to be honest, the executable is already so small that there's pretty much no reason to actually do this)

###To Compile
Just run `make`. If you want a compressed executable, run `make packed`.

###Usage
Run `htbp.exe [PATH_TO_SOME_BSP_FILE]`.
