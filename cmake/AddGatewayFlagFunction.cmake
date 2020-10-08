
###########################################################################################
#2020 Daniel Endre Por									  #
###########################################################################################
#When invoking the pkg-config in the main CMakeLists.txt file				  #
#Gateway returns the required flags with a newlind character at the end.		  #
#This function deletes this newline character (as it messes up the build commands) 	  #
#And adds it to the compiler or linker flags.						  #
###########################################################################################
#How to include:									  #
#											  #
#	1. Add this file to the CMakeLists.txt with the include() directive	 	  #
#	2. invoke the function like this: 	 	  				  #
#		add_gateway_flag(INPUT <the flag you want to add> TO <target of the flag>)#
#		The TO argument must be either:						  #
#		CMAKE_CXX_FLAGS - if you want to add it to the compiler flags		  #
#		CMAKE_EXE_LINKER_FLAGS - if you want to add it to the linker flags	  #
###########################################################################################

function(add_gateway_flag)
	cmake_parse_arguments(PARSED_ARGS 
				""
				"INPUT;TO"
				""
				${ARGN})
	string(LENGTH ${PARSED_ARGS_INPUT} str_len)
	math(EXPR str_new_len ${str_len}-1)
	string(SUBSTRING ${PARSED_ARGS_INPUT} 0 ${str_new_len} output1)
	if(${PARSED_ARGS_TO} STREQUAL CMAKE_CXX_FLAGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${output1}" PARENT_SCOPE)
	else(${PARSED_ARGS_TO} STREQUAL CMAKE_EXE_LINKER_FLAGS)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${output1}" PARENT_SCOPE)
	else()
		message(FATAL_ERROR "Invalid input at add_gateway_flag. Invalid input: ${PARSED_ARGS_TO}")
	endif()
endfunction(add_gateway_flag)