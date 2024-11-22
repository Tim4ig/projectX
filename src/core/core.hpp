
#pragma once

#include <memory>

#include "exception.hpp"
#include "window.hpp"

#include "thread/thread_manager.hpp"

#include "render/d3d11factory.hpp"
#include "render/d3d11renderer.hpp"

namespace x::core
{
    class Core
    {
    public:
        ~Core();

        void Init();
        void StartLoop();
    private:
        std::unique_ptr<Window> m_window;
        ThreadManager* m_threadManager = nullptr;

        std::unique_ptr<render::D3D11Factory> m_factory;
        std::unique_ptr<render::D3D11Renderer> m_renderer;

        bool m_UserLoop();
        bool m_SystemLoop();
    };
}
