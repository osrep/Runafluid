#ifndef DISTINIT_H_
    #define DISTINIT_H_

    #ifdef ITM_CONSTANTS    
        void distinit(ItmNs::Itm::distribution &distribution, ItmNs::Itm::coreprof &coreprof);        
    #else   
        void distinit(IdsNs::IDS::distribution &distribution, IdsNs::IDS::core_profiles &core_profiles, int timeindex);
    #endif
    
#endif /* DISTINIT_H_ */
