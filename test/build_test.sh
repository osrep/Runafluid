#!/bin/tcsh

# runafluid parameter editors (make t)

cd ..

make dev
if ( ( $?ITM_ENVIRONMENT_LOADED) )  then
    if ( $ITM_ENVIRONMENT_LOADED == yes)  then
        echo "*** Compiler set to ITM ***"
            fc2k test/fc2k/runafluid_neEdit.xml 
            fc2k test/fc2k/runafluid_teEdit.xml 
            fc2k test/fc2k/runafluid_qe_expdecay.xml
    else
        echo "*** Compiler set to IMAS ***"
            fc2k test/fc2k_imas/runafluid_neEdit.xml 
            fc2k test/fc2k_imas/runafluid_teEdit.xml 
    endif
endif
