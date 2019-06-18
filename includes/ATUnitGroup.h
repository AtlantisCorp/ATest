//
//  ATUnitGroup.h
//  ATest
//
//  Created by jacques tronconi on 16/06/2019.
//  Copyright © 2019 jacques tronconi. All rights reserved.
//

#ifndef ATUnitGroup_h
#define ATUnitGroup_h

#include "ATUnit.h"

namespace ATest
{
    class UnitGroup : public UnitBase
    {
        std::map < std::shared_ptr < UnitBase >, Error > m_subunits;
        
        std::atomic < bool > m_error_happened;
        
        Error m_error;
        
        std::shared_ptr < UnitBase > m_errored_subunit;
        
        std::atomic < size_t > m_last_unit_runned;
        
        std::atomic < bool > m_should_break_on_error;
        
    public:
        
        UnitGroup();
        
        ~UnitGroup() = default;
        
        void addUnit(const std::shared_ptr < UnitBase >& subunit);
        
        bool run();
        
        Error error() const;
    };
}

#endif /* ATUnitGroup_h */
