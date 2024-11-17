
#pragma once

#include "exception.hpp"
#include "window.hpp"

namespace x::core
{
    class Core
    {
    public:
        void Init();
        void StartLoop();
    private:
        Window m_window;
    };
}
