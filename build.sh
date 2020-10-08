#!/bin/bash

########## compiling code #########

# update submodules before building code
. ./handle_submodules.sh
update=${SUBMODULE_UPDATE:-true}
submodules=${SUBMODULES:-"ext/Runaphys"}
update_submodules "$submodules" $update



# without test files
mkdir build && cd build
cmake .. && make

