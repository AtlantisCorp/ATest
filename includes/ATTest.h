//
//  ATTest.h
//  ATest
//
//  Created by jacques tronconi on 16/06/2019.
//  Copyright © 2019 jacques tronconi. All rights reserved.
//

#ifndef ATTest_h
#define ATTest_h

#include "ATUnitGroup.h"

namespace ATest
{
    class Test : public UnitBase
    {
        std::string m_name;
        
        std::shared_ptr < UnitGroup > m_group;
        
    public:
        
        Test(const std::string& name);
        
        void addUnit(const std::shared_ptr<UnitBase>& unit);
        
        bool run();
        
        void throw_error();
        
        Error error() const;
    };
}

#endif /* ATTest_h */
