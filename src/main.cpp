
#include "core.hpp"

int main()
{
    try
    {
        x::core::Core core;
        core.Init();
        core.StartLoop();
    }
    catch (std::exception & e)
    {
        logger.Exception("[FATAL]", e.what());
    }
    catch (...)
    {
        logger.Exception("[FATAL]", "(UNKNOWN)");
    }

    return 0;
}
