# HTBP
Hazard Team BSP Profiler

Hazard Team BSP Profiler
==========

Copyright © 2015 by DKY (Ryan Lam)

The __Hazard Team BSP Profiler (HTBP)__ is an experimental tool for Source Engine level designers. It can be used to profile a [Source Engine BSP file (compiled map file)](https://developer.valvesoftware.com/wiki/Source_BSP_File_Format) and find out which portions of the file are taking up the most amount of space. Armed with this knowledge, a level designer will then be able to focus on the portions of the level that need the most space optimization.

It simply loads the BSP header struct from the file and walks through each of the lump header structs, each of which have metadata containing the size of each lump.

Current version: ___0.0.0 DEV___

###Requirements
* A working C compiler and C standard library.
* Some form of `make`.

###To Compile
Just run `make`.

###Usage
Run `htbp.exe [PATH_TO_SOME_BSP_FILE]`.
