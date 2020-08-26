if($ENV{ITM_ENVIRONMENT_LOADED})
	set(SOURCE_FILES
		runafluid.cpp
		codeparams.cpp
		cpo_utils.cpp
		efieldedit.cpp
		distinit.cpp
	)
	add_library(runafluid ${SOURCE_FILES})
elseif($ENV{IMAS_ENVIRONMENT_LOADED})
	set(SOURCE_FILES
		runafluid_imas.cpp
		codeparams.cpp
		ids_utils.cpp
		efieldedit_imas.cpp
		distinit.cpp
	)
	add_library(runafluid_imas ${SOURCE_FILES})
endif()