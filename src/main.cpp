
#include "core.hpp"

#include <iostream>

int main()
{
    try
    {
        x::core::Core core;
        core.Init();
        core.StartLoop();
    }
    catch (const x::core::utils::Exception& e)
    {
        std::cout << e.ToString() << '\n';
    }
}
