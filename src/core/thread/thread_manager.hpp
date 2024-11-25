#pragma once

#include <thread>
#include <functional>
#include <future>
#include <set>

namespace x::core
{
    using thread = unsigned int;

    class ThreadManager
    {
    public:
        ThreadManager();
        ~ThreadManager();

        static ThreadManager* GetDefaultManager();

        thread Start(const std::function<void()>&& function);
        void Wait(thread id);
        void Free(thread id);
        bool IsRunning(thread id) const;

        void WaitAll();
        void Update();

    private:
        class ThreadObject
        {
        public:
            ThreadObject(std::function<void()> function, std::atomic_bool& wasException);
            ~ThreadObject();

            void Wait();
            void RethrowException() const;
            bool IsRunning() const;

        private:
            std::thread m_thread;
            std::function<void()> m_worker;
            std::function<void()> m_function;
            std::atomic_bool m_running = false;

            std::exception_ptr m_exception;
            std::atomic_bool& m_wasException;
        };

        std::mutex m_mutex;
        std::atomic_bool m_wasException = false;
        std::set<thread> m_poolIDs;
        std::set<thread> m_throwIDs;
        std::unordered_map<thread, ThreadObject> m_pool;

        static std::unique_ptr<ThreadManager> m_defaultManager;
    };
}
