#!/bin/tcsh -fe

########## compiling code #########

# without test files
mkdir build && cd build
cmake .. && make

