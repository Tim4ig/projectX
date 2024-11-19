
#include "core.hpp"

int main()
{
    try
    {
        x::core::Core core;
        core.Init();
        core.StartLoop();
        return 0;
    }
    catch (...)
    {
        logger.Error("Unhandled exception occurred. Exiting...");
    }
}
