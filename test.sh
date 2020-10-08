#!/bin/tcsh 

########## compiling code #########

mkdir build && cd build
cmake .. -DTESTING_ENABLED=yes && make
test/RunafluidTests

#script that runs gcov coverage measurments
#These commands move the required coverage source to the place where gcov searhes for them

if ( ( $?ITM_ENVIRONMENT_LOADED) )  then
   	 if ( $ITM_ENVIRONMENT_LOADED == yes)  then
		cd ..
		
		set RUNAFLUID_DIR=./build/src/CMakeFiles/runafluid.dir
		set RUNAFLUID_TEST_DIR=./build/test/CMakeFiles/RunafluidTests.dir
		
		mv $RUNAFLUID_DIR/codeparams.cpp.gcno ./src
		mv $RUNAFLUID_DIR/cpo_utils.cpp.gcno ./src
		mv $RUNAFLUID_DIR/distinit.cpp.gcno ./src
		mv $RUNAFLUID_DIR/runafluid.cpp.gcno ./src
		mv $RUNAFLUID_DIR/codeparams.cpp.gcda ./src
		mv $RUNAFLUID_DIR/cpo_utils.cpp.gcda ./src
		mv $RUNAFLUID_DIR/distinit.cpp.gcda ./src
		mv $RUNAFLUID_DIR/runafluid.cpp.gcda ./src
		
		mv $RUNAFLUID_TEST_DIR/test_cpo.gcno ./test
		mv $RUNAFLUID_TEST_DIR/test_cpo.gcda ./test
		
		gcov src/codeparams.cpp.cpp 
		gcov src/cpo_utils.cpp.cpp
		gcov src/distinit.cpp.cpp
		gcov src/runafluid.cpp.cpp
		gcov test/test_cpo.cpp.cpp
		
		mkdir gcov_reports 
		mv *.gcov gcov_reports
	endif
endif


