cmake_minimum_required(VERSION 3.1)

project(Runafluid VERSION 2.0.0
              LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

include(cmake/AddGatewayFlagFunction.cmake)


#ITM Enviroment 
if($ENV{ITM_ENVIRONMENT_LOADED})
	message("###Compiler set to ITM###")

        find_package(HDF5)

	execute_process(COMMAND tcsh -c "eval-pkg-config --cflags ual-cpp-gnu --cflags hdf5-cpp-gnu --cflags itmconstants --cflags xmllib-$ITM_INTEL_OBJECTCODE"
			OUTPUT_VARIABLE cxx_flags)	
	#execute_process(COMMAND tcsh -c "eval-pkg-config --cflags xmllib-$ITM_INTEL_OBJECTCODE"
	#		OUTPUT_VARIABLE cxx_flags2)
	execute_process(COMMAND tcsh -c "eval-pkg-config --libs ual-cpp-gnu hdf5-cpp-gnu xmllib-$ITM_INTEL_OBJECTCODE"
			OUTPUT_VARIABLE linker_flags)
	add_gateway_flag(INPUT ${cxx_flags} TO CMAKE_CXX_FLAGS)
	add_gateway_flag(INPUT ${linker_flags} TO CMAKE_EXE_LINKER_FLAGS)

	include_directories($ENV{ITM_XMLLIB_DIR}/$ENV{ITM_XMLLIB_VERSION}/$ENV{DATAVERSION}/include/)
	include_directories("include" "ext/Runaphys/include")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DITM_CONSTANTS")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lTreeShr -lTdiShr -lXTreeShr")

	set(SOURCE_FILES src/runafluid.cpp
			 src/codeparams.cpp
			 src/cpo_utils.cpp
			 src/efieldedit.cpp
			 src/distinit.cpp
			 ext/Runaphys/src/critical_field.cpp
			 ext/Runaphys/src/growth_rate.cpp
			 ext/Runaphys/src/avalanche.cpp
			 ext/Runaphys/src/dreicer.cpp
			 ext/Runaphys/src/hdf5export.cpp)

	add_library(Runafluid ${SOURCE_FILES})	
#IMAS environment 
elseif($ENV{IMAS_ENVIRONMENT_LOADED})
	message("###Compiler set to IMAS###")

	execute_process(COMMAND tcsh -c "pkg-config --cflags imas-cpp blitz imas-constants-cpp libxml-2.0 xmllib"
			OUTPUT_VARIABLE shell_script1)
	execute_process(COMMAND tcsh -c "pkg-config --libs imas-cpp blitz libxml-2.0 xmllib"
			OUTPUT_VARIABLE shell_script2)

	add_gateway_flag(INPUT ${shell_script1} TO CMAKE_CXX_FLAGS)
	add_gateway_flag(INPUT ${shell_script2} TO CMAKE_EXE_LINKER_FLAGS)

	include_directories("include" "ext/Runaphys/include")

	set(SOURCE_FILES src/runin_imas.cpp
			 src/codeparams_imas.cpp
			 src/ids_utils.cpp
			 ext/Runaphys/src/critical_field.cpp
			 ext/Runaphys/src/growth_rate.cpp)
	add_library(runin_imas ${SOURCE_FILES})
else()
	message(FATAL_ERROR "###Neither ITM or IMAS loaded###")
endif()

#Use the -DTESTING_ENABLED=yes flag if you want to build the tests
if(TESTING_ENABLED)
	add_subdirectory(test)
endif()