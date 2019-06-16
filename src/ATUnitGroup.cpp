//
//  ATUnitGroup.cpp
//  ATest
//
//  Created by jacques tronconi on 16/06/2019.
//  Copyright © 2019 jacques tronconi. All rights reserved.
//

#include "ATUnitGroup.h"

namespace ATest
{
    void UnitGroup::addUnit(const std::shared_ptr<UnitBase> &subunit)
    {
        m_subunits[subunit] = Error();
    }
    
    bool UnitGroup::run()
    {
        m_error_happened = false;
        m_errored_subunit = nullptr;
        m_error = Error();
        m_last_unit_runned = 0;
        
        for (auto& subunit : m_subunits)
        {
            if (!subunit.first)
            {
                m_error_happened = true;
                m_error = Error(ENullSubUnit, "UnitGroup holds a null subunit.");
                break;
            }
            
            else if (!subunit.first->run())
            {
                m_error_happened = true;
                m_error = Error(EReturnedError, "A subunit has returned an error.");
                m_errored_subunit = subunit.first;
                break;
            }
            
            m_last_unit_runned++;
        }
        
        return !m_error_happened;
    }
    
    Error UnitGroup::error() const
    {
        return m_error;
    }
}
