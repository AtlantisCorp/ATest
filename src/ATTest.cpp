//
//  ATTest.cpp
//  ATest
//
//  Created by jacques tronconi on 16/06/2019.
//  Copyright © 2019 jacques tronconi. All rights reserved.
//

#include "ATTest.h"

namespace ATest
{
    Test::Test(const std::string& name): m_name(name), m_group(std::make_shared<UnitGroup>())
    {
        
    }
    
    void Test::addUnit(const std::shared_ptr<UnitBase> &unit)
    {
        m_group->addUnit(unit);
    }
    
    bool Test::run()
    {
        return m_group->run();
    }
    
    void Test::throw_error()
    {
        if (m_group->error().code() != ENoError)
            throw m_group->error();
    }
    
    Error Test::error() const
    {
        return m_group->error();
    }
}
