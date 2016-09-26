#!/bin/tcsh -fe

########## compiling code #########

# without test files
make

# with test files
# make t

####### building FC2K files #######

# runafluid (make)
fc2k -docfile doc/runafluid.txt -codeversion 1.1.0 fc2k/runafluid.xml 
fc2k -docfile doc/runafluid_efieldEdit.txt -codeversion 1.1.0 test/fc2k/runafluid_efieldEdit.xml 

# runafluid parameter editors (make t)
# fc2k test/fc2k/runafluid_neEdit.xml 
# fc2k test/fc2k/runafluid_teEdit.xml 
