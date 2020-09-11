#script that runs gcov coverage measurments
#These commands move the required coverage source to the place where gcov searhes for them


mv ./build/src/CMakeFiles/runafluid.dir/codeparams.cpp.gcno ./src
mv ./build/src/CMakeFiles/runafluid.dir/cpo_utils.cpp.gcno ./src
mv ./build/src/CMakeFiles/runafluid.dir/distinit.cpp.gcno ./src
mv ./build/src/CMakeFiles/runafluid.dir/runafluid.cpp.gcno ./src
mv ./build/src/CMakeFiles/runafluid.dir/codeparams.cpp.gcda ./src
mv ./build/src/CMakeFiles/runafluid.dir/cpo_utils.cpp.gcda ./src
mv ./build/src/CMakeFiles/runafluid.dir/distinit.cpp.gcda ./src
mv ./build/src/CMakeFiles/runafluid.dir/runafluid.cpp.gcda ./src

mv ./build/test/CMakeFiles/RunafluidTests.dir/test_cpo.gcno ./test
mv ./build/test/CMakeFiles/RunafluidTests.dir/test_cpo.gcda ./test

gcov src/codeparams.cpp.cpp 
gcov src/cpo_utils.cpp.cpp
gcov src/distinit.cpp.cpp
gcov src/runafluid.cpp.cpp
gcov test/test_cpo.cpp.cpp

mkdir gcov_reports 
mv *.gcov gcov_reports

