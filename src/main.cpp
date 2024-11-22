
#include "core.hpp"

#include "thread/thread_manager.hpp"

int main() try
{
    x::core::ThreadManager threadManager;

    const auto task = threadManager.Start([]()
    {
        while (true)
        {
            std::cout << "Thread 1\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (rand() % 15 == 1)
            {
                std::cout << "Throwing exception\n";
                throw std::exception("test");
            }
        }
    });

    threadManager.WaitAll();

    return 0;
}
catch (...)
{
    logger.Error("Unhandled exception occurred. Exiting...");
    return 1;
}
