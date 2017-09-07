#!/bin/bash
# This script starts unit tests of Runin.

if (  ( $?GTEST) ) then
    make test || exit
    chmod 755 test.bin || exit
    ./test.bin || exit

else
    echo "GTEST environmentally variable is not exist. \n"
    echo "Set GTEST environmentally variable with the absolute path of googletest folder (add lines in .cshrc)"
    echo "# google test init"
    echo 'setenv GTEST $ITMWORK/gtest-1.8.0/googletest'
    echo 'setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH\:$GTEST\n'
    echo "Please download the latest Google test from github\n https://github.com/google/googletest \n"
    echo "Install the Google test"
    echo "1. Go to googletest/make folder"
    echo "2. run make command"
    echo "3. Go to googletest folder"
    echo '4. run cmake $GTEST command'
    echo "5. run make command\n"
endif
