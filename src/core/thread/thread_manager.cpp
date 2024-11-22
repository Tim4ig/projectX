
#include "thread_manager.hpp"

#include <ranges>
#include <iostream>

namespace x::core
{
    ThreadManager::ThreadObject::ThreadObject(std::function<void()> function, std::atomic_bool& wasException) : m_function(std::move(function)), m_wasException(wasException)
    {
        m_worker = [this]
        {
            try
            {
                m_running = true;
                std::atomic_notify_all(&m_running);
                m_function();
                m_running = false;
            }
            catch (...)
            {
                m_exception = std::current_exception();
                m_wasException = true;
                m_running = false;
                std::atomic_notify_all(&m_running);
            }
        };

        m_thread = std::thread(m_worker);
        std::atomic_wait(&m_running, false);
    }

    ThreadManager::ThreadObject::~ThreadObject()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    void ThreadManager::ThreadObject::Wait()
    {
        if (!m_running || !m_thread.joinable()) return;
        m_thread.join();
        RethrowException();
    }

    void ThreadManager::ThreadObject::RethrowException() const
    {
        if (m_exception)
        {
            std::rethrow_exception(m_exception);
        }
    }

    bool ThreadManager::ThreadObject::IsRunning() const
    {
        return m_running;
    }

    ThreadManager::~ThreadManager()
    {
        WaitAll();
    }

    thread ThreadManager::Start(std::function<void()> function)
    {
        std::lock_guard lock(m_mutex);

        unsigned int id = 1;
        while (m_poolIDs.contains(id)) ++id;

        m_pool.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(id),
            std::forward_as_tuple(std::move(function), std::ref(m_wasException))
        );

        m_poolIDs.insert(id);

        return id;
    }

    void ThreadManager::Wait(thread id)
    {
        if (m_pool.contains(id))
        {
            m_pool.at(id).Wait();
        }
    }

    void ThreadManager::Free(thread id)
    {
        if (m_pool.contains(id))
        {
            m_poolIDs.erase(id);
            m_pool.at(id).Wait();
        }
    }

    bool ThreadManager::IsRunning(thread id) const
    {
        if (m_pool.contains(id))
        {
            return m_pool.at(id).IsRunning();
        }

        return false;
    }

    void ThreadManager::WaitAll()
    {
        for (auto &obj : m_pool | std::views::values)
        {
            obj.Wait();
        }
    }

    void ThreadManager::Update()
    {
        if (m_wasException)
        {
            for (auto& [id, obj] : m_pool)
            {
                try
                {
                    if (!m_throwIDs.contains(id))
                    {
                        obj.RethrowException();
                    }
                }
                catch (...)
                {
                    m_throwIDs.insert(id);
                    std::rethrow_exception(std::current_exception());
                }
            }

            m_wasException = false;
        }
    }
}
