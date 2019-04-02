#!/bin/tcsh -f

if (  ( $?GTEST) ) then
    make test || exit
    chmod 755 test.bin || exit
    ./test.bin || exit

else
    echo "GTEST environmental variable does not exist. \n"
    echo "Please download the latest Google test from github\n https://github.com/google/googletest \n"
    echo "Use the latest released version \n"
    echo "Build the Google test"
    echo "Set GTEST environmental variable with the absolute path of googletest folder (may add lines in automatic tcsh login script)"
    echo "# google test init"
    echo 'setenv GTEST /path/to/googletest'
    echo 'setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH\:$GTEST\n'
endif
