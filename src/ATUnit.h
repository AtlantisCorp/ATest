//
//  ATUnit.h
//  ATest
//
//  Created by jacques tronconi on 16/06/2019.
//  Copyright © 2019 jacques tronconi. All rights reserved.
//

#ifndef ATUnit_h
#define ATUnit_h

#include "ATError.h"
#include "ATComparator.h"

namespace ATest
{
    class UnitBase
    {
    public:
        
        virtual bool run() = 0;
        
        virtual Error error() const = 0;
    };
    
    template < typename Result, template < typename R > class Com = IsEqual >
    class Unit : public UnitBase
    {
        std::function < Result(void) > m_callable;
        
        Result m_normal_result;
        
        Result m_result;
        
        std::atomic < bool > m_runned;
        
        std::atomic < bool > m_error_happened;
        
        Error m_error;
        
        Com<Result> m_comparator;
        
    public:
        template < typename... Args >
        explicit Unit(const Result& normal_result, Result(*func)(Args...), Args&&... args)
        {
            m_callable = std::bind(func, std::forward < Args >(args)...);
            m_normal_result = normal_result;
            m_runned = false;
            m_error_happened = false;
        }
        
        /** @brief Runs the test and returns true if it happens normally. */
        bool run()
        {
            try
            {
                if (m_callable)
                {
                    m_result = m_callable();
                    m_runned = true;
                    
                    if (!m_comparator.compare(m_result, m_normal_result))
                    {
                        m_error_happened = true;
                        m_error = Error(EResultInvalid, "Result is invalid but function happened well.");
                    }
                    
                    else
                    {
                        m_error_happened = false;
                        m_error = Error(ENoError, "");
                    }
                }
                
                else
                {
                    m_runned = false;
                    m_error_happened = true;
                    m_error = Error(ENoCallable, "No callable for test unit.");
                }
            }
            
            catch(const std::exception& e)
            {
                m_runned = true;
                m_error_happened = true;
                m_error = Error(EReturnedError, e.what());
            }
            
            return !m_error_happened;
        }
        
        /** @brief Returns an error result if the test failed. */
        Error error() const
        {
            return m_error;
        }
    };
    
    template < typename Result, typename... Args >
    static std::shared_ptr < UnitBase > make_unit(const Result& expected, Result(*callable)(Args...), Args&&... args)
    {
        return std::make_shared < Unit < Result > >(expected, callable, std::forward<Args>(args)...);
    }
    
    template < template < typename R > class Com, typename Result, typename... Args >
    static std::shared_ptr < UnitBase > make_unit(const Result& expected, Result(*callable)(Args...), Args&&... args)
    {
        return std::make_shared < Unit < Result, Com > >(expected, callable, std::forward<Args>(args)...);
    }
}

#endif /* ATUnit_h */
