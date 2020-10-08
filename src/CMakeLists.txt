set(SOURCE_FILES
		../ext/Runaphys/src/avalanche.cpp
		../ext/Runaphys/src/control.cpp
		../ext/Runaphys/src/critical_field.cpp
		../ext/Runaphys/src/dreicer.cpp
		../ext/Runaphys/src/growth_rate.cpp
		../ext/Runaphys/src/hdf5export.cpp
		../ext/Runaphys/src/checks.cpp

)


if($ENV{ITM_ENVIRONMENT_LOADED})
	set(SOURCE_FILES
		${SOURCE_FILES}
		distinit.cpp
		runafluid.cpp
		cpo_utils.cpp
		codeparams.cpp

	)
	add_library(runafluid ${SOURCE_FILES})
elseif($ENV{IMAS_ENVIRONMENT_LOADED})
	set(SOURCE_FILES
		${SOURCE_FILES}
		distinit_imas.cpp
		runafluid_imas.cpp
		ids_utils.cpp
		codeparams_imas.cpp
	)
	add_library(runafluid_imas ${SOURCE_FILES})
endif()

