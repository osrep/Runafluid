#ifndef DISTINIT_H_
    #define DISTINIT_H_

    #ifdef ITM_CONSTANTS    
        void distinit(ItmNs::Itm::distribution &distribution, ItmNs::Itm::coreprof &coreprof);        
    #elif IMAS_CONSTANTS    
        void distinit(const IdsNs::IDS::distribution &distribution, const IdsNs::IDS::core_profiles &core_profiles);
    #endif
    
#endif /* DISTINIT_H_ */
