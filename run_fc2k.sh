#!/bin/tcsh

####### building FC2K files #######

# runafluid (make)

# runaway indicator (EU-IM)

if ( ( $?ITM_ENVIRONMENT_LOADED) )  then
    if ( $ITM_ENVIRONMENT_LOADED == yes)  then
        echo "*** Compiler set to ITM ***"
        rmactor runaway_fluid
        rmactor runafluid_efieldEdit
        fc2k -docfile doc/runaway_fluid.txt fc2k/runaway_fluid.xml 
        fc2k -docfile doc/runafluid_efieldEdit.txt fc2k/runafluid_efieldEdit.xml 
    else
        echo "*** Compiler set to IMAS ***"
        rmactor runaway_fluid
        rmactor runafluid_efieldEdit
        fc2k -docfile doc/runaway_fluid.txt fc2k_imas/runaway_fluid.xml 
        fc2k -docfile doc/runafluid_efieldEdit.txt fc2k_imas/runafluid_efieldEdit.xml 
    endif
endif
