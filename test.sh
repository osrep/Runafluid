#!/bin/tcsh -fe

########## compiling code #########

mkdir build && cd build
cmake .. -DTESTING_ENABLED=yes && make
test/RunafluidTests

