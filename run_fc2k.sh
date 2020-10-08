#!/bin/tcsh

####### building FC2K files #######

# runafluid (make)

# runafluid (EU-IM)

if ( ( $?ITM_ENVIRONMENT_LOADED) )  then
    if ( $ITM_ENVIRONMENT_LOADED == yes)  then
        echo "*** Compiler set to ITM ***"
        rmactor runaway_fluid
        fc2k -docfile doc/runaway_fluid.txt fc2k/runaway_fluid.xml 
    endif
endif

# runafluid (IMAS)

if ( ( $?IMAS_ENVIRONMENT_LOADED) )  then
    if ( $IMAS_ENVIRONMENT_LOADED == yes)  then
        echo "*** Compiler set to IMAS ***"
        rmactor runaway_fluid
        fc2k -docfile doc/runaway_fluid.txt fc2k_imas/runaway_fluid.xml 
    endif
endif
