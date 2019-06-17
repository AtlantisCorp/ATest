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
    /** @brief The base for all our units.
     *
     *  This base is used to store all our units independantly of their return type or their test type. Each unit
     *  is runned using the 'run' function, and on a false returned value, an error can be retrieved with the
     *  'error' function. This simple base is derived into the Units (the class that perform an actual test) and
     *  the UnitGroup that groups multiple UnitBase.
     *
     */
    class UnitBase
    {
    public:
        /** @brief The default destructor. */
        virtual ~UnitBase() = default;
        
        /** @brief Runs the unit.
         *
         *  @return
         *  False if an error occured while running, true otherwise.
         */
        virtual bool run() = 0;
        
        /** @brief Returns the error object if an error occured.
         *
         *  If no error occured, this function returns a default Error instance where Error::code() returns ENoError.
         */
        virtual Error error() const = 0;
    };
    
    /** @brief The base for all unit test.
     *
     *  A unit is a single test performed by a function. This test has a return value, which is compared given the
     *  compareason function to an expected value. If the expected and the returned value mismatches, an error with
     *  the code EResultInvalid is spotted.
     *
     *  @note
     *  Notes that if the function tested actually throws an exception, this exception is kept by the unit and the
     *  exception message is stored in the error object.
     *
     *  Despite the presence of two atomic properties, this class is not thread-safe.
     *
     */
    template < typename Result, template < typename R > class Com = IsEqual >
    class Unit : public UnitBase
    {
        //! @brief The function called by this unit.
        std::function < Result(void) > m_callable;
        
        //! @brief The result expected.
        Result m_normal_result;
        
        //! @brief The result returned by the callable.
        Result m_result;
        
        //! @brief A boolean true if this unit has been runned successfully.
        std::atomic < bool > m_runned;
        
        //! @brief A boolean true if this unit stores an error.
        std::atomic < bool > m_error_happened;
        
        //! @brief The error stored by this unit.
        Error m_error;
        
        //! @brief The compareason function used by this unit. 
        Com<Result> m_comparator;
        
    public:
        /** @brief Constructs a new unit.
         *
         *  The unit is constructed from a function with some arbitrary args and the args the unit should send to this
         *  function. As the unit doesn't use any tuple, the args are stored inside a std::function created from
         *  the std::bind function.
         *
         *  @param normal_result
         *  The result expected for this function with those arguments.
         *
         *  @param func
         *  The function to call when running this unit.
         *
         *  @param args
         *  The arguments to pass to the function when running the unit.
         */
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
    
    /** @brief A partial specialization of Unit when the resulting type is void.
     *
     *  This class only specializes Unit when the resulting function type is void. Thus no compareason happens and
     *  no result is expected for the unit. Instead, it only calls the function inside a try/catch block and catch
     *  all exceptions that may be thrown by the callable.
     *
     */
    template < template < typename R > class Com >
    class Unit < void, Com > : public UnitBase
    {
        //! @brief The callable object for our unit.
        std::function < void(void) > m_callable;
        
        //! @brief Boolean indicating true if this unit has runned.
        std::atomic < bool > m_runned;
        
        //! @brief Boolean indicating true if this unit has an error.
        std::atomic < bool > m_error_happened;
        
        //! @brief The error stored in this unit.
        Error m_error;
        
    public:
        /** @brief Constructs a new unit.
         *
         *  The unit is constructed from a function with some arbitrary args and the args the unit should send to this
         *  function. As the unit doesn't use any tuple, the args are stored inside a std::function created from
         *  the std::bind function.
         *
         *  @param func
         *  The function to call when running this unit.
         *
         *  @param args
         *  The arguments to pass to the function when running the unit.
         */
        template < typename... Args >
        explicit Unit(void(*func)(Args...), Args&&... args)
        {
            m_callable = std::bind(func, std::forward < Args >(args)...);
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
                    m_callable();
                    m_runned = true;
                    
                    m_error_happened = false;
                    m_error = Error(ENoError, "");
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
    
    /** @brief Creates a new Unit with a non-void returning function and an expected result. */
    template < typename Result,
        typename... Args,
        typename = std::enable_if_t<std::is_same<Result, void>::value == false>
    >
    static std::shared_ptr < UnitBase > make_unit(const Result& expected, Result(*callable)(Args...), Args&&... args)
    {
        return std::make_shared < Unit < Result > >(expected, callable, std::forward<Args>(args)...);
    }
    
    /** @brief Creates a new Unit with a non-void returning function, an expected result and a compareason function. */
    template < template < typename R > class Com,
        typename Result,
        typename... Args,
        typename = std::enable_if_t<std::is_same<Result, void>::value == false>
    >
    static std::shared_ptr < UnitBase > make_unit(const Result& expected, Result(*callable)(Args...), Args&&... args)
    {
        return std::make_shared < Unit < Result, Com > >(expected, callable, std::forward<Args>(args)...);
    }
    
    /** @brief Creates a new Unit with a void returning function. */
    template < typename Result,
        typename... Args,
        typename = std::enable_if_t<std::is_same<Result, void>::value>
    >
    static std::shared_ptr < UnitBase > make_unit(Result(*callable)(Args...), Args&&... args)
    {
        return std::make_shared < Unit < Result > >(callable, std::forward<Args>(args)...);
    }
}

#endif /* ATUnit_h */
