#!/bin/tcsh -fe

########## compiling code #########

# with test files
make t

# without test files
# make

####### building FC2K files #######

# runafluid (make)
fc2k -docfile doc/runafluid.txt  -codeversion 1.0.1 fc2k/runaway_fluid.xml 

# runafluid parameter editors (make t)
fc2k test/fc2k/runafluid_efieldEdit.xml 
fc2k test/fc2k/runafluid_neEdit.xml 
fc2k test/fc2k/runafluid_teEdit.xml 
