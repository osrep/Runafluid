cmake_minimum_required(VERSION 3.1)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(Runafluid VERSION 2.0.0
                  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-pthread -g -fPIC -std=c++11")



include(cmake/AddGatewayFlagFunction.cmake)

#ITM Enviroment 
if($ENV{ITM_ENVIRONMENT_LOADED})
	message("###Compiler set to ITM###")

	if(TESTING_ENABLED)
		#gcov flags
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-avx  -fprofile-arcs -ftest-coverage")
		message("-- Coverage flags set")
	endif()
	
	execute_process(COMMAND eval-pkg-config --cflags ual-cpp-gnu --cflags hdf5-cpp-gnu --cflags itmconstants --cflags xmllib-$ENV{ITM_GCC_OBJECTCODE}
			OUTPUT_VARIABLE cxx_flags)
	execute_process(COMMAND eval-pkg-config --libs ual-cpp-gnu hdf5-cpp-gnu xmllib-$ENV{ITM_GCC_OBJECTCODE}
			OUTPUT_VARIABLE linker_flags)
	add_gateway_flag(INPUT ${cxx_flags} TO CMAKE_CXX_FLAGS)
	add_gateway_flag(INPUT ${linker_flags} TO CMAKE_EXE_LINKER_FLAGS)

	include_directories($ENV{ITM_XMLLIB_DIR}/$ENV{ITM_XMLLIB_VERSION}/$ENV{DATAVERSION}/include/)
	include_directories("include" "ext/Runaphys/include")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DITM_CONSTANTS")

	add_subdirectory(src)

#IMAS environment 
elseif($ENV{IMAS_ENVIRONMENT_LOADED})
	message("###Compiler set to IMAS###")

	execute_process(COMMAND tcsh -c "pkg-config --cflags imas-cpp --cflags blitz --cflags imas-constants-cpp --cflags libxml-2.0 xmllib --cflags hdf5-cpp-gnu"
			OUTPUT_VARIABLE cxx_flags)
	execute_process(COMMAND tcsh -c "pkg-config --libs imas-cpp blitz libxml-2.0 xmllib hdf5-cpp-gnu"
			OUTPUT_VARIABLE linker_flags)

	add_gateway_flag(INPUT ${cxx_flags} TO CMAKE_CXX_FLAGS)
	add_gateway_flag(INPUT ${linker_flags} TO CMAKE_EXE_LINKER_FLAGS)

	include_directories("include" "ext/Runaphys/include")

	add_subdirectory(src)
else()
	message(FATAL_ERROR "###Neither ITM or IMAS environment loaded###")
endif()

#Use the -DTESTING_ENABLED=yes flag if you want to build the tests
if(TESTING_ENABLED)
	add_subdirectory(test)
endif()
