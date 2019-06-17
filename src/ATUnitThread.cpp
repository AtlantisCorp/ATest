//
//  ATUnitThread.cpp
//  atest
//
//  Created by jacques tronconi on 17/06/2019.
//

#include "ATUnitThread.h"

namespace ATest
{
    UnitThread::UnitThread(): m_is_running(false)
    {
        
    }
    
    UnitThread::UnitThread(const std::shared_ptr < UnitGroup >& group): m_unit_group(group), m_is_running(false)
    {
        
    }
    
    bool UnitThread::run()
    {
        return try_run();
    }
    
    bool UnitThread::try_run()
    {
        auto group = std::atomic_load(&m_unit_group);
        
        if (m_is_running || !group)
            return false;
        
        m_is_running = true;
        
        std::lock_guard < std::mutex > lock(m_mutex);
        {
            bool result = group->run();
            m_is_running = false;
            return result;
        }
    }
    
    bool UnitThread::run(const std::chrono::milliseconds &waiting_time)
    {
        if (waiting_time > waiting_time.zero())
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto end = start;
            
            while (m_is_running && (end - start) < waiting_time)
            {
                end = std::chrono::high_resolution_clock::now();
            }
            
            if ((end - start) >= waiting_time)
            {
                return false;
            }
        }
        
        return try_run();
    }
    
    Error UnitThread::error() const
    {
        auto group = std::atomic_load(&m_unit_group);
        if (!group) return Error();
        
        std::lock_guard < std::mutex > lock(m_mutex);
        return group->error();
    }
    
    bool UnitThread::isRunning() const
    {
        return m_is_running;
    }
    
    void UnitThread::addUnit(const std::shared_ptr<UnitBase> &unit)
    {
        auto group = std::atomic_load(&m_unit_group);
        
        if (group)
        {
            std::lock_guard < std::mutex > lock(m_mutex);
            group->addUnit(unit);
        }
    }
}
