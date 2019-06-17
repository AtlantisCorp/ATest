//
//  ATUnitThread.h
//  ATest
//
//  Created by jacques tronconi on 17/06/2019.
//

#ifndef ATUnitThread_h
#define ATUnitThread_h

#include "ATUnitGroup.h"

namespace ATest
{
    /** @brief Provides a wrapper around UnitGroup for multithreaded purposes.
     *
     *  UnitThread englobates a mutex around a UnitGroup. Multiple UnitThread may be safely runned from different
     *  threads to runs the different units in multiple threads.
     *  However if one unit is runned simultaneously from different threads, the result is undefined and may (will)
     *  be corrupted.
     *
     */
    class UnitThread : public UnitBase
    {
        std::shared_ptr < UnitGroup > m_unit_group;
        
        mutable std::mutex m_mutex;
        
        std::atomic < bool > m_is_running;
        
    public:
        
        UnitThread();
        
        UnitThread(const std::shared_ptr < UnitGroup >& group);
        
        bool run();
        
        bool try_run();
        
        bool run(const std::chrono::milliseconds& waiting_time);
        
        Error error() const;
        
        bool isRunning() const;
        
        void addUnit(const std::shared_ptr < UnitBase >& unit);
    };
}

#endif /* ATUnitThread_h */
