enable_testing()

if(DEFINED GOOGLE_TEST_SOURCE)
	message("-- Google-test found")
	add_subdirectory(${GOOGLE_TEST_SOURCE}  ${CMAKE_BINARY_DIR}/gtest)
	include_directories(${GTEST_INCLUDE_DIRS})
else()
	message("-- Google-test not found on the system")
	message("-- Performing manual download of Google-test")
	# Download and unpack googletest at configure time
	configure_file(${PROJECT_SOURCE_DIR}/cmake/GoogleTestDownload.cmake googletest-download/CMakeLists.txt)
	
	execute_process(
	COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
	if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
	endif()
	execute_process(
	COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
	if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
	endif()
	
	# Add googletest directly to our build. This defines
	# the gtest and gtest_main targets.
	add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
			${CMAKE_CURRENT_BINARY_DIR}/googletest-build
			EXCLUDE_FROM_ALL)
endif()

# Set Runin test sources
SET(TEST_SOURCES
    test_cpo.cpp
)

add_executable(RunafluidTests ${TEST_SOURCES})

target_link_libraries(RunafluidTests runafluid gtest_main pthread)
