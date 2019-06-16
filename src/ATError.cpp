//
//  ATError.cpp
//  ATest
//
//  Created by jacques tronconi on 16/06/2019.
//  Copyright © 2019 jacques tronconi. All rights reserved.
//

#include "ATError.h"

namespace ATest
{
    Error::Error(ErrorCode error_code, const std::string& message): m_error_code(error_code), m_message(message)
    {
        
    }
    
    Error::Error(const Error& rhs): m_error_code(rhs.m_error_code), m_message(rhs.m_message)
    {
        
    }
    
    ErrorCode Error::code() const
    {
        return m_error_code;
    }
    
    const char* Error::what() const noexcept
    {
        return m_message.data();
    }
}
