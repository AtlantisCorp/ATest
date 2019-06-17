//
//  ATError.h
//  ATest
//
//  Created by jacques tronconi on 16/06/2019.
//  Copyright © 2019 jacques tronconi. All rights reserved.
//

#ifndef ATError_h
#define ATError_h

#include "ATStdIncludes.h"

namespace ATest
{
    enum ErrorCode
    {
        ENoError = 0,
        EResultInvalid,
        ENoCallable,
        EReturnedError,
        ENullSubUnit
    };
    
    class Error : public std::exception
    {
        ErrorCode m_error_code = ENoError;
        
        std::string m_message;
        
    public:
        
        Error() = default;
        
        Error(ErrorCode error_code, const std::string& message);
        
        Error(const Error& rhs);
        
        ErrorCode code() const;
        
        const char* what() const noexcept;
    };
}

#endif /* ATError_h */
