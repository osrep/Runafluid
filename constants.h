#ifndef CONSTANTS_H_
    #define CONSTANTS_H_

    #ifdef ITM_CONSTANTS
        #include <itm_constants.h>
        
    #elif IMAS_CONSTANTS
        #include <imas_constants.h>
        
    #else
        // value of pi
        #define ITM_PI		3.141592653589793238462643383280

        // speed of light [m/s]
        #define ITM_C		2.99792458e8

        // electron volt [eV]
        #define ITM_EV		1.602176565e-19

        // elementary charge [coulomb]
        #define ITM_QE		ITM_EV

        // electron mass [kg]
        #define ITM_ME		9.10938291e-31

        //vacuum permeability
        #define ITM_MU0		4.0e-7 * ITM_PI

        // vacuum permittivity
        #define ITM_EPS0	1.0 / (ITM_MU0 * ITM_C * ITM_C)

        // ERROR CODE
        #define ITM_INVALID_INT	-999999999
        #define ITM_INVALID_FLOAT	-9.0e40

    #endif

	// rate identifiers
	const int N_RATES = 4;
	const int RATEID_DREICER = 0;
	const int RATEID_AVALANCHE = 1;
	const int RATEID_ELECTRIC_FIELD = 2;
	const int RATEID_CRITICAL_FIELD = 3;

    // distsource_identifier for runaways (7)
    const int DISTSOURCE_IDENTIFIER = 7;

#endif /* CONSTANTS_H_ */
