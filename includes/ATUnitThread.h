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
     *  UnitThread launches its running thread with std::async and stores the future in this class. The UnitThread
     *  cannot be relaunched untill the current future has finished its task. You should call 'wait()' to ensure
     *  this, or check manually the status of the running thread with 'isRunning()'.
     *
     *  Multiple UnitThreads can run simultaneously but always keep in mind that the same unit test cannot be launched
     *  from two different threads, as Unit and UnitGroup are *not* thread-safe.
     *
     */
    class UnitThread : public UnitBase
    {
        //! @brief The UnitGroup to run.
        std::shared_ptr < UnitGroup > m_unit_group;
        
        //! @brief The Mutex to lock this thread when accessing its data.
        mutable std::mutex m_mutex;
        
        //! @brief The running flag: true if currently running, false otherwise.
        std::atomic < bool > m_is_running;
        
        //! @brief The shared state of the currently running thread.
        std::future < bool > m_running_thread;
        
    public:
        /** @brief Constructs an empty UnitThread. */
        UnitThread();
        
        /** @brief Constructs a UnitThread and sets its group of units. */
        UnitThread(const std::shared_ptr < UnitGroup >& group);
        
        /** @brief Runs the thread. */
        bool run();
        
        /** @brief Tries to run the thread. If the UnitThread is already running, returns false. */
        bool try_run();
        
        /** @brief Runs the thread.
         *
         *  @param waiting_time
         *  If the UnitThread is already running, this value defines the maximum amount of time this unit can wait
         *  before relaunching its thread. If the thread is still running at the end of this period, it returns false
         *  and the thread is not launched.
         *
         *  @return
         *  True on success, false otherwise.
         */
        bool run(const std::chrono::milliseconds& waiting_time);
        
        /** @brief Returns the current error. */
        Error error() const;
        
        /** @brief Returns true if this UnitThread is running. */
        bool isRunning() const;
        
        /** @brief Adds a unit to the UnitGroup of this unit. */
        void addUnit(const std::shared_ptr < UnitBase >& unit);
        
        /** @brief Waits for \ref m_running_thread to finish and returns the value of \ref m_is_running. */
        bool wait();
    };
}

#endif /* ATUnitThread_h */
