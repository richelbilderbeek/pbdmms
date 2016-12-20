# Building

How to create an executable.

## Building on LWP

Either load the `.pro` file from Qt Creator, or use `qmake` to build:

 * `qmake my.pro` or `qmake -qt=qt5 x.pro`: creates a makefile for project `x`
 * `make` or `make debug` or `make release`: build that makefile in debug, debug or release mode respectively
 * `./x`: start the created executable `x`

## Building on Peregrine

Either load the `.pro` file from Qt Creator, or use `qmake` to build:

 * `module load GCC/5.1.0`: load g++ version 5.1.0
 * `module load Boost/1.61.0-foss-2016a`: load Boost 1.61.0
 * `module load Qt5`: load `qmake`
 * `qmake my.pro`: creates a makefile for project `x`
 * `make` or `make debug` or `make release`: build that makefile in debug, debug or release mode respectively
 * `./x`: start the created executable `x`

Problems that will occur:

 * Peregrine does not know `g++-5`
 * Peregrine may give warnings that are escaleted to error

To solve this:

 * Copy your `.pro` file to something like `_peregrine.pro`
 * In that file, replace `g++-5` by just `g++`
 * Remove the `-Werror` flag
