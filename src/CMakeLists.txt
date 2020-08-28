if($ENV{ITM_ENVIRONMENT_LOADED})
	set(SOURCE_FILES
		runafluid.cpp
		codeparams.cpp
		cpo_utils.cpp
		distinit.cpp
		../ext/Runaphys/src/avalanche.cpp
		../ext/Runaphys/src/control.cpp
		../ext/Runaphys/src/critical_field.cpp
		../ext/Runaphys/src/dreicer.cpp
		../ext/Runaphys/src/growth_rate.cpp
		../ext/Runaphys/src/hdf5export.cpp
		../ext/Runaphys/src/checks.cpp

	)
	add_library(runafluid ${SOURCE_FILES})
elseif($ENV{IMAS_ENVIRONMENT_LOADED})
	set(SOURCE_FILES
		runafluid_imas.cpp
		codeparams.cpp
		ids_utils.cpp
		distinit.cpp
		ext/Runaphys/src/avalanche.cpp
		ext/Runaphys/src/checks.cpp
		ext/Runaphys/src/control.cpp
		ext/Runaphys/src/critical_field.cpp
		ext/Runaphys/src/dreicer.cpp
		ext/Runaphys/src/growth_rate.cpp
		ext/Runaphys/src/hdf5export.cpp
	)
	add_library(runafluid_imas ${SOURCE_FILES})
endif()